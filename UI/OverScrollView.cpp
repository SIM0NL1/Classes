//
//  OverScrollView.cpp
//  XXL
//
//  Created by 5agame on 15/5/14.
//
//

#include "OverScrollView.h"
#include "GameMain.h"
#include "GameUIData.h"
#include "GameMusicControl.h"

OverScrollView::OverScrollView()
{
	m_startPoint = Point::ZERO;
	m_endPoint = Point :: ZERO;
	_isChmap = false;
	current_offset = 0;
	current_index = 0;
    qiehuan = nullptr;
}

OverScrollView::~OverScrollView()
{
}

OverScrollView* OverScrollView::create(OverScrollViewDelegate* delegate)
{
	OverScrollView* scroll = new OverScrollView();
	if (scroll&&scroll->init(delegate))
	{
		scroll->autorelease();
	} 
	else
	{
		CC_SAFE_RELEASE(scroll);
	}
	return scroll;
}

bool OverScrollView::init(OverScrollViewDelegate* delegate)
{
	Size size = Size(GLB_SIZE.width*2-1,GLB_SIZE.height);
	if (!ScrollView::initWithViewSize(size))
	{
		return false;
	}
	//判断delegate;
	CCASSERT(delegate,"delegate should not be NULL!");
	setDelegate(delegate);

	Layer* pLayer = Layer :: create();
	pLayer->setContentSize(Size(640*2-1,1136));
	this->addChild(pLayer,Z_First);
    
	pTableView = OverTableView::create();
	pTableView->scrollViewForDistance();
    if (GameUIData::getInstance()->getVerticalGps() == -1)
    {
        horizontalGPS(false);
        GameUIData::getInstance()->setVerticalGps(GameUIData::getInstance()->getHorizontalGps());
    }
    else
    {
        verticalGPS();
    }
	pLayer->addChild(pTableView);

	//
    qiehuan = Armature::create("qiehuan_map");
	qiehuan->setAnchorPoint(Vec2(0.5f,0.5f));
	qiehuan->setPosition(Vec2(639,GLB_SIZE.height*0.5+80.f));
	this->addChild(qiehuan,Z_Third);
    GameFunctions::getInstance()->vertexZ(&qiehuan);
	qiehuan->getAnimation()->playWithIndex(0);

	pChmap = Sprite::create(RESOURCE("qiehuan_fanwei.png"));
	pChmap->setAnchorPoint(Vec2(0.5f,0.5f));
	pChmap->setPosition(Vec2(639,GLB_SIZE.height*0.5+70.f));
	this->addChild(pChmap,Z_Second);

	this->setDirection(extension::ScrollView::Direction::HORIZONTAL);

	listener = EventListenerTouchOneByOne::create();     //创建单点触摸监听;
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(OverScrollView::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(OverScrollView::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(OverScrollView::onTouchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(OverScrollView::onTouchCancelled,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

bool OverScrollView::onTouchBegan(Touch* touch,Event* unused_event)
{
	_dragging = false;
	//
	Point currentPoint = touch->getLocation();
	if (current_index)
	{
		currentPoint.x+=640;
	}
	if(pChmap->getBoundingBox().containsPoint(currentPoint))
	{
		_isChmap = true;
		pTableView->tableView->setTouchEnabled(false);
        qiehuan->getAnimation()->playWithIndex(1);
	}
	current_offset = this->getContentOffset().x;
	return true;
}

void OverScrollView::onTouchMoved(Touch* touch,Event* unused_event)
{
	if (_isChmap)
	{
		m_startPoint = touch->getStartLocation();
		m_endPoint = touch->getLocation();
		float offset = m_endPoint.x-m_startPoint.x;
		// 切换界面特效;
		chmapEffect(offset);
		// 右边禁止右滑;
		if (current_index && offset<0)
			return;
		// 左边禁止左滑;
		if (!current_index && offset>0)
			return;
		// 可以调节速度;
		offset = current_offset+offset*1;
		// 关闭两边的黑边;
		if (offset>0)
		{
			offset = 0;
		}
		else if (offset<-GLB_SIZE.width+1)
		{
			offset = -GLB_SIZE.width+1;
		}
		this->setContentOffset(Vec2(offset,0));
	}
}

void OverScrollView::onTouchEnded(Touch* touch,Event* unused_event)
{
	if (_isChmap)
	{
		m_endPoint = this->getContentOffset();
		float offset = m_endPoint.x - current_offset;
		adjustScrollView(offset);
		_isChmap = false;
        qiehuan->getAnimation()->playWithIndex(0);
	}
	_dragging = true;
}

void OverScrollView::onTouchCancelled(Touch* touch,Event* unused_event)
{
	log("OverScrollView::TouchCancelled------");
}

void OverScrollView::adjustScrollView(float offset)
{
	Vec2 vec;
	vec = Vec2(-(current_index*640),0);
    //小于200回到原来位置
    if (abs(offset)<200 && abs(offset)>50)
    {
        this->setContentOffsetInDuration(vec,0.1f);
        g_pGameMain->ensureBtnFade(current_index);
        extractFun();
        return;
    }
    else if (abs(offset)<=50)
    {
        //模拟按钮，I'm genius,HaHa;
        simulateBtn();
        return;
    }

	int i = abs(offset/(640))+1;
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

	vec = Vec2(-(current_index*(640)),0);
	this->setContentOffsetInDuration(vec,0.15f);

	g_pGameMain->ensureBtnFade(current_index);
    horizontalGPS();
}

void OverScrollView::horizontalGPS(bool flag/*=true*/)
{
    //定位;
    float offset = GameUIData::getInstance()->getHorizontalGps();
    if (flag)
    {
        pTableView->tableView->setContentOffsetInDuration(Vec2(0,offset),0.8f);
    }
    else
    {
        pTableView->tableView->setContentOffset(Vec2(0,offset));
    }
    //刷新GPS;
    if (current_index)
    {
        //翻动之后是右边;
        offset = GameUIData::getInstance()->getNormalMissionHeight();
    }
    else
    {
        //左边;
        offset = GameUIData::getInstance()->getChallengeMissionHeight();
    }
    GameUIData::getInstance()->setHorizontalGps(offset);
    if (flag)
    {
        this->runAction(Sequence::create(DelayTime::create(0.8f),CallFunc::create(CC_CALLBACK_0(OverScrollView::extractFun,this)),nullptr));
    } 
    else
    {
        extractFun();
    }
    //不同地图背景音乐也不同;
    if (current_index)
    {
        GameMusicControl::getInstance()->musicOn(CMAP_BACK_MUSIC_PATH);
        GameMusicControl::getInstance()->g_FlagBgMusic = true;
    }
    else if(GameMusicControl::getInstance()->g_FlagBgMusic)
    {
        GameMusicControl::getInstance()->musicOn();
    }
}

void OverScrollView::verticalGPS()
{
    int index = GameUIData::getInstance()->getVerticalIndex();
    Vec2 vec = Vec2(-(current_index*640),0);
    this->setContentOffset(vec);
    g_pGameMain->ensureBtnFade(index);
    
    float offset = GameUIData::getInstance()->getVerticalGps();
    pTableView->tableView->setContentOffset(Vec2(0,offset));
}

void OverScrollView::setContentOffsetInDuration(Vec2 offset,float dt)
{
	ScrollView::setContentOffsetInDuration(offset,dt);
	this->schedule(CC_SCHEDULE_SELECTOR(OverScrollView::performedAnimatedScroll));
}

void OverScrollView::setContentOffset(Vec2 offset)
{
	ScrollView::setContentOffset(offset);
	if (_delegate != nullptr)
	{
		_delegate->pageViewDidScroll(this);
	}
}

Node* OverScrollView::getPageAtIndex(int index)
{
	return nullptr;
}

int OverScrollView::getCurPageIndex()
{
	return current_index;
}

void OverScrollView::performedAnimatedScroll(float dt)
{
	if (_dragging)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(OverScrollView::performedAnimatedScroll));
		return;
	}

	if (_delegate != nullptr)
	{
		_delegate->pageViewDidScroll(this);
	}
}

void OverScrollView::chmapEffect(float offset)
{
	float percentage=0;
	int opacity=0;
	if (!current_index && offset<0)	// 左;
	{
		percentage = fabs(offset)/(GLB_SIZE.width*0.4);
		if (fabs(offset) <= GLB_SIZE.width*0.4)
		{
			opacity = 255*percentage;
			g_pGameMain->setLeftBtnFadeInterface(255-opacity);
		}
		else
		{
			opacity = 255*percentage-255;
			opacity = opacity>255 ? 255 : opacity;
			g_pGameMain->setRightBtnFadeInterface(opacity);
		}
	}
	else if (current_index && offset>0)	// 右;
	{
		percentage = offset/(GLB_SIZE.width*0.4);
		if (offset <= GLB_SIZE.width*0.4)
		{
			opacity = 255*percentage;
			g_pGameMain->setRightBtnFadeInterface(255-opacity);
		}
		else
		{
			opacity = 255*percentage-255;
			opacity = opacity>255 ? 255 : opacity;
			g_pGameMain->setLeftBtnFadeInterface(opacity);
		}
	}
}

void OverScrollView::extractFun()
{
    pTableView->tableView->setTouchEnabled(true);
}

void OverScrollView::simulateBtn()
{
    Vec2 vec = Vec2(current_index*640-640,0);
    this->setContentOffsetInDuration(vec,0.25f);
    current_index = !current_index;
    g_pGameMain->ensureBtnFade(current_index);
    horizontalGPS();
}

