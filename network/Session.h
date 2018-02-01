#ifndef __KIT_SESSION_H__
#define __KIT_SESSION_H__

#include "kitsys.h"
#include "netsys.h"
#include "Ref.h"
#include "List.h"
#include "Socket.h"

namespace kit {

class Packet;

class Session : public Ref
{
public:
    static SessionID session_id_count_;
    KIT_CREATE_FUNC(Session)
    Session();
    virtual ~Session();

    void init(Socket* sock);

    SessionID getID() const { return sid_; }

    void setSocket(Socket* sock);
    Socket* getSocket() const { return socket_; }

    bool sendPacket(Packet* pack);
    bool recvPacket(Packet* pack);

    PacketQue& getPackets() { return recv_packets_; }
protected:
    SessionID sid_;
    // 连接上socket的时间
    uint32_t time_;
    // 关联的socket
    Socket* socket_;
    // 待发送的包
    PacketQue send_packets_;
    // 待处理的包
    PacketQue recv_packets_;
};

} // namespace kit

#endif

