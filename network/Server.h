#ifndef __KIT_SERVER_H__
#define __KIT_SERVER_H__

#include "Ref.h"
#include "kitsys.h"
#include "HashList.h"

namespace kit {

const uint16_t CONNECTION_LIMIT = 0x7FFF;

class Socket;

class IServer : public Ref
{
public:
	IServer();
    virtual ~IServer();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update() = 0;

    inline int32_t getTimeval() const { return timeval_; }
    inline void setTimeval(int32_t timeval) { timeval_ = timeval; }
protected:
    virtual int32_t addSocket(int32_t fd, Socket* sock = NULL);
    virtual int32_t delSocket(int32_t fd);
    Socket* getSocket(int32_t fd);

protected:
	Socket* socket_;
    bool active_;

    // wait time
    int32_t timeval_;

    // socket list
    HashListDef socket_list_[CONNECTION_LIMIT];
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Server_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Server_win.h>
#endif

#endif
