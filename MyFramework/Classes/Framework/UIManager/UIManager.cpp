/**
*  @brief: ui管理类
*  @Created by fjut on 16-11-03
*/

#include "UIManager.h"
#include "ShelterUI.h"
#include "SceneBase.h"
#include "LayerBase.h"
#include "LoadingUI.h"

UIManager& UIManager::getInstance()
{
	static UIManager instance_;
	return instance_;
}

void UIManager::destroyInstance()
{
	//UIManager::getInstance().closeAllScenes();
}

static const string sheltername = "UI_SHELTER";
void UIManager::showShelterUI()
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "showShelterUI fail, running scene is nil");
		return;
	}
	auto shelterUI = sceneInfo.scene->getChildByName(sheltername);
	if (shelterUI == nullptr)
	{
		shelterUI = ShelterUI::create();
		shelterUI->setName(sheltername);
		shelterUI->setLocalZOrder(shelterZOrder);
		sceneInfo.scene->addChild(shelterUI);
	}
	else
	{
		//shelterUI->setVisible(true);
		//((Layer*)shelterUI)->setTouchEnabled(true);
	}
}

void UIManager::closeShelterUI()
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeShelterUI fail, running scene is nil");
		return;
	}
	auto shelterUI = sceneInfo.scene->getChildByName(sheltername);
	if (shelterUI != nullptr)
	{
		shelterUI->removeFromParentAndCleanup(true);
		//shelterUI->setVisible(false);
		//((Layer*)shelterUI)->setTouchEnabled(false);
	}
}

Node* UIManager::getRunningScene()
{
	auto scene = Director::getInstance()->getRunningScene();
	if (scene == nullptr)
	{
		scene = Scene::create();
		Director::getInstance()->runWithScene(scene);
	}
	return scene;
}

void UIManager::runScene(Node* scene)
{
	if (scene == nullptr)
	{
		CCLOG("%s\n", "runScene fail, scene is nil");
		return;
	}
	closeAllScenes();
	getRunningScene()->addChild(scene);
	((SceneBase*)scene)->sceneEnter();

	SceneInfo sceneInfo;
	sceneInfo.scene = scene;
	m_vecScene.push_back(sceneInfo);
}

// 获取当前场景信息
SceneInfo& UIManager::getCurScene()
{
	if (m_vecScene.size() == 0)
	{
		static SceneInfo scenInfo;
		return scenInfo;
	}
	return m_vecScene.back();
}

// 关闭所有场景
void UIManager::closeAllScenes()
{
	for (unsigned int k = 0; k < m_vecScene.size(); k++)
	{
		closeScene();
	}
}

// 关闭当前场景
void UIManager::closeScene()
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeScene fail, running scene is nil");
		return;
	}
	closeAllLayers();
	((SceneBase*)sceneInfo.scene)->sceneExit();
	m_vecScene.pop_back();
	sceneInfo.scene->removeFromParentAndCleanup(true);
	sceneInfo.scene = nullptr;
}

void UIManager::runLayer(Node* layer, UIActionType actionTag, bool bSwallowTouchs)
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "runLayer fail, running scene is nil");
		return;
	}

	closeAllLayers();
	((LayerBase*)layer)->swallowTouchs(bSwallowTouchs);
	layer->setLocalZOrder(uiZOrder);
	sceneInfo.scene->addChild(layer);
	((LayerBase*)layer)->viewEnter();
	((LayerBase*)layer)->addUpdateObserver();
	sceneInfo.layer.push_back(layer);

	if (actionTag != unknowAction)
	{
		showShelterUI();
		auto action1 = (FiniteTimeAction*)getUIActionByType(layer, actionTag);
		auto action2 = CallFunc::create([=]() {
			((LayerBase*)layer)->viewEnterDone();
			closeShelterUI();
		});
		layer->runAction(Sequence::create(action1, action2, nullptr));
	}
}

void UIManager::closeAllLayers()
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeAllLayers fail, running scene is nil");
		return;
	}
	if (sceneInfo.layer.size() == 0)
	{
		CCLOG("%s\n", "closeAllLayers fail, running layers is nil");
		return;
	}
	unsigned int n = sceneInfo.layer.size();
	while (n-- > 0)
	{
		closeLayer();
	}

	auto shelterUI = sceneInfo.scene->getChildByName(sheltername);
	if (shelterUI != nullptr)
	{
		shelterUI->removeFromParentAndCleanup(true);
	}
}

void UIManager::closeLayer()
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeLayer fail, running scene is nil");
		return;
	}

	if (sceneInfo.layer.size() == 0)
	{
		CCLOG("%s\n", "closeLayer fail, cur layer is nil");
		return;
	}
	Node* layer = sceneInfo.layer.back();
	((LayerBase*)layer)->viewUnRegEvents();
	((LayerBase*)layer)->removeUpdateObserver();
	((LayerBase*)layer)->viewExit();
	layer->removeFromParentAndCleanup(true);
	layer = nullptr;

	sceneInfo.layer.pop_back();
}

Node* UIManager::getLayerByName(string name)
{
	SceneInfo& sceneInfo = getCurScene();
	for (unsigned int k = sceneInfo.layer.size() - 1; k >= 0 ; k--)
	{
		Node* layer = sceneInfo.layer.at(k);
		string name_ = ((LayerBase*)layer)->getLayerName();
		if (name_ == name)
		{
			return layer;
		}
	}
	return nullptr;
}

Node* UIManager::getLayerByObj(Node* obj)
{
	SceneInfo& sceneInfo = getCurScene();
	for (unsigned int k = sceneInfo.layer.size() - 1; k >= 0; k--)
	{
		Node* layer = sceneInfo.layer.at(k);
		if (layer == obj)
		{
			return layer;
		}
	}
	return nullptr;
}

void UIManager::removeLayerFromVec(string name)
{
	SceneInfo& sceneInfo = getCurScene();
	unsigned int idx = -1;
	for (unsigned int k = sceneInfo.layer.size() - 1; k >= 0; k--)
	{
		Node* layer = sceneInfo.layer.at(k);
		string name_ = ((LayerBase*)layer)->getLayerName();
		if (name_ == name)
		{
			idx = k;
			break;
		}
	}
	if (idx != -1)
	{
		sceneInfo.layer.erase(sceneInfo.layer.begin() + idx);
	}
	else
	{
		CCLOG("%s\n", "removeLayerFromVec fail, can not find the layer");
	}
}

void UIManager::closeLayer(string name, UIActionType actionTag)
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeLayerByName fail, running scene is nil");
		return;
	}
	// 顺序不可变
	Node* layer = getLayerByName(name);
	((LayerBase*)layer)->viewUnRegEvents();
	((LayerBase*)layer)->removeUpdateObserver();
	((LayerBase*)layer)->viewExit();

	removeNodeCallback callback = [=] {
		removeLayerFromVec(name);
		layer->removeFromParentAndCleanup(true);
		//layer = nullptr;
	};
	if (actionTag != unknowAction)
	{
		showShelterUI();
		auto action1 = (FiniteTimeAction*)getUIActionByType(layer, actionTag);
		auto action2 = CallFunc::create([=]() {
			((LayerBase*)layer)->viewEnterDone();
			callback();
			closeShelterUI();
		});
		layer->runAction(Sequence::create(action1, action2, nullptr));
	}
	else
	{
		callback();
	}
}

void UIManager::closeLayer(Node* obj, UIActionType actionTag)
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "closeLayerByLayer fail, running scene is nil");
		return;
	}
	// 顺序不可变
	Node* layer = getLayerByObj(obj);
	string name = ((LayerBase*)layer)->getLayerName();
	((LayerBase*)layer)->viewUnRegEvents();
	((LayerBase*)layer)->removeUpdateObserver();
	((LayerBase*)layer)->viewExit();

	removeNodeCallback callback = [=] {
		removeLayerFromVec(name);
		layer->removeFromParentAndCleanup(true);
		//layer = nullptr;
	};
	if (actionTag != unknowAction)
	{
		showShelterUI();
		auto action1 = (FiniteTimeAction*)getUIActionByType(layer, actionTag);
		auto action2 = CallFunc::create([=]() {
			((LayerBase*)layer)->viewEnterDone();
			callback();
			closeShelterUI();
		});
		layer->runAction(Sequence::create(action1, action2, nullptr));
	}
	else
	{
		callback();
	}
}

int UIManager::getMaxZOrderValByUIType(string tag)
{
	int offsetZOrder = 2;
	int zOrder = 0;
	SceneInfo& sceneInfo = getCurScene();
	for (unsigned int k = 0; k < sceneInfo.layer.size(); k++)
	{
		Node* layer = sceneInfo.layer.at(k);
		string name = ((LayerBase*)layer)->getLayerName();
		string str = name.substr(0, 1);
		if (str == tag)
		{
			int zOrder_ = layer->getLocalZOrder();
			if (zOrder_ > zOrder)
			{
				zOrder = zOrder_;
			}
		}
	}
	return zOrder;
}

void UIManager::pushLayer(Node* layer, UIActionType actionTag, bool bSwallowTouchs)
{
	string name = ((LayerBase*)layer)->getLayerName();
	if (name.size() == 0)
	{
		CCLOG("%s\n", "pushLayer fail, getLayerName can't return nil");
		return;
	}
	string str = name.substr(0, 1);
	int zOrder = getMaxZOrderValByUIType(str);
	if (str == "T")
	{
		zOrder = (zOrder == 0) ? tipsZOrder : zOrder;
	}
	else if (str == "M")
	{
		zOrder = (zOrder == 0) ? menuZOrder : zOrder;
	}
	else if (str == "U")
	{
		zOrder = (zOrder == 0) ? uiZOrder : zOrder;
	}
	else
	{
		CCLOG("%s\n", "pushLayer fail, getLayerName should return the string like: Tips_xx, Menu_xx, UI_xx");
		return;
	}
	pushLayer_(layer, actionTag, zOrder, bSwallowTouchs);
}

void UIManager::pushLayer_(Node* layer, UIActionType actionTag, int zOrder, bool bSwallowTouchs)
{
	SceneInfo& sceneInfo = getCurScene();
	if (sceneInfo.scene == nullptr)
	{
		CCLOG("%s\n", "pushLayer fail, running scene is nil");
		return;
	}
	((LayerBase*)layer)->swallowTouchs(bSwallowTouchs);
	layer->setLocalZOrder(zOrder);
	sceneInfo.scene->addChild(layer);
	((LayerBase*)layer)->viewEnter();
	((LayerBase*)layer)->addUpdateObserver();
	sceneInfo.layer.push_back(layer);

	if (actionTag != unknowAction)
	{
		showShelterUI();
		auto action1 = (FiniteTimeAction*)getUIActionByType(layer, actionTag);
		auto action2 = CallFunc::create([=]() {
			((LayerBase*)layer)->viewEnterDone();
			closeShelterUI();
		});
		layer->runAction(Sequence::create(action1, action2, nullptr));
	}
}

Action* UIManager::getUIActionByType(Node* node, UIActionType tag)
{
	actionCallback action1 = [=]()
	{
		auto action1 = CallFunc::create([=]() {
			node->setScale(0);
		});
		auto action2 = EaseBackOut::create(ScaleTo::create(0.2f, 1));
		auto action = Sequence::create(action1, action2, nullptr);
		return action;
	};

	actionCallback action2 = [=]()
	{
		auto action1 = CallFunc::create([=]() {
			node->setScale(1);
		});
		auto action2 = EaseSineIn::create(ScaleTo::create(0.1f, 0));
		auto action = Sequence::create(action1, action2, nullptr);
		return action;
	};
	vector<actionCallback> vecAction = { action1, action2 };
	if (tag < 0 || tag > (int)vecAction.size())
	{
		return nullptr;
	}
	return vecAction.at(tag)();
}

static const char* loadingName = "loadingName";
void UIManager::showLoadingUI()
{
	auto scene = getRunningScene();
	if (scene != nullptr)
	{
		auto ly = LoadingUI::create();
		ly->setName(loadingName);
		scene->addChild(ly, 100);
	}
}

void UIManager::closeLoadingUI()
{
	auto scene = getRunningScene();
	if (scene != nullptr)
	{
		auto ly = scene->getChildByName(loadingName);
		if (ly != nullptr)
		{
			ly->removeFromParentAndCleanup(true);
		}
	}
}

