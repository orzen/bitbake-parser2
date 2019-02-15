#ifndef PYOBJ_AST_H
#define PYOBJ_AST_H

#include <Python.h>

typedef void (*AstHandleFunc) (PyObject *ast,
                               PyObject *args);

void ast_handle_method(PyObject *ast, PyObject *args);

void ast_handle_python_method(PyObject *ast, PyObject *args);

void ast_handle_export(PyObject *ast, PyObject *args);

void ast_handle_export_funcs(PyObject *ast, PyObject *args);

void ast_handle_addtask(PyObject *ast, PyObject *args);

void ast_handle_deltask(PyObject *ast, PyObject *args);

void ast_handle_addhandler(PyObject *ast, PyObject *args);

void ast_handle_inherit(PyObject *ast, PyObject *args);

void ast_handle_data(PyObject *ast, PyObject *args);

void ast_handle_unset(PyObject *ast, PyObject *args);

void ast_handle_unset_flag(PyObject *ast, PyObject *args);

void ast_handle_include(PyObject *ast, PyObject *args);

#endif
