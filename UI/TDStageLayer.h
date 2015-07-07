//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDStageLayer:Tower Defence Stage Layer;
//@功能描述②: 塔防舞台层，用于展示横向兵攻击BOSS的游戏内容;
//@功能描述③: 此类主要是和GameUILayer相交接的接口类，不会过多暴露逻辑代码;
//**************************************************************

#ifndef __TDStageLayer_H__
#define __TDStageLayer_H__

#include "GameFunctions.h"
#include "TDBoss.h"
#include "TDSoldier.h"

class TDStageLayer : public LayerColor
{
public:
    vector<TDBoss*> vec_boss;
    vector<TDSoldier*> vec_soldier;
    int m_nBossAdd,m_nSoldierAdd;
    int m_nBossId,m_nSoldierId;
    bool m_AllBossDead;
    vector<string> target_file;
    vector<int> target_num;
    vector<int> target_index;
    
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static TDStageLayer* m_self;
    static TDStageLayer* getInstance();
    void destroyInstance();
    void showUI();
	//士兵出生点;
	void addSoldierBirthland();
	//根据过关条件初始化BOSS;
    void bossFactory(int type);
    //根据每次消除得分生产士兵;
    void soldierFactory(int score);
    //唤醒Boss,Boss单体伤害;
    void wakeOrAttack(int soldierId);
    //Boss受到攻击,生命值减少;
    void bossGetHit(int i);
    //Boss反击;
	void bossAttackBack();
	//是否有士兵存在,影响Boss是否沉睡;
	bool isHaveSoldier();
    //Timer焦点事件回调;
    void timerFromFrame(Ref* pSender);
    //初始换过关条件;
    void passTarget(string file,int num,int index);
    void getTarget(string& file,int& num,int& index,int id);
    int getTargetNum();
    void updateBossTag(int num,int index);
    void targetFinish();
    
private:
    TDStageLayer();
    virtual ~TDStageLayer();
};

#endif