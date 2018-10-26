%{
#include <stdio.h>
#include <stdlib.h>

#include "files.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int row_num;
extern int col_num;

void yyerror(const char *error_str);

%}

%define parse.error verbose
%define parse.trace

%union {
	char *var;
	int ival;
	float fval;
	char *vval;
	char *sval;
	char *op;
}

%token INHERIT

%token STRING
%token <ival> INT_LITERAL
%token <fval> FLOAT_LITERAL
%token <vval> VAR_LITERAL
%token <sval> STRING_LITERAL

%token ASSIGN ADD_ASSIGN
%token EOL STRING_CONTINUATION



%start bitbake

%destructor { free($$); } <sval>

%%

bitbake:
	expressions EOL {printf("\nexpressions\n");}
	;

expressions:
	expression
	| expressions EOL expression {printf("\nexpr & expr\n");}
	;

expression:
	assignment_expr {printf("\nassign\n");}
	| inherit_expr {printf("\ninherit\n");}
	;

inherit_expr:
	INHERIT string_list
	;

string_list:
	STRING
	| string_list STRING
	| string_list STRING_CONTINUATION STRING
	;

assignment_expr:
	STRING ASSIGN term
	;

term:
	STRING
	| STRING_LITERAL
	| INT_LITERAL
	| FLOAT_LITERAL
	| VAR_LITERAL
	;

%%

void yyerror(const char *error_str) {
	printf("\nParse error at '%d: %d': '%s'\n", ++row_num, ++col_num,
	       error_str);
	exit(-1);
}

int main(int argc, char **argv) {
	FILE *file = NULL;
	int r = -1;
	char *buf = NULL;
	size_t buf_s = 0;

	int i = 0;
	for (i = 0; i < argc; i++) {
		printf("arg: '%s'\n", argv[0]);
	}

	file = fopen("libconfutils_1.3.4.bb", "r");
	if (file == NULL) {
		fprintf(stderr, "Failed to open file\n");
		exit(1);
	}

	yyin = file;

	do {
		yyparse();
	} while (!feof(yyin));
}
