#include "Packet.h"
#include "Buffer.h"

namespace kit {

Packet::Packet()
: buf_(NULL)
{
}

Packet::~Packet()
{
    KIT_SAFE_RELEASE(buf_);
}

void Packet::init(int16_t pid, Buffer* buf)
{
    buf_ = buf;
    buf_->retain();

    header_.pid = pid;
    header_.length = (uint16_t)buf->getWrittenSize();
}

void Packet::setBuffer(Buffer* buf)
{
    if (buf_)
        buf_->release();
    buf->retain();
    buf_ = buf;
}

void Packet::delBuffer()
{
    if (buf_)
    {
        buf_->release();
        buf_ = NULL;
    }
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

