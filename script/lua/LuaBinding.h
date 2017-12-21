#ifndef __LUA_BINDDING_H__
#define __LUA_BINDDING_H__

struct lua_State;

void test();

void register_NormalFunction(lua_State* L);

void register_Class_Ref(lua_State* L);

void register_Class_Server(lua_State* L);

void register_Class_Client(lua_State* L);

void register_Class_Application(lua_State* L);

#endif

