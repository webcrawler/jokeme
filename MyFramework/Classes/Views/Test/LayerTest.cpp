
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
	regCallbackByName("Button_1", CC_CALLBACK_2(LayerTest::callback, this));
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

	CommonCfg& cfg = g_file.getCommonCfg();
	cfg.bEffect = true;
	g_file.modifyCommonCfg(cfg);
	cfg = g_file.getCommonCfg();


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


}

void LayerTest::callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	g_ui.closeLayer(this);

	//g_timer.unRegTimer(this, "kj");

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