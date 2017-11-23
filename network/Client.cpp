#include "Client.h"
#include "netsys.h"
#include "Socket.h"
#include "SockAddr.h"
#include "Logger.h"

namespace kit {

const int32_t MAX_LISTEN = 50;

IClient::IClient()
: socket_(NULL)
, active_(false)
{
}

IClient::~IClient()
{
    if (socket_ != NULL)
    {
        socket_->release();
        socket_ = NULL;
    }

}

int32_t IClient::startup(const char* ip, int32_t port)
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
        ERR("[IClient](startup) sockaddr error! %s, %d", ip, port);
        return -1;
    }

    // 创建socket
    Socket* sock = Socket::create();
    sock->init(KIT_AF_INET, KIT_SOCK_STREAM, 0);

    if (!sock->valid())
    {
        ERR("[IClient](startup) socket error! %s, %d", ip, port);
        return -2;
    }

    sock->setAddr(addr);

    // connect交付到线程去做
    /*
    if (sock->connect(addr) < 0)
    {
        ERR("[IClient](startup) connect error! %s, %d", ip, port);
        return -3;
    }
    */

    sock->retain();
    socket_ = sock;

    active_ = true;

    return 0;
}

void IClient::update()
{
}

int32_t IClient::shutdown()
{
    active_ = false;
	if (socket_)
	{
		socket_->close();
        socket_->release();
		socket_ = NULL;
	}

    return 0;
}

void IClient::sendPacket(Buffer* buf)
{
    socket_->sendPacket(buf);
}

}

