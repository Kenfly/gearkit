#include "app_client.h"
#include "Client.h"
#include "BufferPool.h"
#include "Packet.h"
#include "gearkit.h"

class TimerFunctor
{
public:
    TimerFunctor(kit::Client* client): client_(client)
    {
    }
    bool operator()(uint32_t id)
    {
        kit::Buffer* buf = g_BufPool->createBuffer(64);
        const char* s = "hello world";
        (*buf) << s;

        kit::Packet* packet = kit::Packet::create();
        packet->init(1, buf);
        client_->sendPacket(packet);
        return false;
    }

private:
    kit::Client* client_;
};

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

    TimerFunctor func(client_);

    client_->addTimer(120, func, 120);

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

