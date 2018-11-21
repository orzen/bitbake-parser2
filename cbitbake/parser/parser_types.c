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

void cbb_conf_s_free(struct cbb_conf_s *conf) {
	if (conf == NULL) {
		return;
	}

	if (conf->key != NULL) {
		g_free(conf->key);
	}

	if (conf->val != NULL) {
		g_free(conf->val);
	}

	if (conf->flag != NULL) {
		g_free(conf->flag);
	}

	conf = NULL;
}

void cbb_conf_s_print(struct cbb_conf_s *conf) {
	if (conf == NULL) {
		fprintf(stderr, "cbb_conf_s is EMPTY!");
		return;
	}

	printf("* cbb_conf_s:\n\tkey: '%s'\n\tval: '%s'\n\tflag: '%s'\n\texport: '%d'\n\tassign_op: '%d'\n",
	       conf->key,
	       conf->val,
	       conf->flag,
	       conf->export,
	       conf->op);
}

void cbb_kw_s_free(struct cbb_kw_s *kw) {
	if (kw == NULL) {
		return;
	}

	if (kw->val != NULL) {
		g_free(kw->val);
	}

	kw = NULL;
}

void cbb_kw_s_print(struct cbb_kw_s *kw) {
	if (kw == NULL) {
		fprintf(stderr, "cbb_kw_s is EMPTY!");
		return;
	}

	printf("* cbb_kw_s:\n\ttype: '%d'\n\tval: '%s'\n",
	       kw->type,
	       kw->val);
}

void cbb_block_s_free(struct cbb_block_s *block) {
	if (block == NULL) {
		return;
	}

	if (block->name != NULL) {
		g_free(block->name);
	}

	if (block->expr != NULL) {
		g_free(block->expr);
	}

	block = NULL;
}

void cbb_block_s_print(struct cbb_block_s *block) {
	if (block == NULL) {
		fprintf(stderr, "cbb_block_s is EMPTY!");
		return;
	}

	printf("* cbb_block_s:\n\tname: '%s'\n\texpr: '%s'\n",
	       block->name,
	       block->expr);
}

void cbb_gen_s_free(struct cbb_gen_s *gen) {
	if (gen == NULL) {
		return;
	}

	if (gen->filename != NULL) {
		g_free(gen->filename);
	}

	if (gen->data == NULL) {
		cbb_fail("Trying to free empty data!");
	}
	switch (gen->type) {
		case conf:
			cbb_conf_s_free(gen->data);
			break;
		case kw:
			cbb_kw_s_free(gen->data);
			break;
		case block:
			cbb_block_s_free(gen->data);
			break;
		default:
			cbb_fail("Invalid type!");
			break;
	}

	gen = NULL;
}

void cbb_gen_s_print(struct cbb_gen_s *gen) {
	if (gen == NULL) {
		fprintf(stderr, "cbb_gen_s is EMPTY!");
		return;
	}

	printf("**** START GEN ****\n");
	printf("* cbb_gen_s:\n\tfilename: '%s'\n\tlineno: '%d'\n",
	       gen->filename,
	       gen->lineno);
	if (gen->data == NULL) {
		cbb_fail("Failed to print empty data!");
	}

	switch (gen->type) {
		case conf:
			cbb_conf_s_print(gen->data);
			break;
		case kw:
			cbb_kw_s_print(gen->data);
			break;
		case block:
			cbb_block_s_print(gen->data);
			break;
		default:
			cbb_fail("Invalid type!");
			break;
	}
	printf("**** END GEN ****\n");
}
