//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/24;
//@功能描述①: TDBoss: Tower Defence Boss;
//@功能描述②: 塔防BOSS;
//@功能描述③: BOSS属性:出生地，生命特征，动画;
//**************************************************************

#include "TDBoss.h"
#include "TDStageLayer.h"
#include "GameMusicControl.h"

TDBoss::TDBoss():m_bossLCA(nullptr),m_BossHpProgress(nullptr)
{
	m_nType = 0;
	m_sName = "";
	m_fwakeTime = 0;
	m_nShieldType = 0;
	m_nShieldNum = 0;
	m_Armature = nullptr;
	boss_shield = nullptr;
	m_labTarget1 = nullptr;
	m_labTarget2 = nullptr;
	m_labTarget3 = nullptr;
	m_ImgTag1 = nullptr;
	m_ImgTag2 = nullptr;
	m_ImgTag3 = nullptr;
	m_fHpBackUp = 0;
}

TDBoss::~TDBoss()
{

}

void TDBoss::initAttributeWithIndex(int type,int id)
{
	m_nType = type;
	m_birthPosition = Point(GLB_SIZE.width-120,100);
	m_nId = id;
	m_nHP = 100;
	m_fHpBackUp = static_cast<float>(m_nHP);
	m_nAttackRate = 1;
	m_nDPS = 200;
	//初始化BossHp;
	Sprite* bg = Sprite::create(RESOURCE("td/ui_hpboss01.png"));
	bg->setPosition(Vec2(0,100.f));
	this->addChild(bg,Z_First);
	m_BossHpProgress = ProgressTimer::create(Sprite::create(RESOURCE("td/ui_hpboss02.png")));//创建一个进程条;
	m_BossHpProgress->setAnchorPoint(Vec2::ZERO);
	m_BossHpProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率;
	m_BossHpProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型;
	m_BossHpProgress->setMidpoint(Point(0,0.5));//设置进度的运动方向;
	m_BossHpProgress->setPosition(Vec2::ZERO);
	bg->addChild(m_BossHpProgress,Z_First);
	m_BossHpProgress->setPercentage(100);

	//初始化Boss类型;
	initType();
	m_bossLCA = TDBossLCA :: create(m_nAttackRate);
	this->addChild(m_bossLCA);
}

void TDBoss::initType()
{
	switch (m_nType)
	{
	case 1://带护盾的Boss;
		{
			m_Armature = Armature::create("banrenma");
			m_Armature->setAnchorPoint(Vec2(0.5f,0.5f));
			this->addChild(m_Armature);
			//护盾;
			boss_shield = Armature::create("shield");
			boss_shield->setAnchorPoint(Vec2(0.5f,0.5f));
			boss_shield->setPosition(Vec2::ZERO);
			this->addChild(boss_shield,Z_Second);
			boss_shield->getAnimation()->play("defence");
			//过关目标;
			initTarget();
		}
		break;
	case 2://不带护盾的Boss;
		{
			m_Armature = Armature::create("banrenma");
			m_Armature->setAnchorPoint(Vec2(0.5f,0.5f));
			this->addChild(m_Armature);
		}
		break;
	case 3://
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}
}

void TDBoss::initTarget()
{
	int type = TDStageLayer::getInstance()->getTargetNum();
	string file = "";
	int num = 0;
	int index = 0;
	string number = "";
	switch (type)
	{
	case 3:
		{
			TDStageLayer::getInstance()->getTarget(file,num,index,2);
			m_ImgTag3 = Sprite::create(file);
			m_ImgTag3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_ImgTag3->setPosition(Vec2(-160,150));
			m_ImgTag3->setScale(0.5f);
			boss_shield->addChild(m_ImgTag3,Z_First);
			number = __String::createWithFormat("%d",num)->getCString();
			m_labTarget3 = LabelAtlas::create(number, "defen_number.png", 16, 22, '0');
			m_labTarget3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_labTarget3->setPosition(Vec2(-120,150));
			boss_shield->addChild(m_labTarget3,Z_First);
		}
	case 2:
		{
			TDStageLayer::getInstance()->getTarget(file,num,index,1);
			m_ImgTag2 = Sprite::create(file);
			m_ImgTag2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_ImgTag2->setPosition(Vec2(-160,200));
			m_ImgTag2->setScale(0.5f);
			boss_shield->addChild(m_ImgTag2,Z_First);
			number = __String::createWithFormat("%d",num)->getCString();
			m_labTarget2 = LabelAtlas::create(number, "defen_number.png", 16, 22, '0');
			m_labTarget2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_labTarget2->setPosition(Vec2(-120,200));
			boss_shield->addChild(m_labTarget2,Z_First);
		}
	case 1:
		{
			TDStageLayer::getInstance()->getTarget(file,num,index,0);
			m_ImgTag1 = Sprite::create(file);
			m_ImgTag1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_ImgTag1->setPosition(Vec2(-160,250));
			m_ImgTag1->setScale(0.5f);
			boss_shield->addChild(m_ImgTag1,Z_First);
			number = __String::createWithFormat("%d",num)->getCString();
			m_labTarget1 = LabelAtlas::create(number, "defen_number.png", 16, 22, '0');
			m_labTarget1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_labTarget1->setPosition(Vec2(-120,250));
			boss_shield->addChild(m_labTarget1,Z_First);
		}
		break;
	default: break;;
	}
}

void TDBoss::updateTarget(int num,int index)
{
	Sprite* finish = nullptr;
	if (!num)
	{
		finish = Sprite :: create("wancheng_ui.png");
		finish->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		GameMusicControl::getInstance()->soundControl(TDFINISH,false);
	}
	string number = __String::createWithFormat("%d",num)->getCString();
	switch (index)
	{
	case 2:
		m_labTarget3->setString(number);
		if (!num)
		{
			m_labTarget3->setVisible(false);
			boss_shield->addChild(finish,Z_First);
			finish->setPosition(m_labTarget3->getPosition());
		}
		break;
	case 1:
		m_labTarget2->setString(number);
		if (!num)
		{
			m_labTarget2->setVisible(false);
			boss_shield->addChild(finish,Z_First);
			finish->setPosition(m_labTarget2->getPosition());
		}
		break;
	case 0:
		m_labTarget1->setString(number);
		if (!num)
		{
			m_labTarget1->setVisible(false);
			boss_shield->addChild(finish,Z_First);
			finish->setPosition(m_labTarget1->getPosition());
		}
		break;
	default:
		break;
	}
}

void TDBoss::wake()
{
	//播放暴怒动画;
	m_Armature->getAnimation()->play("angry",-1,-1);
	this->schedule(CC_SCHEDULE_SELECTOR(TDBoss::wakeDelay));
}

void TDBoss::wakeDelay(float t)
{
	if (m_Armature->getAnimation()->isComplete())
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(TDBoss::wakeDelay));
		wakeEnd();
	}
}

void TDBoss::wakeEnd()
{
	if (m_bossLCA)
	{
		m_bossLCA->attack();
	}
}

void TDBoss::updateHpProgress(int& hp)
{
	m_BossHpProgress->setPercentage(hp*100.f/m_fHpBackUp);
}



