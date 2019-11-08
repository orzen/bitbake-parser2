#ifndef LOG_H
#define LOG_H

#include <glib.h>
#include <Python.h>

// Generic logging
#define log_debug(FMT, ...) g_debug("DBG: %s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#define log_warn(FMT, ...) g_warning("WARN: %s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#define log_err(FMT, ...) g_error("ERR: %s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__); exit(1);

// Python logging
#define py_err(ERR, FMT, ...) log_pyerr(ERR, "PY_ERR: %s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__)
void log_pyerr(PyObject *error, const gchar *format, ...);

// Lexer and parser logging
extern gchar *filename;
extern gint row_num;

// TODO remove
#define lex_err(format, ...) _lex_err("LEX_ERR: %s[%d]: " format "\n", __func__, __LINE__, ##__VA_ARGS__)
void _lex_err(const gchar *format, ...);

#endif
