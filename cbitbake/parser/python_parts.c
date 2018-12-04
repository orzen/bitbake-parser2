#include <glib.h>
#include <Python.h>
#include <strings.h>

#include "files.h"

#if 0
__func_start_regexp__
__inherit_regexp__
__export_func_regexp__
__addtask_regexp__
__addhandler_regexp__
__infunc__
__body__
__residue__
__classname__
#endif

static PyObject* api_handle(PyObject *self, PyObject *args) {
	gchar *filename = NULL;
	gchar *basename = NULL;
	gchar **split = NULL;
	PyObject *d = NULL;
	gint include = -1;
	gint r = -1;

	r = PyArg_ParseTuple(args, "sOi", &filename, &d, &include);
	if (r == 0) {
		return NULL;
	}
	// TODO figure out who's owning the memory of filename, d and include
	printf("filename '%s', include '%d'\n", filename, include);

	basename = g_path_get_basename(filename);
	split = cbb_split_extension(filename);
	if (split == NULL) {
		g_free(basename);
		return NULL;
	}

	// TODO init(d)
	PyObject *resp = Py_BuildValue("sb", "FILE", 0);
	PyObject *oldfile = PyObject_CallMethod(d, "getVar", "sb", resp);
	gint res = -2;
	PyArg_ParseTuple(oldfile, "i", &res);

	printf("res getVar '%d'", res);

	//TODO this is and intensional error
	return resp;
#if 0
	if (!g_strcmp0(".bbclass", ext)) {

	}
#endif

}

#if 0
static PyObject* inherit_node() {
	PyObject *dict = NULL;

	dict = PyDict_New();
}
#endif

static PyMethodDef cparse_methods[] = {
	{"handle",  api_handle, METH_VARARGS, "A C implementation of bitbake's parse.BBHandler"},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef cparse_module = {
	PyModuleDef_HEAD_INIT,
	"cparse",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
	             or -1 if the module keeps state in global variables. */
	cparse_methods
};

PyMODINIT_FUNC PyInit_cbbhandler() {
	PyObject *m = NULL;
	PyObject *error = NULL;

	m = PyModule_Create(&cparse_module);
	if (m == NULL) {
		return NULL;
	}

	error = PyErr_NewException("cbbhandler.error", NULL, NULL);
	Py_INCREF(error);

	PyModule_AddObject(m, "error", error);

	return m;
}
