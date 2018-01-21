#include "LuaCore.h"
#include "LuaBinding.h"
#include "lua.hpp"
#include "Logger.h"

#include "LuaDebug.h"

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
    // TODO: might set outcope
    L = luaL_newstate();
    luaL_openlibs(L);

    register_CommonConstant(L);
    register_NormalFunction(L);
    register_Class_Ref(L);
    register_Class_Logger(L);
    register_Class_Timer(L);
    register_Class_Buffer(L);
    register_Class_BufferPool(L);
    register_Class_Packet(L);
    register_Class_PacketHandler(L);
    register_Class_Protocol(L);
    register_Class_Terminal(L);
    register_Class_Client(L);
    register_Class_Server(L);
    register_Class_ScriptCore(L);
    register_Class_ScriptManager(L);
    register_Class_Gear(L);

#ifdef KIT_DEBUG_MODE
    register_debug(L);
#endif

    return true;
}

void LuaCore::destroy()
{
    lua_close(L);
}

int LuaCore::executeString(const char* codes)
{
    int ret = luaL_loadstring(L, codes);
    if (ret != LUA_OK)
    {
        ERR("[LuaCore](executeString) fail! %s", codes);
        return -1;
    }
    return executeFunc(0);
}

int LuaCore::executeFile(const char* file_name)
{
    std::string full_path = path_ + file_name;

    int ret = luaL_loadfile(L, full_path.c_str());
    if (ret != LUA_OK)
    {
        ERR("[LuaCore](executeFile) fail! %s", file_name);
        return -1;
    }

    return executeFunc(0);
}

int LuaCore::executeFunction(const char* function_name)
{
    lua_getglobal(L, function_name);
    if (!lua_isfunction(L, -1))
    {
        ERR("[LuaCore](executeFunction) fail! %s", function_name);
        lua_pop(L, 1);
        return -1;
    }
    return executeFunc(0);
}

int LuaCore::executeFunc(int arg_num)
{
    int func_index = -(arg_num + 1);
    if (!lua_isfunction(L, func_index))
    {
        LOG("[LUA]value at stack[%d] is not function", func_index);
        lua_pop(L, arg_num + 1);
        return 0;
    }
    int traceback = 0;
    lua_getglobal(L, "__G__TRACKBACK__");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
    }
    else
    {
        lua_insert(L, func_index - 1);
        traceback = func_index - 1;
    }
    int error = 0;
    error = lua_pcall(L, arg_num, 1, traceback);
    if (error)
    {
        if (traceback == 0)
        {
            LOG("[LUA ERROR] %s", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        else
        {
            lua_pop(L, 2);
        }
        return 0;
    }

    // get return value
    int ret = 0;
    if (lua_isnumber(L, -1))
    {
        ret = (int)lua_tointeger(L, -1);
    }
    else if (lua_isboolean(L, -1))
    {
        ret = (int)lua_toboolean(L, -1);
    }
    lua_pop(L, 1);
    if (traceback)
    {
        lua_pop(L, 1);
    }

    return ret;
}

