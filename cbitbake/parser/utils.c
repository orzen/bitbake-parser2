#include <stdio.h>
#include <glib.h>

#include "utils.h"

extern gchar *filename;
extern gint row_num;

void _cbb_fail(const gchar *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	exit(1);
}

gchar* kw_keyword_itoa(gint keyword) {
	gchar *tmp = NULL;

	if (keyword == include) {
		tmp = g_strdup("include");
	} else if (keyword == inherit) {
		tmp = g_strdup("inherit");
	} else {
		tmp = g_strdup("require");
	}

	if (tmp == NULL) {
		cbb_fail("Failed to duplicate string!");
	}

	return tmp;
}

GHashTable* add_str(GHashTable *table, const gchar *key, const gchar *val) {
	gboolean r = FALSE;

	r = g_hash_table_insert(table, g_strdup(key), g_strdup(val));
	if (r != TRUE) {
		cbb_fail("Failed to add string to hashtable!");
	}

	return table;
}

GHashTable* add_int(GHashTable *table, const gchar *key, gint val) {
	gboolean r = FALSE;
	gint *value = calloc(1, sizeof(gint));
	*value = val;

	r = g_hash_table_insert(table, g_strdup(key), value);
	if (r != TRUE) {
		cbb_fail("Failed to add string to hashtable!");
	}

	return table;
}

GHashTable* new(enum parser_type type) {
	GHashTable *tmp = NULL;
	gint *lineno = NULL;
	gint *parser_type = NULL;
	gboolean r = FALSE;

	tmp = g_hash_table_new_full(g_str_hash,
	                            g_str_equal,
	                            g_free,
	                            g_free);

	r = g_hash_table_insert(tmp, g_strdup("filename"), g_strdup(filename));
	if (r != TRUE) {
		cbb_fail("Failed to insert hashtable value!");
	}

	lineno = calloc(1, sizeof(gint));
	*lineno = row_num;

	r = g_hash_table_insert(tmp, g_strdup("lineno"), lineno);
	if (r != TRUE) {
		cbb_fail("Failed to insert hashtable value!");
	}

	parser_type = calloc(1, sizeof(gint));
	*parser_type = type;

	r = g_hash_table_insert(tmp, g_strdup("type"), parser_type);
	if (r != TRUE) {
		cbb_fail("Failed to insert hashtable value!");
	}

	return tmp;
}

void conf_set(GHashTable *table,
              const gchar *key,
              const gchar *val,
              const gchar *flag,
              enum assign_op op) {
	enum quote_type quote = single_quote;
	if (val[0] == '"') {
		quote = double_quote;
	}

	add_str(table, "key", key);
	add_str(table, "val", val);
	add_str(table, "flag", flag);
	add_int(table, "quote", quote);
	add_int(table, "op", op);
}

void block_set(GHashTable *table,
               const gchar *key,
               const gchar *expr,
               gint fakeroot,
               gint python) {
	if (table == NULL || key == NULL || expr == NULL) {
		cbb_fail("empty pointer args");
	}

	add_str(table, "key", key);
	add_str(table, "expr", expr);
	add_int(table, "fakeroot", fakeroot);
	add_int(table, "python", python);
}
