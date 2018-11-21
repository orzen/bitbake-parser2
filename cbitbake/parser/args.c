#include <argp.h>
#include <errno.h>

const char *cbitbake_version = "cbitbake 0.0.1";
const char *cbitbake_email = "mattias.hansson@axis.com";

static char *desc = "A C implementation of bitbake\n";
static char *arg_usage = "ARG1 [RECIPES...]";

static struct argp_option options[] = {
	{"input", 'i'},
	{0, 0}
};

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int cbb_is_regular_file(const char *file_path) {
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

int cbb_open_file(const char *file_path, FILE **fd) {
	FILE *file = NULL;
	int r = -1;

	r = cbb_is_regular_file(file_path);
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
