/**
*  @brief: ui����
*  @Created by fjut on 16-11-02
*/

#include "LayerBase.h"

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

Button* LayerBase::getBtnByName(const string& name)
{
	auto widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		return static_cast<Button*>(widget);
	}
	return nullptr;
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

Widget* LayerBase::regCallbackByName(const string& name, const Widget::ccWidgetTouchCallback& callback)
{
	auto widget = getWidgetByName(name);
	if (widget != nullptr)
	{
		widget->addTouchEventListener(callback);
	}
	return widget;
}

//void LayerBase::setViewEnterAndExitCallback(const viewCallback& viewEnter, const viewCallback& viewExit)
//{
//	m_viewEnter = viewEnter;
//	m_viewExit = viewExit;
//}

//void LayerBase::viewEnter()
//{
//	// ����
//	//
//
//
//	if (m_viewEnter != nullptr)
//	{
//		m_viewEnter();
//	}
//}
//

//void LayerBase::viewExit()
//{
//}

void LayerBase::viewUnRegEvents()
{
	g_event.unRegEventByTarget(this);
}