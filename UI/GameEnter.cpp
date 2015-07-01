//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①: ;
//@功能描述②: ;
//@功能描述③: ;
//**************************************************************

#include "GameEnter.h"
#include "GameUIData.h"
#include "GameMain.h"
#include "GameSceneState.h"
#include "GameMusicControl.h"

GameEnter :: GameEnter()
{
}

GameEnter :: ~GameEnter()
{
}

Scene* GameEnter :: createScene()
{
    do {
        Scene* scene = Scene :: create();
        CC_BREAK_IF(!scene);

        GameMusicControl::getInstance()->musicOn();
        
        GameEnter* pLayer = GameEnter :: create();
        CC_BREAK_IF(!pLayer);
        scene->addChild(pLayer);
        
        return scene;
    } while (false);
    log("Fun GameEnter :: createScene() ERROR ");
    return nullptr;
}

bool GameEnter :: init()
{
    do {
        CC_BREAK_IF(!Layer::init());
        Sprite* bg = Sprite :: create(RESOURCE("logo/logo.png"));
		bg->setAnchorPoint(Vec2::ZERO);
		bg->setPosition(Vec2::ZERO);
		this->addChild(bg);
        
        Sprite* loading = Sprite::create(RESOURCE("logo/loading.png"));
        loading->setAnchorPoint(Vec2(0.5,0.5));
        loading->setPosition(Vec2(GLB_SIZE.width*0.5,200));
        this->addChild(loading,Z_Second);
        loading->runAction(RepeatForever::create(RotateBy::create(1,360)));
        
        GameMusicControl::getInstance()->preloadAll();
        CallFunc* loadData = CallFunc::create(CC_CALLBACK_0(GameEnter::loadJsonData,this));
        CallFunc* loadarmature = CallFunc::create(CC_CALLBACK_0(GameEnter::loadArmature,this));
        CallFunc* pEnter = CallFunc::create(CC_CALLBACK_0(GameEnter::enterGame,this));
        this->runAction(Sequence::create(loadData,loadarmature,DelayTime::create(0.5f),pEnter,nullptr));
        
        return true;
    } while (false);
    log("Fun GameEnter :: init ERROR");
    return false;
}

void GameEnter :: enterGame()
{
	log("GameEnter :: enterGame ***********");
	SCENE_CHANGE_NORMAL(SceneState::UIGameMain);
}

void GameEnter::loadJsonData()
{
    GameUIData::getInstance()->writeData();
    GameUIData::getInstance()->setNormalMissionProgress(20);
    GameUIData::getInstance()->setChallengeMissionProgress(12);
    GameUIData::getInstance()->setCurNormalMission(1);
    GameUIData::getInstance()->setCurChallengeMission(1);
    GameUIData::getInstance()->readPosData(JsonFileType::NORMALMISSION);
    GameUIData::getInstance()->readPosData(JsonFileType::CHALLENGEMISSION);
    GameUIData::getInstance()->readMissionProgressData(JsonFileType::NORMALMISSION);
    GameUIData::getInstance()->readMissionProgressData(JsonFileType::CHALLENGEMISSION);
    //向Json写入数据，亲测好用;
    //GameUIData::getInstance()->writeMissionProgressData(JsonFileType::NORMALMISSION);
    
	for (int i=1;i<=ci_NormalMissionNum;++i)
	{
		Vec2 temp=GameUIData::getInstance()->getNormalMissionPos(i);
		CCLOG("normal mission %d position x = %f,y = %f",i,temp.x,temp.y);
	}
	for (int i=1;i<=ci_ChallengeMissionNum;++i)
	{
		Vec2 temp=GameUIData::getInstance()->getChallengeMissionPos(i);
		CCLOG("challenge mission %d position x = %f,y = %f",i,temp.x,temp.y);
	}
	log("* >->->D * >->->D * >->->D * >->->D * >->->D *");
	GameUIData::getInstance()->readRoleData();
    GameUIData::getInstance()->initGPS();
}

void GameEnter :: loadArmature()
{
	loadMyArmature();

    ArmatureDataManager::getInstance()->addArmatureFileInfo("Anemoi0.png" , "Anemoi0.plist" , "Anemoi.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Apollo0.png" , "Apollo0.plist" , "Apollo.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Asteria0.png" , "Asteria0.plist" , "Asteria.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Cronus0.png" , "Cronus0.plist" , "Cronus.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Doris0.png" , "Doris0.plist" , "Doris.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Gaea0.png" , "Gaea0.plist" , "Gaea.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Hecate0.png" , "Hecate0.plist" , "Hecate.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("hephaestus0.png" , "hephaestus0.plist" , "hephaestus.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Hades0.png" , "Hades0.plist" , "Hades.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Hades_20.png" , "Hades_20.plist" , "Hades_2.ExportJson");
    
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("nice_effect0.png" , "nice_effect0.plist" , "nice_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("excellent_effect0.png" , "excellent_effect0.plist" , "excellent_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("unbelievable_effect0.png" , "unbelievable_effect0.plist" , "unbelievable_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_xuanzhong0.png" , "effect_xuanzhong0.plist" , "effect_xuanzhong.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_chuizi0.png" , "effect_chuizi0.plist" , "effect_chuizi.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("chuizi_icon0.png" , "chuizi_icon0.plist" , "chuizi_icon.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiawu_icon0.png" , "jiawu_icon0.plist" , "jiawu_icon.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("lingxing_icon0.png" , "lingxing_icon0.plist" , "lingxing_icon.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("mobang_icon0.png" , "mobang_icon0.plist" , "mobang_icon.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("nuqi_icon0.png" , "nuqi_icon0.plist" , "nuqi_icon.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("shizi_icon0.png" , "shizi_icon0.plist" , "shizi_icon.ExportJson");
    
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_suilie0.png" , "effect_suilie0.plist" , "effect_suilie.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_suilie1.png" , "effect_suilie1.plist" , "effect_suilie.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_bingkuaisui0.png" , "effect_bingkuaisui0.plist" , "effect_bingkuaisui.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_bingkuaisui1.png" , "effect_bingkuaisui1.plist" , "effect_bingkuaisui.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("effect_tiechuangsui0.png" , "effect_tiechuangsui0.plist" , "effect_tiechuangsui.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("tongsexiao0.png" , "tongsexiao0.plist" , "tongsexiao.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("blue_liang0.png" , "blue_liang0.plist" , "blue_liang.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("green_liang0.png" , "green_liang0.plist" , "green_liang.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("purple_liang0.png" , "purple_liang0.plist" , "purple_liang.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("red_liang0.png" , "red_liang0.plist" , "red_liang.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("white_liang0.png" , "white_liang0.plist" , "white_liang.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("yellow_liang0.png" , "yellow_liang0.plist" , "yellow_liang.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("hengxiao_effect0.png" , "hengxiao_effect0.plist" , "hengxiao_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiugong01_effect0.png" , "jiugong01_effect0.plist" , "jiugong01_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiugong02_effect0.png" , "jiugong02_effect0.plist" , "jiugong02_effect.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("bushu_ui0.png" , "bushu_ui0.plist" , "bushu_ui.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("guanqiamubiao_ui0.png" , "guanqiamubiao_ui0.plist" , "guanqiamubiao_ui.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("xialuojiantou0.png" , "xialuojiantou0.plist" , "xialuojiantou.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("hudun_effect0.png" , "hudun_effect0.plist" , "hudun_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("buju_effect0.png" , "buju_effect0.plist" , "buju_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("xialuojiantou0.png" , "xialuojiantou0.plist" , "xialuojiantou.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("tsx_eff0.png" , "tsx_eff0.plist" , "tsx_eff.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jvqi_effect0.png" , "jvqi_effect0.plist" , "jvqi_effect.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("map01_line0.png" , "map01_line0.plist" , "map01_line.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("map01_yun0.png" , "map01_yun0.plist" , "map01_yun.ExportJson");
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_zanting0.png" , "ui_zanting0.plist" , "ui_zanting.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_db10.png" , "ui_db10.plist" , "ui_db1.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiafen0.png" , "jiafen0.plist" , "jiafen.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_cgsb010.png" , "ui_cgsb010.plist" , "ui_cgsb01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("djs_010.png" , "djs_010.plist" , "djs_01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_jia5010.png" , "ui_jia5010.plist" , "ui_jia501.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiugong03_effect0.png" , "jiugong03_effect0.plist" , "jiugong03_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("jiugong04_effect0.png" , "jiugong04_effect0.plist" , "jiugong04_effect.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_mbdc010.png" , "ui_mbdc010.plist" , "ui_mbdc01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_diwen10.png" , "ui_diwen10.plist" , "ui_diwen1.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_fs0.png" , "ui_fs0.plist" , "ui_fs.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_fz010.png" , "ui_fz010.plist" , "ui_fz01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_ggjl010.png" , "ui_ggjl010.plist" , "ui_ggjl01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("bgmb_020.png" , "bgmb_020.plist" , "bgmb_02.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("xmap01_stone0.png" , "xmap01_stone0.plist" , "xmap01_stone.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_fz010.png" , "ui_fz010.plist" , "ui_fz01.ExportJson");
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ui_shijianyongjin0.png" , "ui_shijianyongjin0.plist" , "ui_shijianyongjin.ExportJson");

}

void GameEnter::loadMyArmature()
{
    ArmatureDataManager::getInstance()->addArmatureFileInfo(RESOURCE("animature/guang02_effect/guang02_effect0.png"),RESOURCE("animature/guang02_effect/guang02_effect0.plist"),RESOURCE("animature/guang02_effect/guang02_effect.ExportJson"));
    ArmatureDataManager::getInstance()->addArmatureFileInfo(RESOURCE("animature/qiehuan_map/qiehuan_map0.png"),RESOURCE("animature/qiehuan_map/qiehuan_map0.plist"),RESOURCE("animature/qiehuan_map/qiehuan_map.ExportJson"));
    
	ArmatureDataManager::getInstance()->addArmatureFileInfo(RESOURCE("animature/banrenma/banrenma.ExportJson"));
	SpriteFrameCacheHelper::getInstance()->addSpriteFrameFromFile(RESOURCE("animature/banrenma/banrenma0.plist"), RESOURCE("animature/banrenma/banrenma0.png"));
	SpriteFrameCacheHelper::getInstance()->addSpriteFrameFromFile(RESOURCE("animature/banrenma/banrenma1.plist"), RESOURCE("animature/banrenma/banrenma1.png"));
	SpriteFrameCacheHelper::getInstance()->addSpriteFrameFromFile(RESOURCE("animature/banrenma/banrenma2.plist"), RESOURCE("animature/banrenma/banrenma2.png"));
    
    //ArmatureDataManager::getInstance()->addArmatureFileInfo(RESOURCE("animature/xinlong01/xinlong010.png"),RESOURCE("animature/xinlong01/xinlong010.plist"),RESOURCE("animature/xinlong01/xinlong01.ExportJson"));
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo(RESOURCE("animature/shield/shield0.png"),RESOURCE("animature/shield/shield0.plist"),RESOURCE("animature/shield/shield.ExportJson"));
}

