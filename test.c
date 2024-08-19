#include <locale.h>
#include <stdio.h>
#include <stdio.h>
#include <windows.h>

static void
myassert(int condition)
{
	if (!condition)
	{
		fprintf(stderr, "XXX assertion failed! decimal_point=[%s]\n", localeconv()->decimal_point);
		exit(1);
	}
}

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

	for (int i = 0; i < 1000000; ++i)
	{
		setlocale(LC_ALL, "fr-FR");
		myassert(strcmp(localeconv()->decimal_point, ",") == 0);
		setlocale(LC_ALL, "en-GB");
		myassert(strcmp(localeconv()->decimal_point, ".") == 0);
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
	
	for (int i = 0; i < NTHREADS; ++i)
		threads[i] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[i]);
	
	WaitForMultipleObjects(NTHREADS, threads, TRUE, INFINITE);

	for (int i = 0; i < NTHREADS; ++i)
		CloseHandle(threads[i]);

	return EXIT_SUCCESS;
}

