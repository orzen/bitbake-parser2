#include <glib.h>
#include <Python.h>
#include <unistd.h>

#include "log.h"
#include "pyobj_d.h"

void cbb_print_pyobj(const gchar *filename, PyObject *o) {
	FILE *fd = NULL;

	printf("Writing object to '%s'\n", filename);

	fd = fopen(filename, "w");
	PyObject_Print(o, fd, Py_PRINT_RAW);

	fclose(fd);
}

void cbb_init(PyObject *d) {
	PyObject *topdir = NULL;
	gchar buf[2048];
	gchar *cwd = NULL;

	if (d == NULL) {
		return;
	}

	topdir = cbb_d_get_var(d, "TOPDIR", 0);
	if (topdir == Py_None) {
		printf("set TOPDIR\n");
		cwd = getcwd(buf, sizeof(buf));

		cbb_d_set_var(d, "TOPDIR", cwd);
	}
}

PyObject* pyutils_get_method(PyObject *module, const gchar *method_str) {
	PyObject *method = NULL;

	if (module == NULL || method_str == NULL) {
		log_warn("empty args");
		return NULL;
	}

	method = PyObject_GetAttrString(module, method_str);
	if (method == NULL) {
		g_warning("failed to get method '%s'", method_str);
		return NULL;
	}

	return method;
}

PyObject* get_method_from_module(const gchar *module_str, const gchar *method_str) {
	PyObject *module = NULL;
	PyObject *method = NULL;

	module = PyImport_ImportModule(module_str);
	if (module == NULL) {
		g_warning("failed to import module '%s' to access '%s'", module_str, method_str);
		return NULL;
	}

	method = PyObject_GetAttrString(module, method_str);
	if (method == NULL) {
		g_warning("failed to get method '%s' from module '%s'", method_str, module_str);
		Py_DECREF(module);
		return NULL;
	}

	Py_DECREF(module);

	return method;
}

/*
def resolve_file(fn, d):
    if not os.path.isabs(fn):                                                                                                             
        bbpath = d.getVar("BBPATH")                                                                                                       
        newfn, attempts = bb.utils.which(bbpath, fn, history=True)                                                                        
        for af in attempts:                                                                                                               
            mark_dependency(d, af)                                                                                                        
        if not newfn:
            raise IOError(errno.ENOENT, "file %s not found in %s" % (fn, bbpath))                                                         
        fn = newfn                                                                                                                        
    else:
        mark_dependency(d, fn)                                                                                                            

    if not os.path.isfile(fn):
        raise IOError(errno.ENOENT, "file %s not found" % fn)                                                                             
        
    return fn
 */

gchar* cbb_resolve_file(const gchar *filename, PyObject *d) {
	PyObject *bbpath = NULL;
	PyObject *method = NULL;
	PyObject *args = NULL;
	PyObject *keywords = NULL;
	PyObject *res = NULL;
	PyObject *new_filename_obj = NULL;
	PyObject *attempts = NULL;
	Py_ssize_t size = 0;
	gchar *new_filename_str = NULL;
	gint r = -1;

	if (filename == NULL || d == NULL) {
		g_warning("empty args");
		return NULL;
	}

	if (!g_path_is_absolute(filename)) {
		bbpath = cbb_d_get_var(d, "BBPATH", 1);

		method = get_method_from_module("bb.utils", "which");
		if (method == NULL) {
			g_warning("failed to get method 'which' from bb.utils");
			Py_DECREF(bbpath);
			return NULL;
		}

		args = Py_BuildValue("(Os)", bbpath, filename);
		if (args == NULL) {
			g_warning("failed to build args");
			Py_DECREF(bbpath);
			Py_DECREF(method);
			return NULL;
		}

		keywords = PyDict_New();
		if (keywords == NULL) {
			g_warning("failed to create dict for keywords");
			Py_DECREF(bbpath);
			Py_DECREF(args);
			Py_DECREF(method);
			return NULL;
		}
		r = PyDict_SetItemString(keywords, "history", Py_True);
		if (r != 0) {
			g_warning("failed to set keyword arg");
			Py_DECREF(bbpath);
			Py_DECREF(args);
			Py_DECREF(keywords);
			Py_DECREF(method);
			return NULL;
		}

		res = PyObject_Call(method, args, keywords);
		Py_DECREF(method);
		Py_DECREF(args);
		Py_DECREF(keywords);

		if (!PyTuple_Check(res)) {
			g_error("response is not a tuple as expected");
			Py_DECREF(res);
			return NULL;
		}

		size = PyTuple_Size(res);
		if (size != 2) {
			g_warning("unexpected response size '%zu' != 2", size);
			Py_DECREF(res);
			return NULL;
		}

		new_filename_obj = PyTuple_GetItem(res, 0);
		if (new_filename_obj == NULL) {
			g_warning("failed to get new_filename from tuple");
			Py_DECREF(res);
			return NULL;
		}

		attempts = PyTuple_GetItem(res, 1);
		if (attempts == NULL) {
			g_warning("failed to get attempts from tuple");
			Py_DECREF(res);
			Py_DECREF(new_filename_obj);
			return NULL;
		}
		Py_DECREF(res);
	}

	new_filename_str = PyBytes_AsString(new_filename_obj);
	Py_DECREF(new_filename_obj);

	return new_filename_str;
}
