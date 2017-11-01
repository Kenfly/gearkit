#include <cstdio>
#include "base.h"
#include "network.h"

int main(int argc, char* argv[])
{
    kit::Server* server = new kit::Server();
    server->startup("127.0.0.1", 3333);
    LOG("server startup!!");
    return 0;
}
