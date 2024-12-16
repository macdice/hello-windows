#include <windows.h>

int
main(int argc, char *argv[])
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
	{
		wchar_t command[128];

		_snwprintf(command,
				   sizeof(command) / sizeof(wchar_t),
				   L"test-w-client.exe hello%cworld from codepoint %04x",
				   codepoints[i],
				   codepoints[i]);
		_wsystem(command);

		_snwprintf(command,
				   sizeof(command) / sizeof(wchar_t),
				   L"test-a-client.exe hello%cworld from codepoint %04x",
				   codepoints[i],
				   codepoints[i]);
		_wsystem(command);
	}

    return 0;
}
