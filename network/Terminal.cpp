#include "Terminal.h"

namespace kit {

Terminal::Terminal()
: timer_(NULL)
{
}

Terminal::~Terminal()
{
    KIT_SAFE_RELEASE(timer_)
}

bool Terminal::baseInit()
{
    timer_ = Timer::create(false);
    timer_->init(this);
    return true;
}

uint32_t Terminal::addTimer(uint32_t delay, TMHandler handler, uint32_t cycle)
{
    uint32_t id = timer_->addTimer(delay, cycle);
    timer_map_.set(id, handler);
    return 0;
}

void Terminal::delTimer(uint32_t id)
{
    timer_map_.del(id);
}

bool Terminal::handleTimer(uint32_t id)
{
    TMHandler handler;
    if (timer_map_.get(id, handler))
    {
        return handler(id);
    }
    return true;
}

void Terminal::update()
{
    timer_->update();
}

} // namespace kit

