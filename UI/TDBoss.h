//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDBoss: Tower Defence Boss;
//@功能描述②: 塔防BOSS;
//@功能描述③: BOSS属性:出生地，生命特征，动画;
//**************************************************************

#ifndef __TDBoss_H__
#define __TDBoss_H__

#include "GameFunctions.h"
#include "TDRoleBase.h"
#include "TimeMachine.h"

class TDBossLCA;
class TDBoss : public TDRoleBase
{
public:
	int m_nType;	//BOSS类型:普通BOSS,带盾的BOSS,防御塔,门,宝箱;
	string m_sName;	//BOSS名字;
	float m_fwakeTime;	//BOSS受到攻击后，多长时间进行反击;
	int m_nShieldType;	//BOSS盾牌类型;
	int m_nShieldNum;	//BOSS盾牌值;
	TD_BossState state;	//BOSS状态;
	Armature* m_Armature;	//BOSS动画;
	Armature* boss_shield;	//盾;
	TDBossLCA* m_bossLCA;	//Boss生命周期;
	LabelAtlas *m_labTarget1,*m_labTarget2,*m_labTarget3;
	Sprite *m_ImgTag1,*m_ImgTag2,*m_ImgTag3;

public:
	TDBoss();
	virtual ~TDBoss();
	//初始化Boss的类型,Boss的vector的ID;
	void initAttributeWithIndex(int type,int id);
	virtual void attack();
	void death();
	void defence();
	void angry();
	void initTarget();
	void updateTarget(int num,int index);
};

#endif