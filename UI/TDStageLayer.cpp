//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDStageLayer:Tower Defence Stage Layer;
//@功能描述②: 塔防舞台层，用于展示横向兵攻击BOSS的游戏内容;
//@功能描述③: 此类主要是和GameUILayer相交接的接口类，不会过多暴露逻辑代码;
//**************************************************************

#include "TDStageLayer.h"
#include "TimeMachine.h"
#include "GameMusicControl.h"

TDStageLayer* TDStageLayer::m_self = nullptr;
TDStageLayer::TDStageLayer()
{
	vec_boss.clear();
	vec_soldier.clear();
	m_nBossAdd = 0;
	m_nSoldierAdd = 0;
	m_nBossId = 0;
	m_nSoldierId = -1;
	m_AllBossDead = false;
	target_file.clear();
	target_num.clear();
	target_index.clear();
}

TDStageLayer::~TDStageLayer()
{

}

bool TDStageLayer::init()
{
	do 
	{
		CC_BREAK_IF(!LayerColor::init());
		vec_boss.clear();
		vec_soldier.clear();
		target_file.clear();
		target_num.clear();
		target_index.clear();
		return true;
	} while (false);
	return false;
}

void TDStageLayer::onEnter()
{
	LayerColor::onEnter();
	//NotificationCenter::getInstance()->addObserver(this,CC_CALLFUNCO_SELECTOR(), csMSG_BOSSACTRATE, nullptr);
	NotificationCenter::getInstance()->addObserver(this,CC_CALLFUNCO_SELECTOR(TDStageLayer::soldierAttackAgain), csMSG_SOLDIERACTRATE, nullptr);
	
}

void TDStageLayer::onExit()
{
	//NotificationCenter::getInstance()->removeObserver(this, csMSG_BOSSACTRATE);
	NotificationCenter::getInstance()->removeObserver(this, csMSG_SOLDIERACTRATE);
	LayerColor::onExit();
}

TDStageLayer* TDStageLayer::getInstance()
{
	if (!m_self)
	{
		m_self = new TDStageLayer();
		m_self->init();
		m_self->showUI();
	}
	return m_self;
}

void TDStageLayer::destroyInstance()
{
	if (m_self)
	{
		vec_boss.clear();
		vec_soldier.clear();
		target_file.clear();
		target_num.clear();
		target_index.clear();
		m_self->removeFromParentAndCleanup(true);
		CC_SAFE_DELETE(m_self);
	}
	m_self = nullptr;
}

void TDStageLayer::showUI()
{
	this->setContentSize(Size(GLB_SIZE.width,200));
	this->setPosition(Vec2(0,GLB_SIZE.height*0.5+200));
}

void TDStageLayer::bossFactory(int type)
{
	TDBoss* boss = new TDBoss();
	boss->initAttributeWithIndex(type,m_nBossAdd);
	m_nBossAdd++;
	this->addChild(boss,Z_First);
	boss->setPosition(boss->m_birthPosition);
	vec_boss.push_back(boss);
}

void TDStageLayer::soldierFactory(int score)
{
	if (m_AllBossDead)
	{
		return;
	}
	TDSoldier* soldier = new TDSoldier();	
	if (score>=60)//&
	{
		soldier->initAttributeWithIndex(1,m_nSoldierAdd);
	}
	m_nSoldierAdd++;
	this->addChild(soldier,Z_First);
	soldier->setPosition(soldier->m_birthPosition);
	vec_soldier.push_back(soldier);
}

void TDStageLayer::wakeOrAttack(int soldierId)
{
	for (int i=0;i<vec_boss.size();i++)
	{
		m_nBossId = i;
		m_nSoldierId = soldierId;
		switch (vec_boss.at(i)->state)
		{
		case TD_BossState::Sleep:
			{
				//Boss被唤醒,开始受到伤害,延时几秒后开始攻击士兵;
				vec_boss.at(m_nBossId)->state = TD_BossState::Wake;
				//生命值减少;
				bossGetHit(m_nBossId);
				//延时时间需要读表,每次Boss唤醒延时,自此Boss开始反击;
				this->runAction(Sequence::create(DelayTime::create(1.f),CallFunc::create(CC_CALLBACK_0(TDStageLayer::bossAttackAnimation,this)),nullptr));
				this->schedule(CC_SCHEDULE_SELECTOR(TDStageLayer::bossTimerPerFrame));
				return;
			}
		case TD_BossState::Wake:
			{
				//生命值减少;
				bossGetHit(m_nBossId);
				//Boss按照自己的攻击频率攻击;
				this->runAction(Sequence::create(DelayTime::create(2.f),CallFunc::create(CC_CALLBACK_0(TDStageLayer::bossAttackAnimation,this)),nullptr));
				break;
			}
		case TD_BossState::Death:
			break;
		default:
			break;
		}
	}
}

void TDStageLayer::wakeOrAttack()
{

}

void TDStageLayer::bossGetHit(int i)
{
	//Boss HP减少;
	switch (vec_boss.at(i)->m_nType)
	{
	case 1:
		//普通Boss;
		{
			if (vec_boss.at(i)->m_nHP-10>0)
			{
				//生命值损耗为士兵的攻击力,需要读表;
				vec_boss.at(i)->m_nHP -= 10;
				CCLOG("TD-----Boss Hp  =  %d  ",vec_boss.at(i)->m_nHP);
			}
			else
			{
				vec_boss.at(i)->m_nHP -= 10;
				vec_boss.at(i)->state = TD_BossState::Death;
				//干掉Boss,我就是老大,Boss死亡动画;
				vec_boss.at(i)->death();
				this->schedule(CC_SCHEDULE_SELECTOR(TDStageLayer::killBoss));
				//干掉最后一个Boss,GameOver;
				if (i+1==vec_boss.size())
				{
					//游戏胜利,接口;
					m_AllBossDead = true;
 					NotificationCenter::getInstance()->postNotification(csMSG_BOSSDIED,NULL);

				}
			}
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:break;
	}
}

void TDStageLayer::bossAttackAnimation()
{
	if (vec_boss.at(m_nBossId)->state == TD_BossState::Death)
	{
		//移除Boss;
		vec_boss.at(m_nBossId)->removeFromParentAndCleanup(true);
		return;
	}
	else if (vec_soldier.at(m_nSoldierId)->m_nHP<0)
	{
		//移除士兵;
		//vec_soldier.at(m_nSoldierId)->died();
		return;
	}
	else 
	{
		//Boss攻击动画;
		vec_boss.at(m_nBossId)->attack();
		GameMusicControl::getInstance()->soundControl(TDBOSS_ATTACK,false);
		this->schedule(schedule_selector(TDStageLayer::bossAttackBack));
	}
}

void TDStageLayer::bossAttackBack(float t)
{
	//Boss反击;
	if (vec_boss.at(m_nBossId)->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDStageLayer::bossAttackBack));
		vec_soldier.at(m_nSoldierId)->m_nHP -= 200;
		if (vec_soldier.at(m_nSoldierId)->m_nHP<0)
		{
			vec_soldier.at(m_nSoldierId)->state = TD_SoldierState::Death;
			//播放士兵死亡动画;
			vec_soldier.at(m_nSoldierId)->m_Armature->getAnimation()->play("hephaestus_dead");
			//移除士兵;
			this->schedule(CC_SCHEDULE_SELECTOR(TDStageLayer::killSoldier));
		}
	}
}

void TDStageLayer::soldierAttackAgain(Ref* pSender)
{
	if (vec_soldier.at(m_nSoldierId)->state == TD_SoldierState::Death)
	{
		//vec_soldier.at(m_nSoldierId)->m_soldierLCA->soldierDied();
		//移除士兵;
		//vec_soldier.at(m_nSoldierId)->died();
		return;
	}
	else if(vec_boss.at(m_nBossId)->state == TD_BossState::Death)
	{
		return;
	}
	else
	{
		vec_soldier.at(m_nSoldierId)->continueAttack();
	}
}

void TDStageLayer::bossTimerPerFrame(float t)
{
	time_t long_time;;
	time(&long_time);
	static long sleepTimer = long_time;

	for (int i=0;i<vec_soldier.size();i++)
	{
		if (vec_soldier.at(i)->state == TD_SoldierState::Attack)
		{
			sleepTimer = long_time;
			return;
		}
	}
	//2秒内Boss睡眠;
	if (long_time-2>=sleepTimer)
	{
		vec_boss.at(m_nBossId)->state = TD_BossState::Sleep;
		//Boss保持初始动作;
		vec_boss.at(m_nBossId)->m_Armature->getAnimation()->play("nomal");
		this->unschedule(CC_SCHEDULE_SELECTOR(TDStageLayer::bossTimerPerFrame));
	}
}

void TDStageLayer::killBoss(float t)
{
	if (vec_boss.at(m_nBossId)->m_Armature->getAnimation()->isComplete())
	{
		//移除Boss;
		vec_boss.at(m_nBossId)->removeFromParentAndCleanup(true);
		this->unschedule(schedule_selector(TDStageLayer::killBoss));
	}
}

void TDStageLayer::killSoldier(float t)
{
	if (vec_soldier.at(m_nSoldierId)->m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(schedule_selector(TDStageLayer::killSoldier));
		//移除士兵;
		vec_soldier.at(m_nSoldierId)->died();
		for (int i=m_nSoldierId;i>=0;i--)
		{
			vec_soldier.at(i)->removeFromParentAndCleanup(true);
		}
	}
}

//原设想在时光鸡中每次传递焦点事件;
void TDStageLayer::timerFromFrame(Ref* pSender)
{
	__String* temp = (__String*)pSender;
}

void TDStageLayer::passTarget(string file,int num,int index)
{
	target_file.push_back(file);
	target_num.push_back(num);
	target_index.push_back(index);
}

void TDStageLayer::getTarget(string& file,int& num,int& index,int id)
{
	file = target_file.at(id);
	num = target_num.at(id);
	index = target_index.at(id);
}

int TDStageLayer::getTargetNum()
{
	return target_file.size();
}

void TDStageLayer::updateBossTag(int num,int index)
{
	vec_boss.at(m_nBossId)->updateTarget(num,index);
}

void TDStageLayer::targetFinish()
{
    vec_boss.at(m_nBossId)->state = TD_BossState::Death;
    //干掉Boss,我就是老大,Boss死亡动画;
    vec_boss.at(m_nBossId)->death();
    this->schedule(CC_SCHEDULE_SELECTOR(TDStageLayer::killBoss));
    if (vec_soldier.size()>0 && vec_soldier.at(m_nSoldierId)->state!=TD_SoldierState::Death)
    {
        vec_soldier.at(m_nSoldierId)->victory();
    }
}

void TDStageLayer::addSoldierBirthland()
{
    //出兵点动画;
    Sprite* pSpr = Sprite::create();
    pSpr->setAnchorPoint(Vec2(0.5f,0.5f));
    pSpr->setPosition(Vec2(50.f,80.f));
    this->addChild(pSpr,Z_First);
    //-创建动作-;
    Animation* pAniamtion = Animation :: create();
    char strPic[50] = {0};
    //-加载帧-;
    for (int i=1;i<8;i++)
    {
        sprintf(strPic,"animature/chubingdian/chubing_tx0%d.png",i);
        pAniamtion->addSpriteFrameWithFile(RESOURCE(strPic));
    }
    //-加载动作-;
    pAniamtion->setDelayPerUnit(0.2f);
    pAniamtion->setRestoreOriginalFrame(true);
    pAniamtion->setLoops(-1);
    //-创建动画-;
    Animate* pAction = Animate :: create(pAniamtion);
    pSpr->runAction(pAction);
}
