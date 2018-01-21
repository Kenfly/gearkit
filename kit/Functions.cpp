#include "Functions.h"

namespace kit {

void split(const std::string& text, std::vector<std::string>& vec, char separator)
{
    std::string::size_type pos = 0;
    std::string::size_type ed = 0;
    std::string::size_type length = text.length();
    while (pos < length)
    {
        if (text[pos] == separator)
        {
            ++pos;
            continue;
        }
        ed = text.find(separator, pos);
        if (ed == std::string::npos)
            break;
        vec.push_back(text.substr(pos, ed - pos));
        pos = ed + 1;
    }
    if (pos < length)
        vec.push_back(text.substr(pos, length - pos));
}

void splitWithString(const std::string& text, std::vector<std::string>& vec, const std::string& separator)
{
    std::string::size_type pos = 0;
    std::string::size_type ed = 0;
    std::string::size_type length = text.length();
    std::string::size_type length_sp = separator.length();
    while (pos < length)
    {
        ed = text.find(separator, pos);
        if (ed == std::string::npos)
            break;
        if (ed == pos)
        {
            pos += length_sp;
            continue;
        }
        vec.push_back(text.substr(pos, ed - pos));
        pos = ed + 1;
    }
    if (pos < length)
        vec.push_back(text.substr(pos, length - pos));
}


} // namepsace kit

