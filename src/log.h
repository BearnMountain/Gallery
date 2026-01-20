#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <defines.h>

typedef enum {
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR
} LOG_TYPE;

#define log(message, ...)      log_category(LOG_INFO, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define log_warn(message, ...) log_category(LOG_WARN, __FILE__, __LINE__, message, ##__VA_ARGS__)
#define log_err(message, ...)  log_category(LOG_ERROR, __FILE__, __LINE__, message, ##__VA_ARGS__)

void log_category(LOG_TYPE type, const char* file_name, const u32 files_line_number, const char* message, ...)
	__attribute__((format(printf, 4, 5)));

#endif
