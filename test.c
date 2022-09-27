#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	HANDLE handle;
	OVERLAPPED overlapped;

	handle = CreateFile("test.txt", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE) {
		printf("CreateFile failed\n");
		return EXIT_FAILURE:
	}

	printf("pointer %d\n", SetFilePointer(handle, 0, NULL, FILE_CURRENT));
	if (!WriteFile(handle, "hello world\n", 12, NULL, &overlapped)) {
		printf("WriteFile failed\n");
		return EXIT_FAILURE;
	}
	printf("pointer %d\n", SetFilePointer(handle, 0, NULL, FILE_CURRENT));

	return EXIT_SUCCESS;
}

