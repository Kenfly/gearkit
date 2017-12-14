#ifndef __KIT_REF_H__
#define __KIT_REF_H__

#include <new>

namespace kit {

class Ref
{
protected:
    Ref();
    virtual bool baseInit(void) { return true; }

    unsigned int reference_count_;
public:
    virtual ~Ref();

    void retain();
    void release();

    Ref* autoRelease();
    unsigned int getReferenceCount() const { return reference_count_; };
};

#define KIT_SAFE_RELEASE(r) \
    if (r) \
        r->release(); \
    r = 0;

#define KIT_SAFE_RETAIN(r) \
    if (r) \
        r->retain();

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
        ret = 0; \
    }
    
} // namespace kit

#endif

