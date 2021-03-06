#include "BufferPool.h"
#include "Logger.h"

namespace kit {

BufferPool::BufferPool()
{
}

BufferPool::~BufferPool()
{
    clear();
}

bool BufferPool::baseInit()
{
    return true;
}

Buffer* BufferPool::createBuffer(size_t size)
{
    int index = 3;
    size_t fix_size = 8;
    while (fix_size < size)
    {
        fix_size <<= 1;
        ++index;
    }

    Buffer* buf = NULL;
    if (index >= BUFFER_QUEUE_CNT || !ques_[index].pop(buf))
    {
		buf = Buffer::create(false);
		buf->init(fix_size);
    }
    buf->reset();
    //动态修改tail_，改成跟size一样大小
    buf->setSize(size);

    DBG("[BufferPool](createBuffer)[%d/%d]", size, fix_size);
    return buf;
}

void BufferPool::destroyBuffer(Buffer* buffer)
{
    KIT_ASSERT(buffer->getReferenceCount() == 1);
    size_t size = buffer->getCapacity();
    int index = 3;
    do
    {
        ++index;
        size >>= 1;
    } while(size);
    if (index >= BUFFER_QUEUE_CNT || !ques_[index].push(buffer))
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
{
}

BufferPoolManager::~BufferPoolManager()
{
    KIT_SAFE_RELEASE(pool_)
}

bool BufferPoolManager::baseInit()
{
    pool_ = BufferPool::create(false);
    return true;
}

} // namespace kit


