#ifndef __JS_CORE_H__
#define __JS_CORE_H__

#include "ScriptSupport.h"

class JSCore : public kit::ScriptProtocol
{
public:
    JSCore();
    virtual ~JSCore();

    virtual ScriptType getScriptType() const { return SCRIPT_TYPE_JAVASCRIPT; }

    virtual void init();
    virtual void destroy();

    // @return 0 corectly.
    virtual int executeString(const char* codes);
    virtual int executeFile(const char* filename);
    virtual int executeFunction(const char* function_name);

private:
};

#endif

