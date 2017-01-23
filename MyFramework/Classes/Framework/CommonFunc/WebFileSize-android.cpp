/**
*  @brief: 获取远程文件大小
*  @Created by fjut on 17-01-14
*/

#include "WebFileSize.h"
#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

static const char* PACKAGE_NAME = "org/cocos2dx/cpp/WebFileSize";
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
static bool getMethod(JniMethodInfo& mi, const char* methodName, const char* signature)
{
	return JniHelper::getStaticMethodInfo(mi, PACKAGE_NAME, methodName, signature);
}

static void releaseMethod(JniMethodInfo& mi)
{
	mi.env->DeleteLocalRef(mi.classID);
}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
void WebFileSize::addWebFileSizeTask(const std::string& url, const std::function<void(float fileSize)>& callback_)
{
	string size = "";
	JniMethodInfo mi;
	bool isHave = getMethod(mi, "getWebFileSize", "(Ljava/lang/String;)Ljava/lang/String;");
	if (isHave)
	{
		jstring jsUrl = mi.env->NewStringUTF(url.c_str());
		jstring jstr_value = (jstring)mi.env->CallStaticObjectMethod(mi.classID, mi.methodID, jsUrl);
		size = cocos2d::JniHelper::jstring2string(jstr_value);
		mi.env->DeleteLocalRef(jsUrl);
		mi.env->DeleteLocalRef(jstr_value);
		releaseMethod(mi);
	}
	float val = atof(size.c_str());
	val = val / (1024 * 1024);
	if (callback_) callback_(val);
}
#endif