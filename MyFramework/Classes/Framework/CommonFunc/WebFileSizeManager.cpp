/**
*  @brief: 获取远程文件大小
*  @Created by fjut on 17-01-14
*/

#include "WebFileSizeManager.h"
//#include <curl/curl.h>
//#include <curl/easy.h>
//#include <thread>
//
//#define LOW_SPEED_LIMIT 1L
//#define LOW_SPEED_TIME	5L
//
//size_t save_header(void *ptr, size_t size, size_t nmemb, void *data)
//{
//	return (size_t)(size * nmemb);
//}
//
//void WebFileSizeGetter::getFileSize(const string& url, const std::function<void(float fileSize)>& callback_, const string& flag)
//{
//	m_callback = callback_;
//	m_flag = flag;
//	auto curl = curl_easy_init();
//	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");    
//	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
//	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, save_header);
//	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
//	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
//	auto t = std::thread(&WebFileSizeGetter::onURLThread, this, curl);
//	t.detach();
//}
//
//void WebFileSizeGetter::onURLThread(void* curl)
//{
//	double downLength = 0;
//	CURLcode res;
//	do 
//	{
//		res = curl_easy_perform(curl);
//		if (res != 0) break;
//		else
//		{
//			long retcode = 0;
//			CURLcode return_code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
//			//CCLOG("return_code:%ld", retcode);
//			if ((CURLE_OK != return_code) || !retcode) break;
//			// 响应内容长度
//			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downLength);
//			//CCLOG("downLength:%f", downLength);
//		}
//	} while (0);
//	curl_easy_cleanup(curl);
//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//		float size_ = downLength /(1024*1024); // MB
//		if (m_callback) m_callback(size_);
//		g_webfilesize.removeWebFileSizeTask(m_flag);
//	});
//}
//
//static std::string keyWithHash(const std::string& url)
//{
//	char buf[256];
//	sprintf(buf, "%zd", std::hash<std::string>()(url));
//	return buf;
//}
//
//WebFileSizeManager& WebFileSizeManager::getInstance()
//{
//	static WebFileSizeManager instance_;
//	return instance_;
//}
//
//void WebFileSizeManager::destroyInstance()
//{
//}
//
//void WebFileSizeManager::addWebFileSizeTask(const string& url, const std::function<void(float fileSize)>& callback_)
//{
//	string str = keyWithHash(url);
//	auto it = m_map.find(str);
//	if (it != m_map.end())
//	{
//		CCLOG("addWebFileSizeTask fail, the task = %s has exist !", url.c_str());
//		return;
//	}
//	auto p = new WebFileSizeGetter();
//	p->getFileSize(url, callback_, str);
//	m_map[str] = p;
//}
//
//void WebFileSizeManager::removeWebFileSizeTask(const string& flag)
//{
//	auto it = m_map.find(flag);
//	if (it != m_map.end())
//	{
//		delete m_map[flag];
//		m_map.erase(it);
//	}
//}