#include "LuaDebug.h"
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"

#include "DebugInput.h"


void register_debug(lua_State* L)
{
    register_Class_DebugInput(L);
}


void register_Class_DebugInput(lua_State* L)
{
    LuaIntf::LuaBinding(L)
        .beginModule("kit")
            .beginExtendClass<kit::DebugInput, kit::Ref>("debuginput")
                .addStaticFunction("create", &kit::DebugInput::create)
                .addFunction("set_handler", &kit::DebugInput::setHandler)
            .endClass()
        .endModule();
}
