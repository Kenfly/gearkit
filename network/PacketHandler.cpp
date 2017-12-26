#include "PacketHandler.h"
#include "Packet.h"
#include "Protocol.h"
#include "msg.h"
#include "Logger.h"

namespace kit {


PacketProtocolHandler::PacketProtocolHandler()
: protocol_handler_(NULL)
{
}

PacketProtocolHandler::~PacketProtocolHandler()
{
}

void PacketProtocolHandler::onPacket(int32_t sid, Packet* pack)
{
    uint16_t pto_id = pack->getPID() - PT_PACKET_ID_MAX;
    Protocol* pto = NULL;
    if (protocol_map_.get(pto_id, pto))
    {
        pto->unserialize(pack->getBuffer());
        // TODO: handle protocol
    }
    else
    {
        LOG("[PacketProtocolHandler](onPacket) no protocol: %d\n", pto_id);
    }
}

void PacketProtocolHandler::setProtocolHandler(ProtocolHandler* handler)
{
    KIT_SAFE_RELEASE(protocol_handler_)
    protocol_handler_ = handler;
    KIT_SAFE_RETAIN(protocol_handler_);
}

void PacketProtocolHandler::registerProtocol(int32_t pid, Protocol* pto)
{
    unregisterProtocol(pid);
    protocol_map_.set(pid, pto);
    KIT_SAFE_RETAIN(pto)
}

void PacketProtocolHandler::unregisterProtocol(int32_t pid)
{
    Protocol* pto = NULL;
    if (protocol_map_.get(pid, pto))
    {
        KIT_SAFE_RELEASE(pto);
        protocol_map_.del(pid);
    }
}

Protocol* PacketProtocolHandler::getProtocol(int32_t pid)
{
    Protocol* pto = NULL;
    protocol_map_.get(pid, pto);
    return pto;
}

} // namespace kit

