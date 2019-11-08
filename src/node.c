#include <stdio.h>
#include <glib.h>

#include "node.h"
#include "parser_types.h"

extern gint row_num;

static struct node_int* node_internal_new(enum node_type type,
                                          void *data) {
	struct node_int *internal = NULL;

	internal = calloc(1, sizeof(struct node_int));
	if (internal == NULL) {
		g_warning("failed to allocate node internals");
		return NULL;
	}

	internal->type = type;
	internal->lineno = row_num;
	internal->data = data;

	return internal;
}

#if 0
void free_node(GNode node) {
	struct node_int *internal = NULL;

	if (node == NULL) {
		return;
	}

	if (node->data = NULL) {
		
	}
}
#endif

static GNode* new(enum node_type type, void *data) {
	GNode *node = NULL;
	struct node_int *internal = NULL;

	if (data == NULL) {
		g_warning("empty data, exiting");
		return NULL;
	}

	internal = node_internal_new(type, data);
	if (internal == NULL) {
		return NULL;
	}

	node = g_node_new(internal);
	if (node == NULL) {
		g_warning("failed to create new node");
		return NULL;
	}

	return node;
}

GNode* new_int(enum node_type type, gint data_int) {
	gint *integer = NULL;

	integer = calloc(1, sizeof(gint));
	if (integer == NULL) {
		g_warning("failed to allocate integer");
		return NULL;
	}

	*integer = data_int;

	return new(type, integer);
}

GNode* new_str(enum node_type type, const gchar *data_str) {
	gchar *string = NULL;

	if (data_str == NULL) {
		g_warning("failed to create node, string is NULL");
		return NULL;
	}

	string = g_strdup(data_str);
	if (string == NULL) {
		g_warning("failed to duplicate string");
		return NULL;
	}

	return new(type, string);
}

GNode* append_int(GNode *parent, enum node_type type, gint data_int) {
	GNode *child = NULL;

	if (parent == NULL) {
		g_warning("unspecified parent");
		return NULL;
	}

	child = new_int(type, data_int);
	if (child == NULL) {
		g_warning("failed to create child node");
		return NULL;
	}

	g_node_append(parent, child);

	return parent;
}

GNode* append_str(GNode *parent, enum node_type type, const gchar *data_str) {
	GNode *child = NULL;

	if (parent == NULL) {
		g_warning("unspecified parent");
		return NULL;
	}

	child = new_str(type, data_str);
	if (child == NULL) {
		g_warning("failed to create child node");
		return NULL;
	}

	g_node_append(parent, child);

	return parent;
}

GNode *append_node(GNode *parent, GNode *child) {
	if (parent == NULL || child == NULL) {
		g_warning("failed to append node, empty args");
		return NULL;
	}

	g_node_prepend(parent, child);

	return parent;
}

gboolean node_debug_traverse_func(GNode *node, gpointer data) {
	gint depth = g_node_depth(node) * 2;
	gint i = 0;
	struct node_int *internal = node->data;

	printf("|");
	for (i = 0; i < depth; i++) {
		printf("_");
	}
	printf("%s:%d\n", types_itoa(internal->type), internal->lineno);

	return FALSE;
}

void node_print_tree(GNode *root) {
	if (root == NULL) {
		g_warning("root node is NULL");
		return;
	}

	g_node_traverse(root, G_PRE_ORDER, G_TRAVERSE_ALL, -1, node_debug_traverse_func, NULL);
}
