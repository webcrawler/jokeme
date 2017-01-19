
#include "SceneGame.h"

SceneGame* SceneGame::create()
{
	SceneGame* ptr = new SceneGame();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return ptr;
}

void SceneGame::sceneEnter()
{
	CCLOG("%s\n", "SceneGame::sceneEnter");
}

void SceneGame::sceneExit()
{
	CCLOG("%s\n", "SceneGame::sceneExit()");
}

