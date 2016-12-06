/**
*  @Brief: ÎÄ¼þÏÂÔØ
*  @Created by fjut on 16-12-06
*/

#ifndef __H_DOWNLOADER_MANAGER_H__
#define __H_DOWNLOADER_MANAGER_H__

#include "cocos2d.h"
#include "network/CCDownloader.h"

using namespace cocos2d;
using namespace network;

class DownloaderManager
{
public:
	DownloaderManager();
	static DownloaderManager& getInstance();
	static void destroyInstance();

public:
	typedef std::function<void(const DownloadTask& task, std::vector<unsigned char>& data)> OnDataTaskSuccess;
	typedef std::function<void(const DownloadTask& task)> OnFileTaskSuccess;
	typedef std::function<void(const DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)> OnTaskProgress;
	typedef std::function<void(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)> OnTaskError;

public:
	typedef struct DownloaderParam_
	{
		std::string srcUrl;
		std::string storagePath;
		std::string identifier;
		OnDataTaskSuccess onDataTaskSuccess;
		OnFileTaskSuccess onFileTaskSuccess;
		OnTaskProgress onTaskProgress;
		OnTaskError onTaskError;
	}DownloaderParam;

private:
	Downloader* createDownloaderTask(DownloaderParam& downloaderParam);
	void removeDownloaderTask(const std::string& srcUrl);
	
public:
	Downloader* createDownloaderFileTask(DownloaderParam& downloaderParam);
	Downloader* createDownloadDataTask(DownloaderParam& downloaderParam);

private:
	std::map<std::string, Downloader*> m_downloaderMap;
};

#define g_downloader DownloaderManager::getInstance()

#endif 
