#include <stdio.h>
#include <windows.h>

int
main(int argc, char *argv[])
{
	printf("test-a-client.exe: argv =");
	for (int i = 0; i < argc; ++i)
		printf(" [%s]", argv[i]);
		printf("\n");
	}
    return 0;
}
