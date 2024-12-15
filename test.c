#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <windows.h>
#include <shellapi.h>

#pragma comment(lib, "shell32.lib")

int
main(int argc, char *argv[])
{
	if (argc < 2)
	{
		/*
		 * https://stackoverflow.com/questions/18169006/all-the-whitespace-characters-is-it-language-independent
		 */
		const wchar_t codepoints[] = {
			0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x0020, 0x0085, 0x00A0,
			0x1680, 0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006,
			0x2007, 0x2008, 0x2009, 0x200A, 0x2028, 0x2029, 0x202F, 0x205F,
			0x3000
		};
		for (int i = 0; i < sizeof(codepoints) / sizeof(wchar_t); ++i)
			_wsystem(L"test.exe hello%cworld", codepoints[i]);
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
