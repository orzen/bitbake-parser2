#ifndef _PARSER_TYPES_H_
#define _PARSER_TYPES_H_

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

#if 0
enum sli_type {
	export = 0,
	unset,
};

enum mli_type {
	inherit,
	include,
	require,
	deltask,
	export_funcs
};

enum parser_type {
	block = 0,
	conf,
	sli,
	mli,
	addtask,
};
#endif

#endif
