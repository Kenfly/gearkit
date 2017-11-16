#include "BufferPool.h"

namespace kit {

BufferPool::BufferPool()
{
}

BufferPool::~BufferPool()
{
}

Buffer* BufferPool::createBuffer(uint32_t size)
{
    int32_t index = 0;
    uint32_t fix_size = 8;
    while (fix_size >= size)
    {
        ++index;
        fix_size <<= 1;
    }

    Buffer* buf = NULL;
    if (index < BUFFER_QUEUE_CNT)
    {
        BufferQue& que = ques_[index];
        if (!que.pop(buf))
        {
            buf = new Buffer();
            buf->init(fix_size);
        }
    }
    if (buf == NULL)
    {
        buf = new Buffer();;
        buf->init(fix_size);
    }

    return buf;
}

void BufferPool::destroyBuffer(Buffer* buffer)
{
    uint32_t size = buffer->getSize();
    int32_t index = 0;
    uint32_t fix_size = 8;
    while (fix_size >= size)
    {
        ++index;
        fix_size <<= 1;
    }
    if (index < BUFFER_QUEUE_CNT)
    {
        BufferQue& que = ques_[index];
        if (que.push(buffer))
            buffer = NULL;
    }
    if (buffer)
    {
        buffer->release();
    }
}

} // namespace kit


