#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int files_read_file(const char *filename,
              char **read_buffer,
              size_t *buffer_size) {
	FILE *fd = NULL;
	int r = 0;
	struct stat r_stat = {0};
	char *buf = NULL;
	ssize_t buf_s = -1;

	if (filename == NULL || read_buffer == NULL || *read_buffer != NULL ||
	    buffer_size == NULL) {
		return 1;
	}

	fd = fopen(filename, "r");
	if (fd == NULL) {
		fprintf(stderr, "Failed to open file '%s': '%s'\n", filename, strerror(errno));
		return 1;
	}

	r = stat(filename, &r_stat);
	if (r != 0) {
		fprintf(stderr, "Failed to stat file '%s': '%s'\n", filename,
		        strerror(errno));
		fclose(fd);
		return 1;
	}

	if (r_stat.st_size > SIZE_MAX) {
		fprintf(stderr, "File is too large, please implement partial read '%s': '%s'\n",
		        filename,
		        strerror(errno));
		fclose(fd);
		return 1;
	}

	buf = calloc(r_stat.st_size + 1, sizeof(char));
	if (buf == NULL) {
		fprintf(stderr, "Failed to allocate memory for read buffer\n");
		fclose(fd);
		return 1;
	}

	buf_s = fread(buf, r_stat.st_size, 1, fd);
	buf[r_stat.st_size] = '\0';
	if (buf_s == 0) {
		if (feof(fd))
			printf("Received EOF\n");

		if (ferror(fd)) {
			fprintf(stderr, "Read returned 0 implement eof/error handling\n");
			clearerr(fd);
			fclose(fd);
			free(buf);
			return 1;
		}
	}

	*buffer_size = r_stat.st_size + 1;
	*read_buffer = buf;

	fclose(fd);

	return 0;
}
