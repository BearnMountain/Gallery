#include "io.h"
#include "log.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define IO_READ_CHUNK_SIZE 2097152 // 20MiB should be max size for now

File io_file_read(const char* path) {
	File file = { .is_valid = false };

	FILE *fp = fopen(path, "rb");
	if (!fp || ferror(fp)) {
		log_err("failed to read file: %s. errno: %d", path, errno);
	}

	char *data = NULL;
	char *tmp;
	u32 used = 0;
	u32 size = 0;
	u32 n;

	while (true) {
		if (used + IO_READ_CHUNK_SIZE + 1 > size) {
			size = used + IO_READ_CHUNK_SIZE + 1;

			if (size <= used) {
				free(data);
				log_err("Input file too large: %s\n", path);
				return file;
			}

			tmp = realloc(data, size);
			if (!tmp) {
				free(data);
				log_err("Not enoguh free memory to read file: %s", path);
				return file;
			}
			data = tmp;
		}

		n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
		if (n == 0)
			break;

		used += n;
	}

	if (ferror(fp)) {
		free(data);
		log_err("failed to read file: %s. errno: %d", path, errno);
		return file;
	}

	tmp = realloc(data, used + 1);
	if (!tmp) {
		free(data);
		log_err("not enough free memory to read file: %s", path);
	}
	data = tmp;
	data[used] = 0;

	file.data = data;
	file.length = used;
	file.is_valid = true;

	return file;
}
u32 io_file_write(void* buffer, u32 size, const char *path) {
	(void)buffer;
	(void)size;
	(void)path;
	log_err("in progress...");
	return 0;
}
