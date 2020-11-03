#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

void
doit(const char *collcollate)
{
	NLSVERSIONINFOEX version = {sizeof(NLSVERSIONINFOEX)};
	WCHAR wide_collcollate[LOCALE_NAME_MAX_LENGTH];
	size_t converted_size;

	printf("%s", collcollate);
	if (mbstowcs_s(&converted_size, wide_collcollate, LOCALE_NAME_MAX_LENGTH, collcollate, _TRUNCATE) != 0)
	{
		printf(" (failed to convert to wchar)\n");
	}
	else if (GetNLSVersionEx(COMPARE_STRING, wide_collcollate, &version))
	{
		printf(" -> version %d.%d,%d.%d",
			   (version.dwNLSVersion >> 8) & 0xFFFF,
			   version.dwNLSVersion & 0xFF,
			   (version.dwDefinedVersion >> 8) & 0xFFFF,
			   version.dwDefinedVersion & 0xFF);
	}
	else
	{
		printf(" -> error %d\n", GetLastError());
	}
}

int
main(int argc, char *argv[])
{
	doit("English_United States");
	doit("English_United States.1252");
	doit("English_United States.UTF-8");
	doit("en-US");
	doit("en-US.1252");
	doit("en-US.UTF-8");
}
