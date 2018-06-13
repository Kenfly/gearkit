#include "Gear.h"
#include "RefPool.h"
#include "Logger.h"
#include "Gear.h"
#include "BufferPool.h"
#include "RefPool.h"
#include "Logger.h"
#include "Singleton.h"

#define DEFAULT_FRAME_RATE 60
namespace kit {

Gear::Gear()
: frame_(0)
, frame_rate_(DEFAULT_FRAME_RATE)
, frame_msec_(1000 / DEFAULT_FRAME_RATE)
, valid_(false)
, begine_time_(0)
, wait_time_(0)
{
}

Gear::~Gear()
{
}

bool Gear::baseInit()
{
    return true;
}

void Gear::logic()
{
    update();
}

void Gear::begin()
{
    g_BufPool;
    begine_time_ = kit::time() - frame_msec_;
    DBG("[Gear](start) begin_time:%d frame_msec_:%d", begine_time_, frame_msec_);

    valid_ = true;
}

void Gear::stop()
{
    valid_ = false;
}

void Gear::end()
{
    Singleton<BufferPoolManager>::destroyInstance();
    Singleton<RefPoolManager>::destroyInstance();
    Singleton<Logger>::destroyInstance();

    Singleton<Gear>::destroyInstance();
}

void Gear::mainLoop()
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
        DBG("[Gear](mainLoop) WAIT TIME:%d, %d\n", wait_time_, frame_);

    // 空闲时间处理
    if (spare > 0)
    {
        // ret pool 不作时间计算了
        g_RefPool->getCurrentPool()->clear();
        spare = spareLogic(spare);
        //DBG("[Gear](mainLoop) SPARE TIME:%d", spare);
        if (spare > 0)
            kit::sleep(spare);
    }
}

void Gear::setFrameRate(unsigned int rate)
{
    frame_rate_ = rate;
    frame_msec_ = (1000 / frame_rate_);
}

} // namespace kit

