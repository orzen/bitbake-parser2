#include <glib.h>
#include <Python.h>

#include "pylog.h"

gchar* bb_parse_resolve_file(GHashTable *modules, PyObject *error, const gchar *filename, PyObject *d) {
	PyObject *mod_bb_parse = NULL;
	PyObject *resolve_file = NULL;
	PyObject *abs_fn = NULL;

	mod_bb_parse = g_hash_table_lookup(modules, "bb.parse");
	if (mod_bb_parse == NULL) {
		py_err(error, "failed to load module 'bb.parse'");
		return NULL;
	}

	resolve_file = PyObject_GetAttrString(mod_bb_parse, "resolve_file");
	if (resolve_file == NULL) {
		py_err(error, "failed to load function 'resolve_file'");
		return NULL;
	}
	if (PyCallable_Check(resolve_file) == 0) {
		py_err(error, "function 'resolve_file' is not callable");
		return NULL;
	}

	abs_fn = PyObject_CallFunction(resolve_file, "sO", filename, d);
	if (abs_fn == NULL || PyErr_Occurred()) {
		py_err(error, "failed to resolve file '%s'", filename);
		return NULL;
	}

	return PyBytes_AsString(abs_fn);
}
