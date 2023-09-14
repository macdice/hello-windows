// modified https://social.msdn.microsoft.com/Forums/en-US/18769abd-fca0-4d3c-9884-1a38ce27ae90/wsapoll-and-nonblocking-connects-to-nonexistent-ports?forum=wsk

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2ipdef.h>
typedef SOCKET sock_t;
#define SOCKERR GetLastError()
#define sockpoll WSAPoll
#define SOCKCONNBLOCK WSAEWOULDBLOCK
#define SOCKCONNREFUSED WSAECONNREFUSED
#else
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#define sockpoll poll
#define SOCKCONNBLOCK EINPROGRESS
#define SOCKCONNREFUSED ECONNREFUSED
#define SOCKERR errno
typedef int sock_t;
#endif

#define ASSERT(x) \
    do \
    {    \
        if (!(x))    \
        {    \
            printf("ASSERT(%s) failed\n", #x);    \
            exit(1);    \
        }    \
    } while (0)

void printFlag(char* name, short value, int flag)
{
    printf("%s =\t%#010x\n", name, value & flag);
}

void printAllFlags(const char* header, short revents)
{
    printf(header);
    printFlag("POLLRDNORM", revents, POLLRDNORM);
    printFlag("POLLRDBAND", revents, POLLRDBAND);
    printFlag("POLLIN", revents, POLLIN);
    printFlag("POLLPRI", revents, POLLPRI);
    printFlag("POLLWRNORM", revents, POLLRDNORM);
    printFlag("POLLOUT", revents, POLLOUT);
    printFlag("POLLWRBAND", revents, POLLWRBAND);
    printFlag("POLLERR", revents, POLLERR);
    printFlag("POLLHUP", revents, POLLHUP);
    printFlag("POLLNVAL", revents, POLLNVAL);
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    ASSERT(WSAStartup(0x202, &wsaData) == 0);
#endif

    sock_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(sock != (sock_t)-1);

#ifdef _WIN32
    ULONG on = 1;
    ASSERT(ioctlsocket(sock, FIONBIO, &on) != -1);
#else
    int on = 1;
    ASSERT(ioctl(sock, FIONBIO, &on) != -1);
#endif

    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(0xFEFE);
    sa.sin_addr.s_addr = htonl(0x7F000001);

    int ret = connect(sock, (sockaddr*)&sa, sizeof(sa));
    ASSERT(ret == 0 || SOCKERR == SOCKCONNBLOCK ||
        SOCKERR == SOCKCONNREFUSED);
    if (ret != 0 && SOCKERR == SOCKCONNBLOCK)
    {
        printf("Waiting for connect to succeed or fail...\n");
        pollfd fds;
        memset(&fds, 0, sizeof(fds));
        fds.fd = sock;
        /* Setting these flags will fail immediately: POLLPRI POLLWRBAND POLLERR POLLHUP POLLNVAL */
        //fds.events = POLLRDNORM | POLLRDBAND | POLLIN /*| POLLPRI*/ | POLLWRNORM | POLLOUT /*| POLLWRBAND*/ /*| POLLERR*/ /*| POLLHUP*/ /*| POLLNVAL*/;
        fds.events = POLLIN | POLLOUT;
        printAllFlags("\nFlags set:\n", fds.events);
        fds.revents = -1;
        ret = sockpoll(&fds, 1, -1);
        printf("\npoll returned: %d\nflags=%x\n", ret, fds.revents);
        printAllFlags("\nFlags received:\n", fds.revents);
        if (ret == -1)
            printf("ERROR: %d\n", SOCKERR);
    }

    return 0;
}
