//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/21;
//@功能描述①: 角色系统展示界面;
//@功能描述②: 用于角色展示，选择，升级，查看等功能;
//@功能描述③: 场景类，提供切换、调用接口,支持弹窗;
//**************************************************************

#include "RoleDisplay.h"
#include "GameMusicControl.h"

//控件类;
RoleWidget::RoleWidget()
{
	m_roleBg = nullptr;
	m_roleBtn = nullptr;
	m_roleFightBtn = nullptr;
	m_roleFightSpr = nullptr;
}

bool RoleWidget::init()
{
	do 
	{
		CC_BREAK_IF(!Layout::init());
		//底图;
		m_roleBg = Sprite::create(RESOURCE("guangquan002.png"));
		m_roleBg->setAnchorPoint(Vec2::ZERO);
		m_roleBg->setPosition(Vec2::ZERO);
		this->addChild(m_roleBg,Z_Back);
		//角色按钮;
		m_roleBtn = Button::create(RESOURCE("tili_02.png"),RESOURCE("tili_03"));
		m_roleBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleBtn->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,m_roleBg->getContentSize().height*0.5));
		this->addChild(m_roleBtn,Z_First);
		m_roleBtn->addTouchEventListener(CC_CALLBACK_2(RoleWidget::BtnCall,this));
		m_roleBtn->setTag(RoleWidgetBtnRole);
		//出战按钮;
		m_roleFightBtn = Button::create(RESOURCE("add_01.png"),RESOURCE("add_002.png"));
		m_roleFightBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleFightBtn->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,0));
		this->addChild(m_roleFightBtn,Z_First);
		m_roleFightBtn->addTouchEventListener(CC_CALLBACK_2(RoleWidget::BtnCall,this));
		m_roleFightBtn->setTag(RoleWidgetBtnFight);
		//表示当前已出战的图片;
		m_roleFightSpr = Sprite::create(RESOURCE("diban01.png"));
		m_roleFightSpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleFightSpr->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,0));
		this->addChild(m_roleFightSpr,Z_First);
		m_roleFightSpr->setVisible(false);

		return true;
	} while (false);
	return false;
}

void RoleWidget::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	int tag = ((Button*)pSender)->getTag();
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		switch (tag)
		{
		case RoleWidgetBtnRole:onBtnRole();break;
		case RoleWidgetBtnFight:onBtnFight();break;
		default:break;
		}
		break;
	default:break;
	}
}

void RoleWidget::onBtnRole()
{

}

void RoleWidget::onBtnFight()
{

}

////////////////////////////////////////////////


//角色;
RoleDisplay::RoleDisplay():m_btnExit(nullptr)
{
	m_vecRoleWidget.clear();
}

Scene* RoleDisplay::createScene()
{
	do 
	{
		Scene* scene = Scene :: create();
		CC_BREAK_IF(!scene);

		RoleDisplay* layer = RoleDisplay::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

		return scene;
	} while (false);
	return nullptr;
}

bool RoleDisplay::init()
{
	do 
	{
		CC_BREAK_IF(!Layer::init());

		showUI();

		return true;
	} while (false);
	return false;
}

void RoleDisplay::showUI()
{
	Sprite* bg = Sprite::create(RESOURCE("gamemissionset/bgt_01.png"));
	bg->setPosition(Vec2::ZERO);
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg,Z_Back);

	m_btnExit = Button :: create(RESOURCE("gamemissionset/gb_01.png"),RESOURCE("gamemissionset/gb_02.png"));
	m_btnExit->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnExit->setPosition(Vec2(GLB_SIZE.width-80.f,GLB_SIZE.height-80.f));
	this->addChild(m_btnExit,Z_First);
	m_btnExit->addTouchEventListener(CC_CALLBACK_2(RoleDisplay::BtnCall,this));
	m_btnExit->setTag(RoleDisplayBtnExit);

	//五种角色控件;
	m_vecRoleWidget.clear();
	for (unsigned int i=0;i<5;i++)
	{
		RoleWidget* cache = RoleWidget::create();
		cache->setAnchorPoint(Vec2::ZERO);
		this->addChild(cache,Z_First);
		m_vecRoleWidget.push_back(cache);
		//CC_SAFE_DELETE(cache);
	}
	m_vecRoleWidget.at(0)->setPosition(Vec2(0,GLB_SIZE.height*0.5+200.f));
	m_vecRoleWidget.at(1)->setPosition(Vec2(GLB_SIZE.width*0.5-85,GLB_SIZE.height*0.5+200.f));
	m_vecRoleWidget.at(2)->setPosition(Vec2(GLB_SIZE.width-190,GLB_SIZE.height*0.5+200.f));
	m_vecRoleWidget.at(3)->setPosition(Vec2(0,GLB_SIZE.height*0.5-100.f));
	m_vecRoleWidget.at(4)->setPosition(Vec2(GLB_SIZE.width-190,GLB_SIZE.height*0.5-100.f));

}

void RoleDisplay::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	int tag = ((Button*)pSender)->getTag();
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		switch (tag)
		{
		case RoleDisplayBtnExit:
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

void RoleDisplay::onBtnExit()
{
	GameMusicControl::getInstance()->btnPlay();
	SCENE_CHANGE_FADE(SceneState::UIGameMain);
}
