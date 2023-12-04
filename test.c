#include <stdio.h>
#include <ws2tcpip.h>

#define F(x) printf(#x " = \"%s\"\n", gai_strerror(x))

int main()
{
	F(EAI_AGAIN);
	F(EAI_BADFLAGS);
	F(EAI_FAIL);
	F(EAI_FAMILY);
	F(EAI_MEMORY);
	F(EAI_NONAME);
	F(EAI_SERVICE);
	F(EAI_SOCKTYPE);
	F(42);
    return 0;
}
