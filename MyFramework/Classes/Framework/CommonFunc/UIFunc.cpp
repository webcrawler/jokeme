/**
*  @brief: ui公共函数
*  @Created by fjut on 16-11-19
*/

#include "UIFunc.h"
#include <math.h>

UIFunc& UIFunc::getInstance()
{
	static UIFunc instance_;
	return instance_;
}

void UIFunc::destroyInstance()
{
}

UIFunc::UIFunc()
{
}

//-- 获取scrollView
//-- scrollViewSize: scrollView size
//-- tbNode : 显示的节点vector
//-- rowNum : 每行显示个数(当tbNode里的Node大小不同时rowNum只能为1)
//-- tbOffset : vector格式{ a, b, c }->（选填）
//-- a : cellNode在x轴上偏移量, b : cellNode之间在x轴间隔偏移(针对rowNum不为1), c：cellNode之间在y轴间隔偏移
//-- cellNodeCall : 点击Node call func （选填）
//-- clippingType : 裁剪类型ClippingType::STENCIL, ClippingType::SCISSOR(选填默认STENCIL)
//-- bShowBar: 是否显示滚动条（选填）
ScrollView* UIFunc::getScrollView(const Size& scrollViewSize, const std::vector<Widget*>& tbNode, unsigned rowNum,
	const std::vector<float>& tbOffset, const Widget::ccWidgetTouchCallback& cellNodeCall,
	Layout::ClippingType clippingType, bool bShowBar)
{
	if (tbNode.size() == 0 || rowNum <= 0)
	{
		CCLOG("%s\n", "getScrollView fail, param is bad !");
		return nullptr;
	}

	auto scroll = ScrollView::create();
	scroll->setBounceEnabled(true);
	scroll->setDirection(ScrollView::Direction::VERTICAL);
	scroll->setTouchEnabled(true);
	scroll->setSize(scrollViewSize);
	scroll->setClippingType(clippingType);
	scroll->setScrollBarEnabled(bShowBar);

	unsigned int totalNum = tbNode.size();
	unsigned int cellNum = totalNum / rowNum + ((totalNum%rowNum == 0) ? 0 : 1);
	Size nodeSize = Size(tbNode.at(0)->getSize().width*tbNode.at(0)->getScaleX() + tbOffset.at(1),
		tbNode.at(0)->getSize().height*tbNode.at(0)->getScaleX() + tbOffset.at(2));

	function<Layout*(unsigned int)> getCellNode = [=](unsigned int cellIdx)
	{
		auto layout = Layout::create();
		layout->setBackGroundColorOpacity(0);
		layout->setTouchEnabled(false);
		layout->setSize(Size(scrollViewSize.width, nodeSize.height));
		for (unsigned int k = 1; k <= rowNum; k++)
		{
			unsigned int n = (cellIdx - 1)*rowNum + k;
			if (n > totalNum)
			{
				break;
			}
			Widget* node = tbNode.at(n - 1);
			node->setAnchorPoint(Vec2::ONE*0.5f);
			node->setPosition(Vec2(tbOffset.at(0) + nodeSize.width*(k - 1) + nodeSize.width*0.5f, nodeSize.height*0.5f));
			node->setTag(n);
			node->setTouchEnabled(true);
			node->setSwallowTouches(false);
			if (cellNodeCall != nullptr)
			{
				node->addTouchEventListener(cellNodeCall);
			}
			layout->addChild(node);
		}
		return layout;
	};

	float totalHeight = 0;
	if (rowNum == 1)
	{
		for (auto& v : tbNode)
		{
			Size size_ = v->getSize();
			totalHeight = totalHeight + size_.height + tbOffset.at(2);
		}
	}
	else
	{
		totalHeight = cellNum*(tbNode.at(0)->getSize().height*tbNode.at(0)->getScaleX() + tbOffset.at(2));
	}
	totalHeight = (totalHeight < scrollViewSize.height) ? scrollViewSize.height - 1.0f : totalHeight;
	scroll->setInnerContainerSize(Size(scrollViewSize.width, totalHeight));

	if (rowNum == 1)
	{
		float posX = 0;
		float posY = 0;
		for (unsigned int k = 1; k <= tbNode.size(); k++)
		{
			Widget* v = tbNode.at(k - 1);
			float sizeHeight = v->getSize().height + tbOffset.at(2);
			v->setAnchorPoint(Vec2::ONE*0.5f);
			posX = v->getSize().width*0.5f + tbOffset.at(0);
			function<float(unsigned int)> getHeight = [=](unsigned int idx)
			{
				float totalH = 0;
				for (int k1 = 1; k1 <= idx - 1; k1++)
				{
					float h = tbNode.at(k1 - 1)->getSize().height + tbOffset.at(2);
					totalH = totalH + h;
				}
				return totalH;
			};

			float tempL = sizeHeight*0.5f;
			posY = totalHeight - tempL - getHeight(k);
			v->setPosition(Vec2(posX, posY));
			v->setTag(k);
			v->setTouchEnabled(true);
			if (cellNodeCall != nullptr)
			{
				v->addTouchEventListener(cellNodeCall);
			}
			scroll->addChild(v);
		}
	}
	else
	{
		float posY = totalHeight - nodeSize.height;
		for (unsigned int k = 1; k <= cellNum; k++)
		{
			auto cellNode = getCellNode(k);
			cellNode->setPosition(Vec2(0, posY));
			posY = posY - nodeSize.height;
			scroll->addChild(cellNode, 2);
		}
	}
	return scroll;
}

//-- 获取横向scrollView
//-- scrollViewSize: scrollView size
//-- tbNode : 显示的节点vector
//-- rowNum : 每列显示个数(当tbNode里的Node大小不同时rowNum只能为1)
//-- tbOffset : vector格式{ a, b, c }->（选填）
//-- a : cellNode在y轴上偏移量, b : cellNode之间在y轴间隔偏移(针对rowNum不为1), c：cellNode之间在x轴间隔偏移
//-- cellNodeCall : 点击Node call func （选填）
//-- clippingType : 裁剪类型ClippingType::STENCIL, ClippingType::SCISSOR(选填默认STENCIL)
//-- bShowBar: 是否显示滚动条（选填）
ScrollView* UIFunc::getScrollViewHorizontal(const Size& scrollViewSize, const std::vector<Widget*>& tbNode, unsigned int rowNum,
	const std::vector<float>& tbOffset, const Widget::ccWidgetTouchCallback& cellNodeCall,
	Layout::ClippingType clippingType, bool bShowBar)
{
	if (tbNode.size() == 0 || rowNum <= 0)
	{
		CCLOG("%s\n", "getScrollViewHorizontal fail, param is bad !");
		return nullptr;
	}

	auto scroll = ScrollView::create();
	scroll->setBounceEnabled(true);
	scroll->setDirection(ScrollView::Direction::HORIZONTAL);
	scroll->setTouchEnabled(true);
	scroll->setSize(scrollViewSize);
	scroll->setClippingType(clippingType);
	scroll->setScrollBarEnabled(bShowBar);

	unsigned int totalNum = tbNode.size();
	unsigned int cellNum = totalNum / rowNum + ((totalNum%rowNum == 0) ? 0 : 1);

	float width = tbNode.at(0)->getSize().width*tbNode.at(0)->getScaleX() + tbOffset.at(2);
	float height = (tbNode.at(0)->getSize().height*tbNode.at(0)->getScaleX() + tbOffset.at(1))*rowNum;
	Size nodeSize = Size(width, height);

	function<Layout*(unsigned int)> getCellNode = [=](unsigned int cellIdx)
	{
		auto layout = Layout::create();
		layout->setBackGroundColorOpacity(0);
		layout->setTouchEnabled(false);
		layout->setSize(Size(nodeSize.width, nodeSize.height));
		for (unsigned int k = 1; k <= rowNum; k++)
		{
			unsigned int n = cellIdx + cellNum*(k - 1);
			if (n > totalNum)
			{
				break;
			}
			Widget* node = tbNode.at(n - 1);
			node->setAnchorPoint(Vec2::ONE*0.5f);
			node->setPosition(Vec2(nodeSize.width*0.5f, nodeSize.height / rowNum*(rowNum - k + 0.5f)));
			node->setTag(n);
			node->setTouchEnabled(true);
			node->setSwallowTouches(false);
			if (cellNodeCall != nullptr)
			{
				node->addTouchEventListener(cellNodeCall);
			}
			layout->addChild(node);
		}
		return layout;
	};

	float totalWidth = 0;
	if (rowNum == 1)
	{
		for (auto& v : tbNode)
		{
			Size size_ = v->getSize();
			totalWidth = totalWidth + size_.width + tbOffset.at(2);
		}
	}
	else
	{
		totalWidth = cellNum*(tbNode.at(0)->getSize().width*tbNode.at(0)->getScaleX() + tbOffset.at(2));
	}
	scroll->setInnerContainerSize(Size(totalWidth, scrollViewSize.height));

	if (rowNum == 1)
	{
		float posY = 0;
		float posX = 0;
		for (unsigned int k = 1; k <= tbNode.size(); k++)
		{
			Widget* v = tbNode.at(k - 1);
			float sizeWidth = v->getSize().width + tbOffset.at(2);
			v->setAnchorPoint(Vec2::ONE*0.5f);
			posY = scrollViewSize.height*0.5f + tbOffset.at(0);
			function<float(unsigned int)> getWidth = [=](unsigned int idx)
			{
				float totalW = 0;
				for (int k1 = 1; k1 <= idx - 1; k1++)
				{
					float w = tbNode.at(k1 - 1)->getSize().width + tbOffset.at(2);
					totalW = totalW + w;
				}
				totalW = totalW + sizeWidth*0.5f;
				return totalW;
			};

			posX = getWidth(k);
			v->setPosition(Vec2(posX, posY));
			v->setTag(k);
			v->setTouchEnabled(true);
			v->setSwallowTouches(false);
			if (cellNodeCall != nullptr)
			{
				v->addTouchEventListener(cellNodeCall);
			}
			scroll->addChild(v);
		}
	}
	else
	{
		for (unsigned int k = 1; k <= cellNum; k++)
		{
			auto cellNode = getCellNode(k);
			//cellNode->setAnchorPoint(Vec2::ZERO*0.5f);
			//float posX = width*0.5f + width*(k - 1);
			float posX = width*(k - 1);
			//cellNode->setPosition(Vec2(posX, scrollViewSize.height*0.5f + tbOffset.at(0)));
			cellNode->setPosition(Vec2(posX, scrollViewSize.height*0.5f - cellNode->getSize().height*0.5f + tbOffset.at(0)));
			scroll->addChild(cellNode, 2);
		}
	}
	return scroll;
}

//-- 获取pageView
//-- pageViewSize: pageView size
//-- tbNode : 显示的节点vector
//-- tbPagePerNum : {n1, n2}->n1 : 横向Node个数，n2 : 纵向Node个数（选填，默认横纵向只有一个）
//-- tbOffset : vector格式{ a, b, c }->（选填）
//-- a : node在x轴上偏移量(tbPagePerNum为1则不填), b : node之间在x轴间隔偏移, c：node之间在y轴间隔偏移
//-- nodeCall : 点击Node call func （选填）
//-- pageCall : pageView 拖动响应（选填）
PageView* UIFunc::getPageView(const Size& pageViewSize, const std::vector<Widget*>& tbNode, const std::vector<unsigned int>& tbPagePerNum,
	const std::vector<float>& tbOffset, const Widget::ccWidgetTouchCallback& nodeCall, 
	const PageView::ccPageViewCallback& pageCall, bool bIndicator)
{
	if (tbNode.size() == 0)
	{
		CCLOG("%s\n", "getPageView fail, param is bad !");
		return nullptr;
	}

	Size nodeSize = Size(tbNode.at(0)->getSize().width + tbOffset.at(1), tbNode.at(0)->getSize().height + tbOffset.at(2));
	unsigned int totalNum = tbNode.size();
	unsigned int onePageNum = tbPagePerNum.at(0) * tbPagePerNum.at(1);
	unsigned int pageNum = totalNum / onePageNum + (((totalNum%onePageNum) == 0) ? 0 : 1);
	bool bSingle = false;
	if (tbPagePerNum.at(0) == 1 && tbPagePerNum.at(1) == 1)
	{
		bSingle = true;
		pageNum = totalNum;
	}

	function<Layout*(unsigned int padeIdx)> getPageNode = [=](unsigned int pageIdx)
	{
		auto layout = Layout::create();
		layout->setBackGroundColorOpacity(0);
		layout->setTouchEnabled(false);
		layout->setSize(pageViewSize);

		float w = (tbNode.at(0)->getSize().width + tbOffset.at(1))*tbPagePerNum.at(0);
		float h = (tbNode.at(0)->getSize().height + tbOffset.at(2))*tbPagePerNum.at(1);
		float startX = (pageViewSize.width - w)*0.5f;
		float startY = (pageViewSize.height - h)*0.5f;

		for (unsigned int k = 1; k <= onePageNum; k++)
		{
			unsigned int n = (pageIdx - 1)*onePageNum + k;
			if (n > totalNum)
			{
				break;
			}
			Widget* node = tbNode.at(n - 1);
			node->setAnchorPoint(Vec2::ONE*0.5f);
			unsigned int nRow = (k - 1) / tbPagePerNum.at(0);
			unsigned int key = (k%tbPagePerNum.at(0) == 0) ? tbPagePerNum.at(0) : k%tbPagePerNum.at(0);
			float posX = tbOffset.at(0) + nodeSize.width*0.5f + nodeSize.width*(key - 1) + startX;
			posX = (k%tbPagePerNum.at(0) == 1) ? tbOffset.at(0) + startX + nodeSize.width*0.5f : posX;
			float posY = (pageViewSize.height - nodeSize.height*0.5f) - nodeSize.height*nRow - startY;
			if (posX > (pageViewSize.width - nodeSize.width*0.5f))
			{
				CCLOG("%s\n", "getPageView() row num is too much, beyond the window area!");
			}
			if (posY < nodeSize.height*0.5f)
			{
				CCLOG("%s\n", "getPageView() columns num is too much, beyond the window area!");
			}
			node->setPosition(Vec2(posX, posY));
			node->setTag(n);
			node->setTouchEnabled(true);
			if (nodeCall != nullptr)
			{
				node->addTouchEventListener(nodeCall);
			}
			layout->addChild(node);
		}
		return layout;
	};

	auto pageView = PageView::create();
	pageView->setTouchEnabled(true);
	pageView->setSize(pageViewSize);
	pageView->setIndicatorEnabled(bIndicator);

	if (pageCall != nullptr)
	{
		pageView->addEventListener(pageCall);
	}

	if (bSingle)
	{
		for (unsigned int k = 1; k <= tbNode.size(); k++)
		{
			auto v = tbNode.at(k - 1);
			auto layout = Layout::create();
			layout->setBackGroundColorOpacity(0);
			layout->setTouchEnabled(false);
			layout->setSize(pageViewSize);
			layout->addChild(v);
			v->setAnchorPoint(Vec2::ONE*0.5f);
			v->setPosition(Vec2(pageViewSize.width, pageViewSize.height)*0.5f);
			v->setTouchEnabled(true);
			v->setTag(k);
			if (nodeCall != nullptr)
			{
				v->addTouchEventListener(nodeCall);
			}
			pageView->addPage(layout);
		}
	}
	else
	{
		for (unsigned int k = 1; k <= pageNum; k++)
		{
			auto node = getPageNode(k);
			auto size = node->getSize();
			//node->setPosition(Vec2(size.width, size.height)*0.5f);
			node->setPosition(Vec2(pageViewSize.width, pageViewSize.height)*0.5f);
			pageView->addPage(node);
		}
	}
	return pageView;
}

EditBox* UIFunc::getEditBoxForNormal(const string& img, EditBoxDelegate* pDelegate, const string& placeHolder, int len, int fontSize, Color3B color)
{
	auto p9sprite = Scale9Sprite::create(img);
	auto editbox = EditBox::create(p9sprite->getBoundingBox().size, p9sprite);
	editbox->setAnchorPoint(Vec2(0, 0));
	editbox->setMaxLength(len);
	editbox->setFontSize(fontSize);
	editbox->setDelegate(pDelegate);
	editbox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	editbox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	editbox->setFontColor(color);
	editbox->setPlaceHolder(placeHolder.c_str());
	return editbox;
}

EditBox* UIFunc::getEditBoxForPwd(const string& img, EditBoxDelegate* pDelegate, const string& placeHolder, int len, int fontSize, Color3B color)
{
	auto p9sprite = Scale9Sprite::create(img);
	auto editbox = EditBox::create(p9sprite->getBoundingBox().size, p9sprite);
	editbox->setAnchorPoint(Vec2(0, 0));
	editbox->setMaxLength(len);
	editbox->setFontSize(fontSize);
	editbox->setDelegate(pDelegate);
	editbox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	//editbox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	editbox->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	editbox->setFontColor(color);
	editbox->setPlaceHolder(placeHolder.c_str());
	return editbox;
}

ClippingNode* UIFunc::getStencilNode(const string& charImg)
{
	auto clip = ClippingNode::create(Sprite::create(charImg));
	// todo
	return clip;
}

