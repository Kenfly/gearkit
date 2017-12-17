#include "Thread_linux.h"

namespace kit {

// mutex
Mutex::Mutex()
{
}

bool Mutex::baseInit()
{
    // 嵌套锁
    pthread_mutexattr_t ma;
    pthread_mutexattr_init( &ma );
    pthread_mutexattr_settype( &ma, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init(&mutex_, &ma);
    pthread_mutexattr_destroy( &ma );
    return true;
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutex_);
}

int32_t Mutex::lock()
{
    return pthread_mutex_lock(&mutex_);
}

int32_t Mutex::unlock()
{
    return pthread_mutex_unlock(&mutex_);
}

int32_t Mutex::tryLock()
{
    return pthread_mutex_trylock(&mutex_);
}


// spin lock
SpinMutex::SpinMutex()
{
    pthread_spin_init(&mutex_, PTHREAD_PROCESS_SHARED);
}

SpinMutex::~SpinMutex()
{
    pthread_spin_destroy(&mutex_);
}

bool SpinMutex::baseInit()
{
    pthread_spin_init(&mutex_, PTHREAD_PROCESS_SHARED);
    return true;
}

int32_t SpinMutex::lock()
{
    return pthread_spin_lock(&mutex_);
}

int32_t SpinMutex::unlock()
{
    return pthread_spin_unlock(&mutex_);
}

int32_t SpinMutex::tryLock()
{
    return pthread_spin_trylock(&mutex_);
}

////////////////////////////////////////////
//thread

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


