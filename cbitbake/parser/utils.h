#ifndef _UTILS_H_
#define _UTILS_H_

#include <glib.h>

#define cbb_fail(msg) _cbb_fail(__func__, __LINE__, msg)
void _cbb_fail(const gchar *func, gint line, const gchar *msg);

void append_entry();

void conf_set2(const gchar *key,
              const gchar *val,
              const gchar *flag,
              enum assign_op op,
              enum quote_type quote,
              gboolean export);

gchar* sl_keyword_itoa(gint keyword);
void sl_set(gint keyword, const gchar *val);

void block_set2(const gchar *name, const gchar *expr, gboolean python);

void append_entry();

#endif
