/**
*  @brief: ui管理类
*  @Created by fjut on 16-11-03
*/

#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "cocos2d.h"
#include "UIValDefine.h"
#include "LayerBase.h"
#include "SceneBase.h"

using namespace cocos2d;
using namespace std;

struct SceneInfo
{
public:
	SceneInfo() 
	{
		scene = nullptr;
	};

	SceneInfo(Node* scene, vector<Node*> layer)
	{
		scene = scene;
		layer = layer;
	}

	Node* scene;
	vector<Node*> layer;
};

class UIManager
{
public:
	static UIManager& getInstance();
	static void destroyInstance();

public:
	Node* getRunningScene();
	void closeAllScenes();
	void runScene(Node* scene);
	void runLayer(Node* layer, UIActionType actionTag = unknowAction, bool bSwallowTouchs = true);
	void pushLayer(Node* layer, UIActionType actionTag = unknowAction, bool bSwallowTouchs = true);
	void closeLayer(string name, UIActionType actionTag = unknowAction);
	void closeLayer(Node* obj, UIActionType actionTag = unknowAction);

private:
	SceneInfo& getCurScene();
	Node* getLayerByName(string name);
	Node* getLayerByObj(Node* obj);
	void closeScene();
	void closeLayer();
	void closeAllLayers();

private:
	void showShelterUI();
	void closeShelterUI();
	void removeLayerFromVec(string name);
	int getMaxZOrderValByUIType(string tag);
	void pushLayer_(Node* layer, UIActionType actionTag, int zOrder, bool bSwallowTouchs);
	Action* getUIActionByType(Node* node, UIActionType tag);

public:
	void showLoadingUI();
	void closeLoadingUI();

private:
	vector<SceneInfo> m_vecScene;
	typedef std::function<Action*()> actionCallback;
	typedef function<void()> removeNodeCallback;
};

#define g_ui UIManager::getInstance()

#endif 
