//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/21;
//@功能描述①: 角色系统展示界面;
//@功能描述②: 用于角色展示，选择，升级，查看等功能;
//@功能描述③: 场景类，提供切换、调用接口,支持弹窗;
//**************************************************************
/*
解锁和试用设计思路;
1、设置当前选择的角色标记;
2、设置当前应该出战的角色标记;
3、每次进入角色选择界面时初始化解锁的角色,初始化时需要判断两次,如果是早就解锁的则正常处理,如果是新近解锁的则播放解锁特效;
4、每次进入出战界面配备出战角色，包括试用角色。到指定关卡后与试用条件相比较，满足则判断当前选择角色是否是试用角色，不是则换成试用角色;
5、向游戏内传递出战角色标记的设计逻辑:如果当前关卡没有试用角色，则传递出战标记的角色。否则传递使用角色的ID，但是出战角色的标记不变;
6、试用是在解锁关卡时使用，试用结束即解锁。然后在大地图界面会有新解锁的提示。进入角色界面不会再有提示动画。另外人民币开启的角色，会在播放动画会更新解锁标记;

按钮状态判断;
1、选中按钮时，用对号显示;
2、选择出战的按钮显示出战中;
3、未解锁的按钮，用锁头显示;
4、解锁状态：满足解锁条件的显示600水晶召唤;
   未满足解锁条件的显示一键召唤;

切换按钮;
1、通过不同的角色Id，显示不同的角色形象;
2、到user_role表（对应的cache）中读取 等级和当前喂养值;
3、到roledata表（对应的cache）中读取 当前级别满级喂养值，单次喂养值，一键满级喂养值，级别说明文字;

单次喂养设计;
需要获取当前宠物Id，然后到user_role表中读取当前等级和喂养值;
1、计算当前水晶币是否充足，更新之;
2、刷新进度条进度和数值显示，并且保存;
3、刷新一键满级剩余值;
4、如果当前级别满了则更新级别、单次喂养值、一键满级喂养值、进度条进度和显示数值;

一键满级喂养设计;
1、计算水晶币是否充足;
2、刷新进度条进度和数值显示;
3、刷新新级别的喂养值和一键满级值;

水晶币设计;
1、每次水晶币消耗都要判断是否充足，否则显示充值界面并回调统一的充值接口;
2、数值变化之后要更新存储、更新显示;
*/
#include "RoleDisplay.h"
#include "GameMusicControl.h"

//控件类;
RoleWidget::RoleWidget():m_roleBg(nullptr),m_roleBtn(nullptr),m_roleChoice(nullptr),m_roleFightSpr(nullptr),m_roleLock(nullptr),m_nId(-1)
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
		m_roleBtn->setVisible(true);
		//选中对号;
		m_roleChoice = Sprite::create(RESOURCE("role/ui_xuanzhong.png"));
		m_roleChoice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleChoice->setPosition(Vec2(m_roleBg->getContentSize().width-15,30));
		this->addChild(m_roleChoice,Z_Second);
		m_roleChoice->setVisible(false);
		//表示当前已出战的图片;
		m_roleFightSpr = Sprite::create(RESOURCE("role/ui_inwar.png"));
		m_roleFightSpr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleFightSpr->setPosition(Vec2(m_roleBg->getContentSize().width*0.5,m_roleBg->getContentSize().height-10));
		this->addChild(m_roleFightSpr,Z_Second);
		m_roleFightSpr->setVisible(false);
		//未开启;
		m_roleLock = Sprite::create(RESOURCE("role/ui_suo.png"));
		m_roleLock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_roleLock->setPosition( vecSub(m_roleBg->getContentSize(),Vec2(10.f,10.f)) );
		this->addChild(m_roleLock,Z_Second);
		m_roleLock->setVisible(true);

		return true;
	} while (false);
	return false;
}

void RoleWidget::BtnCall(Ref* pSender,Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
		onBtnRole(CC_CALLBACK_1(RoleDisplay::widgetBtnCallBack,g_pRoleDisplay),m_nId);
		break;
	default:break;
	}
}

void RoleWidget::onBtnRole(parameter_1_int callback,int& id)
{
	callback(id);
}

////////////////////////////////////////////////

//角色;
extern RoleDisplay* g_pRoleDisplay = nullptr;
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
	progressBack(nullptr),
	m_cwProgress(nullptr),
	m_labRoleLock(nullptr),
	m_level(nullptr),
	m_labProgress(nullptr),
	m_roleArmature(nullptr),
	m_labPreviousText(nullptr),
	m_labTargetText(nullptr),
	m_spriteBigbtn(nullptr),
	callBackEnable(false),
	middleBack(nullptr),
	feedCrystal(nullptr),
	fullCrystal(nullptr),
	m_sprArrow(nullptr),
	paiA(nullptr),
	paiB(nullptr)
{
	m_vecRoleWidget.clear();
	g_pRoleDisplay = this;
}

RoleDisplay::~RoleDisplay()
{
	m_vecRoleWidget.clear();
	g_pRoleDisplay = nullptr;
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
	m_labCrystal->setString(__String::createWithFormat("%d",Get_Diamonds)->getCString());
	//中间底板;
	middleBack = Sprite::create(RESOURCE("role/ui_cwbg02.png"));
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
	m_spriteBigbtn = Sprite::create(RESOURCE("role/ui_gowar.png"));
	m_spriteBigbtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_spriteBigbtn->setPosition(vecMid(m_btnBig));
	m_btnBig->addChild(m_spriteBigbtn,Z_First);

	initRoleWidget();

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
	dunzi->setPosition(vecSub(vecMid(middleBack->getContentSize()),Vec2(130.f,-40.f)));
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
	progressBack = Sprite::create(RESOURCE("role/ui_lvjy01.png"));
	progressBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	progressBack->setPosition(Vec2(0,-10.f));
	dunzi->addChild(progressBack,Z_First);
	progressBack->setVisible(false);
	m_cwProgress = ProgressTimer::create(Sprite::create(RESOURCE("role/ui_lvjy02.png")));//创建一个进程条;
	m_cwProgress->setAnchorPoint(Vec2(0.5,0.5));
	m_cwProgress->setPosition( vecMid(progressBack->getContentSize()) );
	progressBack->addChild(m_cwProgress,Z_First);
	m_cwProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率;
	m_cwProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型;
	m_cwProgress->setMidpoint(Point(0,0));//设置进度的运动方向;
	m_cwProgress->setPercentage(0.f);
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
	m_level->setString("1");
	//等级进度lable 0/640;
	m_labProgress = Label::createWithCharMap(RESOURCE("role/ui_numbercw03.png"),16,24,'0');
	m_labProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_labProgress->setPosition( vecMid(progressBack->getContentSize(),Vec2(10.f,0)) );
	progressBack->addChild(m_labProgress,Z_Second);
	m_labProgress->setString("0:640");
	//解锁条件说明底板;
	m_sprRoleLockBack = Sprite::create(RESOURCE("role/ui_ckuang03.png"));
	m_sprRoleLockBack->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_sprRoleLockBack->setPosition( vecSub(dunzi->getContentSize(),0.5f,Vec2(10.f,40.f) ) );
	dunzi->addChild(m_sprRoleLockBack,Z_First);
	m_sprRoleLockBack->setVisible(false);
	//解锁条件文字;
	m_labRoleLock = Label::createWithSystemFont("","Arial",30);
	m_labRoleLock->setWidth(190.f);
	m_labRoleLock->setColor(Color3B::BLACK);
	m_labRoleLock->setAnchorPoint(Vec2::ZERO);
	m_labRoleLock->setPosition(Vec2(40.f,20.f));
	m_sprRoleLockBack->addChild(m_labRoleLock,Z_First);
	//文字说明牌;
	paiA = Sprite::create(RESOURCE("role/ui_cwbg03.png"));
	paiA->setAnchorPoint(Vec2::ZERO);
	paiA->setPosition(40.f,40.f);
	middleBack->addChild(paiA,Z_First);
	paiB = Sprite::create(RESOURCE("role/ui_cwbg03.png"));
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
	//string text = GameUIData::getInstance()->getDragonData(4,3)->m_sDescribe;//(GameUIData::getInstance()->vec_Role.back())->m_sDescribe;
	string text = "";
	//系统字;
	m_labPreviousText = Label::createWithSystemFont(text.c_str(),"Arial",30);
	//方正姚体常规;
// 	TTFConfig temp(RESOURCE("fonts/FZYTK.ttf"),35);
// 	m_labPreviousText = Label::createWithTTF(temp,text,TextHAlignment::LEFT,180.f);
// 	m_labPreviousText->enableOutline(Color4B(255,0,0,0));
// 	m_labPreviousText->enableGlow(Color4B::GREEN);//荧光颜色为绿色;
	m_labPreviousText->setWidth(200.f);
	m_labPreviousText->setColor(Color3B::BLACK);
	m_labPreviousText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_labPreviousText->setPosition(Vec2(40.f,165.f));
	paiA->addChild(m_labPreviousText,Z_First);
	//目标等级说明文本;
	m_labTargetText = Label::createWithSystemFont(text.c_str(),"Arial",30);
	m_labTargetText->setWidth(200.f);
	m_labTargetText->setColor(Color3B::BLACK);
	m_labTargetText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_labTargetText->setPosition(Vec2(40.f,165.f));
	paiB->addChild(m_labTargetText,Z_First);
	//箭头;
	m_sprArrow = Sprite::create(RESOURCE("role/ui_jt01.png"));
	m_sprArrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_sprArrow->setPosition( vecMid(middleBack->getContentSize(),Vec2(0,-380.f)) );
	middleBack->addChild(m_sprArrow,Z_First);
	//喂养button;
	m_roleFeed = Button::create(RESOURCE("role/ui_sjniu01.png"),RESOURCE("role/ui_sjniu02.png"),RESOURCE("role/ui_sjniu03.png"));
	m_roleFeed->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_roleFeed->setPosition( vecMid(middleBack->getContentSize(),Vec2(330.f,330.f)));
	middleBack->addChild(m_roleFeed,Z_First);
	m_roleFeed->setTag(RoleDisplayBtnFeed);
	m_roleFeed->addTouchEventListener(CC_CALLBACK_2(RoleDisplay::BtnCall,this));
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
	feedCrystal = Sprite::create(RESOURCE("role/ui_zuanshi02.png"));
	feedCrystal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	feedCrystal->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f-40.f,m_roleFeed->getContentSize().height+30.f));
	m_roleFeed->addChild(feedCrystal,Z_First);
	//一键满级button;
	m_roleFullGrade = Button::create(RESOURCE("role/ui_sjniu01.png"),RESOURCE("role/ui_sjniu02.png"),RESOURCE("role/ui_sjniu03.png"));
	m_roleFullGrade->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_roleFullGrade->setPosition( vecMid(middleBack->getContentSize(),Vec2(330.f,-0.f)) );
	middleBack->addChild(m_roleFullGrade,Z_First);
	m_roleFullGrade->setTag(RoleDisplayBtnFullGrade);
	m_roleFullGrade->addTouchEventListener(CC_CALLBACK_2(RoleDisplay::BtnCall,this));
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
	fullCrystal = Sprite::create(RESOURCE("role/ui_zuanshi02.png"));
	fullCrystal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	fullCrystal->setPosition(Vec2(m_roleFeed->getContentSize().width*0.5f-40.f,m_roleFeed->getContentSize().height+30.f));
	m_roleFullGrade->addChild(fullCrystal,Z_First);
	int temp = Get_Role_Choice;
	initAllWidget(temp);
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
		case RoleDisplayBtnFeed:onBtnFeed();break;
		case RoleDisplayBtnFullGrade:onBtnFullGrade();break;
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
	if (callBackEnable)//用于关闭界面按钮响应事件;
	{
		return;
	}
	//当前选中的角色id;
	int index = Get_Role_Choice;
	if (GameUIData::getInstance()->getUserRoleData(index).roleLevel)
	{
		//已经解锁;
		GameUIData::getInstance()->setIntegerForKey("role_fight",index);
		//出战中;
		BIGBTN_FIGHTING;
		btnBright(m_btnBig,false);
		for (int i = 0;i<m_vecRoleWidget.size();++i)
		{
			if (i == index-1)
			{
				m_vecRoleWidget.at(i)->m_roleFightSpr->setVisible(true);
			}
			else
			{
				m_vecRoleWidget.at(i)->m_roleFightSpr->setVisible(false);
			}
		}
	}
	else
	{
		//判断是600钻石解锁还是人民币解锁;
		if (GameUIData::getInstance()->getNormalMissionProgress() >= GameUIData::getInstance()->getDragonData(index,1)->m_aUnlockCondition.at(0))
		{
			//600钻石;

		}
		else
		{
			//人民币;

		}
	}

	//m_vecRoleWidget.at(i)->m_roleLock->setVisible(true);
}

void RoleDisplay::onBtnFeed()
{
	if (callBackEnable)//用于关闭界面按钮响应事件;
	{
		return;
	}
	//当前选中的角色id;
	int index = Get_Role_Choice;
	UserRole temp = GameUIData::getInstance()->getUserRoleData(index);
	if (temp.roleLevel<1)//级别值要防范小于1;
	{
		CCLOG(" RoleDisplay::onBtnFeed ERROR");
		return;
	}
	GameDragonBase* pDragon = GameUIData::getInstance()->getDragonData(index,temp.roleLevel);
	//需要刷新的值:经验值,一键满级值,水晶值,进度条.如果升级了,还需刷新级别,级别说明,级别图标,经验总值;
	int nowCrystal = Get_Diamonds;
	int perFeed = pDragon->m_nUpGradeTotal/10;
	if ( nowCrystal >= perFeed )//水晶够用;
	{
		//水晶扣减;
		extractReduceCrystal(nowCrystal-perFeed);
		//总共需要喂养值;
		int total = pDragon->m_nUpGradeTotal;
		//封装进度条进度和百分比;
		extractProgressPercent(temp.curFeed+perFeed,total);
		//保存喂养数据;
		GameUIData::getInstance()->writeUserRoleData(index,temp.roleLevel,temp.curFeed+perFeed);
		//封装一键满级和喂养的文字显示;
		extractFullGradeAndFeeded(index, temp, total);

		//计算如果刚好满级的情况;
		if (temp.curFeed+perFeed==total)
		{
			if (temp.roleLevel<3)
			{
				//进度条缓慢降到0;
				setCallBackEnable(true);
				m_cwProgress->runAction(Sequence::create(
					DelayTime::create(0.5f),
					ProgressTo::create(1.f,0.f),
					CallFunc::create(CC_CALLBACK_0(RoleDisplay::setCallBackEnable,this,false)),
					nullptr));
				//重新获取数据;
				pDragon = GameUIData::getInstance()->getDragonData(index,temp.roleLevel+1);
				perFeed = pDragon->m_nUpGradeTotal/10;
				total = pDragon->m_nUpGradeTotal;
				//进度条进度显示;
				m_labProgress->setString(__String::createWithFormat("%d:%d",0,total)->getCString());
				//级别Icon等级;
				m_level->setString(__String::createWithFormat("%d",temp.roleLevel+1)->getCString());
				//文字牌等级和文字说明;
				extractLevelAndLvText(index,temp.roleLevel+1);
				//保存喂养数据;
				GameUIData::getInstance()->writeUserRoleData(index,temp.roleLevel+1,0);
				//一键满级和喂养的文字显示;
				temp = GameUIData::getInstance()->getUserRoleData(index);
				extractFullGradeAndFeeded(index, temp, total);
			}
			else//满级了;
			{
				//级别Icon等级;
				m_level->setString(__String::createWithFormat("%d",temp.roleLevel+1)->getCString());
				m_cwProgress->setPercentage(100.f);	//进度条显示百分百;
				m_labProgress->setVisible(false);	//进度值隐藏;
				paiA->setPosition( Vec2(middleBack->getContentSize().width*0.5f-paiA->getContentSize().width*0.5f,40.f) );	//牌A位置居中;
				paiB->setVisible(false);	//牌B消失;
				m_sprArrow->setVisible(false);
				m_labLvPrevious->setString(__String::createWithFormat("%d",temp.roleLevel+1)->getCString());	//最高级数字;
				m_labPreviousText->setString(GameUIData::getInstance()->getDragonData(index,temp.roleLevel+1)->m_sDescribe);	//最高级描述;
				btnBright(m_roleFeed,false);
				btnBright(m_roleFullGrade,false);
				feedCrystal->setVisible(false);
				fullCrystal->setVisible(false);
				m_labFeedCost->setVisible(false);
				m_labFullGradeCost->setVisible(false);
				GameUIData::getInstance()->writeUserRoleData(index,temp.roleLevel+1,0);
			}
		}
	}
	else
	{
		MessageBox("主公，水晶不足......","提示");
		//RMB接口;
	}
}

void RoleDisplay::onBtnFullGrade()
{
	if (callBackEnable)//用于关闭界面按钮响应事件;
	{
		return;
	}
	int index = Get_Role_Choice;
	UserRole temp = GameUIData::getInstance()->getUserRoleData(index);
	if (temp.roleLevel<1)//级别值要防范小于1;
	{
		CCLOG(" RoleDisplay::onBtnFullGrade ERROR");
		return;
	}
	GameDragonBase* pDragon = GameUIData::getInstance()->getDragonData(index,temp.roleLevel);
	//需要刷新的值:经验值,一键满级值,水晶值,进度条.如果升级了,还需刷新级别,级别说明,级别图标,经验总值;
	int nowCrystal = Get_Diamonds;
	int sum = 0;
	for (int i=1;i<=GameUIData::getInstance()->_mapAllRole.at(index).size();i++)
	{
		sum += GameUIData::getInstance()->_mapAllRole.at(index).at(i)->m_nUpGradeTotal;
	}
	int feeded = 0;
	for (int j=1;j<temp.roleLevel;j++)
	{
		feeded += GameUIData::getInstance()->_mapAllRole.at(index).at(j)->m_nUpGradeTotal;
	}
	if ( nowCrystal >= sum-feeded-temp.curFeed )//水晶够用;
	{
		//水晶扣减;
		extractReduceCrystal(nowCrystal - (sum-feeded-temp.curFeed));
		//级别Icon等级;
		m_level->setString(__String::createWithFormat("%d",MAX)->getCString());
		m_cwProgress->setPercentage(100.f);	//进度条显示百分百;
		m_labProgress->setVisible(false);	//进度值隐藏;
		paiA->setPosition( Vec2(middleBack->getContentSize().width*0.5f-paiA->getContentSize().width*0.5f,40.f) );	//牌A位置居中;
		paiB->setVisible(false);	//牌B消失;
		m_sprArrow->setVisible(false);
		m_labLvPrevious->setString(__String::createWithFormat("%d",MAX)->getCString());	//最高级数字;
		m_labPreviousText->setString(GameUIData::getInstance()->getDragonData(index,MAX)->m_sDescribe);	//最高级描述;
		btnBright(m_roleFeed,false);
		btnBright(m_roleFullGrade,false);
		feedCrystal->setVisible(false);
		fullCrystal->setVisible(false);
		m_labFeedCost->setVisible(false);
		m_labFullGradeCost->setVisible(false);
		GameUIData::getInstance()->writeUserRoleData(index,MAX,0);
	}
	else
	{
		MessageBox("主公，水晶不足......","提示");
		//RMB接口;
	}
}

void RoleDisplay::widgetBtnCallBack(int& id)
{
	CCLOG("RoleDisplay::widgetBtnCallBack  %d ",id);
	if (callBackEnable)//用于关闭界面按钮响应事件;
	{
		return;
	}
	//对号;
	for (unsigned int i=0;i<m_vecRoleWidget.size();++i)
	{
		if (i == id-1)
		{
			m_vecRoleWidget.at(i)->m_roleChoice->setVisible(true);
			GameUIData::getInstance()->setIntegerForKey("role_choice",id);
		}
		else
		{
			m_vecRoleWidget.at(i)->m_roleChoice->setVisible(false);
		}
	}
	//出战;
	if (id == 1)
	{
		BIGBTN_FIGHT;
		m_vecRoleWidget.at(id-1)->m_roleFightSpr->setVisible(false);
		m_vecRoleWidget.at(id-1)->m_roleLock->setVisible(true);
	}
	else if (id == 2)	//600yuan;
	{
		//600钻;
		BIGBTN_Zuan600;
		m_vecRoleWidget.at(id-1)->m_roleFightSpr->setVisible(false);
		m_vecRoleWidget.at(id-1)->m_roleLock->setVisible(true);
	}
	else if (id == 3)
	{
		//12yuan；
		BIGBTN_Yuan12;
		m_vecRoleWidget.at(id-1)->m_roleFightSpr->setVisible(false);
		m_vecRoleWidget.at(id-1)->m_roleLock->setVisible(true);
	}
	initAllWidget(id);
}

void RoleDisplay::initRoleWidget()
{
	//3种角色控件;
	m_vecRoleWidget.clear();
	for (unsigned int i=0;i<3;i++)
	{
		RoleWidget* cache = RoleWidget::create();
		cache->m_nId = i+1;
		//对号,默认第一个为选中;
		if (i == GameUIData::getInstance()->getIntegerForKey("role_choice",1)-1)
		{
			cache->m_roleChoice->setVisible(true);
		}
		//出战中;
		if (i+1 == GameUIData::getInstance()->getIntegerForKey("role_fight",0))
		{
			cache->m_roleFightSpr->setVisible(true);
		}
		//锁头;
		if (GameUIData::getInstance()->getUserRoleData(i+1).roleLevel)
		{
			cache->m_roleLock->setVisible(false);
		}
		this->addChild(cache,Z_First);
		m_vecRoleWidget.push_back(cache);
	}
	m_vecRoleWidget.at(0)->setPosition(Vec2(GLB_SIZE.width*0.5-200.f-66.f,GLB_SIZE.height*0.5+320.f));
	m_vecRoleWidget.at(1)->setPosition(Vec2(GLB_SIZE.width*0.5-66.f,GLB_SIZE.height*0.5+320.f));
	m_vecRoleWidget.at(2)->setPosition(Vec2(GLB_SIZE.width*0.5+200.f-66.f,GLB_SIZE.height*0.5+320.f));

}

void RoleDisplay::initAllWidget(int& id)
{
	UserRole temp = GameUIData::getInstance()->getUserRoleData(id);
	GameDragonBase* pDragon = GameUIData::getInstance()->getDragonData(id,1);

	//不同角色不同形象;
	switch (id)
	{
	case 1:
		m_roleArmature->setVisible(true);
		break;
	case 2:
// 		string name = GameFunctions::getInstance()->soldierTypeName[0][4];
// 		m_roleArmature = Armature::create(name);
// 		m_roleArmature->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
// 		m_roleArmature->setPosition(Vec2(dunzi->getContentSize().width*0.5,dunzi->getContentSize().height-20.f));
// 		m_roleArmature->setScale(1.6f);
// 		dunzi->addChild(m_roleArmature,Z_First);
// 		m_roleArmature->getAnimation()->play("hephaestus_angry",-1,1);

		m_roleArmature->setVisible(false);

		break;
	case 3:
// 		string name = GameFunctions::getInstance()->soldierTypeName[0][4];
// 		m_roleArmature = Armature::create(name);
// 		m_roleArmature->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
// 		m_roleArmature->setPosition(Vec2(dunzi->getContentSize().width*0.5,dunzi->getContentSize().height-20.f));
// 		m_roleArmature->setScale(1.6f);
// 		dunzi->addChild(m_roleArmature,Z_First);
// 		m_roleArmature->getAnimation()->play("hephaestus_angry",-1,1);

		m_roleArmature->setVisible(false);

		break;
	}
	
	if (temp.roleLevel && temp.roleLevel<MAX)//已经解锁,判断是出战还是出战中;
	{
		//还原;
		feedCrystal->setVisible(true);
		fullCrystal->setVisible(true);
		m_labFeedCost->setVisible(true);
		m_labFullGradeCost->setVisible(true);
		m_labProgress->setVisible(true);
		paiA->setPosition( Vec2(40,40.f) );
		paiB->setVisible(true);
		m_sprArrow->setVisible(true);
		//锁头;
		m_vecRoleWidget.at(id-1)->m_roleLock->setVisible(false);
		//进度条及进度;
		progressBack->setVisible(true);
		m_sprRoleLockBack->setVisible(false);
		//等级图标;
		m_level->setString(__String::createWithFormat("%d",temp.roleLevel)->getCString());
		//总共需要喂养值;
		int total = GameUIData::getInstance()->getDragonData(id,temp.roleLevel)->m_nUpGradeTotal;
		//封装进度条进度和百分比;
		extractProgressPercent(temp.curFeed,total);
		//一键满级和喂养的文字显示;
		extractFullGradeAndFeeded(id, temp, total);
		//喂养和一键满级按钮;
		btnBright(m_roleFeed,true);
		btnBright(m_roleFullGrade,true);
		//级别和级别说明文字;
		extractLevelAndLvText(id,temp.roleLevel);
		//判断是出战还是出战中;
		extractFighting(id);	
	}
	else if (temp.roleLevel == MAX)	//满级;
	{
		//级别Icon等级;
		m_level->setString(__String::createWithFormat("%d",temp.roleLevel)->getCString());
		progressBack->setVisible(true);
		m_sprRoleLockBack->setVisible(false);
		m_cwProgress->setPercentage(100.f);	//进度条显示百分百;
		m_labProgress->setVisible(false);	//进度值隐藏;
		paiA->setPosition( Vec2(middleBack->getContentSize().width*0.5f-paiA->getContentSize().width*0.5f,40.f) );	//牌A位置居中;
		paiB->setVisible(false);	//牌B消失;
		m_sprArrow->setVisible(false);
		m_labLvPrevious->setString(__String::createWithFormat("%d",temp.roleLevel)->getCString());	//最高级数字;
		m_labPreviousText->setString(GameUIData::getInstance()->getDragonData(id,temp.roleLevel)->m_sDescribe);	//最高级描述;
		btnBright(m_roleFeed,false);
		btnBright(m_roleFullGrade,false);
		feedCrystal->setVisible(false);
		fullCrystal->setVisible(false);
		m_labFeedCost->setVisible(false);
		m_labFullGradeCost->setVisible(false);
		//判断出战还是出战中;
		extractFighting(id);
	}
	else//尚未解锁;
	{
		//还原;
		feedCrystal->setVisible(true);
		fullCrystal->setVisible(true);
		m_labFeedCost->setVisible(true);
		m_labFullGradeCost->setVisible(true);
		m_labProgress->setVisible(true);
		paiA->setPosition( Vec2(40,40.f) );
		paiB->setVisible(true);
		m_sprArrow->setVisible(true);

		//锁头;
		m_vecRoleWidget.at(id-1)->m_roleLock->setVisible(true);
		//解锁说明;
		progressBack->setVisible(false);
		m_sprRoleLockBack->setVisible(true);
		//解锁类型一的解锁条件;
		int condition = pDragon->m_aUnlockCondition.at(0);
		m_labRoleLock->setString(pDragon->m_sUnlockText);
		//喂养和一键满级按钮禁用;
		btnBright(m_roleFeed,false);
		btnBright(m_roleFullGrade,false);
		//级别和级别说明文字;
		extractLevelAndLvText(id,1);
		//总共需要喂养值;
		int total = GameUIData::getInstance()->getDragonData(id,1)->m_nUpGradeTotal;
		//一键满级和喂养;
		extractFullGradeAndFeeded(id,temp,total);
		//判断是600钻石解锁还是人民币解锁;
		if (GameUIData::getInstance()->getNormalMissionProgress() >= pDragon->m_aUnlockCondition.at(0))
		{
			//600钻石;
			btnBright(m_btnBig,true);
		}
		else
		{
			//人民币;
			btnBright(m_btnBig,true);
		}
	}
}

void RoleDisplay::btnBright(Button* pSender,bool flag)
{
	pSender->setBright(flag);
	pSender->setEnabled(flag);
}

void RoleDisplay::extractLevelAndLvText( int& id,int level )
{
	m_labLvPrevious->setString(__String::createWithFormat("%d",level)->getCString());
	m_labLvTarget->setString(__String::createWithFormat("%d",level+1)->getCString());
	m_labPreviousText->setString(GameUIData::getInstance()->getDragonData(id,level)->m_sDescribe);
	m_labTargetText->setString(GameUIData::getInstance()->getDragonData(id,level+1)->m_sDescribe);
}

void RoleDisplay::extractReduceCrystal(int crystal)
{
	Set_Diamonds(crystal);
	m_labCrystal->setString(__String::createWithFormat("%d",crystal)->getCString());
}

void RoleDisplay::extractProgressPercent(int cur,int total)
{
	m_labProgress->setString(__String::createWithFormat("%d:%d",cur,total)->getCString());
	m_cwProgress->setPercentage(cur*100.f/total);
}

void RoleDisplay::extractFullGradeAndFeeded(int& id, UserRole &temp, int total)
{
	//一键满级和喂养的文字显示;
	int sum = 0;
	for (int i=1;i<=GameUIData::getInstance()->_mapAllRole.at(id).size();i++)
	{
		sum += GameUIData::getInstance()->_mapAllRole.at(id).at(i)->m_nUpGradeTotal;
	}
	int feeded = 0;
	for (int j=1;j<temp.roleLevel;j++)
	{
		feeded += GameUIData::getInstance()->_mapAllRole.at(id).at(j)->m_nUpGradeTotal;
	}
	m_labFullGradeCost->setString(__String::createWithFormat("%d",sum-temp.curFeed-feeded)->getCString());
	m_labFeedCost->setString(__String::createWithFormat("%d",total/10)->getCString());
}

void RoleDisplay::extractFighting(int& id)
{
	//判断是出战还是出战中;
	if (id == Get_Role_Fight)
	{
		//出战中;
		BIGBTN_FIGHTING;
		for (int i = 0;i<m_vecRoleWidget.size();++i)
		{
			if (i == id-1)
			{
				m_vecRoleWidget.at(i)->m_roleFightSpr->setVisible(true);
			}
			else
			{
				m_vecRoleWidget.at(i)->m_roleFightSpr->setVisible(false);
			}
		}
		btnBright(m_btnBig,false);
	}
	else
	{
		//出战;
		BIGBTN_FIGHT;
		m_vecRoleWidget.at(id-1)->m_roleFightSpr->setVisible(false);
		btnBright(m_btnBig,true);
	}
}
