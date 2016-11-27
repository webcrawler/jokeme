
#include "LayerLogin.h"

LayerLogin* LayerLogin::create()
{
	LayerLogin* ptr = new LayerLogin();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string LayerLogin::getWidgetFilePath()
{
	//return "layer_login";
	return "layer_test";
}

string LayerLogin::getLayerName()
{
	return "UI_LayerLogin";
}

void LayerLogin::updateUI()
{
	auto img = getImgByName("Image_1");
	//btn->addTouchEventListener(CC_CALLBACK_2(LayerLogin::callback, this));
	//btn = static_cast<Button*>(getWidgetByName("Button_2"));
	//btn->addTouchEventListener(CC_CALLBACK_2(LayerLogin::callback, this));
	//btn = static_cast<Button*>(getWidgetByName("Button_2_0"));
	//btn->addTouchEventListener(CC_CALLBACK_2(LayerLogin::callback, this));

	//addEdBox();

	/*
	auto node = getWidgetByName("Image_1");
	std::vector<Widget*> vec;
	for (int k = 0; k < 89; k++)
	{
		auto node_ = node->clone();
		vec.push_back(node_);
		auto label = static_cast<Text*>(node_->getChildByName("Text_1"));
		label->setString(StringTool::formatStr("%d", k));
	}


	auto panel = getWidgetByName("panel");
	auto scroll = UIFunc::getScrollView(panel->getSize(), vec, 3, {10, 3, 15}, CC_CALLBACK_2(LayerLogin::callback1, this));
	panel->addChild(scroll);
	*/

	/*
	auto node = getWidgetByName("Image_1");
	std::vector<Widget*> vec;
	for (int k = 0; k < 89; k++)
	{
		auto node_ = node->clone();
		vec.push_back(node_);
		auto label = static_cast<Text*>(node_->getChildByName("Text_1"));
		label->setString(StringTool::formatStr("%d", k));
	}


	auto panel = getWidgetByName("panel");
	auto scroll = UIFunc::getScrollViewHorizontal(panel->getSize(), vec, 5, { 0, 10, 10 }, CC_CALLBACK_2(LayerLogin::callback1, this));
	panel->addChild(scroll);
	*/

	auto node = getWidgetByName("Image_1");
	std::vector<Widget*> vec;
	for (int k = 0; k < 89; k++)
	{
		auto node_ = node->clone();
		vec.push_back(node_);
		auto label = static_cast<Text*>(node_->getChildByName("Text_1"));
		label->setString(StringTool::formatStr("%d", k));
	}


	auto panel = getWidgetByName("panel");
	auto scroll = UIFunc::getPageView(panel->getSize(), vec, {3, 10}, { 0, 0, 0 }, CC_CALLBACK_2(LayerLogin::callback1, this),
		CC_CALLBACK_2(LayerLogin::pageViewEvent, this));
	panel->addChild(scroll);
	


}

void LayerLogin::pageViewEvent(Ref* pSender, PageView::EventType type)
{
	switch (type)
	{
	case PageView::EventType::TURNING:
	{
		PageView* pageView = dynamic_cast<PageView*>(pSender);

		string str = (StringUtils::format("page = %ld", static_cast<long>(pageView->getCurrentPageIndex() + 1)));
		CCLOG("%s\n", str.c_str());
	}
	break;

	default:
		break;
	}
}

void LayerLogin::callback1(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	int tag = ((Button*)pSender)->getTag();
	CCLOG("tag = %d \n", tag);


}

void LayerLogin::addEdBox()
{
	auto textField1 = getWidgetByName("Image_2_1_0");
	auto textField2 = getWidgetByName("Image_2_1_1");

	ui::Scale9Sprite *s9sprite = ui::Scale9Sprite::create("MyRes/Login/ditu.png");
	m_pAccount = ui::EditBox::create(s9sprite->getBoundingBox().size, s9sprite);
	m_pAccount->setAnchorPoint(Vec2(0, 0));
	m_pAccount->setMaxLength(12);
	m_pAccount->setFontSize(36);
	m_pAccount->setDelegate(this);
	m_pAccount->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pAccount->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pAccount->setFontColor(Color4B::WHITE);
	textField1->addChild(m_pAccount);
	m_pAccount->setText("123654");

	s9sprite = ui::Scale9Sprite::create("MyRes/Login/ditu.png");
	m_pPwd = ui::EditBox::create(s9sprite->getBoundingBox().size, s9sprite);
	m_pPwd->setAnchorPoint(Vec2(0, 0));
	m_pPwd->setMaxLength(12);
	m_pPwd->setFontSize(36);
	m_pPwd->setDelegate(this);
	m_pPwd->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	m_pPwd->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pPwd->setFontColor(Color4B::WHITE);
	textField2->addChild(m_pPwd);
	m_pPwd->setText("455555");
	
}

void LayerLogin::editBoxEditingDidBegin(ui::EditBox* editBox)
{
}

void LayerLogin::editBoxReturn(ui::EditBox* editBox)
{
	int f = 0;
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

void LayerLogin::callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	string name = ((Button*)pSender)->getName();
	if (name == "Button_1")
	{
		//g_ui.pushLayer(LayerLogin1::create(), uiAction_open, false);
		//g_event.sendEvent(event_login, "666");
		//g_event.sendEvent(event_register, "555");

		//g_net.startConnection();

		//快速登录
		struct CMD_GP_FastLogon
		{
			unsigned char					byDevice;			    // 设备类型0PC，1手机
			char16_t						strMachineCode[33];		// 机器标识
		};

		//MD5 md5("");
		//u16string _md5Password;
		//StringUtils::UTF8ToUTF16(md5.hexdigest(), _md5Password);
		//CMD_GP_FastLogon fastLogon{ 0 };
		//fastLogon.byDevice = 1;
		//std::u16string u16MachineCode;
		//StringUtils::UTF8ToUTF16(getSerialNumber(), u16MachineCode);
		//u16MachineCode.copy(fastLogon.strMachineCode, u16MachineCode.length());


		//g_net.SendData(MDM_GP_LOGON, SUB_GP_LOGON_FAST, (unsigned char*)(&fastLogon), sizeof(fastLogon));

		//g_net.sendData(6, 3, NULL, NULL);

	}
	else if (name == "Button_2")
	{
		g_ui.pushLayer(LayerReg::create(), uiAction_open);
	}
	else if (name == "Button_2_0")
	{
		// 联系我们
	}
}


void LayerLogin::eventTest(void* data)
{
	CCLOG("%s, %s\n", "LayerLogin::eventTest(): ", data);
}

void LayerLogin::eventTest1(void* data)
{
	CCLOG("%s, %s\n", "LayerLogin::eventTest1(): ", data);
}

void LayerLogin::viewEnter()
{
	g_event.regEvent(event_register, CC_CALLBACK_1(LayerLogin::eventTest, this), this);
	g_event.regEvent(event_login, std::bind(&LayerLogin::eventTest1, this, placeholders::_1), this);
	CCLOG("%s\n", "LayerLogin::viewEnter()");
}

void LayerLogin::viewExit()
{
	CCLOG("%s\n", "LayerLogin::viewExit()");
}

void LayerLogin::viewEnterDone()
{
	CCLOG("%s\n", "LayerLogin::viewEnterDone()");
}

void LayerLogin::viewExitDone()
{
	CCLOG("%s\n", "LayerLogin::viewExitDone()");
}