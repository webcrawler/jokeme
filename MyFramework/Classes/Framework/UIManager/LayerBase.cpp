/**
*  @brief: ui»ùÀà
*  @Created by fjut on 16-11-02
*/

#include "LayerBase.h"
#include "../CommonFunc/Game_Val_Define.h"
#include "../CommonFunc/TimeManager.h"
#include "../CommonFunc/AudioManager.h"

LayerBase::LayerBase()
{
	mRoot = nullptr;
}

LayerBase::~LayerBase()
{

}

bool LayerBase::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	addWidgetToLayer();

	auto delayAction = CallFunc::create([&](){
		updateUI();
	});
	runAction(delayAction);

	return true;
}

void LayerBase::swallowTouchs(bool bSwallowTouchs)
{
	if (!bSwallowTouchs) return; // todo
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(bSwallowTouchs);
	listener->onTouchBegan = CC_CALLBACK_2(LayerBase::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LayerBase::addWidgetToLayer(bool bAdapted)
{
	string file = getWidgetFilePath();
	if (file.size() == 0)
	{
		return;
	}
	file = "csb/" + file + ".csb";
	mRoot = static_cast<Widget*>(CSLoader::createNode(file.c_str()));
	if (bAdapted)
	{
		mRoot = static_cast<Widget*>(CSLoader::createNodeWithVisibleSize(file.c_str()));
	}
	else
	{
		mRoot = static_cast<Widget*>(CSLoader::createNode(file.c_str()));
	}
	addChild(mRoot);
}

bool LayerBase::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	return true;
}

Widget* LayerBase::getWidgetByName(const string& name)
{
	if (mRoot == nullptr)
	{
		return mRoot;
	}
	return Helper::seekWidgetByName(mRoot, name);
}

Button* LayerBase::getBtnByName(const string& name, bool bEffect)
{
	auto widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		auto btn = static_cast<Button*>(widget);
		//if (bEffect)
		//{
		//	btn->addTouchEventForReleaseUp([=](Ref* pSender) {
		//		g_audio.playEffect(effect_common_btn);
		//	});
		//}
		return btn;
	}
	return nullptr;
}

Button* LayerBase::regBtnUpCall(const string& name, const ccBtnReleaseUp& callback, bool bEffect)
{
	auto widget = getWidgetByName(name);
	return regBtnUpCall(widget, callback, bEffect);
}

Button* LayerBase::regBtnUpCall(Widget* widget, const ccBtnReleaseUp& callback, bool bEffect)
{
	auto btn = static_cast<Button*>(widget);
	if (btn != nullptr)
	{
		btn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
			if (type != Widget::TouchEventType::ENDED) return;
			if (bEffect)
			{
				g_audio.playEffect(effect_common_btn);
			}
			if (callback)
			{
				callback(btn, btn->getTag(), btn->getName());
			}
		});
	}
	return btn;
}

Button* LayerBase::regBtnCall(const string& name, const Widget::ccWidgetTouchCallback& callback, bool bEffect)
{
	auto widget = getWidgetByName(name);
	return regBtnCall(widget, callback, bEffect);
}

Button* LayerBase::regBtnCall(Widget* widget, const Widget::ccWidgetTouchCallback& callback, bool bEffect)
{
	auto btn = static_cast<Button*>(widget);
	if (btn != nullptr)
	{
#if 0 // with modify cocos2dx code
		btn->addTouchEventListener(callback);
		if (bEffect)
		{
			btn->addTouchEventForReleaseUp([=](Ref* pSender) {
				g_audio.playEffect(effect_common_btn);
			});
		}
#else
		btn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
			callback(pSender, type);
			if (type == Widget::TouchEventType::ENDED)
			{
				g_audio.playEffect(effect_common_btn);
			}
		});
#endif
	}
	return btn;
}

ImageView* LayerBase::getImgByName(const string& name)
{
	auto widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		return static_cast<ImageView*>(widget);
	}
	return nullptr;
}

Text* LayerBase::getTextByName(const string& name)
{
	auto widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		return static_cast<Text*>(widget);
	}
	return nullptr;
}

void LayerBase::addUpdateObserver()
{
	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(LayerBase::updateEveryFrame),
		scheduleKey1, nullptr);
	__NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(LayerBase::updateEveryInterval),
		scheduleKey2, nullptr);
}

void LayerBase::removeUpdateObserver()
{
	__NotificationCenter::getInstance()->removeObserver(this, scheduleKey1);
	__NotificationCenter::getInstance()->removeObserver(this, scheduleKey2);
}

void LayerBase::viewUnRegEvents()
{
	g_event.unRegEventByTarget(this);
	g_timer.unRegTimerByTarget(this);
}
