#ifndef __KIT_PACKET_HANDLER_H__
#define __KIT_PACKET_HANDLER_H__

#include "Ref.h"
#include "Map.h"

#ifndef PROTOCOL_LENGTH
#define PROTOCOL_LENGTH (512)
#endif


namespace kit {

class Packet;

// handler base
struct PacketHandler : public Ref
{
    virtual void onPacket(int32_t sid, Packet* pack) {};
};


// protocol handler
class Protocol;

struct ProtocolHandler : public Ref
{
    virtual void onProtocol(int32_t sid, Protocol* pto) {};
};

// packet -> protocol 
class PacketProtocolHandler : public PacketHandler
{
public:
    KIT_CREATE_FUNC(PacketProtocolHandler)
    PacketProtocolHandler();
    virtual ~PacketProtocolHandler();

    //@override
    virtual void onPacket(int32_t sid, Packet* pack);

    // deal protocol
    virtual void setProtocolHandler(ProtocolHandler* handler);
    virtual ProtocolHandler* getProtocolHandler() const { return protocol_handler_; }

    virtual void registerProtocol(int32_t pid, Protocol* pto);
    virtual void unregisterProtocol(int32_t pid);
    virtual Protocol* getProtocol(int32_t pid);
protected:
    typedef Map<int32_t, Protocol*, PROTOCOL_LENGTH> ProtocolMap;
    ProtocolMap protocol_map_;

    ProtocolHandler* protocol_handler_;
};


} // namespace kit

#endif 

