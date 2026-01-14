#ifndef _IO_H_
#define _IO_H_

#include "defines.h"

typedef struct {
	char* data;
	u32 length;
	b32 is_valid;
} File;

File io_file_read(const char* path);
u32 io_file_write(void* buffer, u32 size, const char* path);


#endif
