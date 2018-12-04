#ifndef FILES_H
#define FILES_H

#include <glib.h>
#include <stdio.h>

gchar** cbb_split_extension(const gchar *filename);
gint cbb_open_file(const gchar *filename, FILE **fd);


#endif
