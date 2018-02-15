#include "Session.h"
#include "Socket.h"
#include "Packet.h"

namespace kit {

SessionID Session::session_id_count_ = 0;

Session::Session()
: sid_(++session_id_count_)
, time_(0)
, socket_(NULL)
{
}

Session::~Session()
{
}

void Session::init(Socket* sock)
{
    setSocket(sock);
}

void Session::setSocket(Socket* sock)
{
    if (socket_)
        socket_->release();
    socket_ = sock;
    socket_->retain();

    time_ = kit::time();
}

bool Session::sendPacket(Packet* pack)
{
    // 判断是放到缓存列队，还是直接给socket发送
    if (send_packets_.count() == 0 && socket_ && socket_->valid())
    {
        int32_t ret = socket_->sendPacket(pack);
        return ret == 0;
    } else {
        send_packets_.push(pack);
        return false;
    }
}

bool Session::recvPacket(Packet* pack)
{
    KIT_SAFE_RETAIN(pack);
    recv_packets_.push(pack);
    return true;
}

bool Session::flush()
{
    Packet* pk = nullptr;
    while (!send_packets_.front(pk))
    {
        if (socket_->sendPacket(pk) != 0)
            return false;
        send_packets_.pop(pk);
    }
	return true;
}

} // namespace kit

