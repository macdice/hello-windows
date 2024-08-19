#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI f(LPVOID p)
{
	fprintf(stderr, "thread %d: default localeconv() = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	setlocale(LC_ALL, "es-ES");

	fprintf(stderr, "thread %d: es-ES localeconv() = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);

	fprintf(stderr, "thread %d: es-ES localeconv() in thread-local mode = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	setlocale(LC_ALL, "de-DE");

	fprintf(stderr, "thread %d: de-DE localeconv() in thread-local mode = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	fprintf(stderr, "thread %d: starting torture test...\n",
		(int) GetCurrentThreadId());

	for (int i = 0; i < 10000; ++i)
	{
		setlocale(LC_ALL, "fr-FR");
		assert(strcmp(localeconv()->decimal_point, ",") == 0);
		setlocale(LC_ALL, "en-GB");
		assert(strcmp(localeconv()->decimal_point, ".") == 0);
	}

	fprintf(stderr, "thread %d: done\n",
		(int) GetCurrentThreadId());

	return 0;
}

#define NTHREADS 8

int
main(int argc, char *argv[])
{
	HANDLE threads[NTHREADS];
	DWORD thread_ids[NTHREADS];
	
	_set_error_mode(_OUT_TO_STDERR);

	for (int i = 0; i < NTHREADS; ++i)
		threads[i] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[i]);
	
	WaitForMultipleObjects(NTHREADS, threads, TRUE, INFINITE);

	for (int i = 0; i < NTHREADS; ++i)
		CloseHandle(threads[i]);

	return EXIT_SUCCESS;
}

