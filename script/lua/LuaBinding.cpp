#include "LuaBinding.h"
#include <stdio.h>
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "Ref.h"
#include "Server.h"

void test()
{
    printf("hello lua!\n");
}
void test2(int b)
{
    printf("test2:%d\n", b);
}

void register_NormalFunction(lua_State* L)
{
    luabridge::getGlobalNamespace(L)
        .beginNamespace("kit")
            .addFunction("test", test)
        .endNamespace();

    luabridge::getGlobalNamespace(L)
        .beginNamespace("kit")
            .addFunction("test2", test2)
        .endNamespace();
}

void register_Class_Ref(lua_State* L)
{
    luabridge::getGlobalNamespace(L)
        .beginNamespace("kit")
            .beginClass<kit::Ref>("Ref")
                .addFunction("retain", &kit::Ref::retain)
                .addFunction("release", &kit::Ref::release)
                .addFunction("autorelease", &kit::Ref::autoRelease)
                .addFunction("get_reference_count", &kit::Ref::getReferenceCount)
            .endClass()
        .endNamespace();
}

void register_Class_Server(lua_State* L)
{
    luabridge::getGlobalNamespace(L)
        .beginNamespace("kit")
            .deriveClass<kit::Server, kit::Ref>("Server")
                .addStaticFunction("create", &kit::Server::create)
            .endClass()
        .endNamespace();
}

void register_Class_Client(lua_State* L)
{
}

