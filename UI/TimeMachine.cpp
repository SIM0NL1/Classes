//**********************************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/27;
//@功能描述①: TimeMachine : 不要以为偶是时光鸡！;
//@功能描述②: 时间管理器,用于管理复杂的时间线程,配合更新函数,观察者,一起使用;
//@功能描述③: 塔防的Boss，士兵的生命周期，将来的资源“多线程”加载等等;
//**********************************************************************************

#include "TimeMachine.h"
#include "TDSoldier.h"
#include "TDBoss.h"
#include "GameMusicControl.h"
#include "TDStageLayer.h"

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
	bossState = TD_BossState::None;
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
	setBossState(TD_BossState::Birth);
	initBossLCA();
	return true;
}

void TDBossLCA::initBossLCA()
{
	this->schedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
}

void TDBossLCA::bossUpdate(float t)
{
	TD_BossState state = getBossState();
	switch (state)
	{
	case TD_BossState::None:break;
	case TD_BossState::Birth:
		setBossState(TD_BossState::Sleep);
		break;
	case TD_BossState::Sleep:
		sleeP();
		break;
	case TD_BossState::Wake:
		{
			time(&bossLongTime);
			long tempLifeTime = bossLongTime;
			if (tempLifeTime-bossAttackTimer>=bossAttackRate)
			{
				this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
				attack();
			}
			break;
		}
	case TD_BossState::Wait:
		{
			time(&bossLongTime);
			long tempLifeTime = bossLongTime;
			if (tempLifeTime-bossAttackTimer>=4)
			{
				setBossState(TD_BossState::Sleep);
			}
			else
			{
				bool flag;
				flag = TDStageLayer::getInstance()->isHaveSoldier();
				if (flag)
				{
					setBossState(TD_BossState::Wake);
					attack();
				}
			}
			break;
		}
	case TD_BossState::Death:
		return;
		break;
	default:break;
	}

}

void TDBossLCA::sleeP()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
	//播放沉睡状态的动画;
	((TDBoss*)this->getParent())->m_Armature->getAnimation()->play("nomal",-1,1);
}

void TDBossLCA::attack()
{
	if (this->getBossState()!=TD_BossState::Wake)
	{
		return;
	}
	//播放攻击动画;
	((TDBoss*)this->getParent())->m_Armature->getAnimation()->play("attack",-1,-1);
	GameMusicControl::getInstance()->soundControl(TDBOSS_ATTACK,false);
	this->schedule(CC_SCHEDULE_SELECTOR(TDBossLCA::attackDelay));
}

void TDBossLCA::attackDelay(float t)
{
	if (((TDBoss*)this->getParent())->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::attackDelay));
		attackEnd();
	}
}

void TDBossLCA::attackEnd()
{
	if (this->getBossState()!=TD_BossState::Wake)
	{
		return;
	}
	int type = ((TDBoss*)this->getParent())->m_nType;
	switch (type)
	{
	case 1:
		{
			bool flag;
			TDStageLayer::getInstance()->bossAttackBack();
			time(&bossLongTime);
			bossAttackTimer = bossLongTime;
			flag = TDStageLayer::getInstance()->isHaveSoldier();
			if (!flag)
			{
				((TDBoss*)this->getParent())->m_Armature->getAnimation()->play("angry",-1,1);
				this->setBossState(TD_BossState::Wait);
			}
			this->schedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
			break;
		}	
	case 2:break;
	case 3:break;
	case 4:break;
	case 5:break;
	default:break;
	}
}

void TDBossLCA::died()
{
	setBossState(TD_BossState::Death);
	if( isScheduled(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate)) )
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::bossUpdate));
	}
	//播放死亡动画;
	((TDBoss*)this->getParent())->m_Armature->getAnimation()->play("dead",-1,-1);
	this->schedule(CC_SCHEDULE_SELECTOR(TDBossLCA::diedDelay));
}

void TDBossLCA::diedDelay(float t)
{
	if (((TDBoss*)this->getParent())->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDBossLCA::diedDelay));
		diedEnd();
	}
}

void TDBossLCA::diedEnd()
{
	((TDBoss*)(this->getParent()))->removeFromParentAndCleanup(true);
	this->removeFromParentAndCleanup(true);
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
	soldierState = TD_SoldierState::None;
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
	setSoldierState(TD_SoldierState::Birth);
	initSoldierLCA();
	return true;
}

void TDSoldierLCA::initSoldierLCA()
{
	this->schedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
}

void TDSoldierLCA::soldierUpdate(float t)
{
	TD_SoldierState state = getSoldierState();
	switch (state)
	{
	case TD_SoldierState::None:break;
	case TD_SoldierState::Birth:
		//更新士兵状态;
		setSoldierState(TD_SoldierState::Run);
		break;
	case TD_SoldierState::Run:
		run();
		break;
	case TD_SoldierState::Attack:
		{
			//会每一帧去判断,此时需要判断多少帧是一秒;
			time(&soldierLongTime);
			long tempLifeTime = soldierLongTime;
			//CCLOG("soldier timer = %ld,  now = %ld,  rate = %ld ",soldierAttackTimer,tempLifeTime,soldierAttackRate);
			if (tempLifeTime-soldierAttackTimer>=soldierAttackRate)
			{
				this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
				attack();
			}
		}
		break;
	case TD_SoldierState::Death:
		return;
		break;
	default:break;
	}
}

void TDSoldierLCA::run()
{
	this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
	((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->play("hephaestus_walk",-1,1);
	GameMusicControl::getInstance()->soundControl(TDSOLDIER_RUN,false);
	int id = ((TDSoldier*)(this->getParent()))->m_nId;
	((TDSoldier*)(this->getParent()))->runAction(Sequence::create(MoveBy::create(2.f,Vec2(355-5*(id%4),0))
		,CallFunc::create(CC_CALLBACK_0(TDSoldierLCA::attack,this))
		,nullptr
		));
}

void TDSoldierLCA::attack()
{
	if (this->getSoldierState() == TD_SoldierState::Death)
	{
		return;
	}
	//更新士兵状态;
	setSoldierState(TD_SoldierState::Attack);
	//播放攻击动画;
	((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->play("hephaestus_attack",-1,-1);
	GameMusicControl::getInstance()->soundControl(TDSOLDIER_ATTACK,false);
	this->schedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::attackDelay));
}

void TDSoldierLCA::attackDelay(float t)
{
	if (((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::attackDelay));
		attackEnd();
	}
}

void TDSoldierLCA::attackEnd()
{
	if (this->getSoldierState() == TD_SoldierState::Death)
	{
		return;
	}
	((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->play("hephaestus_nomal",-1,1);
	int id = ((TDSoldier*)(this->getParent()))->m_nId;
	//唤醒Boss或者继续攻击Boss;
	TDStageLayer::getInstance()->wakeOrAttack(id);
	time(&soldierLongTime);
	soldierAttackTimer = soldierLongTime;
	this->schedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
}

void TDSoldierLCA::died()
{
	setSoldierState(TD_SoldierState::Death);
	this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::soldierUpdate));
	((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->play("hephaestus_dead",-1,-1);
	this->schedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::diedDelay));
}

void TDSoldierLCA::diedDelay(float t)
{
	if (((TDSoldier*)(this->getParent()))->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldierLCA::diedDelay));
		diedEnd();
	}
}

void TDSoldierLCA::diedEnd()
{
	((TDSoldier*)(this->getParent()))->removeFromParentAndCleanup(true);
	
}


//-Soldier-End-

////////////////////////////////////////////

