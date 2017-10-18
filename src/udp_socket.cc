#include <unistd.h>
#include <cstring>
#include <iostream>

#include "exceptions.h"
#include "udp_socket.h"

cmd::bound_udp_socket::bound_udp_socket(const cmd::address_list &addrs) : sock_fd{-1}
{
    // Loop through each address trying each
    for (auto addr = addrs.addrs; addr != nullptr; addr = addr->ai_next) {
        sock_fd = ::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock_fd == -1)
            continue;

        if (::bind(sock_fd, addr->ai_addr, addr->ai_addrlen) != -1) {
            this->addr = *addr;
            return;  // Success!
        }

        ::close(sock_fd);  // close, try next
    }
    throw cmd::socket_exception("Could bind socket to host: " + addrs.host);
}

cmd::bound_udp_socket::~bound_udp_socket()
{
    close();
}

void cmd::bound_udp_socket::close()
{
    if (sock_fd >= 0)
        ::close(sock_fd);
    sock_fd = -1;
}

ssize_t cmd::bound_udp_socket::send(const cmd::address &addr, const void *buffer, size_t size,
                                    int flags)
{
    return sendto(sock_fd, buffer, size, flags, addr.ai_addr, addr.ai_addrlen);
}

ssize_t cmd::bound_udp_socket::send(const cmd::address &addr, const std::string &str, int flags)
{
    return send(addr, str.c_str(), str.size(), flags);
}

ssize_t cmd::bound_udp_socket::send(const void *buffer, size_t size, int flags)
{
    return ::send(sock_fd, buffer, size, flags);
}

ssize_t cmd::bound_udp_socket::send(const std::string &str, int flags)
{
    return send(str.c_str(), str.size(), flags);
}

ssize_t cmd::bound_udp_socket::recv(void *buffer, size_t size, int flags)
{
    return ::recv(sock_fd, buffer, size, flags);
}

ssize_t cmd::bound_udp_socket::recv(std::vector<unsigned char> &buf, int flags)
{
    return recv(buf.data(), buf.size(), flags);
}

const cmd::address cmd::bound_udp_socket::get_address() const {
    return addr;
}

cmd::address_list::address_list(int port) : host{"localhost"}
{
    struct addrinfo hints {
    };
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;   // Request datagram
    hints.ai_protocol = IPPROTO_UDP;  // UDP
    hints.ai_flags = AI_PASSIVE;
    int ret = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &addrs);
    if (ret != 0) {
        throw cmd::socket_exception("Error getaddrinfo: " + std::string(gai_strerror(ret)));
    }
}

cmd::address_list::address_list(const std::string &host, int port) : host{host}
{
    struct addrinfo hints {
    };
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // Use IPv4 or IPv6
    hints.ai_socktype = SOCK_DGRAM;   // Request datagram
    hints.ai_protocol = IPPROTO_UDP;  // UDP
    int ret = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &addrs);
    if (ret != 0) {
        throw cmd::socket_exception("Error getaddrinfo: " + std::string(gai_strerror(ret)));
    }
}

cmd::address_list::~address_list()
{
    if (addrs)
        freeaddrinfo(addrs);
}

cmd::udp_socket::udp_socket() : sock_fd{-1}
{
    sock_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_fd == -1)
        throw cmd::socket_exception("Could not create IPv4 socket for UDP");
}

cmd::udp_socket::~udp_socket()
{
    close();
}

void cmd::udp_socket::close()
{
    if (sock_fd >= 0)
        ::close(sock_fd);
    sock_fd = -1;
}

ssize_t cmd::udp_socket::send(const cmd::address &addr, const void *buffer, size_t size, int flags)
{
    return ::sendto(sock_fd, buffer, size, flags, addr.ai_addr, addr.ai_addrlen);
}

ssize_t cmd::udp_socket::send(const cmd::address &addr, const std::string &str, int flags)
{
    return send(addr, str.c_str(), str.size(), flags);
}
