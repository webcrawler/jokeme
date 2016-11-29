/**
*  @brief: ui事件管理
*  @Created by fjut on 16-11-04
*/

#include "UIEventDispather.h"

UIEventDispather& UIEventDispather::getInstance()
{
	static UIEventDispather instance_;
	return instance_;
}

void UIEventDispather::destroyInstance()
{
}

void UIEventDispather::regEvent(UIEventTag eventTag, const Callback& callback, cocos2d::Ref* obj)
{
	auto it = m_mapEvent.find(eventTag);
	if (it != m_mapEvent.end())
	{
		EventInfo info;
		info.callback = callback;
		info.obj = obj;
		it->second.push_back(info);
	}
	else
	{
		EventVec eventVec;
		EventInfo info;
		info.callback = callback;
		info.obj = obj;
		eventVec.push_back(info);
		m_mapEvent.insert(pair<UIEventTag, EventVec>(eventTag, eventVec));
	}
}

void UIEventDispather::sendEvent(UIEventTag eventTag, void* data)
{
	auto it = m_mapEvent.find(eventTag);
	if (it != m_mapEvent.end())
	{
		for (unsigned int k = 0; k < it->second.size(); k++)
		{
			if (it->second.at(k).callback)
			{
				it->second.at(k).callback(data);
			}
		}
	}
	else
	{
		CCLOG("%s%d", "sendEvent cann't find the callback with the eventTag:", eventTag);
	}
}

void UIEventDispather::delaySendEvent(UIEventTag eventTag, void* data)
{
	auto scene = cocos2d::Director::getInstance()->getRunningScene();
	if (scene == nullptr)
	{
		auto action = cocos2d::CallFunc::create([=]() {
			sendEvent(eventTag, data);
		});
		scene->runAction(action);
	}
}

void UIEventDispather::unRegEvent(UIEventTag eventTag)
{
	auto it = m_mapEvent.find(eventTag);
	if (it != m_mapEvent.end())
	{
		it->second.clear();
	}
	else
	{
		CCLOG("%s%d", "unRegEvent cann't find the callback with the eventTag:", eventTag);
	}
}

void UIEventDispather::unRegEventByTarget(cocos2d::Ref* obj)
{
	for (auto& it: m_mapEvent)
	{
		for (auto iter = it.second.begin(); iter != it.second.end();)
		{
			if (iter->obj == obj)
			{
				iter = it.second.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}
