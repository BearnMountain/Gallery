#include "log.h"

#include <stdio.h>

#define COLOR_RESET  "\033[0m"
#define COLOR_INFO   "\033[37m"
#define COLOR_WARN   "\033[33m"
#define COLOR_ERROR  "\033[31m"
#define MAX_PADDING 15

void log_category(LOG_TYPE type, const char* file_name, const u32 files_line_number, const char* message, ...) {
	va_list args;
	const char* color;
	const char* prefix;
	va_start(args, message);

	switch (type) {
		case LOG_WARN:
			color = COLOR_WARN;
			prefix = "WARN";
			break;
		case LOG_ERROR:
			color = COLOR_ERROR;
			prefix = "ERROR";
			break;
		default: 
			color = COLOR_INFO;
			prefix = "INFO";
			break;
	}

	char header[256];
	snprintf(header, sizeof(header), "%s(%s:%u)", prefix, file_name, files_line_number);
	fprintf(stderr, "%s%-20s%s", color, header, COLOR_RESET);
    vfprintf(stderr, message, args);       
	fprintf(stderr, "\n");

	va_end(args);
}
