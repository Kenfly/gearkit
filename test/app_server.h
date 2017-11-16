#ifndef __APP_H__
#define __APP_H__

#include "Application.h"
#include "Server.h"

class App : public kit::Application
{
public:
    App();
    virtual ~App();

    virtual void run();
    virtual void stop();
    virtual void logic();
    virtual uint32_t spareLogic(uint32_t spare_time);

private:
    kit::Server* server_;
};

#endif
