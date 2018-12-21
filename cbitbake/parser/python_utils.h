#ifndef PYTHON_UTILS
#define PYTHON_UTILS

#include <glib.h>
#include <Python.h>

void cbb_print_pyobj(const gchar *filename, PyObject *o);
void cbb_init(PyObject *d);
gchar* cbb_resolve_file(const gchar *filename, PyObject *d);

#endif
