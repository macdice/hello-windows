#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	if (argc < 2)
	{
		_wsystem(L"test.exe --child");
	}
	else
	{
		char *command_line_a;
		wchar_t *command_line_w;

		command_line_a = GetCommandLineA();
		command_line_w = GetCommandLineW();

		printf("GetCommandLineA() = %s\n", command_line_a);
		wprintf("GetCommandLineW() = %s\n", command_line_w);
	}
    return 0;
}
