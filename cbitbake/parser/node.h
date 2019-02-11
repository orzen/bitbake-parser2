#ifndef NODE_H

#include <glib.h>

enum node_type {
	root = 1,
	func,
	inherit,
	include,
	require,
	addtask,
	deltask,
	export_funcs,
	unset,
/* === Sub-types ===  */
	body,
/* Assign-types */
	op,
	var,
	flag,
	exported,
/* Func-types */
	python,
	fakeroot,
/* Addtask-types */
	after,
	before,
};

GNode* new_int(enum node_type type, gint data_int);
GNode* new_str(enum node_type type, const gchar *data_str);
GNode* append_int(GNode *parent, enum node_type type, gint data_int);
GNode* append_str(GNode *parent, enum node_type type, const gchar *data_str);
GNode *append_node(GNode *parent, GNode *child);


#endif
