#include "Thread.h"

namespace kit {

AutoLock::AutoLock(IMutex* mutex)
: mutex_(mutex)
{
    mutex->lock();
}

AutoLock::~AutoLock()
{
    mutex_->unlock();
}

////////////////////////////////////////////////

IThread::IThread()
: active_(false)
, mutex_(NULL)
, handler_(NULL)
{
}

IThread::~IThread()
{
    mutex_->lock();
    if (active_)
        stop();
    mutex_->unlock();

    mutex_->release();
}

bool IThread::baseInit()
{
    mutex_ = Mutex::create(false);
    return true;
}

void IThread::init(ThreadHandler* handler)
{
    setHandler(handler);
}

void IThread::setHandler(ThreadHandler* handler)
{
    handler_ = handler;
}

void IThread::run()
{
    if (handler_)
        handler_->handleThread();
}

void IThread::stop()
{
    mutex_->lock();
    if (active_)
    {
        active_ = false;
        mutex_->unlock();
        this->join();
        return;
    }
    mutex_->unlock();
    return;
}

} // namespace kit

