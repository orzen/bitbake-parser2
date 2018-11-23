#ifndef _PARSER_TYPES_H_
#define _PARSER_TYPES_H_

enum parser_type {
	block = 0,
	conf,
	kw,
};

enum assign_op {
	predot_assign = 0,
	postdot_assign,
	prepend_assign,
	append_assign,
	colon_assign,
	ques_assign,
	lazyques_assign,
	assign
};

enum quote_type {
	single_quote = 0,
	double_quote
};

enum kw_type {
	include = 0,
	inherit,
	require
};

#if 0
struct cbb_conf_s {
	gchar *key;
	gchar *val;
	gchar *flag;
	gboolean export;
	enum assign_op op;
	enum quote_type quote;
};

struct cbb_kw_s {
	enum kw_type type;
	gchar *val;
};

struct cbb_block_s {
	gchar *name;
	gchar *expr;
	gboolean python;
};

struct cbb_gen_s {
	gchar *filename;
	gint lineno;
	enum parser_type type;
	void *data;
};

struct cbb_conf_s* cbb_conf_s_new();
void cbb_conf_s_free(struct cbb_conf_s *conf);
void cbb_conf_s_print(struct cbb_conf_s *conf);

struct cbb_kw_s* cbb_kw_s_new();
void cbb_kw_s_free(struct cbb_kw_s *list);
void cbb_kw_s_print(struct cbb_kw_s *list);

struct cbb_block_s* cbb_block_s_new();
void cbb_block_s_free(struct cbb_block_s *block);
void cbb_block_s_print(struct cbb_block_s *block);

struct cbb_gen_s* cbb_gen_s_new();
void cbb_gen_s_free(struct cbb_gen_s *gen);
void cbb_gen_s_print(struct cbb_gen_s *gen);
#endif

#endif
