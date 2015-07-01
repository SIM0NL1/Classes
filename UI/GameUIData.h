//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①:UI数据单例;
//@功能描述②:Json读取，存储;
//@功能描述③:数据汇总;
//**************************************************************

#ifndef __GameUIData_H__
#define __GameUIData_H__

#include <iostream>
#include "cocos/platform/CCStdC.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "GameDragonBase.h"
using namespace std;
using namespace rapidjson;
USING_NS_CC;

#include "GameDefine.h"

struct MissionPro
{
    int id;
    int start;
    int score;
    int state;
};

class GameDragonBase;
class GameUIData
{
public:
    GameUIData();
    ~GameUIData();
    
    static GameUIData* m_self;
    static GameUIData* getInstance();
    void deleteInstance();
    void writeData();
    //读取关卡坐标;
    void readPosData(JsonFileType fileType);
    //读取关卡进度;
    void readMissionProgressData(JsonFileType fileType);
    //写入关卡进度数据;
    void writeMissionProgressData(JsonFileType fileType);
    //读取角色数据;
    void readRoleData();
    void setIntegerForKey(string key,int data);
    void setBooleanForKey(string key,bool data);
    void setStringForKey(string key,string data);
    void setFloatForKey(string key,float data);
    Vec2 getNormalMissionPos(int id);
    Vec2 getChallengeMissionPos(int id);
    //获取当前关卡进度;
    MissionPro getMissionProgress(int id,JsonFileType fileType);
    CC_SYNTHESIZE(int,normalMissionProgress,NormalMissionProgress);	//普通关卡总进度;
    CC_SYNTHESIZE(int,curNormalMission,CurNormalMission);	//当前普通关卡;
    CC_SYNTHESIZE(int,challengeMissionProgress,ChallengeMissionProgress);	//副本关卡总进度;
    CC_SYNTHESIZE(int,curChallengeMission,CurChallengeMission);	//当前副本关卡;
    CC_SYNTHESIZE(float,horizontalGps,HorizontalGps);	//水平方线切换地图坐标定位;
    CC_SYNTHESIZE(float,verticalGps,VerticalGps);	//场景切换时地图坐标定位;
    CC_SYNTHESIZE(int,verticalIndex,VerticalIndex);	//场景切换时地图Page定位;
    vector<string> split(string str,string pattern);
    vector<GameDragonBase*> vec_Role;
    void initGPS();
    float getNormalMissionHeight();
    float getChallengeMissionHeight();
private:
    vector<Vec2> missionpos;	//普通关卡坐标;
    vector<Vec2> challengepos;	//挑战关卡坐标;
    vector<MissionPro> _vecNormalPro;	//普通关卡进度;
    vector<MissionPro> _vecChallengePro;	//挑战关卡进度;
};

#endif