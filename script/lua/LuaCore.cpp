#include "LuaCore.h"
#include "LuaBinding.h"
#include "lua.hpp"


LuaCore::LuaCore()
: L(NULL)
{
}

LuaCore::~LuaCore()
{
    if (L)
        destroy();
}

bool LuaCore::baseInit()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    registerNormalFunction(L);
}

void LuaCore::destroy()
{
    lua_close(L);
}

int LuaCore::executeString(const char* codes)
{
    int ret = luaL_dostring(L, codes);
    return 0;
}

int LuaCore::executeFile(const char* filename)
{
    return 0;
}

int LuaCore::executeFunction(const char* function_name)
{
    return 0;
}

int LuaCore::executeFunc(int arg_num)
{
    return 0;
}

