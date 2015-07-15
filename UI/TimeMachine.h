//**********************************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/27;
//@功能描述①: TimeMachine : 不要以为偶是时光鸡！;
//@功能描述②: 时间管理器(单例),用于管理复杂的时间线程,配合更新函数,观察者,一起使用;
//@功能描述③: 塔防的Boss，士兵的生命周期，将来的资源“多线程”加载等等;
//**********************************************************************************

#ifndef __TimeMachine_H__
#define __TimeMachine_H__

#include "GameFunctions.h"

class TimeMachine : public Node
{
public:
	TimeMachine();
	virtual ~TimeMachine();
	static TimeMachine* m_self;
	time_t longTime;
	long long_timer;

public:
	static TimeMachine* getInstance();
	void destroyMachine();
	virtual void onEnter();
	virtual void onExit();
	//开启总闸模式;
	void startTimer();
	void update(float t);
	//关闭总闸模式;
	void closeTimer();

	//启动Boss攻击周期;
	void startBossAC(int& attackRate);
	//启动士兵攻击周期;
	void startSoldierAC(int& attackRate);

	//关闭生命周期,需要指定关闭哪一个,谁的生命周期;
};
//BOSS;
class TDBossLCA : public Node
{
public:
	TDBossLCA();
	~TDBossLCA();
	long bossAttackTimer;	//生命计时器,记录每一个时间点,随时更新;
	time_t bossLongTime;
	//BOSS状态;
	CC_SYNTHESIZE(TD_BossState,bossState,BossState);
	CC_SYNTHESIZE(int,bossAttackRate,BossAttackRate);

public:
	static TDBossLCA* create(int& attackRate);
	bool init(int& attackRate);
	void initBossLCA();
	void bossUpdate(float t);
	void sleeP();
	void attack();
	void attackDelay(float t);
	void attackEnd();
	void died();
	void diedDelay(float t);
	void diedEnd();
};
//SOLDIER;
class TDSoldierLCA : public Node
{
public:
	TDSoldierLCA();
	~TDSoldierLCA();
	long soldierAttackTimer;	//生命计时器,记录每一个时间点,随时更新;
	time_t soldierLongTime;
	//士兵状态;
	CC_SYNTHESIZE(TD_SoldierState,soldierState,SoldierState);
	CC_SYNTHESIZE(int,soldierAttackRate,SoldierAttackRate);

public:
	static TDSoldierLCA* create(int& attackRate);
	bool init(int& attackRate);
	void initSoldierLCA();
	void soldierUpdate(float t);
	void run();
	//攻击;
	void attack();
	void attackDelay(float t);
	void attackEnd();
	void died();
	void diedDelay(float t);
	void diedEnd();

};

#endif