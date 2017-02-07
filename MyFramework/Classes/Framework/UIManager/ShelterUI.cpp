/**
*  @brief: ui屏蔽层
*  @Created by fjut on 16-11-04
*/

#include "ShelterUI.h"

bool ShelterUI::init()
{
	if (!Layer::init()) 
	{
		return false;
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* pTouch, Event* pEvent)
	{
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
