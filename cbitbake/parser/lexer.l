%{
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"

gint row_num = 1;
gint col_num = 1;
void count();
%}
%option noyywrap debug

WORD [a-zA-Z0-9_\.'\$\{\}-]
/* ES [a-zA-Z\.@/0-9_;\*-] */
%x BLOCK

%%

<BLOCK>^[\}]        { count(); BEGIN(INITIAL); return BLOCK_END; }
<BLOCK>[[:print:]]+ { count(); yylval.sval = g_strdup(yytext);
                      return BLOCK_CONTENT; }
<BLOCK>[[:space:][:blank:]]+  {
                      count(); yylval.sval = g_strdup(yytext);
                      return BLOCK_CONTENT; }
{WORD}+[[:space:]]*"()"[[:space:]]*"{"$ {
                      count(); yylval.sval = g_strdup(yytext); BEGIN(BLOCK);
                      return BLOCK_START; }

"#".*[\n]           { count(); }
\"\"\"[^"]*\"\"\"   { count(); }

"export"            { count(); return EXPORT; }
"inherit"           { count(); return INHERIT; }
"include"           { count(); return INCLUDE; }
"require"           { count(); return REQUIRE; }
"python"            { count(); return PYTHON; }

"="                 { count(); return ASSIGN; }
"=."                { count(); return PREDOT_ASSIGN; }
".="                { count(); return POSTDOT_ASSIGN; }
"=+"                { count(); return PREPEND_ASSIGN; }
"+="                { count(); return APPEND_ASSIGN; }
":="                { count(); return COLON_ASSIGN; }
"?="                { count(); return QUES_ASSIGN; }
"??="               { count(); return LAZYQUES_ASSIGN; }

"\\\n"+             { count(); return STRING_CONTINUATION; }
"\n"+               { count(); return EOL; }

[\[\]\(\)\{\}]      { count(); return yytext[0]; }
[ \t\v\f]+          { count(); }

\"(\\.|[^"])*\" |
'(\\.|[^'])*'       { count(); yylval.sval = g_strdup(yytext); return STRING; }
{WORD}+             { count(); yylval.sval = g_strdup(yytext); return WORD; }

.                   { fprintf(stderr, "\nUnrecognized character '%s', '%d: %d'\n", yytext, row_num, col_num); exit(-1); }


%%
// (.|[\$\n/@,:;\*!&%\<\>])+   { count(); yylval.sval = g_strdup(yytext); return BLOCK_CONTENT; }

// "python"[[:space:]]+{WORD}+[[:space:]]*"()"[[:space:]]*"{"$
//                    { count(); BEGIN(block); return PYTHON_BLOCK; }
//^[\}]              { count(); yylval.sval = g_strdup(yytext); return BLOCK_END; }

void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n') {
			col_num = 0;
			row_num++;
			yylineno++;
		} else if (yytext[i] == '\t') {
			col_num += 8 - (col_num % 8);
		} else {
			col_num++;
		}
	}

	ECHO;
}
