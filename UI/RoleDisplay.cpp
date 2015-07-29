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
RoleWidget::RoleWidget():m_roleBg(nullptr),m_roleBtn(nullptr),m_roleChoice(nullptr),m_roleFightSpr(nullptr),m_roleLock(nullptr)
{
}

bool RoleWidget::init()
{
	do 
	{
		CC_BREAK_IF(!Layout::init());
		//底图;
		m_roleBg = Sprite::create(RESOURCE("role/ui_ckuang01.png"));
		m_roleBg->setAnchorPoint(Vec2::ZERO);
		m_roleBg->setPosition(Vec2::ZERO);
		this->addChild(m_roleBg,Z_Back);
		//角色按钮;
		m_roleBtn = Button::create(RESOURCE("role/ui_ckuang01.png"));
		m_roleBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleBtn->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,m_roleBg->getContentSize().height*0.5));
		this->addChild(m_roleBtn,Z_First);
		m_roleBtn->addTouchEventListener(CC_CALLBACK_2(RoleWidget::BtnCall,this));
		m_roleBtn->setTag(RoleWidgetBtnRole);
		m_roleBtn->setVisible(true);
		//选中对号;
		m_roleChoice = Sprite::create(RESOURCE("role/ui_xuanzhong.png"));
		m_roleChoice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleChoice->setPosition(Vec2(m_roleBg->getContentSize().width-15,30));
		this->addChild(m_roleChoice,Z_Second);
		//表示当前已出战的图片;
		m_roleFightSpr = Sprite::create(RESOURCE("role/ui_inwar.png"));
		m_roleFightSpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleFightSpr->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,m_roleBg->getContentSize().height-10));
		this->addChild(m_roleFightSpr,Z_Second);
		m_roleFightSpr->setVisible(true);
		//未开启;
		m_roleLock = Sprite::create(RESOURCE("role/ui_suo.png"));
		m_roleLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleLock->setPosition( vecSub(m_roleBg->getContentSize(),Vec2(10.f,10.f)) );
		this->addChild(m_roleLock,Z_Second);


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
RoleDisplay::RoleDisplay():m_btnExit(nullptr),
	m_labCrystal(nullptr),
	m_btnBig(nullptr),
	m_roleName(nullptr),
	m_roleFeed(nullptr),
	m_roleFullGrade(nullptr),
	m_labFeedCost(nullptr),
	m_labFullGradeCost(nullptr),
	m_labLvPrevious(nullptr),
	m_labLvTarget(nullptr),
	m_cwProgress(nullptr),
	m_level(nullptr),
	m_labProgress(nullptr),
	m_roleArmature(nullptr),
	m_labPreviousText(nullptr),
	m_labTargetText(nullptr)
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
	Sprite* bg = Sprite::create(RESOURCE("role/ui_cwbg.png"));
	bg->setPosition(Vec2::ZERO);
	bg->setAnchorPoint(Vec2::ZERO);
	this->addChild(bg,Z_Back);
	//顶部底板;
	Sprite* topBack = Sprite::create(RESOURCE("role/ui_cwbg04.png"));
	topBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	topBack->setPosition(Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height-topBack->getContentSize().height*0.5));
	this->addChild(topBack,Z_First);
	//退出按钮;
	m_btnExit = Button :: create(RESOURCE("role/ui_gb02.png"),RESOURCE("role/ui_gb01.png"));
	m_btnExit->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnExit->setPosition( vecSub(GLB_SIZE,m_btnExit->getContentSize(),0.5f) );
	this->addChild(m_btnExit,Z_Second);
	m_btnExit->addTouchEventListener(CC_CALLBACK_2(RoleDisplay::BtnCall,this));
	m_btnExit->setTag(RoleDisplayBtnExit);
	//“宠物”;
	Sprite* chongwu = Sprite::create(RESOURCE("role/ui_cw.png"));
	chongwu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	chongwu->setPosition( Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height-chongwu->getContentSize().height*0.5) );
	this->addChild(chongwu,Z_Second);
	//水晶栏;
	Sprite* crystalBack = Sprite::create(RESOURCE("role/ui_zuankuang.png"));
	crystalBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	crystalBack->setPosition(Vec2(0,GLB_SIZE.height-crystalBack->getContentSize().height*0.5f-10.f));
	this->addChild(crystalBack,Z_Second);
	//水晶数量;
	m_labCrystal = Label::createWithCharMap(RESOURCE("role/ui_numbercw01.png"),18,25,'0');
	m_labCrystal->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_labCrystal->setPosition(Vec2(85.f,GLB_SIZE.height-crystalBack->getContentSize().height*0.5f-10.f));
	this->addChild(m_labCrystal,Z_Third);
	m_labCrystal->setString("998");
	//中间底板;
	Sprite* middleBack = Sprite::create(RESOURCE("role/ui_cwbg02.png"));
	middleBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	middleBack->setPosition( vecMid(GLB_SIZE,Vec2(0,-60)) );
	this->addChild(middleBack,Z_First);
	//大按钮;
	m_btnBig = Button::create(RESOURCE("role/ui_daniu01.png"),RESOURCE("role/ui_daniu02.png"),RESOURCE("role/ui_daniu03.png"));
	m_btnBig->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_btnBig->setPosition( Vec2(GLB_SIZE.width*0.5f,20.f+m_btnBig->getContentSize().height*0.5f));
	this->addChild(m_btnBig,Z_First);
	m_btnBig->addTouchEventListener(CC_CALLBACK_2(RoleDisplay::BtnCall,this));
	m_btnBig->setTag(RoleDisplayBtnBig);
	//五种角色控件;
	m_vecRoleWidget.clear();
	for (unsigned int i=0;i<3;i++)
	{
		RoleWidget* cache = RoleWidget::create();
		//cache->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		this->addChild(cache,Z_First);
		m_vecRoleWidget.push_back(cache);
		//CC_SAFE_DELETE(cache);
	}
	m_vecRoleWidget.at(0)->setPosition(Vec2(GLB_SIZE.width*0.5-200.f-66.f,GLB_SIZE.height*0.5+320.f));
	m_vecRoleWidget.at(1)->setPosition(Vec2(GLB_SIZE.width*0.5-66.f,GLB_SIZE.height*0.5+320.f));
	m_vecRoleWidget.at(2)->setPosition(Vec2(GLB_SIZE.width*0.5+200.f-66.f,GLB_SIZE.height*0.5+320.f));

	//宠物名底板;
	Sprite* nameBack = Sprite::create(RESOURCE("role/ui_ckuang02.png"));
	nameBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	nameBack->setPosition(middleBack->getContentSize().width*0.5,middleBack->getContentSize().height-20.f);
	middleBack->addChild(nameBack,Z_First);
	//宠物名称;
	m_roleName = Sprite::create(RESOURCE("role/ui_name01.png"));
	m_roleName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_roleName->setPosition(vecMid(nameBack->getContentSize()));
	nameBack->addChild(m_roleName,Z_First);
	//宠物墩子;
	Sprite* dunzi = Sprite::create(RESOURCE("role/ui_dz.png"));
	dunzi->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	dunzi->setPosition(vecSub(vecMid(middleBack->getContentSize()),Vec2(140.f,-60.f)));
	middleBack->addChild(dunzi,Z_First);
	//宠物形象及动画;
	string name = GameFunctions::getInstance()->soldierTypeName[0][4];
	m_roleArmature = Armature::create(name);
	m_roleArmature->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_roleArmature->setPosition(Vec2(dunzi->getContentSize().width*0.5,dunzi->getContentSize().height-20.f));
	m_roleArmature->setScale(1.6f);
	dunzi->addChild(m_roleArmature,Z_First);
	m_roleArmature->getAnimation()->play("hephaestus_angry",-1,1);
	//宠物级别进度条;
	Sprite* progressBack = Sprite::create(RESOURCE("role/ui_lvjy01.png"));
	progressBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	progressBack->setPosition(Vec2(0,-10.f));
	dunzi->addChild(progressBack,Z_First);
	m_cwProgress = ProgressTimer::create(Sprite::create(RESOURCE("role/ui_lvjy02.png")));//创建一个进程条;
	m_cwProgress->setAnchorPoint(Vec2(0.5,0.5));
	m_cwProgress->setPosition( vecMid(progressBack->getContentSize()) );
	progressBack->addChild(m_cwProgress,Z_First);
	m_cwProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率;
	m_cwProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型;
	m_cwProgress->setMidpoint(Point(0,0));//设置进度的运动方向;
	m_cwProgress->setPercentage(50.f);
	//进度icon;
	Sprite* progressIcon = Sprite::create(RESOURCE("role/ui_lvdj.png"));
	progressIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	progressIcon->setPosition(Vec2(-30,progressBack->getContentSize().height*0.5));
	progressBack->addChild(progressIcon,Z_First);
	//等级;
	m_level = Label::createWithCharMap(RESOURCE("role/ui_numbercw04.png"),26,36,'0');
	m_level->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_level->setPosition(Vec2(75.f,35.f));
	progressIcon->addChild(m_level,Z_First);
	m_level->setString("2");
	//等级进度lable 0/640;
	m_labProgress = Label::createWithCharMap(RESOURCE("role/ui_numbercw03.png"),16,24,'0');
	m_labProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_labProgress->setPosition( vecMid(progressBack->getContentSize(),Vec2(10.f,0)) );
	progressBack->addChild(m_labProgress,Z_Second);
	m_labProgress->setString("0:640");
	//文字说明牌;
	Sprite* paiA = Sprite::create(RESOURCE("role/ui_cwbg03.png"));
	paiA->setAnchorPoint(Vec2::ZERO);
	paiA->setPosition(40.f,40.f);
	middleBack->addChild(paiA,Z_First);
	Sprite* paiB = Sprite::create(RESOURCE("role/ui_cwbg03.png"));
	paiB->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	paiB->setPosition( Vec2(middleBack->getContentSize().width-40.f,40.f) );
	middleBack->addChild(paiB,Z_First);
	//lv;
	Sprite* lvPrevious = Sprite::create(RESOURCE("role/ui_lv.png"));
	lvPrevious->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lvPrevious->setPosition( vecMid(paiA->getContentSize(),Vec2(-15.f,160.f)) );
	paiA->addChild(lvPrevious,Z_First);
	Sprite* lvTarget = Sprite::create(RESOURCE("role/ui_lv.png"));
	lvTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lvTarget->setPosition( vecMid(paiB->getContentSize(),Vec2(-15.f,160.f)) );
	paiB->addChild(lvTarget,Z_First);
	//等级label;
	m_labLvPrevious = Label::createWithCharMap(RESOURCE("role/ui_numbercw02.png"),18,19,'0');
	m_labLvPrevious->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_labLvPrevious->setPosition( vecMid(paiA->getContentSize(),Vec2(35.f,160.f)) );
	paiA->addChild(m_labLvPrevious,Z_First);
	m_labLvPrevious->setString("1");
	m_labLvTarget = Label::createWithCharMap(RESOURCE("role/ui_numbercw02.png"),18,19,'0');
	m_labLvTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_labLvTarget->setPosition( vecMid(paiB->getContentSize(),Vec2(35.f,160.f)) );
	paiB->addChild(m_labLvTarget,Z_First);
	m_labLvTarget->setString("2");
	//目前等级说明文本;
	string text = (GameUIData::getInstance()->vec_Role.back())->m_sDescribe;
	//系统字;
	m_labPreviousText = Label::createWithSystemFont(text.c_str(),"Arial",30);
	//方正姚体常规;
// 	TTFConfig temp(RESOURCE("fonts/FZYTK.ttf"),35);
// 	m_labPreviousText = Label::createWithTTF(temp,text,TextHAlignment::LEFT,180.f);
// 	m_labPreviousText->enableOutline(Color4B(255,0,0,0));
// 	m_labPreviousText->enableGlow(Color4B::GREEN);//荧光颜色为绿色;
	m_labPreviousText->setWidth(190.f);
	m_labPreviousText->setColor(Color3B::BLACK);
	m_labPreviousText->setAnchorPoint(Vec2::ZERO);
	m_labPreviousText->setPosition(Vec2(40.f,80.f));
	paiA->addChild(m_labPreviousText,Z_First);
	//目标等级说明文本;
	m_labTargetText = Label::createWithSystemFont(text.c_str(),"Arial",30);
	m_labTargetText->setWidth(190.f);
	m_labTargetText->setColor(Color3B::BLACK);
	m_labTargetText->setAnchorPoint(Vec2::ZERO);
	m_labTargetText->setPosition(Vec2(40.f,80.f));
	paiB->addChild(m_labTargetText,Z_First);
	//箭头;
	Sprite* jiantou = Sprite::create(RESOURCE("role/ui_jt01.png"));
	jiantou->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	jiantou->setPosition( vecMid(middleBack->getContentSize(),Vec2(0,-380.f)) );
	middleBack->addChild(jiantou,Z_First);
	//喂养button;
	m_roleFeed = Button::create(RESOURCE("role/ui_sjniu01.png"),RESOURCE("role/ui_sjniu02.png"),RESOURCE("role/ui_sjniu03.png"));
	m_roleFeed->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_roleFeed->setPosition( vecMid(middleBack->getContentSize(),Vec2(330.f,330.f)));
	middleBack->addChild(m_roleFeed,Z_First);
	//“喂养”;
	Sprite* feed = Sprite::create(RESOURCE("role/ui_wy.png"));
	feed->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	feed->setPosition(vecMid(m_roleFeed->getContentSize()));
	m_roleFeed->addChild(feed,Z_First);
	//label;
	m_labFeedCost = Label::createWithCharMap(RESOURCE("role/ui_numbercw01.png"),18,25,'0');
	m_labFeedCost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_labFeedCost->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f,m_roleFeed->getContentSize().height+30.f));
	m_roleFeed->addChild(m_labFeedCost,Z_First);
	m_labFeedCost->setString("20");
	//sprite;
	Sprite* feedCrystal = Sprite::create(RESOURCE("role/ui_zuanshi02.png"));
	feedCrystal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	feedCrystal->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f-40.f,m_roleFeed->getContentSize().height+30.f));
	m_roleFeed->addChild(feedCrystal,Z_First);
	//一键满级button;
	m_roleFullGrade = Button::create(RESOURCE("role/ui_sjniu01.png"),RESOURCE("role/ui_sjniu02.png"),RESOURCE("role/ui_sjniu03.png"));
	m_roleFullGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_roleFullGrade->setPosition( vecMid(middleBack->getContentSize(),Vec2(330.f,-0.f)) );
	middleBack->addChild(m_roleFullGrade,Z_First);
	//“一键满级”;
	Sprite* fullGrade = Sprite::create(RESOURCE("role/ui_yjmj.png"));
	fullGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	fullGrade->setPosition(vecMid(m_roleFullGrade->getContentSize()));
	m_roleFullGrade->addChild(fullGrade,Z_First);
	//label;
	m_labFullGradeCost = Label::createWithCharMap(RESOURCE("role/ui_numbercw01.png"),18,25,'0');
	m_labFullGradeCost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_labFullGradeCost->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f,m_roleFeed->getContentSize().height+30.f));
	m_roleFullGrade->addChild(m_labFullGradeCost,Z_First);
	m_labFullGradeCost->setString("654");
	//sprite;
	Sprite* fullCrystal = Sprite::create(RESOURCE("role/ui_zuanshi02.png"));
	fullCrystal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	fullCrystal->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f-40.f,m_roleFeed->getContentSize().height+30.f));
	m_roleFullGrade->addChild(fullCrystal,Z_First);
}

void RoleDisplay::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	int tag = ((Button*)pSender)->getTag();
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		switch (tag)
		{
		case RoleDisplayBtnExit:onBtnExit();break;
		case RoleDisplayBtnBig:onBtnBig();break;
		default:break;
		}
		break;
	default:break;
	}
}

void RoleDisplay::onBtnExit()
{
	GameMusicControl::getInstance()->btnPlay();
	SCENE_CHANGE_FADE(SceneState::UIGameMain);
}

void RoleDisplay::onBtnBig()
{
	CCLOG(" RoleDisplay::onBtnBig ");
}
