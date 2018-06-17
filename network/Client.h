#ifndef __KIT_CLIENT_H__
#define __KIT_CLIENT_H__

#include "Terminal.h"
#include "netsys.h"
#include "Queue.h"

namespace kit {

class ClientStateMgr;
class Socket;
class Packet;

#ifndef CLIENT_EVENT_LENGTH
#define CLIENT_EVENT_LENGTH 128
#endif

class IClient : public Terminal
{
public:
	IClient();
    virtual ~IClient();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update();
    void sendProtocol(const Protocol* pto);

    Socket* getSocket(void) const { return socket_; }
protected:
    virtual bool baseInit();
    void handlePollEvent();
    void handleSocketRecv(Socket* sock);
private:
    Packet* pk_C_CONNECT();
protected:
    ClientStateMgr* state_mgr_;
	Socket* socket_;
    Session* session_;

    bool active_;

    // event que
    typedef Queue<PollEvent, CLIENT_EVENT_LENGTH> EventQue;
    EventQue event_que_;
};

} // namespcae kit

#ifdef PLATFORM_LINUX
#include "platform/linux/Client_linux.h"
#elif defined PLATFORM_WINDOWS
#include "platform/win/Client_win.h"
#endif

#endif
