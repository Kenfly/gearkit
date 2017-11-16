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
}

uint32_t App::spareLogic(uint32_t spare_time)
{
    return spare_time;
}

