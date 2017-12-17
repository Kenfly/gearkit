#include "LuaBinding.h"
#include <stdio.h>
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"

void test()
{
    printf("hello lua!\n");
}

void registerNormalFunction(lua_State* L)
{
}
