#include <glib.h>
#include <Python.h>

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
