#ifndef __KIT_THREAD_H__
#define __KIT_THREAD_H__

namespace kit {

class Thread : public Ref
{
public:
    Thread();
    virtual ~Thread();
    
    void join();
    int start();
    int stop();
};

} // namespace kit

#endif
