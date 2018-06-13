#include "Thread_linux.h"
#include "Mutex_linux.h"

namespace kit {

void* thread_start(void *p)
{
	if(!p) return NULL;

	Thread *thr = (Thread*)p;
	thr->run();
    return NULL;
}

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::join()
{
    pthread_join(pid_, NULL);
}

int32_t Thread::start()
{
    mutex_->lock();
    active_ = true;
    int32_t ret = pthread_create(&pid_, NULL, thread_start, (void*)this);
    mutex_->unlock();
    return ret;
}

} // namespace kit


