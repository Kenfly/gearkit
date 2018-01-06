#include "Terminal.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "Socket.h"
#include "Logger.h"

namespace kit {

Terminal::Terminal()
: packet_handler_(NULL)
{
}

Terminal::~Terminal()
{
    KIT_SAFE_RELEASE(packet_handler_);
}

bool Terminal::baseInit()
{
    return true;
}

void Terminal::handleRecvPackets(Socket* socket)
{
    PacketHandler* handler = packet_handler_;
    if (handler == NULL)
    {
        ERR("[Terminal](handleRecvPackets) no packet handler!");
        return;
    }

    PacketQue& que = socket->getRecvQueue();
    Packet* pack = NULL;
    int cnt = que.count();
    for (int i = 0; i != cnt; ++i)
    {
        if (!que.pop(pack))
            break;
        // TODO: sid
        handler->onPacket(0, pack);
        pack->release();
    }
}

void Terminal::setPacketHandler(PacketHandler* handler)
{
    KIT_SAFE_RELEASE(packet_handler_)
    packet_handler_ = handler;
    KIT_SAFE_RETAIN(packet_handler_)
}

} // namespace kit

