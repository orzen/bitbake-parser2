#include <glib.h>
#include <stdio.h>

#include "convert.h"
#include "parser_types.h"
#include "pyobj_ast.h"

#if 0
#define TYPE_NEW(TYPE) \
struct cbb_##TYPE##_s* cbb_##TYPE##_s_new() {\
	return calloc(1, sizeof(struct cbb_##TYPE##_s));\
}

TYPE_NEW(conf)
TYPE_NEW(kw)
TYPE_NEW(block)
TYPE_NEW(gen)
#undef TYPE_NEW
#endif

struct type_info {
	const gchar *name;
	void *ast_callback;
};

static struct type_info type_table[] = {
	{"root", NULL},
	{"conf", &ast_handle_data},
	{"func", &ast_handle_method},
	{"inherit", &ast_handle_inherit},
	{"include", &ast_handle_include},
	{"require", &ast_handle_include},
	{"addtask", &ast_handle_addtask},
	{"deltask", &ast_handle_deltask},
	{"addhandler", &ast_handle_addhandler},
	{"export_funcs", &ast_handle_export_funcs},
	{"unset", &ast_handle_unset},
	{"body", NULL},
	{"var", NULL},
	{"flag", NULL},
	{"exported", &ast_handle_export},
	{"python", NULL, NULL, &ast_handle_python_method},
	{"fakeroot", NULL},
	{"after", NULL},
	{"before", NULL},
	{NULL, NULL, NULL, NULL} /* sentinel */
};

void* types_get_ast_callback(enum node_type type) {
	return type_table[type].ast_callback;
}

const gchar* types_itoa(enum node_type type) {
	return type_table[type].name;
}

gchar* cbb_types_assign_op_itoa(enum assign_op op) {
	gchar *str = NULL;
	switch(op) {
	case predot_assign:
		str = g_strdup("predot");
		break;
	case postdot_assign:
		str = g_strdup("postdot");
		break;
	case prepend_assign:
		str = g_strdup("prepend");
		break;
	case append_assign:
		str = g_strdup("append");
		break;
	case colon_assign:
		str = g_strdup("colon");
		break;
	case ques_assign:
		str = g_strdup("ques");
		break;
	case lazyques_assign:
		str = g_strdup("lazyques");
		break;
	case assign:
		str = g_strdup("assign");
		break;
	};

	return str;
}
