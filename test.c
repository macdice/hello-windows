#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
doit(const char *name)
{
	char *full;

	full = _fullpath(NULL, name, 0);
	if (full) {
		printf("%s -> %s\n", name, full);
		free(full);
	}
	else
		printf("%s -> NULL, errno = %d, %s\n", name, errno, strerror(errno));
}

int
main(int argc, char *argv[])
{
	doit("c:\\windows");
	doit("c:\\does not exist.txt");
	return 0;
}

