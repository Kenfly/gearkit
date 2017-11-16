#include "Client_linux.h"
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
#include <pthread.h>
#include "Logger.h"

namespace kit {

const int32_t MAX_LISTEN = 50;

Client::Client()
: poll_fd_(0)
, thread_id_(0)
{
}

Client::~Client()
{
}

void* thread_start(void* p)
{
    if (!p) 
        return (void*)-1;

    Client* client = (Client*)p;
    client->run();

    return 0;
}

int32_t Client::startup(const char* ip, int32_t port)
{
    int ret = IClient::startup(ip, port);
    if (ret < 0)
        return ret;

    active_ = false;

    // epoll
    int32_t poll_fd = epoll_create(1);
    if (poll_fd < 0)
    {
        ERR("[Client](startup) epoll_create error! %s, %d", __FILE__, __LINE__);
        return -5;
    }

    poll_fd_ = poll_fd;

    int32_t sockfd = socket_->getHandle();
    ret = addCtrl(sockfd, EPOLLET | EPOLLOUT | EPOLLIN);
    if (ret < 0)
    {
        ERR("[Client](startup) add ctrl error! %s, %d", __FILE__, __LINE__);
        return -6;
    }

    // thread
    pthread_t pid;
    ret = pthread_create(&pid, NULL, thread_start, (void*)this);
    if (ret < 0)
    {
        ERR("[Client](startup) create thread error! %s, %d", __FILE__, __LINE__);
        return -7;
    }
    thread_id_ = (int64_t)pid;

    return 0;
}

void Client::run()
{
    auto addr = socket_->getAddr();
    while (socket_->connect(addr) < 0)
    {
        kit::sleep(2000);
        DBG("[Client](run) connect ip:%s port:%d", addr->ip.c_str(), addr->port);
    }

    active_ = true;

    LOG("[Client](startup) successfly! ip: %s port: %d", addr->ip.c_str(), addr->port);

    // 该函数线程运行，用event_que_与主线程通信
    // 如果que push失败，会把未处理事件放到events前面
    int32_t cnt = 0;
    int32_t rest = 0;
    int32_t epoll_fd = poll_fd_;

    struct epoll_event events[CLIENT_EVENT_CNT];
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
            DBG("[Client](run) EPOLL WAIT");
            continue;
        }

        cnt = epoll_wait(epoll_fd, events, rest, timeval_);
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
            handleEvents(ev.events);
            //事件通知主线程作处理
            out_ev.events = ev.events;
            out_ev.fd = ev.data.fd;
            if (!que.push(out_ev))
            {
                DBG("[Client](run) EventQue push ERROR!");
                break;
            }
        }
    }
}

void Client::update()
{
    // 处理epoll事件
    PollEvent ev;
    int32_t cnt = event_que_.count();

    for (int i = 0; i != cnt; ++i)
    {
        if (!event_que_.pop(ev))
            break;
        DBG("[Client](update) poll event fd=%d", ev.fd);
    }
}

void Client::handleEvents(int32_t events)
{
    if (events & EPOLLIN)
    {
        DBG("[Client](handleEvents) EPOLLIN");
    }
    if (events & EPOLLOUT)
    {
        DBG("[Client](handleEvents) EPOLLOUT");
    }
    if (events & EPOLLERR)
    {
        // 断开
        //TODO: do some disconnect 
        DBG("[Client](handleEvents) EPOLLERR");
    }
}

int32_t Client::addCtrl(int32_t fd, int32_t events)
{
    // add epoll ctl
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    int32_t ret = epoll_ctl(poll_fd_, EPOLL_CTL_ADD, fd, &ev);
    return ret;
}

int32_t Client::delCtrl(int32_t fd)
{
    // del epoll ctl
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = 0;
    ev.data.fd = fd;
    int32_t ret = epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd, &ev);
    return ret;
}

int32_t Client::shutdown()
{
    IClient::shutdown();
    if (thread_id_)
    {
        // 等待处理线程结束
        pthread_join((pthread_t)thread_id_, NULL);
        thread_id_ = 0;
    }

    LOG("[Client](shutdown) successfly!");
    return 0;
}


}

