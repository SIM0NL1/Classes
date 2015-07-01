//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDSoldier:Tower Defence Soldier;
//@功能描述②: 塔防士兵;
//@功能描述③: 士兵属性:出生点，生命特征，等级(类别)，攻击特征，动画;
//**************************************************************

#ifndef __TDSoldier_H__
#define __TDSoldier_H__

#include "GameFunctions.h"
#include "TDRoleBase.h"
#include "TimeMachine.h"

class TDSoldierLCA;
class TDSoldier : public TDRoleBase
{
public:
	int m_nLevel;	//士兵等级;
	int m_nSummon;	//召唤所需分数;
	TD_SoldierState state;	//士兵状态;
	Armature* m_Armature;	//士兵动画;
	TDSoldierLCA* m_soldierLCA;

public:
	TDSoldier();
	virtual ~TDSoldier();
	//初始化士兵的等级和vector的ID;
	void initAttributeWithIndex(int level,int id);
	//攻击;
	virtual void attack();

	void delayStand();

	void continueAttack();
	void stand(float t);
	void died();
    void victory();
};

#endif