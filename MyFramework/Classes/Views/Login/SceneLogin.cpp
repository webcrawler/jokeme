
#include "SceneLogin.h"

SceneLogin* SceneLogin::create()
{
	SceneLogin* ptr = new SceneLogin();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return ptr;
}

void SceneLogin::sceneEnter()
{
	CCLOG("%s\n", "SceneLogin::sceneEnter");
}

void SceneLogin::sceneExit()
{
	CCLOG("%s\n", "SceneLogin::sceneExit()");
}
