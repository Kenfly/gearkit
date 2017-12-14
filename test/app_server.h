#ifndef __APP_H__
#define __APP_H__

#include "Application.h"
#include "Server.h"
#include "Timer.h"

class App : public kit::Application, public kit::TimerHandler
{
public:
    App();
    virtual ~App();

    virtual void run();
    virtual void stop();
    virtual void logic();
    virtual uint32_t spareLogic(uint32_t spare_time);

protected:
    virtual bool handleTimer(uint32_t id);
private:
    kit::Server* server_;
    kit::Timer* timer_;
};

#endif
