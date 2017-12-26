#ifndef __KIT_SCRIPT_SUPPORT_H__
#define __KIT_SCRIPT_SUPPORT_H__

#include "Ref.h"
#include <string>

namespace kit {

enum ScriptType {
    SCRIPT_TYPE_NONE = 0,
    SCRIPT_TYPE_JAVASCRIPT,
    SCRIPT_TYPE_LUA,
    SCRIPT_TYPE_PYTHON,

    SCRIPT_TYPE_MAX,
};

class ScriptCore : public Ref
{
public:
    ScriptCore() {}
    virtual ~ScriptCore() {}

    virtual ScriptType getScriptType() { return SCRIPT_TYPE_NONE; }

    virtual void destroy() {}

    virtual void setScriptDir(const char* path) { path_ = path; }
    virtual const char* getScriptDir() { return path_.c_str(); }

    // @return 0 corectly.
    virtual int executeString(const char* codes) = 0;
    virtual int executeFile(const char* file_name) = 0;
    virtual int executeFunction(const char* function_name) = 0;

protected:
    std::string path_;
};

class ScriptManager
{
public:
    ScriptManager();
    virtual ~ScriptManager();

    virtual bool baseInit();
    virtual ScriptCore* getCore(ScriptType type) const { return cores_[type]; }
    virtual void addCore(ScriptCore* core);
private:
    ScriptCore* cores_[SCRIPT_TYPE_MAX];
};

} // namespace kit

#define g_ScriptManager (kit::Singleton<kit::ScriptManager>::instance())

#endif

