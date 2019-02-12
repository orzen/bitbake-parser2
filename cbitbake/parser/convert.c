#include <glib.h>
#include <Python.h>
#include <stdarg.h>

#include "log.h"
#include "node.h"
#include "parser_types.h"

struct traverse_data {
	GList *parents;
	PyObject *list;
	PyObject *tuple;
};

static gint convert_traverse_to_pydict(GNode *node, PyObject **dict) {
	GNode *child = NULL;
	struct node_int *internal = NULL;

	if (node == NULL) {
		log_warn("failed to traverse children, empty node");
		return -1;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		internal = child->data;
		g_warning("type: '%s', lineno: '%d'",
		          types_itoa(internal->type), internal->lineno);
	}

	return 0;
}

static gint convert_traverse_to_pylist(GNode *node, PyObject **list) {
	GNode *child = NULL;
	struct node_int *internal = NULL;

	if (node == NULL) {
		log_warn("failed to traverse children, empty node");
		return -1;
	}

	for (child = node->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		internal = child->data;
		g_warning("type: '%s', lineno: '%d'",
		          types_itoa(internal->type), internal->lineno);
	}

	return 0;
}

static gint convert_traverse_func(GNode *root,
                                  gpointer user_data) {
	GNode *child = NULL;
	struct node_int *internal = NULL;

	if (root == NULL) {
		log_warn("empty args");
		return -1;
	}

	for (child = root->children;
	     child != NULL;
	     child = g_node_next_sibling(child)) {
		internal = child->data;
		gint type = internal->type;
		PyObject *args = NULL;
		void *ast_callback;

		args = PyDict_new();
		if (args == NULL) {
			log_warn("failed to create PyDict");
			return -1;
		}

		g_warning("type: '%s', lineno: '%d'",
		          types_itoa(internal->type), internal->lineno);

		if (internal->type == conf) {
			gint *assign = NULL;
			assign = internal->data;
			g_warning("conf: '%s'", cbb_types_assign_op_itoa(*assign));
			convert_traverse_to_pydict(child, );
		} else {
			g_warning("%s: '%s'", types_itoa(type),
			          (gchar *) internal->data);
			convert_traverse_to_pylist(child, );
		}

		//TODO create match-object

		ast_callback = types_get_ast_callback(internal->type);
	}

	return 0;
}

PyObject* convert_ast_to_python(GNode *root) {
	struct traverse_data *u_data = NULL;

	if (root == NULL) {
		g_warning("unspecified root node");
		return NULL;
	}

	u_data = calloc(1, sizeof(struct traverse_data));

	convert_traverse_func(root, u_data);

	return NULL;
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

#if 0
#define GEN_CONVERT_KW(TYPE) \
gint _convert_kw_##TYPE##(GHashTable *entry) { \
	void *callback = &ast_handle_##TYPE; \
}

GEN_CONVERT_KW(inherit)
GEN_CONVERT_KW(include)
GEN_CONVERT_KW(require)
#undef GEN_CONVERT_KW

gint entry_to_match_obj(PyObject **res_dict, PyObject *ast, GHashTable *tbl) {
	PyObject *obj_filename = NULL;
	PyObject *obj_lineno = NULL;
	PyObject *tmp_dict = NULL;
	gint r = -1;

	if (dict == NULL || tbl == NULL) {
		g_warning("empty arguments");
		return -1;
	}

	if (*dict != NULL) {
		g_warning("pointer is already assigned");
		return -1;
	}

	tmp_dict = PyDict_New();
	if (tmp_dict == NULL) {
		g_warning("failed to allocate PyDict");
		return -1;
	}

	if (r != 0) {
		g_warning("failed to configure match object");
		Py_DECREF(tmp_dict);
		Py_DECREF(obj_filename);
		Py_DECREF(obj_lineno);
		Py_DECREF(match);
		return -1;
	}

	*res_dict = tmp_dict;

	Py_DECREF(obj_filename);
	Py_DECREF(obj_lineno);

	return 0;
}

gint block_to_pydict(PyObject **dict, GHashTable *tbl) {
	gchar *k_key = NULL;
	gchar *k_expr = NULL;
	gint *k_fakeroot = NULL;
	gint *k_python = NULL;
	PyObject *tmp_dict = NULL;
	PyUnicode *uni_key = NULL;
	PyUnicode *uni_expr = NULL;
	PyUnicode *uni_fakeroot = NULL;
	PyUnicode *uni_python = NULL;

	k_key = g_hash_table_lookup(tbl, "key");
	if (k_key == NULL) {
		g_warning("key is missing");
		return -1;
	}
	k_expr = g_hash_table_lookup(tbl, "expr");
	if (k_expr == NULL) {
		g_warning("expr is missing");
		return -1;
	}
	k_fakeroot = g_hash_table_lookup(tbl, "fakeroot");
	if (k_fakeroot == NULL) {
		g_warning("fakeroot is missing");
		return -1;
	}
	k_python = g_hash_table_lookup(tbl, "python");
	if (k_python == NULL) {
		g_warning("python is missing");
		return -1;
	}

	tmp_dict = PyDict_New();
	if (tmp_dict == NULL) {
		g_warning("failed to allocate PyDict");
		return -1;
	}

	uni_key = PyUnicode_FromString("key");
	uni_expr = PyUnicode_FromString("expr");
	uni_fakeroot = PyUnicode_FromString("fakeroot");
	uni_python = PyUnicode_FromString("python");

}

static gint arrange_conf(PyObject **dict, GHashTable *tbl) {
	gchar *k_key    = NULL;
	gchar *k_val    = NULL;
	gchar *k_flag   = NULL;
	gchar *k_quote  = NULL;
	gchar *k_op     = NULL;
	gchar *k_export = NULL;
	PyObject *tmp_dict = NULL;
	PyUnicode *uni_key    = NULL;
	PyUnicode *uni_val    = NULL;
	PyUnicode *uni_flags  = NULL;
	PyUnicode *uni_quote  = NULL;
	PyUnicode *uni_op     = NULL;
	PyUnicode *uni_export = NULL;

	if (dict == NULL || tbl == NULL) {
		g_warning("empty arguments");
		return -1;
	}

	if (*dict != NULL) {
		g_warning("pointer is already assigned");
		return -1;
	}

	uni_key    = PyUnicode_FromString("key");
	uni_val    = PyUnicode_FromString("val");
	uni_flags  = PyUnicode_FromString("flag");
	uni_quote  = PyUnicode_FromString("quote");
	uni_op     = PyUnicode_FromString("op");
	uni_export = PyUnicode_FromString("export");

	k_key    = g_hash_table_lookup(tbl, "key");
	k_val    = g_hash_table_lookup(tbl, "val");
	k_flag   = g_hash_table_lookup(tbl, "flag");
	k_quote  = g_hash_table_lookup(tbl, "quote");
	k_op     = g_hash_table_lookup(tbl, "op");
	k_export = g_hash_table_lookup(tbl, "export");

}

static _include_to_match() {
	PyObject *match_obj = NULL;

	match_obj = Match_new_int();
}

static PyObject* convert_kw_to_match_list(enum kw_type kw_t, GHashTable *tbl) {
	gint *k_keyword = NULL;
	gchar *k_expr   = NULL;
	gint *k_export  = NULL;
	PyObject *match = NULL;

	PyUnicode *uni_quote  = PyUnicode_FromString("keyword");
	PyUnicode *uni_expr   = PyUnicode_FromString("expr");

	match = Match_new_int();
	if (match == NULL) {
		g_warning("failed to create match object");
		return -1;
	}

	expr = g_hash_table_lookup(tbl, "expr");

	switch (kw_t) {
	case inherit:
	case include:
	case require:
		convert_va_args_to_pylist("is", kw_t, expr);
		break;
	case export_funcs:
		break;
	case deltask:
		break;
	};

	match__set_list();
	match__set_dict();

	return match;
}

static PyObject* convert_sl_to_list() {
	/*
	 * handleExport
	 * handleUnset
	 * handleUnsetFlag
	 * handleData
	 */

}

static PyObject* arrange_addtask() {
	PyUnicode *uni_content = PyUnicode_FromString("content");
	PyUnicode *uni_after   = PyUnicode_FromString("after");
	PyUnicode *uni_before  = PyUnicode_FromString("before");

	gchar *k_content = g_hash_table_lookup(tbl, "content");
	gchar *k_after   = g_hash_table_lookup(tbl, "after");
	gchar *k_before  = g_hash_table_lookup(tbl, "before");

}

static gint propagate_block_stmt(PyObject *ast, GHashTable *tbl,
                                 const gchar *filename, gint lineno) {
	gchar *key = NULL;
	gchar *expr = NULL;
	gint *fakeroot = NULL;
	gint *python = NULL;

	if (ast == NULL || tbl == NULL || filename == NULL) {
		g_warning("empty args");
		return -1;
	}

	key = g_hash_table_lookup(tbl, "key");
	expr = g_hash_table_lookup(tbl, "expr");
	fakeroot = g_hash_table_lookup(tbl, "fakeroot");
	python = g_hash_table_lookup(tbl, "python");
}

static gint propagate_conf_stmt(PyObject *ast, GHashTable *tbl,
                                const gchar *filename, gint lineno) {
	gchar *key = NULL;
	gchar *val = NULL;
	gchar *flag = NULL;
	gint *quote = NULL;
	gint *op = NULL;
	gint *export = NULL;

	if (ast == NULL || tbl == NULL || filename == NULL) {
		g_warning("empty args");
		return -1;
	}

	key = g_hash_table_lookup(tbl, "key");
	val = g_hash_table_lookup(tbl, "val");
	flag = g_hash_table_lookup(tbl, "expr");
	quote = g_hash_table_lookup(tbl, "quote");
	op = g_hash_table_lookup(tbl, "op");
	export = g_hash_table_lookup(tbl, "export");

}

static gint propagate_kw_stmt(PyObject *ast, GHashTable *tbl,
                              const gchar *filename, gint lineno) {
	gchar *keyword = NULL;
	gchar *expr = NULL;
	gint *export = NULL;

	if (ast == NULL || tbl == NULL || filename == NULL) {
		g_warning("empty args");
		return -1;
	}

	keyword = g_hash_table_lookup(tbl, "keyword");
	expr = g_hash_table_lookup(tbl, "expr");
	export = g_hash_table_lookup(tbl, "export");
}

static gint propagate_addtask_stmt(PyObject *ast, GHashTable *tbl,
                                   const gchar *filename, gint lineno) {
	gchar *content = NULL;
	gchar *after = NULL;
	gchar *before = NULL;
	gint *export = NULL;

	if (ast == NULL || tbl == NULL || filename == NULL) {
		g_warning("empty args");
		return -1;
	}

	key = g_hash_table_lookup(tbl, "key");
	expr = g_hash_table_lookup(tbl, "expr");
	fakeroot = g_hash_table_lookup(tbl, "fakeroot");
	python = g_hash_table_lookup(tbl, "python");

}

gint propagate_arbitrary(PyObject *ast, GHashTable *tbl) {
	gchar *filename = NULL;
	gint *lineno = NULL;
	gint *type = NULL;
	gint r = -1;

	if (ast == NULL || tbl == NULL) {
		g_warning("empty args");
		return -1;
	}

	filename = g_hash_table_lookup(tbl, "filename");
	lineno = g_hash_table_lookup(tbl, "lineno");
	type = g_hash_table_lookup(tbl, "type");

	switch (*type) {
		case block: 
			r = propagate_block_stmt(ast, tbl, filename, *lineno);
			break;
		case conf:
			r = propagate_conf_stmt(ast, tbl, filename, *lineno);
			break;
		case kw:
			r = propagate_kw_stmt(ast, tbl, filename, *lineno);
			break;
		case addtask:
			r = propagate_addtask_stmt(ast, tbl, filename, *lineno);
			break;
	}

	return r;
}
#endif
