#include "LuaBinding.h"
#include <stdio.h>
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"
#include "Logger.h"

#include "types.h"
#include "Ref.h"
#include "Timer.h"
#include "Buffer.h"
#include "BufferPool.h"
#include "LuaBuffer.h"
#include "Packet.h"
#include "Protocol.h"
#include "LuaProtocol.h"
#include "Client.h"
#include "Server.h"
#include "Gear.h"
#include "ScriptSupport.h"

void test_table(const LuaIntf::LuaRef& tb)
{
    for (auto& e : tb)
    {
        auto value = e.value<LuaIntf::LuaRef>();
        printf("%d\n", value.toValue<int>());
    }
    //printf("xxxx:%d\n", tb.get<int>(2));
}

void register_CommonConstant(lua_State* L)
{
    // types.h
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addConstant("TYPE_INT8", TYPE_INT8)
            .addConstant("TYPE_UINT8", TYPE_INT8)
            .addConstant("TYPE_INT16", TYPE_INT16)
            .addConstant("TYPE_UINT16", TYPE_UINT16)
            .addConstant("TYPE_INT32", TYPE_INT32)
            .addConstant("TYPE_UINT32", TYPE_UINT32)
            .addConstant("TYPE_INT64", TYPE_INT64)
            .addConstant("TYPE_UINT64", TYPE_UINT64)
            .addConstant("TYPE_STRING", TYPE_STRING)

            .addConstant("TYPE_ARRAY", TYPE_ARRAY)
            .addConstant("TYPE_TABLE", TYPE_TABLE)
        .endModule();
}

void register_NormalFunction(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addFunction("test_table", test_table)
        .endModule();
}

void register_Class_Ref(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginClass<kit::Ref>("ref")
                .addFunction("retain", &kit::Ref::retain)
                .addFunction("release", &kit::Ref::release)
                .addFunction("autorelease", &kit::Ref::autoRelease)
                .addFunction("get_reference_count", &kit::Ref::getReferenceCount)
                .addFunction("add_child", &kit::Ref::addChild)
                .addFunction("del_child", &kit::Ref::delChild)
                .addFunction("clear_children", &kit::Ref::clearChildren)
                .addFunction("remove_from_parent", &kit::Ref::removeFromParent)
            .endClass()
        .endModule();
}

void register_Class_Timer(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Timer, kit::Ref>("timer")
                .addStaticFunction("create", &kit::Timer::create)
                .addFunction("add_timer", &kit::Timer::addTimer, LUA_ARGS(uint32_t, kit::TimerHandler, LuaIntf::_def<uint32_t, 0>))
                .addFunction("del_timer", &kit::Timer::delTimer)
            .endClass()
        .endModule();
}

void register_Class_Buffer(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Buffer, kit::Ref>("bufferbase")
                //.addStaticFunction("create", &kit::Buffer::create)
                .addFunction("init", &kit::Buffer::init)
                .addFunction("reset", &kit::Buffer::reset)
                .addFunction("get_size", &kit::Buffer::getSize)
                .addFunction("get_memory_size", &kit::Buffer::getMemorySize)
                .addFunction("get_written_size", &kit::Buffer::getWrittenSize)
                .addFunction("get_writable_size", &kit::Buffer::getWritableSize)
                .addFunction("get_readable_size", &kit::Buffer::getReadableSize)
                .addFunction("skip_write", &kit::Buffer::skipWrite)
                .addFunction("skip_read", &kit::Buffer::skipRead)
                .addFunction("debug_print", &kit::Buffer::debugPrint) 
                .addFunction("write_buffer", &kit::Buffer::writeBuffer) // disable ?
                .addFunction("read_buffer", &kit::Buffer::readBuffer) // disable ? 
                .addFunction("w_string", &kit::Buffer::operator<< <const char*> )

                .addFunction("w_int8_t", &kit::Buffer::operator<< <int8_t> )
                .addFunction("r_int8_t", [](kit::Buffer* self){ int8_t t; (*self)>>t; return t;} )
                .addFunction("w_uint8_t", &kit::Buffer::operator<< <uint8_t> )
                .addFunction("r_uint8_t", [](kit::Buffer* self){ uint8_t t; (*self)>>t; return t;} )

                .addFunction("w_int16_t", &kit::Buffer::operator<< <int16_t> )
                .addFunction("r_int16_t", [](kit::Buffer* self){ int16_t t; (*self)>>t; return t;} )
                .addFunction("w_uint16_t", &kit::Buffer::operator<< <uint16_t> )
                .addFunction("r_uint16_t", [](kit::Buffer* self){ uint16_t t; (*self)>>t; return t;} )

                .addFunction("w_int32_t", &kit::Buffer::operator<< <int32_t> )
                .addFunction("r_int32_t", [](kit::Buffer* self){ int32_t t; (*self)>>t; return t;} )
                .addFunction("w_uint32_t", &kit::Buffer::operator<< <uint32_t> )
                .addFunction("r_uint32_t", [](kit::Buffer* self){ uint32_t t; (*self)>>t; return t;} )
                .addFunction("write_format", &buffer_write_format)
                .addFunction("read_format", &buffer_read_format)
            .endClass()
        .endModule();
}

void register_Class_BufferPool(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::BufferPool, kit::Ref>("bufferpool")
                .addFunction("create_buffer", &kit::BufferPool::createBuffer)
                .addFunction("destroy_buffer", &kit::BufferPool::destroyBuffer)
                .addFunction("clear", &kit::BufferPool::clear)
            .endClass()
        .endModule();
}

void register_Class_Packet(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Packet, kit::Ref>("packet")
                .addStaticFunction("create", &kit::Packet::create)
                .addFunction("init", &kit::Packet::init)
                .addFunction("set_seed", &kit::Packet::getSeed)
                .addFunction("get_seed", &kit::Packet::setSeed)
                .addFunction("get_length", &kit::Packet::getLength)
                .addFunction("set_buffer", &kit::Packet::setBuffer)
                .addFunction("get_buffer", &kit::Packet::getBuffer)
                .addFunction("del_buffer", &kit::Packet::delBuffer)
            .endClass()
        .endModule();
}

void register_Class_PacketHandler(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::PacketHandler, kit::Ref>("packethandler")
            .endClass()
            .beginExtendClass<kit::PacketProtocolHandler, kit::PacketHandler>("packetptohandler")
            .endClass()
        .endModule();
}

void register_Class_Protocol(lua_State* L)
{
    /*
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addConstant("PTDATA_TYPE_NONE", kit::PTDATA_TYPE_NONE)
            .addConstant("PTDATA_TYPE_NORMAL", kit::PTDATA_TYPE_NORMAL)
            .addConstant("PTDATA_TYPE_VECTOR", kit::PTDATA_TYPE_VECTOR)
            .addConstant("PTDATA_TYPE_PROTOCOL", kit::PTDATA_TYPE_PROTOCOL)
            .beginExtendClass<kit::PTData, kit::Ref>("ptdata")
                .addFunction("serialize", &kit::PTData::serialize)
                .addFunction("unserialize", &kit::PTData::unserialize)
                .addFunction("get_key", &kit::PTData::getKey)
                .addFunction("get_type", &kit::PTData::getType)
            .endClass()
            .beginExtendClass<kit::PTDataNormal, kit::PTData>("ptdatanormal")
                .addFunction("get_value", &kit::PTDataNormal)
            .endClass()
        .endModule();
    */
}

void register_Class_Client(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Client, kit::Ref>("client")
                .addStaticFunction("create", &kit::Client::create)
                .addFunction("startup", &kit::Client::startup)
            .endClass()
        .endModule();
}

void register_Class_Server(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Server, kit::Ref>("server")
                .addStaticFunction("create", &kit::Server::create)
                .addFunction("startup", &kit::Server::startup)
            .endClass()
        .endModule();
}

void register_Class_ScriptCore(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addConstant("SCRIPT_TYPE_NONE", kit::SCRIPT_TYPE_NONE)
            .addConstant("SCRIPT_TYPE_JAVASCRIPT", kit::SCRIPT_TYPE_JAVASCRIPT)
            .addConstant("SCRIPT_TYPE_LUA", kit::SCRIPT_TYPE_LUA)
            .addConstant("SCRIPT_TYPE_PYTHON", kit::SCRIPT_TYPE_PYTHON)
            .beginExtendClass<kit::ScriptCore, kit::Ref>("scriptcore")
                .addFunction("get_script_dir", &kit::ScriptCore::getScriptDir)
                .addFunction("set_script_dir", &kit::ScriptCore::setScriptDir)
                .addFunction("execute_string", &kit::ScriptCore::executeString)
                .addFunction("execute_file", &kit::ScriptCore::executeFile)
                .addFunction("execute_function", &kit::ScriptCore::executeFunction)
            .endClass()
        .endModule();
}

void register_Class_ScriptManager(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::ScriptManager, kit::Ref>("scriptmanager")
                .addFunction("get_core", &kit::ScriptManager::getCore)
                .addFunction("add_core", &kit::ScriptManager::addCore)
            .endClass()
        .endModule();
}

void register_Class_Gear(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Gear, kit::Ref>("gear")
                .addStaticFunction("instance", &kit::Singleton<kit::Gear>::instance)
                .addFunction("stop", &kit::Gear::stop)
                .addFunction("get_frame", &kit::Gear::getFrame)
                .addFunction("get_frame_rate", &kit::Gear::getFrameRate)
                .addFunction("get_frame_delta", &kit::Gear::getFrameDelta)
                //get buffer poll
                .addFunction("get_buffer_pool", [](kit::Gear* self) { return g_BufPoolMng->getPool(); })
            .endClass()
        .endModule();
}

