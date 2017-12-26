#ifndef __KIT_GEAR_H__
#define __KIT_GEAR_H__

#include "Ref.h"

namespace kit {

class Gear : public Ref
{
public:
    //KIT_CREATE_FUNC(Gear)
    Gear();
    virtual ~Gear();

    virtual bool baseInit();

    virtual void begin();
    virtual void stop();
    virtual void end();
    virtual void logic();
    // 空余帧，参数为空闲的时间, 返回剩余时间
    virtual uint32_t spareLogic(uint32_t spare_time) { return spare_time; };

    // 每帧调用
    void mainLoop();

    // 获取当前帧
    uint32_t getFrame() const { return frame_; }
    // 帧率
    uint32_t getFrameRate() const { return frame_rate_; }
    void setFrameRate(uint32_t rate);
    // 帧delta
    uint32_t getFrameDelta() const { return frame_delta_; }
    void setNextDeltaZero(bool zero) { next_delta_zero_ = zero; }

    bool isValid() const { return valid_; }
protected:
    // 当前帧
    uint32_t frame_;
    // 帧率
    uint32_t frame_rate_;
    // 一帧毫秒数理想值
    uint32_t frame_msec_;
    // 当前帧
    uint32_t frame_delta_;


    // 下一帧delta为0
    bool next_delta_zero_;
    bool valid_;
private:
    uint32_t begine_time_;
    uint32_t wait_time_;
};

} // namespace kit

#define g_Gear (kit::Singleton<kit::Gear>::instance())

#endif

