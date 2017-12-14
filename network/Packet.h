#ifndef __KIT_PACKET_H__
#define __KIT_PACKET_H__

#include "kitsys.h"
#include "Ref.h"

namespace kit {

class Buffer;

struct PacketHeader
{
    uint16_t seed; //验证种子
    uint16_t length; //包体长度
    int16_t pid; //协义
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

    void init(int16_t pid, Buffer* buf);

    void setSeed(uint16_t seed) { header_.seed = seed; }
    int16_t getPID() const { return header_.pid; }
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

struct IPacketHandler : public Ref
{
    virtual void onPacket(int32_t sid, Buffer* buf) = 0;
};

} // namespace kit

#endif

