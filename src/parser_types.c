#include <glib.h>
#include <stdio.h>

#include "bbcompat.h"
#include "parser_types.h"
#include "pyobj_ast.h"

struct type_info {
	const gchar *name;
	AstHandleFunc ast_callback;
	gint ast_num_args;
};

static struct type_info type_table[] = {
	{"root", NULL, 0},
	{"conf", ast_handleData, 4},
	{"func", ast_handleMethod, 6},
	{"inherit", ast_handleInherit, 4},
	{"include", ast_handleInclude, 5},
	{"require", ast_handleInclude, 5},
	{"addtask", ast_handleAddTask, 4},
	{"deltask", ast_handleDelTask, 4},
	{"addhandler", ast_handleBBHandlers, 4},
	{"export_funcs", ast_handleExportFuncs, 5},
	{"unset", ast_handleUnset, 4},
	{"exported", ast_handleExport, 4},
	{"body", NULL, 0},
	{"var", NULL, 0},
	{"flag", NULL, 0},
	{"exp", NULL, 0},
	{"apo", NULL, 0},
	{"python", ast_handlePythonMethod, 6},
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
