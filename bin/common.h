#include "version.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define str(x) __str(x)
#define __str(x) #x

#define count_of(arr) (sizeof(arr)/sizeof(0[arr]))

int
parseint(const char *string, long *result) {
	char *end;
	errno = 0;
	*result = strtol(string, &end, 10);
	return !(*end || errno);
}

const char *
strprefix(const char *str, const char *prefix) {
	while (*prefix) {
		if (!(*str && *str == *prefix)) return 0;
		prefix++; str++;
	}
	return str;
}

void
print_option(char short_opt, const char *long_opt, const char *description) {
	if (short_opt)
		printf("  -%c, ", short_opt);
	else
		printf("      ");

	printf("--%-13s  %s\n", long_opt, description);
}

void
print_version() {
	int major, minor, revision;
	hoedown_version(&major, &minor, &revision);
	printf("Built with Hoedown v%d.%d.%d.\n", major, minor, revision);
}
