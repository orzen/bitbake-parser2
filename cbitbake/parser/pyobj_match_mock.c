#include <glib.h>
#include <Python.h>
#include <stdarg.h>

static const gchar *PACKAGE_VERSION = "0.0.1";
static PyObject *REMOCK_ERROR;

#define DEBUG g_warning("%s:%s:%d", __FILE__, __func__, __LINE__);
#define FAIL(msg) PyErr_SetString(REMOCK_ERROR, "argument must be a dict");

struct match_data {
	PyObject_HEAD
	PyObject *value_obj;
	GList *value_list;
} MatchData;


static PyObject* Match_group(struct match_data *self, PyObject *args) {
	gchar *value = NULL;
	gint idx = -1;
	gint r = -1;

	DEBUG

	if (self == NULL || args == NULL) {
		g_warning("one or more of the arguments is NULL");
		return NULL;
	}

	r = PyArg_ParseTuple(args, "i", &idx);
	if (r != 0) {
		g_warning("failed to parse args");
		return NULL;
	}

	if (idx < 0) {
		g_warning("index out of range");
		return NULL;
	}

	value = g_list_nth_data(self->value_list, idx);
	if (value == NULL) {
		g_warning("value is NULL");
	}

	return Py_BuildValue("s", value);
}


static PyObject* arrange_kw(GHashTable *tbl) {
	gchar *expr = NULL;
	gint *export  = NULL;
	gint *kw = NULL;
	gchar *keyword = NULL;
	PyObject *list = NULL;
	gint r = -1;

	if (tbl == NULL) {
		g_warning("empty args");
		return NULL;
	}

	kw = g_hash_table_lookup(tbl, "keyword");
	if (kw == NULL) {
		g_warning("keyword is not specified");
		return NULL;
	}

	expr = g_hash_table_lookup(tbl, "expr");
	if (expr == NULL) {
		g_warning("empty keyword expression");
	}

	list = PyList_New(2);
	if (list == NULL) {
		g_warning("failed to allocate PyList");
		return NULL;
	}

	keyword = cbb_types_kw_itoa(*kw);
	r = PyList_Append(list, PyUnicode_FromString(keyword));
	g_free(keyword);
	if (r != 0) {
		g_warning("failed to append keyword");
		Py_DECREF(list);
		return NULL;
	}

	r = PyList_Append(list, PyUnicode_FromString(expr));
	if (r != 0) {
		g_warning("failed to append keyword expression");
		Py_DECREF(list);
		return NULL;
	}

	return list;
}

static gint Match_set_int(PyObject *self, enum parser_type type,  GHashTable *tbl) {
	gint *entry_type = NULL;
	PyObject *group_list = NULL;

	if (self == NULL || tbl == NULL) {
		g_warning("empty args");
		return NULL;
	}

	entry_type = g_hash_table_lookup(tbl, "type");
	if (entry_type == NULL) {
		g_warning("entry is missing type info");
		return NULL;
	}

	switch(*entry_type) {
		case block:
			arrange_block();
			break;
		case conf:
			arrange_conf();
			break;
		case kw:
			group_list = arrange_kw(tbl);
			break;
		case addtask:
			arrange_addtask();
			break;
	};

	if (group_list == NULL) {
		g_warning("failed to create group list");
		return -1;
	}
}

static PyObject* Match_va_set(PyObject *self, PyObject *args) {
	PyObject *arg_dict = NULL;
	PyObject *type_obj = NULL;
	PyObject *type_uni = NULL;
	gint type_int = NULL;
	gint i = -1;
	gint r = -1;

	DEBUG

	if (self == NULL || args == NULL) {
		g_warning("self or format is NULL");
		return Py_False;
	}

	DEBUG

	r = PyArg_ParseTuple(args, "O!", &PyDict_Type, &arg_dict);

	DEBUG
	if (!r) {
		PyErr_SetString(REMOCK_ERROR, "argument must be a dict");
		g_warning("failed to parse args");
		return Py_False;
	}

	DEBUG

	if (arg_dict == NULL) {
		g_warning("empty arguments!");
		return Py_False;
	}

	if (PyDict_CheckExact(arg_dict) != TRUE) {
		g_warning("argument is a derivative from dict");
		return Py_False;
	}

	DEBUG
	type_uni = PyUnicode_FromString("type");
	type_obj = PyDict_GetItem(arg_dict, type_uni);
	Py_DECREF(type_uni);
	type_int = PyLong_AsLong(type_o);

	switch(type_int) {
		case block:
			arrange_block();
			break;
		case conf:
			arrange_conf();
			break;
		case kw:
			arrange_kw();
			break;
		case addtask:
			arrange_addtask();
			break;
	};
	g_warning("TYPE: '%d'", type_int);

	return Py_True;

#if 0
	self->value_obj = obj;
	self->value_list = list;
#endif

}

static void Match_dealloc(struct match_data *self) {
	DEBUG

	g_list_free_full(self->value_list, g_free);

	Py_XDECREF(self->value_obj);

	Py_TYPE(self)->tp_free((PyObject*) self);
}

PyObject* Match_new_int() {
	PyObject *match_args = NULL;
	PyObject *match_kwds = NULL;
	PyObject *match_obj = NULL;

	match_args = Py_BuildValue("s", "internal");
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
	}

	return (PyObject*) self;
}

static gint Match_init(struct match_data *self, PyObject *args, PyObject *kwds) {
	gchar *pattern = NULL;
	gint r = -1;

	g_warning("__INIT__");

	if (args == NULL) {
		return -1;
	}

	r = PyArg_ParseTuple(args, "s", &pattern);
	if (r != 0) {
		return -1;
	}

	return 0;
}

static PyObject* Match_stub() {
	return NULL;
}

static PyMethodDef Match_methods[] = {
	{"group", (PyCFunction) Match_group, METH_VARARGS, "Stubbed alternative for Match.group"},
	{"set",  (PyCFunction) Match_va_set, METH_VARARGS, "Stubbed alternative for re.match"},
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
