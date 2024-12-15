#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <windows.h>
#include <shellapi.h>

#pragma comment(lib, "shell32.lib")

#define PUNCTUATION_SPACE L"\x2008"

int
main(int argc, char *argv[])
{
	if (argc < 2)
	{
		_wsystem(L"test.exe hello" PUNCTUATION_SPACE "world");
	}
	else
	{
		char *command_line_a;
		wchar_t *command_line_w;
		wchar_t **argv_w;
		int argc_w;

		command_line_a = GetCommandLineA();
		command_line_w = GetCommandLineW();
		argv_w = CommandLineToArgvW(command_line_w, &argc_w);

		printf("GetCommandLineA() = [%s]\n", command_line_a);
		wprintf(L"GetCommandLineW() = [%s]\n", command_line_w);
		printf("CommandLineToArgvW =");
		for (int i = 0; i < argc_w; ++i)
			wprintf(L" [%s]", argv_w[i]);
		printf("\n");
		printf("main()'s argv =");
		for (int i = 0; i < argc; ++i)
			printf(" [%s]", argv[i]);
		printf("\n");
	}
    return 0;
}
