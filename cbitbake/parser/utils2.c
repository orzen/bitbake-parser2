#include <glib.h>

#include "parser_types.h"
#include "utils.h"

extern gchar *filename;
extern gint yylineno;

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
	*lineno = yylineno;

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
              enum assign_op op,
              gint export) {
	enum quote_type quote = single_quote;
	if (val[0] == '"') {
		quote = double_quote;
	}

	add_str(table, "key", key);
	add_str(table, "val", val);
	add_str(table, "flag", flag);
	add_int(table, "quote", quote);
	add_int(table, "op", op);
	add_int(table, "export", export);
}

void block_set(GHashTable *table,
               const gchar *key,
               const gchar *expr,
               gint python) {
	add_str(table, "key", key);
	add_str(table, "expr", expr);
	add_int(table, "python", python);
}
