#ifndef PYO_BB_PARSE
#define PYO_BB_PARSE

#include <glib.h>
#include <Python.h>

gchar* bb_parse_resolve_file(GHashTable *modules,
                             PyObject *error,
                             const gchar *filename,
                             PyObject *d);

#endif
