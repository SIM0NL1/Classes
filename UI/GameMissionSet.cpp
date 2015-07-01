//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/03;
//@功能描述①: GameMissionSet.cpp;
//@功能描述②: 从关卡进入游戏，配置数据，配置角色;
//@功能描述③: UI功能类;
//**************************************************************

#include "GameMissionSet.h"
#include "../GameUILayer.h"
#include "../DataCenter.h"
#include "GameUIData.h"
#include "GameMusicControl.h"

GameMissionSet::GameMissionSet()
{
	m_btnStart = nullptr;
	m_btnExit = nullptr;
}

GameMissionSet::~GameMissionSet()
{

}

Scene* GameMissionSet::createScene()
{
	do 
	{
		Scene* scene = Scene :: create();
		CC_BREAK_IF(!scene);
		GameMissionSet* layer = GameMissionSet :: create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
		return scene;
	} while (false);
	return nullptr;
}

bool GameMissionSet::init()
{
	do 
	{
		CC_BREAK_IF(!Layer::init());

		showUI();

		return true;
	} while (false);
	return false;
}

void GameMissionSet::showUI()
{
	Sprite* bg = Sprite::create(RESOURCE("gamemissionset/bgt_01.png"));
	bg->setPosition(Vec2::ZERO);
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg,Z_Back);

	m_btnStart = Button :: create(RESOURCE("gamemissionset/kaishi_01.png"),RESOURCE("gamemissionset/kaishi_02.png"));
	m_btnStart->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnStart->setPosition(Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height*0.5));
	this->addChild(m_btnStart,Z_First);
	m_btnStart->addTouchEventListener(CC_CALLBACK_2(GameMissionSet::BtnCall,this));
	m_btnStart->setTag(GameMissionSetBtnStart);

	m_btnExit = Button :: create(RESOURCE("gamemissionset/gb_01.png"),RESOURCE("gamemissionset/gb_02.png"));
	m_btnExit->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnExit->setPosition(Vec2(GLB_SIZE.width-80.f,GLB_SIZE.height-80.f));
	this->addChild(m_btnExit,Z_First);
	m_btnExit->addTouchEventListener(CC_CALLBACK_2(GameMissionSet::BtnCall,this));
	m_btnExit->setTag(GameMissionSetBtnExit);

}

void GameMissionSet::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	int tag = ((Button*)pSender)->getTag();
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		switch (tag)
		{
			case GameMissionSetBtnStart:
				onBtnStart();
				break;
			case GameMissionSetBtnExit:
				onBtnExit();
				break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void GameMissionSet::onBtnStart()
{
	CCLOG(" Start ");
    GameMusicControl::getInstance()->btnPlay();
    int id = GameUIData::getInstance()->getCurNormalMission();
    DataCenter::getInstance()->initMapInfo(id);
    SCENE_CHANGE_NORMAL(SceneState::DDGameUILayer);
    
}

void GameMissionSet::onBtnExit()
{
	CCLOG(" Exit ");
    GameMusicControl::getInstance()->btnPlay();
	SCENE_CHANGE_NORMAL(SceneState::UIGameMain);
}

