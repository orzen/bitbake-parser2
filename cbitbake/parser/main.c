#include <glib.h>
#include <stdio.h>

#include "convert.h"
#include "lexer.h"
#include "node.h"

gint main(gint argc, gchar **argv) {
	gint i = 0;

	for (i = 1; i < argc; i++) {
		GNode *root = NULL;
		printf("arg[%d]: '%s'\n", i, argv[i]);
		root = cbb_parse_file(argv[i]);
		//node_print_tree(root);
		convert_ast_to_python(root);
	}
}
