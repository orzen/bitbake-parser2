#ifndef _PARSER_TYPES_H_
#define _PARSER_TYPES_H_

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

#endif
