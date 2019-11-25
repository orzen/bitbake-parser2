#include <glib.h>
#include <stdio.h>

#include "lexer.h"
#include "node.h"

gint main(gint argc, gchar **argv) {
	gint i = 0;

	for (i = 1; i < argc; i++) {
		GNode *root = NULL;
		printf("arg[%d]: '%s'\n", i, argv[i]);
		root = parse_file(argv[i]);
		node_print_tree(root);
	}
}
