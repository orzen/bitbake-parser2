#ifndef _UTILS_H_
#define _UTILS_H_

#include <glib.h>

#if 0
void conf_set(GHashTable *table,
              const gchar *key,
              const gchar *val,
              const gchar *flag,
              enum assign_op op);

void block_set(GHashTable *table,
               const gchar *key,
               const gchar *expr,
               gint fakeroot,
               gint python);

GHashTable* add_str(GHashTable *table, const gchar *key, const gchar *val);
GHashTable* add_int(GHashTable *table, const gchar *key, gint val);
GHashTable* new(enum parser_type type);
#endif

#endif
