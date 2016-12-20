/**
*  @Brief: zipÏÂÔØ¸üÐÂ
*  @Created by fjut on 16-12-07
*/

#include "MyAssertManager.h"
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//#include <dirent.h>
//#include <sys/stat.h>
//#endif
//
//static std::string createDownloadedDir(const std::string& path)
//{
//	std::string pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
//	pathToSave += path;
//	// Create the folder if it doesn't exist
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
//	DIR *pDir = NULL;
//	pDir = opendir(pathToSave.c_str());
//	if (!pDir)
//	{
//		mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
//	}
//#else
//	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
//	{
//		CreateDirectoryA(pathToSave.c_str(), 0);
//	}
//#endif
//	return pathToSave;
//}
//
//MyAssertManager::MyAssertManager()
//{
//}
//
//MyAssertManager& MyAssertManager::getInstance()
//{
//	static MyAssertManager _instance;
//	return _instance;
//}
//
//void MyAssertManager::destroyInstance()
//{
//}
//
//void MyAssertManager::createAssertTask(const string& packageUrl, const string& versionFileUrl, const string& storagePath,
//	const AssetsManager::ErrorCallback& errorCallback, const AssetsManager::ProgressCallback& progressCallback,
//	const AssetsManager::SuccessCallback& successCallback)
//{
//	auto it = m_map.find(packageUrl);
//	if (it != m_map.end())
//	{
//		CCLOG("the assertmanager is handling, packageUrl = %s", packageUrl.c_str());
//		return nullptr;
//	}
//	auto asser = new MyAssert();
//	asser->createAssert(packageUrl, versionFileUrl, storagePath, errorCallback, progressCallback, successCallback);
//	m_map[packageUrl] = asser;
//	return asser;
//}
//
//MyAssert::MyAssert()
//{
//	assert_ = nullptr;
//}
//
//MyAssert::~MyAssert()
//{
//	CC_SAFE_DELETE(assert_);
//}
//
//AssetsManager* MyAssert::createAssert(const string& packageUrl, const string& versionFileUrl, const string& storagePath,
//	const AssetsManager::ErrorCallback& errorCallback, const AssetsManager::ProgressCallback& progressCallback,
//	const AssetsManager::SuccessCallback& successCallback)
//{
//	std::string path = createDownloadedDir(storagePath);
//	assert_ = new AssetsManager(packageUrl.c_str(), versionFileUrl.c_str(), path.c_str());
//	errorCallback_ = errorCallback;
//	progressCallback_ = progressCallback;
//	successCallback_ = successCallback;
//	assert_->setDelegate(this);
//	return assert_;
//}
//
//void MyAssert::onError(AssetsManager::ErrorCode errorCode)
//{
//	//if (errorCallback_) errorCallback_(errorCode);
//}
//
//void MyAssert::onProgress(int percent)
//{
//
//}
//
//void MyAssert::onSuccess()
//{
//
//}

//void MyAssert::onNewVersion(const string& version)
//{
//}