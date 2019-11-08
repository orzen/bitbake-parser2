#ifndef CONVERT_H
#define CONVERT_H

#include <glib.h>

#include "pyobj_ast.h"
#include "types.h"

AstHandleFunc types_get_ast_callback(enum node_type type);
gint types_get_ast_num_args(enum node_type type);
const gchar* types_itoa(enum node_type type);
const gchar* types_op_itoa(enum op_type op);

#endif
