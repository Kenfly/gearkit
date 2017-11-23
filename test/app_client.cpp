#include "app_client.h"
#include "Client.h"
#include "BufferPool.h"
#include "gearkit.h"


App::App()
: client_(NULL)
{
}

App::~App()
{
    if (client_)
        client_->release();
}

void App::run()
{
    kit::Application::run();

    if (client_)
    {
        client_->release();
        client_ = NULL;
    }
    auto client = kit::Client::create();
    auto ret = client->startup("127.0.0.1", 3333);
    if (ret != 0)
    {
        ERR("[App](run) client starup error!");
        return;
    }

    client_ = client;
    client_->retain();

    kit::Buffer* buf = g_BufPool->createBuffer(64);
    (*buf) << "hello world";
    client_->sendPacket(buf);

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
    client_->update();
}

uint32_t App::spareLogic(uint32_t spare_time)
{
    return spare_time;
}

