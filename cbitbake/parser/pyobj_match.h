#ifndef PYOBJ_MATCH_H
#define PYOBJ_MATCH_H

#include <glib.h>
#include <Python.h>

PyObject* Match_new_int();

gint Match_set_list(PyObject *self, PyObject *list);
gint Match_set_dict(PyObject *self, PyObject *dict);

#endif
