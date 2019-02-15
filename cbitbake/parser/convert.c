#include <glib.h>
#include <Python.h>
#include <stdarg.h>

#include "log.h"
#include "node.h"
#include "parser_types.h"
#include "pyobj_ast.h"
#include "pyobj_match.h"

static gint convert_child_to_pydict(PyDict *dict,
                                    GNode *node,
                                    AstHandleFunc ast_override) {
	GNode *child = NULL;
	struct node_int *internal = NULL;
	gint r = -1;
	const gchar *key = NULL;
	PyObject *value = NULL;

	if (node == NULL) {
		log_warn("failed to traverse children, empty node");
		return -1;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		internal = child->data;

		switch(internal->type) {
		case body:
			key = "value";
			r = PyDict_SetItemString(dict, "apo",
			                         internal->data[0]);
			if (r != 0) {
				log_warn("failed to add apo");
				return -1;
			}
			break;
		case exp:
			key = "exp";
			value = Py_True;
			break;
		default:
			key = types_itoa(internal->type);
			value = PyBytes_FromString(internal->data);
		}

		r = PyDict_SetItemString(dict, key, value);
		if (r != 0) {
			log_warn("failed to add key '%s', value '%s' to dict", key, val);
			return -1;
		}
	}

	return 0;
}

static gint convert_child_to_pylist(PyObject *list,
                                    GNode *node,
                                    AstHandleFunc ast_override) {
	GNode *child = NULL;
	struct node_int *internal = NULL;
	gint r = -1;

	if (node == NULL) {
		log_warn("failed to traverse children, empty node");
		return -1;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		PyObject *value = NULL;
		internal = child->data;

		switch(internal->type) {
		case:
			value = PyLong_FromLong(internal->data);
			break;
		default:
			value = PyBytes_FromString(internal->data);
		}

		r = PyList_Append(list, value);
		if (r != 0) {
			log_warn("failed to append item to list");
			return -1;
		}
	}

	return 0;
}

static gint convert_child_to_tuple(PyObject *tuple,
                                   GNode *node,
                                   AstHandleFunc ast_override) {
	GNode *child = NULL;
	struct node_int *internal = NULL;
	gint i = 3;
	gint r = -1;

	if (tuple == NULL || node == NULL) {
		log_warn("empty args");
		return -1;
	}

	if (ast_override != NULL) {
		log_warn("override already set");
		return -1;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		PyObject *value = NULL;
		internal = child->data;

		switch(internal->type) {
		case python:
			ast_override = &ast_handle_python_method;
		case fakeroot:
			value = PyBool_FromLong(internal->data);
			break;
		default:
			value = PyBytes_FromString(internal->data);
		}

		r = PyTuple_SetItem(tuple, i, value);
		if (r != 0) {
			log_warn("failed to add item to tuple");
			return -1;
		}

		i++;
	}

	return 0;
}

static gint ast_request_add_generic(PyObject *tuple,
                               PyObject *statements,
                               gchar *filename,
                               gint lineno) {

	if (tuple == NULL || statements == NULL || filename == NULL) {
		log_warn("empty args");
		return -1;
	}

	r = PyTuple_SetItem(args, 0, statements);
	if (r != 0) {
		log_warn("failed to add statements to ast args");
		return -1;
	}
	r = PyTuple_SetItem(args, 1, PyBytes_FromString(filename));
	if (r != 0) {
		log_warn("failed to add statements to ast args");
		return -1;
	}
	r = PyTuple_SetItem(args, 2, PyLong_FromLong(lineno));
	if (r != 0) {
		log_warn("failed to add statements to ast args");
		return -1;
	}

	return 0;
}

/*
 * Returns a GList with two elements, callback function and callback args, on
 * success, otherwise returns NULL.
 */
static GList* ast_finalize_request(PyObject *args, GNode *entry, enum node_type type) {
	PyObject *data = NULL;
	PyObject *match = NULL;
	gint r = -1;
	GList *ret = NULL;
	AstHandleFunc ast_func = NULL;

	switch (type) {
	case conf:
		data = PyDict_New();
		gint *op = NULL;

		op = entry->data;

		r = PyDict_SetItemString(data, types_op_itoa(*op), Py_True);
		if (r != 0) {
			log_warn("failed to add op to PyDict");
			return NULL;
		}

		r = convert_child_to_pydict(data, child, ast_func);
		break;
	case addtask:
		data = PyDict_New();

		r = PyDict_SetItemString(data, "func", entry->data);
		if (r != 0) {
			log_warn("failed to add addtask to PyDict");
			return NULL;
		}

		r = convert_child_to_pydict(data, node, ast_func);
		break;
	case deltask:
		data = PyDict_New();
		r = convert_child_to_pydict(data, node, ast_func);
		break;
	case func:
		r = convert_child_to_tuple(args, node, ast_func);
		data = args;
		break;
	default:
		data = PyList_New();
		r = convert_child_to_pylist(data, node, ast_func);
	}

	if (r != 0) {
		log_warn("failed to convert child node to args");
		return NULL;
	}

	if (PyList_CheckExact(data) == Py_True) {
		match = Match_new_int();
		r = Match_set_list(match, data);
	} else if (PyDict_CheckExact(data) == Py_True) {
		match = Match_new_int();
		r = Match_set_dict(match, data);
	}

	if (ast_func == NULL) {
		ast_func = types_get_ast_callback(type);
	}

	ret = g_list_append(NULL, ast_func);
	ret = g_list_append(ret, data);

	return ret;
}

static GList* construct_ast_request(GNode *entry,
                                    PyObject *statements,
                                    gchar *filename) {
	struct node_int *internal = NULL;
	PyObject *args = NULL;
	gint type = -1;
	gint num_args = 0;
	gint r = -1;

	internal = entry->data;
	type = internal->type;

	num_args = types_get_ast_num_args(type);

	args = PyTuple_New(num_args);
	if (args == NULL) {
		log_warn("failed to create ast args");
		return NULL;
	}

	r = ast_request_add_generic(args, statements, filename, internal->lineno);
	if (r != 0) {
		log_warn("failed to append generic information");
		return NULL;
	}

	return ast_finalize_request(args, entry, type);
}

gint convert_ast_to_python(PyObject *ast,
                           PyObject *statements,
                           GNode *root,
                           gchar *filename) {
	GNode *child = NULL;
	GList *func_and_args = NULL;
	AstHandleFunc ast_handler = NULL;
	PyObject *ast_args = NULL;

	if (root == NULL || filename == NULL) {
		log_warn("empty args");
		return -1;
	}

	for (child = root->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		func_and_args = construct_ast_request(child,
		                                      statements,
		                                      filename);
		if (func_and_args == NULL) {
			log_warn("invalid AST data");
			return -1;
		}

		ast_handler = func_and_args->data;
		//func_and_args = g_list_next(func_and_args);
		ast_args = func_and_args->next->data;

		ast_handler(ast, ast_args);
	}

	return 0;
}

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
