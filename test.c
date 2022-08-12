#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <afunix.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

void
doit(size_t length)
{
	SOCKET sock;
	struct sockaddr_un *addr;
	size_t addr_size;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		return;
	}

	addr_size = sizeof(addr->sun_family) + length + 3 + 1; /* "C:/" + NUL */
	addr = malloc(addr_size);
	memset(addr, 0, addr_size);
	addr->sun_family = AF_UNIX;
	memcpy(&addr->sun_path, "C:/", 3);
	for (size_t i = 0; i < length; ++i)
		addr->sun_path[i + 3] = 'x';

	/* make the prefix distinct to find out if error 10048 is because name is chomped at 240...*/
	//if (length > 10)
//		sprintf(&addr->sun_path[4], "%zu", length);

	printf("Can I make a socket with path length %zu? ", length); //addr->sun_path);

	if (bind(sock, (struct sockaddr *) addr, 3 + length + sizeof(addr->sun_family)) == SOCKET_ERROR)
	{
		printf("bind() failed: %d\n", WSAGetLastError());
		return;
	}
	printf("yes!\n");
	//unlink(addr->sun_path); // leave it behind in the file system
	free(addr);
	closesocket(sock);
}

int
main(int argc, char *argv[])
{
	WSADATA wsadata;
	int rc;

	rc = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (rc != 0)
	{
		printf("WSAStartup failed: %d\n", rc);
		return;
	}

	doit(10);
	doit(10);	/* verify that duplicate name fails */
	doit(100);
	for (int i = 240; i < 270; i++)
		doit(i);
	doit(260);
	doit(300);
}

