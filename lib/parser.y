%{
#include <stdio.h>
#include <glib.h>

#include "args.h"
#include "files.h"
#include "node.h"
#include "parser_types.h"

extern gint row_num;
%}

%define api.pure full
%define parse.error verbose

%code requires {
#include <glib.h>
#include "lexer.h"
}
%verbose

%lex-param {yyscan_t yyscanner}
%parse-param {yyscan_t yyscanner} {GNode **acc}

/* Block statements */
%token FAKEROOT PYTHON EXPORT PREDOT_ASSIGN POSTDOT_ASSIGN
       PREPEND_ASSIGN APPEND_ASSIGN COLON_ASSIGN QUES_ASSIGN LAZYQUES_ASSIGN
       ASSIGN ADDTASK ADDTASK_AFTER ADDTASK_BEFORE UNSET
%token <sval> ADDTASK_CONTENT BLOCK_START BLOCK_CONTENT SLI_CONTENT
              MLI_CONTENT WORD STRING STRING_CONTINUATION
%token <ival> SINGLE_QUOTE DOUBLE_QUOTE INHERIT INCLUDE REQUIRE DELTASK
              EXPORT_FUNCTIONS

%type <ival> sli_id
%type <sval> block_expr mli_expr
%type <node> addtask_stmt addtask_expr block_stmt conf_stmt export_stmt
             assign_op sli_stmt mli_id mli_stmt recipe statements statement

%start recipe

/*
%destructor { printf("\nfreeing\n"); g_free($$); $$ = NULL; } <sval>
%destructor { g_list_free_full($$, (GDestroyNotify) g_hash_table_unref); $$ = NULL; } <list>
*/

%%

recipe:
	statements { *acc = $1; }
	;

statements:
	statement { $$ = new_str(root, "root");
	            $$ = append_node($$, $1); }
	| statement statements { $$ = append_node($2, $1); }
	;

statement:
	block_stmt { $$ = $1; }
	| sli_stmt { $$ = $1; }
	| mli_stmt { $$ = $1; }
	| addtask_stmt { $$ = $1; }
	| export_stmt { $$ = $1; }
	| conf_stmt { $$ = $1; }
	;

block_stmt:
	BLOCK_START block_expr {
		$$ = new_str(func, $1);
		$$ = append_str($$, body, $2);
		$$ = append_int($$, python, 0);
		$$ = append_int($$, fakeroot, 0); }
	| FAKEROOT BLOCK_START block_expr {
		$$ = new_str(func, $2);
		$$ = append_str($$, body, $3);
		$$ = append_int($$, python, 0);
		$$ = append_int($$, fakeroot, 1); }
	| PYTHON BLOCK_START block_expr {
		$$ = new_str(func, $2);
		$$ = append_str($$, body, $3);
		$$ = append_int($$, python, 1);
		$$ = append_int($$, fakeroot, 0); }
	| FAKEROOT PYTHON BLOCK_START block_expr {
		$$ = new_str(func, $3);
		$$ = append_str($$, body, $4);
		$$ = append_int($$, python, 1);
		$$ = append_int($$, fakeroot, 1); }
	;

block_expr:
	BLOCK_CONTENT { $$ = $1; }
	| BLOCK_CONTENT block_expr {
		$$ = g_strdup_printf("%s%s", $1, $2);
		g_free($1); g_free($2); }
	;

conf_stmt:
	WORD assign_op STRING {
		$$ = append_str($2, var, $1);
		$$ = append_str($$, body, $3); }
	| WORD '[' WORD ']' assign_op STRING {
		$$ = append_str($5, var, $1);
		$$ = append_str($$, body, $6);
		$$ = append_str($$, flag, $3); }
	;

assign_op:
	PREDOT_ASSIGN { $$ = new_int(conf, predot_assign); }
	| POSTDOT_ASSIGN { $$ = new_int(conf, postdot_assign); }
	| PREPEND_ASSIGN { $$ = new_int(conf, prepend_assign); }
	| APPEND_ASSIGN { $$ = new_int(conf, append_assign); }
	| COLON_ASSIGN { $$ = new_int(conf, colon_assign); }
	| QUES_ASSIGN { $$ = new_int(conf, ques_assign); }
	| LAZYQUES_ASSIGN { $$ = new_int(conf, lazyques_assign); }
	| ASSIGN { $$ = new_int(conf, assign); }
	;

export_stmt:
	EXPORT conf_stmt { $$ = append_int($2, exp_var, 1); }
	| EXPORT WORD { $$ = new_str(exported, $2); }
	;

sli_stmt:
	sli_id SLI_CONTENT { $$ = new_str($1, $2); }
	| sli_id SLI_CONTENT '[' SLI_CONTENT ']' {
		$$ = new_str($1, $2);
		$$ = append_str($$, flag, $4); }
	;

sli_id:
	UNSET { $$ = unset; }
	;

mli_stmt:
	mli_id mli_expr {
		$$ = append_str($1, body, $2); }
	;

mli_id:
	INHERIT { $$ = new_str($1, "inherit"); }
	| INCLUDE { $$ = new_str($1, "include"); }
	| REQUIRE { $$ = new_str($1, "require"); }
	| DELTASK { $$ = new_str($1, "deltask"); }
	| EXPORT_FUNCTIONS { $$ = new_str($1, "export_funcs"); }
	;

mli_expr:
	MLI_CONTENT { $$ = $1;}
	| MLI_CONTENT STRING_CONTINUATION mli_expr {
		$$ = g_strdup_printf("%s%s%s", $3, $2, $1);
		g_free($1); g_free($2); g_free($3);}

addtask_stmt:
	ADDTASK addtask_expr { $$ = $2; }
	;

addtask_expr:
	ADDTASK_CONTENT { $$ = new_str(addtask, $1); }
	| addtask_expr ADDTASK_AFTER ADDTASK_CONTENT { $$ = append_str($1, after, $3); }
	| addtask_expr ADDTASK_BEFORE ADDTASK_CONTENT { $$ = append_str($1, before, $3); }
	;

%%

#if 0
sli_expr:
	WORD { $$ = $1; }
	| WORD sli_expr { $$ = g_strdup_printf("%s %s", $1, $2); }
	;

mli_expr:
	WORD { $$ = $1; }
	| WORD mli_expr { $$ = g_strdup_printf("%s %s", $1, $2); }
	| WORD STRING_CONTINUATION mli_expr {
		$$ = g_strdup_printf("%s %s", $1, $3); }
	;
#endif
