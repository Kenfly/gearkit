#ifndef __LUA_BUFFER_H__
#define __LUA_BUFFER_H__

#include "kitsys.h"
struct lua_State;

namespace kit {
class Buffer;
}

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

int32_t buffer_write_format(kit::Buffer* buffer, lua_State* L);
int32_t buffer_read_format(kit::Buffer* buffer, lua_State* L);

#endif

