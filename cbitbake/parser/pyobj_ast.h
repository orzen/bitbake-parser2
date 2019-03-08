#ifndef PYOBJ_AST_H
#define PYOBJ_AST_H

#include <Python.h>

typedef void (*AstHandleFunc) (PyObject *args);

void ast_handleMethod(PyObject *args);

void ast_handlePythonMethod(PyObject *args);

void ast_handleExport(PyObject *args);

void ast_handleExportFuncs(PyObject *args);

void ast_handleAddTask(PyObject *args);

void ast_handleDelTask(PyObject *args);

void ast_handleBBHandlers(PyObject *args);

void ast_handleInherit(PyObject *args);

void ast_handleData(PyObject *args);

void ast_handleUnset(PyObject *args);

void ast_handleUnsetFlag(PyObject *args);

void ast_handleInclude(PyObject *args);

#endif
