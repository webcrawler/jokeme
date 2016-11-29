#ifndef __UI_EVENT_DISPATHER_H__
#define __UI_EVENT_DISPATHER_H__

/**
*  @brief: ui事件管理
*  @Created by fjut on 16-11-04
*/

#include "cocos2d.h"
#include "UIEventDefine.h"

using namespace std;

class UIEventDispather
{
public:
	static UIEventDispather& getInstance();
	static void destroyInstance();

public:
	typedef function<void(void*)> Callback;
	struct EventInfo
	{
		Callback callback;
		cocos2d::Ref* obj;
	};
	typedef vector<EventInfo> EventVec;

public:
	void regEvent(UIEventTag eventTag, const Callback& callback, cocos2d::Ref* obj);
	void sendEvent(UIEventTag eventTag, void* data = nullptr);
	void delaySendEvent(UIEventTag eventTag, void* data = nullptr);
	void unRegEvent(UIEventTag eventTag);
	void unRegEventByTarget(cocos2d::Ref* obj);

private:
	map<UIEventTag, EventVec> m_mapEvent;

};

#define g_event UIEventDispather::getInstance()

#endif 
