#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	if (!fullpath(NULL, "c:\\does not exists.txt", 0))
		printf("errno = %d, %s\n", errno, strerror(errno));

	return 0;
}

