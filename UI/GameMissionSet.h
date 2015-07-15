//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/03;
//@功能描述①: GameMissionSet.h;
//@功能描述②: 从关卡进入游戏，配置数据，配置角色;
//@功能描述③: UI功能类;
//**************************************************************

#ifndef __GameMissionSet_H__
#define __GameMissionSet_H__

#include "GameFunctions.h"

enum GameMissionSetBtnTag
{
	GameMissionSetBtnStart,
	GameMissionSetBtnExit,
};

//class TimeMachine;
class GameMissionSet : public Layer
{
public:
	Label *m_labHpCoin,*m_labTimer;
	Button *m_btnStart,*m_btnExit;
	int m_nHp;
	int m_nMin,m_nSec;	//倒计时的分秒;
	long long_time;

public:
	static Scene* createScene();
	CREATE_FUNC(GameMissionSet);
	void showUI();
	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnStart();
	void onBtnExit();
	void startHpTimer();	//开启更新;
	void updateHp(float t);	//每次上线更新体力时间;
	void reduceHp();

private:
	GameMissionSet();
	virtual ~GameMissionSet();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
};
extern GameMissionSet* g_pGameMissionSet;
#endif