#include "LuaBinding.h"
#include <stdio.h>
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"
#include "Logger.h"

#include "LuaFunctions.h"
#include "Ref.h"
#include "Timer.h"
#include "Buffer.h"
#include "BufferPool.h"
#include "LuaBuffer.h"
#include "Packet.h"
#include "LuaProtocol.h"
#include "Client.h"
#include "Server.h"
#include "Gear.h"
#include "ScriptSupport.h"

void test_table(lua_State* L)
{
    LuaIntf::LuaRef tb = LuaIntf::Lua::pop<LuaIntf::LuaRef>(L);
    printf("tablesize:%d\n", tb.len());
    for (auto& e : tb)
    {
        //printf("%d\n", e.value<int>());
        printf("%s,%d\n", e.key<const std::string&>().c_str(), e.value<int>());
    }
    printf("xxxx:%s\n", tb.get<std::string>(2).c_str());
}

void register_CommonConstant(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            DPART(.addConstant("DEBUG_MODE", 1))
        .endModule();
}

void register_NormalFunction(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addFunction("test_table", test_table)
            .addFunction("split", luaSplit)
        .endModule();
}

void register_Class_Logger(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
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
                .addFunction("to_string", &kit::Ref::toString)
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
            .beginExtendClass<LuaBuffer, kit::Ref>("buffer")
                .addStaticFunction("create", &LuaBuffer::create)
                .addFunction("init", &LuaBuffer::init)
                .addFunction("reset", &LuaBuffer::reset)
                .addFunction("get_size", &LuaBuffer::getSize)
                .addFunction("getCapacity", &LuaBuffer::getCapacity)
                .addFunction("get_written_size", &LuaBuffer::getWrittenSize)
                .addFunction("get_writable_size", &LuaBuffer::getWritableSize)
                .addFunction("get_readable_size", &LuaBuffer::getReadableSize)
                .addFunction("skip_write", &LuaBuffer::skipWrite)
                .addFunction("skip_read", &LuaBuffer::skipRead)
                .addFunction("to_string", &LuaBuffer::toString) 
                .addFunction("write_buffer", &LuaBuffer::writeBuffer) // disable ?
                .addFunction("read_buffer", &LuaBuffer::readBuffer) // disable ? 
                .addFunction("w_string", &LuaBuffer::operator<< <const char*> )

                .addFunction("w_int8_t", &LuaBuffer::operator<< <int8_t> )
                .addFunction("r_int8_t", [](LuaBuffer* self){ int8_t t; (*self)>>t; return t;} )
                .addFunction("w_uint8_t", &LuaBuffer::operator<< <uint8_t> )
                .addFunction("r_uint8_t", [](LuaBuffer* self){ uint8_t t; (*self)>>t; return t;} )

                .addFunction("w_int16_t", &LuaBuffer::operator<< <int16_t> )
                .addFunction("r_int16_t", [](LuaBuffer* self){ int16_t t; (*self)>>t; return t;} )
                .addFunction("w_uint16_t", &LuaBuffer::operator<< <uint16_t> )
                .addFunction("r_uint16_t", [](LuaBuffer* self){ uint16_t t; (*self)>>t; return t;} )

                .addFunction("w_int32_t", &LuaBuffer::operator<< <int32_t> )
                .addFunction("r_int32_t", [](LuaBuffer* self){ int32_t t; (*self)>>t; return t;} )
                .addFunction("w_uint32_t", &LuaBuffer::operator<< <uint32_t> )
                .addFunction("r_uint32_t", [](LuaBuffer* self){ uint32_t t; (*self)>>t; return t;} )

                .addFunction("write_format", &LuaBuffer::luaWriteFormat)
                .addFunction("read_format", &LuaBuffer::luaReadFormat)
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

void register_Class_Protocol(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .addConstant("INT8", toint(kit::PTType::INT8))
            .addConstant("UINT8", toint(kit::PTType::UINT8))
            .addConstant("INT16", toint(kit::PTType::INT16))
            .addConstant("UINT16", toint(kit::PTType::UINT16))
            .addConstant("INT32", toint(kit::PTType::INT32))
            .addConstant("UINT32", toint(kit::PTType::UINT32))
            .addConstant("INT64", toint(kit::PTType::INT64))
            .addConstant("UINT64", toint(kit::PTType::UINT64))
            .addConstant("VARINT", toint(kit::PTType::VARINT))
            .addConstant("STRING", toint(kit::PTType::STRING))

            .addConstant("ARRAY", toint(kit::PTType::ARRAY))
            .addConstant("TABLE", toint(kit::PTType::TABLE))
            .beginExtendClass<kit::Protocol, kit::Ref>("protocolbase")
                .addFunction("init", &kit::Protocol::init)
            .endClass()
            .beginExtendClass<LuaProtocol, kit::Protocol>("protocol")
                .addStaticFunction("create", &LuaProtocol::create)
                .addFunction("build", &LuaProtocol::buildFromTable)
                .addFunction("serialize", &LuaProtocol::serializeToTable)
                .addFunction("unserialize", &LuaProtocol::unserializeFromTable)
            .endClass()
        .endModule();
}

void register_Class_Terminal(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Terminal, kit::Ref>("terminal")
                .addFunction("add_protocol", &kit::Terminal::addProtocol)
                .addFunction("del_protocol", &kit::Terminal::delProtocol)
                .addFunction("send_protocol", &kit::Terminal::sendProtocol)
            .endClass()
        .endModule();
}

void register_Class_Client(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Client, kit::Terminal>("client")
                .addStaticFunction("create", &kit::Client::create)
                .addFunction("startup", &kit::Client::startup)
                .addFunction("send_protocol", &kit::Client::sendProtocol)
            .endClass()
        .endModule();
}

void register_Class_Server(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::Server, kit::Terminal>("server")
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


