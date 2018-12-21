#include <glib.h>
#include <Python.h>

void ast_handle_python_method(PyObject *ast) {
}

void ast_export(PyObject *ast) {
	ast.handleExportFuncs(statements, fn, lineno, m, __classname__)
}

void ast_add_task(PyObject *ast) {
	ast.handleAddTask(statements, fn, lineno, m)
}

void ast_del_task(PyObject *ast) {
	ast.handleDelTask(statements, fn, lineno, m)
}

void ast_add_handler(PyObject *ast) {
	ast.handleBBHandlers(statements, fn, lineno, m)
}

void ast_inherit(PyObject *ast) {
	ast.handleInherit(statements, fn, lineno, m)
}

void ast_handle_data(PyObject *ast) {
	ast.handleData(statements, fn, lineno, groupd)
}

void ast_handle_include(PyObject *ast) {
	ast.handleInclude(statements, fn, lineno, m, False)

}
