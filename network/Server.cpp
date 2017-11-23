#include "Server.h"
#include "netsys.h"
#include "Socket.h"
#include "SockAddr.h"
#include "Logger.h"

namespace kit {

const int32_t MAX_LISTEN = 50;

IServer::IServer()
: socket_(NULL)
, active_(false)
{
}

IServer::~IServer()
{
    if (socket_ != NULL)
    {
        socket_->release();
        socket_ = NULL;
    }

}

int32_t IServer::startup(const char* ip, int32_t port)
{
	if (socket_)
	{
		shutdown();
	}

    // 创建sockaddr
    SockAddr* addr = SockAddr::create();
    addr->init(ip, port, KIT_AF_INET);
    if (!addr->valid())
    {
        ERR("[IServer](startup) sockaddr error! %s, %d", ip, port);
        return -1;
    }

    // 创建socket
    Socket* sock = Socket::create();
    sock->init(KIT_AF_INET, KIT_SOCK_STREAM, 0);

    if (!sock->valid())
    {
        ERR("[IServer](startup) socket error! %s, %d", ip, port);
        return -2;
    }

    // socket bind
    if (sock->bind(addr) < 0)
    {
        ERR("[IServer](startup) bind error! %s, %d", ip, port);
        return -3;
    }

    // socket listen
    if (sock->listen(MAX_LISTEN) < 0)
    {
        ERR("[IServer](startup) listen error! %s, %d", ip, port);
        return -4;
    }

    sock->retain();
    socket_ = sock;

    addSocket(sock->getHandle(), sock);

    active_ = true;

    return 0;
}

void IServer::update()
{
    // 处理epoll事件
    PollEvent ev;
    int32_t cnt = event_que_.count();
    int32_t sock_fd;
    int32_t sock_ev;
    Socket* sock;

    for (int i = 0; i != cnt; ++i)
    {
        if (!event_que_.pop(ev))
            break;
        sock_fd = ev.fd;
        sock_ev = ev.events;
        sock = getSocket(sock_fd);
        if (sock)
            continue;
        if (sock_ev & KIT_POLLIN)
        {
            sock->dealRecv();
        }
        if (sock_ev & KIT_POLLOUT)
        {
            int32_t ret = sock->flushSend();
            if (ret == -1)
            {
                sock_ev |= KIT_POLLERR;
            }
        }
        if (sock_ev & KIT_POLLERR)
        {
            // 真正删除
            sock->release();
            socket_array_.del(sock_fd);
        }

        DBG("[Server](update) poll event fd=%d", ev.fd);
    }
}

int32_t IServer::addSocket(int32_t fd, Socket* sock)
{
    if (sock == NULL)
    {
        // create socket
        sock = Socket::create();
        sock->init(fd);
    }

    sock->delete_ = false;
    sock->retain();
    socket_array_.add(fd, sock);
    return 0;
}

int32_t IServer::delSocket(int32_t fd)
{
    // del socket
    Socket* sock = getSocket(fd);
    if (sock)
    {
        sock->close();
        sock->delete_ = true;
        //不在这里删除，避免漏掉线程传过来的消息处理。
        //sock->release();
        //socket_array_.del(fd);
        return 0;
    }
    else
    {
        return -1;
    }
}

Socket* IServer::getSocket(int32_t fd)
{
    Socket* s = NULL;
    socket_array_.get(fd, s);
    return s;
}

int32_t IServer::shutdown()
{
    active_ = false;
	if (socket_)
	{
		socket_->close();
        socket_->release();
		socket_ = NULL;
	}

    Socket* sock = NULL;
    socket_array_.resetNext();
    while (socket_array_.next(sock))
    {
        sock->close();
        sock->release();
    }
    socket_array_.clear();
    return 0;
}


}

