/**
*  @brief: 本地化配置文件读取
*  @Created by fjut on 16-11-28
*/

#ifndef __H_LOCAL_STRING_H__
#define __H_LOCAL_STRING_H__

#include "cocos2d.h"

using namespace cocos2d;

class LocalizedString
{
public:
	LocalizedString();
	static LocalizedString& getInstance();
	static void destroyInstance();

public:
	Value getValue(const std::string& str);
	std::string getStr(const std::string& str);
	int getInt(const std::string& str);
	float getFloat(const std::string& str);
	bool getBool(const std::string& str);

private:
	ValueMap m_plist;

};

#define g_local LocalizedString::getInstance()

#endif 
