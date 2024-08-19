#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdio.h>
#include <windows.h>

DWORD WINAPI f(LPVOID p)
{
	printf("thread %d: default localeconv() = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	setlocale(LC_ALL, "es-ES");

	printf("thread %d: es-ES localeconv() = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);

	printf("thread %d: es-ES localeconv() in thread-local mode = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	setlocale(LC_ALL, "de-DE");

	printf("thread %d: de-DE localeconv() in thread-local mode = %p\n",
		(int) GetCurrentThreadId(),
		localeconv());

	printf("thread %d: starting torture test...\n",
		(int) GetCurrentThreadId());

	for (int i = 0; i < 1000000; ++i)
	{
		setlocale(LC_ALL, "fr-FR");
		assert(strcmp(localeconv()->decimal_point, ",") == 0);
		setlocale(LC_ALL, "en-GB");
		assert(strcmp(localeconv()->decimal_point, ".") == 0);
	}

	printf("thread %d: done\n");

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

