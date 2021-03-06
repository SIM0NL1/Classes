﻿//**************************************************************
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
class RoleWidget : public Layout
{
public:
	int m_nId;
	Sprite* m_roleBg;		//角色底图;
	Button* m_roleBtn;		//角色按钮;
	Sprite* m_roleChoice;	//角色选中对号;
	Sprite* m_roleFightSpr;	//角色已经出战图片;
	Sprite* m_roleLock;		//角色锁定;

public:
	CREATE_FUNC(RoleWidget);
	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnRole(parameter_1_int callback,int& id);

private:
	RoleWidget();
	virtual bool init();

};

//角色;
enum RoleDisplayBtnTag
{
	RoleDisplayBtnExit = 1<<0,
	RoleDisplayBtnBig = 1<<1,
	RoleDisplayBtnFeed = 1<<2,
	RoleDisplayBtnFullGrade = 1<<3,
};

#define BIGBTN_FIGHT		m_spriteBigbtn->setTexture(RESOURCE("role/ui_gowar.png"))
#define BIGBTN_FIGHTING		m_spriteBigbtn->setTexture(RESOURCE("role/ui_inwar2.png"))
#define BIGBTN_Zuan600		m_spriteBigbtn->setTexture(RESOURCE("role/ui_600zuan.png"))
#define BIGBTN_Yuan12		m_spriteBigbtn->setTexture(RESOURCE("role/ui_12yuan.png"))

const int MAX = 4;

class RoleDisplay : public Layer
{
public:
	Button* m_btnExit;
	vector<RoleWidget*> m_vecRoleWidget;
	Label* m_labCrystal;
	Button* m_btnBig;	//召唤，出战按钮;
	Sprite* m_spriteBigbtn;		//大按钮上的文字图片;
	Sprite* m_roleName;	//宠物名;
	Button *m_roleFeed,*m_roleFullGrade;	//喂养,一键满级;
	Label *m_labFeedCost,*m_labFullGradeCost;	//喂养花费,一键满级花费;
	Label *m_labLvPrevious,*m_labLvTarget;	//目前等级和目标等级;
	Sprite* progressBack;	//进度条底板;
	ProgressTimer* m_cwProgress;
	Sprite* m_sprRoleLockBack;	//角色未解锁底板;
	Label* m_labRoleLock;		//角色解锁条件说明;
	Label* m_level;	//等级;
	Label* m_labProgress;	//等级进度;
	Armature* m_roleArmature;	//角色形象;
	Label *m_labPreviousText,*m_labTargetText;	//目前等级说明和目标等级说明;
	Sprite *paiA,*paiB,*m_sprArrow;
	Sprite* middleBack;
	Sprite *feedCrystal,*fullCrystal;
public:
	static Scene* createScene();
	CREATE_FUNC(RoleDisplay);
	void showUI();
	//初始化角色控件,包括是否解锁,是否出战,当前选项;
	void initRoleWidget();
	//初始化所有界面控件;
	void initAllWidget(int& id);

	void extractFighting(int& id);

	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnExit();
	void onBtnBig();
	void onBtnFeed();
	void onBtnFullGrade();
	void widgetBtnCallBack(int& id);
	void btnBright(Button* pSender,bool flag);
	//封装级别和级别说明文本;
	void extractLevelAndLvText( int& id,int level );
	//封装水晶总数削减;
	void extractReduceCrystal(int crystal);
	//封装进度条进度和百分比;
	void extractProgressPercent(int cur,int total);
	//封装一键满级和喂养的文字显示;
	void extractFullGradeAndFeeded(int& id, UserRole &temp, int total);
	CC_SYNTHESIZE(bool ,callBackEnable,CallBackEnable);

private:
	RoleDisplay();
	~RoleDisplay();
	virtual bool init();

};
extern RoleDisplay* g_pRoleDisplay;
#endif