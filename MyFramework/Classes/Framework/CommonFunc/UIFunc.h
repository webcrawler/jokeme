/**
*  @brief: ui¹«¹²º¯Êý
*  @Created by fjut on 16-11-19
*/

#ifndef __UI_FUNC_H__
#define __UI_FUNC_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
using namespace cocos2d;
using namespace std;
using namespace ui;

class UIFunc
{
public:
	UIFunc();
	static UIFunc& getInstance();
	static void destroyInstance();

public:
	ScrollView* getScrollView(const Size& scrollViewSize, const std::vector<Widget*>& tbNode, unsigned int rowNum = 1,
		const std::vector<float>& tbOffset = { 0, 0, 0 }, const Widget::ccWidgetTouchCallback& cellNodeCall = nullptr,
		Layout::ClippingType clippingType = Layout::ClippingType::STENCIL, bool bShowBar = false);

	ScrollView* getScrollViewHorizontal(const Size& scrollViewSize, const std::vector<Widget*>& tbNode, unsigned int rowNum = 1,
		const std::vector<float>& tbOffset = { 0, 0, 0 }, const Widget::ccWidgetTouchCallback& cellNodeCall = nullptr,
		Layout::ClippingType clippingType = Layout::ClippingType::STENCIL, bool bShowBar = false);

	PageView* getPageView(const Size& pageViewSize, const std::vector<Widget*>& tbNode, const std::vector<unsigned int>& tbPagePerNum = { 1, 1 },
		const std::vector<float>& tbOffset = { 0, 0, 0 }, const Widget::ccWidgetTouchCallback& nodeCall = nullptr,
		const PageView::ccPageViewCallback& pageCall = nullptr, bool bIndicator = false);

	EditBox* getEditBoxForNormal(const string& img, EditBoxDelegate* pDelegate, const string& placeHolder = "",
		int len = 10, int fontSize = 20, Color3B color = Color3B::WHITE);

	EditBox* getEditBoxForPwd(const string& img, EditBoxDelegate* pDelegate, const string& placeHolder = "",
		int len = 10, int fontSize = 20, Color3B color = Color3B::WHITE);

	ClippingNode* getStencilNode(const string& charImg);

};

#define g_uifunc UIFunc::getInstance()

#endif 
