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
    if (active_)
    {
        shutdown();
    }
    if (socket_ != NULL)
    {
        socket_->release();
        socket_ = NULL;
    }
}

bool IServer::baseInit()
{
    Terminal::baseInit();

    memset(sockets_, 0, sizeof(Socket*) * CONNECTION_LIMIT);

    return true;
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

void IServer::handlePollEvent()
{
    // 处理epoll事件
    PollEvent ev;
    int32_t cnt = event_que_.count();
    SocketID sock_fd;
    int32_t sock_ev;
    Socket* sock;

    for (int i = 0; i != cnt; ++i)
    {
        if (!event_que_.pop(ev))
            break;
        sock_fd = (SocketID)ev.fd;
        sock_ev = ev.events;
        sock = getSocket(sock_fd);
        if (sock == NULL || sock == socket_)
        {
            continue;
        }
        // 客户端消息
        if (sock_ev & KIT_POLLIN)
        {
            //FIXME: 收到协议包处理
            sock->test_packet();
        }
        if (sock_ev & KIT_POLLOUT)
        {
            sock->readyOut_ = true;
            int32_t ret = sock->flushSend();
            if (ret == -1)
            {
                sock_ev |= KIT_POLLERR;
            }
        }
        if (sock_ev & KIT_POLLERR)
        {
            // 真正删除
            delSocket(sock_fd);
            // 告诉session掉线
        }

        DBG("[Server](update) poll event fd=%d", ev.fd);
    }
}

void IServer::update()
{
    Terminal::update();

    handlePollEvent();
}

//@thread
int32_t IServer::addSocket(SocketID fd, Socket* sock)
{
    Socket* old_sock = getSocket(fd);
    if (old_sock)
    {
        // del old
        delSocket(fd);
    }
    if (sock == NULL)
    {
        // create socket
        sock = Socket::create();
        sock->init(fd);
    }

    sock->delete_ = false;
    sock->retain();
    sockets_[fd] = sock;
    return 0;
}

//@thread
int32_t IServer::delSocket(SocketID fd)
{
    // del socket
    Socket* sock = getSocket(fd);
    if (sock)
    {
        sock->close();
        sock->delete_ = true;
        sock->release();
        sockets_[fd] = NULL;
        return 0;
    }
    else
    {
        return -1;
    }
}

Socket* IServer::getSocket(SocketID fd)
{
    return sockets_[fd];
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

    Socket* sock;
    for (int i = 0; i != CONNECTION_LIMIT; ++i)
    {
        sock = sockets_[i];
        if (sock)
        {
            sock->close();
            sock->release();
        }
    }

    return 0;
}

}

