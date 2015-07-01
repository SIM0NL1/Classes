//
//  GameDragonBase.h
//  author:Simon
//	
//  Created by 5agame on 15/5/25.
//	角色系统龙的基类;
//

#ifndef __GameDragonBase_H__
#define __GameDragonBase_H__

#include "GameFunctions.h"

class GameDragonBase
{
	//属性;
public:
	int m_nDragonId;				//角色ID
	string m_sDragonChineseName;	//角色中文名;
	string m_sEnglishName;			//角色英文名;
	int m_nDragonType;				//角色类型，基础龙，守护龙;
	int m_nUnlockType;				//解锁类型;
	vector<int> m_aUnlockCondition;	//解锁条件;
	int m_nTrial;					//试用关卡;
	int m_aPrice[2];			//召唤价格,[0]是解锁前(人民币),[1]是解锁后(水晶),-1表示不能召唤，0是正在使用;
	string m_sAttribute;		//属性;
	int m_nDragonLevel;			//角色等级;
	string m_sSkill;			//角色技能;
	float m_fImpair;			//伤害加成;
	int m_nSkillId;				//技能ID
	int m_nSkillType;			//技能类型;
	int m_nCollect;				//技能发动条件，需收集多少个元素;
	string m_sDescribe;			//技能描述;
	int m_nSkillNum;			//技能值，发动技能后有多少元素产生效果;
	int m_nFeed;				//单词喂养消耗;
	int m_nUpGradeTotal;		//升级总共消费，一键满级需要的水晶总数，单次升级是总数/10，一键满级还需*水晶折;
	float m_fDiscount;			//一键满级水晶折扣;
	string m_sAnimate;			//动作文件名;
	string m_sImage;			//形象;
	string m_sIcon;				//角色Icon;

	//二次加载;
	bool m_bIsLock;				//是否解锁
	//伤害值;

	//实例化;
public:
	GameDragonBase();
	virtual ~GameDragonBase();
	void initAttribute();

private:
};

#endif
