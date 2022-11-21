#include "TcpSocket.hpp"

Address::Address()
    : sock_len(sizeof(sockaddr_in))
{
    memset(&addr, 0, sock_len);
}

Address::Address(const char *ip, int port)
    : sock_len(sizeof(sockaddr_in))
{
    memset(&addr, 0, sock_len);
    int result = inet_pton(AF_INET, ip, &(addr.sin_addr));
    switch (result)
    {
    case -1:
        TCP::error("Address Error");
        break;

    case 0:
        TCP::warning("Address InetPton Warning");
        break;

    default:
        break;
    }
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
}

Address &Address::operator=(const Address &__o)
{
    return *(Address *)(memcpy(this, &__o, sizeof(Address)));
}

void TCP::error(const char *msg)
{
    printf("%s: #%d\n", msg, errno);
    exit(-1);
}

void TCP::warning(const char *msg)
{
    printf("%s: #%d\n", msg, errno);
}

TCP::TCP()
    : sock_fd(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)), addr_info(Address()), backlog(0)
{
#ifdef _WIN32
    blocking();
#endif
}

TCP::TCP(const TCP &__o)
    : sock_fd(__o.sock_fd), addr_info(__o.addr_info), backlog(0)
{
#ifdef _WIN32
    blocking();
#endif
}

TCP &TCP::operator=(const TCP &__o)
{
    if (sock_fd != -1)
#ifdef __linux__
        close(sock_fd);
#endif
#ifdef _WIN32
        closesocket(sock_fd);
#endif
    sock_fd = __o.sock_fd;
    addr_info = __o.addr_info;
    backlog = __o.backlog;
}

void TCP::operator<<(const Address &addr)
{
    addr_info = addr;
    if (bind(sock_fd, (sockaddr *)&addr_info.addr, addr_info.sock_len) == ERROR)
        error("Bind Error");
    if (listen(sock_fd, backlog) == ERROR)
        error("Listen Error");
}

void TCP::operator>>(const Address &addr)
{
    addr_info = addr;
    if (connect(sock_fd, (sockaddr *)&addr_info.addr, addr_info.sock_len) == ERROR)
        warning("Connect Error");
}

void TCP::operator>>(TCP &__o)
{
    if (__o.sock_fd != INVALID_SOCKET)
#ifdef __linux__
        close(sock_fd);
#endif
#ifdef _WIN32
        closesocket(sock_fd);
#endif
    __o.sock_fd = accept(sock_fd, (sockaddr *)&__o.addr_info.addr, &__o.addr_info.sock_len);
}

void TCP::allocate_socket()
{
    if (sock_fd != INVALID_SOCKET)
#ifdef __linux__
        close(sock_fd);
#endif
#ifdef _WIN32
        closesocket(sock_fd);
#endif
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void TCP::close_socket()
{
    if (sock_fd == INVALID_SOCKET)
        return;
#ifdef __linux__
        close(sock_fd);
#endif
#ifdef _WIN32
        closesocket(sock_fd);
#endif
    sock_fd = INVALID_SOCKET;
}

void TCP::sd_read()
{
    if (sock_fd == INVALID_SOCKET)
        return;
    shutdown(sock_fd, SHUT_RD);
}

void TCP::sd_write()
{
    if (sock_fd == INVALID_SOCKET)
        return;
    shutdown(sock_fd, SHUT_WR);
}

void TCP::sd_both()
{
    if (sock_fd == INVALID_SOCKET)
        return;
    shutdown(sock_fd, SHUT_RDWR);
}

#ifdef __linux__
int TCP::getopt(int level, int opt_name, void *optval, socklen_t *optlen)
{
    return getsockopt(sock_fd, level, opt_name, optval, optlen);
}

int TCP::setopt(int level, int opt_name, const void *optval, socklen_t optlen)
{
    return setsockopt(sock_fd, level, opt_name, optval, optlen);
}
#endif

#ifdef _WIN32
int TCP::getopt(int level, int opt_name, char *optval, socklen_t *optlen)
{
    return getsockopt(sock_fd, level, opt_name, optval, optlen);
}

int TCP::setopt(int level, int opt_name, const char *optval, socklen_t optlen)
{
    return setsockopt(sock_fd, level, opt_name, optval, optlen);
}

void TCP::blocking()
{
    u_long blockingMode = 0;
    if (ioctlsocket(sock_fd, FIONBIO, &blockingMode) == SOCKET_ERROR)
        error("Blocking Error");
}
#endif

void TCP::non_blocking()
{
#ifdef __linux__
    int flag = fcntl(sock_fd, F_GETFL, 0);
    if (fcntl(sock_fd, F_SETFL, flag | O_NONBLOCK) == -1)
        warning("Non Blocking Error");
#endif
#ifdef _WIN32
    u_long nonBlockingMode = 1;
    if (ioctlsocket(sock_fd, FIONBIO, &nonBlockingMode) == SOCKET_ERROR)
        error("Non Blocking Error");
#endif
}

int TCP::write(const char *msg, size_t msg_len)
{
    return send(sock_fd, msg, msg_len, 0);
}

int TCP::read(char *buff, size_t buff_len)
{
    return recv(sock_fd, buff, buff_len, 0);
}