
#include "LayerGame.h"

LayerGame* LayerGame::create()
{
	LayerGame* ptr = new LayerGame();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string LayerGame::getWidgetFilePath()
{
	return "";
}

string LayerGame::getLayerName()
{
	return "UI_LayerGame";
}

void LayerGame::updateUI()
{
	showGame();
}

void LayerGame::showGame()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerGame::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto size = Director::getInstance()->getVisibleSize();
	auto bg = ImageView::create(game_res("bg.png"));
	auto bgSize = bg->getSize();
	bg->setScaleX(size.width / bgSize.width);
	bg->setScaleY(size.height / bgSize.height);
	bg->setPosition(Vec2(size.width*0.5f, size.height*0.5f));
	//addChild(bg);

	m_gun = ImageView::create(game_res("gun.png"));
	m_gun->setPosition(Vec2(size.width*0.5f, m_gun->getSize().height*0.5f));
	addChild(m_gun);



}

void LayerGame::rotateGun(Vec2 pos)
{
	Vec2 dis = pos - m_gun->getPosition();
	m_gun->setRotation(Vec2(dis.y, dis.x).getAngle() / 3.14f * 180);
}

bool LayerGame::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	auto location = pTouch->getLocation();
	rotateGun(location);

	return true;
}