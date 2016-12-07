/**
*  @Brief: zipÏÂÔØ¸üĞÂ
*  @Created by fjut on 16-12-07
*/

#include "MyAssertManager.h"

MyAssertManager::MyAssertManager()
{
}

MyAssertManager& MyAssertManager::getInstance()
{
	static MyAssertManager _instance;
	return _instance;
}

void MyAssertManager::destroyInstance()
{

}

AssetsManager* MyAssertManager::create(const char* packageUrl, const char* versionFileUrl, const char* storagePath,
	AssetsManager::ErrorCallback errorCallback, AssetsManager::ProgressCallback progressCallback, AssetsManager::SuccessCallback successCallback)
{
	AssetsManager* manager = AssetsManager::create(packageUrl, versionFileUrl, storagePath, errorCallback, progressCallback, successCallback);
	return manager;
}