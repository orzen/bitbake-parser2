%{
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "lexer.h"
#include "node.h"
#include "parser.tab.h"
#include "utils.h"

gint row_num = 1;
gint col_num = 1;
gchar *filename;

void count(yyscan_t scanner);
gchar* duptext(yyscan_t scanner);

%}
%option noyywrap bison-bridge reentrant stack debug

WORD [a-zA-Z0-9_\.'\$\{\}-]

%x BLOCK
%x ADDTASK
%x SLI
%x MLI

%%

<BLOCK>^[\}]        { count(yyscanner);
                      yy_pop_state(yyscanner); }
<BLOCK>[[:print:]]+ { count(yyscanner);
                      yylval->sval = duptext(yyscanner);
                      return BLOCK_CONTENT; }
<BLOCK>[[:space:][:blank:]]+ {
                      count(yyscanner);
                      yylval->sval = duptext(yyscanner);
                      return BLOCK_CONTENT; }
{WORD}+[[:space:]]*"()"[[:space:]]*"{"$ {
                        count(yyscanner);
                        yylval->sval = duptext(yyscanner);
                        yy_push_state(BLOCK, yyscanner);
                        return BLOCK_START; }

<ADDTASK>"after"      { count(yyscanner); return ADDTASK_AFTER; }
<ADDTASK>"before"     { count(yyscanner); return ADDTASK_BEFORE; }
<ADDTASK>\n           { count(yyscanner);
                        yy_pop_state(yyscanner); }
<ADDTASK>[[:space:]]+ { count(yyscanner); }
<ADDTASK>[[:print:]]+ { count(yyscanner);
                        yylval->sval = duptext(yyscanner);
                        return ADDTASK_CONTENT; }
"addtask"             { count(yyscanner);
                        yy_push_state(ADDTASK, yyscanner);
                        return ADDTASK; }

"#".*[\n]           { count(yyscanner); }
\"\"\"[^"]*\"\"\"   { count(yyscanner); }

"fakeroot"          { count(yyscanner); return FAKEROOT; }
"python"            { count(yyscanner); return PYTHON; }
"export"            { count(yyscanner); return EXPORT; }

<SLI>\n             { count(yyscanner);
                      yy_pop_state(yyscanner); }
<SLI>[[:space:]]+   { count(yyscanner); }
<SLI>[[:print:]]+   { count(yyscanner);
                      yylval->sval = duptext(yyscanner);
                      return SLI_CONTENT; }
"unset"             { count(yyscanner);
                      yy_push_state(SLI, yyscanner);
                      return UNSET; }

<MLI>[[:print:]]+   { count(yyscanner);
                      yylval->sval = duptext(yyscanner);
                      return MLI_CONTENT; }
<MLI>"\\\n"+        { count(yyscanner);
                      yylval->sval = duptext(yyscanner);
                      return STRING_CONTINUATION; }
<MLI>\n\n           { count(yyscanner);
                      yy_pop_state(yyscanner); }
<MLI>[[:space:]]+   { count(yyscanner); }
"inherit"           { count(yyscanner);
                      yylval->ival = inherit;
                      yy_push_state(MLI, yyscanner);
                      return INHERIT; }
"include"           { count(yyscanner);
                      yylval->ival = include;
                      yy_push_state(MLI, yyscanner);
                      return INCLUDE; }
"require"           { count(yyscanner);
                      yylval->ival = require;
                      yy_push_state(MLI, yyscanner);
                      return REQUIRE; }
"deltask"           { count(yyscanner);
                      yylval->ival = deltask;
                      yy_push_state(MLI, yyscanner);
                      return DELTASK; }
"EXPORT_FUNCTIONS"  { count(yyscanner);
                      yylval->ival = export_funcs;
                      yy_push_state(MLI, yyscanner);
                      return EXPORT_FUNCTIONS; }

"="                 { count(yyscanner); return ASSIGN; }
"=."                { count(yyscanner); return PREDOT_ASSIGN; }
".="                { count(yyscanner); return POSTDOT_ASSIGN; }
"=+"                { count(yyscanner); return PREPEND_ASSIGN; }
"+="                { count(yyscanner); return APPEND_ASSIGN; }
":="                { count(yyscanner); return COLON_ASSIGN; }
"?="                { count(yyscanner); return QUES_ASSIGN; }
"??="               { count(yyscanner); return LAZYQUES_ASSIGN; }

"\\\n"+             { count(yyscanner); return STRING_CONTINUATION; }
"\n"+               { count(yyscanner); }

[\[\]\(\)\{\}]      { count(yyscanner); return yytext[0]; }
[ \t\v\f]+          { count(yyscanner); }

\"(\\.|[^"])*\" |
'(\\.|[^'])*'       { count(yyscanner); yylval->sval = duptext(yyscanner); return STRING; }
{WORD}+             { count(yyscanner); yylval->sval = duptext(yyscanner); return WORD; }

.                   { fprintf(stderr, "\nUnrecognized character '%s', '%d: %d'\n", yytext, row_num, col_num); exit(-1); }
%%

gchar* duptext(yyscan_t scanner) {
	gchar *tmp = NULL;
	gint len = -1;
	gchar *yy_text = NULL;

	len = yyget_leng(scanner);
	yy_text = yyget_text(scanner);
	yy_text[len - 1] = '\0';

	tmp = g_strdup(yy_text);
	if (tmp == NULL) {
		cbb_fail("Failed to duplicate scanner text!");
	}

	return tmp;
}

void count(yyscan_t scanner) {
	gint i = 0;
	gint len = -1;
	gchar *text = NULL;

	len = yyget_leng(scanner);
	text = yyget_text(scanner);
	//text[len - 1] = '\0';

	for (i = 0; text[i] != '\0'; i++) {
		if (text[i] == '\n') {
			col_num = 1;
			row_num++;
		} else if (text[i] == '\t') {
			col_num += 8 - (col_num % 8);
		} else {
			col_num++;
		}
	}

	{
		FILE *out = NULL;
		size_t size = -1;

		out = yyget_out(scanner);
		size = fprintf(out, "%s", text);
		if (size != len) {
			printf("\nsize '%zu', len '%d'\n", size, len);
			cbb_fail("Failed to put back buffer content");
		}
	}
}

void yyerror(yyscan_t yyscanner, GNode **acc, const gchar *error_str) {
	printf("\nParse error at '%d: %d': '%s'\n", row_num, col_num,
	       error_str);
	exit(-1);
}

GNode* cbb_parse_file(const gchar *fn) {
	yyscan_t scanner;
	GNode *acc = NULL;
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

	r = yylex_init(&scanner);
	if (r != 0) {
		cbb_fail("Failed to initialize scanner!");
	}

	yyset_in(file, scanner);

	r = yyparse(scanner, &acc);
	if (r < 0) {
		cbb_fail("Failed to parse!");
	}

	fclose(file);
	file = NULL;

	yylex_destroy(scanner);

	if (acc == NULL) {
		g_warning("acc is NULL\n");
	} 

	return acc;
}
