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

	/* Server starts listening for connections. */
	unlink(sa.sun_path);
	listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(listen_socket != (sock_t) -1);
	assert(bind(listen_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);
	assert(listen(listen_socket, 5) == 0);

	printf("=== synchronous ===\n");

	/* Client connects. */
	client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(client_socket != (sock_t) -1);
	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	/* Server accepts connection, sends GOODBYE and hangs up. */
	server_socket = accept(listen_socket, NULL, NULL);
	assert(server_socket != (sock_t) -1);
	assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
	closesocket(server_socket);

	/* Client sends query, then reads response. */
	r = send(client_socket, SELECT, sizeof(SELECT), 0);
	error = get_error(r);
	printf("send -> %d, error = %d\n", r, error);
	r = recv(client_socket, buffer, sizeof(buffer), 0);
	error = get_error(r);
	printf("recv -> \"%.*s\", error = %d\n", r > 0 ? r : 0, buffer, error);

	closesocket(client_socket);

#ifdef __FreeBSD__

	printf("=== posix aio ===\n");

	/* Client connects. */
	client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	assert(client_socket != (sock_t) -1);
	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	{
		/* Client starts receiving asynchronously. */
		struct aiocb aiocb = {
			.aio_fildes = client_socket,
			.aio_buf = buffer,
			.aio_nbytes = sizeof(buffer),
			.aio_sigevent = { .sigev_notify = SIGEV_NONE }
		};
		struct aiocb *aiocb_done;
		assert(aio_read(&aiocb) == 0);

		/* Server accepts connection, sends GOODBYE and hangs up. */
		server_socket = accept(listen_socket, NULL, NULL);
		assert(server_socket != (sock_t) -1);
		assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
		closesocket(server_socket);

		/* Client tries to send a query, and reaps async response. */
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

#ifdef _WIN32

	printf("=== windows overlapped ===\n");

	/* Client connects. */
	client_socket = WSASocket(AF_UNIX, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	assert(client_socket != (sock_t) -1);
	assert(connect(client_socket, (struct sockaddr *) &sa, sizeof(sa)) == 0);

	{
		/* Client starts receiving asynchronously. */
		WSAOVERLAPPED overlapped = {.hEvent = WSACreateEvent()};
		WSABUF wbuffer = {
			.buf = buffer,
			.len = sizeof(buffer)
		};
		DWORD flags;
		DWORD transferred;
		BOOL result;
		assert(WSARecv(client_socket, &wbuffer, 1, NULL, 0, &overlapped, NULL) == SOCKET_ERROR);
		printf("error %d\n", WSAGetLastError());
		assert(WSAGetLastError() == WSA_IO_PENDING);

		/* Server accepts connection, sends GOODBYE and hangs up. */
		server_socket = accept(listen_socket, NULL, NULL);
		assert(server_socket != (sock_t) -1);
		assert(send(server_socket, GOODBYE, sizeof(GOODBYE), 0) == sizeof(GOODBYE));
		closesocket(server_socket);

		/* Client tries to send a query, and reaps async response. */
		r = send(client_socket, SELECT, sizeof(SELECT), 0);
		error = get_error(r);
		printf("send -> %d, error = %d\n", r, error);
		result = WSAGetOverlappedResult(client_socket, &overlapped, &transferred, TRUE, &flags);
		error = WSAGetLastError();
		printf("async recv -> \"%.*s\", error = %d\n", result ? transferred : 0, buffer, error);
	}

	closesocket(client_socket);

#endif

	closesocket(listen_socket);
    return 0;
}
