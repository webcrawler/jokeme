
#include "LayerReg.h"

LayerReg* LayerReg::create()
{
	LayerReg* ptr = new LayerReg();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string LayerReg::getWidgetFilePath()
{
	return "layer_reg";
}

string LayerReg::getLayerName()
{
	return "UI_LayerReg";
}

void LayerReg::updateUI()
{
	Button* btn = static_cast<Button*>(getWidgetByName("Button_2"));
	btn->addTouchEventListener(CC_CALLBACK_2(LayerReg::callback, this));
	btn = static_cast<Button*>(getWidgetByName("Button_2_0"));
	btn->addTouchEventListener(CC_CALLBACK_2(LayerReg::callback, this));
}

#define MDM_GP_LOGON	1	
#define SUB_GP_LOGON_FAST	4

static std::string getSerialNumber()
{
#if 0
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	char	volumeInfo[100];
	DWORD   volSerialNumber;
	DWORD   volMaxComponentLength;
	DWORD   volFileSystemFlags;
	char    fileSystemNameBuffer[100];
	GetVolumeInformationA("c:\\", volumeInfo, sizeof(volumeInfo), &volSerialNumber,
		&volMaxComponentLength, &volFileSystemFlags, fileSystemNameBuffer, sizeof(fileSystemNameBuffer));

	char str[64];
	sprintf(str, "%u", volSerialNumber);
	return MD5(str).hexdigest();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo info;
	std::string strSerialNumber;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/model/SystemModule", "getSerialNumber", "()Ljava/lang/String;");
	if (ret)
	{
		jstring jsSerialNumber = (jstring)info.env->CallStaticObjectMethod(info.classID, info.methodID);
		strSerialNumber = JniHelper::jstring2string(jsSerialNumber);
	}
	return MD5(strSerialNumber).hexdigest();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString *deviceID = [OpenUDID value];
	const char* deviceid = [deviceID UTF8String];
	//shen+ may have problem,need modify
	char szImei[16] = { 0 };
	int index = 0;
	for (int i = 0; i < strlen(deviceid) && index < 15; i += 2) {
		int temp = char2Int(deviceid[i]) + char2Int(deviceid[i + 1]);
		if (temp >= 30) {
			szImei[index++] = temp - 30 + '0';
		}
		else if (temp >= 20) {
			szImei[index++] = temp - 20 + '0';
		}
		else if (temp >= 10) {
			szImei[index++] = temp - 10 + '0';
		}
		else {
			szImei[index++] = temp + '0';
		}
	}
	return MD5(szImei).hexdigest();
#endif

#endif
	return "";
}

void LayerReg::callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	string name = ((Button*)pSender)->getName();
	if (name == "Button_2")
	{
		g_ui.closeLayer(this);
	}
	else if (name == "Button_2_0")
	{
		// ×¢²áµÇÂ¼
	}
}


void LayerReg::eventTest(void* data)
{
	CCLOG("%s, %s\n", "LayerReg::eventTest(): ", data);
}

void LayerReg::eventTest1(void* data)
{
	CCLOG("%s, %s\n", "LayerReg::eventTest1(): ", data);
}

void LayerReg::viewEnter()
{
	g_event.regEvent(event_register, CC_CALLBACK_1(LayerReg::eventTest, this), this);
	g_event.regEvent(event_login, std::bind(&LayerReg::eventTest1, this, placeholders::_1), this);
	CCLOG("%s\n", "LayerReg::viewEnter()");
}

void LayerReg::viewExit()
{
	CCLOG("%s\n", "LayerReg::viewExit()");
}

void LayerReg::viewEnterDone()
{
	CCLOG("%s\n", "LayerReg::viewEnterDone()");
}

void LayerReg::viewExitDone()
{
	CCLOG("%s\n", "LayerReg::viewExitDone()");
}