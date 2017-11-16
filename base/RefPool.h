#ifndef __KIT_REF_POLL_H__
#define __KIT_REF_POLL_H__

/**
 * ref pool
 * step 1.push a pool to manager(g_RefPool)
 * step 2.every frame you should call g_RefPool->getCurrentPool()->clear() to release the ref
 *
 **/

#include <vector>
#include "Singleton.h"

namespace kit {

class Ref;

class RefPool
{
public:
    RefPool();
    virtual ~RefPool();

    void addRef(Ref* ref);
    void delRef(Ref* ref);
    void clear();
private:
    typedef std::vector<Ref*> RefVec;
    RefVec ref_vec_;
};

class RefPoolManager
{
public:
    RefPoolManager();
    virtual ~RefPoolManager();

    RefPool* getCurrentPool() const { return cur_pool_; }
    void push(RefPool* pool);
    void pop();
private:
    RefPool* cur_pool_;

    typedef std::vector<RefPool*> PoolVec;
    PoolVec pool_vec_;
};


#define g_RefPool (kit::Singleton<kit::RefPoolManager>::instance())

} // namespace kit

#endif

