#ifndef __Lua_CORE_H__
#define __Lua_CORE_H__

#include "ScriptSupport.h"
#include "kitsys.h"

struct lua_State;

class LuaCore : public kit::ScriptCore
{
public:
    KIT_CREATE_FUNC(LuaCore)

    LuaCore();
    virtual ~LuaCore();

    virtual kit::ScriptType getScriptType() const { return kit::SCRIPT_TYPE_JAVASCRIPT; }

    virtual bool baseInit();

    virtual void destroy();

    // @return 0 corectly.
    virtual int executeString(const char* codes);
    virtual int executeFile(const char* filename);
    virtual int executeFunction(const char* function_name);
protected:
    int executeFunc(int arg_num);
private:
    lua_State* L;
};

#endif

