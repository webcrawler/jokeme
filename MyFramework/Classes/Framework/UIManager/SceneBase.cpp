/**
*  @brief: ui场景基类
*  @Created by fjut on 16-11-02
*/

#include "SceneBase.h"
//
//void SceneBase::setSceneEnterAndExitCallback(const scneCallback& sceneEnter, const scneCallback& sceneExit)
//{
//	m_sceneEnter = sceneEnter;
//	m_sceneExit = sceneExit;
//}

bool SceneBase::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

//void SceneBase::sceneEnter()
//{
//	if (m_sceneEnter != nullptr)
//	{
//		m_sceneEnter();
//	}
//}
//
//void SceneBase::sceneExit()
//{
//	if (m_sceneExit != nullptr)
//	{
//		m_sceneExit();
//	}
//}
