#ifndef __KIT_REF_H__
#define __KIT_REF_H__

#include "kitsys.h"
#include <new>
#include <vector>
#include <string>

namespace kit {

class Ref
{
protected:
    Ref();

    unsigned int reference_count_;
public:
    virtual ~Ref();

    virtual bool baseInit(void) { return true; }
    void retain();
    void release();

    virtual void update();

    Ref* autoRelease();
    unsigned int getReferenceCount() const { return reference_count_; }

    void addChild(Ref* obj);
    void delChild(Ref* obj);
    void clearChildren();
    virtual void removeFromParent();

    template<typename T>
    T* to() const { return reinterpret_cast<T*>(this); }
    virtual std::string toString() const { return "----[Ref]----"; }
private:
    typedef std::vector<Ref*> RefVec;
    RefVec children_;
    Ref* parent_;
};

#define KIT_SAFE_RELEASE(r) \
    if (r) \
    { \
        r->release(); \
        r = 0; \
    }

#define KIT_SAFE_RETAIN(r) \
    if (r) \
    { \
        r->retain(); \
    }

#define KIT_CREATE_FUNC(__TYPE__) \
    static __TYPE__* create(bool inpool = true) \
    { \
        __TYPE__* ret = new(std::nothrow) __TYPE__(); \
        if (ret && ret->baseInit()) \
        { \
            if (inpool) \
                ret->autoRelease(); \
            return ret; \
        } \
        KIT_SAFE_RELEASE(ret) \
        return ret; \
    }

} // namespace kit

#endif

