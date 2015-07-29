//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/23;
//@功能描述①: 封装Button;
//@功能描述②: 解决滑动和触摸优先级吞吃的问题,既可以传递下层的滑动，又可以触发本层的触摸事件;
//@功能描述③: 判断触摸范围,开启监听,判断滑动距离,切换图片显示选中状态,设置Visible,设置Enable;
//**************************************************************

#include "OverSlideButton.h"

OverSlideButton::OverSlideButton():
	_buttonDisplayRenderer(nullptr),
	_buttonNormalImage(""),
	_buttonSelectedImage(""),
	_buttonDisableImage(""),
	_touchBeginPoint(Vec2::ZERO),
	_slideDistance(0.0f),
	_eventTouchOnelistener(nullptr),
	_validSlide(false),
	_target(nullptr),
	_sender(nullptr),
	_touchCallback(nullptr)
{
}

OverSlideButton::~OverSlideButton()
{
}


OverSlideButton* OverSlideButton::create()
{
	OverSlideButton* widget = new (std::nothrow) OverSlideButton();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

OverSlideButton* OverSlideButton::create(const std::string& normalImage, const std::string& selectedImage /*= ""*/, const std::string& disableImage /*= ""*/)
{
	OverSlideButton *btn = new (std::nothrow) OverSlideButton;
	if (btn && btn->init(normalImage,selectedImage,disableImage))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}

bool OverSlideButton::init()
{
	if (Widget::init())
	{
		return true;
	}
	return false;
}

bool OverSlideButton::init(const std::string& normalImage, const std::string& selectedImage /*= ""*/, const std::string& disableImage /*= ""*/)
{
	do
	{
		CC_BREAK_IF(!Widget::init());
		//初始化图片;
		_buttonNormalImage = normalImage;
		_buttonSelectedImage = selectedImage;
		_buttonDisableImage = disableImage;
		_buttonDisplayRenderer = Sprite::create(_buttonNormalImage);
		_buttonDisplayRenderer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//_buttonDisplayRenderer->getContentSize().width*0.5,_buttonDisplayRenderer->getContentSize().height*0.5
		_buttonDisplayRenderer->setPosition(Vec2::ZERO);
		this->addChild(_buttonDisplayRenderer);

		//添加监听;
		_eventTouchOnelistener = EventListenerTouchOneByOne::create();     //创建单点触摸监听;
		_eventTouchOnelistener->setSwallowTouches(false);
		_eventTouchOnelistener->onTouchBegan = CC_CALLBACK_2(OverSlideButton::onTouchBegan,this);
		_eventTouchOnelistener->onTouchMoved = CC_CALLBACK_2(OverSlideButton::onTouchMoved,this);
		_eventTouchOnelistener->onTouchEnded = CC_CALLBACK_2(OverSlideButton::onTouchEnded,this);
		_eventTouchOnelistener->onTouchCancelled = CC_CALLBACK_2(OverSlideButton::onTouchCancelled,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(_eventTouchOnelistener,this);

		return true;
	} while (false);
	return false;
}

bool OverSlideButton::onTouchBegan(Touch* touch,Event* unused_event)
{
	_touchBeginPoint = this->convertToNodeSpace(touch->getLocation());
	if (_buttonDisplayRenderer->getBoundingBox().containsPoint(_touchBeginPoint))
	{
		_buttonDisplayRenderer->setTexture(_buttonSelectedImage);
		_validSlide = true;
	}
	return true;
}

void OverSlideButton::onTouchMoved(Touch* touch,Event* unused_event)
{
	Point curPoint = this->convertToNodeSpace(touch->getLocation());
	if (!_buttonDisplayRenderer->getBoundingBox().containsPoint(curPoint))
	{
		_validSlide = false;
	}
}

void OverSlideButton::onTouchEnded(Touch* touch,Event* unused_event)
{
	_buttonDisplayRenderer->setTexture(_buttonNormalImage);
	if (_validSlide && _touchCallback)
	{
		_touchCallback(this, TouchEventType::ENDED);
	}
}

void OverSlideButton::setBright(bool bright)
{
	_buttonDisplayRenderer->setTexture(_buttonDisableImage);
}

void OverSlideButton::setVisible(bool visible)
{
	_eventTouchOnelistener->setEnabled(visible);
	_buttonDisplayRenderer->setVisible(visible);
}

void OverSlideButton::setEnabled(bool enable)
{
	_eventTouchOnelistener->setEnabled(enable);
}

void OverSlideButton::addCallBack(const ccWidgetTouchCallback& callback)
{
	this->_touchCallback = callback;
}

const Size& OverSlideButton::getContentSize() const
{
	if (_buttonDisplayRenderer)
	{
		return _buttonDisplayRenderer->getContentSize();
	}
	else
	{
        Size s = Size(0,0);
        Size& t = s;
		return t;
	}
}
