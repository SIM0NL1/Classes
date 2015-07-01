//
//	GameNormalMission.cpp
//	Author:Simon
//	Date:  2015.5.18
//	本类作为普通关卡节点父类;
//	定义关卡基本属性：继承layout 坐标，状态，动画，按钮，图片，数字;
//

#include "GameNormalMission.h"
#include "GameUIData.h"
#include "GameNormalMap.h"
#include "GameSceneState.h"
#include "GameMusicControl.h"

GameNormalMission::GameNormalMission()
{
	m_btnDizuo = nullptr;
	m_btnStone = nullptr;
	m_labMissionId = nullptr;
	missionStartNum = 0;
	m_dizuoSize = Size::ZERO;
	m_stoneSize = Size::ZERO;
}

GameNormalMission::~GameNormalMission()
{
}

bool GameNormalMission::init()
{
	do 
	{
		CC_BREAK_IF(!Layout::init());

		initMission();

		return true;
	} while (false);
	log("Fun GameNormalMission :: init ERROR");
	return false;
}

void GameNormalMission::initMission()
{
	m_btnDizuo = Button :: create(RESOURCE("stone_dizuo.png"),RESOURCE("stone_dizuo.png"));
	m_btnDizuo->setTag(T_First);
	m_btnDizuo->addTouchEventListener(CC_CALLBACK_2(GameNormalMission::BtnCall,this));
	m_btnDizuo->setAnchorPoint(Vec2(0.5f,0.5f));
	m_dizuoSize = m_btnDizuo->getContentSize();
	m_btnDizuo->setPosition(Vec2(m_dizuoSize.width*0.5,m_dizuoSize.height*0.5));
	this->addChild(m_btnDizuo,Z_First);
	m_btnDizuo->setSwallowTouches(false);

	m_btnStone = Button :: create(RESOURCE("stone_001.png"),RESOURCE("stone_003.png"),RESOURCE("stone_002.png"));
	m_btnStone->setTag(T_Second);
	m_btnStone->addTouchEventListener(CC_CALLBACK_2(GameNormalMission::BtnCall,this));
	m_btnStone->setAnchorPoint(Vec2(0.5f,0.5f));
	m_stoneSize = m_btnStone->getContentSize();
	m_btnStone->setPosition(Vec2(m_dizuoSize.width*0.5,m_dizuoSize.height+15.f));
	this->addChild(m_btnStone,Z_Second);
	m_btnStone->setSwallowTouches(false);

	m_labMissionId = Label::createWithCharMap(RESOURCE("jindu_number.png"),16,25,'0');
	m_labMissionId->setAnchorPoint(Vec2(0.5f,0.5f));
	m_labMissionId->setPosition(Vec2(m_stoneSize.width*0.5,m_stoneSize.height*0.5+20));
	m_btnStone->addChild(m_labMissionId);

}

void GameNormalMission::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	int tag = ((Button*)pSender)->getTag();
	switch (type)
	{
        case Widget::TouchEventType::ENDED:
            {
                Point start = ((Button*)pSender)->getTouchBeganPosition();
                Point end = ((Button*)pSender)->getTouchEndPosition();
                if (fabs(end.y-start.y)<10)
                {
                    if (tag&1)
                    {
                        onBtnDizuo();
                    }
                    else
                    {
                        onBtnStone();
                    }

                }
            }
            break;
        default:break;
	}
}

void GameNormalMission::setBtnTouch(bool flag)
{
	m_btnDizuo->setEnabled(flag);
	m_btnStone->setEnabled(flag);
}

void GameNormalMission::setMissionState(GameMissionState state)
{
	switch (state)
	{
	case GameMissionState::MISSION_CLOSE:
		{
			m_btnStone->setBright(false);
			m_btnStone->setEnabled(false);
			m_btnDizuo->setEnabled(false);
			//m_labMissionId->setPositionY(m_labMissionId->getPositionY()-20.f);
            m_labMissionId->setVisible(false);
			break;
		}
	case GameMissionState::MISSION_NOW:
		{
			missionNow();
			missionOpen();
			break;
		}
	case GameMissionState::MISSION_OPEN:
		{
			missionOpen();
			break;
		}
	default:
		break;
	}
}

void GameNormalMission::onBtnDizuo()
{
	int id = this->getTag();
	log(" %d  GameNormalMission::onBtnDizuo",id);
}

void GameNormalMission::onBtnStone()
{
	int id = this->getTag();
	log(" %d  GameNormalMission::onBtnStone",id);
    GameMusicControl::getInstance()->btnPlay(1);
    GameUIData::getInstance()->setCurNormalMission(id);
	SCENE_CHANGE_FADE(SceneState::UIGameMissionSet);
}

void GameNormalMission::setMissionPorperty(int id)
{
	m_labMissionId->setString(StringUtils::format("%d",id));
	int normalProgress = GameUIData::getInstance()->getNormalMissionProgress();

	if (id < normalProgress)
	{
		setMissionState(GameMissionState::MISSION_OPEN);
	}
	else if (id == normalProgress)
	{
		setMissionState(GameMissionState::MISSION_NOW);
	}
	else
	{
		setMissionState(GameMissionState::MISSION_CLOSE);
	}
}

void GameNormalMission::missionOpen()
{
    m_btnStone->setBright(true);
    m_btnStone->setEnabled(true);
    m_btnDizuo->setEnabled(true);
    int id = this->getTag();
    float delayTime = 2.f;
    delayTime += id%4*0.5;
    float offset = id%10+8;

    Armature* guangEffect = Armature::create("guang02_effect");
    guangEffect->setAnchorPoint(Vec2(0.5f,0.5f));
    guangEffect->setPosition(Vec2(m_dizuoSize.width*0.5,m_dizuoSize.height));
    m_btnDizuo->addChild(guangEffect,Z_First);
    guangEffect->getAnimation()->playWithIndex(0);
    m_btnStone->runAction(RepeatForever::create(Sequence::create(MoveBy::create(delayTime,Vec2(0,offset))
                                                                 ,DelayTime::create(0.2f)
                                                                 ,MoveBy::create(delayTime,Vec2(0,-offset))
                                                                 ,DelayTime::create(0.2f)
                                                                 ,nullptr)));
}

void GameNormalMission::missionNow()
{
	Sprite* halo = Sprite::create(RESOURCE("guang_effect.png"));
	halo->setAnchorPoint(Vec2(0.5f,0.5f));
	halo->setPosition(Vec2(m_stoneSize.width*0.5,m_stoneSize.height*0.5+12));
	m_btnStone->addChild(halo,Z_Back);
	halo->runAction(RepeatForever::create(RotateBy::create(6.f,360.f)));
    halo->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.f,0.6f),ScaleTo::create(1.f,1.f),nullptr)));
}

void GameNormalMission::setMissionStartNum(int missionId,int startNum)
{
	//保存与ID对应关卡的星星数量
}

void GameNormalMission::missionShow(int missionId)
{
	//根据ID获取星星数量

	//设置星星显示;
	static int startNum=1;
	for (int i=1;i<=startNum;++i)
	{
		Sprite* start = Sprite::create(RESOURCE(StringUtils::format("baoshi_00%d.png",i)));
		start->setAnchorPoint(Vec2::ZERO);
		start->setPosition(Vec2::ZERO);
		m_btnDizuo->addChild(start,Z_First);
	}
	++startNum;
	if (startNum>3)
	{
		startNum=1;
	}

}
