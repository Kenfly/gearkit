#ifndef __KIT_CLIENT_H__
#define __KIT_CLIENT_H__

#include "Terminal.h"
#include "Queue.h"
#include "netsys.h"

namespace kit {

class Socket;
class Packet;
class IPacketHandler;

const uint16_t CLIENT_EVENT_CNT = 128;

class IClient : public Terminal
{
public:
	IClient();
    virtual ~IClient();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update();

    void sendPacket(Packet* buf);
protected:
    virtual void handlePollEvent();
protected:
	Socket* socket_;

    IPacketHandler* packet_handler_;
    bool active_;

    // event que
    typedef Queue<PollEvent,CLIENT_EVENT_CNT> EventQue;
    EventQue event_que_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include <platform/linux/Client_linux.h>
#elif defined PLATFORM_WINDOWS
#include <platform/win/Client_win.h>
#endif

#endif
