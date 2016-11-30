/**
*  @brief: 本地化配置文件读取
*  @Created by fjut on 16-11-28
*/

#include "LocalizedString.h"

static const char* local_plist = "config/localstring.plist";

LocalizedString::LocalizedString()
{
	m_plist = FileUtils::getInstance()->getValueMapFromFile(local_plist);
}

LocalizedString& LocalizedString::getInstance()
{
	static LocalizedString _instance;
	return _instance;
}

void LocalizedString::destroyInstance()
{

}

Value LocalizedString::getValue(const std::string& str)
{
	auto it = m_plist.find(str);
	if (it == m_plist.end())
	{
		CCLOG("can't find the value by the key: %s", str.c_str());
		return Value::Null;
	}
	return m_plist[str];
}

std::string LocalizedString::getStr(const std::string& str)
{
	auto value = getValue(str);
	if (!value.isNull())
	{
		return value.asString();
	}
	CCASSERT(!value.isNull(), "LocalizedString::getStr");
	return "";
}

int LocalizedString::getInt(const std::string& str)
{
	auto value = getValue(str);
	if (!value.isNull())
	{
		return value.asInt();
	}
	CCASSERT(!value.isNull(), "LocalizedString::getInt");
	return 0;
}

float LocalizedString::getFloat(const std::string& str)
{
	auto value = getValue(str);
	if (!value.isNull())
	{
		return value.asFloat();
	}
	CCASSERT(!value.isNull(), "LocalizedString::getFloat");
	return 0;
}

bool LocalizedString::getBool(const std::string& str)
{
	auto value = getValue(str);
	if (!value.isNull())
	{
		return value.asBool();
	}
	CCASSERT(!value.isNull(), "LocalizedString::getBool");
	return false;
}
