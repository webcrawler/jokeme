/**
*  @brief: ui¼ÓÔØÆÁ±Î²ã
*  @Created by fjut on 16-11-04
*/
#include "LoadingUI.h"

bool LoadingUI::init()
{
	if (!Layer::init()) 
	{
		return false;
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); CC_CALLBACK_2(LoadingUI::onTouchBegan, this);
	listener->onTouchBegan = [=](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	showUI();
	return true;
}

void LoadingUI::showUI()
{

}
