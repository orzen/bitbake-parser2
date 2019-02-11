#include <stdio.h>
#include <glib.h>

#include "utils.h"

extern gchar *filename;
extern gint row_num;

void _cbb_fail(const gchar *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	exit(1);
}

