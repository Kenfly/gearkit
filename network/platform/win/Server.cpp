#include "Server.h"
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
: socket_(NULL)
{
}

Server::~Server()
{
}

void Server::startup(const char* ip, int32_t port)
{
	if (socket_)
	{
		shutdown();
	}

    // 创建sockaddr
    SockAddr addr;
    addr.init(ip, port, AF_INET);
    if (!addr.valid())
    {
        ERR("[Server](starup) sockaddr error! %s, %d", ip, port);
        return;
    }

    // 创建socket
	socket_ = new Socket();
    socket_->init(AF_INET, SOCK_STREAM, 0);
    if (!socket_->valid())
    {
        ERR("[Server](starup) socket error! %s, %d", ip, port);
        return;
    }

    // socket bind
    if (socket_->bind(addr) < 0)
    {
        ERR("[Server](starup) bind error! %s, %d", ip, port);
        return;
    }

    // socket listen
    if (socket_->listen(MAX_LISTEN) < 0)
    {
        ERR("[Server](starup) listen error! %s, %d", ip, port);
        return;
    }

    return;
}

void Server::shutdown()
{
	if (socket_)
	{
		socket_->close();
        delete socket_;
		socket_ = NULL;
	}

    /*
	for (int i = 0; i < CONNECTION_LIMIT; ++i)
	{
		if (socket_list_[i] != NULL)
		{
			socket_list_[i]->close();
			socket_list_[i] = NULL;
		}
	}
    */
}

}

