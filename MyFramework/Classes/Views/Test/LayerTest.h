#ifndef __LATYER_TEST_H__
#define __LATYER_TEST_H__

#include "../../Define_.h"

class LayerTest : public LayerBase
{
public:
	static LayerTest* create();

	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

	virtual void viewEnter();
	virtual void viewExit();
	virtual void viewEnterDone();
	virtual void viewExitDone();

	virtual void updateEveryFrame(Ref* obj);

	void callback(Ref* pSender, Widget::TouchEventType type);
	void callback1(Ref* pSender, Widget::TouchEventType type);
	void pageViewEvent(Ref* pSender, PageView::EventType type);
	void callback2(Button* btn, int tag, const std::string& name);

public:
	void eventTest(void* data);
	void eventTest1(void* data);

public:
	void timercall1(float dt);
	void timercall2(float dt);


private:
	cocos2d::ui::EditBox* m_pAccount;
	cocos2d::ui::EditBox* m_pPwd;
};

#endif 
