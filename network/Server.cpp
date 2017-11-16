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
, timeval_(-1)
{
    HashList_init(socket_list_, CONNECTION_LIMIT);
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
}

int32_t IServer::addSocket(int32_t fd, Socket* sock)
{
    if (sock == NULL)
    {
        // create socket
        sock = Socket::create();
        sock->init(fd);
    }

    HashList_addElement(socket_list_, fd, sock);

    return 0;
}

int32_t IServer::delSocket(int32_t fd)
{
    // del socket
    Socket* sock = getSocket(fd);
    if (sock)
    {
        sock->close();
        HashList_delElement(socket_list_, fd);

        return 0;
    }
    else
    {
        return -1;
    }
}

Socket* IServer::getSocket(int32_t fd)
{
    return (Socket*)HashList_getElement(socket_list_, fd);
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

    int index = 0;
    while (index = HashList_next(socket_list_, index))
    {
        Socket* sock = (Socket*)HashList_getElement(socket_list_, index);
        if (sock)
            sock->close();
    }

    HashList_init(socket_list_, CONNECTION_LIMIT);
    return 0;
}


}

