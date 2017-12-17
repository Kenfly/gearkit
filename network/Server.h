#ifndef __KIT_SERVER_H__
#define __KIT_SERVER_H__

//
// 服务器
// 实现方式：
//  1.只开一条线程, 线程处理socket listen/accept/recv/, 然后生成事件列队给主线程
//  2.主线程处理事件。
//  3.主线程与线程均可操作delSocket，所以要加锁，其他地方不需要加锁。
//
//

#include "Terminal.h"
#include "netsys.h"
#include "Queue.h"
#include "Array.h"
#include "Map.h"

namespace kit {

const uint16_t CONNECTION_LIMIT = 0x7FFF;
const uint16_t SERVER_EVENT_CNT = 512;

class Socket;
class Session;
class Mutex;

class IServer : public Terminal
{
public:
	IServer();
    virtual ~IServer();
	virtual int32_t startup(const char* ip, int32_t port);
	virtual int32_t shutdown();

    // 每帧调用
    virtual void update() = 0;
protected:
    virtual bool baseInit();

    // 主线程处理pollevent
    virtual void handlePollEvent();

    virtual int32_t addSocket(SocketID fd, Socket* sock = NULL);
    virtual int32_t delSocket(SocketID fd);
    Socket* getSocket(int32_t fd);

protected:
	Socket* socket_;
    bool active_;
    Mutex* del_socket_mutex_;

    // socket list
    //typedef Array<Socket*, CONNECTION_LIMIT> SocketArray;
    //SocketArray socket_array_;
    Socket* sockets_[CONNECTION_LIMIT];

    // session list
    typedef Map<SessionID, Session*, CONNECTION_LIMIT> SessionMap;
    SessionMap session_map_;

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

