#ifndef __KIT_CLIENT_LINUX_H__
#define __KIT_CLIENT_LINUX_H__

#include "Client.h"
#include "Thread.h"
#include <sys/epoll.h>

namespace kit {

class Client : public IClient, ThreadHandler
{
public:
    KIT_CREATE_FUNC(Client)

	Client();
    virtual ~Client();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update();
protected:
    int32_t addCtrl(int32_t fd, int32_t events);
    int32_t delCtrl(int32_t fd);

    void handleEvents(int32_t events);
    void handleThread();
private:
    int32_t poll_fd_;
    Thread* thread_;
};

} // namespcae kit

#endif
