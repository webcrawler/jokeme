
#include "SceneTest.h"

SceneTest* SceneTest::create()
{
	SceneTest* ptr = new SceneTest();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return ptr;
}

void SceneTest::sceneEnter()
{
	CCLOG("%s\n", "SceneTest::sceneEnter");
}

void SceneTest::sceneExit()
{
	CCLOG("%s\n", "SceneTest::sceneExit()");
}
