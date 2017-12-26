#include "BufferPool.h"

namespace kit {

BufferPool::BufferPool()
{
}

BufferPool::~BufferPool()
{
    clear();
}

Buffer* BufferPool::createBuffer(uint32_t size)
{
    int32_t index = 0;
    uint32_t fix_size = 1;
    uint32_t size_tmp = size;
    while (size_tmp >>= 1)
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
            buf = Buffer::create(false);
            buf->init(fix_size);
        }
    }
    if (buf == NULL)
    {
        buf = Buffer::create(false);;
        buf->init(fix_size);
    }
    buf->reset();
    //动态修改tail_，改成跟size一样大小
    buf->tail_ = buf->head_ + size;

    return buf;
}

void BufferPool::destroyBuffer(Buffer* buffer)
{
    uint32_t size = buffer->getMemorySize();
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

void BufferPool::clear()
{
    Buffer* p = NULL;
    for (int i = 0; i != BUFFER_QUEUE_CNT; ++i)
    {
        BufferQue& que = ques_[i];
        while (que.pop(p))
        {
            p->release();
        }
    }
}


///////////////////////////////////////////////////
// BufferPoolManager
///////////////////////////////////////////////////

BufferPoolManager::BufferPoolManager()
: pool_(NULL)
, thread_pool_(NULL)
{
}

BufferPoolManager::~BufferPoolManager()
{
    KIT_SAFE_RELEASE(pool_)
    KIT_SAFE_RELEASE(thread_pool_)
}

bool BufferPoolManager::baseInit()
{
    pool_ = new BufferPool();
    thread_pool_ = new BufferPool();
    return true;
}

} // namespace kit


