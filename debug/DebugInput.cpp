#include "DebugInput.h"
#include <stdio.h>
//TODO: mult platform
#include <sys/ioctl.h>
#include <unistd.h>

namespace kit {

DebugInput::DebugInput()
{
}

DebugInput::~DebugInput()
{
}

bool DebugInput::baseInit()
{
    int32_t noblock = 1;
    ioctl(STDIN_FILENO, FIONBIO, &noblock);
    return true;
}

void DebugInput::update()
{
    int32_t len = read(STDIN_FILENO, buf_, DEBUG_INPUT_SIZE);
    if (len > 0)
    {
        buf_[len - 1] = '\0';
        auto ix = handler_map_.find(buf_);
        if (ix != handler_map_.end())
        {
            ix->second(buf_);
        }
    }
}

void DebugInput::setHandler(const std::string& cmd, DebugCmdHandler handler)
{
    handler_map_[cmd] = handler;
}

} // namespace kit

