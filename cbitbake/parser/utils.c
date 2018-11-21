#include <stdio.h>
#include <glib.h>

#include "parser_types.h"

// From bitbake.y
extern GHashTable *parse_tree;
extern struct cbb_gen_s *cur_gen;
extern struct cbb_conf_s *cur_conf;
extern struct cbb_kw_s *cur_kw;
extern struct cbb_block_s *cur_block;

#define cbb_fail(format, ...) _cbb_fail("FAILED: %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__)

void _cbb_fail(const gchar *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	exit(1);
}

#define ENSURE(TYPE) \
void ensure_##TYPE() {\
	if (cur_##TYPE != NULL) {\
		return;\
	}\
	struct cbb_##TYPE##_s *tmp = NULL;\
	tmp = cbb_##TYPE##_s_new();\
	if (tmp == NULL) {\
		cbb_fail("Failed to allocate cur_##TYPE##!");\
	}\
	cur_##TYPE = tmp;\
}

ENSURE(conf)
ENSURE(kw)
ENSURE(block)
ENSURE(gen)
#undef ENSURE

void conf_set2(const gchar *key,
	      const gchar *val,
	      const gchar *flag,
	      enum assign_op op,
	      enum quote_type quote,
	      gboolean export) {
	if (key == NULL) {
		cbb_fail("Failed key is NULL!");
	}

	ensure_conf();

	cur_conf->key = g_strdup(key);
	if (cur_conf->key == NULL) {
		cbb_fail("Failed to duplicate string!");
	}

	if (val != NULL) {
		cur_conf->val = g_strdup(val);
		if (cur_conf->val == NULL) {
			cbb_fail("Failed to duplicate string!");
		}
	}

	if (flag != NULL) {
		cur_conf->flag = g_strdup(flag);
		if (cur_conf->flag == NULL) {
			cbb_fail("Failed to duplicate string!");
		}
	}

	cur_conf->op = op;
	cur_conf->quote = quote;
	cur_conf->export = export;
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

void kw_set(gint keyword, const gchar *val) {
	if (val == NULL) {
		cbb_fail("Failed 'val == NULL'!");
	}

	ensure_kw();

	cur_kw->type = keyword;

	cur_kw->val = g_strdup(val);
	if (cur_kw->val == NULL) {
		cbb_fail("Failed duplicate string!");
	}
}

void block_set2(const gchar *name, const gchar *expr, gboolean python) {
	if (name == NULL || expr == NULL) {
		cbb_fail("Failed 'arg == NULL'!");
	}

	ensure_block();

	cur_block->name = g_strdup(name);
	if (cur_block->name == NULL) {
		cbb_fail("Failed to duplicate string!");
	}

	cur_block->expr = g_strdup(expr);
	if (cur_block->expr == NULL) {
		cbb_fail("Failed to duplicate string!");
	}

	cur_block->python = python;
}

#if 0
void append_entry() {
	gboolean r = FALSE;
	gchar *key = NULL;

	ensure_gen();

	if (cur_conf != NULL) {
		cur_gen->type = conf;
		cur_gen->data = cur_conf;

		cur_conf = NULL;
	} else if (cur_kw != NULL) {
		cur_gen->type = kw;
		cur_gen->data = cur_kw;

		cur_kw = NULL;
	} else if (cur_block != NULL) {
		cur_gen->type = block;
		cur_gen->data = cur_block;

		cur_block = NULL;
	} else {
		cbb_fail("No entries to append!");
	}

	key = g_strdup_printf("%s:%d", filename, rule_begins);
	cur_gen->filename = g_strdup(filename);
	cur_gen->lineno = rule_begins;
	cbb_gen_s_print(cur_gen);

	r = g_hash_table_replace(parse_tree, key, cur_gen);
	if (r != TRUE) {
		cbb_fail("Failed to append entry '%s' to parse tree!", key);
	}
	cur_gen = NULL;
}
#endif
