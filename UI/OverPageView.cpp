//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/01;
//@功能描述①: 封装ScrollView,模拟PageView功能;
//@功能描述②: 修改滑动范围;
//@功能描述③: ;
//**************************************************************

#include "OverPageView.h"
#include "GameMain.h"

const int OverPageView_Tag = 10086;

OverPageView* OverPageView::create(Size size,OverPageViewDelegate* delegate)
{
	OverPageView* page = new OverPageView();
	if (page&&page->init(size,delegate))
	{
		page->autorelease();
	} 
	else
	{
		CC_SAFE_RELEASE(page);
	}
	return page;
}

bool OverPageView::init(Size size,OverPageViewDelegate* delegate)
{
	if (!ScrollView::initWithViewSize(size))
	{
		return false;
	}
	//判断delegate;
	CCASSERT(delegate,"delegate should not be NULL!");
	setDelegate(delegate);
	if (_delegate)
	{
		//获取page的大小;
		pageSize = _delegate->sizeForPerPage();
	}
	//init Data
	pageCount = 0;
	current_index = 0;
	// Is there everything OK?
	this->setTouchEnabled(false);
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch,Event* event)
	{
		_turning = true;		//pageview is turning;
		_dragging = false;
		if (_direction == ScrollView::Direction::HORIZONTAL)
		{
			current_offset = this->getContentOffset().x;
		} 
		else
		{
			current_offset = this->getContentOffset().y;
		}
		return true;
	};
	listener->onTouchMoved = [&](Touch* touch,Event* event)
	{
		float start,end;
		if (_direction == ScrollView::Direction::HORIZONTAL)
		{
			start = touch->getStartLocation().x;
			end = touch->getLocation().x;
		} 
		else
		{
			start = touch->getStartLocation().y;
			end = touch->getLocation().y;
		}
		float offset = end-start;
		// * 1 调节滚动速度;
		if (_direction ==  ScrollView::Direction::HORIZONTAL)
		{
			this->setContentOffset(Vec2(current_offset+offset*1,0));
		} 
		else
		{
			this->setContentOffset(Vec2(0,current_offset+offset*1));
		}
	};
	listener->onTouchEnded = [&](Touch* touch,Event* event)
	{
		_turning = false;
		float start = current_offset,end;
		if (_direction == ScrollView::Direction::HORIZONTAL)
		{
			end = this->getContentOffset().x;
		} 
		else
		{
			end = this->getContentOffset().y;
		}
		float offset = end-start;
		this->adjust(offset);
		_dragging = true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	listener->setEnabled(true);

	return true;
}

void OverPageView::setContentOffsetInDuration(Vec2 offset,float dt)
{
	ScrollView::setContentOffsetInDuration(offset,dt);
	this->schedule(CC_SCHEDULE_SELECTOR(OverPageView::performedAnimatedScroll));
}

void OverPageView::setContentOffset(Vec2 offset)
{
	ScrollView::setContentOffset(offset);
	if (_delegate != nullptr)
	{
		_delegate->pageViewDidScroll(this);
	}
}

void OverPageView::addPage(Node* node)
{
	if (_direction == ScrollView::Direction::HORIZONTAL)
	{
		node->setPosition(Point(pageCount*pageSize.width+node->getPositionX(),node->getPositionY()));
		this->setContentSize(Size((pageCount+1)*pageSize.width,pageSize.height));
	} 
	else
	{
		node->setPosition(Point(node->getPositionX(),pageCount*pageSize.height+node->getPositionY()));
		this->setContentSize(Size(pageSize.width,(pageCount+1)*pageSize.height));
	}
	node->setTag(pageCount+OverPageView_Tag);
	_container->addChild(node);
	pageCount++;
}

Node* OverPageView::getPageAtIndex(int index)
{
	if (index < pageCount && index >= 0)
	{
		return _container->getChildByTag(index + OverPageView_Tag);
	}
	return NULL;
}

void OverPageView::performedAnimatedScroll(float dt)
{
	if (_dragging)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(OverPageView::performedAnimatedScroll));
		return;
	}

	if (_delegate != nullptr)
	{
		_delegate->pageViewDidScroll(this);
	}
}

void OverPageView::adjust(float offset)
{
	Vec2 vec;
	float xOrY;
	if (_direction == ScrollView::Direction::HORIZONTAL)
	{
		vec = Vec2(-(current_index*(pageSize.width)),0);
		xOrY = pageSize.width;
	}
	else
	{
		vec = Vec2(0,-(current_index*(pageSize.height)));
		xOrY = pageSize.height;
	}
	//小于50回到原来位置
	if (abs(offset)<50)
	{
		this->setContentOffsetInDuration(vec,0.1f);
		return;
	}

	int i = abs(offset/(xOrY))+1;
	if (offset<0)
	{
		current_index += i;
	}
	else
	{
		current_index -= i;
	}
	if (current_index < 0)
	{
		current_index = 0;
	}
	else if(current_index > 1)
	{
		current_index = 1;
	}

	if (_direction == ScrollView::Direction::HORIZONTAL)
	{
		vec = Vec2(-(current_index*(pageSize.width)),0);
	} 
	else
	{
		vec = Vec2(0,-(current_index*(pageSize.height)));
	}
	this->setContentOffsetInDuration(vec,0.15f);
	g_pGameMain->ensureBtnFade(current_index);
}

int OverPageView::getCurPageIndex()
{
	return current_index;
}
