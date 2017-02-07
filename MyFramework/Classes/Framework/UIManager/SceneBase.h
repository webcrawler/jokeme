/**
*  @brief: ui场景基类
*  @Created by fjut on 16-11-02
*/

#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class SceneBase: public Layer
{
protected:
	virtual bool init();

	//typedef function<void()> scneCallback;
	//virtual void setSceneEnterAndExitCallback(const scneCallback& sceneEnter = nullptr, const scneCallback& sceneExit = nullptr);

public:
	virtual void sceneEnter() = 0;
	virtual void sceneExit() = 0;

private:
	//scneCallback m_sceneEnter, m_sceneExit;
};

#endif 
