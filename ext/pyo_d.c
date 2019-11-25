#include <glib.h>
#include <Python.h>

#include "log.h"

PyObject* pyo_d_get_var(PyObject *d, const gchar *var, gboolean expand) {
	PyObject *method_resp = NULL;

	if (d == NULL || var == NULL) {
		printf("empty args\n");
		return NULL;
	}

	method_resp = PyObject_CallMethod(d, "getVar", "(si)", var, expand);
	if (method_resp == NULL) {
		printf("empty response from getVar '%s'\n", var);
	}

	// TODO investigate if the return value always is of type string
	return method_resp;
}

void pyo_d_set_var(PyObject *d, const gchar *var, const gchar *value) {
	if (d == NULL || var == NULL || value == NULL) {
		printf("empty args\n");
		return;
	}

	PyObject_CallMethod(d, "setVar", "ss", var, value);
}

void pyo_d_init(PyObject *d) {
	PyObject *topdir = NULL;
	gchar buf[2048];
	gchar *cwd = NULL;

	if (d == NULL) {
		log_warn("failed to initialize d");
		return;
	}

	topdir = pyo_d_get_var(d, "TOPDIR", 0);
	if (topdir == Py_None) {
		printf("set TOPDIR\n");
		cwd = getcwd(buf, sizeof(buf));
		pyo_d_set_var(d, "TOPDIR", cwd);
	}
}
