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
    Size tSize;
    void showUI();
    void initMap();
    void setBtnTouch(bool flag);
    void BtnCall(Ref* pSender,Widget::TouchEventType type);
    void onBtnShop();
    void onBtnAchievement();
    void onBtnMagicBox();
    void onBtnRole();
    void onBtnEndless();
    void onBtnRegisterLeft();
    void onBtnNewManGift();
    void onBtnRegisterRight();
    void onBtnMagicBook();
    void onBtnHP();
    void onBtnLongBi();
    void onBtnDiamond();
    //
    void ensureBtnFade(int id);
    //
    int getPageViewId();
    void setLeftBtnFadeInterface(int opacity);
    void setRightBtnFadeInterface(int opacity);
    void controlPageView(bool flag);
    void controlTableView(bool flag);
    OverScrollView* m_pPageView;
    
    virtual Size sizeForPerPage();
    virtual void pageViewDidScroll(OverScrollView* scrollView);
    
private:
    GameMain();
    ~GameMain();
    GameNormalMap* normallayer;
    GameChallengeMap* challengelayer;
    Point m_vStartPoint,m_vEndPoint;
    Button *m_btnHP,*m_btnLongBi,*m_btnDiamond;
    Label *m_labHP,*m_labLongBi,*m_labDiamond;
};
extern GameMain* g_pGameMain;
#endif