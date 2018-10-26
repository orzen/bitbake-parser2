%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitbake.tab.h"

int row_num = 1;
int col_num = 0;
void count();
%}
%option noyywrap debug

D [0-9]
L [a-zA-Z_-]
stringtext ([^"])

%%

inherit                  { count(); return INHERIT; }
[ \t\v\f]+               { count(); }
[\\\\n]+                 { count(); return STRING_CONTINUATION;}
[\n]+                    { count(); return EOL; }
{D}+"."{D}+              { count();
                           yylval.fval = atof(yytext);
                           return FLOAT_LITERAL; }
{D}+                     { count(); yylval.ival = atoi(yytext); return
                           INT_LITERAL; }
\$\{[a-zA-Z0-9_-]+\}     { count(); yylval.vval = strdup(yytext); return
                           VAR_LITERAL; }
\"{stringtext}+\"        { count();  yylval.sval = strdup(yytext);
                           return STRING_LITERAL; }
{L}+                     { count(); return STRING; }
"="                      { count(); return ASSIGN; }
"+="                     { count(); return ADD_ASSIGN; }
.                        { fprintf(stderr, "\nUnrecognized character '%s', '%d: %d'\n", yytext, row_num, col_num); exit(-1); }

%%

void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			col_num = 0;
		else if (yytext[i] == '\t')
			col_num += 8 - (col_num % 8);
		else
			col_num++;

	ECHO;
}
