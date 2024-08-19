#include <locale.h>
#include <stdio.h>
#include <windows.h>

DWORD static f(LPVOID p)
{
	printf("localeconv() = %p\n", localeconv());
}

int
main(int argc, char *argv[])
{
	HANDLE threads[2];
	DWORD thread_ids[2];
	
	threads[0] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[0]);
	threads[1] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[1]);
	WaitForMultipleObjects(2, threads_ids, true, INIFINITE);
	CLoseHandle(threads[0]);
	CLoseHandle(threads[1]);
	return EXIT_SUCCESS;
}

