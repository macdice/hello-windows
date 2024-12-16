#include <stdio.h>
#include <windows.h>

int
wmain(int argc, wchar_t *argv[])
{
	printf("test-w-client.exe: argv =");
	for (int i = 0; i < argc; ++i)
		wprintf(L" [%s]", argv[i]);
		printf("\n");
	}
    return 0;
}
