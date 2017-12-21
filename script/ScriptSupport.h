#ifndef __KIT_SCRIPT_SUPPORT_H__
#define __KIT_SCRIPT_SUPPORT_H__

#include <string>

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

    virtual void setScriptDir(const char* path) { path_ = path; }
    virtual const char* getScriptDir() { return path_.c_str(); }

    // @return 0 corectly.
    virtual int executeString(const char* codes) = 0;
    virtual int executeFile(const char* file_name) = 0;
    virtual int executeFunction(const char* function_name) = 0;

    std::string path_;
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
