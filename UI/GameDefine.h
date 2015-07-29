/*
 Author:Simon
 Date:  2015.5.13
 本类是一个公共头文件类,包含一些公共宏定义,枚举类型;
 */

#ifndef __GameDefine_H__
#define __GameDefine_H__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;

const float PI = (3.1415926f);
const int ci_HP_MAX = 60;		//最大体力上限;
const int ci_MapNum = 5;		//地图张数;
const int ci_NormalMissionNum = 40;		//普通关卡总关数;
const int ci_ChallengeMissionNum = 20;	//特殊关卡总关数;
const unsigned int ci_HpSecond = 360;	//6*60seconds回复一点体力;

//取到屏幕大小;
//const Size WIN_SIZE = Director::getInstance()->getWinSize();	//不知道为什么，总是0

//逻辑大小;
const Size GLB_SIZE = Size(640.f,1136.f);
//坐标比例;
//const float GLBX = WIN_SIZE.width/GLB_SIZE.width;
//const float GLBY = WIN_SIZE.height/GLB_SIZE.height;

//观察者模式焦点事件;
const string csMSG_BOSSDIED = "BossDied";
const string csMSG_HP = "TackOFFHP";
const string csMSG_HPTIMER = "HPTimer";

//安全路径;
#define _JSON_PATH_  FileUtils::getInstance()->getWritablePath().append("userdata.json")
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//const string _JSON_PATH_ = "UI/uidata/userdata.json";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//const string _JSON_PATH_ = "/Users/b081mac/Documents/Project/DragonGame/Resources/UI/uidata/userdata.json";
//#endif

//userData中的key值;
const string cs_CurUserHp = "CurUserHp";	//当前玩家体力值;
const string cs_PreHpTimer = "PreviousHpTimer";	//玩家系统时间标记;

//ZOrder
enum GameZOrder
{
    Z_Back		= -1,
    Z_First		= 1 << 1,
    Z_Second	= 1 << 2,
    Z_Third		= 1 << 3,
    Z_Fourth	= 1 << 4,
    Z_Fifth		= 1 << 5,
};
//Tag
enum GameTag
{
	T_Zero		= 1 << 0,
	T_First		= 1 << 1,
	T_Second	= 1 << 2,
	T_Third		= 1 << 3,
	T_Fourth	= 1 << 4,
	T_Fifth		= 1 << 5,
	T_Sixth		= 1 << 6,
};
//场景切换状态;
enum class SceneState
{
    UIGameNone			= 1 << 0,
    UIGameEnter			= 1 << 1,	//入口;
    UIGameMain			= 1 << 2,	//大地图;
    UIGameMissionSet	= 1 << 3,	//游戏开始之前,关卡配置;
    DDGameUILayer		= 1 << 4,	//游戏界面;
	UIRoleDisplay		= 1 << 5,	//角色界面;
};
//关卡状态;
enum class GameMissionState
{
    MISSION_NOW			= 1 << 0,	//目前关卡;
    MISSION_OPEN		= 1 << 1,	//已开启卡;
    MISSION_CLOSE		= 1 << 2,	//未开启卡;
};
//关卡星级;
enum class GameMissionStart
{
    START_ZERO,
    START_ONE,
    START_TWO,
    START_THREE,
};
//Json文件;
enum class JsonFileType
{
    NORMALMISSION,
    CHALLENGEMISSION,
};
//soldier状态;
enum class TD_SoldierState
{
    None		= 1 << 0,
    Birth		= 1 << 1,
    Run			= 1 << 2,
    Attack		= 1 << 3,
    Death		= 1 << 4,
};
//Boss状态;
enum class TD_BossState
{
    None		= 1 << 0,
    Birth		= 1 << 1,
    Sleep		= 1 << 2,
    Wake		= 1 << 3,
    Wait		= 1 << 4,
    Death		= 1 << 5,
};

//游戏离开方式;
enum class QuitGameType
{
    NONE			= 1 << 0,
    PAUSE			= 1 << 1,
    WinQuit			= 1 << 2,
    WinContinue		= 1 << 3,
    FailQuit		= 1 << 4,
    FailContinue	= 1 << 5,
};

#endif