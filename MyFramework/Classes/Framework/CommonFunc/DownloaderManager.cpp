/**
*  @Brief: ÎÄ¼þÏÂÔØ
*  @Created by fjut on 16-12-06
*/

#include "DownloaderManager.h"

DownloaderManager::DownloaderManager()
{
}

DownloaderManager& DownloaderManager::getInstance()
{
	static DownloaderManager _instance;
	return _instance;
}

void DownloaderManager::destroyInstance()
{

}

void DownloaderManager::removeDownloaderTask(const std::string& srcUrl)
{
	auto it = m_downloaderMap.find(srcUrl);
	if (it == m_downloaderMap.end())
	{
		CCLOG("removeDownloaderTask fail, the task = %s is not found !", srcUrl.c_str());
	}
	delete m_downloaderMap[srcUrl];
	m_downloaderMap.erase(it);
	//CCLOG("removeDownloaderTask: %s", srcUrl);
}

Downloader* DownloaderManager::createDownloaderTask(DownloaderParam& downloaderParam)
{
	auto downloader = new Downloader();
	downloader->onTaskProgress = [=](const DownloadTask& task,
		int64_t bytesReceived,
		int64_t totalBytesReceived,
		int64_t totalBytesExpected)
	{
		//CCLOG("onTaskProgress++++");
		if (downloaderParam.onTaskProgress)
		{
			downloaderParam.onTaskProgress(task, bytesReceived, totalBytesReceived, totalBytesExpected);
		}
	};

	downloader->onFileTaskSuccess = [=](const DownloadTask& task)
	{
		//CCLOG("onFileTaskSuccess++++: %s", task.requestURL.c_str());
		if (downloaderParam.onFileTaskSuccess)
		{
			downloaderParam.onFileTaskSuccess(task);
		}
		removeDownloaderTask(task.requestURL);
	};

	downloader->onDataTaskSuccess = [=](const DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		//CCLOG("onDataTaskSuccess++++: %s", task.requestURL.c_str());
		if (downloaderParam.onDataTaskSuccess)
		{
			downloaderParam.onDataTaskSuccess(task, data);
		}
		removeDownloaderTask(task.requestURL);
	};

	downloader->onTaskError = [=](const DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		//CCLOG("onTaskError++++: %s", task.requestURL.c_str());
		if (downloaderParam.onTaskError)
		{
			downloaderParam.onTaskError(task, errorCode, errorCodeInternal, errorStr);
		}
		removeDownloaderTask(task.requestURL);
	};
	return downloader;
}

Downloader* DownloaderManager::createDownloaderFileTask(DownloaderParam& downloaderParam)
{
	if (m_downloaderMap.find(downloaderParam.srcUrl) != m_downloaderMap.end())
	{
		CCLOG("createDownloaderFileTask fail, the task = %s is downloading", downloaderParam.srcUrl.c_str());
		return nullptr;
	}

	auto downloader = createDownloaderTask(downloaderParam);
	downloader->createDownloadFileTask(downloaderParam.srcUrl, downloaderParam.storagePath, downloaderParam.identifier);

	m_downloaderMap[downloaderParam.srcUrl] = downloader;
	return downloader;
}

Downloader* DownloaderManager::createDownloadDataTask(DownloaderParam& downloaderParam)
{
	if (m_downloaderMap.find(downloaderParam.srcUrl) != m_downloaderMap.end())
	{
		CCLOG("createDownloadDataTask fail, the task = %s is downloading", downloaderParam.srcUrl.c_str());
		return nullptr;
	}

	auto downloader = createDownloaderTask(downloaderParam);
	downloader->createDownloadDataTask(downloaderParam.srcUrl, downloaderParam.identifier);

	m_downloaderMap[downloaderParam.srcUrl] = downloader;
	return downloader;
}