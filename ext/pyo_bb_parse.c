#include <glib.h>
#include <Python.h>

#include "log.h"

const gchar* bb_parse_resolve_file(PyObject *bb_parse,
                                   PyObject *error,
                                   const gchar *filename,
                                   PyObject *d) {
	PyObject *resolve_file = NULL;
	PyObject *abs_fn = NULL;
	const gchar *ret = NULL;

	G_DEBUG_HERE();

	if (!bb_parse || !error || !filename || !d) {
		log_warn("invalid arguments");
		return NULL;
	}

	resolve_file = PyObject_GetAttrString(bb_parse, "resolve_file");
	if (resolve_file == NULL) {
		log_warn("failed to load function 'resolve_file'");
		return NULL;
	}
	if (PyCallable_Check(resolve_file) == 0) {
		log_warn("function 'resolve_file' is not callable");
		return NULL;
	}

	abs_fn = PyObject_CallFunction(resolve_file, "sO", filename, d);
	if (abs_fn == NULL || PyErr_Occurred()) {
		log_warn("failed to resolve file '%s'", filename);
		return NULL;
	}

	ret = g_strdup(PyUnicode_AsUTF8(abs_fn));
	log_dbg("resolved file '%s'", ret);

	Py_DECREF(resolve_file);
	Py_DECREF(abs_fn);

	return ret;
}
