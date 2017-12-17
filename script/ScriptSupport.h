#ifndef __KIT_SCRIPT_SUPPORT_H__
#define __KIT_SCRIPT_SUPPORT_H__

namespace kit {

enum ScriptType {
    SCRIPT_TYPE_NONE = 0,
    SCRIPT_TYPE_JAVASCRIPT,
    SCRIPT_TYPE_LUA,
    SCRIPT_TYPE_PYTHON,
};

struct ScriptProtocol
{
    ScriptProtocol() {}
    virtual ~ScriptProtocol() {}

    virtual ScriptType getScriptType() { return SCRIPT_TYPE_NONE; }

    virtual void init() {}
    virtual void destroy() {}

    // @return 0 corectly.
    virtual int executeString(const char* codes) = 0;
    virtual int executeFile(const char* filename) = 0;
    virtual int executeFunction(const char* function_name) = 0;
};

class ScriptManager
{
public:
    ScriptManager();
    virtual ~ScriptManager();

    virtual void init();
};

} // namespace kit

#endif
