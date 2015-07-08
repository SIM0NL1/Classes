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

class Progress;
class TDBossLCA;
class TDBoss : public TDRoleBase
{
public:
	int m_nType;	//BOSS类型:普通BOSS,带盾的BOSS,防御塔,门,宝箱;
	string m_sName;	//BOSS名字;
	float m_fwakeTime;	//BOSS受到攻击后，多长时间进行反击;
	int m_nShieldType;	//BOSS盾牌类型;
	int m_nShieldNum;	//BOSS盾牌值;
	Armature* m_Armature;	//BOSS动画;
	Armature* boss_shield;	//盾;
	TDBossLCA* m_bossLCA;	//Boss生命周期;
	LabelAtlas *m_labTarget1,*m_labTarget2,*m_labTarget3;
	Sprite *m_ImgTag1,*m_ImgTag2,*m_ImgTag3;
	float m_fHpBackUp;
	ProgressTimer* m_BossHpProgress;

public:
	TDBoss();
	virtual ~TDBoss();
	//初始化Boss的类型,Boss的vector的ID;
	void initAttributeWithIndex(int type,int id);
	void wake();
	void wakeDelay(float t);
	void wakeEnd();
	void initType();
	void initTarget();
	void updateTarget(int num,int index);
	void updateHpProgress(int& hp);
};

#endif