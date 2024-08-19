#include <locale.h>
#include <stdio.h>
#include <windows.h>

DWORD WINAPI f(LPVOID p)
{
	printf("localeconv() = %p\n", localeconv());
	return 0;
}

int
main(int argc, char *argv[])
{
	HANDLE threads[2];
	DWORD thread_ids[2];
	
	threads[0] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[0]);
	threads[1] = CreateThread(NULL, 0, f, NULL, 0, &thread_ids[1]);
	WaitForMultipleObjects(2, threads, true, INIFINITE);
	CLoseHandle(threads[0]);
	CLoseHandle(threads[1]);
	return EXIT_SUCCESS;
}

