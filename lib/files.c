#include <errno.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

gchar** files_split_extension(const gchar *filename) {
	gchar **result = NULL;
	gchar *tmp = NULL;
	gchar *delimiter = NULL;

	if (filename == NULL) {
		return NULL;
	}

	tmp = g_strdup(filename);

	delimiter = strrchr(tmp, '.');
	if (delimiter == NULL) {
		g_free(tmp);
		return NULL;
	}

	result = calloc(2, sizeof(gchar));

	// Copy extension
	result[1] = g_strdup(delimiter);
	if (result[1] == NULL) {
		g_free(tmp);
		g_free(result);

		return NULL;
	}

	// Copy filename without extension
	*delimiter = '\0';
	result[0] = g_strdup(tmp);
	*delimiter = '.';
	if (result[0] == NULL) {
		g_free(tmp);
		g_free(result[1]);
		g_free(result);

		return NULL;
	}

	g_free(tmp);

	return result;
}

int files_is_regular_file(const char *file_path) {
	int r = -1;
	struct stat stat_r;

	r = stat(file_path, &stat_r);
	if (r < 0) {
		fprintf(stderr, "Failed to stat file '%s': '%s'\n", file_path,
		        strerror(errno));
		return -1;
	}

	if ((stat_r.st_mode & S_IFREG) == 0) {
		fprintf(stderr, "File '%s' is not a regular file, aborting\n",
		        file_path);
		return -1;
	}

	return 0;
}

int files_open_file(const char *file_path, FILE **fd) {
	FILE *file = NULL;
	int r = -1;

	r = files_is_regular_file(file_path);
	if (r < 0) {
		return -1;
	}

	file = fopen(file_path, "r");
	if (file == NULL) {
		fprintf(stderr, "Failed to open file '%s'\n", file_path);
		return -1;
	}

	*fd = file;

	return 0;
}
