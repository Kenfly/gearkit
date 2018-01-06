#ifndef __LUA_DEBUG_H__
#define __LUA_DEBUG_H__


struct lua_State;

void register_debug(lua_State* L);

// debug
void register_Class_DebugInput(lua_State* L);
#endif

