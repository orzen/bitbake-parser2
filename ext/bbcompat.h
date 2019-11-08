#ifndef BBCOMPAT_H
#define BBCOMPAT_H

#include <glib.h>
#include <Python.h>

gint bbcompat_call_bbast(GNode *root,
                         PyObject *statements,
                         const gchar *filename);
//PyObject* convert_seq_to_pylist(gchar *fmt, ...);

#endif
