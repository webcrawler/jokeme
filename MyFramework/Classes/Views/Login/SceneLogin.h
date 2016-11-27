#ifndef __SCENE_LOGIN_H__
#define __SCENE_LOGIN_H__

#include "../../Define_.h"

class SceneLogin : public SceneBase
{
public:
	static SceneLogin* create();

	virtual void sceneEnter();
	virtual void sceneExit();

};

#endif 
