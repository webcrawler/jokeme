/**
*  @brief: 获取远程文件大小
*  @Created by fjut on 17-01-14
*/

#ifndef __WEB_FILE_SIZE_H__
#define __WEB_FILE_SIZE_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class WebFileSize
{
public:
	static void addWebFileSizeTask(const std::string& url, const std::function<void(float fileSize)>& callback_);


};


#endif 
