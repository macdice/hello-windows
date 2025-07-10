#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

const char *or_null(const char *s)
{
	return s == NULL ? "<null>" : s;
}

int
main(int argc, char *argv[])
{
	if (setlocale(LC_ALL, "English, United Kingdom") == NULL)
		printf("error 1\n");
	printf("got: %s\n", or_null(setlocale(LC_ALL, NULL)));
	if (setlocale(LC_ALL, "English_United Kingdom") == NULL)
		printf("error 2\n");
	printf("got: %s\n", or_null(setlocale(LC_ALL, NULL)));
	if (setlocale(LC_ALL, "English_United Kingdom.1252") == NULL)
		printf("error 3\n");
	printf("got: %s\n", or_null(setlocale(LC_ALL, NULL)));

	return EXIT_SUCCESS;
}

