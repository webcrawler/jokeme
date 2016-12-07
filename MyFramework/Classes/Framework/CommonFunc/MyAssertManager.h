/**
*  @Brief: zipÏÂÔØ¸üÐÂ
*  @Created by fjut on 16-12-07
*/

#ifndef __H_MY_ASSERTMANAGER_MANAGER_H__
#define __H_MY_ASSERTMANAGER_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MyAssertManager
{
public:
	MyAssertManager();
	static MyAssertManager& getInstance();
	static void destroyInstance();

public:
	AssetsManager* create(const char* packageUrl, const char* versionFileUrl, const char* storagePath,
		AssetsManager::ErrorCallback errorCallback, AssetsManager::ProgressCallback progressCallback, AssetsManager::SuccessCallback successCallback);
};

#define g_assert MyAssertManager::getInstance()

#endif 
