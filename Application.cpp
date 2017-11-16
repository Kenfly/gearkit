#include "Application.h"
#include "RefPool.h"
#include "Logger.h"

#define DEFAULT_FRAME_RATE 60
namespace kit {

Application::Application()
: frame_(0)
, frame_rate_(DEFAULT_FRAME_RATE)
, frame_msec_(1000 / DEFAULT_FRAME_RATE)
, begine_time_(0)
, wait_time_(0)
, valid_(false)
{
}

Application::~Application()
{
}

void Application::run()
{
    // 创建ref pool
    g_RefPool->push(new RefPool());

    begine_time_ = kit::time() - frame_msec_;
    DBG("[Application](run) begin_time:%d frame_msec_:%d", begine_time_, frame_msec_);

    valid_ = true;
}

void Application::mainLoop()
{
    begine_time_ += frame_msec_;

    ++frame_;

    logic();

    uint32_t end_time = kit::time();
    //LOG("FRAME:%d,%d", frame_, end_time);
    uint32_t spare = 0;
    if (end_time > begine_time_)
    {
        // 超时
        uint32_t passed = end_time - begine_time_;
        if (passed <= frame_msec_)
        {
            spare = frame_msec_ - passed;
        }
        else
        {
            wait_time_ = passed - frame_msec_;
        }
    }
    else
    {
        // 有空余时间
        spare = begine_time_ - end_time;
    }

    if (wait_time_ > 0)
    {
        if (wait_time_ >= spare)
        {
            wait_time_ -= spare;
            spare = 0;
        }
        else
        {
            wait_time_ = 0;
            spare -= wait_time_;
        }
    }
    if (wait_time_ > 0)
        DBG("[Application](mainLoop) WAIT TIME:%d, %d\n", wait_time_, frame_);

    // 空闲时间处理
    if (spare > 0)
    {
        // ret pool 不作时间计算了
        g_RefPool->getCurrentPool()->clear();
        spare = spareLogic(spare);
        //DBG("[Application](mainLoop) SPARE TIME:%d", spare);
        if (spare > 0)
            kit::sleep(spare);
    }
}

void Application::stop()
{
    valid_ = false;
    // 清除ref pool
    g_RefPool->pop();
}

void Application::setFrameRate(unsigned int rate)
{
    frame_rate_ = rate;
    frame_msec_ = (1000 / frame_rate_);
}

} // namespace kit

