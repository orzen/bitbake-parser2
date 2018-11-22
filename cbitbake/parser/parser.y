%{
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "args.h"
#include "files.h"
#include "parser_types.h"
#include "utils.h"
#include "utils2.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int row_num;
extern int col_num;

GHashTable *parse_tree;
gchar *filename;
struct cbb_gen_s *cur_gen;
struct cbb_conf_s *cur_conf;
struct cbb_kw_s *cur_kw;
struct cbb_block_s *cur_block;

void yyerror(const char *error_str);

%}

%code requires {
#include <glib.h>
}

/*
%define api.value.type {union YYSTYPE}
*/
%define parse.error verbose
%define parse.trace

%union {
	gint ival;
	gchar *sval;
	GHashTable *hashtable;
	GList *list;
}

%token <ival> SINGLE_QUOTE DOUBLE_QUOTE
%token <ival> INCLUDE INHERIT REQUIRE
%token <sval> WORD STRING BLOCK_START BLOCK_CONTENT

%token EXPORT PYTHON
%token PREDOT_ASSIGN POSTDOT_ASSIGN PREPEND_ASSIGN APPEND_ASSIGN
%token COLON_ASSIGN QUES_ASSIGN LAZYQUES_ASSIGN ASSIGN
%token EOL STRING_CONTINUATION BLOCK_END

%type <ival> assign_op
%type <sval> block_expr kw_expr
%type <list> recipe statements
%type <hashtable> statement conf_stmt kw_keyword kw_stmt block_stmt

%start recipe

%destructor { printf("\nfree sval\n"); g_free($$); $$ = NULL; } <sval>
%destructor { g_list_free_full($$, (GDestroyNotify) g_hash_table_unref); $$ = NULL; } <list>

/*
%destructor { g_hash_table_unref($$); $$ = NULL; } <hashtable>
*/

%%

recipe:
	statements { $$ = $1; }
	;

statements:
	statement EOL { $$ = g_list_append(NULL, $1); }
	| statement EOL statements { $$ = g_list_append($3, $1); }
	;

statement:
	/* Single line */
	block_stmt { $$ = $1; }
	| conf_stmt { $$ = $1; }
	| kw_stmt { $$ = $1; }
	;

block_stmt:
	BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $1, $2, 0); }
	| PYTHON BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $2, $3, 1); }
	;

block_expr:
	BLOCK_CONTENT { $$ = $1; }
	| BLOCK_CONTENT block_expr {
		$$ = g_strdup_printf("%s%s", $1, $2); }
	;

conf_stmt:
	WORD assign_op STRING {
		$$ = new(conf);
		conf_set($$, $1, $3, NULL, $2, 0); }
	| EXPORT WORD assign_op STRING {
		$$ = new(conf);
		conf_set($$, $2, $4, NULL, $3, 1); }
	| WORD '[' WORD ']' assign_op STRING {
		$$ = new(conf);
		conf_set($$, $1, $6, $3, $5, 0); }
	| EXPORT WORD '[' WORD ']' assign_op STRING {
		$$ = new(conf);
		conf_set($$, $2, $7, $4, $6, 1); }
	;

assign_op:
	PREDOT_ASSIGN { $$ = predot_assign; }
	| POSTDOT_ASSIGN { $$ = postdot_assign; }
	| PREPEND_ASSIGN { $$ = prepend_assign; }
	| APPEND_ASSIGN { $$ = append_assign; }
	| COLON_ASSIGN { $$ = colon_assign; }
	| QUES_ASSIGN { $$ = ques_assign; }
	| LAZYQUES_ASSIGN { $$ = lazyques_assign; }
	| ASSIGN { $$ = assign; }
	;

kw_stmt:
	kw_keyword kw_expr { $$ = add_str($1, "expr", $2); }
	;

kw_keyword:
	INCLUDE { $$ = new(kw); add_int($$, "keyword", $1); }
	| INHERIT { $$ = new(kw); add_int($$, "keyword", $1); }
	| REQUIRE { $$ = new(kw); add_int($$, "keyword", $1); }
	;

kw_expr:
	WORD { $$ = g_strdup($1); }
	| WORD kw_expr { g_free($$); $$ = g_strdup_printf("%s %s", $1, $2); }
	| WORD STRING_CONTINUATION kw_expr {
		g_free($$); $$ = g_strdup_printf("%s %s", $1, $3); }
	;

%%

void yyerror(const char *error_str) {
	printf("\nParse error at '%d: %d': '%s'\n", ++row_num, ++col_num,
	       error_str);
	exit(-1);
}

void parse_file(const gchar *fn) {
	FILE *file = NULL;
	int r = -1;

	r = cbb_open_file(fn, &file);
	if (r < 0) {
		cbb_fail("Failed to open file!");
	}

	g_free(filename);
	filename = g_strdup(fn);
	if (filename == NULL) {
		cbb_fail("Failed to duplicate string!");
	}

	parse_tree = g_hash_table_new_full(g_str_hash,
	                                   g_str_equal,
	                                   g_free,
	                                   (GDestroyNotify) cbb_gen_s_free);
	yyin = file;

	do {
		yyparse();
	} while (!feof(yyin));

	GHashTableIter iter;
	gpointer key, val;
	g_hash_table_iter_init(&iter, parse_tree);
	while (g_hash_table_iter_next (&iter, &key, &val)) {
		printf("\nITER KEY: '%s'\n", (gchar *) key);
		cbb_gen_s_print(val);
	}

	fclose(file);
	file = NULL;
	g_hash_table_unref(parse_tree);
	parse_tree = NULL;
}

int main(int argc, char **argv) {
	int i = 0;

	for (i = 1; i < argc; i++) {
		printf("arg[%d]: '%s'\n", i, argv[i]);
		parse_file(argv[i]);
	}
}
