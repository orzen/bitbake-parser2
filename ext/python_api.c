#include <glib.h>
#include <strings.h>
#include <stdio.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "bbcompat.h"
#include "files.h"
#include "lexer.h"
#include "log.h"
#include "pyo_ast2.h"
#include "pyo_d.h"
#include "pyo_bb_parse.h"
#include "python_utils.h"
#include "node.h"

#if 0
	__func_start_regexp__
	__inherit_regexp__
	__export_func_regexp__
	__addtask_regexp__
	__addhandler_regexp__
	__infunc__
	__body__
	__residue__
	__classname__
#endif

struct state {
	GList *body;
	GList *infunc;
	gchar *class_name;
	GList *residue;
	PyObject *err; // Module exception
	PyObject *mod_bb_parse;
	struct ast *ast;
	PyObject *obj_d;
	PyObject *cls_statementgroup;
	PyObject *cached_statements;
};

static struct state *S;

static PyObject* get_statements(const gchar *filename,
                           const gchar *abs_filename,
                           const gchar *basename) {
	GNode *parse_result;
	PyObject *statements;
	gint r = -1;

	log_dbg("filename '%s', abs_filename '%s', basename '%s'",
	        filename, abs_filename, basename);

	statements = PyDict_GetItemString(S->cached_statements, abs_filename);
	if (statements) {
		return statements;
	}

	statements = ast_statementgroup_new(S->cls_statementgroup);
	if (!statements) {
		log_err("failed to create new statements");
	}

	parse_result = parse_file(abs_filename);
	if (!parse_result) {
		log_err("invalid parse result");
	}
	node_print_tree(parse_result);

	r = compat_call_bbast(S->ast, parse_result, statements, filename);

	// If the file is a bbclass or inc, cache the statements
	if (g_str_has_suffix(filename, ".bbclass") ||
	    g_str_has_suffix(filename, ".inc")) {
		r = PyDict_SetItemString(S->cached_statements,
		                         abs_filename,
		                         statements);
		if (r == -1) {
			log_err("failed to cache statements for '%s'",
			        abs_filename);
		}
	}

	return statements;
}

static PyObject* api_handle(PyObject *self, PyObject *args) {
	gchar *filename = NULL;
	gchar *basename = NULL;
	gchar *abs_path = NULL;
	gchar **split = NULL;
	PyObject *d = NULL;
	PyObject *oldfile = NULL;
	gint include = -1;
	gint r = -1;

	if (self == NULL || args == NULL) {
		log_warn("Either self or args is NULL");
		Py_RETURN_NONE;
	}

	r = PyArg_ParseTuple(args, "sOi", &filename, &d, &include);
	if (r == 0) {
		log_warn("failed to parse args");
		Py_RETURN_NONE;
	}

	if (filename == NULL) {
		log_warn("empty filename");
		Py_RETURN_NONE;
	}

	pyo_d_init(d);

	basename = g_path_get_basename(filename);
	/*
	 * split is an array, index 0 is the filename and index 1 is the file
	 * extension.
	 */
	split = g_strsplit(basename, ".", -1);
	if (split == NULL) {
		log_warn("failed to determine basename");
		g_free(basename);
		Py_RETURN_NONE;
	}
	g_free(basename);

	if (!g_strcmp0(".bbclass", split[1])) {
		//TODO Is this necessary in BBHandler.py?
		log_warn("extension is .bbclass");
		Py_RETURN_NONE;
	}
	g_strfreev(split);

	if (include) {
		oldfile = pyo_d_get_var(d, "FILE", 0);
		if (oldfile == NULL) {
			log_warn("oldfile is null");
			Py_RETURN_NONE;
		}
	}

	abs_path = bb_parse_resolve_file(S->mod_bb_parse, S->err, filename, d);
	if (!abs_path) {
		log_warn("failed to resolve file");
		Py_RETURN_NONE;
	}

	get_statements(filename, abs_path, basename);

	g_free(abs_path);

	return Py_BuildValue("");
}

// TODO these are dependencies that needs to be implemented
//bb.parse.BBHandler.cached_statements = {}
//bb.parse.BBHandler.inherit
//bb.parse.ConfHandler.include

static PyMethodDef bbcparser_methods[] = {
	{"handle",  api_handle, METH_VARARGS, "Parsing a given file and returning AstNodes for ast.py"},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

void state_free() {
	if (S) {
		if (S->ast) {
			ast_free(S->ast);
		}

		if (S->mod_bb_parse) {
			Py_DECREF(S->mod_bb_parse);
		}

		if (S->cls_statementgroup) {
			Py_DECREF(S->cls_statementgroup);
		}

		if (S->cached_statements) {
			Py_DECREF(S->cached_statements);
		}

		if (S->err) {
			Py_DECREF(S->err);
		}

		Py_DECREF(S->err);
		g_free(S);
	}
}

void module_free() {
	state_free();
}

void state_new() {
	S = g_new0(struct state, 1);
	if (S == NULL) {
		log_warn("failed to allocate state");
		goto error;
	}

	G_DEBUG_HERE();

	S->err = PyErr_NewException("bb_cparser.error", NULL, NULL);
	Py_INCREF(S->err);

	S->ast = ast_new();
	if (S->ast == NULL) {
		log_warn("failed to import bb.parse.ast");
		goto error;
	}

	// Import bb.parse
	S->mod_bb_parse = PyImport_ImportModule("bb.parse");
	if (S->mod_bb_parse == NULL) {
		log_warn("failed to import bb.parse");
		goto error;
	}

	S->cls_statementgroup = ast_import_statementgroup(S->ast->mod);
	if (S->cls_statementgroup == NULL) {
		log_warn("failed to import StatementGroup");
		goto error;
	}

	S->cached_statements = PyDict_New();
	if (S->cached_statements == NULL) {
		log_warn("failed to create statements dict");
		goto error;
	}

	return;
error:
	ast_free(S->ast);
	state_free();
}

static struct PyModuleDef bbcparser_module = {
	PyModuleDef_HEAD_INIT,
	.m_name = "bbcparser",   /* name of module */
	.m_doc = "A C implementation of bitbake's parse.BBHandler", /* module documentation, may be NULL */
	.m_size = -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	.m_methods = bbcparser_methods
};
//	.m_slots = NULL,
//	.m_traverse = NULL,
//	.m_clear = NULL,
//	.m_free = module_free

PyMODINIT_FUNC PyInit_bbcparser() {
	PyObject *m = NULL;

	m = PyModule_Create(&bbcparser_module);
	if (m == NULL) {
		log_warn("failed to create module");
		goto out;
	}

	state_new();

	PyModule_AddObject(m, "error", S->err);

	return m;

out:
	log_warn("bbcparser out");

	//if (m) {
	//	Py_DECREF(m);
	//}

	return NULL;
}
