%{
#include <stdio.h>
#include <glib.h>

#include "args.h"
#include "files.h"
#include "parser_types.h"
#include "utils.h"
%}

%define api.pure full
%define parse.error verbose

%code requires {
#include <glib.h>
#include "lexer.h"
}
%verbose

%lex-param {yyscan_t yyscanner}
%parse-param {yyscan_t yyscanner} {GList **acc}

%token <ival> SINGLE_QUOTE DOUBLE_QUOTE
%token <ival> DELTASK EXPORT EXPORT_FUNCTIONS INCLUDE INHERIT REQUIRE
%token <sval> WORD STRING BLOCK_START BLOCK_CONTENT ADDTASK_CONTENT

%token ADDTASK ADDTASK_AFTER ADDTASK_BEFORE ADDTASK_END
%token FAKEROOT PYTHON
%token PREDOT_ASSIGN POSTDOT_ASSIGN PREPEND_ASSIGN APPEND_ASSIGN
       COLON_ASSIGN QUES_ASSIGN LAZYQUES_ASSIGN ASSIGN
%token EOL STRING_CONTINUATION BLOCK_END

%type <ival> assign_op
%type <sval> block_expr kw_expr
%type <list> recipe statements
%type <hashtable> addtask_stmt addtask_expr block_stmt conf_stmt export_stmt statement
                  kw_keyword kw_stmt

%start recipe

/*
%destructor { printf("\nfreeing\n"); g_free($$); $$ = NULL; } <sval>
%destructor { g_list_free_full($$, (GDestroyNotify) g_hash_table_unref); $$ = NULL; } <list>
*/

%%

recipe:
	statements { *acc = g_list_copy($1); $$ = $1;
	printf("\nLENGTH '%d'\n", g_list_length($1));
	}
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
	| addtask_stmt { $$ = $1; }
	| export_stmt { $$ = $1; }
	;

export_stmt:
	EXPORT conf_stmt {
		$$ = add_int($2, "export", 1); }
	| EXPORT kw_expr {
		$$ = new(kw);
		add_int($$, "keyword", $1);
		add_str($$, "expr", $2);
		add_int($$, "export", 1); }
	;

block_stmt:
	BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $1, $2, 0, 0); }
	| FAKEROOT BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $2, $3, 1, 0); }
	| PYTHON BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $2, $3, 0, 1); }
	| FAKEROOT PYTHON BLOCK_START block_expr BLOCK_END {
		$$ = new(block);
		block_set($$, $3, $4, 1, 1); }
	;

block_expr:
	BLOCK_CONTENT { $$ = $1; }
	| BLOCK_CONTENT block_expr {
		$$ = g_strdup_printf("%s%s", $1, $2);
		g_free($1); g_free($2);}
	;

conf_stmt:
	WORD assign_op STRING {
		$$ = new(conf);
		conf_set($$, $1, $3, NULL, $2); }
	| WORD '[' WORD ']' assign_op STRING {
		$$ = new(conf);
		conf_set($$, $1, $6, $3, $5); }
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
	kw_keyword kw_expr {
		$$ = add_str($1, "expr", $2);
		add_int($$, "export", 0); }
	;

kw_keyword:
	DELTASK { $$ = new(kw); add_int($$, "keyword", $1); }
	| EXPORT_FUNCTIONS { $$ = new(kw); add_int($$, "keyword", $1); }
	| INCLUDE { $$ = new(kw); add_int($$, "keyword", $1); }
	| INHERIT { $$ = new(kw); add_int($$, "keyword", $1); }
	| REQUIRE { $$ = new(kw); add_int($$, "keyword", $1); }
	;

kw_expr:
	WORD { $$ = $1; }
	| WORD kw_expr { $$ = g_strdup_printf("%s %s", $1, $2); }
	| WORD STRING_CONTINUATION kw_expr {
		$$ = g_strdup_printf("%s %s", $1, $3); }
	;

addtask_stmt:
	ADDTASK addtask_expr ADDTASK_END { $$ = $2; }
	;

addtask_expr:
	ADDTASK_CONTENT { $$ = new(addtask); add_str($$, "content", $1); }
	| addtask_expr ADDTASK_AFTER ADDTASK_CONTENT { add_str($1, "after", $3); }
	| addtask_expr ADDTASK_BEFORE ADDTASK_CONTENT { add_str($1, "before", $3); }
	;

%%
