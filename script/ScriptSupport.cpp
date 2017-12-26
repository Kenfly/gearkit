#include "ScriptSupport.h"
#include <memory.h>

namespace kit {

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
}

bool ScriptManager::baseInit()
{
    memset(cores_, 0, sizeof(ScriptCore*) * SCRIPT_TYPE_MAX);
    return true;
}

void ScriptManager::addCore(ScriptCore* core)
{
    ScriptType type = core->getScriptType();

    ScriptCore* old_core = cores_[type];
    KIT_SAFE_RELEASE(old_core);

    core->retain();
    cores_[type] = core;
}

} // namespace kit

