﻿//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDStageLayer:Tower Defence Stage Layer;
//@功能描述②: 塔防舞台层，用于展示横向兵攻击BOSS的游戏内容;
//@功能描述③: 此类主要是和GameUILayer相交接的接口类，不会过多暴露逻辑代码;
//**************************************************************

#include "TDStageLayer.h"
#include "TimeMachine.h"
#include "GameMusicControl.h"
#include "GameUIData.h"

TDStageLayer* TDStageLayer::m_self = nullptr;
TDStageLayer::TDStageLayer()
{
	vec_boss.clear();
	vec_soldier.clear();
	m_nBossAdd = 0;
	m_nSoldierAdd = 0;
	m_nBossId = 0;
	m_nSoldierId = -1;
	m_AllBossDead = false;
	target_file.clear();
	target_num.clear();
	target_index.clear();
}

TDStageLayer::~TDStageLayer()
{

}

bool TDStageLayer::init()
{
	do 
	{
		CC_BREAK_IF(!LayerColor::init());
		return true;
	} while (false);
	return false;
}

void TDStageLayer::onEnter()
{
	LayerColor::onEnter();
}

void TDStageLayer::onExit()
{
	LayerColor::onExit();
}

TDStageLayer* TDStageLayer::getInstance()
{
	if (!m_self)
	{
		m_self = new TDStageLayer();
		m_self->init();
		m_self->showUI();
	}
	return m_self;
}

void TDStageLayer::destroyInstance()
{
	if (m_self)
	{
		vec_boss.clear();
		vec_soldier.clear();
		target_file.clear();
		target_num.clear();
		target_index.clear();
		m_self->removeFromParentAndCleanup(true);
		CC_SAFE_DELETE(m_self);
	}
	m_self = nullptr;
}

void TDStageLayer::showUI()
{
	this->setContentSize(Size(GLB_SIZE.width,200));
	this->setPosition(Vec2(0,GLB_SIZE.height*0.5+200));
}

void TDStageLayer::bossFactory(int type)
{
	TDBoss* boss = new TDBoss();
	boss->initAttributeWithIndex(type,m_nBossAdd);
	m_nBossAdd++;
	this->addChild(boss,Z_First);
	boss->setPosition(boss->m_birthPosition);
	vec_boss.push_back(boss);
}

void TDStageLayer::soldierFactory(int score)
{
	if (m_AllBossDead)
	{
		return;
	}
	TDSoldier* soldier = new TDSoldier();
	soldier->setRoleType(1);
	if (score<180 && score>=60)
	{
		soldier->initAttributeWithIndex(1,m_nSoldierAdd);
	}
	else if (score<600 && score>=180)
	{
		soldier->initAttributeWithIndex(2,m_nSoldierAdd);
	}
	else if (score<2000 && score>=600)
	{
		soldier->initAttributeWithIndex(3,m_nSoldierAdd);
	}
	else if (score<5000 && score>=2000)
	{
		soldier->initAttributeWithIndex(4,m_nSoldierAdd);
	}
	else if (score>=5000)
	{
		soldier->initAttributeWithIndex(5,m_nSoldierAdd);
	}
	else
	{
		CCLOG("(O)--+_·_+--(O) score = %d",score);
	}
	m_nSoldierAdd++;
	this->addChild(soldier,Z_First);
	soldier->setPosition(soldier->m_birthPosition);
	vec_soldier.push_back(soldier);
}

void TDStageLayer::wakeOrAttack(int& soldierId,int& hit)
{
	for (int i=0;i<vec_boss.size();i++)
	{
		m_nBossId = i;
		m_nSoldierId = soldierId;
		switch (vec_boss.at(i)->m_bossLCA->getBossState())
		{
		case TD_BossState::Sleep:
			{
				//Boss被唤醒,开始受到伤害,延时几秒后开始攻击士兵;
				//生命值减少;
				vec_boss.at(i)->m_bossLCA->setBossState(TD_BossState::Wake);
				bossGetHit(m_nBossId,hit);
				vec_boss.at(m_nBossId)->wake();
				return;
			}
		case TD_BossState::Wake:
			{
				//生命值减少;
				bossGetHit(m_nBossId,hit);
				return;
			}
		case TD_BossState::Death:
			//vec_boss.at(i)->m_bossLCA->removeFromParentAndCleanup(true);
			break;
		default:
			break;
		}
	}
}

void TDStageLayer::bossGetHit(int& id,int& hit)
{
	//Boss HP减少;
	switch (vec_boss.at(id)->m_nType)
	{
	case 1:
		//有防护盾的Boss;
		{
            if(!vec_boss.at(id)->boss_shield->isVisible())
            {
                //生命值损耗为士兵的攻击力,需要读表;
                vec_boss.at(id)->m_nHP -= hit;
				vec_boss.at(id)->updateHpProgress(vec_boss.at(id)->m_nHP);
                CCLOG("TD--damage = %d  ---Boss Hp  =  %d  ",hit,vec_boss.at(id)->m_nHP);
                if (vec_boss.at(id)->m_nHP <= 0)
                {
                    //干掉Boss,我就是老大,Boss死亡动画;
                    vec_boss.at(id)->m_bossLCA->died();
                    //干掉最后一个Boss,GameOver;
                    if (id+1==vec_boss.size())
                    {
						//游戏胜利,接口;
                        m_AllBossDead = true;
						NotificationCenter::getInstance()->postNotification(csMSG_BOSSDIED,NULL);
                    }
                }
            }
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	default:break;
	}
}

void TDStageLayer::bossAttackBack()
{
	//范围攻击;
	for(auto it=vec_soldier.begin();it!=vec_soldier.end();)
	{
        float posX = (*it)->getPosition().x;
		if ((*it)->getPosition().x>=360)
		{
			(*it)->m_nHP -= 200;
			if ((*it)->m_nHP<0)
			{
				(*it)->m_soldierLCA->died();
				it = vec_soldier.erase(it);
			}
		}
		else
		{
			it++;
		}
	}
}

bool TDStageLayer::isHaveSoldier()
{
	//缺少一个强有力的判断条件，目的是区分此时此刻是否有士兵正在攻击状态;
	//目前容易使人误解的是，士兵死了，Boss还要补一刀，或者士兵刚出现Boss就在挥舞大刀;
	//无外乎两种判断，一种是判断状态，另一种是你判断位置，总之都需要遍历;
	//遍历就会延时;
	bool flag = false;
	if (vec_soldier.size())
	{
		flag = true;
	}
	return flag;
// 	for (unsigned int i=0;i<vec_soldier.size();i++)
// 	{
// 		if (vec_soldier.at(i)->getPosition().y>=360)
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
}

//原设想在时光鸡中每次传递焦点事件;
void TDStageLayer::timerFromFrame(Ref* pSender)
{
	__String* temp = (__String*)pSender;
}

void TDStageLayer::passTarget(string file,int num,int index)
{
	target_file.push_back(file);
	target_num.push_back(num);
	target_index.push_back(index);
}

void TDStageLayer::getTarget(string& file,int& num,int& index,int id)
{
	file = target_file.at(id);
	num = target_num.at(id);
	index = target_index.at(id);
}

int TDStageLayer::getTargetNum()
{
	return static_cast<int>(target_file.size());
}

void TDStageLayer::updateBossTag(int num,int index)
{
	vec_boss.at(m_nBossId)->updateTarget(num,index);
}

void TDStageLayer::targetFinish()
{
    if (vec_boss.at(m_nBossId)->boss_shield->isVisible())
    {
        vec_boss.at(m_nBossId)->boss_shield->setVisible(false);
        //    //干掉Boss,我就是老大,Boss死亡动画;
        //    vec_boss.at(m_nBossId)->m_bossLCA->died();
        //	if (vec_soldier.size())
        //	{
        //		vec_soldier.at(0)->victory();
        //	}
    }
    
}

void TDStageLayer::addSoldierBirthland()
{
	//出兵点动画;
	Sprite* pSpr = Sprite::create();
	pSpr->setAnchorPoint(Vec2(0.5f,0.5f));
	pSpr->setPosition(Vec2(50.f,80.f));
	this->addChild(pSpr,Z_First);
	//-创建动作-;
	Animation* pAniamtion = Animation :: create();
	char strPic[50] = {0};
	//-加载帧-;
	for (int i=1;i<8;i++)
	{
		sprintf(strPic,"animature/chubingdian/chubing_tx0%d.png",i);
		pAniamtion->addSpriteFrameWithFile(RESOURCE(strPic));
	}
	//-加载动作-;
	pAniamtion->setDelayPerUnit(0.2f);
	pAniamtion->setRestoreOriginalFrame(true);
	pAniamtion->setLoops(-1);
	//-创建动画-;
	Animate* pAction = Animate :: create(pAniamtion);
	pSpr->runAction(pAction);
}

void TDStageLayer::getScoreAndStart(int& score,int& start)
{
	//胜利之后需要做的事;
	//1.判断当前关卡是不是进度关卡，如果是则保存进度加一，是不是都刷新关卡数据，注意数据取最优值保存;
	//2.不用管当前正在玩的关卡，这个实在点击按钮时就确定的;
	int nowPlay = GameUIData::getInstance()->getCurNormalMission();
// 	int nowProgress = GameUIData::getInstance()->getNormalMissionProgress();
// 	if ( nowPlay==nowProgress )
// 	{
// 		GameUIData::getInstance()->setNormalMissionProgress(nowProgress+1);
// 	}
	//刷新数据;
	MissionPro temp;
	temp.id = nowPlay;
	temp.start = start;
	temp.score = score;
	temp.state = 1;
	GameUIData::getInstance()->writeMissionProgressData(JsonFileType::NORMALMISSION,nowPlay,temp);
	//
}

