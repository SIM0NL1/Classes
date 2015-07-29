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
#include "GameUIData.h"

//控件类,每种角色控件,包括底图,按钮,动画,数据等;
enum RoleWidgetBtnTag
{
	RoleWidgetBtnRole,
};

class RoleWidget : public Layout
{
public:
	Sprite* m_roleBg;		//角色底图;
	Button* m_roleBtn;		//角色按钮;
	Sprite* m_roleChoice;	//角色选中;
	Sprite* m_roleFightSpr;	//角色已经出战图片;
	Sprite* m_roleLock;		//角色锁定;

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
	RoleDisplayBtnExit = 1<<0,
	RoleDisplayBtnBig = 1<<1,
};

class RoleDisplay : public Layer
{
public:
	Button* m_btnExit;
	vector<RoleWidget*> m_vecRoleWidget;
	Label* m_labCrystal;
	Button* m_btnBig;	//召唤，出战按钮;
	Sprite* m_roleName;	//宠物名;
	Button *m_roleFeed,*m_roleFullGrade;	//喂养,一键满级;
	Label *m_labFeedCost,*m_labFullGradeCost;	//喂养花费,一键满级花费;
	Label *m_labLvPrevious,*m_labLvTarget;	//目前等级和目标等级;
	ProgressTimer* m_cwProgress;
	Label* m_level;	//等级;
	Label* m_labProgress;	//等级进度;
	Armature* m_roleArmature;	//角色形象;
	Label *m_labPreviousText,*m_labTargetText;	//目前等级说明和目标等级说明;
public:
	static Scene* createScene();
	CREATE_FUNC(RoleDisplay);
	void showUI();

	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnExit();
	void onBtnBig();

private:
	RoleDisplay();
	virtual bool init();

};

#endif