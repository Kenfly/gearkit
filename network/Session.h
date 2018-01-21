#ifndef __KIT_SESSION_H__
#define __KIT_SESSION_H__

#include "kitsys.h"
#include "netsys.h"
#include "Ref.h"
#include "List.h"

namespace kit {

class Packet;
class Socket;

class Session : public Ref
{
public:
    static SessionID session_id_count_;
    KIT_CREATE_FUNC(Session)
    Session();
    virtual ~Session();

    void init(Socket* sock);

    //每帖调用
    void update();

    SessionID getID() const { return sid_; }

    void setSocket(Socket* sock);
    Socket* getSocket() const { return socket_; }

    // 发送一个包, -1:socket错误, 0:发送完成, 1:未发送完成
    int32_t sendPacket(Packet* pack);
    // 接收一个包, -1:socket错误, 0:接收完成, 1:未接收完成
    int32_t recvPacket(Packet*& pack);
protected:
    SessionID sid_;
    // 连接上socket的时间
    uint32_t time_;
    // 关联的socket
    Socket* socket_;
    // 待发送的包
    List<Packet*> send_packets_;
    // 待处理的包
    List<Packet*> recv_packets_;
};

} // namespace kit

#endif

