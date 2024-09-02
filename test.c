#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <afunix.h>
typedef SOCKET sock_t;
#pragma comment(lib, "ws2_32.lib") 
#else
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
typedef int sock_t;
#define closesocket close
#endif

#ifdef __FreeBSD__
#include <aio.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define SOCKET_PATH "c:\\foo"
#else
#define SOCKET_PATH "/tmp/foo"
#endif
#define GOODBYE "FATAL: flux capacitor failed"
#define SELECT "SELECT 1 + 1;"

static int
get_error(int r)
{
#ifdef _WIN32
	return r < 0 ? WSAGetLastError() : 0;
#else
	return r < 0 ? errno : 0;
#endif
}

int main()
{
	const unsigned long one = 1;
    struct sockaddr_un sa = {
		.sun_family = AF_UNIX,
		.sun_path = SOCKET_PATH
	};
	char buffer[1024];
	sock_t listen_socket;
	sock_t server_socket;
	sock_t client_socket;
	int error;
	int r;

#ifdef _WIN32
    WSADATA wsaData;
    assert(WSAStartup(0x202, &wsaData) == 0);
#else
	signal(SIGPIPE, SIG_IGN);
#endif

#if 0
	memset(&sa, 0, sizeof(sa));
	sa.sun_family = AF_UNIX;
	strncpy(sa.sun_path, sizeof(sa.sun_path) - 1, SOCKET_PATH);
#endif

	unlink(sa.sun_path);

    listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    assert(listen_socket != (sock_t) -1);
	assert(bind(listen_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);
	assert(listen(listen_socket, 5) == 0);


	/*=================================================================
	 * Synchronous
	 *=================================================================*/

	printf("=== synchronous sockets ===\n");

	client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(client_socket != (sock_t) -1);
	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	server_socket = accept(listen_socket, NULL, NULL);
	assert(server_socket != (sock_t) -1);
	assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
	closesocket(server_socket);

	r = send(client_socket, SELECT, sizeof(SELECT), 0);
	error = get_error(r);
	printf("send -> %d, error = %d\n", r, error);
	r = recv(client_socket, buffer, sizeof(buffer), 0);
	error = get_error(r);
	printf("recv -> \"%.*s\", error = %d\n", r > 0 ? r : 0, buffer, error);

	closesocket(client_socket);

#ifdef __FreeBSD__

	/*=================================================================
	 * Same sort of thing on FreeBSD.  (I just wrote this so I could
	 * get the program structure right and then translate it to
	 * Windowsian via CI...)
	 *=================================================================*/

	printf("=== posix aio ===\n");

	client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(client_socket != (sock_t) -1);
	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	{
		struct aiocb aiocb = {
			.aio_fildes = client_socket,
			.aio_buf = buffer,
			.aio_nbytes = sizeof(buffer),
			.aio_sigevent = { .sigev_notify = SIGEV_NONE }
		};
		struct aiocb *aiocb_done;
		assert(aio_read(&aiocb) == 0);

		server_socket = accept(listen_socket, NULL, NULL);
		assert(server_socket != (sock_t) -1);
		assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
		closesocket(server_socket);

		r = send(client_socket, SELECT, sizeof(SELECT), 0);
		error = get_error(r);
		printf("send -> %d, error = %d\n", r, error);
		r = aio_waitcomplete(&aiocb_done, NULL);
		assert(aiocb_done == &aiocb);
		error = r < 0 ? errno : 0;
		printf("async recv -> \"%.*s\", error = %d\n", r > 0 ? r : 0, buffer, error);
	}

	closesocket(client_socket);

#endif


	closesocket(listen_socket);
    return 0;
}
