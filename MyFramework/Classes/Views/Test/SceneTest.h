#ifndef __SCENE_TEST_H__
#define __SCENE_TEST_H__

#include "../../Define_.h"

class SceneTest : public SceneBase
{
public:
	static SceneTest* create();

	virtual void sceneEnter();
	virtual void sceneExit();

};

#endif 
