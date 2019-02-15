#include <glib.h>
#include <Python.h>
#include <stdarg.h>

#include "log.h"

static const gchar *PACKAGE_VERSION = "0.0.1";
static PyObject *REMOCK_ERROR;

struct match_data {
	PyObject_HEAD
	PyObject *dict;
	PyObject *list;
} MatchData;

static PyObject* Match_group(struct match_data *self, PyObject *args) {
	gchar *value = NULL;
	gint idx = -1;
	gint r = -1;
	PyObject *value = NULL;
	PyObject *index = NULL;

	if (self == NULL || args == NULL) {
		PY_ERROR("one or more of the arguments is NULL");
		return Py_None;
	}

	r = PyArg_ParseTuple(args, "O", &index);
	if (r != 0) {
		PY_ERROR("failed to parse args");
		return Py_None;
	}

	if (PyUnicode_Check(index) == Py_True) {
		log_warn("ARG UNICODE");
	} else if (PyLong_Check(index) == Py_True) {
		log_warn("ARG LONG");
	} else if (PyBytes_Check(index) == Py_True) {
		log_warn("ARG BYTES");
	}

	if (idx < 0) {
		PY_ERROR("index out of range");
		return Py_None;
	}

	value = PyList_GetItem(self->list, idx);
	if (value == NULL) {
		PY_ERROR("value is NULL");
		return Py_None;
	}

	return Py_BuildValue("O", value);
}

gint Match_set_dict(PyObject *self, PyObject *dict) {
	if (self == NULL || dict == NULL) {
		log_warn("empty args");
		return -1;
	}

	self->dict = dict;

	return 0;
}

gint Match_set_list(PyObject *self, PyObject *list) {
	if (self == NULL || list == NULL) {
		log_warn("empty args");
		return -1;
	}

	self->list = list;

	return 0;
}

static void Match_dealloc(struct match_data *self) {
	DEBUG

	g_list_free_full(self->value_list, g_free);

	Py_XDECREF(self->value_obj);

	Py_XDECREF(self->dict);
	Py_XDECREF(self->list);
	Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* Match_new_int() {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;

	match_args = Py_BuildValue("");
	match_kwds = PyDict_New();

	match_obj = Match_new(&Match_type, match_args, match_kwds);

	Py_DECREF(match_args);
	Py_DECREF(match_kwds);

	return match_obj;
}

static PyObject* Match_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	struct match_data *self = NULL;

	g_warning("__NEW__");

	self = (struct match_data *) type->tp_alloc(type, 0);
	if (self != NULL) {
		self->value_obj = Py_None;
		self->value_list = NULL;

		self->dict = PyDict_New();
		if (self->dict == NULL) {
			log_warn("failed to allocate dict for Match object");
		}

		self->list = PyList_New();
		if (self->list == NULL) {
			log_warn("failed to allocate list for Match object");
		}
	}

	return (PyObject*) self;
}

static gint Match_init(struct match_data *self, PyObject *args, PyObject *kwds) {
	gchar *pattern = NULL;
	gint r = -1;

	g_warning("__INIT__");

	if (args == NULL) {
		PY_ERROR("empty args");
		return -1;
	}

	return 0;
}

static PyMethodDef Match_methods[] = {
	{"group", (PyCFunction) Match_group, METH_VARARGS, "Stubbed alternative for Match.group"},
	{} /* sentinel */
};

static PyTypeObject Match_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	.tp_name = "re.Match",
	.tp_basicsize = sizeof(struct match_data),
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_doc = "Stubbed version of re's Match-object",
	.tp_init = (initproc) Match_init,
	.tp_new = PyType_GenericNew,
	.tp_alloc = PyType_GenericAlloc,
	.tp_dealloc = (destructor) Match_dealloc,
	.tp_methods = Match_methods,
};

static PyObject* remock_compile(PyObject *self, PyObject *args) {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;
	struct match_data *match_data = NULL;
	gchar *pattern = NULL;
	gint r = -1;

	g_warning("%s:%s:%d", __FILE__, __func__, __LINE__);

	r = PyArg_ParseTuple(args, "s", &pattern);
	if (r == 0) {
		PyErr_SetString(REMOCK_ERROR, "Failed to parse args");
		return NULL;
	}

	match_args = Py_BuildValue("s", pattern);
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
#if 0
	tmp_arr = g_strsplit(tmp, " ", -1);
	if (tmp_arr == NULL) {
		return NULL;
	}

	if (self->string_arr != NULL) {
		g_strfreev(self->string_arr);
		self->string_arr = NULL;
	}

	self->string_arr = tmp_arr;
#endif
}

static PyMethodDef module_methods[] = {
	{"compile", (PyCFunction) remock_compile, METH_VARARGS, "Stubbed alternative for re.compile()"},
	{"match",  (PyCFunction) remock_match, METH_VARARGS, "Stubbed alternative for re.match()"},
	{} /* sentinel */
};

static struct PyModuleDef module = {
	PyModuleDef_HEAD_INIT,
	"remock",
	"Stubbed version of the re module",
	-1,
	module_methods
};

PyMODINIT_FUNC PyInit_remock(void) {
	PyObject *m = NULL;
	gint r = -1;

	r = PyType_Ready(&Match_type);
	if (r < 0) {
		return NULL;
	}

	m = PyModule_Create(&module);
	if (m == NULL) {
		return NULL;
	}

	REMOCK_ERROR = PyErr_NewException("remock.error", NULL, NULL);

	r = PyModule_AddStringConstant(m, "__version__", PACKAGE_VERSION);
	if (r != 0) {
		Py_DECREF(m);
		return NULL;
	}

	Py_INCREF(&Match_type);
	r = PyModule_AddObject(m, "Match", (PyObject *) &Match_type);
	if (r != 0) {
		Py_DECREF(&Match_type);
		Py_DECREF(m);

		return NULL;
	}

	return m;
}
