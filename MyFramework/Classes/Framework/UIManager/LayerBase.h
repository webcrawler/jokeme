/**
*  @brief: ui����
*  @Created by fjut on 16-11-02
*/

#ifndef __LATYER_BASE_H__
#define __LATYER_BASE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../EventDispather/Define_EventDispather.h"

using namespace cocostudio;
using namespace cocos2d;
using namespace std;
using namespace ui;

class LayerBase: public Layer
{
public:
	LayerBase();
	~LayerBase();

protected:
	virtual bool init();
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual Widget* getWidgetByName(const string& name);
	virtual Button* getBtnByName(const string& name);
	virtual ImageView* getImgByName(const string& name);
	virtual Text* getTextByName(const string& name);

protected:
	virtual Widget* regCallbackByName(const string& name, const Widget::ccWidgetTouchCallback& callback);

public:
	virtual string getWidgetFilePath() = 0;
	virtual string getLayerName() = 0;
	virtual void updateUI() = 0;

public:
	void swallowTouchs(bool bSwallowTouchs);

public:
	virtual void viewEnter() {};
	virtual void viewEnterDone() {};
	virtual void viewUnRegEvents();
	virtual void viewExit() {};
	virtual void viewExitDone() {};

protected:
	//typedef function<void()> viewCallback;
	//virtual void setViewEnterAndExitCallback(const viewCallback& viewEnter = nullptr, const viewCallback& viewExit = nullptr);

private:
	void addWidgetToLayer(bool bAdapted = true);
	
protected:
	Widget* mRoot;
	//viewCallback m_viewEnter, m_viewExit;
};

#endif 