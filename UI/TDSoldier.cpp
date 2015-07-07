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

TDSoldier::TDSoldier():m_soldierLCA(nullptr)
{
    m_nLevel = 0;
    m_nSummon = 0;
    m_Armature = nullptr;
	roletype = 1;
}

TDSoldier::~TDSoldier()
{
    
}

void TDSoldier::initAttributeWithIndex(int level,int id)
{
    m_nLevel = level;
    m_birthPosition = Point(20,0);
    m_nId = id;
    m_nHP = 100;
    m_nAttackRate = 2;
    m_nDPS = 20;

	int type = getRoleType();
	//根据不同的角色,分配不同的士兵形态,后期需要统一处理;
	string name = GameFunctions::getInstance()->soldierTypeName[type-1][level-1];

	m_Armature = Armature::create(name);//"soldier_Type1_Level5"
	m_Armature->setAnchorPoint(Vec2::ZERO);
	m_Armature->setScale(1+0.1*(level-1));
	this->addChild(m_Armature);
	m_soldierLCA = TDSoldierLCA::create(m_nAttackRate);
	this->addChild(m_soldierLCA);
}

void TDSoldier::victory()
{
    m_Armature->getAnimation()->play("hephaestus_win");
}

