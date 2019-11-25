#ifndef LEXER_H
#define LEXER_H

#include <glib.h>

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

typedef union {
	gint ival;
	gchar *sval;
	GNode *node;
} YYSTYPE;

int yyget_lineno(yyscan_t yyscanner);
int yylex(YYSTYPE *yylval, yyscan_t yyscanner);
void yyerror(yyscan_t yyscanner, GNode **acc, const gchar *err);
int yyparse(yyscan_t yyscanner, GNode **acc);

GNode* parse_file(const gchar *fn);

#endif
