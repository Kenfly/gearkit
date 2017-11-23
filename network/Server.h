#ifndef __KIT_SERVER_H__
#define __KIT_SERVER_H__

#include "Ref.h"
#include "netsys.h"
#include "Queue.h"
#include "Array.h"

namespace kit {

const uint16_t CONNECTION_LIMIT = 0x7FFF;
const uint16_t SERVER_EVENT_CNT = 512;

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
protected:
    virtual int32_t addSocket(int32_t fd, Socket* sock = NULL);
    virtual int32_t delSocket(int32_t fd);
    Socket* getSocket(int32_t fd);

protected:
	Socket* socket_;
    bool active_;

    // socket list
    Array<Socket*, CONNECTION_LIMIT> socket_array_;

    // event que
    typedef Queue<PollEvent, SERVER_EVENT_CNT> EventQue;
    EventQue event_que_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Server_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Server_win.h>
#endif

#endif
