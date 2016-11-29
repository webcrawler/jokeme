/**
*  @brief: ∫∫◊÷≈‰÷√
*  @Created by fjut on 16-11-03
*/

#include "LocalizedString.h"

LocalizedString& LocalizedString::getInstance()
{
	static LocalizedString instance_;
	return instance_;
}

void LocalizedString::destroyInstance()
{
	//LocalizedString::getInstance().closeAllScenes();
}

