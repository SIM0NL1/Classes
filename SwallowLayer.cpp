#include "SwallowLayer.h"


SwallowLayer::SwallowLayer(void)
{
}


SwallowLayer::~SwallowLayer(void)
{
    
}

bool SwallowLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    
	_listener_touch = EventListenerTouchOneByOne::create();
	_listener_touch->onTouchBegan = CC_CALLBACK_2(SwallowLayer::onTouchBegan,this);
	_listener_touch->onTouchMoved = CC_CALLBACK_2(SwallowLayer::onTouchMoved,this);
	_listener_touch->onTouchEnded = CC_CALLBACK_2(SwallowLayer::onTouchEnded,this);
	_listener_touch->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);

	return true;
}

bool SwallowLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void SwallowLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void SwallowLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void SwallowLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{

}