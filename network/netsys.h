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

#define KIT_WOULDBLOCK EWOULDBLOCK
#define KIT_SOCKERR -1


#elif defined PLATFORM_WINDOWS

//FIXME
#define KIT_AF_INET AF_INET
#define KIT_SOCK_STREAM SOCK_STREAM

#define KIT_POLLIN EPOLLIN
#define KIT_POLLOUT EPOLLOUT
#define KIT_POLLERR EPOLLERR

#define KIT_WOULDBLOCK WSAEWOULDBLOCK
#define KIT_SOCKERR -1

#endif 

#include "kitsys.h"

namespace kit {

// 定义session id类型
typedef int32_t SessionID;
// 定义socket id类型
typedef int32_t SocketID;
typedef int32_t ProtocolID;

const SessionID DSESSIONERR = -1;
const SessionID SIDNULL = 0;
const ProtocolID DPROTOCOLERR = -1;
const ProtocolID PIDNULL = 0;
const SocketID DSOCKERR = -1;

const uint32_t PACKET_SIZE = 512;

struct PollEvent {
    int32_t events;
    int32_t fd;
};

} // namespace kit

#endif
