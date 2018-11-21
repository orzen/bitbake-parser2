#ifndef _UTILS2_H_
#define _UTILS2_H_

#include <glib.h>

#include "parser_types.h"

void conf_set(GHashTable *table,
              const gchar *key,
              const gchar *val,
              const gchar *flag,
              enum assign_op op,
              gint export);

void block_set(GHashTable *table,
               const gchar *key,
               const gchar *expr,
               gint python);

GHashTable* add_str(GHashTable *table, const gchar *key, const gchar *val);
GHashTable* add_int(GHashTable *table, const gchar *key, gint val);
GHashTable* new(enum parser_type type);

#endif
