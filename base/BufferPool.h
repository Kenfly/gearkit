#ifndef __KIT_BUFFER_POOL_H__
#define __KIT_BUFFER_POOL_H__

//非线程安全，线程里不要调用create/destroy
//buffer大小只有: 8, 16, 32, 64, 128, 512, 1024...

#include "Buffer.h"
#include "Queue.h"
#include "Singleton.h"

namespace kit {

const int32_t BUFFER_QUEUE_CNT = 8;

class BufferPool
{
public:
    BufferPool();
    virtual ~BufferPool();

    Buffer* createBuffer(uint32_t size);
    void destroyBuffer(Buffer* buffer);
private:
    typedef Queue<Buffer*, 200> BufferQue;
    BufferQue ques_[BUFFER_QUEUE_CNT];
};

} // namespace kit

#define g_BufPool (kit::Singleton<kit::BufferPool>::instance())

#endif

