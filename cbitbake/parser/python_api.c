#include <glib.h>
#include <Python.h>
#include <strings.h>
#include <stdio.h>

#include "files.h"
#include "lexer.h"
#include "pyobj_d.h"
#include "python_utils.h"

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

static PyObject *PARSER_ERROR;

#if 0
	static void foo(){
		PyObject *path = PyImport_ImportModule("os.path");
		//PyObject *path = PyObject_GetAttrString(os, "path");
		//PyObject *fun_exists = PyObject_GetAttrString(path, "exists");
		PyObject *os_res = PyObject_CallMethod(path, "exists", "s", "bogus.txt");
		if (os_res == NULL) {
			printf("failed to call exists\n");
		}
		//os_res = PyObject_CallFunction(fun_exists, "s", "bogus.txt");
		if (os_res == NULL) {
			printf("failed to call exists\n");
		}
		printf("bogus.txt exists? check '%d', val '%d'\n", PyBool_Check(os_res), os_res == Py_True);
		os_res = PyObject_CallMethod(path, "exists", "s", "cbbparser.cpython-37m-x86_64-linux-gnu.so");
		//os_res = PyObject_CallFunction(fun_exists, "s", "cbbparser.cpython-37m-x86_64-linux-gnu.so");
		printf("cbbparser.cpython-37m-x86_64-linux-gnu.so exists? '%d'\n", os_res == Py_True);
	}
#endif


static gint import_modules(GHashTable *modules, gint num, ...) {
	va_list ap;
	PyObject *module = NULL;
	gchar *module_name = NULL;
	gint r = 0;
	gint i = 0;

	va_start(ap, 0);
	for (i = 0; i < num; i++) {
		module_name = va_arg(ap, char*);

		module = PyImport_ImportModule(module_name);
		if (module == NULL) {
			g_warning("failed to import module '%s'", module_name);
			r = -1;
		}

		if (!g_hash_table_insert(modules,
					 g_strdup(module_name),
					 module)) {
			g_warning("failed to store module '%s' in hashtable", module_name);
			r = -1;
		}
	}
	va_end(ap);

	return r;
}

gint handle_resolve_kw_callback(enum kw_type kw_t, void **callback) {
	gint force = 0;
}

gint handle_type_kw(GHashTable *entry) {
	gint *keyword = NULL;

	if (entry == NULL) {
		g_warning("empty arguments");
		return -1;
	}

	keyword = g_hash_table_lookup(entry, "keyword");
	if (keyword == NULL) {
		g_warning("failed to resolve keyword");
		return -1;
	}

	convert_kw_to_match(*keyword, entry);


	ast_handle_

	return 0;
}

static void get_statements(const gchar *filename,
			   const gchar *abs_filename,
			   const gchar *basename) {
	GNode *parse_result;
	gint r = -1;

	//TODO add caching

	parse_result = cbb_parse_file(abs_filename);
	if (parse_result == NULL) {
		log_error("invalid parse result");
	}

	//TODO move init and free
	ast_cb_init(ast);

	r = convert_ast_to_python(parse_result, NULL, filename);

	//TODO continue here

	ast_cb_free();
}

static PyObject* api_handle(PyObject *self, PyObject *args) {
	GHashTable *modules;
	gchar *filename = NULL;
	gchar *basename = NULL;
	gchar *abs_path = NULL;
	gchar **split = NULL;
	PyObject *ast = NULL;
	PyObject *d = NULL;
	PyObject *oldfile = NULL;
	gint include = -1;
	gint r = -1;

	if (self == NULL || args == NULL) {
		g_warning("Either self or args is NULL");
		return NULL;
	}

		// TODO is it necessary to have global variables? if used by upwards by
		// ast.py then maybe, otherwise use a GHashtable downwards

	r = PyArg_ParseTuple(args, "OsOi", &ast, &filename, &d, &include);
	if (r == 0) {
		g_warning("failed to parse args");
		return NULL;
	}

	if (!PyDict_Check(user_data)) {
		g_warning("user_data is not a dict");
		return NULL;
	}

	if (filename == NULL) {
		g_warning("empty filename");
		return NULL;
	}

	modules = g_hash_table_new_full(g_str_hash,
					g_str_equal,
					g_free,
					(GDestroyNotify) Py_DecRef);

	r = import_modules(modules, 1, "bb.parse");

	cbb_init(d);

	basename = g_path_get_basename(filename);
	/*
	 * split is an array, index 0 is the filename and index 1 is the file
	 * extension.
	 */
	split = cbb_split_extension(basename);
	if (split == NULL) {
		g_free(basename);
		return NULL;
	}
	g_free(basename);

	if (!g_strcmp0(".bbclass", split[1])) {
		//TODO Is this necessary in BBHandler.py?
		g_debug("extension is .bbclass");
	}

	if (include) {
		oldfile = cbb_d_get_var(d, "FILE", 0);
		if (oldfile == NULL) {
			g_warning("oldfile is null");
			return NULL;
		}
	}

	PyObject *mod_bb_parse = g_hash_table_lookup(modules, "bb.parse");
	PyObject *resolve_file = PyObject_GetAttrString(mod_bb_parse, "resolve_file");

	if (resolve_file == NULL) {
		g_warning("resolve_file is empty");
		PyErr_SetString(PARSER_ERROR, "No function named 'resolve_file' for module 'bb.parse'");
		return NULL;
	}
	if (PyCallable_Check(resolve_file) == 0) {
		g_warning("resolve_file is not callable");
		PyErr_SetString(PARSER_ERROR, "Function is not callable");
		return NULL;
	}

	PyObject *abs_fn = PyObject_CallFunction(resolve_file, "sO", filename, d);
	if (abs_fn == NULL || PyErr_Occurred()) {
		g_warning("failed to resolve file '%s'", filename);
		PyErr_SetString(PARSER_ERROR, "Failed to resolve file");
		return NULL;
	}

	abs_path = PyBytes_AsString(abs_fn);
	get_statements(filename, abs_path, basename);



	//TODO this is and intensional error
	return oldfile;
}

static PyMethodDef cbb_parser_methods[] = {
	{"handle",  api_handle, METH_VARARGS, "Parsing a given file and returning AstNodes for ast.py"},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef cbb_parser_module = {
	PyModuleDef_HEAD_INIT,
	"cparse",   /* name of module */
	"A C implementation of bitbake's parse.BBHandler", /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
	             or -1 if the module keeps state in global variables. */
	cbb_parser_methods
};

PyMODINIT_FUNC PyInit_cbbparser() {
	PyObject *m = NULL;

	m = PyModule_Create(&cbb_parser_module);
	if (m == NULL) {
		return NULL;
	}

	PARSER_ERROR = PyErr_NewException("cbb_parser.error", NULL, NULL);
	Py_INCREF(PARSER_ERROR);

	PyModule_AddObject(m, "error", PARSER_ERROR);

	return m;
}
