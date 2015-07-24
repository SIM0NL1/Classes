//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①:;
//@功能描述②:;
//@功能描述③:;
//**************************************************************

#include "GameDragonBase.h"

GameDragonBase::GameDragonBase()
{
	m_nDragonId = -1;
	m_sDragonChineseName = "";
	m_sEnglishName = "";
	m_nDragonType = -1;
	m_nUnlockType = -1;
	m_aUnlockCondition.clear();
	m_nTrial = -1;
	m_sAttribute = "";
	m_nDragonLevel = -1;
	m_sSkill = "";
	m_fImpair = -1;
	m_nSkillId = -1;
	m_nSkillType = -1;
	m_nCollect = -1;
	m_sDescribe = "";
	m_nSkillNum = -1;
	m_nFeed = -1;
	m_nUpGradeTotal = -1;
	m_fDiscount = -1;
	m_sAnimate = "";
	m_sImage = "";
	m_sIcon = "";
	m_bIsLock = false;
}

GameDragonBase::~GameDragonBase()
{

}

void GameDragonBase::initAttribute()
{

}