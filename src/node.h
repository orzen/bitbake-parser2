#ifndef NODE_H

#include <glib.h>
#include "parser_types.h"

struct node_int {
	//enum node_type type;
	gint type;
	gint lineno;
	void *data;
};

GNode* new_int(enum node_type type, gint data_int);
GNode* new_str(enum node_type type, const gchar *data_str);
GNode* append_int(GNode *parent, enum node_type type, gint data_int);
GNode* append_str(GNode *parent, enum node_type type, const gchar *data_str);
GNode *append_node(GNode *parent, GNode *child);

void node_print_tree(GNode *root);

#endif
