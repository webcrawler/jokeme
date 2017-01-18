/**
*  @brief: 获取远程文件大小
*  @Created by fjut on 17-01-14
*/

#include "WebFileSize.h"
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
// todo
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
void WebFileSize::addWebFileSizeTask(const std::string& url, const std::function<void(float fileSize)>& callback_)
{
    // create request with url
    NSString* urlstring = [NSString stringWithUTF8String:url];
    NSURL* url_ = [NSURL URLWithString:urlstring];
	
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url_ 
                                            cachePolicy:NSURLRequestReloadIgnoringCacheData 
                                        timeoutInterval:3.0f];
    // 设置为头信息请求
    [request setHTTPMethod:@"HEAD"];
    NSURLResponse* response;
    NSError* error;
    // 注意这里使用了同步请求，直接将文件大小返回
    [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    // 取得内容长度
	float val = response.expectedContentLength;
	val = val / (1024 * 1024);
	if (callback_) callback_(val);
}
#endif