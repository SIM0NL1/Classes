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
#include "GameUIData.h"

extern GameMissionSet* g_pGameMissionSet = nullptr;
GameMissionSet::GameMissionSet():m_btnStart(nullptr),m_btnExit(nullptr),m_labHpCoin(nullptr),m_labTimer(nullptr)
{
	m_nHp = -1;
	long_time = 0;
	g_pGameMissionSet = this;
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

	m_nHp = GameUIData::getInstance()->getIntegerForKey(cs_CurUserHp.c_str(),60);
	m_labHpCoin = Label::createWithCharMap(RESOURCE("shuliang_number.png"),16,27,'0');
	m_labHpCoin->setAnchorPoint(Vec2(0.5f,0.5f));
	m_labHpCoin->setPosition(Vec2(m_labHpCoin->getContentSize().width*0.5+80.f,GLB_SIZE.height-m_labHpCoin->getContentSize().height-80.f));
	m_labHpCoin->setString(__String::createWithFormat("%d:60",m_nHp)->getCString());
	this->addChild(m_labHpCoin,Z_First);

	m_labTimer = Label :: createWithSystemFont("00:00","Arial",16);
	m_labTimer->setAnchorPoint(Vec2(0.5f,0.5f));
	m_labTimer->setPosition(Vec2(m_labHpCoin->getContentSize().width*0.5+40.f,GLB_SIZE.height-m_labHpCoin->getContentSize().height-100.f));
	this->addChild(m_labTimer,Z_First);

	if (m_nHp==60)
	{
		m_labTimer->setVisible(false);
	}
	else
	{
		startHpTimer();
	}
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
	m_nHp = GameUIData::getInstance()->getIntegerForKey(cs_CurUserHp.c_str());
    if (m_nHp>=5)
    {
		GameMusicControl::getInstance()->btnPlay();
		reduceHp();
		int id = GameUIData::getInstance()->getCurNormalMission();
		DataCenter::getInstance()->initMapInfo(id);
		SCENE_CHANGE_NORMAL(SceneState::DDGameUILayer);
    }
	else
	{
		MessageBox("体力不足","提示");
		return;
	}
}

void GameMissionSet::onBtnExit()
{
    GameMusicControl::getInstance()->btnPlay();
	SCENE_CHANGE_FADE(SceneState::UIGameMain);
}



void GameMissionSet::onEnter()
{
	Node::onEnter();
}

void GameMissionSet::onExit()
{
	Node::onExit();
}

void GameMissionSet::reduceHp()
{
	//扣除5点体力值;
	m_nHp -= 5;
	m_labHpCoin->setString(__String::createWithFormat("%d:60",m_nHp)->getCString());
	GameUIData::getInstance()->setIntegerForKey(cs_CurUserHp.c_str(),m_nHp);
	if (!this->isScheduled(CC_SCHEDULE_SELECTOR(GameMissionSet::updateHp)))
	{
		m_nMin = 5;
		m_nSec = 60;
		m_labTimer->setVisible(true);
		this->schedule(CC_SCHEDULE_SELECTOR(GameMissionSet::updateHp));
	}
}

void GameMissionSet::startHpTimer()
{
	time_t t;
	time(&t);
	unsigned int now = t;	//本次系统时间;
	unsigned int previous =  GameUIData::getInstance()->getLongIntegerForKey(cs_PreHpTimer.c_str(),now);	//上次系统时间;
	//刷新时间;
	m_nHp = (m_nHp+(now-previous)/ci_HpSecond)>60 ? 60 : (m_nHp+(now-previous)/ci_HpSecond);
	GameUIData::getInstance()->setIntegerForKey(cs_CurUserHp.c_str(),m_nHp);
	m_labHpCoin->setString(__String::createWithFormat("%d:60",m_nHp)->getCString());
	int hp = ci_HP_MAX-m_nHp;

	if (now-previous>=hp*ci_HpSecond)	//已经回复满;
	{
		m_labTimer->setVisible(false);
	}
	else
	{
		long remainder = hp*ci_HpSecond-(now-previous);	//满状态剩余秒数;
		m_nMin = (remainder/60)%6;	//剩余倒计时分钟数;
		m_nSec = remainder%60;		//剩余倒计时秒数;
		if (!m_nSec && !m_nMin)
		{
			m_nMin = 5;
			m_nSec = 60;
		}
		this->schedule(CC_SCHEDULE_SELECTOR(GameMissionSet::updateHp));
	}
}

void GameMissionSet::updateHp(float t)
{
	time_t longTime;
	time(&longTime);
	unsigned int temp = longTime;
	m_nHp = GameUIData::getInstance()->getIntegerForKey(cs_CurUserHp.c_str());
	if (temp-long_time >= 1)//1
	{
		long_time = temp;
		--m_nSec;
		if (m_nSec<0)
		{
			--m_nMin;
			if (m_nMin<0)
			{
				GameUIData::getInstance()->setIntegerForKey(cs_CurUserHp.c_str(),m_nHp+1);
				//记录;
				GameUIData::getInstance()->setLongIntegerForKey(cs_PreHpTimer.c_str(),temp);

				m_labHpCoin->setString(__String::createWithFormat("%d:60",m_nHp)->getCString());
				if (m_nHp+1==60)
				{
					m_nMin = 0;
					m_nSec = 0;
					this->unschedule(CC_SCHEDULE_SELECTOR(GameMissionSet::updateHp));
					m_labTimer->setVisible(false);
					return;
				}
				else
				{
					m_nMin = 5;
				}
			}
			m_nSec = 59;
		}

		if (m_nSec<10)
		{
			m_labTimer->setString(__String::createWithFormat("0%d : 0%d",m_nMin,m_nSec)->getCString());
		}
		else
		{
			m_labTimer->setString(__String::createWithFormat("0%d : %d",m_nMin,m_nSec)->getCString());
		}
	}
}
