#include <glib.h>
#include <Python.h>

#if 0
void ast_handle_entry(PyObject *ast, GHashTable *entry) {
	gchar *filename = NULL;
	gint *lineno = NULL;
	gint *type = NULL;

	filename = g_hash_table_lookup(tbl, "filename");
	lineno = g_hash_table_lookup(tbl, "lineno");
	type = g_hash_table_lookup(tbl, "type");
	if (filename == NULL || lineno == NULL || type == NULL) {
		g_warning("empty generic values");
		return -1;
	}

	obj_filename = PyUnicode_FromString(filename);
	if (obj_filename == NULL) {
		g_warning("failed to create unicode from '%s'", filename);
		Py_DECREF(tmp_dict);
		return -1;
	}
	obj_lineno = PyLong_FromLong(lineno);
	if (obj_lineno == NULL) {
		g_warning("failed to create long from '%d'", lineno);
		Py_DECREF(tmp_dict);
		Py_DECREF(obj_filename);
		return -1;
	}

	switch(*type) {
	case block:
		block_to_match();
		break;
	case conf:
		arrange_conf();
		break;
	case kw:
		keyword = g_hash_table_lookup(entry, "keyword");
		match = convert_kw_to_match(*keyword, entry);
		break;
	case addtask:
		arrange_addtask();
		break;
	};
}

#endif

void ast_handle_method(PyObject *ast) {
	//handleMethod(statements, filename, lineno, func_name, body, python, fakeroot):
	//    statements.append(MethodNode(filename, lineno, func_name, body, python, fakeroot))
	return;
}

void ast_handle_python_method(PyObject *ast) {
//handlePythonMethod(statements, filename, lineno, funcname, modulename, body):
//    statements.append(PythonMethodNode(filename, lineno, funcname, modulename, body))
	return;
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

void ast_handle_add_handler(PyObject *ast) {
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

void ast_handle_data(PyObject *ast) {
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
	//PyObject *args = Py_BuildValue("OOOOO", statements, filename, lineno,
	//                               match, force);
	//ast.handleInclude(statements, fn, lineno, m, False)

	return;
}

#if 0

handleInclude(statements, filename, lineno, m, force):
    statements.append(IncludeNode(filename, lineno, m.group(1), force))

// same as exporting a variable assignment
handleExport(statements, filename, lineno, m):
    statements.append(ExportNode(filename, lineno, m.group(1)))



handleData(statements, filename, lineno, groupd):
    statements.append(DataNode(filename, lineno, groupd))






#endif
