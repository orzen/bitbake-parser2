#ifndef PYOBJ_MATCH_H
#define PYOBJ_MATCH_H

#include <Python.h>

struct match_data {
	PyObject_HEAD
	PyObject *dict;
	PyObject *list;
} MatchData;

PyObject* Match_group(struct match_data *self, PyObject *args);
PyObject* Match_groupdict(struct match_data *self);

gint Match_init(struct match_data *self, PyObject *args, PyObject *kwds);
PyObject* Match_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
void Match_dealloc(struct match_data *self);

PyTypeObject Match_type;

PyObject* Match_new_int(PyObject *dict, PyObject *list);

#endif
