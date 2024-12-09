#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

/* Name of this executable so we can run a subprocess. */
#define EXE "test.exe"

/* Locale names known to imply particular encodings. */
#define LOCALE_UTF8 "en-US.UTF-8"
#define LOCALE_1252 "en-US"

/* "café" encoded three ways. */
#define WORD1_UTF16 L"caf\x00e9"
#define WORD1_1252 "caf\xe9"
#define WORD1_UTF8 "caf\xc3\xa9"

/* "crême" encoded three ways. */
#define WORD2_UTF16 "cr\x00came"
#define WORD2_1252 "cr\xcame"
#define WORD2_UTF8 "cr\xc3\xaame"

/* A couple of emojis encoded as UTF-16 (coffee and smiley). */
#define EMOJI1_UTF16 L"\x2615"
#define EMOJI2_UTF16 L"\x263a"

/* A filename with a non-ASCII character encoded three ways. */
#define FILENAME_UTF16 WORD1_UTF16 ".txt"
#define FILENAME_1252 WORD1_1252 ".txt"
#define FILENAME_UTF8 WORD1_UTF8 ".txt"

static void
file_system_function_test(const char *locale, const char *function, const char *encoding)
{
	int fd;
	HANDLE handle;
	const char *filename;

	if (strcmp(encoding, "UTF-8") == 0)
		filename = FILENAME_UTF8;
	else if (strcmp(encoding, "1252") == 0)
		filename = FILENAME_1252;
	else {
		printf("unexpected encoding: %s\n", encoding);
		exit(1);
	}

	/* Create the file using a UTF-16 name. */
	fd = _wopen(FILENAME_UTF16, _O_CREAT | _O_RDWR, _S_IREAD | _S_IWRITE);
	if (fd < 0) {
		printf("can't create file!\n");
		exit(1);
	}
	close(fd);

	/* Set the locale so we can test how it affects this operation. */
	setlocale(LC_ALL, locale);

	printf("locale %s: does %s work with name encoded in %s? ", locale, function, encoding);

	if (strcmp(function, "_open") == 0) {
		fd = _open(filename, _O_RDWR);
		if (fd >= 0)
		{
			printf("yes\n");
			close(fd);
		} else {
			printf("no, errno = %d (%s)\n", errno, strerror(errno));
		}
	} else if (strcmp(function, "CreateFile") == 0) {
		handle = CreateFile(filename,
							GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL, 0);
		if (handle != INVALID_HANDLE_VALUE) {
			printf("yes\n");
			CloseHandle(handle);
		} else {
			printf("no, GetLastError() = %u\n", GetLastError());
		}
	} else if (strcmp(function, "_unlink") == 0) {
		if (_unlink(filename) == 0)
			printf("yes\n");
		else
			printf("no, errno = %d (%s)\n", errno, strerror(errno));
	} else if (strcmp(function, "rename") == 0) {
		if (rename(filename, "other") == 0)
			printf("yes\n");
		else
			printf("no, errno = %d (%s)\n", errno, strerror(errno));
	} else if (strcmp(function, "_mkdir") == 0) {
		if (_mkdir(filename) == -1) {
			if (errno == EEXIST)
				printf("yes\n");
			else
				printf("no, errno = %d (%s)\n", errno, strerror(errno));
		} else {
			_rmdir(filename);
			printf("no (succeeded unexpectedly)\n");
		}
	} else {
		printf("unknown function: %s\n", function);
		exit(1);
	}

	/* Clean up all possible junk. */
	_wunlink(FILENAME_UTF16);
	_unlink("other");
}

static void
parent(void)
{
	/* Let's see if setlocale changes the ACP. */
	printf("parent: my ACP = %u\n", GetACP());
	setlocale(LC_ALL, LOCALE_UTF8);
	printf("parent: my ACP after setlocale = %u\n", GetACP());
	/* -> nope */

	/* Let's see how setlocale affects UTF-8 passed via environ[] and argv[]. */
	setlocale(LC_ALL, LOCALE_UTF8);
	putenv("FOO=" WORD1_UTF8);
	system(EXE " --child " WORD2_UTF8);
	/* -> it arrives correctly encoded in the child's ACP (converted) */

	/* Let's see how setlocale affects 1252 passed via environ[] and argv[]. */
	setlocale(LC_ALL, LOCALE_1252);
	putenv("FOO=" WORD1_1252);
	system(EXE " --child " WORD2_1252);
	/* -> it arrives correctly encoded in the child's ACP */

	/* Let's see how UTF-16 is passed via environ[] and argv[]. */
	_wputenv(L"FOO=" WORD1_UTF16);
	_wsystem(L"test.exe --child " WORD2_UTF16);
	/* -> it arrives correctly encoded in the child's ACP (converted) */

	/* Let's see how UTF-16 is passed when it is not convertible to 1252. */
	_wputenv(L"FOO=xxx" EMOJI1_UTF16 "xxx");
	_wsystem(L"test.exe --child xxx" EMOJI2_UTF16 "xxx");
	/* -> unconvertible chars arrive as '?', but _wgetenv() sees EMOJI1_UTF16 */

	/* Let's see if the char environment is kept in sync with the wchar_t one. */
	setlocale(LC_ALL, LOCALE_1252);
	putenv("BAR=" WORD1_1252);
	printf("parent: putenv() updates wchar_t env as expected? %s",
		   (_wgetenv(L"BAR") &&
			wcscmp(_wgetenv(L"BAR"), WORD1_UTF16) == 0) ? "yes" : "no");
	printf("\n");
	/* -> yes */

	/* Test global locale's effect on file system functions. */
	const char *locales[] = {
		LOCALE_UTF8,
		LOCALE_1252,
		"C",
		NULL
	};
	const char *functions[] = {
		"_open",
		"CreateFile",
		"_unlink",
		"rename",
		"_mkdir",
		NULL
	};
	const char *encodings[] = {
		"UTF-8",
		"1252",
		NULL
	};
	for (const char **function = functions; *function; function++)
		for (const char **locale = locales; *locale; locale++)
			for (const char **encoding = encodings; *encoding; encoding++)
				file_system_function_test(*locale, *function, *encoding);

	/* -> no effect, char paths are always interpreted as ACP */
}

static void
print_hex_char_string(const char *s)
{
	const uint8_t *b = (const uint8_t *) s;

	if (!b)
		printf("NULL");
	else
		while (*b)
			printf(" %02x", *b++);
}

static void
print_hex_wchar_string(const wchar_t *s)
{
	const uint16_t *b = (const uint16_t *) s;

	if (!b)
		printf("NULL");
	else
		while (*b)
			printf(" %04x", *b++);
}

static void
child(const char *arg)
{
	printf("child: my ACP = %u, env[FOO] = %s, arg = %s\n", GetACP(), getenv("FOO"), arg);
	printf("child: environment variable FOO: ");
	print_hex_char_string(getenv("FOO"));
	printf("\n");
	printf("child: environment variable FOO in wchar_t environment: ");
	print_hex_wchar_string(_wgetenv(L"FOO"));
	printf("\n");
	printf("child: command line argument: ");
	print_hex_char_string(arg);
	printf("\n");
}

int
main(int argc, char *argv[])
{
	if (argc >= 2)
		child(argv[2]);
	else
		parent();
    return 0;
}
