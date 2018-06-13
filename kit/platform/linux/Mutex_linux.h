#ifndef __KIT_MUTEX_LINUX_H__
#define __KIT_MUTEX_LINUX_H__

#include "Mutex.h"
#include <pthread.h>

namespace kit {

class Mutex : public IMutex
{
public:
    KIT_CREATE_FUNC(Mutex)

    Mutex();
    virtual ~Mutex();

    virtual int32_t lock();
    virtual int32_t unlock(); 
    virtual int32_t tryLock();
protected:
    virtual bool baseInit();

    pthread_mutex_t mutex_;
};

class SpinMutex : public IMutex
{
public:
    KIT_CREATE_FUNC(SpinMutex)

    SpinMutex();
    virtual ~SpinMutex();

    virtual int32_t lock();
    virtual int32_t unlock(); 
    virtual int32_t tryLock();
protected:
    virtual bool baseInit();

    pthread_spinlock_t mutex_;
};


} // namespace kit

#endif
