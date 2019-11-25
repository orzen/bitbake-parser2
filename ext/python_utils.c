#include <glib.h>
#include <Python.h>
#include <unistd.h>

#include "log.h"
#include "pyo_d.h"

#if 0
PyObject* pyu_import_module(const gchar *module_name) {
	PyObject *module = NULL;

	module = PyImport_ImportModule(module_name);
	if (module != NULL) {
		*obj = module;
		return 0;
	}

	log_warn("failed to import module '%s'", module_name);
	return -1;
}

static gint import_modules(GHashTable *modules, gint num, ...) {
	va_list ap;
	PyObject *module = NULL;
	gchar *module_name = NULL;
	gint r = 0;
	gint i = 0;

	va_start(ap, 0);
	for (i = 0; i < num; i++) {
		module_name = va_arg(ap, char*);
		if (pyu_import_module(&module, module_name) != 0) {
			return -1;
		}


		if (!g_hash_table_insert(modules,
					 g_strdup(module_name),
					 module)) {
			g_warning("failed to store module '%s' in hashtable", module_name);
			return = -1;
		}
	}
	va_end(ap);

	return 0;
}
#endif

void cbb_print_pyobj(const gchar *filename, PyObject *o) {
	FILE *fd = NULL;

	printf("Writing object to '%s'\n", filename);

	fd = fopen(filename, "w");
	PyObject_Print(o, fd, Py_PRINT_RAW);

	fclose(fd);
}

PyObject* pyutils_get_method(PyObject *module, const gchar *method_str) {
	PyObject *method = NULL;

	if (module == NULL || method_str == NULL) {
		log_warn("empty args");
		return NULL;
	}

	method = PyObject_GetAttrString(module, method_str);
	if (method == NULL) {
		log_warn("failed to get method '%s'", method_str);
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
		bbpath = pyo_d_get_var(d, "BBPATH", 1);

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
