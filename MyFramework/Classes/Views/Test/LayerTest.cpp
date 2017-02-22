
#include "LayerTest.h"

LayerTest* LayerTest::create()
{
	LayerTest* ptr = new LayerTest();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string LayerTest::getWidgetFilePath()
{
	return "layer_test";
}

string LayerTest::getLayerName()
{
	return "UI_LayerTest";
}

void LayerTest::updateUI()
{
	//ValueMap plist = FileUtils::getInstance()->getValueMapFromFile("MyRes/android9patch.plist");
	//string str = plist["res_ver"].asString();

	auto img = getImgByName("Image_1");
	auto btn = getBtnByName("Button_1");
	//regBtnUpCall("Button_1", CC_CALLBACK_3(LayerTest::callback2, this));
	//regBtnUpCall(btn, CC_CALLBACK_3(LayerTest::callback2, this));
	regBtnCall("Button_1", CC_CALLBACK_2(LayerTest::callback1, this));

	//btn->addTouchEventListener(CC_CALLBACK_2(LayerTest::callback, this));

	//auto btn->addTouchEventListener(CC_CALLBACK_2(LayerTest::callback, this));
	//auto btn = static_cast<Button*>(getWidgetByName("Button_2"));
	//btn->addTouchEventListener(CC_CALLBACK_2(LayerTest::callback, this));
	//btn = static_cast<Button*>(getWidgetByName("Button_2_0"));
	//btn->addTouchEventListener(CC_CALLBACK_2(LayerTest::callback, this));

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
	//auto scroll = g_uifunc.getScrollView(panel->getSize(), vec, 3, {10, 3, 15}, CC_CALLBACK_2(LayerTest::callback1, this));
	auto scroll = g_uifunc.getScrollView(panel->getSize(), vec, 1, { 10, 3, 15 }, CC_CALLBACK_2(LayerTest::callback1, this));
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
	//auto scroll = g_uifunc.getScrollViewHorizontal(panel->getSize(), vec, 5, { 0, 10, 10 }, CC_CALLBACK_2(LayerTest::callback1, this));
	auto scroll = g_uifunc.getScrollViewHorizontal(panel->getSize(), vec, 1, { 0, 10, 10 }, CC_CALLBACK_2(LayerTest::callback1, this));
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
	auto scroll = g_uifunc.getPageView(panel->getSize(), vec, {1, 1}, { 0, 0, 0 }, CC_CALLBACK_2(LayerTest::callback1, this),
		CC_CALLBACK_2(LayerTest::pageViewEvent, this));
	panel->addChild(scroll);
	*/

	//g_file.deleteFile();

	string str = g_local.getStr("res_ver");
	bool b = g_local.getBool("lua_debug");

	//g_file.deleteAllUsers();
	auto allUser = g_file.getAllLocalUsers();
	auto user = g_file.getLocalUserByName("name1");

	UserInf inf;
	inf.name = "name1";
	inf.pwd = "pwd1";
	g_file.addLocalUser(inf);

	allUser = g_file.getAllLocalUsers();
	user = g_file.getLocalUserByName("name1");

	inf.name = "name2";
	inf.pwd = "pwd2";
	g_file.addLocalUser(inf);
	allUser = g_file.getAllLocalUsers();
	g_file.deleteUserByName("name1");

	g_file.addLocalUser(inf);
	allUser = g_file.getAllLocalUsers();
	user = g_file.getLocalUserByName("name1");

	g_file.deleteAllUsers();
	allUser = g_file.getAllLocalUsers();
	user = g_file.getLocalUserByName("name1");

	g_timer.startSchedule();

	g_timer.regTimer(5, CC_CALLBACK_1(LayerTest::timercall1, this), CC_CALLBACK_1(LayerTest::timercall2, this), this, "kj");
	g_timer.regTimer(5, nullptr, nullptr, this, "kj0");\

	CommonCfg cfg = g_file.getCommonCfg();
	cfg.bEffect = true;
	g_file.modifyCommonCfg(cfg);
	cfg = g_file.getCommonCfg();

	DownloaderManager::DownloaderParam param;
	param.srcUrl = "http://www.kuu789.com/downloads/game.apk"; //"http://www.cocos2d-x.org/attachments/802/cocos2dx_landscape.png";
	param.storagePath = FileUtils::getInstance()->getWritablePath() + "DownloaderManager/joke.apk"; //"DownloaderManager/123.png";
	//param.identifier = ""
	param.onDataTaskSuccess = [=](const DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		CCLOG("onDataTaskSuccess++++: %s", task.requestURL.c_str());
	};
	param.onFileTaskSuccess = [=](const DownloadTask& task)
	{
		CCLOG("onFileTaskSuccess++++: %s", task.requestURL.c_str());
	};
	auto label = Text::create("", "Aril", 35);
	auto size = Director::getInstance()->getVisibleSize();
	label->setPosition(Vec2(size.width, size.height)*0.5f);
	addChild(label);
	param.onTaskProgress = [=](const DownloadTask& task,
		int64_t bytesReceived,
		int64_t totalBytesReceived,
		int64_t totalBytesExpected)
	{
		CCLOG("onTaskProgress %.2f", totalBytesReceived*100.0f/ totalBytesExpected);
		string sstr = StringUtils::format("%.2f", totalBytesReceived*100.0f / totalBytesExpected);
		label->setText(sstr);
	};
	param.onTaskError = [=](const DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		CCLOG("onTaskError++++: %s", task.requestURL.c_str());
	};
	//g_downloader.createDownloaderFileTask(param);
	//string delFile = FileUtils::getInstance()->getWritablePath() + "joke.apk";
	//FileUtils::getInstance()->removeFile(delFile);
	//string delFile1 = FileUtils::getInstance()->getWritablePath() + "DownloaderManager/";
	//FileUtils::getInstance()->removeDirectory(delFile1);

	// socket io test
	g_socketio.testSocketOpen();
	// test for git branch

	//const char* ver_addr = "http://14.18.141.29/File/Resources.aspx?TypeID=1";
	//const char* game_addr = "http://14.18.141.29/downloads/AllHappy.zip";
	//const char* storagePath = "res/game";
	//auto assertMgr = g_assert.create(game_addr, ver_addr, storagePath,
	//	[=](int errorcode) {
	//	CCLOG("errorcode = %d", errorcode);
	//},
	//	[=](int per) {
	//	CCLOG("per = %d", per);
	//},
	//	[=]() {
	//	CCLOG("success update");
	//});
	//assertMgr->checkUpdate();
	//assertMgr->update();

}

void LayerTest::timercall1(float dt)
{
	CCLOG("hhhh LayerTest  = %f", dt);
}

void LayerTest::timercall2(float dt)
{
	CCLOG("hhhh LayerTest end  = %f", dt);
}

void LayerTest::updateEveryFrame(Ref* obj)
{
	//CCLOG("%s", "hhhh LayerTest");
}

void LayerTest::updateEveryInterval(Ref* obj)
{
	// test heartbeat
	g_socketio.testSocketEmit();
}

void LayerTest::pageViewEvent(Ref* pSender, PageView::EventType type)
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

void LayerTest::callback1(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	int tag = ((Button*)pSender)->getTag();
	CCLOG("tag = %d \n", tag);

	//g_socketio.testSocketSend();
	//g_socketio.disconnect1();

	//string delFile = FileUtils::getInstance()->getWritablePath() + "joke.apk";
	//FileUtils::getInstance()->removeFile(delFile);
	//string delFile1 = FileUtils::getInstance()->getWritablePath() + "DownloaderManager/";
	//FileUtils::getInstance()->removeDirectory(delFile1);
}

void LayerTest::callback2(Button* btn, int tag, const std::string& name)
{
	CCLOG("tag = %d", tag);
	CCLOG("name = %s", name.c_str());
}

void LayerTest::callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	//g_ui.closeLayer(this);

	//g_timer.unRegTimer(this, "kj");

	//g_audio.setAudioCfgWithMusic(true);
	//g_audio.playMusic(music_bg_1);

	//g_audio.setAudioCfgWithEffect(true);
	//g_audio.playEffect(effect_common_btn);

	string name = ((Button*)pSender)->getName();
	if (name == "Button_1")
	{
		CCLOG("66666");
		//g_ui.pushLayer(LayerTest1::create(), uiAction_open, false);
		//g_event.sendEvent(event_login, "666");
		//g_event.sendEvent(event_register, "555");
	}
	else if (name == "Button_2")
	{
		//g_ui.pushLayer(LayerReg::create(), uiAction_open);
		g_ui.closeLayer(this);
	}
	else if (name == "Button_2_0")
	{
		// 联系我们
	}
}


void LayerTest::eventTest(void* data)
{
	CCLOG("%s, %s\n", "LayerTest::eventTest(): ", data);
}

void LayerTest::eventTest1(void* data)
{
	CCLOG("%s, %s\n", "LayerTest::eventTest1(): ", data);
}

void LayerTest::viewEnter()
{
	g_event.regEvent(event_register, CC_CALLBACK_1(LayerTest::eventTest, this), this);
	g_event.regEvent(event_login, std::bind(&LayerTest::eventTest1, this, placeholders::_1), this);
	CCLOG("%s\n", "LayerTest::viewEnter()");
}

void LayerTest::viewExit()
{
	CCLOG("%s\n", "LayerTest::viewExit()");
}

void LayerTest::viewEnterDone()
{
	CCLOG("%s\n", "LayerTest::viewEnterDone()");
}

void LayerTest::viewExitDone()
{
	CCLOG("%s\n", "LayerTest::viewExitDone()");
}