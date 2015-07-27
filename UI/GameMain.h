//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①: ;
//@功能描述②: ;
//@功能描述③: ;
//**************************************************************

#ifndef __GameMain_H__
#define __GameMain_H__

#include "GameFunctions.h"
#include "GameNormalMap.h"
#include "GameChallengeMap.h"
#include "OverScrollView.h"

enum GameMainBtnType
{
    BtnShop,
    BtnAchievement,
    BtnMagicBox,
    BtnRole,
    BtnEndless,
    BtnRegisterLeft,
    BtnNewManGift,
    BtnMagicBook,
    BtnRegisterRight,
    BtnHP,
    BtnLongBi,
    BtnDiamond,
};

class GameMain : public Layer,public OverScrollViewDelegate
{
public:
    static Scene* createScene();
    CREATE_FUNC(GameMain);
    virtual bool init();
    
    Button *m_btnShop,*m_btnAchievement,*m_btnMagicBox,*m_btnRole;
    Button *m_btnEndless,*m_btnRegisterLeft,*m_btnNewManGift;
    Button *m_btnMagicBook,*m_btnRegisterRight;
	Label *m_labHP,*m_labLongBi,*m_labDiamond;
	cocos2d::ProgressTimer* loadProgress;//进度条;
    Size tSize;
	int m_hp;
	Label* m_labTimer;
	int m_nMin,m_nSec;	//倒计时的分秒;
	long long_time;
	bool m_IsContinue;

    void showUI();
    void initMap();
    void setBtnTouch(bool flag);
	void setAllBtnTouch(bool flag);
    void BtnCall(Ref* pSender,Widget::TouchEventType type);
    void onBtnShop();
    void onBtnAchievement();
    void onBtnMagicBox();
    void onBtnRole();
    void onBtnEndless();
    void onBtnRegisterLeft();
	//游戏胜利开启新关卡的开启效果;
	void extractOpenMethod(int nowProgress);
	//游戏胜利开启新关卡的移动效果;
	void extractMoveMethod();
	void onBtnNewManGift();
    void onBtnRegisterRight();
    void onBtnMagicBook();
    void onBtnHP();
    void onBtnLongBi();
    void onBtnDiamond();
    //地图切换按钮渐隐渐现特效;
    void ensureBtnFade(int id);
    int getPageViewId();
    void setLeftBtnFadeInterface(int opacity);
    void setRightBtnFadeInterface(int opacity);
    void controlPageView(bool flag);
    void controlTableView(bool flag);
    OverScrollView* m_pPageView;
    
    virtual Size sizeForPerPage();
    virtual void pageViewDidScroll(OverScrollView* scrollView);
	//更新体力值;
	void updateHpShow(int hp);
    //响应游戏返回,自动开启下一关,弹窗;
	void quitGameCallBack(QuitGameType type);
	//控制界面内除关卡按钮外的其他可触控件开关;
	void uiTouchEnable(bool flag);

	void startHpTimer();	//开启更新;
	void updateHp(float t);	//每次上线更新体力时间;
	
private:
    GameMain();
    ~GameMain();
    GameNormalMap* normallayer;
    GameChallengeMap* challengelayer;
    Point m_vStartPoint,m_vEndPoint;
    Button *m_btnHP,*m_btnLongBi,*m_btnDiamond;
    
};
extern GameMain* g_pGameMain;
#endif