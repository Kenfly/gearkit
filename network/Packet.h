#ifndef __KIT_PACKET_H__
#define __KIT_PACKET_H__

#include "kitsys.h"
#include "netsys.h"
#include "Ref.h"

namespace kit {

class Buffer;

struct PacketHeader
{
    ProtocolID pid; //协义
    uint16_t seed; //验证种子
    uint16_t length; //包体长度
};

// 包最大在小
const uint32_t PACKET_MAX_SIZE = 0xFFFF;
// 包头大小
const uint32_t PACKET_HEADER_SIZE = sizeof(PacketHeader);

class Packet : public Ref
{
public:
    KIT_CREATE_FUNC(Packet)
    Packet();
    virtual ~Packet();

    void init(ProtocolID pid, Buffer* buf);

    void setSeed(uint16_t seed) { header_.seed = seed; }
    ProtocolID getPID() const { return header_.pid; }
    uint16_t getSeed() const { return header_.seed; }
    uint16_t getLength() const { return header_.length; }
    void setBuffer(Buffer* buf);
    Buffer* getBuffer() const { return buf_; }
    void delBuffer();

    void packHeader(Buffer* head_buf);
    void unpackHeader(Buffer* head_buf);
public:
    PacketHeader header_;
    Buffer* buf_;
};

// socket 协议
const ProtocolID C_CONNECT = 0x1;
const ProtocolID S_CONNECT = 0x2;
const ProtocolID C_RECONNECT = 0x3;
const ProtocolID S_RECONNECT = 0x4;

} // namespace kit

#endif

