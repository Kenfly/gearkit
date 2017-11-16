#ifndef __KIT_SERVER_LINUX_H__
#define __KIT_SERVER_LINUX_H__

#include "Server.h"
#include "netsys.h"
#include "Queue.h"
#include <sys/epoll.h>

namespace kit {

const uint16_t SERVER_EVENT_CNT = 512;

class Server : public IServer
{
public:
    KIT_CREATE_FUNC(Server)

	Server();
    virtual ~Server();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 异步wait
    void run();
    // 每帧调用
    virtual void update();
private:
    int32_t addCtrl(int32_t fd, int32_t events);
    int32_t delCtrl(int32_t fd);

    //成功返回0，异常返回-1
    int32_t handleSocket(Socket* sock, int32_t events);
private:
    int32_t poll_fd_;
    int64_t thread_id_;

    // event que
    typedef Queue<PollEvent, SERVER_EVENT_CNT> EventQue;
    EventQue event_que_;
};

} // namespcae kit

#endif
