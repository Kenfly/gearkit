#ifndef __KIT_BUFFER_POOL_H__
#define __KIT_BUFFER_POOL_H__

//非线程安全，线程里不要调用create/destroy
//buffer大小只有: 1, 2, 4, 8, 16, 32, 64, 128, 512, 1024...

#include "Buffer.h"
#include "SyncQueue.h"
#include "Singleton.h"

#ifndef BUFFER_POOL_SIZE
#define BUFFER_POOL_SIZE (200)
#endif
#ifndef BUFFER_QUEUE_CNT
#define BUFFER_QUEUE_CNT (10)
#endif

namespace kit {

class BufferPool : public Ref
{
public:
    KIT_CREATE_FUNC(BufferPool)

    BufferPool();
    virtual ~BufferPool();
    virtual bool baseInit();

    Buffer* createBuffer(size_t size);
    void destroyBuffer(Buffer* buffer);

    void clear();
private:
    typedef SyncQueue<Buffer*, BUFFER_POOL_SIZE> BufferQue;
    BufferQue ques_[BUFFER_QUEUE_CNT];
};

class BufferPoolManager : public Ref
{
public:
    BufferPoolManager();
    virtual ~BufferPoolManager();

    virtual bool baseInit();

    BufferPool* getPool() const { return pool_; }
private:
    BufferPool* pool_;
};

} // namespace kit

#define g_BufPoolMng (kit::Singleton<kit::BufferPoolManager>::instance())

// buf pool
#define g_BufPool (g_BufPoolMng->getPool())

#endif

