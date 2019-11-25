#include <stdarg.h>
#include <string.h>
#include <glib.h>
#include <Python.h>

#include "log.h"
#include "pyo_ast2.h"
#include "python_utils.h"

#define IMPORT_METHOD_OR_ERROR(AST, METHODS, HANDLER) \
	if ((METHODS->HANDLER = pyutils_get_method(AST, #HANDLER)) == NULL) goto error;

#define FREE_IF_SET(NAME) \
	if (NAME) g_free(NAME);

#define GEN_AST_HANDLER(NAME, FMT) \
PyObject* ast_##NAME(struct ast *ast, ...) { \
	PyObject *args = NULL; \
	PyObject *ret = NULL; \
	const char fmt[] = #FMT; \
	va_list ap; \
 \
	va_start(ap, fmt); \
	args = Py_VaBuildValue(fmt, ap); \
	va_end(ap); \
 \
	ret = PyObject_CallObject(ast->methods->NAME, args); \
	if (ret == NULL) { \
		log_warn("failed to call ast '%s'", #NAME); \
	} \
	Py_DECREF(args); \
	return ret; \
}

// args: statements, filename, lineno, func_name, body, python, fakeroot
GEN_AST_HANDLER(handleMethod, Osisspp)
// args: statements, filename, lineno, funcname, modulename, body
GEN_AST_HANDLER(handlePythonMethod, Osisss)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleExport, OsiO)
// args: statements, filename, lineno, m, classname
GEN_AST_HANDLER(handleExportFuncs, OsiOs)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleAddTask, OsiO)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleDelTask, OsiO)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleBBHandlers, OsiO)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleInherit, OsiO)
// args: statements, filename, lineno, groupd
GEN_AST_HANDLER(handleData, OsiO)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleUnset, OsiO)
// args: statements, filename, lineno, m
GEN_AST_HANDLER(handleUnsetFlag, OsiO)
// args: statements, filename, lineno, match, force
GEN_AST_HANDLER(handleInclude, OsiOp)
#undef GEN_AST_HANDLER

PyObject* ast_import_statementgroup(PyObject *mod_ast) {
	PyObject *cls_stmtgrp = NULL;

	cls_stmtgrp = PyObject_GetAttrString(mod_ast, "StatementGroup");
	if (cls_stmtgrp == NULL) {
		log_warn("failed to get attribute StatementGroup");
		return NULL;
	}

	return cls_stmtgrp;
}

PyObject* ast_statementgroup_new(PyObject *cls_stmtgrp) {
	PyObject *stmtgrp_obj = NULL;
	PyObject *empty_construct = Py_BuildValue("()");

	stmtgrp_obj = PyObject_CallObject(cls_stmtgrp, empty_construct);
	if (!stmtgrp_obj) {
		log_warn("failed to create new StatementGroup");
		Py_DECREF(empty_construct);
		return NULL;
	}

	Py_DECREF(empty_construct);
	return stmtgrp_obj;
}

struct ast* ast_new() {
	struct ast *a = NULL;
	struct methods *m = NULL;
	PyObject *mod = NULL;

	a = g_new0(struct ast, 1);
	if (!a) {
		log_warn("failed to allocate ast");
	}

	m = g_new0(struct methods, 1);
	if (!m) {
		log_warn("failed to allocate methods for ast");
	}

	// Import ast.py
	mod = PyImport_ImportModule("bb.parse.ast");
	if (!mod) {
		log_warn("failed to import ast");
		goto error;
	}

	IMPORT_METHOD_OR_ERROR(mod, m, handleMethod)
	IMPORT_METHOD_OR_ERROR(mod, m, handlePythonMethod)
	IMPORT_METHOD_OR_ERROR(mod, m, handleExport)
	IMPORT_METHOD_OR_ERROR(mod, m, handleExportFuncs)
	IMPORT_METHOD_OR_ERROR(mod, m, handleAddTask)
	IMPORT_METHOD_OR_ERROR(mod, m, handleDelTask)
	IMPORT_METHOD_OR_ERROR(mod, m, handleBBHandlers)
	IMPORT_METHOD_OR_ERROR(mod, m, handleInherit)
	IMPORT_METHOD_OR_ERROR(mod, m, handleData)
	IMPORT_METHOD_OR_ERROR(mod, m, handleUnset)
	IMPORT_METHOD_OR_ERROR(mod, m, handleUnsetFlag)
	IMPORT_METHOD_OR_ERROR(mod, m, handleInclude)

#undef IMPORT_METHOD_OR_ERROR

	a->mod = mod;
	a->methods = m;

	return a;
error:
	FREE_IF_SET(m->handleMethod)
	FREE_IF_SET(m->handlePythonMethod)
	FREE_IF_SET(m->handleExport)
	FREE_IF_SET(m->handleExportFuncs)
	FREE_IF_SET(m->handleAddTask)
	FREE_IF_SET(m->handleDelTask)
	FREE_IF_SET(m->handleBBHandlers)
	FREE_IF_SET(m->handleInherit)
	FREE_IF_SET(m->handleData)
	FREE_IF_SET(m->handleUnset)
	FREE_IF_SET(m->handleUnsetFlag)
	FREE_IF_SET(m->handleInclude)

	FREE_IF_SET(m);
	FREE_IF_SET(mod);
	FREE_IF_SET(a);

	return NULL;
}

void ast_free(struct ast *a) {
	struct methods *m = NULL;
	if (!a) {
		log_warn("trying to free an empty ast");
		return;
	}

	m = a->methods;

	FREE_IF_SET(m->handleMethod)
	FREE_IF_SET(m->handlePythonMethod)
	FREE_IF_SET(m->handleExport)
	FREE_IF_SET(m->handleExportFuncs)
	FREE_IF_SET(m->handleAddTask)
	FREE_IF_SET(m->handleDelTask)
	FREE_IF_SET(m->handleBBHandlers)
	FREE_IF_SET(m->handleInherit)
	FREE_IF_SET(m->handleData)
	FREE_IF_SET(m->handleUnset)
	FREE_IF_SET(m->handleUnsetFlag)
	FREE_IF_SET(m->handleInclude)
	FREE_IF_SET(m);

	FREE_IF_SET(a->mod);
	FREE_IF_SET(a);
}
