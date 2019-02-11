#include <glib.h>
#include <stdio.h>

#include "parser_types.h"
#include "utils.h"

#define TYPE_NEW(TYPE) \
struct cbb_##TYPE##_s* cbb_##TYPE##_s_new() {\
	return calloc(1, sizeof(struct cbb_##TYPE##_s));\
}

TYPE_NEW(conf)
TYPE_NEW(kw)
TYPE_NEW(block)
TYPE_NEW(gen)
#undef TYPE_NEW

gchar* cbb_types_assign_op_itoa(enum assign_op op) {
	switch(op) {
	case predot_assign:
		return g_strdup("predot");
		break;
	case postdot_assign:
		return g_strdup("postdot");
		break;
	case prepend_assign:
		return g_strdup("prepend");
		break;
	case append_assign:
		return g_strdup("append");
		break;
	case colon_assign:
		return g_strdup("colon");
		break;
	case ques_assign:
		return g_strdup("ques");
		break;
	case lazyques_assign:
		return g_strdup("lazyques");
		break;
	case assign:
		return g_strdup("assign");
		break;
	};
}

gchar* cbb_types_kw_itoa(enum kw_type kw) {
	switch (kw) {
	case include:
		return g_strdup("include");
		break;
	case inherit:
		return g_strdup("inherit");
		break;
	case require:
		return g_strdup("require");
		break;
	case export_funcs:
		return g_strdup("EXPORT_FUNCTIONS");
		break;
	case deltask:
		return g_strdup("deltask");
		break;
	};
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

gint types_get_type(GHashTable *entry) {
	if (entry == NULL) {
		return -1;
	}

	// TODO continue here!
	g_hash_table_lookup(entry, "type");
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
