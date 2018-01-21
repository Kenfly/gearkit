#include "LuaFunctions.h"
#include "Functions.h"
#include "lua.hpp"
#include "Logger.h"

int32_t luaSplit(lua_State* L)
{
    int32_t narg = lua_gettop(L);
    if (narg != 2)
    {
        ERR("(luaSplit) narg < 3");
        return 0;
    }
    size_t len;
    const char* text = luaL_checklstring(L, 1, &len);
    if (!text)
    {
        ERR("(luaSplit) invalid text");
        return 0;
    }

    const char* separator = luaL_checklstring(L, 2, &len);
    if (!separator)
    {
        ERR("(luaSplit) invalid separator");
        return 0;
    }

    std::vector<std::string> vec;

    if (len == 1)
    {
        kit::split(std::string(text), vec, separator[0]);
    }
    else
    {
        kit::splitWithString(std::string(text), vec, std::string(separator));
    }
    len = vec.size();
    for(auto s : vec)
    {
        lua_pushlstring(L, s.c_str(), s.length());
    }
    return (int32_t)len;
}

