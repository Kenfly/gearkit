#ifndef __KIT_SOCKET_H__
#define __KIT_SOCKET_H__

#include "netbase.h"

#define DSOCKERR		-1

struct sockaddr;

namespace kit {

class Socket
{
public:
	Socket();
	virtual ~Socket();

	void init(int32_t family, int32_t type, int32_t protocol);

	void open();
	void close();

	int32_t connect(const sockaddr* addr);
	int32_t shutdown(int32_t mode);
	int32_t bind(const sockaddr* addr);
	int32_t listen(int32_t count);
	int32_t accept(sockaddr* addr);
	int32_t getErrno();
    bool valid() { return sock_ != DSOCKERR; };

	int32_t send(const char* buf, int32_t size, int32_t mode);
	int32_t recv(char* buf, int32_t size, int32_t mode);
	int32_t sendTo(const char* buf, int32_t size, int32_t mode, const sockaddr *_addr);
	int32_t recvFrom(char* buf, int32_t size, int32_t mode, sockaddr *_addr);

	int32_t ioctl(unsigned long cmd, char* argp);
	int32_t setOption(int32_t level, int32_t optname, const char* optval, int32_t optlen);
	int32_t getOption(int32_t level, int32_t optname, char* optval, int32_t* optlen);

	void setHandle(int32_t _sock);
	inline int32_t getHandle() const { return sock_; }
protected:
	int32_t sock_;
};

} // namespcae kit

#endif

