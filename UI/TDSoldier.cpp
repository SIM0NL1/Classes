//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDSoldier:Tower Defence Soldier;
//@功能描述②: 塔防士兵;
//@功能描述③: 士兵属性:出生点，生命特征，等级(类别)，攻击特征，动画;
//**************************************************************

#include "TDSoldier.h"
#include "TDStageLayer.h"
#include "GameMusicControl.h"

TDSoldier::TDSoldier()
{
    m_nLevel = 0;
    m_nSummon = 0;
    state = TD_SoldierState::Birth;
    m_Armature = nullptr;
    m_soldierLCA = nullptr;
}

TDSoldier::~TDSoldier()
{
    
}

void TDSoldier::initAttributeWithIndex(int level,int id)
{
    m_nLevel = level;
    m_birthPosition = Point(50,50);
    m_nId = id;
    m_nHP = 100;
    m_fInterval = 2;
    m_nDPS = 20;
    
    //更新士兵状态;
    state = TD_SoldierState::Run;
    m_Armature = Armature::create("hephaestus");
    m_Armature->setAnchorPoint(Vec2(0.5f,0.5f));
    m_Armature->getAnimation()->play("hephaestus_walk");
    this->addChild(m_Armature);
    GameMusicControl::getInstance()->soundControl(TDSOLDIER_RUN,false);
    m_Armature->runAction(Sequence::create(MoveBy::create(2.f,Vec2(360,0))
                                           ,CallFunc::create(CC_CALLBACK_0(TDSoldier::attack,this))
                                           ,nullptr
                                           ));
}

void TDSoldier::attack()
{
    //更新士兵状态;
    state = TD_SoldierState::Attack;
    //播放攻击动画;
    m_Armature->getAnimation()->play("hephaestus_attack");
    GameMusicControl::getInstance()->soundControl(TDSOLDIER_ATTACK,false);
    this->runAction(Sequence::create(DelayTime::create(0.1f),CCCallFunc::create(CC_CALLBACK_0(TDSoldier::delayStand,this)),nullptr));
}

void TDSoldier::delayStand()
{
    //攻击一次然后站立,唤醒Boss，判断是否继续攻击;
    this->schedule(CC_SCHEDULE_SELECTOR(TDSoldier::stand));
}

void TDSoldier::stand(float t)
{
    if (m_Armature->getAnimation()->isComplete())
    {
        m_Armature->getAnimation()->play("hephaestus_nomal");
        this->unschedule(CC_SCHEDULE_SELECTOR(TDSoldier::stand));
        //唤醒Boss或者继续攻击Boss;
        TDStageLayer::getInstance()->wakeOrAttack(m_nId);
        //延时之后,继续攻击;
        int rate = 2;
        m_soldierLCA = TDSoldierLCA::create(rate);
        this->addChild(m_soldierLCA);
        
    }
}

//按照攻击速度进行持续攻击;
void TDSoldier::continueAttack()
{
    m_Armature->getAnimation()->play("hephaestus_attack");
    GameMusicControl::getInstance()->soundControl(TDSOLDIER_ATTACK,false);
    TDStageLayer::getInstance()->wakeOrAttack(m_nId);
}

void TDSoldier::died()
{
    m_soldierLCA->soldierDied();
    this->removeFromParentAndCleanup(true);
}

void TDSoldier::victory()
{
    m_Armature->getAnimation()->play("hephaestus_win");
}

