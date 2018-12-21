#include <glib.h>
#include <Python.h>

struct match_mock {
	PyObject_HEAD
	gchar **string_arr;
};

static void MatchMock_dealloc(struct match_mock *self) {
	g_strfreev(self->string_arr);
	self->string_arr = NULL;

	self->ob_type->tp_free((PyObject*) self);
}

static PyObject* MatchMock_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
	struct match_mock *self = NULL;

	self = type->tp_alloc(type, 0);
	self->string_arr = NULL;

	return (PyObject *) self;
}

static gint MatchMock_init(struct match_mock *self, PyObject *args, PyObject *kwds) {
	gchar *tmp = NULL;

	if (!PyArg_ParseTuple(args, "s", &tmp)) {
		return -1;
	}

	self->string_arr = g_strsplit(tmp, " ", -1);

	return 0;
}
