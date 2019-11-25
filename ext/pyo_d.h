#ifndef PYOBJ_D
#define PYOBJ_D

#include <glib.h>
#include <Python.h>

void pyo_d_init(PyObject *d);
PyObject* pyo_d_get_var(PyObject *d, const gchar *var, gboolean expand);
void pyo_d_set_var(PyObject *d, const gchar *var, const gchar *value);

#endif
