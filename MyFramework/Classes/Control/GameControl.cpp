/**
*  @brief: Game control
*  @Created by fjut on 16-11-19
*/
#include "GameControl.h"

#define TEST 1

GameControl& GameControl::getInstance()
{
	static GameControl instance_;
	return instance_;
}

void GameControl::destroyInstance()
{
}

GameControl::GameControl()
{
	FileUtils::getInstance()->addSearchPath("res");

}

void GameControl::startLoginScene()
{
#if 0
	auto scene = Director::getInstance()->getRunningScene();
	if (scene == nullptr)
	{
		g_ui.runScene(SceneTest::create());
		g_ui.runLayer(LayerTest::create(), uiAction_open);
	}
#else
	auto scene = Director::getInstance()->getRunningScene();
	// start login game
	if (scene == nullptr)
	{
		g_ui.runScene(SceneLogin::create());
		g_ui.runLayer(LayerLogin::create(), uiAction_open);
	}
	else
	{
		scene = Scene::create();
		Director::getInstance()->replaceScene(scene);

		auto action = CallFunc::create([=]() {
			g_ui.runScene(SceneLogin::create());
			g_ui.runLayer(LayerLogin::create(), uiAction_open);
		});
		scene->runAction(action);
	}
#endif
}

void GameControl::startMenuScene()
{

}

void GameControl::startGameScene()
{
	// clean hall res
	// todo

	// close hall scene
	g_ui.closeAllScenes();

	// next frame invoke
	auto scene = g_ui.getRunningScene();
	auto action = CallFunc::create([=]()
	{
		// enter game 
		// todo
	});
	scene->runAction(action);
}