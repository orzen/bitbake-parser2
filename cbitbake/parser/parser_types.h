#ifndef _PARSER_TYPES_H_
#define _PARSER_TYPES_H_

#include <glib.h>

enum node_type {
	root = 0,
	conf,
	func,
	inherit,
	include,
	require,
	addtask,
	deltask,
	addhandler,
	export_funcs,
	unset,
/* === Sub-types ===  */
	body,
/* Assign-types */
	var,
	flag,
	exported,
/* Func-types */
	python,
	fakeroot,
/* Addtask-types */
	after,
	before
};

enum assign_op {
	predot_assign = 0,
	postdot_assign,
	prepend_assign,
	append_assign,
	colon_assign,
	ques_assign,
	lazyques_assign,
	assign
};

enum quote_type {
	single_quote = 0,
	double_quote
};

const gchar* types_itoa(enum node_type type);
gchar* cbb_types_assign_op_itoa(enum assign_op op);

#endif
