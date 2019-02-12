#ifndef PYOBJ_AST_H
#define PYOBJ_AST_H

#include <Python.h>

void ast_handle_method(PyObject *ast);

void ast_handle_python_method(PyObject *ast);

void ast_handle_export(PyObject *ast);

void ast_handle_addtask(PyObject *ast);

void ast_handle_deltask(PyObject *ast);

void ast_handle_add_handler(PyObject *ast);

void ast_handle_inherit(PyObject *ast);

void ast_handle_data(PyObject *ast);

void ast_handle_unset(PyObject *ast, gchar *flag);

void ast_handle_unset_flag(PyObject *ast);

void ast_handle_include(PyObject *ast,
                        const PyObject *statements,
                        const PyObject *filename,
                        const PyObject *lineno,
                        const PyObject *match,
                        const PyObject *force);

#endif
