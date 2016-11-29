/**
*  @brief: 游戏常量定义
*  @Created by fjut on 16-11-03
*/

#ifndef __UI_VAL_DEFINE_H__
#define __UI_VAL_DEFINE_H__

enum LayerOrder
{
	shelterZOrder = 1500,
	tipsZOrder = 1000,
	menuZOrder = 500,
	uiZOrder = 100
};

enum UIActionType
{
	unknowAction = -1,
	uiAction_open,
	uiAction_close
};

#endif 
