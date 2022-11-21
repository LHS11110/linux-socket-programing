#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#ifdef __APPLE__
#define _Linux
#endif
#ifdef __linux__
#define _Linux
#endif

#ifdef _Linux
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define _Linux
#endif
#ifdef _WIN32
#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include <ws2tcpip.h>
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#define socklen_t int
#define _Window
#endif

#define ERR -1

class Address
{
protected:
    sockaddr_in addr;
    socklen_t sock_len;

public:
    Address();
    Address(const char *ip, int port);

    Address &operator=(const Address &);

    friend class TCP;
};

class TCP
{
private:
    static void error(const char *msg);
    static void warning(const char *msg);

protected:
#ifdef _Linux
    int sock_fd, backlog;
    Address addr_info;
#endif
#ifdef _Window
    SOCKET sock_fd;
    int backlog;
    Address addr_info;
#endif

public:
    TCP();
    TCP(const TCP &);

    TCP &operator=(const TCP &);
    void operator<<(const Address &addr);
    void operator>>(const Address &addr);
    void operator>>(TCP &);

    void allocate_socket();
    void close_socket();
    void sd_read();
    void sd_write();
    void sd_both();
#ifdef _Linux
    int getopt(int level, int opt_name, void *optval, socklen_t *optlen);
    int setopt(int level, int opt_name, const void *optval, socklen_t optlen);
    int write(const char *msg, size_t msg_len);
    int read(char *buff, size_t buff_len);
#endif
#ifdef _Window
    int getopt(int level, int opt_name, char *optval, socklen_t *optlen);
    int setopt(int lever, int opt_name, const char *optval, socklen_t optlen);
    void blocking();
    int write(const char *msg, int msg_len);
    int read(char *buff, int buff_len);
#endif
    void non_blocking();

    friend class Address;
};

#endif