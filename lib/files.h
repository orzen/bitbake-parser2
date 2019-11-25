#ifndef FILES_H
#define FILES_H

#include <glib.h>
#include <stdio.h>

gchar** files_split_extension(const gchar *filename);
gint files_open_file(const gchar *filename, FILE **fd);

#endif
