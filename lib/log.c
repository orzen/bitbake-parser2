#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

void _lex_err(const gchar *format, ...) {
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	exit(1);
}
