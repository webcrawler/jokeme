#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "define_.h"

#include "cocos-ext.h"
#include "network/HttpClient.h"

using namespace cocos2d::network;
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1136, 640);  //cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MyFramework", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MyFramework");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

	FileUtils::getInstance()->addSearchPath("res");

    register_all_packages();

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
    //// run
    //director->runWithScene(scene);

	char p_temp[sizeof(int)] = {0};
	char* _p_buffer = "0x50";
	memcpy(p_temp, _p_buffer, sizeof(int));
	int hh = *((int*)p_temp);
	int joke = 0;

	unsigned short host_port = 0x1234, net_port;
	unsigned long host_addr = 0x12345678, net_addr;
	net_port = htons(host_port);
	net_addr = htonl(host_addr);
	printf("Host ordered port: %#x\n", host_port);
	printf("Network ordered port: %#x\n", net_port);
	printf("Host ordered address: %#lx\n", host_addr);
	printf("Network ordered address: %#lx\n", net_addr);

	//g_webfilesize.addWebFileSizeTask("http://www.cocos2d-x.org/attachments/802/cocos2dx_landscape.png", [=](float fileSize) {
	//CCLOG("fileSize1 = %.5f", fileSize);
	//});
	//g_webfilesize.addWebFileSizeTask("http://cdn.cocimg.com/assets/images/logo.png", [=](float fileSize) {
	//	CCLOG("fileSize2 = %.5f", fileSize);
	//});

	// win32
	//cocos2d::network::HttpClient::addWebFileSizeTask("http://www.kuu789.com/downloads/game.apk", [=](float fileSize) {
	//	CCLOG("fileSize3 = %.5f", fileSize);
	//	auto scene = Scene::create();
	//	auto label = Text::create(StringUtils::format("%.2f", fileSize), "Aril", 35);
	//	label->setPosition(Vec2(200, 200));
	//	scene->addChild(label);
	//	Director::getInstance()->runWithScene(scene);
	//});

	//HttpRequest* request = new HttpRequest();
	//request->setRequestType(HttpRequest::Type::GET);
	//request->setUrl("http://www.kuu789.com/downloads/game.apk");
	//request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
	//	if (!response)
	//	{
	//		return;
	//	}
	//	std::string tag = response->getHttpRequest()->getTag();
	//	if (!response->isSucceed())
	//	{
	//		return;
	//	}
	//	std::vector<char> respData = *response->getResponseData();
	//	string onlineVer = "1.0.0";
	//	onlineVer.assign(respData.begin(), respData.end());
	//});
	//HttpClient::getInstance()->send(request);
	//request->release();

	WebFileSize::addWebFileSizeTask("http://www.kuu789.com/downloads/game.apk", [=](float fileSize) {
		CCLOG("fileSize3 = %.5f", fileSize);
		auto scene = Scene::create();
		auto label = Text::create(StringUtils::format("%.2f", fileSize), "Aril", 35);
		label->setPosition(Vec2(200, 200));
		scene->addChild(label);
		Director::getInstance()->runWithScene(scene);
	});


#if 0
	AlphaScene* pScene = new AlphaScene();
	pScene->init();
	pScene->autorelease();
	CCDirector::sharedDirector()->runWithScene(pScene);
#endif

#if 0
	auto scene = ODSocketTest::createScene();
	director->runWithScene(scene);
#endif 

#if 0
	g_ctrl.startLoginScene();
#endif

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
