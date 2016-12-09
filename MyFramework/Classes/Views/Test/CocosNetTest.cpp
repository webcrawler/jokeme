#include "CocosNetTest.h"

// http://blog.csdn.net/cocosviva/article/details/18970717

static const char* ip = "127.0.0.1"; 
static unsigned short port = 1234;

GameNetDelegate::GameNetDelegate()
{
}

GameNetDelegate::~GameNetDelegate()
{
}

void GameNetDelegate::onMessageReceived(CCBuffer& oBuffer)
{
	CCLOG("onMessageReceived");
	//char* p = oBuffer.readWholeData();
	//unsigned int l = oBuffer.length();
	//p[l] = '\0';
	//CCLOG("%s", p);
	//delete p;

	//int n = oBuffer.readInt();
	//CCLOG("tag = %d", n);

	std::string ss = oBuffer.readString(5);
	
	int n = 1111;
	switch( n)
	{
	case 10:
		{
			CCLOG("%d", oBuffer.readInt());
			CCLOG("%d", oBuffer.readShort());
			CCLOG("%lld", oBuffer.readLongLong());
			CCLOG("%c", oBuffer.readChar());
			CCLOG("%f", oBuffer.readFloat());
			CCLOG("%lf", oBuffer.readDouble());
			CCLOG("%s", oBuffer.readLengthAndString().c_str());
		}
		break;
	case 5:
		{
			// large data
			CCLOG("large data length = %d", oBuffer.length());
		}
		break;
	case 6:
	{
		// large data
		CCLOG("large data length = %d", oBuffer.length());
	}
	break;
	}
}

void GameNetDelegate::onConnected()
{
	CCLOG("onConnected");
}

void GameNetDelegate::onConnectTimeout()
{
	CCLOG("onConnectTimeout");
}

void GameNetDelegate::onDisconnected()
{
	CCLOG("onDisconnected");
}

void GameNetDelegate::onExceptionCaught(CCSocketStatus eStatus)
{
	CCLOG("onExceptionCaught %d", (int)eStatus);
}

void GameNetDelegate::CS_Test()
{
	CCBuffer buffer;
	//buffer.writeInt(8);
	buffer.writeInt(10);
	buffer.writeLongLong(999999999L);
	buffer.writeFloat(77.7f);
	buffer.writeDouble(99.9);
	buffer.writeChar('h');
	buffer.writeLengthAndString("hahahahaha");

	this->send(&buffer);
}

void GameNetDelegate::CS_LargePackage()
{
	CCBuffer buffer;
	buffer.writeInt(5);

	this->send(&buffer);
}

bool AlphaScene::init()
{
	CCScene::init();

	CCMenuItemFont* pConnect = CCMenuItemFont::create("Connect", this, menu_selector(AlphaScene::onConnectClick));
	pConnect->setFontSize(30);
	pConnect->setPosition(ccp(200, 200));

	CCMenuItemFont* pDisconnect = CCMenuItemFont::create("Disconnect", this, menu_selector(AlphaScene::onDisconnectClick));
	pDisconnect->setFontSize(30);
	pDisconnect->setPosition(ccp(350, 200));

	CCMenuItemFont* pLargeData = CCMenuItemFont::create("LargeDataTest", this, menu_selector(AlphaScene::onLargeDataClick));
	pLargeData->setFontSize(30);
	pLargeData->setPosition(ccp(500, 300));

	CCMenuItemFont* pTest = CCMenuItemFont::create("Test", this, menu_selector(AlphaScene::onTestClick));
	pTest->setFontSize(30);
	pTest->setPosition(ccp(500, 200));

	CCMenu* pMenu = CCMenu::create(pConnect, pDisconnect, pTest, pLargeData, NULL);
	pMenu->setPosition(Vec2(-0, -100));
	addChild(pMenu);

	//test frame 
	//CCSprite* picon = CCSprite::create("Icon.png");
	//picon->setPosition(ccp(150, 400));

	//CCMoveBy* pMoveRight = CCMoveBy::create(0.5f, ccp(500, 0));
	//CCMoveBy* pMoveLeft = CCMoveBy::create(0.5f, ccp(-500, 0));
	//CCSequence* pSeq = CCSequence::create(pMoveRight, pMoveLeft, NULL);
	//picon->runAction(CCRepeatForever::create(pSeq));

	//addChild(picon);
	
	return true;
}

void AlphaScene::onConnectClick(CCObject* pSender)
{
	CCInetAddress oAddres;
	//oAddres.setIp("192.168.0.100");  "14.18.141.290"
	oAddres.setIp(ip);  
	oAddres.setPort(port);

	GameNetDelegate::sharedDelegate()->setInetAddress(oAddres);
	GameNetDelegate::sharedDelegate()->connect();
}

void AlphaScene::onDisconnectClick(CCObject* pSender)
{
	GameNetDelegate::sharedDelegate()->disconnect();
}

void AlphaScene::onTestClick(CCObject* pSender)
{
	GameNetDelegate::sharedDelegate()->CS_Test();
}

void AlphaScene::onLargeDataClick(CCObject* pSender)
{
	GameNetDelegate::sharedDelegate()->CS_LargePackage();
}