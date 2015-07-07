//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDRoleBase:Tower Defence Role Base;
//@功能描述②: 塔防角色基类;
//@功能描述③: 作为士兵和BOSS的基类，定义一些公共属性和方法;
//**************************************************************

#ifndef __TDRoleBase_H__
#define __TDRoleBase_H__

#include "GameFunctions.h"

class TDRoleBase : public Layout
{
public:
	int m_nId;		//士兵ID，BOSS ID;
	int m_nHP;		//生命值;
	int m_nAttackRate;	//攻击间隔;
	int m_nDPS;		//单次攻击伤害值;
	Sprite* m_self;	//形象;
	Point m_birthPosition;	//出生地;

public:
	TDRoleBase();
	virtual ~TDRoleBase();
	virtual bool init();
};

#endif