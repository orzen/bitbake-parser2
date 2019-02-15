#include <glib.h>
#include <stdio.h>

#include "convert.h"
#include "parser_types.h"
#include "pyobj_ast.h"

struct type_info {
	const gchar *name;
	AstHandleFunc ast_callback;
	gint ast_num_args;
};

static struct type_info type_table[] = {
	{"root", NULL, 0},
	{"conf", ast_handle_data, 4},
	{"func", ast_handle_method, 6},
	{"inherit", ast_handle_inherit, 4},
	{"include", ast_handle_include, 5},
	{"require", ast_handle_include, 5},
	{"addtask", ast_handle_addtask, 4},
	{"deltask", ast_handle_deltask, 4},
	{"addhandler", ast_handle_addhandler, 4},
	{"export_funcs", ast_handle_export_funcs, 5},
	{"unset", ast_handle_unset, 4},
	{"exported", ast_handle_export, 4},
	{"body", NULL, 0},
	{"var", NULL, 0},
	{"flag", NULL, 0},
	{"exp", NULL, 0},
	{"apo", NULL, 0},
	{"python", ast_handle_python_method, 6},
	{"fakeroot", NULL, 0},
	{"after", NULL, 0},
	{"before", NULL, 0},
	{NULL, NULL, 0} /* sentinel */
};

static const gchar *op_table[] = {
	"predot",
	"postdot",
	"prepend",
	"append",
	"colon",
	"ques",
	"lazyques",
	"assign"
};

AstHandleFunc types_get_ast_callback(enum node_type type) {
	return type_table[type].ast_callback;
}

gint types_get_ast_num_args(enum node_type type) {
	return type_table[type].ast_num_args;
}

const gchar* types_itoa(enum node_type type) {
	return type_table[type].name;
}

const gchar* types_op_itoa(enum op_type op) {
	return op_table[op];
}
