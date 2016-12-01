/**
*  @brief: 定时器管理
*  @Created by fjut on 16-12-01
*/

#include "TimeManager.h"
#include "Game_Val_Define.h"

TimeManager::TimeManager()
{
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
	scheduler->schedule(CC_CALLBACK_1(TimeManager::updateEveryFrame, this), this, 0, false, scheduleKey1);
	scheduler->schedule(CC_CALLBACK_1(TimeManager::updateEveryInterval, this), this, 1.0f, false, scheduleKey2);
}

void TimeManager::endSchedule()
{
	Scheduler* scheduler = Director::getInstance()->getScheduler();
	scheduler->unschedule(scheduleKey1, this);
	scheduler->unschedule(scheduleKey2, this);
}

void TimeManager::updateEveryFrame(float dt)
{
	//log("updateEveryFrame, dt = %f", dt);
	__NotificationCenter::getInstance()->postNotification(scheduleKey1);
}

void TimeManager::updateEveryInterval(float dt)
{
	//log("updateEveryInterval, dt = %f", dt);
	__NotificationCenter::getInstance()->postNotification(scheduleKey2);
}