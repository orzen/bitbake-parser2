#ifndef PYTHON_UTILS
#define PYTHON_UTILS

#include <glib.h>
#include <Python.h>

gint pyu_import_module(PyObject **obj, const gchar *module_name);
PyObject* pyutils_get_method(PyObject *module, const gchar *method_str);

void cbb_print_pyobj(const gchar *filename, PyObject *o);
void cbb_init(PyObject *d);
gchar* cbb_resolve_file(const gchar *filename, PyObject *d);

#endif
