#ifndef __KIT_BUFFER_POOL_H__
#define __KIT_BUFFER_POOL_H__

//非线程安全，线程里不要调用create/destroy
//buffer大小只有: 8, 16, 32, 64, 128, 512, 1024...

#include "Buffer.h"
#include "Queue.h"
#include "Singleton.h"

#ifndef BUFFER_POOL_SIZE
#define BUFFER_POOL_SIZE (200)
#endif

namespace kit {

const int32_t BUFFER_QUEUE_CNT = 8;

class BufferPool : public Ref
{
public:
    BufferPool();
    virtual ~BufferPool();

    Buffer* createBuffer(uint32_t size);
    void destroyBuffer(Buffer* buffer);

    void clear();
private:
    typedef Queue<Buffer*, BUFFER_POOL_SIZE> BufferQue;
    BufferQue ques_[BUFFER_QUEUE_CNT];
};

class BufferPoolManager : public Ref
{
public:
    BufferPoolManager();
    virtual ~BufferPoolManager();

    virtual bool baseInit();

    BufferPool* getPool() const { return pool_; }
    BufferPool* getThreadPool() const { return thread_pool_; }
private:
    BufferPool* pool_;
    BufferPool* thread_pool_;
};

} // namespace kit

#define g_BufPoolMng (kit::Singleton<kit::BufferPoolManager>::instance())

// buf pool
#define g_BufPool (g_BufPoolMng->getPool())

// 这个pool专门给线程单向之间交流
#define g_BufPoolThread (g_BufPoolMng->getThreadPool())

#endif

