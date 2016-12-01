/**
*  @brief: 定时器管理
*  @Created by fjut on 16-12-01
*/

#include "TimeManager.h"
#include "Game_Val_Define.h"

TimeManager::TimeManager()
{
	m_timerArr.clear();
}

TimeManager& TimeManager::getInstance()
{
	static TimeManager _instance;
	return _instance;
}

void TimeManager::destroyInstance()
{

}

void TimeManager::startSchedule()
{
	Scheduler* scheduler = Director::getInstance()->getScheduler();

	scheduler->schedule([=](float dt) {
		__NotificationCenter::getInstance()->postNotification(scheduleKey1);
	}, this, 0, false, scheduleKey1);

	scheduler->schedule([=](float dt) {
		__NotificationCenter::getInstance()->postNotification(scheduleKey2);
		updateEveryInterval(dt);
	}, this, 1.0f, false, scheduleKey2);
}

void TimeManager::endSchedule()
{
	Scheduler* scheduler = Director::getInstance()->getScheduler();
	scheduler->unschedule(scheduleKey1, this);
	scheduler->unschedule(scheduleKey2, this);
}

void TimeManager::updateEveryInterval(float dt)
{
	for (unsigned int k = 0; k < m_timerArr.size(); k++)
	{
		if (m_timerArr[k].interval < m_timerArr[k].total)
		{
			m_timerArr[k].interval += 1;
			if (m_timerArr[k].interval < m_timerArr[k].total)
			{
				if (m_timerArr[k].everyIntervalCall)
				{
					m_timerArr[k].everyIntervalCall(m_timerArr[k].interval);
				}
			}
			else
			{
				if (m_timerArr[k].endCall)
				{
					m_timerArr[k].endCall(m_timerArr[k].interval);
				}
			}
		}
	}

	for (int k = m_timerArr.size() - 1; k >= 0; k--)
	{
		if (m_timerArr[k].interval >= m_timerArr[k].total)
		{
			m_timerArr.erase(m_timerArr.begin() + k);
		}
	}
}

void TimeManager::regTimer(unsigned int interval, const TimerCallback& everyIntervalCall, const TimerCallback& endCall,
	void* target, const std::string& key)
{
	for (unsigned int k = 0; k < m_timerArr.size(); k++)
	{
		if (m_timerArr[k].key == key && m_timerArr[k].target == target)
		{
			CCLOG("regTimer fail, the timer has reg by the name = %s", key.c_str());
			return;
		}
	}
	Timer timer;
	timer.key = key;
	timer.target = target;
	timer.interval = 0;
	timer.total = interval;
	timer.everyIntervalCall = everyIntervalCall;
	timer.endCall = endCall; 
	m_timerArr.push_back(timer);
}

void TimeManager::unRegTimer(void* target, const std::string& key)
{
	for (unsigned int k = 0; k < m_timerArr.size(); k++)
	{
		if (m_timerArr[k].key == key && m_timerArr[k].target == target)
		{
			m_timerArr.erase(m_timerArr.begin() + k);
			break;
		}
	}
}

void TimeManager::unRegTimerByTarget(void* target)
{
	for (int k = m_timerArr.size() - 1; k >= 0; k--)
	{
		if (m_timerArr[k].target == target)
		{
			m_timerArr.erase(m_timerArr.begin() + k);
		}
	}
}