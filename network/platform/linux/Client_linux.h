#ifndef __KIT_CLIENT_LINUX_H__
#define __KIT_CLIENT_LINUX_H__

#include "Client.h"
#include "netsys.h"
#include "Queue.h"
#include <sys/epoll.h>

namespace kit {

const uint16_t CLIENT_EVENT_CNT = 128;

class Client : public IClient
{
public:
    KIT_CREATE_FUNC(Client)

	Client();
    virtual ~Client();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 异步wait
    void run();
    // 每帧调用
    virtual void update();
private:
    int32_t addCtrl(int32_t fd, int32_t events);
    int32_t delCtrl(int32_t fd);

    void handleEvents(int32_t events);
private:
    int32_t poll_fd_;
    int64_t thread_id_;

    // event que
    typedef Queue<PollEvent,CLIENT_EVENT_CNT> EventQue;
    EventQue event_que_;
};

} // namespcae kit

#endif
