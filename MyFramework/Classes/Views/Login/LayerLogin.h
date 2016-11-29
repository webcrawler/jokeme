#ifndef __LATYER_LOGIN_H__
#define __LATYER_LOGIN_H__

#include "../../Define_.h"

class LayerLogin : public LayerBase, public cocos2d::ui::EditBoxDelegate
{
public:
	static LayerLogin* create();

	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

	virtual void viewEnter();
	virtual void viewExit();
	virtual void viewEnterDone();
	virtual void viewExitDone();

	void callback(Ref* pSender, Widget::TouchEventType type);
	void callback1(Ref* pSender, Widget::TouchEventType type);
	void pageViewEvent(Ref* pSender, PageView::EventType type);

public:
	virtual void editBoxEditingDidBegin(ui::EditBox* editBox) ;
	virtual void editBoxReturn(ui::EditBox* editBox) ;
	void addEdBox();

public:
	void eventTest(void* data);
	void eventTest1(void* data);

private:
	cocos2d::ui::EditBox* m_pAccount;
	cocos2d::ui::EditBox* m_pPwd;
};

#endif 
