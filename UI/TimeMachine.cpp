//**********************************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/27;
//@功能描述①: TimeMachine : 不要以为偶是时光鸡！;
//@功能描述②: 时间管理器,用于管理复杂的时间线程,配合更新函数,观察者,一起使用;
//@功能描述③: 塔防的Boss，士兵的生命周期，将来的资源“多线程”加载等等;
//**********************************************************************************

#include "TimeMachine.h"

class TDBossLCA;
class TDSoldierLCA;

TimeMachine* TimeMachine::m_self=nullptr;
TimeMachine::TimeMachine()
{
	longTime = 0;
	long_timer = 0;
}

TimeMachine::~TimeMachine()
{
}

TimeMachine* TimeMachine::getInstance()
{
	if (!m_self)
	{
		m_self = new TimeMachine();
	}
	return m_self;
}

void TimeMachine::destroyMachine()
{
	if (m_self)
	{
		delete(m_self);
	}
	m_self = nullptr;
}

void TimeMachine::startTimer()
{
	//开启计时器;
	this->schedule(CC_SCHEDULE_SELECTOR(TimeMachine::update));
}

void TimeMachine::update(float t)
{
	//向所有分支传递时间;
	time(&longTime);
	long_timer = longTime;
	__String* str = __String::createWithFormat("%ld",long_timer);
	//NotificationCenter::getInstance()->postNotification(csMSG_TIMER,str);
}

void TimeMachine::closeTimer()
{
	//关闭计时器;
	this->unschedule(CC_SCHEDULE_SELECTOR(TimeMachine::update));
}

void TimeMachine::startBossAC(int& attackRate)
{
}

void TimeMachine::startSoldierAC(int& attackRate)
{
}

////////////////////////////////////////////

//-Boss-Begin-
//Boss生命周期;
TDBossLCA::TDBossLCA()
{
	bossAttackTimer = 0;
	bossLongTime = 0;
	bossAttackRate = 0;
}

TDBossLCA::~TDBossLCA()
{

}

TDBossLCA* TDBossLCA::create(int& attackRate)
{
	TDBossLCA* bossSelf = new TDBossLCA();
	if (bossSelf && bossSelf->init(attackRate))
	{
		bossSelf->autorelease();
		return bossSelf;
	}
	CC_SAFE_DELETE(bossSelf);
	return nullptr;
}

bool TDBossLCA::init(int& attackRate)
{
	if (!Node::init())
	{
		return false;
	}
	setBossAttackRate(attackRate);
	initBossLCA();
	return true;
}

void TDBossLCA::initBossLCA()
{
	time(&bossLongTime);
	bossAttackTimer = bossLongTime;
	this->schedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
}

void TDBossLCA::bossUpdate(float t)
{
	//会每一帧去判断,此时需要判断多少帧是一秒;
	time(&bossLongTime);
	long tempLifeTime = bossLongTime;
	if (tempLifeTime-bossAttackTimer>=bossAttackRate)
	{
		//刷新计时点;
		bossAttackTimer = tempLifeTime;
		//触发观察者焦点事件;
		NotificationCenter::getInstance()->postNotification(csMSG_BOSSACTRATE,NULL);
	}
}

void TDBossLCA::bossDied()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
}

//-Boss-End-

////////////////////////////////////////////

//-Soldier-Begin-
//士兵生命周期;
TDSoldierLCA::TDSoldierLCA()
{
	soldierAttackTimer = 0;
	soldierLongTime = 0;
	soldierAttackRate = 0;
}

TDSoldierLCA::~TDSoldierLCA()
{

}

TDSoldierLCA* TDSoldierLCA::create(int& attackRate)
{
	TDSoldierLCA* soldierSelf = new TDSoldierLCA();
	if (soldierSelf && soldierSelf->init(attackRate))
	{
		soldierSelf->autorelease();
		return soldierSelf;
	}
	CC_SAFE_DELETE(soldierSelf);
	return nullptr;
}

bool TDSoldierLCA::init(int& attackRate)
{
	if (!Node::init())
	{
		return false;
	}
	setSoldierAttackRate(attackRate);
	initSoldierLCA();
	return true;
}

void TDSoldierLCA::initSoldierLCA()
{
	time(&soldierLongTime);
	soldierAttackTimer = soldierLongTime;
	this->schedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
}

void TDSoldierLCA::soldierUpdate(float t)
{
	//会每一帧去判断,此时需要判断多少帧是一秒;
	time(&soldierLongTime);
	long tempLifeTime = soldierLongTime;
	//CCLOG("soldier timer = %ld,  now = %ld,  rate = %ld ",soldierAttackTimer,tempLifeTime,soldierAttackRate);
	if (tempLifeTime-soldierAttackTimer>=soldierAttackRate)
	{
		//刷新计时点;
		soldierAttackTimer = tempLifeTime;
		//触发观察者焦点事件;
		NotificationCenter::getInstance()->postNotification(csMSG_SOLDIERACTRATE,NULL);
	}
}


void TDSoldierLCA::soldierDied()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
}


//-Soldier-End-

////////////////////////////////////////////

