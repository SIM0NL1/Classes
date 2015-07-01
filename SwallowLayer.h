#ifndef __DRAGONEGGTRIPLE_SWALLOWLATER_H__
#define __DRAGONEGGTRIPLE_SWALLOWLATER_H__

#include "cocos2d.h"

USING_NS_CC;

class SwallowLayer :
	public Layer
{
public:
	SwallowLayer(void);
	~SwallowLayer(void);

	virtual bool init();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);

	CREATE_FUNC(SwallowLayer);

	EventListenerTouchOneByOne* _listener_touch;
};

#endif


