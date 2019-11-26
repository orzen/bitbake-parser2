#include <glib.h>
#include <Python.h>
#include <stdarg.h>

#include "convert.h"
#include "log.h"
#include "node.h"
#include "parser_types.h"
#include "pyo_ast2.h"
#include "pyo_match.h"
#include "string.h"

#define STEP_ASSIGN(NODE, CHILD, VAR) \
	if ((CHILD = g_node_next_sibling(CHILD)) != NULL) { \
		VAR = CHILD->data; \
	}

#define DICT_SET_OR_LOG(NAME) \
	if (PyDict_SetItemString(groupd, "##NAME##", data) != 0) { \
		log_warn("failed to add '##NAME##' to groupd"); \
	}

/*
 * This is 
 * Export
 * Unset
 * BBHandlers
 * Inherit
 */
static void compat_handle_common(struct ast *ast,
                                 GNode *node,
                                 PyObject *statements,
                                 const gchar *filename) {
	GNode *child = NULL;
	struct node_int *n = NULL;
	PyObject *list = NULL;
	PyObject *match = NULL;
	AstHandleFunc func = NULL;

	G_DEBUG_HERE();

	if (!ast || !node || !statements || !filename) {
		log_warn("invalid arguments");
		goto error;
	}

	n = node->data;

	list = PyList_New(2);
	if (!list) {
		log_warn("failed to create list");
		return;
	}

	if (PyList_SetItem(list, 1, PyUnicode_FromString(n->data)) != 0) {
		log_warn("failed to set list item to common data");
		goto error;
	}

	match = Match_new_int(NULL, list);
	if (!match) {
		log_warn("failed to create match object");
		goto error;
	}

	func = types_get_ast_callback(n->type);
	func(ast, statements, filename, n->lineno, match);

	return;
error:
	if (list) {
		Py_DECREF(list);
	}
}

static void compat_handle_data(struct ast *ast,
                               GNode *node,
                               PyObject *statements,
                               const gchar *filename) {
	GNode *child = NULL;
	struct node_int *n = NULL;
	PyObject *groupd = NULL;
	gint *data = NULL;
	gint lineno = -1;
	gint r = -1;

	G_DEBUG_HERE();

	if (!ast || !node || !statements || !filename) {
		log_warn("invalid arguments");
		goto error;
	}

	n = node->data;
	lineno = n->lineno;
	data = n->data;

	groupd = PyDict_New();
	if (!groupd) {
		log_warn("failed to allocate PyDict");
		goto error;
	}

	r = PyDict_SetItemString(groupd, types_op_itoa(*data), Py_True);
	if (r != 0) {
		log_warn("failed to add assign operator to groupd");
		goto error;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		n = child->data;
		PyObject *data = NULL;
		switch (n->type) {
			case var:
				data = PyUnicode_FromString(n->data);
				DICT_SET_OR_LOG(var)
				break;
			case body:
				data = PyUnicode_FromString(n->data);
				DICT_SET_OR_LOG(value)
				break;
			case flag:
				data = PyUnicode_FromString(n->data);
				DICT_SET_OR_LOG(flag)
				break;
			case exported:
				data = PyBool_FromLong(n->data);
				DICT_SET_OR_LOG(exp)
				break;
			default:
				log_warn("invalid config type '%d'", n->type);
		}
		if (data) {
			Py_DECREF(data);
			data = NULL;
		}
	}

#undef DICT_SET_OR_LOG

	ast_handleData(ast,
	               statements,
	               filename,
	               lineno,
	               groupd);

error:
	if (groupd) {
		Py_DECREF(groupd);
	}
}

static void compat_handle_method(struct ast *ast,
                                 GNode *node,
                                 PyObject *statements,
                                 const gchar *filename) {
	GNode *child = NULL;
	struct node_int *n = NULL;
	gint lineno = -1;

	n = child->data;
	lineno = n->lineno;
	log_warn("FUNC: file '%s':'%d', '%d'", filename, lineno, n->type);

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		n = child->data;
		log_warn("FUNC: file '%s':'%d', '%d'", filename, lineno, n->type);
	}
}

static void handle_node(struct ast *ast,
                        GNode *entry,
                        PyObject *statements,
                        gchar *filename) {
	struct node_int *internal = NULL;

	G_DEBUG_HERE();

	internal = entry->data;

// Export
// Unset
// BBHandlers
// Inherit
	switch (internal->type) {
		case conf:
			log_dbg("data");
			compat_handle_data(ast,
			                   entry,
			                   statements,
			                   filename);
			break;
		case inherit:
			log_dbg("inherit");
			compat_handle_common(ast,
			                     entry,
			                     statements,
			                     filename);
			break;
		case exported:
			log_dbg("export");
			compat_handle_common(ast,
			                     entry,
			                     statements,
			                     filename);
			break;
		case unset:
			log_dbg("unset");
			compat_handle_common(ast,
			                     entry,
			                     statements,
			                     filename);
			break;
		default:
			log_warn("unhandled type '%d'", internal->type);
	}
}

gint compat_call_bbast(struct ast *ast,
                       GNode *root,
                       PyObject *statements,
                       gchar *filename) {
	GNode *child = NULL;
	AstHandleFunc ast_handler = NULL;
	PyObject *ast_args = NULL;

	if (root == NULL || filename == NULL) {
		log_warn("empty args");
		return -1;
	}

	for (child = root->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		handle_node(ast, child, statements, filename);
	}

	return 0;
}

#if 0
PyObject* convert_seq_to_pylist(gchar *fmt, ...) {
	va_list ap;
	gint d;
	gint r;
	gchar *s;
	PyObject *list;
	PyObject *arg = NULL;

	if (fmt == NULL) {
		return NULL;
	}

	list = PyList_New(0);
	if (list == NULL) {
		return NULL;
	}

	va_start(ap, fmt);
	while (*fmt) {
		switch (*fmt++) {
		case 's':
			s = va_arg(ap, gchar *);
			arg = PyUnicode_FromString(s);

			r = PyList_Append(list, arg);
			Py_DECREF(arg);
			arg = NULL;
			if (r != 0) {
				g_warning("failed to append value '%s' to list", s);
				Py_DECREF(list);
				return NULL;
			}
			break;
		case 'd':
			d = va_arg(ap, gint);
			arg = PyLong_FromLong(d);

			r = PyList_Append(list, arg);
			Py_DECREF(arg);
			arg = NULL;
			if (r != 0) {
				g_warning("failed to append value '%d' to list", d);
				Py_DECREF(list);
				return NULL;
			}
			break;
		}
	}
	va_end(ap);

	return list;
}
#endif
