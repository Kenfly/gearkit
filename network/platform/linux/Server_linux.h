#ifndef __KIT_SERVER_LINUX_H__
#define __KIT_SERVER_LINUX_H__

#include "Server.h"
#include "netsys.h"
#include "Queue.h"
#include "Thread.h"

namespace kit {


class Server : public IServer, public ThreadHandler
{
public:
    KIT_CREATE_FUNC(Server)

	Server();
    virtual ~Server();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update();
protected:
    int32_t addCtrl(int32_t fd, int32_t events);
    int32_t delCtrl(int32_t fd);

    // 成功返回0，异常返回-1
    int32_t handleSocket(Socket* sock, int32_t events);

    // 线程回调
    virtual void handleThread();
private:
    int32_t poll_fd_;
    Thread* thread_;
};

} // namespcae kit

#endif
