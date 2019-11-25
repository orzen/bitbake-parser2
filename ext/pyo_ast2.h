#ifndef PYOBJ_AST_H
#define PYOBJ_AST_H

#include <glib.h>
#include <Python.h>

struct ast {
	PyObject *mod;
	struct methods *methods;
};

struct methods {
	PyObject *handleMethod;
	PyObject *handlePythonMethod;
	PyObject *handleExport;
	PyObject *handleExportFuncs;
	PyObject *handleAddTask;
	PyObject *handleDelTask;
	PyObject *handleBBHandlers;
	PyObject *handleInherit;
	PyObject *handleData;
	PyObject *handleUnset;
	PyObject *handleUnsetFlag;
	PyObject *handleInclude;
};


typedef PyObject* (*AstHandleFunc) (struct ast *ast, ...);

PyObject* ast_handleMethod(struct ast *ast, ...);

PyObject* ast_handlePythonMethod(struct ast *ast, ...);

PyObject* ast_handleExport(struct ast *ast, ...);

PyObject* ast_handleExportFuncs(struct ast *ast, ...);

PyObject* ast_handleAddTask(struct ast *ast, ...);

PyObject* ast_handleDelTask(struct ast *ast, ...);

PyObject* ast_handleBBHandlers(struct ast *ast, ...);

PyObject* ast_handleInherit(struct ast *ast, ...);

PyObject* ast_handleData(struct ast *ast, ...);

PyObject* ast_handleUnset(struct ast *ast, ...);

PyObject* ast_handleUnsetFlag(struct ast *ast, ...);

PyObject* ast_handleInclude(struct ast *ast, ...);

PyObject* ast_import_statementgroup(PyObject *mod_ast);

PyObject* ast_statementgroup_new(PyObject *cls_stmtgrp);

struct ast* ast_new();

void ast_free(struct ast *a);

#endif
