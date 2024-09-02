#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
typedef SOCKET sock_t;
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "/tmp/foo"
#define HELLO "HELLO\n"
#define GOODBYE "FATAL: flux capacitor failed\n"
#define SELECT "SELECT 1 + 1\n"

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
	 * Traditional BSD sockets
	 *=================================================================*/

#ifdef _WIN32
	client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(client_socket != (sock_t) -1);
	assert(ioctlsocket(client_socket, FIONBIO, &one) == 0);
#else
	client_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
	assert(client_socket != (sock_t) -1);
#endif

	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	server_socket = accept(listen_socket, NULL, NULL);
	assert(server_socket != (sock_t) -1);

	assert(send(server_socket, HELLO, sizeof(HELLO), 0) == sizeof(HELLO));

	assert(recv(client_socket, buffer, sizeof(buffer), 0) == sizeof(HELLO));
	assert(strcmp(buffer, HELLO) == 0);

	assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
	closesocket(server_socket);

	r = send(client_socket, SELECT, sizeof(SELECT), 0);
	error = get_error(r);
	printf("send after server closed -> %d, %d\n", r, error);
	r = recv(client_socket, buffer, sizeof(buffer), 0);
	error = get_error(r);
	printf("recv after server closed -> %d, %d\n", r, error);

	closesocket(client_socket);


	closesocket(listen_socket);
    return 0;
}
