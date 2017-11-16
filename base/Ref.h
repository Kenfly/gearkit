#ifndef __KIT_REF_H__
#define __KIT_REF_H__

#include <new>

namespace kit {

class Ref
{
protected:
    Ref();
    unsigned int reference_count_;
public:
    virtual ~Ref();

    void retain();
    void release();

    Ref* autoRelease();
    unsigned int getReferenceCount() const { return reference_count_; };
};


#define KIT_CREATE_FUNC(__TYPE__) \
    static __TYPE__* create() \
    { \
        __TYPE__* ret = new(std::nothrow) __TYPE__(); \
        if (ret) \
        { \
            ret->autoRelease(); \
            return ret; \
        } \
        ret = nullptr; \
    }
    
} // namespace kit

#endif

