/**
*  @Brief: zip下载更新
*  @Created by fjut on 16-12-07
*/

#ifndef __H_MY_ASSERTMANAGER_MANAGER_H__
#define __H_MY_ASSERTMANAGER_MANAGER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

//class MyAssert : public AssetsManagerDelegateProtocol    // todo
//{
//public:
//	MyAssert();
//	~MyAssert();
//
//public:
//	virtual void onError(AssetsManager::ErrorCode errorCode);
//	virtual void onProgress(int percent);
//	virtual void onSuccess();
//	//virtual void onNewVersion(const string& version);
//
//public:
//	AssetsManager* createAssert(const string& packageUrl, const string& versionFileUrl, const string& storagePath,
//		const AssetsManager::ErrorCallback& errorCallback, const AssetsManager::ProgressCallback& progressCallback, 
//		const AssetsManager::SuccessCallback& successCallback);
//
//private:
//	AssetsManager* assert_;
//	AssetsManager::ErrorCallback errorCallback_;
//	AssetsManager::ProgressCallback progressCallback_;
//	AssetsManager::SuccessCallback successCallback_;
//};
//
//class MyAssertManager
//{
//public:
//	MyAssertManager();
//	static MyAssertManager& getInstance();
//	static void destroyInstance();
//
//public:
//	void createAssertTask(const string& packageUrl, const string& versionFileUrl, const string& storagePath,
//		const AssetsManager::ErrorCallback& errorCallback, const AssetsManager::ProgressCallback& progressCallback,
//		const AssetsManager::SuccessCallback& successCallback);
//
//public:
//	std::map<string, MyAssert> m_map;
//};
//
//#define g_assert MyAssertManager::getInstance()

#endif 
