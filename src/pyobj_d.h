#ifndef PYOBJ_D
#define PYOBJ_D

#include <glib.h>
#include <Python.h>

PyObject* cbb_d_get_var(PyObject *d, const gchar *var, gboolean expand);
void cbb_d_set_var(PyObject *d, const gchar *var, const gchar *value);

#endif
