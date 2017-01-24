
#include "Layer3DTest.h"

Layer3DTest* Layer3DTest::create()
{
	Layer3DTest* ptr = new Layer3DTest();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string Layer3DTest::getWidgetFilePath()
{
	return "layer_test";
}

string Layer3DTest::getLayerName()
{
	return "UI_Layer3DTest";
}

void Layer3DTest::updateUI()
{
	mRoot->setVisible(false);

	//auto orc = Sprite3D::create("Sprite3DTest/orc.c3b");
	//orc->setScale(5);
	//orc->setNormalizedPosition(Vec2(.5f, .3f));
	//orc->setPositionZ(40);
	//orc->setRotation3D(Vec3(0, 180, 0));
	//orc->setGlobalZOrder(-1);
	//addChild(orc);

	//auto ship = Sprite3D::create("Sprite3DTest/boss1.obj");
	//ship->setScale(5);
	//ship->setTexture("Sprite3DTest/boss.png");
	//ship->setNormalizedPosition(Vec2(.5, .5));
	//ship->setRotation3D(Vec3(90, 0, 0));
	//ship->setForceDepthWrite(true);
	//addChild(ship);

	//auto sprite = Sprite3D::create("Sprite3DTest/boss1.obj");
	//sprite->setScale(3.f);
	//sprite->setTexture("Sprite3DTest/boss.png");
	//addChild(sprite);
	//sprite->setPosition(Vec2(100, 100));
	//ActionInterval* action;
	//float random = CCRANDOM_0_1();

	//if (random < 0.20)
	//	action = ScaleBy::create(3, 2);
	//else if (random < 0.40)
	//	action = RotateBy::create(3, 360);
	//else if (random < 0.60)
	//	action = Blink::create(1, 3);
	//else if (random < 0.8)
	//	action = TintBy::create(2, 0, -255, -255);
	//else
	//	action = FadeOut::create(2);
	//auto action_back = action->reverse();
	//auto seq = Sequence::create(action, action_back, nullptr);
	//sprite->runAction(RepeatForever::create(seq));

	FileUtils::getInstance()->getWritablePath();


	auto action = CallFunc::create([=] {
		utils::captureScreen(CC_CALLBACK_2(Layer3DTest::afterCaptured, this), "123.png");
	});
	runAction(action);
	
	//atitc_decode();

}

void Layer3DTest::afterCaptured(bool succeed, const std::string& outputFile)
{
	if (succeed)
	{
		// show screenshot
		auto sp = Sprite::create(outputFile);
		addChild(sp);
		Size s = Director::getInstance()->getWinSize();
		sp->setPosition(s.width / 2, s.height / 2);
		sp->setScale(0.25);
	}
	else
	{
		log("Capture screen failed.");
	}
}
