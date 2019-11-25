#ifndef BBCOMPAT_H
#define BBCOMPAT_H

#include <glib.h>
#include <Python.h>

#include "pyo_ast2.h"

gint compat_call_bbast(struct ast *ast,
                       GNode *root,
                       PyObject *statements,
                       const gchar *filename);

#endif
