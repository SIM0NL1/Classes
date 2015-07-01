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

#include "GameMusicControl.h"

GameMusicControl* GameMusicControl::m_pGameMusicControl = nullptr;
GameMusicControl::GameMusicControl()
{
    soundCCS_id = -1;
    bool_btnSwitch = true;
    BSA = 0;
    g_FlagBgMusic = false;
}

GameMusicControl::~GameMusicControl()
{
    
}

GameMusicControl* GameMusicControl::getInstance()
{
    if (!m_pGameMusicControl)
    {
        m_pGameMusicControl = new GameMusicControl();
    }
    return m_pGameMusicControl;
}

void GameMusicControl::deleteInstance()
{
    if (m_pGameMusicControl)
    {
        delete(m_pGameMusicControl);
    }
    m_pGameMusicControl = NULL;
}

int GameMusicControl::playAll( string file_name,bool loop_flag,float volume_flag,int type )		//-broadcast all files,a public call function of music and sound-
{
    unsigned int ccs_id = -1;
    switch (type)
    {
        case 0:
            SimpleAudioEngine :: getInstance()->playBackgroundMusic(file_name.c_str(),loop_flag);
            SimpleAudioEngine :: getInstance()->setBackgroundMusicVolume(volume_flag);
            break;
        case 1:
            ccs_id = SimpleAudioEngine :: getInstance()->playEffect(file_name.c_str(),loop_flag);
            SimpleAudioEngine :: getInstance()->setEffectsVolume(volume_flag);
            break;
        default:
            break;
    }
    return ccs_id;
}

void GameMusicControl::soundControl( int sound_id,bool loop_flag )	//-sound effects control,interface for everywhere-
{
    if (UserDefault :: getInstance()->getBoolForKey("Mute_Switch",false))	//true则返回，默认是false;
    {
        return;
    }
    //-音效关闭，返回-;
    else if (strcmp((UserDefault::getInstance()->getStringForKey("Sound_Switch")).c_str(),"off") == 0)
    {
        return;
    }
    else
    {
        int soundAccount = UserDefault::getInstance()->getIntegerForKey("SoundAccount",100/4);
        string soundFile = "";
        soundFile = SOUND[sound_id];
        
        if (sound_id == BUTTON_OPEN)
        {
            playAll(soundFile,loop_flag,soundAccount,1);
        }
        else
        {
            soundCCS_id = playAll(soundFile,loop_flag,soundAccount,1);
        }
    }
    
}

void GameMusicControl::volumeControl(float value,int type)	//-the volume control of musics and sound effects-
{
    switch (type)
    {
        case 0:
            SimpleAudioEngine :: getInstance()->setBackgroundMusicVolume(value);
            break;
        case 1:
            SimpleAudioEngine :: getInstance()->setEffectsVolume(value);
            break;
        default:
            break;
    }
}

void GameMusicControl::pauseControl( int type )		//-pause the musics and sound effects-
{
    switch (type)
    {
        case 0:	//-背景音乐-;
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            break;
        case 1:	//-音效-;
            SimpleAudioEngine::getInstance()->pauseEffect(soundCCS_id);
            break;
        default:
            SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
            SimpleAudioEngine::getInstance()->pauseAllEffects();
            break;
    }
}

void GameMusicControl::resumeControl( int type )	//-resume the pause of music and sound-
{
    switch (type)
    {
        case 0:	//-背景音乐-;
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            break;
        case 1:	//-音效-;
            SimpleAudioEngine::getInstance()->resumeEffect(soundCCS_id);
            break;
        default:
            SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
            SimpleAudioEngine::getInstance()->resumeAllEffects();
            break;
    }
}

void GameMusicControl::stopControl( int type )	//-stop music and sound-
{
    switch (type)
    {
        case 0:	//-背景音乐-;
            SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);//-false是暂停-;
            break;
        case 1:	//-音效-;
            SimpleAudioEngine::getInstance()->stopEffect(soundCCS_id);
            break;
        default:
            SimpleAudioEngine::getInstance()->stopAllEffects();
            break;
    }
}

int GameMusicControl::preloadAll()						//-preload sound files-;
{
    //SimpleAudioEngine :: getInstance()->preloadBackgroundMusic(MAP_BACK_MUSIC_PATH.c_str());
    int str_len = sizeof(SOUND)/sizeof(const string);
    static unsigned int i =0;
    if (i == str_len)
    {
        log("=======preload the total num of sound is = %d ========",str_len);
        return -1;
    }
    else
    {
        SimpleAudioEngine::getInstance()->preloadEffect(SOUND[i].c_str());
        ++i;
        return i;
    }
    
}

void GameMusicControl::unloadControl( int id,string file_name)		//-unload music and sound files,should be used with caution-
{
    string soundFile = "";
    switch (id)
    {
        case BUTTON_OPEN:
            soundFile = BUTTON_OPEN_PATH;
        case -1:
            soundFile = file_name;
        default:
            break;
    }
    SimpleAudioEngine::getInstance()->unloadEffect(soundFile.c_str());
}

void GameMusicControl::musicOn(string str_music)						//-music on,broadcast,interface for music-
{
    if (UserDefault :: getInstance()->getBoolForKey("Mute_Switch",false))	//-静音模式开启true则返回-;
    {
        return;
    }
    
    int musicAccount = UserDefault::getInstance()->getIntegerForKey("MusicAccount",100/4);
    UserDefault :: getInstance()->setStringForKey("Music_Switch","on");
    UserDefault :: getInstance()->flush();
    playAll(str_music,true,musicAccount*0.01f*2,0);
}

void GameMusicControl::musicOff()						//-music off-
{
    stopControl(0);
    UserDefault :: getInstance()->setStringForKey("Music_Switch","off");
    UserDefault :: getInstance()->flush();
}

void GameMusicControl::soundOn()						//-sound on,interface for sound(1)-
{
    UserDefault :: getInstance()->setStringForKey("Sound_Switch","on");
    UserDefault :: getInstance()->flush();
}

void GameMusicControl::soundOff()						//-sound off-
{
    UserDefault :: getInstance()->setStringForKey("Sound_Switch","off");
    UserDefault :: getInstance()->flush();
}

void GameMusicControl::btnPlay(int flag)
{
    if (bool_btnSwitch)
    {
        BSA++;
        log("---SECRET NUM %d---",BSA);
        if (flag)
        {
            soundControl(BUTTON_SELECT,false);
        }
        else
            soundControl(BUTTON_OPEN,false);
    }}