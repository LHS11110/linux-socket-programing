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
    }
}

void TCP::error(const char *msg)
{
    printf("%s\n", msg);
    exit(-1);
}

void TCP::warning(const char *msg)
{
    printf("%s\n", msg);
}

TCP::TCP()
    : sock_fd(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)), addr_info(Address()), backlog(0)
{
}

TCP::TCP(const TCP &__o)
    : sock_fd(__o.sock_fd), addr_info(__o.addr_info), backlog(0)
{
}

TCP &TCP::operator=(const TCP &__o)
{
    tcp_mutex.lock();
    if (sock_fd != -1)
        close(sock_fd);
    sock_fd = __o.sock_fd;
    addr_info = __o.addr_info;
    backlog = __o.backlog;
    tcp_mutex.unlock();
}

void TCP::operator<<(const Address &addr)
{
    tcp_mutex.lock();
    addr_info = addr;
    if (bind(sock_fd, (sockaddr *)&addr_info.addr, addr_info.sock_len) == ERROR)
    {
        tcp_mutex.unlock();
        error("Bind Error");
    }
    if (listen(sock_fd, backlog) == ERROR)
    {
        tcp_mutex.unlock();
        error("Listen Error");
    }
    tcp_mutex.unlock();
}

void TCP::operator>>(const Address &addr)
{
    tcp_mutex.lock();
    addr_info = addr;
    if (connect(sock_fd, (sockaddr *)&addr_info.addr, addr_info.sock_len) == ERROR)
    {
        tcp_mutex.unlock();
        warning("Connect Error");
        return;
    }
    tcp_mutex.unlock();
}

void TCP::operator>>(TCP &__o)
{
    tcp_mutex.lock();
    __o.sock_fd = accept(sock_fd, (sockaddr *)&__o.addr_info.addr, &__o.addr_info.sock_len);
    tcp_mutex.unlock();
}

void TCP::allocate_socket()
{
    tcp_mutex.lock();
    if (sock_fd != INVALID_SOCKET)
        close(sock_fd);
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    tcp_mutex.unlock();
}

void TCP::close_socket()
{
    tcp_mutex.lock();
    close(sock_fd);
    sock_fd = INVALID_SOCKET;
    tcp_mutex.unlock();
}

void TCP::sd_read()
{
    tcp_mutex.lock();
    if (sock_fd == INVALID_SOCKET)
    {
        tcp_mutex.unlock();
        return;
    }
    shutdown(sock_fd, SHUT_RD);
    tcp_mutex.unlock();
}

void TCP::sd_write()
{
    tcp_mutex.lock();
    if (sock_fd == INVALID_SOCKET)
    {
        tcp_mutex.unlock();
        return;
    }
    shutdown(sock_fd, SHUT_WR);
    tcp_mutex.unlock();
}

void TCP::sd_both()
{
    tcp_mutex.lock();
    if (sock_fd == INVALID_SOCKET)
    {
        tcp_mutex.unlock();
        return;
    }
    shutdown(sock_fd, SHUT_RDWR);
    tcp_mutex.unlock();
}

int TCP::setopt(int sockfd, int level, int opt_name, const void *optval, socklen_t optlen)
{
    tcp_mutex.lock();
    int result = setsockopt(sockfd, level, opt_name, optval, optlen);
    tcp_mutex.unlock();
    return result;
}

int TCP::getopt(int sockfd, int level, int opt_name, void *optval, socklen_t *optlen)
{
    tcp_mutex.lock();
    int result = getsockopt(sockfd, level, opt_name, optval, optlen);
    tcp_mutex.unlock();
    return result;
}

void TCP::non_blocking()
{
    tcp_mutex.lock();
    int flag = fcntl(sock_fd, F_GETFL, 0);
    if (fcntl(sock_fd, F_SETFL, flag | O_NONBLOCK) == -1)
    {
        tcp_mutex.unlock();
        warning("Non Blocking Error");
        return;
    }
    tcp_mutex.unlock();
}

int TCP::write(const char *msg, size_t msg_len)
{
    tcp_mutex.lock();
    int result = send(sock_fd, msg, msg_len, 0);
    tcp_mutex.unlock();
    return result;
}

int TCP::read(char *buff, size_t buff_len)
{
    tcp_mutex.lock();
    int result = recv(sock_fd, buff, buff_len, 0);
    tcp_mutex.unlock();
}