#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "../../Define_.h"

class SceneGame : public SceneBase
{
public:
	static SceneGame* create();
	virtual void sceneEnter();
	virtual void sceneExit();
};

#endif 
