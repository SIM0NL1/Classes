//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/23;
//@功能描述①: 封装Button;
//@功能描述②: 解决滑动和触摸优先级吞吃的问题,既可以传递下层的滑动，又可以触发本层的触摸事件;
//@功能描述③: 判断触摸范围,开启监听,判断滑动距离,切换图片显示选中状态,设置Visible,设置Enable;
//**************************************************************

#ifndef __OverSlideButton_H__
#define __OverSlideButton_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

class OverSlideButton : public Widget
{
public:
	Sprite* _buttonDisplayRenderer;
	std::string _buttonNormalImage;
	std::string _buttonSelectedImage;
	std::string _buttonDisableImage;
	Point _touchBeginPoint;
	float _slideDistance;
	EventListenerTouchOneByOne* _eventTouchOnelistener;
	bool _validSlide;
	Ref* _target;
	void* _sender;
	typedef std::function<void ()> slidebtn_call_back;
	ccWidgetTouchCallback _touchCallback;

public:
	static OverSlideButton* create();
	static OverSlideButton* create(const std::string& normalImage,
		const std::string& selectedImage = "",
		const std::string& disableImage = "");

	virtual bool init() override;
	virtual bool init(const std::string& normalImage,
		const std::string& selectedImage = "",
		const std::string& disableImage = "");

	virtual void addCallBack(const ccWidgetTouchCallback& callback);
	virtual bool onTouchBegan(Touch* touch,Event* unused_event);
	virtual void onTouchMoved(Touch* touch,Event* unused_event);
	virtual void onTouchEnded(Touch* touch,Event* unused_event);

	virtual const Size& getContentSize() const;
	virtual void setBright(bool bright);
	virtual void setVisible(bool visible);
	virtual void setEnabled(bool enable);

private:
	OverSlideButton();
	virtual ~OverSlideButton();

};

#endif