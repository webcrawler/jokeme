/**
*  @brief: ��ȡԶ���ļ���С
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
    // ����Ϊͷ��Ϣ����
    [request setHTTPMethod:@"HEAD"];
    NSURLResponse* response;
    NSError* error;
    // ע������ʹ����ͬ������ֱ�ӽ��ļ���С����
    [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    // ȡ�����ݳ���
	float val = response.expectedContentLength;
	val = val / (1024 * 1024);
	if (callback_) callback_(val);
}
#endif