#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <mutex>
#include <fcntl.h>

#define ERROR -1
#define INVALID_SOCKET -1

class Address
{
protected:
    sockaddr_in addr;
    socklen_t sock_len;

public:
    Address();
    Address(const char *ip, int port);

    friend class TCP;
};

class TCP
{
private:
    static void error(const char *msg);
    static void warning(const char *msg);

protected:
    int sock_fd, backlog;
    Address addr_info;
    std::mutex tcp_mutex;

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
    int setopt(int sockfd, int level, int opt_name, const void *optval, socklen_t optlen);
    int getopt(int sockfd, int level, int opt_name, void *optval, socklen_t *optlen);
    void non_blocking();
    int write(const char *msg, size_t msg_len);
    int read(char *buff, size_t buff_len);

    friend class Address;
};

#endif