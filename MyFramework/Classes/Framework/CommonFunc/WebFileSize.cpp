/**
*  @brief: 获取远程文件大小
*  @Created by fjut on 17-01-14
*/

#include "WebFileSize.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <curl/curl.h>
#include <curl/easy.h>

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME	5L
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void WebFileSize::addWebFileSizeTask(const std::string& url, const std::function<void(float fileSize)>& callback_)
{
	auto curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, [=](void *ptr, size_t size, size_t nmemb, void *data) {
		return (size_t)(size * nmemb);
	});
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);

	double downLength = 0;
	CURLcode res = curl_easy_perform(curl);
	if (res != 0)
	{
		curl_easy_cleanup(curl);
		if (callback_) callback_(downLength);
		return;
	}
	long retCode = 0;
	CURLcode returnCode = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retCode);
	//CCLOG("return_code:%ld", retCode);
	if ((CURLE_OK != returnCode) || !retCode)
	{
		if (callback_) callback_(downLength);
	}
	else
	{
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downLength);
		//CCLOG("downLength:%f", downLength);
		float size_ = downLength / (1024 * 1024); // MB
		if (callback_) callback_(size_);
	}
	curl_easy_cleanup(curl);
}
#endif