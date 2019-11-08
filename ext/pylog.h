#ifndef PYLOG_H
#define PYLOG_H

#include <glib.h>
#include <Python.h>

// Python logging
#define py_err(ERR, FMT, ...) log_pyerr(ERR, "PY_ERR: %s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__)
void log_pyerr(PyObject *error, const gchar *format, ...);

#endif
