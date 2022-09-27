#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	if (!_fullpath(NULL, "c:\\does not exists.txt", 0))
		printf("errno = %d, %s\n", errno, strerror(errno));

	return 0;
}

