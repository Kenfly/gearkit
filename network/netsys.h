#ifndef __KIT_NET_SYS_H__
#define __KIT_NET_SYS_H__

#ifdef PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define KIT_AF_INET AF_INET
#define KIT_SOCK_STREAM SOCK_STREAM

#elif defined PLATFORM_WINDOWS

#define KIT_AF_INET AF_INET
#define KIT_SOCK_STREAM SOCK_STREAM

#endif 

#include "kitsys.h"

namespace kit {

struct PollEvent {
    int32_t events;
    int32_t fd;
};

} // namespace kit

#endif
