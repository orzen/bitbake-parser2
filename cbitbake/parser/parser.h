#ifndef _PARSER_H_
#define _PARSER_H_

#include <glib.h>

union YYSTYPE {
	int ival;
	char *sval;
	GHashTable *hashtable;
	GList *list;
};

#endif
