#ifndef __LATYER_REG_H__
#define __LATYER_REG_H__

#include "../../Define_.h"

class LayerReg: public LayerBase
{
public:
	static LayerReg* create();

	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

	virtual void viewEnter();
	virtual void viewExit();
	virtual void viewEnterDone();
	virtual void viewExitDone();

	void callback(Ref* pSender, Widget::TouchEventType type);

public:
	void eventTest(void* data);
	void eventTest1(void* data);
};

#endif 
