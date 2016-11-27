/**
*  @brief: ∫∫◊÷≈‰÷√
*  @Created by fjut on 16-11-03
*/

#ifndef __LOCALLIZED_STRING_H__
#define __LOCALLIZED_STRING_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class LocalizedString
{
public:
	LocalizedString() { };
	static LocalizedString& getInstance();
	static void destroyInstance();

private:
	ValueMap plist;

};

#define g_Local LocalizedString::getInstance()

#endif 
