#include "Packet.h"
#include "Buffer.h"

namespace kit {

Packet::Packet()
: buf_(nullptr)
{
}

Packet::~Packet()
{
    KIT_SAFE_RELEASE(buf_);
}

void Packet::init(ProtocolID pid, Buffer* buf)
{
    setBuffer(buf);

    header_.pid = pid;
    header_.length = (uint16_t)(buf ? buf->getWrittenSize() : 0);
}

void Packet::setBuffer(Buffer* buf)
{
    KIT_SAFE_RELEASE(buf_);
    buf_ = buf;
    KIT_SAFE_RETAIN(buf_);
}

void Packet::delBuffer()
{
    KIT_SAFE_RELEASE(buf_);
}

void Packet::packHeader(Buffer* head_buf)
{
    (*head_buf) << header_.seed;
    (*head_buf) << header_.length;
    (*head_buf) << header_.pid;
}

void Packet::unpackHeader(Buffer* head_buf)
{
    (*head_buf) >> header_.seed;
    (*head_buf) >> header_.length;
    (*head_buf) >> header_.pid;
}

} // namespace kit

