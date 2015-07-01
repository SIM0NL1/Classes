//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①: ;
//@功能描述②: ;
//@功能描述③: ;
//**************************************************************

#include "GameMain.h"
#include "GameUIData.h"
#include "../GameUILayer.h"
#include "../DataCenter.h"
#include "GameMusicControl.h"

const float _GAP = 0;
const int _COUNT = 2;

extern GameMain* g_pGameMain=nullptr;
GameMain :: GameMain()
{
	m_pPageView = nullptr;
	m_vStartPoint = Point::ZERO;
	m_vEndPoint = Point::ZERO;
	m_btnShop = nullptr;
	m_btnAchievement = nullptr;
	m_btnMagicBox = nullptr;
	m_btnRole = nullptr;
	tSize = Size::ZERO;
	m_btnHP = nullptr;
	m_btnLongBi = nullptr;
	m_btnDiamond = nullptr;
	m_labHP = nullptr;
	m_labLongBi = nullptr;
	m_labDiamond = nullptr;
	g_pGameMain = this;
	normallayer = nullptr;
	challengelayer = nullptr;
}

GameMain :: ~GameMain()
{
}

Scene* GameMain :: createScene()
{
    do {
        Scene* scene = Scene :: create();
        CC_BREAK_IF(!scene);
        GameMain* pLayer = GameMain :: create();
        CC_BREAK_IF(!pLayer);
        scene->addChild(pLayer);
        return scene;
    } while (false);
    log("Fun GameMain :: createScene ERROR");
    return nullptr;
}

bool GameMain :: init()
{
    do{
        CC_BREAK_IF(!Layer::init());

        showUI();
		initMap();

		//Label* pName = Label::createWithSystemFont(GameFunctions::getInstance()->g_sChineseName,"Arial",30.f);
// 		string text = StringUtils::format( "%d",GameUIData::getInstance()->vec_Role.at(20)->m_aUnlockCondition.at(2) );
// 		Label* pName = Label::createWithSystemFont(text,"Arial",30.f);
// 		pName->setPosition(Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height*0.5));
// 		this->addChild(pName,Z_Third);
// 		pName->setColor(Color3B(0,0,0));

        return true;
    } while (false);
    log("Fun GameMain :: init ERROR");
    return false;
}

//
void GameMain :: showUI()
{
    log("------GameMain :: showUI------");
    // 体力框;
    Sprite* tiliRect = Sprite :: create(RESOURCE("tili_01.png"));
    tiliRect->setAnchorPoint(Vec2(0.5f,0.5f));
    tSize = tiliRect->getContentSize();
    tiliRect->setPosition(Vec2(tSize.width*0.5,GLB_SIZE.height-tSize.height*0.5));
    this->addChild(tiliRect,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&tiliRect);
    // 体力背景;
    Sprite* tiliBack = Sprite :: create(RESOURCE("tili_02.png"));
    tiliBack->setAnchorPoint(Vec2(0.5,0.5));
    tiliBack->setPosition(Vec2(tSize.width*0.5,tSize.height*0.5));
    tiliRect->addChild(tiliBack,Z_Back);
    // 体力蓝色体力球;
    cocos2d::ProgressTimer* loadProgress;//进度条;
    loadProgress = ProgressTimer::create(Sprite::create(RESOURCE("tili_03.png")));//创建一个进程条;
    loadProgress->setAnchorPoint(Vec2(0.5,0.5));
    loadProgress->setBarChangeRate(Point(0,1));//设置进程条的变化速率;
    loadProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型;
    loadProgress->setMidpoint(Point(0,0));//设置进度的运动方向;
    loadProgress->setPosition(Vec2(tSize.width*0.5,tSize.height*0.5));
    float percent = static_cast<float>(35+(30.f/60)*40);
    loadProgress->setPercentage(percent);//设置初始值为35;
    tiliBack->addChild(loadProgress,Z_First);

    // 体力按钮和数字;
    m_btnHP = Button::create(RESOURCE("add_001.png"),RESOURCE("add_002.png"));
    m_btnHP->setAnchorPoint(Vec2(0.5f,0.5f));
    m_btnHP->setPosition(Vec2(tSize.width-24,32));
    m_btnHP->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
    m_btnHP->setSwallowTouches(false);
    tiliRect->addChild(m_btnHP,Z_First);
    m_btnHP->setTag(BtnHP);
    m_labHP = Label::createWithCharMap(RESOURCE("shuliang_number.png"),16,27,'0');
    m_labHP->setAnchorPoint(Vec2(0.5f,0.5f));
    m_labHP->setPosition(Vec2(tSize.width*0.5,32));
    m_labHP->setString("30:60");
    tiliRect->addChild(m_labHP,Z_First);
    // 龙币底板，龙币按钮，龙币图标，龙币数字;
    Size dibanSize;
    Sprite* dibanlongbi = Sprite :: create(RESOURCE("diban01.png"));
    dibanSize = dibanlongbi->getContentSize();
    dibanlongbi->setAnchorPoint(Vec2(0.5f,0.5f));
    dibanlongbi->setPosition(Vec2(tSize.width+dibanSize.width*0.5+40,GLB_SIZE.height-dibanSize.height));
    this->addChild(dibanlongbi,Z_First);
    //设置vertex;
    GameFunctions::getInstance()->vertexZ(&dibanlongbi);
    m_btnLongBi = Button::create(RESOURCE("add_01.png"),RESOURCE("add_02.png"));
    m_btnLongBi->setAnchorPoint(Vec2(0.5f,0.5f));
    m_btnLongBi->setPosition(Vec2(dibanSize.width+20,dibanSize.height*0.5));
    m_btnLongBi->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
    m_btnLongBi->setSwallowTouches(false);
    dibanlongbi->addChild(m_btnLongBi);
    m_btnLongBi->setTag(BtnLongBi);
    Sprite* longbi = Sprite::create(RESOURCE("longbi_icon.png"));
    longbi->setAnchorPoint(Vec2(0.5f,0.5f));
    longbi->setPosition(Vec2(-10.f,dibanSize.height*0.5));
    dibanlongbi->addChild(longbi);
    m_labLongBi = Label::createWithCharMap(RESOURCE("shuliang_number.png"),16,27,'0');
    m_labLongBi->setAnchorPoint(Vec2(1.f,0.5f));
    m_labLongBi->setPosition(Vec2(dibanSize.width-20.f,dibanSize.height*0.5));
    m_labLongBi->setString("9999");
    dibanlongbi->addChild(m_labLongBi);
    // 钻石底板，钻石按钮，钻石图标，钻石数字;
    Sprite* dibanDiamond = Sprite :: create(RESOURCE("diban01.png"));
    dibanDiamond->setAnchorPoint(Vec2(0.5f,0.5f));
    dibanDiamond->setPosition(Vec2(tSize.width+dibanSize.width*2+60,GLB_SIZE.height-dibanSize.height));
    this->addChild(dibanDiamond,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&dibanDiamond);
    m_btnDiamond = Button::create(RESOURCE("add_01.png"),RESOURCE("add_02.png"));
    m_btnDiamond->setAnchorPoint(Vec2(0.5f,0.5f));
    m_btnDiamond->setPosition(Vec2(dibanSize.width+20,dibanSize.height*0.5));
    m_btnDiamond->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
    m_btnDiamond->setSwallowTouches(false);
    dibanDiamond->addChild(m_btnDiamond);
    m_btnDiamond->setTag(BtnDiamond);
    Sprite* diamond = Sprite::create(RESOURCE("shuijing_icon.png"));
    diamond->setAnchorPoint(Vec2(0.5f,0.5f));
    diamond->setPosition(Vec2(0.f,dibanSize.height*0.5));
    dibanDiamond->addChild(diamond);
    m_labDiamond = Label::createWithCharMap(RESOURCE("shuliang_number.png"),16,27,'0');
    m_labDiamond->setAnchorPoint(Vec2(1.f,0.5f));
    m_labDiamond->setPosition(Vec2(dibanSize.width-20.f,dibanSize.height*0.5));
    m_labDiamond->setString("99999");
    dibanDiamond->addChild(m_labDiamond);
    //商店按钮;
	m_btnShop = Button :: create(RESOURCE("shangdian_01.png"),RESOURCE("shangdian_02.png"));
	m_btnShop->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnShop->setTag(BtnShop);
	Size btnSize = m_btnShop->getContentSize();
	m_btnShop->setPosition(Vec2(btnSize.width*0.5+10.f,btnSize.height*0.5));
	this->addChild(m_btnShop,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnShop);
	m_btnShop->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnShop->setSwallowTouches(false);
    //成就按钮;
	m_btnAchievement = Button :: create(RESOURCE("chengjiu_01.png"),RESOURCE("chengjiu_02.png"));
	m_btnAchievement->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnAchievement->setTag(BtnAchievement);
	m_btnAchievement->setPosition(Vec2(btnSize.width*1.5+10.f,btnSize.height*0.5));
	this->addChild(m_btnAchievement,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnAchievement);
	m_btnAchievement->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnAchievement->setSwallowTouches(false);
    //魔法包厢;
	m_btnMagicBox = Button :: create(RESOURCE("moxiang_01.png"),RESOURCE("moxiang_02.png"));
	m_btnMagicBox->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnMagicBox->setTag(BtnMagicBox);
	m_btnMagicBox->setPosition(Vec2(btnSize.width*2.5+10.f,btnSize.height*0.5));
	this->addChild(m_btnMagicBox,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnMagicBox);
	m_btnMagicBox->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnMagicBox->setSwallowTouches(false);
    //角色;
	m_btnRole = Button :: create(RESOURCE("juese_01.png"),RESOURCE("juese_02.png"));
	m_btnRole->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnRole->setTag(BtnRole);
	m_btnRole->setPosition(Vec2(btnSize.width*3.5+10.f,btnSize.height*0.5));
	this->addChild(m_btnRole,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnRole);
	m_btnRole->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnRole->setSwallowTouches(false);
	//无尽模式;
	m_btnEndless = Button :: create(RESOURCE("wujin_01.png"),RESOURCE("wujin_02.png"));
	m_btnEndless->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnEndless->setTag(BtnEndless);
	Size btnSize2 = m_btnEndless->getContentSize();
	m_btnEndless->setPosition(Vec2(btnSize2.width*0.5-10.f,GLB_SIZE.height-tSize.height-35.f));
	this->addChild(m_btnEndless,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnEndless);
	m_btnEndless->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnEndless->setEnabled(true);
	m_btnEndless->setOpacity(255);
	m_btnEndless->setSwallowTouches(false);
    //左签到;
	m_btnRegisterLeft = Button :: create(RESOURCE("qiandao_01.png"),RESOURCE("qiandao_02.png"));
	m_btnRegisterLeft->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnRegisterLeft->setTag(BtnRegisterLeft);
	m_btnRegisterLeft->setPosition(Vec2(btnSize2.width*0.5-10.f,GLB_SIZE.height-tSize.height*1.5-70.f));
	this->addChild(m_btnRegisterLeft,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnRegisterLeft);
	m_btnRegisterLeft->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnRegisterLeft->setEnabled(true);
	m_btnRegisterLeft->setOpacity(255);
	m_btnRegisterLeft->setSwallowTouches(false);
    //新手礼包;
	m_btnNewManGift = Button :: create(RESOURCE("xinshou_01.png"),RESOURCE("xinshou_02.png"));
	m_btnNewManGift->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnNewManGift->setTag(BtnNewManGift);
	m_btnNewManGift->setPosition(Vec2(btnSize2.width*0.5-10.f,GLB_SIZE.height-tSize.height*2-95.f));
	this->addChild(m_btnNewManGift,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnNewManGift);
	m_btnNewManGift->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnNewManGift->setEnabled(true);
	m_btnNewManGift->setOpacity(255);
	m_btnNewManGift->setSwallowTouches(false);
    //魔法书;
	m_btnMagicBook = Button :: create(RESOURCE("mofashu_01.png"),RESOURCE("mofashu_02.png"));
	m_btnMagicBook->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnMagicBook->setTag(BtnMagicBook);
	m_btnMagicBook->setPosition(Vec2(GLB_SIZE.width-btnSize2.width*0.5,GLB_SIZE.height-tSize.height-35.f));
	this->addChild(m_btnMagicBook,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnMagicBook);
	m_btnMagicBook->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnMagicBook->setEnabled(false);
	m_btnMagicBook->setOpacity(0);
	m_btnMagicBook->setSwallowTouches(false);
    //右签到;
	m_btnRegisterRight = Button :: create(RESOURCE("qiandao_01.png"),RESOURCE("qiandao_02.png"));
	m_btnRegisterRight->setAnchorPoint(Vec2(0.5f,0.5f));
	m_btnRegisterRight->setTag(BtnRegisterRight);
	m_btnRegisterRight->setPosition(Vec2(GLB_SIZE.width-btnSize2.width*0.5,GLB_SIZE.height-tSize.height*1.5-75.f));
	this->addChild(m_btnRegisterRight,Z_First);
    //设置vertexZ;
    GameFunctions::getInstance()->vertexZ(&m_btnRegisterRight);
	m_btnRegisterRight->addTouchEventListener(CC_CALLBACK_2(GameMain::BtnCall,this));
	m_btnRegisterRight->setEnabled(false);
	m_btnRegisterRight->setOpacity(0);
	m_btnRegisterRight->setSwallowTouches(false);
}

void GameMain::initMap()
{
	m_pPageView = OverScrollView :: create(this);
	this->addChild(m_pPageView);
	m_pPageView->setAnchorPoint(Vec2::ZERO);
	m_pPageView->setPosition(Vec2::ZERO);
	m_pPageView->setClippingToBounds(true);
	m_pPageView->setBounceable(true);
}

Size GameMain::sizeForPerPage()
{
	return Size(GLB_SIZE.width*2-1+_GAP,GLB_SIZE.height);
}

void GameMain::pageViewDidScroll(OverScrollView* scrollView)
{
	//监听滚动事件，滚动时的操作;
	CCLOG("GameMain OverScrollView pageViewDidScroll");
}

void GameMain::setBtnTouch(bool flag)
{
// 	m_btnShop->setEnabled(flag);
// 	m_btnAchievement->setEnabled(flag);
// 	m_btnMagicBox->setEnabled(flag);
// 	m_btnRole->setEnabled(flag);
	m_btnEndless->setEnabled(flag);
	m_btnRegisterLeft->setEnabled(flag);
	m_btnNewManGift->setEnabled(flag);
// 	m_btnHP->setEnabled(flag);
// 	m_btnLongBi->setEnabled(flag);
// 	m_btnDiamond->setEnabled(flag);
	m_btnMagicBook->setEnabled(flag);
	m_btnRegisterRight->setEnabled(flag);
}

void GameMain::BtnCall(Ref* pSender,Widget::TouchEventType type)
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
                    switch(tag)
                    {
                        case BtnShop:
                            onBtnShop();
                            break;
                        case BtnAchievement:
                            onBtnAchievement();
                            break;
                        case BtnMagicBox:
                            onBtnMagicBox();
                            break;
                        case BtnRole:
                            onBtnRole();
                            break;
                        case BtnEndless:
                            onBtnEndless();
                            break;
                        case BtnRegisterLeft:
                            onBtnRegisterLeft();
                            break;
                        case BtnRegisterRight:
                            onBtnRegisterRight();
                            break;
                        case BtnNewManGift:
                            onBtnNewManGift();
                            break;
                        case BtnMagicBook:
                            onBtnMagicBook();
                            break;
                        case BtnHP:
                            onBtnHP();
                            break;
                        case BtnLongBi:
                            onBtnLongBi();
                            break;
                        case BtnDiamond:
                            onBtnDiamond();
                            break;
                    }
                    
                }
            }
            break;
        default:
            break;
	}
}

void GameMain::setLeftBtnFadeInterface(int opacity)
{
	log(" left opacity = %d",opacity);
	m_btnEndless->setOpacity(opacity);
	m_btnRegisterLeft->setOpacity(opacity);
	m_btnNewManGift->setOpacity(opacity);
	m_btnMagicBook->setOpacity(0);
	m_btnRegisterRight->setOpacity(0);
}

void GameMain::setRightBtnFadeInterface(int opacity)
{
	log(" right opacity = %d",opacity);
	m_btnEndless->setOpacity(0);
	m_btnRegisterLeft->setOpacity(0);
	m_btnNewManGift->setOpacity(0);
	m_btnMagicBook->setOpacity(opacity);
	m_btnRegisterRight->setOpacity(opacity);
}

int GameMain::getPageViewId()
{
	return m_pPageView->getCurPageIndex();
}

void GameMain::controlPageView(bool flag)
{
	m_pPageView->listener->setEnabled(flag);
}

void GameMain::controlTableView(bool flag)
{
	normallayer->tableView->setTouchEnabled(flag);		
	challengelayer->tableView->setTouchEnabled(flag);
}

//
void GameMain::ensureBtnFade(int id)
{
	if (id)
	{
		m_btnEndless->setEnabled(false);
		m_btnEndless->setOpacity(0);
		m_btnRegisterLeft->setEnabled(false);
		m_btnRegisterLeft->setOpacity(0);
		m_btnNewManGift->setEnabled(false);
		m_btnNewManGift->setOpacity(0);
		m_btnMagicBook->setEnabled(true);
		m_btnMagicBook->setOpacity(255);
		m_btnRegisterRight->setEnabled(true);
		m_btnRegisterRight->setOpacity(255);
	}
	else
	{
		m_btnEndless->setEnabled(true);
		m_btnEndless->setOpacity(255);
		m_btnRegisterLeft->setEnabled(true);
		m_btnRegisterLeft->setOpacity(255);
		m_btnNewManGift->setEnabled(true);
		m_btnNewManGift->setOpacity(255);
		m_btnMagicBook->setEnabled(false);
		m_btnMagicBook->setOpacity(0);
		m_btnRegisterRight->setEnabled(false);
		m_btnRegisterRight->setOpacity(0);
	}
}

void GameMain::onBtnShop()
{
	log("********GameMain::onBtnShop**********");
    GameMusicControl::getInstance()->btnPlay(1);

}

void GameMain::onBtnAchievement()
{
	log("********GameMain::onBtnAchievement**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnMagicBox()
{
	log("********GameMain::onBtnMagicBox**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnRole()
{
	log("********GameMain::onBtnRole**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnEndless()
{
	log("********GameMain::onBtnEndless**********");
    GameMusicControl::getInstance()->btnPlay(1);
    DataCenter::getInstance()->setTimeLimit(true);
    DataCenter::getInstance()->initMapInfo(0);
    SCENE_CHANGE_NORMAL(SceneState::DDGameUILayer);
    //Director::getInstance()->replaceScene(GameUILayer::gameScene());
}

void GameMain::onBtnRegisterLeft()
{
	log("********GameMain::onBtnRegisterLeft**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnNewManGift()
{
	log("********GameMain::onBtnNewManGift**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnRegisterRight()
{
	log("********GameMain::onBtnRegisterRight**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnMagicBook()
{
	log("********GameMain::onBtnMagicBook**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnHP()
{
	log("********GameMain::onBtnHP**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnLongBi()
{
	log("********GameMain::onBtnLongBi**********");
    GameMusicControl::getInstance()->btnPlay(1);
}

void GameMain::onBtnDiamond()
{
	log("********GameMain::onBtnDiamond**********");
    GameMusicControl::getInstance()->btnPlay(1);
}
