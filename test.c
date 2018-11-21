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

/*
	printf("Unlinking bar.txt...\n");
	if (unlink("bar.txt") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}
CloseHandle(file);
CloseHandle(file2);
*/

	/* this doesn't work */
	printf("Renaming foo.txt to bar.txt...\n");
	if (rename("foo.txt", "bar.txt") != 0)
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	/* this does work */
	const char *new_name = "bar.txt";
	size_t size = strlen(new_name);
	FILE_RENAME_INFO *rename_info = malloc(sizeof(FILE_RENAME_INFO) + size);
	rename_info->Flags = FILE_RENAME_FLAG_POSIX_SEMANTICS;
	rename_info->ReplaceIfExists = true;
	rename_info->RootDirectory = NULL;
	rename_info->FileNameLength = size;
	strcpy(rename_info->FileName, new_name);
	printf("Performing mysterious SetFileInformationByHandle operation...\n");
	if (SetFileInformationByHandle(file, FileRenameInfoEx, &rename_info, sizeof(FILE_RENAME_INFO) + size))
	{
		printf("Failed, error = %d, errno = %d (%s).\n", GetLastError(), errno, strerror(errno));
		exit(1);
	}

	return 0;
}
