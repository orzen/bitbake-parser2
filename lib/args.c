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


