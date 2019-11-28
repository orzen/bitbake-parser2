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
		log_warn("empty args");
		Py_RETURN_NONE;
	}

	r = PyArg_ParseTuple(args, "O", &index);
	if (r != Py_True) {
		log_warn("failed to parse args");
		Py_RETURN_NONE;
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
			Py_RETURN_NONE;
		}
	} else {
		value = PyDict_GetItem(self->dict, index);
		if (value == NULL) {
			log_warn("failed to get match value (dict)");
			Py_RETURN_NONE;
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
	Py_INCREF(self->dict);
	self->list = list;
	Py_INCREF(self->list);

	return 0;
}

void Match_dealloc(struct match_data *self) {
	log_dbg("__DEALLOC__");

	if (self->dict) {
		Py_DECREF(self->dict);
	}
	log_dbg("__DEALLOC__1");
	if (self->list) {
		Py_DECREF(self->list);
	}
	log_dbg("__DEALLOC__2");

	Py_TYPE(self)->tp_free((PyObject*) self);
	log_dbg("__DEALLOC__3");
}

PyObject* Match_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	struct match_data *self = NULL;

	log_dbg("__NEW__");

	self = (struct match_data *) type->tp_alloc(type, 0);
	if (self == NULL) {
		log_warn("failed to allocate match_data");
	}
#if 0
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
#endif

	return (PyObject*) self;
}

PyObject* Match_new_int(PyObject *dict, PyObject *list) {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;

	match_args = Py_BuildValue("(OO)",
	                           dict ? dict : Py_None,
	                           list ? list : Py_None);
	match_kwds = PyDict_New();

	match_obj = Match_new(&Match_type, match_args, match_kwds);

	Py_DECREF(match_args);
	Py_DECREF(match_kwds);

	return match_obj;
}
