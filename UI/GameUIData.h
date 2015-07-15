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
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"

using namespace std;
using namespace rapidjson;
USING_NS_CC;

#include "GameFunctions.h"
#include "GameDragonBase.h"

//基础变量的校验;
#define json_check_is_bool(value, strKey) (value.HasMember(strKey) && value[strKey].IsBool())
#define json_check_is_string(value, strKey) (value.HasMember(strKey) && value[strKey].IsString())
#define json_check_is_int32(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt())
#define json_check_is_uint32(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint())
#define json_check_is_int64(value, strKey) (value.HasMember(strKey) && value[strKey].IsInt64())
#define json_check_is_uint64(value, strKey) (value.HasMember(strKey) && value[strKey].IsUint64())
#define json_check_is_float(value, strKey) (value.HasMember(strKey) && value[strKey].IsFloat())
#define json_check_is_double(value, strKey) (value.HasMember(strKey) && value[strKey].IsDouble())
#define json_check_is_number(value, strKey) (value.HasMember(strKey) && value[strKey].IsNumber())
#define json_check_is_array(value, strKey) (value.HasMember(strKey) && value[strKey].IsArray())

//得到对应类型的数据，如果数据不存在则得到一个默认值;
#define Json_Check_bool(value, strKey) (json_check_is_bool(value, strKey) && value[strKey].GetBool())
#define Json_Check_string(value, strKey) (json_check_is_string(value, strKey) ? value[strKey].GetString() : "")
#define Json_Check_int32(value, strKey) (json_check_is_int32(value, strKey) ? value[strKey].GetInt() : 0)
#define Json_Check_uint32(value, strKey) (json_check_is_uint32(value, strKey) ? value[strKey].GetUint() : 0)
#define Json_Check_int64(value, strKey) (json_check_is_int64(value, strKey) ? ((value)[strKey]).GetInt64() : 0)
#define Json_Check_uint64(value, strKey) (json_check_is_uint64(value, strKey) ? ((value)[strKey]).GetUint64() : 0)
#define Json_Check_float(value, strKey) (json_check_is_float(value, strKey) ? ((value)[strKey]).GetFloat() : 0)
#define Json_Check_double(value, strKey) (json_check_is_double(value, strKey) ? ((value)[strKey]).GetDouble() : 0)
//得到Value指针;
#define Json_Check_value_ptr(value, strKey) (((value).HasMember(strKey)) ? &((value)[strKey]) : nullptr)

struct MissionPro
{
    int id;
    int start;
    int score;
    int state;
};

//soldierData;
struct SoldierData
{
	int spend;
	int hp;
	int dps;
	int rate;
};

//bossData;
struct BossData
{
	//id,name,w_time,boss_image没什么用,不读;
	int type;
	int shield_1;
	int shield_2;
	int shield_3;
	int hp;
	int rate;
	int dps;
	int range;
	string ai;
	string animation;
	int reward;

};

//
struct ShieldData
{
	int id;		//情况Id;
	int type;	//元素,障碍物;
	int elementId;	//元素Id;
	int num;	//数量;
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
   
    //读取关卡坐标;
    void readPosData(JsonFileType fileType);
    //读取关卡进度;
    void readMissionProgressData(JsonFileType fileType);
    //写入关卡进度数据;
    void writeMissionProgressData(JsonFileType fileType);
    //读取角色数据;
    void readRoleData();
	//读取塔防士兵表;
	void readTDSoldierData();
	//读取塔防Boss表;
	void readTDBossData();
	//读取塔防盾表;
	void readTDShieldData();

	void writeData();
    void setIntegerForKey(const char* key,int value);
	void setLongIntegerForKey(const char* key,unsigned int value);
    void setBooleanForKey(const char* key,bool value);
    void setStringForKey(const char* key,string value);
    void setFloatForKey(const char* key,float value);
	int getIntegerForKey(const char* key);
	int getIntegerForKey(const char* key,int defaultValue);
	unsigned int getLongIntegerForKey(const char* key);
	unsigned int getLongIntegerForKey(const char* key,unsigned int defaultValue);
	bool getBooleanForKey(const char* key);
	bool getBooleanForKey(const char* key,bool defaultValue);
	string getStringForKey(const char* key);
	string getStringForKey(const char* key,string defaultValue);
	float getFloatForKey(const char* key);
	float getFloatForKey(const char* key,float defaultValue);
    void removeDataForKey(const char* key);

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
	SoldierData getSoldierData(int type,int level);
	vector<BossData> getBossData(int missionId);
	ShieldData getShieldData(int id);

private:
    vector<Vec2> missionpos;	//普通关卡坐标;
    vector<Vec2> challengepos;	//挑战关卡坐标;
    vector<MissionPro> _vecNormalPro;	//普通关卡进度;
    vector<MissionPro> _vecChallengePro;	//挑战关卡进度;
	//map<int,map<int,SoldierData>> _mapSoldier;	//每种,每级的士兵数据;
	map<int,SoldierData> _mapSingleSoldier;	//每种,每级的士兵数据;
	map<int,map<int,SoldierData>> _mapAllSoldier;
	vector<BossData> _vecSingleBoss;	//没关的Boss,数量不固定;
	map<int,vector<BossData>> _mapAllBoss;	//所有关卡Boss;
	map<int,ShieldData> _mapShield;		//盾数据;		
};

#endif