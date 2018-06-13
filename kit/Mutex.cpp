#include "Mutex.h"

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


} // namspace kit
