#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

int
main(int argc, char *argv[])
{
	NLSVERSIONINFOEX version = {sizeof(NLSVERSIONINFOEX)};
	WCHAR wide_collcollate[LOCALE_NAME_MAX_LENGTH];
	size_t converted_size;

	mbstowcs_s(&converted_size, wide_collcollate, LOCALE_NAME_MAX_LENGTH,
			   _TRUNCATE);
//	MultiByteToWideChar(CP_ACP, 0, collcollate, -1, wide_collcollate,
//						LOCALE_NAME_MAX_LENGTH);
	if (GetNLSVersionEx(COMPARE_STRING, wide_collcollate, &version))
	{
		printf("%d.%d,%d.%d",
			   (version.dwNLSVersion >> 8) & 0xFFFF,
			   version.dwNLSVersion & 0xFF,
			   (version.dwDefinedVersion >> 8) & 0xFFFF,
			   version.dwDefinedVersion & 0xFF);
		return 0;
	}
	else
	{
		fprintf(stderr, "Error %d\n", GetLastError());
		return 1;
	}
}
