
#ifndef __STRING_TOOL_H__
#define __STRING_TOOL_H__

#include <stdio.h>
#include <string>

namespace StringTool 
{
    std::string formatStr(const char* fmt, ...);
    bool stringToBool(const std::string& value);
    std::string replaceString(const std::string& origin_str, const std::string& old_sub_str, const std::string& new_sub_str);
}

#endif 
