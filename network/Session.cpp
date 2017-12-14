#include "Session.h"
#include "Socket.h"

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
}

void Session::update()
{
}

void Session::setSocket(Socket* sock)
{
    if (socket_)
        socket_->release();
    socket_ = sock;
    socket_->retain();
    time_ = kit::time();
}

int32_t Session::sendPacket(Packet* pack)
{
    // 判断是放到缓存列队，还是直接给socket发送
    if (send_packets_.count() > 0 || !socket_ || !socket_->valid())
    {
        send_packets_.push(pack);
        return 1;
    }
    else
    {
    }
}

int32_t Session::recvPacket(Packet*& pack)
{
}

} // namespace kit
