#ifndef __KIT_TERMINAL_H__
#define __KIT_TERMINAL_H__

#include "Ref.h"
#include "Timer.h"
#include "kitsys.h"
#include "Map.h"
#include <functional>

namespace kit {

class Terminal : public Ref, public TimerHandler
{
    typedef std::function<bool(uint32_t)> TMHandler;
public:
	Terminal();
    virtual ~Terminal();

    // 每帧调用
    virtual void update();

    uint32_t addTimer(uint32_t delay, TMHandler handler, uint32_t cycle = 0);
    void delTimer(uint32_t id);
protected:
    virtual bool baseInit();
    virtual bool handleTimer(uint32_t id);
protected:
    Timer* timer_;
    Map<uint32_t, TMHandler, 1024> timer_map_;
};

} // namespcae kit

#endif
