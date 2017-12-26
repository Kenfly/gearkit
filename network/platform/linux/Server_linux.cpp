#include "Server_linux.h"
#include "Socket.h"
#include "SockAddr.h"
#include <unistd.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdlib.h>
#include "Logger.h"

namespace kit {

const int32_t MAX_LISTEN = 50;

Server::Server()
: poll_fd_(0)
, thread_(NULL)
{
}

Server::~Server()
{
}

int32_t Server::startup(const char* ip, int32_t port)
{
    int ret = IServer::startup(ip, port);
    if (ret < 0)
        return ret;

    active_ = false;

    // epoll
    int32_t poll_fd = epoll_create(CONNECTION_LIMIT);
    if (poll_fd < 0)
    {
        ERR("[Server](startup) epoll_create error! %s, %d", __FILE__, __LINE__);
        return -5;
    }

    poll_fd_ = poll_fd;

    int32_t listenfd = socket_->getHandle();
    ret = addCtrl(listenfd, EPOLLET | EPOLLIN);
    if (ret < 0)
    {
        ERR("[Server](startup) add listen socket error! %s, %d", __FILE__, __LINE__);
        return -6;
    }

    active_ = true;

    // thread
    Thread* thread = Thread::create();
    thread->init(this);
    ret = thread->start();
    if (ret < 0)
    {
        ERR("[Server](startup) create thread error! %s, %d", __FILE__, __LINE__);
        return -7;
    }

    thread->retain();
    thread_ = thread;

    LOG("[Server](startup) successfly! ip: %s port: %d", ip, port);
    return 0;
}

void Server::update()
{
    IServer::update();
}

//@thread
int32_t Server::handleSocket(Socket* sock, int32_t events)
{
    if (sock == socket_ && events & EPOLLIN)
    {
        if (events & EPOLLERR)
        {
            DBG("[Server](handleSocket) server disconnect!");
            //TODO:服务器断开
            return -1;
        }

        // accept
        SockAddr* addr = SockAddr::create();
        int32_t fd = socket_->accept(addr);
        if (fd == DSOCKERR)
        {
            ERR("[Server](handleAccept) accept fail!");
            return -1;
        }
        else
        {
            addSocket(fd);
            addCtrl(fd, EPOLLET | EPOLLIN | EPOLLOUT);
            LOG("[Server](handleAccept) accept successfully! fd:%d", fd);

            return 0;
        }
    }

    // 客户端事件
    if (events & EPOLLIN)
    {
        int32_t ret = sock->flushRecv();
        if (ret == -1)
        {
            // 断开
            sock->ready_out = false;
            return -1;
        }
        DBG("[Server](handleSocket) EPOLLIN fd:%d", sock->getHandle());
    }
    if (events & EPOLLOUT)
    {
        // 可以写
        sock->ready_out = true;
        DBG("[Server](handleSocket) EPOLLOUT fd:%d", sock->getHandle());
    }

    if (events & EPOLLERR)
    {
        // 断开
        DBG("[Server](handleSocket) EPOLLERR fd:%d", sock->getHandle());
        return -1;
        //TODO: do some disconnect 
    }
    return 0;
}

// 该函数线程回调，用event_que_与主线程通信
// 只会在线程操作socket函数，所以不用关心线程同步问题
// @thread
void Server::handleThread()
{
    int32_t cnt = 0;
    int32_t rest = 0;
    int32_t epoll_fd = poll_fd_;
    int32_t ret = 0;
    int32_t sock_fd = 0;
    int32_t sock_ev = 0;

    struct epoll_event events[SERVER_EVENT_CNT];
    memset(&events, 0, sizeof(events));

    PollEvent out_ev;
    Socket* socket;
    EventQue& que = event_que_;

    while(active_)
    {
        rest = que.rest();
        if (rest == 0)
        {
            kit::sleep(100);
            DBG("[Server](handleThread) EPOLL WAIT");
            continue;
        }

        cnt = epoll_wait(epoll_fd, events, rest, -1);
        if (cnt == -1 && errno == EINTR)
        {
            // 发生错误，断线
            out_ev.events = EPOLLERR;
            out_ev.fd = socket_->getHandle();
            que.push(out_ev);
            break;
        }

        // 处理epoll事件
        for (int i = 0; i != cnt; ++i)
        {
            const struct epoll_event& ev = events[i];
            sock_fd = ev.data.fd;
            sock_ev = ev.events;

            socket = getSocket(sock_fd);
            if (socket)
                ret = handleSocket(socket, sock_ev);
            else
                ret = -1;
            if (ret == -1)
            {
                delSocket(sock_fd);
                delCtrl(sock_fd);
                sock_ev |= EPOLLERR;
            }

            //事件通知主线程作处理
            out_ev.fd = sock_fd;
            out_ev.events = sock_ev;
            if (!que.push(out_ev))
            {
                // 理论上不可能出现这种情况
                DBG("[Server](handleThread) EventQue push ERROR!");
                break;
            }
        }
        // post
    }
}

//@thread
int32_t Server::addCtrl(int32_t fd, int32_t events)
{
    // add epoll ctl
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    int32_t ret = epoll_ctl(poll_fd_, EPOLL_CTL_ADD, fd, &ev);
    return ret;
}

//@thread
int32_t Server::delCtrl(int32_t fd)
{
    // del epoll ctl
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = 0;
    ev.data.fd = fd;
    int32_t ret = epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd, &ev);
    return ret;
}

int32_t Server::shutdown()
{
    IServer::shutdown();
    KIT_SAFE_RELEASE(thread_);

    LOG("[Server](shutdown) successfly!");
    return 0;
}


}

