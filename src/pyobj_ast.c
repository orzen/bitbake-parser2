#include <glib.h>
#include <Python.h>

#include "log.h"
#include "pyobj_ast.h"
#include "python_utils.h"

static GHashTable *ast_method_table;

#define GEN_AST_HANDLER(NAME) \
void ast_##NAME(PyObject *args) { \
	PyObject *func = NULL; \
	PyObject *ret = NULL; \
 \
	func = g_hash_table_lookup(ast_method_table, "##NAME##"); \
	if (func == NULL) { \
		log_warn("failed to look up ast func '##NAME##'"); \
		return; \
	} \
 \
	ret = PyObject_CallObject(func, args); \
	if (ret == NULL) { \
		log_warn("failed to call ast '##NAME##'"); \
	} \
	return; \
}

#define IMPORT_METHOD_OR_RETURN(AST, HANDLER_NAME) \
	if (ast_import_method(AST, HANDLER_NAME)) return -1


gint ast_import_method(PyObject *ast, const gchar *method_str) {
	PyObject *method = NULL;
	gboolean r = FALSE;

	method = pyutils_get_method(ast, method_str);
	if (method == NULL) {
		log_err("failed to get method '%s'", method_str);
		g_hash_table_unref(ast_method_table);
		return -1;
	}

	r = g_hash_table_insert(ast_method_table,
	                        g_strdup(method_str),
	                        method);
	if (r != TRUE) {
		log_err("failed to insert method '%s' into hashtable",
		          method_str);
		Py_XDECREF(method);
		g_hash_table_unref(ast_method_table);
		return -1;
	}

	return 0;
}

gint ast_cb_init(PyObject *ast) {
	ast_method_table = g_hash_table_new_full(g_str_hash,
	                                         g_str_equal,
	                                         g_free,
	                                         (GDestroyNotify) Py_DecRef);
	if (ast_method_table == NULL) {
		log_err("failed to create ast method table");
		return -1;
	}

	IMPORT_METHOD_OR_RETURN(ast, "handleMethod");
	IMPORT_METHOD_OR_RETURN(ast, "handlePythonMethod");
	IMPORT_METHOD_OR_RETURN(ast, "handleExport");
	IMPORT_METHOD_OR_RETURN(ast, "handleExportFuncs");
	IMPORT_METHOD_OR_RETURN(ast, "handleAddTask");
	IMPORT_METHOD_OR_RETURN(ast, "handleDelTask");
	IMPORT_METHOD_OR_RETURN(ast, "handleBBHandlers");
	IMPORT_METHOD_OR_RETURN(ast, "handleInherit");
	IMPORT_METHOD_OR_RETURN(ast, "handleData");
	IMPORT_METHOD_OR_RETURN(ast, "handleUnset");
	IMPORT_METHOD_OR_RETURN(ast, "handleUnsetFlag");
	IMPORT_METHOD_OR_RETURN(ast, "handleInclude");

	#undef IMPORT_METHOD_OR_RETURN

	return 0;
}

void ast_cb_free() {
	g_hash_table_unref(ast_method_table);
}

GEN_AST_HANDLER(handleMethod)
GEN_AST_HANDLER(handlePythonMethod)
GEN_AST_HANDLER(handleExport)
GEN_AST_HANDLER(handleExportFuncs)
GEN_AST_HANDLER(handleAddTask)
GEN_AST_HANDLER(handleDelTask)
GEN_AST_HANDLER(handleBBHandlers)
GEN_AST_HANDLER(handleInherit)
GEN_AST_HANDLER(handleData)
GEN_AST_HANDLER(handleUnset)
GEN_AST_HANDLER(handleUnsetFlag)
GEN_AST_HANDLER(handleInclude)
#undef GEN_AST_HANDLER

#if 0
void ast_handle_method(PyObject *args) {
	PyObject *func = NULL;
	PyObject *ret = NULL;
	const gchar *func_str = "handleMethod";

	func = g_hash_table_lookup(ast_method_table, func_str);
	if (func == NULL) {
		log_warn("failed to look up ast func '%s'", func_str);
		return;
	}

	ret = PyObject_CallObject(func, args);
	if (ret == NULL) {
		log_warn("failed to call ast '%s'", func_str);
	}
	//handleMethod(statements, filename, lineno, func_name, body, python, fakeroot):
	//    statements.append(MethodNode(filename, lineno, func_name, body, python, fakeroot))
	return;
}

void ast_handle_python_method(PyObject *ast) {
//handlePythonMethod(statements, filename, lineno, funcname, modulename, body):
//    statements.append(PythonMethodNode(filename, lineno, funcname, modulename, body))
	return;
}

void ast_handle_export(PyObject *ast, PyObject *args) {
// same as exporting a variable assignment
//handleExport(statements, filename, lineno, m):
//    statements.append(ExportNode(filename, lineno, m.group(1)))
}

void ast_handle_export_funcs(PyObject *ast) {
//handleExportFuncs(statements, filename, lineno, m, classname):
//    statements.append(ExportFuncsNode(filename, lineno, m.group(1), classname))
	//ast.handleExportFuncs(statements, fn, lineno, m, __classname__)
	return;
}

void ast_handle_addtask(PyObject *ast) {
//handleAddTask(statements, filename, lineno, m):
//    func = m.group("func")
//    before = m.group("before")
//    after = m.group("after")
//    if func is None:
//        return
//
//    statements.append(AddTaskNode(filename, lineno, func, before, after))
	//ast.handleAddTask(statements, fn, lineno, m)
	return;
}

void ast_handle_deltask(PyObject *ast) {
//handleDelTask(statements, filename, lineno, m):
//    func = m.group("func")
//    if func is None:
//        return
//
//    statements.append(DelTaskNode(filename, lineno, func))
	//ast.handleDelTask(statements, fn, lineno, m)
	return;
}

void ast_handle_addhandler(PyObject *ast) {
//handleBBHandlers(statements, filename, lineno, m):
//    statements.append(BBHandlerNode(filename, lineno, m.group(1)))
	//ast.handleBBHandlers(statements, fn, lineno, m)
	return;
}

void ast_handle_inherit(PyObject *ast) {
	//ast.handleInherit(statements, fn, lineno, m)
	//handleInherit(statements, filename, lineno, m):
	//    classes = m.group(1)
	//    statements.append(InheritNode(filename, lineno, classes))
	return;
}

void ast_handle_data(PyObject *ast, PyObject *args) {
//handleData(statements, filename, lineno, groupd):
//    statements.append(DataNode(filename, lineno, groupd))
	//ast.handleData(statements, fn, lineno, groupd)
	return;
}

void ast_handle_unset(PyObject *ast) {
	//handleUnset(statements, filename, lineno, m):
	//    statements.append(UnsetNode(filename, lineno, m.group(1)))
	return;
}

void ast_handle_unset_flag(PyObject *ast) {
//handleUnsetFlag(statements, filename, lineno, m):
//    statements.append(UnsetFlagNode(filename, lineno, m.group(1), m.group(2)))
	return;
}

void ast_handle_include(PyObject *ast,
                        const PyObject *statements,
                        const PyObject *filename,
                        const PyObject *lineno,
                        const PyObject *match,
                        const PyObject *force) {

//handleInclude(statements, filename, lineno, m, force):
//    statements.append(IncludeNode(filename, lineno, m.group(1), force))
	//PyObject *args = Py_BuildValue("OOOOO", statements, filename, lineno,
	//                               match, force);
	//ast.handleInclude(statements, fn, lineno, m, False)

	return;
}
#endif
