#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	if (argc < 2)
	{
		_wsystem(L"test.exe --child a b c");
	}
	else
	{
		char *command_line_a;
		wchar_t *command_line_w;
		wchar_t **argv_w;
		int argc_w;
		char **argv_a;
		int argc_a;

		command_line_a = GetCommandLineA();
		command_line_w = GetCommandLineW();
		printf("GetCommandLineA() = [%s]\n", command_line_a);
		wprintf(L"GetCommandLineW() = [%s]\n", command_line_w);

		argv_a = CommandLineToArgvA(command_line_a, &argc_a);
		argv_w = CommandLineToArgvW(command_line_w, &argc_w);

		printf("CommandLineToArgvA =");
		for (int i = 0; i < argc_a; ++i)
			printf(" [%s]", argv_a[i]);
		printf("\n");
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
