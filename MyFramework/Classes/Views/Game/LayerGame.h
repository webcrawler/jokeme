#ifndef __LATYER_GAME_H__
#define __LATYER_GAME_H__

#include "../../Define_.h"

class LayerGame : public LayerBase
{
public:
	static LayerGame* create();

	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

private:
	void showGame();
	bool onTouchBegan(Touch* pTouch, Event* pEvent);
	void rotateGun(Vec2 pos);

private:
	ImageView* m_gun;

};

#endif 
