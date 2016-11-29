/**
*  @brief: Game control
*  @Created by fjut on 16-11-19
*/

#ifndef __UI_GAME_CONTROL_H__
#define __UI_GAME_CONTROL_H__

#include "../define_.h"

class GameControl
{
public:
	GameControl();
	static GameControl& getInstance();
	static void destroyInstance();

public:
	void startLoginScene();
	void startMenuScene();
	void startGameScene();

};

#define g_ctrl GameControl::getInstance()

#endif 
