#ifndef __KIT_DEBUG_INPUT_H__
#define __KIT_DEBUG_INPUT_H__

#include "Ref.h"
#include "Singleton.h"
#include <functional>
#include <string>
#include <unordered_map>

#ifndef DEBUG_INPUT_SIZE
#define DEBUG_INPUT_SIZE (512)
#endif

namespace kit {

typedef std::function<void(const std::string& cmd)> DebugCmdHandler;

class DebugInput : public Ref
{
public:
    DebugInput();
    virtual ~DebugInput();

    virtual bool baseInit();

    virtual void update();

    virtual void setHandler(const std::string& cmd, DebugCmdHandler handler);
private:
    char buf_[DEBUG_INPUT_SIZE];

    std::unordered_map<std::string, DebugCmdHandler> handler_map_;
};

#define g_Debug (kit::Singleton<kit::DebugInput>::instance())

} // namespace kit

#endif

