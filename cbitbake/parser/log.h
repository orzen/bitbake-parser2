#ifndef LOG_H
#define LOG_H

#include <glib.h>

#define log_warn(FMT, ...) g_warning("%s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#define log_err(FMT, ...) g_error("%s:%s:%d: " FMT , __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#endif