//
//	GameSlide.cpp
//	Author:Simon
//	Date:  2015.5.21
//	本类作为滑动监听层;
//	哈哈哈哈哈哈，这个实在是抑制不住了,I AM A GENIUS，哈哈哈，这个困扰我很久的问题终于解决了，哈哈哈哈;
//	所有问题都可以用一个中间层来解决，这个思想不仅适用于硬件层啊;
//

#include "GameSlide.h"
#include "GameMain.h"
#include "GameNormalMap.h"

GameSlide::GameSlide()
{
	m_startPoint = Point::ZERO;
}

GameSlide::~GameSlide()
{
}

bool GameSlide::init()
{
	do 
	{
		CC_BREAK_IF(!Layer::init());

		listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(false);
		listener->onTouchBegan = CC_CALLBACK_2(GameSlide::onTouchBegan,this);
		listener->onTouchMoved = CC_CALLBACK_2(GameSlide::onTouchMoved,this);
		listener->onTouchEnded = CC_CALLBACK_2(GameSlide::onTouchEnded,this);
		listener->onTouchCancelled = CC_CALLBACK_2(GameSlide::onTouchCancelled,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

		return true;
	} while (false);
	log("GameSlide init ERROR");
	return false;
}

bool GameSlide::onTouchBegan(Touch *touch, Event *unused_event)
{
	m_startPoint = touch->getLocation();
	return true;
}

void GameSlide::onTouchMoved(Touch *touch, Event *unused_event)
{
	//移动过程中禁用左右按钮;
	g_pGameMain->setBtnTouch(false);
	Point move = touch->getLocation();
	setDirection(move);
}

void GameSlide::onTouchEnded(Touch *touch, Event *unused_event)
{
	g_pGameMain->setBtnTouch(true);
	int pageId = g_pGameMain->getPageViewId();
	log(" page id  =  %d",pageId);
	if (!pageId)
	{
		//g_pGameNormalMap->normalActiveMissionBtnTouch(true);
	} 
	else
	{
		log(" find the challenge active mission ");
	}
	//g_pGameMain->controlPageView(true);
	//g_pGameMain->controlTableView(true);
	g_pGameMain->ensureBtnFade(g_pGameMain->getPageViewId());

}

void GameSlide::onTouchCancelled(Touch *touch, Event *unused_event)
{
	log("GameSlide::onTouchCancelled");
	g_pGameMain->setBtnTouch(true);
	int pageId = g_pGameMain->getPageViewId();
	log(" page id  =  %d",pageId);
	if (!pageId)
	{
		//g_pGameNormalMap->normalActiveMissionBtnTouch(true);
	} 
	else
	{
		log(" find the challenge active mission ");
	}
//	g_pGameMain->controlPageView(true);
//	g_pGameMain->controlTableView(true);
}

void GameSlide::setDirection( Point movePoint )
{
	float gap_x = movePoint.x-m_startPoint.x;
	float gap_y = movePoint.y-m_startPoint.y;
	log(" gap_x  =  %f,  gap_y  =  %f",fabsf(gap_x),fabsf(gap_y));
	SlideDirection _direction=SlideDirection::SlideNone;
	float _distance = 0;
	if (fabsf(gap_x)>fabsf(gap_y))
	{
		//水平方向开启关卡模式切换;
		//g_pGameMain->controlPageView(true);
		//g_pGameMain->controlTableView(false);
		_direction = gap_x > 0 ? SlideDirection::SlideRight : SlideDirection::SlideLeft;
		_distance = fabsf(gap_x);
	} 
	else
	{
		//垂直方向关闭关卡模式切换;
		//g_pGameMain->controlTableView(true);
		//g_pGameMain->controlPageView(false);
		_direction = gap_y > 0 ? SlideDirection::SlideUp : SlideDirection::SlideDown;
		_distance = fabsf(gap_y);
	}
	setMainInterface(_distance,_direction);
}

void GameSlide::setMainInterface(float distance,SlideDirection direction)
{
	float percentage=0;
	int opacity=0;
	int pageId = g_pGameMain->getPageViewId();
	log(" page id  =  %d",pageId);
	if (!pageId)
	{
		//g_pGameNormalMap->normalActiveMissionBtnTouch(false);
	} 
	else
	{
		log(" find the challenge active mission ");
	}
	switch (direction)
	{
	case SlideDirection::SlideRight:
		{
			CCLOG("slide Right distance = %f",distance);
			if (pageId)
			{
				percentage = distance/(GLB_SIZE.width*0.4);
				if (distance <= GLB_SIZE.width*0.4)
				{
					opacity = 255*percentage;
					g_pGameMain->setRightBtnFadeInterface(255-opacity);
				}
				else
				{
					opacity = 255*percentage-255;
					g_pGameMain->setLeftBtnFadeInterface(opacity);
				}
			}
			break;
		}
	case SlideDirection::SlideLeft:
		{
			CCLOG("slide Left distance = %f",distance);
			if (!pageId)
			{
				percentage = distance/(GLB_SIZE.width*0.4);
				if (distance <= GLB_SIZE.width*0.4)
				{
					opacity = 255*percentage;
					g_pGameMain->setLeftBtnFadeInterface(255-opacity);
				}
				else
				{
					opacity = 255*percentage-255;
					g_pGameMain->setRightBtnFadeInterface(opacity);
				}
			}
			break;
		}
	case SlideDirection::SlideUp:
		{
			g_pGameMain->ensureBtnFade(pageId);
// 			if (!pageId && !GameFunctions::getInstance()->g_bFlagForMission)
// 			{
// 				g_pGameNormalMap->scrollViewForDistance(distance);
// 			} 
// 			else
// 			{
// 			}
			break;
		}
	case SlideDirection::SlideDown:
		{
			g_pGameMain->ensureBtnFade(pageId);
// 			if (!pageId && !GameFunctions::getInstance()->g_bFlagForMission)
// 			{
// 				g_pGameNormalMap->scrollViewForDistance(-distance);
// 			} 
// 			else
// 			{
// 			}
			break;
		}
	}
}