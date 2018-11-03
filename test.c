#include <direct.h>
#include <stdio.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	printf("Creating directory my_dir...\n");
	if (!CreateDirectoryA("my_dir", NULL))
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Creating file my_dir\\foo.txt...\n");
	HANDLE file = CreateFile("my_dir\\foo.txt", GENERIC_WRITE,
							 FILE_SHARE_DELETE, NULL,
							 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Unlinking file my_dir\\foo.txt...\n");
	if (unlink("my_dir\\foo.txt") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Opening directory my_dir...\n");
	HANDLE dir = CreateFile("my_dir", GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_DELETE,
							0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
	if (dir == INVALID_HANDLE_VALUE)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Removing directory my_dir with rmdir()...\n");
	if (rmdir("my_dir") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	printf("Huh.\n");

	return 0;
}
