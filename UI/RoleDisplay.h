//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/21;
//@功能描述①: 角色系统展示界面;
//@功能描述②: 用于角色展示，选择，升级，查看等功能;
//@功能描述③: 场景类，提供切换、调用接口,支持弹窗;
//**************************************************************

#ifndef __RoleDisplay_H__
#define __RoleDisplay_H__

#include "GameFunctions.h"

//控件类,每种角色控件,包括底图,按钮,动画,数据等;
enum RoleWidgetBtnTag
{
	RoleWidgetBtnRole,
	RoleWidgetBtnFight,
};

class RoleWidget : public Layout
{
public:
	Sprite* m_roleBg;		//角色底图;
	Button* m_roleBtn;		//角色按钮;
	Button* m_roleFightBtn;	//角色出战按钮;
	Sprite* m_roleFightSpr;	//角色已经出战图片;

public:
	CREATE_FUNC(RoleWidget);
	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnRole();
	void onBtnFight();

private:
	RoleWidget();
	virtual bool init();

};

//角色;
enum RoleDisplayBtnTag
{
	RoleDisplayBtnExit,
};

class RoleDisplay : public Layer
{
public:
	Button* m_btnExit;
	vector<RoleWidget*> m_vecRoleWidget;

public:
	static Scene* createScene();
	CREATE_FUNC(RoleDisplay);
	void showUI();

	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnExit();


private:
	RoleDisplay();
	virtual bool init();

};

#endif