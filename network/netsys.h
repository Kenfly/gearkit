#ifndef __KIT_NET_SYS_H__
#define __KIT_NET_SYS_H__

#ifdef PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define KIT_AF_INET AF_INET
#define KIT_SOCK_STREAM SOCK_STREAM

#define KIT_POLLIN EPOLLIN
#define KIT_POLLOUT EPOLLOUT
#define KIT_POLLERR EPOLLERR


#elif defined PLATFORM_WINDOWS

#define KIT_AF_INET AF_INET
#define KIT_SOCK_STREAM SOCK_STREAM

#endif 

#include "kitsys.h"

namespace kit {

const uint32_t PACKET_SIZE = 512;

struct PollEvent {
    int32_t events;
    int32_t fd;
};

} // namespace kit

#endif
