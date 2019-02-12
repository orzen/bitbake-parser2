#ifndef CONVERT_H
#define CONVERT_H

#include <glib.h>
#include <Python.h>

PyObject* convert_ast_to_python(GNode *root);
PyObject* convert_seq_to_pylist(gchar *fmt, ...);

#endif
