
#include "StringTools.h"
#include <stdarg.h>
#include "cocos2d.h"

namespace StringTool
{
    std::string formatStr(const char* fmt, ...)
	{
        char buf[255];
        memset(buf, 255,0);
        va_list args;
        va_start(args,fmt);
        //#if CC_TARGET_OS==IOS
        ////    vsprintf(buf, 255, fmt,args);
        //#else
        //	vsprintf_s(buf, 255, fmt,args);
        //#endif
        
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
        vsnprintf(buf, 255, fmt, args);
#else
        vsprintf_s(buf, 255, fmt, args);
#endif
        va_end(args);
        return std::string(buf);
    }
    
    bool stringToBool(const std::string& s_value)
	{
        bool value = false;
        if(s_value == "true" || s_value == "TRUE" || s_value == "True"){
            value = true;
        }else if(s_value == "false" || s_value == "False" || s_value == "FALSE"){
            value = false;
        }else{
            value = (std::atoi(s_value.c_str())>0);
        }
        
        return  value;
    }
    
    std::string replaceString(const std::string& origin_str, const std::string& old_sub_str, const std::string& new_sub_str)
	{
        std::string str = origin_str; 
        auto pos = str.find(old_sub_str);
        while(pos != std::string::npos){
            str.replace(pos, old_sub_str.length(), new_sub_str);
            pos = str.find(old_sub_str, pos+new_sub_str.length());
        }
        
        return str;
    }
}

