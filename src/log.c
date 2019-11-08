#include <stdio.h>
#include <glib.h>
#include <Python.h>

void log_pyerr(PyObject *error, const gchar *format, ...) {
	va_list ap;
	gchar *str = NULL;

	va_start(ap, format);
	str = g_strdup_vprintf(format, ap);
	va_end(ap);

	g_error(str);
	PyErr_SetString(error, str);

	g_free(str);
}

void _lex_err(const gchar *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	exit(1);
}
