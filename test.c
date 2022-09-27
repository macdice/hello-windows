#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	char *x = fullpath(NULL, "c:\\does not exists.txt", 0);

	if (!x) {
		perror("_fullpath");

	return 0;
}

