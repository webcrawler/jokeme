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
	//void createTimer();

public:
	void startSchedule();
	void endSchedule();

	void updateEveryFrame(float dt);
	void updateEveryInterval(float dt);

};

#define g_timer TimeManager::getInstance()

#endif 

