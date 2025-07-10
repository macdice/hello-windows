#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	if (setlocale(LC_ALL, "English, United Kingdom") == NULL)
		printf("error 1\n");
	printf("got: %s\n", setlocale(LC_ALL, NULL));

	return EXIT_SUCCESS;
}

