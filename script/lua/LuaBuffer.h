#ifndef __LUA_BUFFER_H__
#define __LUA_BUFFER_H__

#include "kitsys.h"
#include <Buffer.h>

struct lua_State;

/*
 * b: int8
 * B: uint8
 * w: int16
 * W: uint16
 * i: int32
 * I: uint32
 * l: int64
 * L: uint64
 * v: varint
 *
 * f: float
 * d: double
 * s: string
 */

class LuaBuffer : public kit::Buffer
{
public:
    KIT_CREATE_FUNC(LuaBuffer)
    int32_t luaWriteVector(lua_State* L);
    int32_t luaReadVector(lua_State* L);

    int32_t luaWriteFormat(lua_State* L);
    int32_t luaReadFormat(lua_State* L);
};

#endif

