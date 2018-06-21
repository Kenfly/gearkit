#ifndef __KIT_FUNCTIONS_H__
#define __KIT_FUNCTIONS_H__

#include "kitsys.h"
#include <vector>
#include <string>

namespace kit {

// string split
void split(const std::string& text, std::vector<std::string>& vec, const char separator);
void splitWithString(const std::string& text, std::vector<std::string>& vec, const std::string& separator);

// format string
std::string formatString(const char* format, ...);

// 2^n > x, return the minmax n
inline void upperlog2x(int32_t x, int32_t& k, int32_t& n)
{
    while (k < x)
    {
        ++n;
        k <<= 1;
    }
}

} // namespace kit

#endif

