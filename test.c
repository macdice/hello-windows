#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdio.h>
#include <windows.h>

DWORD WINAPI f(LPVOID p)
{
	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
	for (int i = 0; i < 10000000; ++i)
	{
		setlocale(LC_ALL, "fr-FR");
		assert(strcmp(localeconv()->decimal_point, ",") == 0);
		setlocale(LC_ALL, "en-GB");
		assert(strcmp(localeconv()->decimal_point, ".") == 0);
	}
	return 0;
}

#define NTHREADS 8

int
main(int argc, char *argv[])
{
	HANDLE threads[NTHREADS];
	DWORD thread_ids[NTHREADS];
	
	for (int i = 0; i < NTHREADS; ++i)
		threads[i] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[i]);
	
	WaitForMultipleObjects(NTHREADS, threads, TRUE, INFINITE);

	for (int i = 0; i < NTHREADS; ++i)
		CloseHandle(threads[i]);

	return EXIT_SUCCESS;
}

