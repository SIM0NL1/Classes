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
	m_nUnlockType = -1;
	m_aUnlockCondition.clear();
	m_nTrial = -1;
	m_nDragonLevel = -1;
	m_fImpair = -1;
	m_nSkillId = -1;
	m_nCollect = -1;
	m_sDescribe = "";
	m_nSkillNum = -1;
	m_nUpGradeTotal = -1;
	m_fDiscount = -1;
	m_bIsLock = false;
}

GameDragonBase::~GameDragonBase()
{
	m_aUnlockCondition.clear();
}

void GameDragonBase::initAttribute()
{

}