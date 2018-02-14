#ifndef __KIT_FUNCTIONS_H__
#define __KIT_FUNCTIONS_H__

#include <vector>
#include <string>

namespace kit {

void split(const std::string& text, std::vector<std::string>& vec, const char separator);
void splitWithString(const std::string& text, std::vector<std::string>& vec, const std::string& separator);

std::string formatString(const char* format, ...);

} // namespace kit

#endif

