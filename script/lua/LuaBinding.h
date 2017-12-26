#ifndef __LUA_BINDING_H__
#define __LUA_BINDING_H__

struct lua_State;

// const
void register_CommonConstant(lua_State* L);

void register_NormalFunction(lua_State* L);

// ref
void register_Class_Ref(lua_State* L);

// timer
void register_Class_Timer(lua_State* L);

// buffer
void register_Class_Buffer(lua_State* L);
void register_Class_BufferPool(lua_State* L);

// packet
void register_Class_Packet(lua_State* L);

// packet handler
void register_Class_PacketHandler(lua_State* L);

// protocol
void register_Class_Protocol(lua_State* L);

// server
void register_Class_Server(lua_State* L);

// client
void register_Class_Client(lua_State* L);

// script core
void register_Class_ScriptCore(lua_State* L);

// script manager
void register_Class_ScriptManager(lua_State* L);

// gear
void register_Class_Gear(lua_State* L);


#endif

