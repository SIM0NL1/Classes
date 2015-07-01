/*************************************************
 Copyright (C), 2015 - 2015, 5agame Software, Ltd.
 Author: Simon (Create)
 Created: Jun. 19, 2015
 Updated:
 /******************!NO DELETE!******************/
/***********************************************************************
 *接口类（GameMusicControl）
 *该类作为项目音乐和音效控制的接口类;
 *该类会提供音乐和音效的预加载，播放、暂停、释放控制，音量控制;
 *该类存放一些全局的文件宏定义，文件路径;
 *这是一个工具类;
 ***********************************************************************/

#ifndef __GAME_MUSIC_CONTROL_H__
#define __GAME_MUSIC_CONTROL_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

#include "GameFunctions.h"

enum COMMON_SOUND_ID
{
    //-音效预加载的ID-//-枚举必须和路径数组的下标相对应-;
    BUTTON_OPEN = 0,
    BUTTON_SELECT,
	TDBOSS_ATTACK,
	TDSOLDIER_ATTACK,
	TDSOLDIER_RUN,
	TDFINISH,
};
// 音乐和音效的路径;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	//-安卓平台;
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	//-WIN32平台-
//#endif
//-音乐文件-;
const string MAP_BACK_MUSIC_PATH = RESOURCE("music/bigmap.mp3");// 大地图背景音乐;
const string CMAP_BACK_MUSIC_PATH = RESOURCE("music/bigmaptx.mp3");// 大地图背景音乐;

//-音效文件-;
const string BUTTON_OPEN_PATH = RESOURCE("sound/uiopen.mp3");
const string BUTTON_SELECT_PATH = RESOURCE("sound/uiselect.mp3");
const string TDBOSS_ATTACK_PATH = RESOURCE("sound/attact_boss.mp3");
const string TDSOLDIER_ATTACK_PATH = RESOURCE("sound/attact_level1.mp3");
const string TDSOLDIER_RUN_PATH = RESOURCE("sound/run.mp3");
const string TDFINISH_PATH = RESOURCE("sound/finish.mp3");

//-音效预加载使用的string数组,和枚举类型相对应-;
static string const SOUND[] =
{
    BUTTON_OPEN_PATH,
    BUTTON_SELECT_PATH,
	TDBOSS_ATTACK_PATH,
	TDSOLDIER_ATTACK_PATH,
	TDSOLDIER_RUN_PATH,
	TDFINISH_PATH,
};

class GameMusicControl
{
private:
    static GameMusicControl* m_pGameMusicControl;
    GameMusicControl();
    ~GameMusicControl();
    bool bool_btnSwitch;
    
public:
    static GameMusicControl* getInstance();
    static void deleteInstance();
    unsigned int BSA;		//按钮点击次数;
    int soundCCS_id;		//记录当前音效ID;
    int preloadAll();		//-预加载-;
    //-播放所有音乐和音效，参数1：文件路径，参数2：是否循环，参数3：音量大小，参数4：音乐还是音效-
    int playAll(string file_name,bool loop_flag,float volume_flag,int type);
    void soundControl(int sound_id,bool loop_flag);		//-音效控制-;
    void volumeControl(float value,int type);			//-音量控制-;
    void pauseControl(int type);	//-暂停-0:背景音乐，1:音效，2:全部音效-;
    void resumeControl(int type);	//-继续-0:背景音乐，1:音效，2:全部音效-;
    void stopControl(int type);		//-停止-0:背景音乐，1:音效，2:全部音效-;
    void unloadControl(int id,string file_name="");		//-卸载-;
    void musicOn(string str_music = MAP_BACK_MUSIC_PATH);		//-背景音乐开-;
    void musicOff();	//-背景音乐关-;
    void soundOn();		//-音效开-;
    void soundOff();	//-音效关-;
    void btnPlay(int flag=0);
    bool g_FlagBgMusic;             //用于标记当前是否转换背景音乐;
    
protected:
    
};
#endif