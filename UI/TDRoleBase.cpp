//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDRoleBase:Tower Defence Role Base;
//@功能描述②: 塔防角色基类;
//@功能描述③: 作为士兵和BOSS的基类，定义一些公共属性和方法;
//**************************************************************

#include "TDRoleBase.h"


TDRoleBase::TDRoleBase()
{
	m_nId = 0;
	m_nHP = 0;
	m_nAttackRate = 0;
	m_nDPS = 0;
	m_self = nullptr;
}

TDRoleBase::~TDRoleBase()
{

}

bool TDRoleBase::init()
{
	if (!Layout::init())
	{
		return false;
	}

	return true;
}