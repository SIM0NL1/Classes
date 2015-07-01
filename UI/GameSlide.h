//
//	GameSlide.h
//	Author:Simon
//	Date:  2015.5.21
//	本类作为滑动监听层
//	只做移动距离监听和处理，不做UI，如果需要，可以调用接口。
//

#ifndef __GameSlide_H__
#define __GameSlide_H__

#include "GameDefine.h"
enum class SlideDirection
{
	SlideNone,
	SlideLeft,
	SlideRight,
	SlideUp,
	SlideDown,
};
class GameSlide : public Layer
{
public:

	CREATE_FUNC(GameSlide);
	bool init();
	EventListenerTouchOneByOne* listener;
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *touch, Event *unused_event);
	void setDirection(Point movePoint);
	//主界面功能接口;
	void setMainInterface(float distance,SlideDirection direction);

private:
	GameSlide();
	~GameSlide();
	Point m_startPoint;

};

#endif