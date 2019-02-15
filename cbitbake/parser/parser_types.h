#ifndef _PARSER_TYPES_H_
#define _PARSER_TYPES_H_

#include <glib.h>

#include "pyobj_ast.h"

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
	exported,
/* === Sub-types ===  */
	body,
/* Assign-types */
	var,
	flag,
	exp_var,
	apo,
/* Func-types */
	python,
	fakeroot,
/* Addtask-types */
	after,
	before
};

enum op_type {
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

AstHandleFunc types_get_ast_callback(enum node_type type);
gint types_get_ast_num_args(enum node_type type);
const gchar* types_itoa(enum node_type type);
const gchar* types_op_itoa(enum op_type op);

#endif
