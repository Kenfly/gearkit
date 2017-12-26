#include "RefPool.h"
#include "Ref.h"

namespace kit {

///////////////////////////////////////////////////////////////
// ref pool
///////////////////////////////////////////////////////////////

RefPool::RefPool()
{
    ref_vec_.reserve(150);
}

RefPool::~RefPool()
{
    clear();
}

void RefPool::addRef(Ref* ref)
{
    ref_vec_.push_back(ref);
}

// should not call this
void RefPool::delRef(Ref* ref)
{
    for (RefVec::iterator ix = ref_vec_.begin(); ix != ref_vec_.end(); ++ix)
    {
        if ((*ix) == ref)
        {
            ref_vec_.erase(ix);
            break;
        }
    }
}

void RefPool::clear()
{
    RefVec vec;
    vec.swap(ref_vec_);
    for (RefVec::iterator ix = vec.begin(); ix != vec.end(); ++ix)
    {
        (*ix)->release();
    }
}


///////////////////////////////////////////////////////////////
// pool manager
///////////////////////////////////////////////////////////////
RefPoolManager::RefPoolManager()
: cur_pool_(0)
{
    pool_vec_.reserve(10);
}

RefPoolManager::~RefPoolManager()
{
    while (!pool_vec_.empty())
    {
        pop();
    }
}

bool RefPoolManager::baseInit()
{
    push(new RefPool());
    return true;
}

void RefPoolManager::push(RefPool* pool)
{
    cur_pool_ = pool;
    pool_vec_.push_back(pool);
}

void RefPoolManager::pop()
{
    if (pool_vec_.empty())
        return;
    
    pool_vec_.pop_back();
    delete cur_pool_;
    if (pool_vec_.empty())
        cur_pool_ = 0;
    else
        cur_pool_ = pool_vec_.back();
}

} // namespace kit

