//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/01;
//@功能描述①: 封装ScrollView,模拟PageView功能;
//@功能描述②: 修改滑动范围;
//@功能描述③: ;
//**************************************************************

#ifndef __OverPageView_H__
#define __OverPageView_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class OverPageView;

class OverPageViewDelegate
{
public:
	OverPageViewDelegate(){};
	virtual ~OverPageViewDelegate(){};
	virtual Size sizeForPerPage()=0;
	virtual void pageViewDidScroll(OverPageView* pageView){};
};

class OverPageView : public extension::ScrollView
{
public:
	static OverPageView* create(Size size,OverPageViewDelegate* delegate);
	virtual bool init(Size size,OverPageViewDelegate* delegate);
	virtual void setContentOffsetInDuration(Vec2 offset,float dt);
	virtual void setContentOffset(Vec2 offset);

	EventListenerTouchOneByOne* listener;
	int pageCount;
	void addPage(Node* node);
	Node* getPageAtIndex(int index);
	int getCurPageIndex();
	//the pageview is turning ?;
	CC_SYNTHESIZE(bool,_turning,Turning);

private:
	void performedAnimatedScroll(float dt);
	int current_index;
	float current_offset;
	//调整offset的函数;
	void adjust(float offset);
	Size pageSize;
	CC_SYNTHESIZE(OverPageViewDelegate*,_delegate,Delegate);
};

#endif