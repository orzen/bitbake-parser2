#include <glib.h>
#include <Python.h>

PyObject* cbb_d_get_var(PyObject *d, const gchar *var, gboolean expand) {
	PyObject *method_resp = NULL;

	if (d == NULL || var == NULL) {
		printf("empty args\n");
		return NULL;
	}

	method_resp = PyObject_CallMethod(d, "getVar", "si",
			var, expand);
	if (method_resp == NULL) {
		printf("empty response from getVar '%s'\n", var);
	}

	// TODO investigate if the return value always is of type string
	return method_resp;
}

void cbb_d_set_var(PyObject *d, const gchar *var, const gchar *value) {
	if (d == NULL || var == NULL || value == NULL) {
		printf("empty args\n");
		return;
	}

	PyObject_CallMethod(d, "setVar", "ss", var, value);
}
