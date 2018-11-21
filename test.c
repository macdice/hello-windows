#include <direct.h>
#include <stdio.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	printf("Creating file foo.txt...\n");
	HANDLE file = CreateFile("foo.txt", GENERIC_WRITE,
							 FILE_SHARE_DELETE, NULL,
							 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Creating file bar.txt...\n");
	HANDLE file2 = CreateFile("bar.txt", GENERIC_WRITE,
							 FILE_SHARE_DELETE, NULL,
							 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file2 == INVALID_HANDLE_VALUE)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Unlinking bar.txt...\n");
	if (unlink("bar.txt") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}
//CloseHandle(file2);

	printf("Renaming foo.txt to bar.txt...\n");
	if (rename("foo.txt", "bar.txt") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	return 0;
}
