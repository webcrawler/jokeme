/**
*  @brief: 定时器管理
*  @Created by fjut on 16-12-01
*/

#ifndef _H_TIME_MANAGER_H_
#define _H_TIME_MANAGER_H_

#include "cocos2d.h"

using namespace cocos2d;

class TimeManager
{
public:
	TimeManager();
	static TimeManager& getInstance();
	static void destroyInstance();

public:
	void startSchedule();
	void endSchedule();

public:
	typedef std::function<void(unsigned int)> TimerCallback;
	//static TimerCallback Null;
	struct Timer
	{
		std::string key;
		void* target;
		unsigned int interval;
		unsigned int total;
		TimerCallback everyIntervalCall;
		TimerCallback endCall;
	};
	typedef std::vector<Timer> TimerArr;
	void regTimer(unsigned int interval, const TimerCallback& everyIntervalCall, const TimerCallback& endCall, 
		void* target, const std::string& key);
	void unRegTimer(void* target, const std::string& key);
	void unRegTimerByTarget(void* target);

private:
	void updateEveryInterval(float dt);

private:
	TimerArr m_timerArr;

};

#define g_timer TimeManager::getInstance()

#endif 

