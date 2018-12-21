#include <glib.h>
#include <stdio.h>

#include "lexer.h"

gint main(gint argc, gchar **argv) {
	gint i = 0;

	for (i = 1; i < argc; i++) {
		printf("arg[%d]: '%s'\n", i, argv[i]);
		cbb_parse_file(argv[i]);
	}
}
