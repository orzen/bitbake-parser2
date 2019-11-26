#include <stdarg.h>
#include <glib.h>
#include <Python.h>

#include "log.h"
#include "pyo_match.h"

PyObject* Match_group(struct match_data *self, PyObject *args) {
	PyObject *value = NULL;
	PyObject *index = NULL;
	gint act_list = 1;
	gint r = -1;

	if (self == NULL || args == NULL) {
		log_warn("empty args")
		return Py_None;
	}

	r = PyArg_ParseTuple(args, "O", &index);
	if (r != 0) {
		log_warn("failed to parse args");
		return Py_None;
	}

	act_list = PyLong_Check(index);

	if (PyUnicode_Check(index) == 1) {
		log_warn("ARG UNICODE");
	} else if (PyLong_Check(index) == 1) {
		log_warn("ARG LONG");
	} else if (PyBytes_Check(index) == 1) {
		log_warn("ARG BYTES");
	}

	if (act_list) {
		gint idx = PyLong_AsLong(index);

		value = PyList_GetItem(self->list, idx);
		if (value == NULL) {
			log_warn("failed to get match value (list)");
			return Py_None;
		}
	} else {
		value = PyDict_GetItem(self->dict, index);
		if (value == NULL) {
			log_warn("failed to get match value (dict)");
			return Py_None;
		}
	}

	return Py_BuildValue("O", value);
}

PyObject* Match_groupdict(struct match_data *self) {
	if (self == NULL) {
		log_warn("empty args");
		return Py_None;
	}

	return Py_BuildValue("O", self->dict);
}

gint Match_init(struct match_data *self, PyObject *args, PyObject *kwds) {
	PyObject *dict = NULL;
	PyObject *list = NULL;
	gint r = -1;

	log_dbg("__INIT__");

	if (args == NULL) {
		log_warn("empty args");
		return -1;
	}

	r = PyArg_ParseTuple(args, "OO", &dict, &list);
	if (!r) {
		log_warn("failed to parse args");
		return -1;
	}

	self->dict = dict;
	self->list = list;

	return 0;
}

void Match_dealloc(struct match_data *self) {
	log_dbg();

	Py_XDECREF(self->dict);
	Py_XDECREF(self->list);

	Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* Match_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	struct match_data *self = NULL;

	log_dbg("__NEW__");

	self = (struct match_data *) type->tp_alloc(type, 0);
	if (self != NULL) {
		self->dict = PyDict_New();
		if (self->dict == NULL) {
			log_warn("failed to allocate dict for Match object");
		}

		self->list = PyList_New(0);
		if (self->list == NULL) {
			log_warn("failed to allocate list for Match object");
		}
	}

	return (PyObject*) self;
}

PyMethodDef Match_methods[] = {
	{"group", (PyCFunction) Match_group, METH_VARARGS, "Stubbed alternative for Match.group"},
	{"groupdict", (PyCFunction) Match_groupdict, METH_NOARGS, "Stubbed alternative for Match.groupdict"},
	{} /* sentinel */
};

PyTypeObject Match_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "bbcparser.Match",
	.tp_basicsize = sizeof(struct match_data),
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_doc = "Stubbed version of re's Match-object",
	.tp_init = (initproc) Match_init,
	.tp_new = Match_new,
	.tp_alloc = PyType_GenericAlloc,
	.tp_dealloc = (destructor) Match_dealloc,
	.tp_methods = Match_methods,
};

#if 0
static PyObject* remock_compile(PyObject *self, PyObject *args) {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;
	gchar *pattern = NULL;
	gint r = -1;

	log_dbg("");

	r = PyArg_ParseTuple(args, "s", &pattern);
	if (r == 0) {
		PyErr_SetString(REMOCK_ERROR, "Failed to parse args");
		return NULL;
	}

	match_args = Py_BuildValue("");
	match_kwds = PyDict_New();

	match_obj = Match_new(&Match_type, match_args, match_kwds);

	Py_DECREF(match_args);
	Py_DECREF(match_kwds);

	if (match_obj == NULL) {
		PyErr_SetString(REMOCK_ERROR, "Failed to create a match object");
		return NULL;
	}

	return match_obj;
}

static PyObject* remock_match(PyObject *self, PyObject *args) {
	//gchar **tmp_arr = NULL;

	return NULL;
	tmp_arr = g_strsplit(tmp, " ", -1);
	if (tmp_arr == NULL) {
		return NULL;
	}

	if (self->string_arr != NULL) {
		g_strfreev(self->string_arr);
		self->string_arr = NULL;
	}

	self->string_arr = tmp_arr;
}
#endif


PyObject* Match_new_int(PyObject *dict, PyObject *list) {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;

	match_args = Py_BuildValue("OO",
	                           dict ? dict : Py_None,
	                           list ? list : Py_None);
	match_kwds = PyDict_New();

	match_obj = Match_new(&Match_type, match_args, match_kwds);

	Py_DECREF(match_args);
	Py_DECREF(match_kwds);

	return match_obj;
}
