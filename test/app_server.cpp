#include "app_server.h"
#include "gearkit.h"

App::App()
: server_(NULL)
{
}

App::~App()
{
    if (server_)
        server_->release();
}

void App::run()
{
    kit::Application::run();

    if (server_)
    {
        server_->release();
        server_ = NULL;
    }
    auto server = kit::Server::create();
    auto ret = server->startup("127.0.0.1", 3333);
    if (ret != 0)
    {
        ERR("[App](run) server starup error!");
        return;
    }

    server_ = server;
    server_->retain();

    timer_ = kit::Timer::create();
    timer_->setHandler(this);
    timer_->retain();
    timer_->addTimer(36000, 0);

    while (valid_)
    {
        mainLoop();
    }
}


void App::stop()
{
    kit::Application::stop();
}

void App::logic()
{
    server_->update();
    timer_->update();
}

uint32_t App::spareLogic(uint32_t spare_time)
{
    return spare_time;
}

bool App::handleTimer(uint32_t id)
{
    printf("[App](handleTimer) id:%d\n", id);
    return false;
}


