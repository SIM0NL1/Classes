//
//  GameUILayer.cpp
//  Dragon
//
//  Created by B081mac on 15/4/14.
//
//

#include "GameUILayer.h"
#include "DataCenter.h"
#include "VisibleRect.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Progress.h"
#include "UI/GameUIData.h"
using namespace CocosDenshion;

#include "UI/GameSceneState.h"
#include "UI/TDStageLayer.h"

GameUILayer::GameUILayer()
:m_pGameLayer(NULL)
{
    
}

GameUILayer::~GameUILayer()  
{
    
}

Scene* GameUILayer::gameScene()
{
    auto scene = Scene::create();
    
    auto layer = GameUILayer::create();
    
    scene->addChild(layer);
    
    return scene;
}



bool GameUILayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    CCLOG("%s",CCFileUtils::getInstance()->getWritablePath().c_str());
    initTop();
    
    initBG();

    if (DataCenter::getInstance()->getTimeLimit())
    {
        initTimeLimit();
    }
    else
    {
        if (m_pGameLayer)
        {
            m_pGameLayer->removeFromParentAndCleanup(true);
        }
        
        m_pGameLayer = GameLayer::create();
        this->addChild(m_pGameLayer,10);
        m_pGameLayer->initPropSprite();
        
        //TD接口,无尽模式没有Boss;
        Layer* pTDLayer = TDStageLayer::getInstance();
        this->addChild(pTDLayer,8);
        
        this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(GameUILayer::showTarget, this)), NULL)) ;
    }
    
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("battle.mp3",true);
    
    return true;
}

void GameUILayer::initTimeLimit()
{
    if (m_pGameLayer)
    {
        m_pGameLayer->removeFromParentAndCleanup(true);
    }
    
    m_pGameLayer = GameLayer::create();
    this->addChild(m_pGameLayer,10);
    m_pGameLayer->initPropSprite();
    
    m_pGameLayer->runAction(Sequence::create(DelayTime::create(0.2), CallFunc::create(CC_CALLBACK_0(GameLayer::appear, m_pGameLayer)), NULL));
    
//    schedule(schedule_selector(GameUILayer::limitTime), 1.0,CC_REPEAT_FOREVER,2);
    
    m_iScoreNum = 0;
    m_iOneScore = 0;
    m_iScoreTarget = 2000;
    m_iLevel = 0;
    m_labelScore->setString("0");
    m_labelScore->setScale(1);
    
    m_labelScoreTarget->setString("2000");
    
    m_iOperationNum = 60;
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    m_labelOperation->setString(str->getCString());
    
    showLevel();
//    m_targetBG->getAnimation()->playWithIndex(2);
}

void GameUILayer::showLevel()
{
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    this->addChild(backLayerColor,99);
    
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);
    
    auto BG = Armature::create("ui_db1");
    BG->getAnimation()->playWithIndex(0);
    BG->setPosition(VisibleRect::center());
    backLayerColor->addChild(BG,1);
    
    auto sprLev = Sprite::create("guanqia_01.png");
    sprLev->setPosition(Vec2(-80, 0));
    BG->addChild(sprLev,1);
    
    auto labLev = LabelAtlas::create("0", "guanqia_01number.png", 34, 49, '0');
    labLev->setPosition(Vec2(80, 0));
    labLev->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    BG->addChild(labLev,1);
    
    __String *str = __String::createWithFormat("%d",m_iLevel + 1);
    labLev->setString(str->getCString());
    
    BG->runAction(Sequence::create(DelayTime::create(1),FadeOut::create(1),CallFuncN::create(CC_CALLBACK_1(GameUILayer::removeLevel, this , backLayerColor)), NULL));
}

void GameUILayer::removeLevel(Node *pSender, Node *pLayer)
{
    if (m_iLevel == 0)
    {
        pSender->removeFromParentAndCleanup(true);
        auto sprNum = Sprite::create("daojishi03.png");
        pLayer->addChild(sprNum, 2);
        sprNum->setPosition(Vec2(320, 500));
        sprNum->setScale(2);
        sprNum->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.8, 0)) ,CallFuncN::create(CC_CALLBACK_1(GameUILayer::beforeStarLimit, this , 2)) , NULL));
    }
    else
    {
        pSender->getParent()->removeFromParentAndCleanup(true);
        schedule(schedule_selector(GameUILayer::limitTime), 1.0,CC_REPEAT_FOREVER,1);
    }

}

void GameUILayer::beforeStarLimit(Node *pSender, int times)
{
    if (times == 0)
    {
        pSender->getParent()->removeFromParentAndCleanup(true);
        schedule(schedule_selector(GameUILayer::limitTime), 1.0,CC_REPEAT_FOREVER,1);
    }
    else
    {
        auto spr = (Sprite*)pSender;
        __String *str = __String::createWithFormat("daojishi0%d.png" , times);
        spr->setTexture(str->getCString());
        times--;
        spr->setScale(2);
        spr->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.8, 0)) ,CallFuncN::create(CC_CALLBACK_1(GameUILayer::beforeStarLimit, this , times)) , NULL));
    }
}

void GameUILayer::showTarget()
{
    LayerColor* backLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChild(backLayerColor,99);
    
    auto BG = Armature::create("bgmb_02");
    BG->getAnimation()->playWithIndex(0);
    BG->setPosition(VisibleRect::center() - Vec2(0, 145));
    backLayerColor->addChild(BG,1);
    
    BG->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(GameUILayer::removeTargetLayer, this,backLayerColor) );
    
    m_mapTarget = DataCenter::getInstance()->getMapInfo()->getMapTarget();
    
    map<GemType ,int > ::iterator it = m_mapTarget.begin();
    int index = 0;
    while (it != m_mapTarget.end())
    {
        displayTarget(it->first, it->second, backLayerColor, index);
        index++;
        it++;
    }
//    m_targetBG->getAnimation()->playWithIndex(3 - (int)m_mapTarget.size());
    
    initData();
}

void GameUILayer::displayTarget(GemType type, int num ,Node * pLayer ,int index)
{
    __String *str = __String::createWithFormat("%d",num);
    LabelAtlas * labelTarget = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
    labelTarget->setString(str->getCString());
    
    Sprite *sprTarget = Sprite::create();
    string fileName = "";
    switch (type)
    {
        case empty:
            fileName = "icefloor.png";
            break;
        case red:
            fileName = "red.png";
            break;
        case yellow:
            fileName = "yellow.png";
            break;
        case blue:
            fileName = "blue.png";
            break;
        case green:
            fileName = "green.png";
            break;
        case purple:
            fileName = "purple.png";
            break;
        case white:
            fileName = "white.png";
            break;
        case ice:
            fileName = "ice.png";
            break;
        case dragontooth:
            fileName = "tooth.png";
            break;
        default:
            break;
    }
    sprTarget->setTexture(fileName);

    pLayer->addChild(labelTarget ,2);
    pLayer->addChild(sprTarget, 2);
    
    sprTarget->setPosition(VisibleRect::center() - Vec2(100, 130 + index * 80));
    sprTarget->setOpacity(0);
    labelTarget->setPosition(sprTarget->getPosition() + Vec2(150, 0));
    labelTarget->setScale(2);
    labelTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelTarget->setOpacity(0);
    
    //传递过关目标到Boss身上;
    TDStageLayer::getInstance()->passTarget(fileName,num,index);
    
    sprTarget->runAction(Sequence::create(DelayTime::create(0.2 + 0.1 * index),FadeIn::create(0.7), NULL));
    labelTarget->runAction(Sequence::create(DelayTime::create(0.1 * index),CallFuncN::create(CC_CALLBACK_1(GameUILayer::gameLayerFadeIn, this, 0.7)), NULL));
}

void GameUILayer::removeTargetLayer(Node *pSender, Node *pLayer)
{
    gameLayerFadeOut(pLayer,0.8);
    pLayer->runAction(Sequence::create(DelayTime::create(0.9), CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, pLayer)), NULL));
    
    m_pGameLayer->runAction(Sequence::create(DelayTime::create(0.9), CallFunc::create(CC_CALLBACK_0(GameLayer::appear, m_pGameLayer)), NULL));
}

void GameUILayer::initData()
{
//    if (m_pGameLayer)
//    {
//        m_pGameLayer->removeFromParentAndCleanup(true);
//    }
//    
//    m_pGameLayer = GameLayer::create();
//    this->addChild(m_pGameLayer,10);
//    m_pGameLayer->initPropSprite();

    m_bWinGame = false;
    m_iScoreNum = 0;
    m_iOneScore = 0;
    m_labelScore->setString("0");
    m_labelScore->setScale(1);
    DataCenter::getInstance()->setWinnerMode(false);
    
    
    for (int i = 0; i < COLLECT_ARRAY; i++)
    {
        m_bComplete[i] = true;
    }
    
    for(int i=0;i<maxgem;i++)
    {
        m_iArrTargetNum[i]=0;
        m_iArrTargetIndex[i]=0;
    }
    
    MapInfo *info = DataCenter::getInstance()->getMapInfo();
    
    m_iOperationNum = info->getOperationNum();
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    m_labelOperation->setString(str->getCString());
    m_labelOperation1->setString(str->getCString());
    
    m_mapTarget = info->getMapTarget();
    
    map<GemType ,int > ::iterator it = m_mapTarget.begin();
    
    int index = 0;
    while (it != m_mapTarget.end())
    {
        m_iArrTargetNum[it->first] = it->second;
        m_iArrTargetIndex[it->first] = index;
        
//        targetNumAndDisplay(index, it->first, it->second);
        
        m_bComplete[index] = false;
//        m_sprAchieve[index]->setVisible(false);
        
        index++;
        
        it++;
    }
//    m_targetBG->getAnimation()->playWithIndex(3 - (int)m_mapTarget.size());
}

void GameUILayer::initBG()
{
    auto sprSkillBG = Sprite::create("map01_003.png");
    sprSkillBG->setPosition(VisibleRect::bottom() + Vec2(0, sprSkillBG->getBoundingBox().size.height/2));
    this->addChild(sprSkillBG,6);
    
    auto sprGameBG = Sprite::create("map01_001.png");
    sprGameBG->setPosition(VisibleRect::bottom() + Vec2(0, sprGameBG->getBoundingBox().size.height/2));
    this->addChild(sprGameBG,5);
    
    auto sprTree = Sprite::create("map01_002.png");
    sprTree->setPosition(Vec2(0, sprGameBG->getPositionY()) + Vec2(0, sprGameBG->getBoundingBox().size.height/2));
    sprTree->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprTree,8);
    
    auto sprTree_1 = Sprite::create("map01_002.png");
    sprTree_1->setPosition(Vec2(sprTree->getContentSize().width,0));
    sprTree_1->setAnchorPoint(Vec2::ZERO);
    sprTree->addChild(sprTree_1);
    
    auto sprSky = Sprite::create("map01_008.png");
    sprSky->setPosition(sprTree->getPosition());
    sprSky->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprSky,7);
    
    auto sprMontain = Sprite::create("map01_007.png");
    sprMontain->setPosition(sprTree->getPosition());
    sprMontain->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprMontain,6);
    
    auto sprCloud = Sprite::create("map01_006.png");
    sprCloud->setPosition(sprTree->getPosition());
    sprCloud->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprCloud,5);
    
    auto sprState1 = Sprite::create("map01_005.png");
    sprState1->setPosition(sprTree->getPosition());
    sprState1->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprState1,4);
    
    auto sprState2 = Sprite::create("map01_004.png");
    sprState2->setPosition(sprTree->getPosition());
    sprState2->setAnchorPoint(Vec2::ZERO);
    this->addChild(sprState2,3);
    
    auto cloud = Armature::create("xmap01_stone");
    cloud->getAnimation()->playWithIndex(0);
    cloud->setPosition(VisibleRect::top() - Vec2(0 , 160));
    this->addChild(cloud,3);
//
//    auto light = Armature::create("map01_line");
//    light->getAnimation()->playWithIndex(0);
//    light->setPosition(VisibleRect::top() - Vec2(300, sprCloud->getBoundingBox().size.height/2));
//    this->addChild(light,3);
    
    auto sprState3 = Sprite::create("map01_taizi.png");
    sprState3->setPosition(sprGameBG->getPosition() + Vec2(-1.4*sprState3->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState3->getBoundingBox().size.height/3));
    this->addChild(sprState3,4);
    
    auto sprState4 = Sprite::create("map01_taizi.png");
    sprState4->setPosition(sprGameBG->getPosition() + Vec2(2.8*sprState4->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState4->getBoundingBox().size.height*0));
    this->addChild(sprState4,4);
    
    auto sprState5 = Sprite::create("map01_taizi.png");
    sprState5->setPosition(sprGameBG->getPosition() + Vec2(-2.8*sprState5->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState5->getBoundingBox().size.height*0));
    this->addChild(sprState5,4);

    if (!DataCenter::getInstance()->getTimeLimit())
    {
        sprSky->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(3, Vec2(sprSky->getPositionX() - sprSky->getBoundingBox().size.width + 640, sprSky->getPositionY())), NULL));
        sprTree->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(3, Vec2(sprSky->getPositionX() - sprSky->getBoundingBox().size.width + 640, sprSky->getPositionY())), NULL));
        sprMontain->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(3, Vec2(sprMontain->getPositionX() - sprMontain->getBoundingBox().size.width + 640, sprMontain->getPositionY())),NULL));
        sprCloud->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(3, Vec2(sprCloud->getPositionX() - sprCloud->getBoundingBox().size.width + 640, sprCloud->getPositionY())),NULL));
        sprState1->runAction(Sequence::create(DelayTime::create(0.5), MoveTo::create(3, Vec2(sprState1->getPositionX() - sprState1->getBoundingBox().size.width + 640, sprState1->getPositionY())),CallFunc::create(CC_CALLBACK_0(GameUILayer::addBoss,this)),NULL));
    }
    
//    auto sprSkillBG = Sprite::create("map01_00.png");
//    sprSkillBG->setPosition(VisibleRect::bottom() + Vec2(0, sprSkillBG->getBoundingBox().size.height/2));
//    this->addChild(sprSkillBG,6);
//    
//    auto sprGameBG = Sprite::create("map01_01.png");
//    sprGameBG->setPosition(VisibleRect::bottom() + Vec2(0, sprGameBG->getBoundingBox().size.height/2));
//    this->addChild(sprGameBG,5);
//    
//    auto sprTree = Sprite::create("map01_02.png");
//    sprTree->setPosition(VisibleRect::top() + Vec2(0, -sprTree->getBoundingBox().size.height/2));
//    this->addChild(sprTree,3);
//    
//    auto sprSky = Sprite::create("map01_05.png");
//    sprSky->setPosition(VisibleRect::top() + Vec2(0, -sprSky->getBoundingBox().size.height/2));
//    this->addChild(sprSky,0);
//    
//    auto sprMontain = Sprite::create("map01_03.png");
//    sprMontain->setPosition(VisibleRect::top() + Vec2(0, -sprSky->getBoundingBox().size.height/2 - sprMontain->getBoundingBox().size.height/1.5));
//    this->addChild(sprMontain,2);
//    
//    auto sprCloud = Sprite::create("map01_04.png");
//    sprCloud->setPosition(VisibleRect::top() + Vec2(0, -sprCloud->getBoundingBox().size.height));
////    this->addChild(sprCloud,1);
//    
//    auto cloud = Armature::create("map01_yun");
//    cloud->getAnimation()->playWithIndex(0);
//    cloud->setPosition(VisibleRect::top() + Vec2(0, -sprCloud->getBoundingBox().size.height));
//    this->addChild(cloud,1);
//    
//    auto light = Armature::create("map01_line");
//    light->getAnimation()->playWithIndex(0);
//    light->setPosition(VisibleRect::top() - Vec2(300, sprCloud->getBoundingBox().size.height/2));
//    this->addChild(light,3);
//    
//    auto sprState1 = Sprite::create("map01_taizi.png");
//    sprState1->setPosition(sprGameBG->getPosition() + Vec2(0, sprGameBG->getBoundingBox().size.height/2 - sprState1->getBoundingBox().size.height/2));
//    this->addChild(sprState1,4);
//    
//    auto sprState2 = Sprite::create("map01_taizi.png");
//    sprState2->setPosition(sprGameBG->getPosition() + Vec2(1.4*sprState2->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState2->getBoundingBox().size.height/3));
//    this->addChild(sprState2,4);
//    
//    auto sprState3 = Sprite::create("map01_taizi.png");
//    sprState3->setPosition(sprGameBG->getPosition() + Vec2(-1.4*sprState3->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState3->getBoundingBox().size.height/3));
//    this->addChild(sprState3,4);
//    
//    auto sprState4 = Sprite::create("map01_taizi.png");
//    sprState4->setPosition(sprGameBG->getPosition() + Vec2(2.8*sprState4->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState4->getBoundingBox().size.height*0));
//    this->addChild(sprState4,4);
//    
//    auto sprState5 = Sprite::create("map01_taizi.png");
//    sprState5->setPosition(sprGameBG->getPosition() + Vec2(-2.8*sprState5->getBoundingBox().size.width, sprGameBG->getBoundingBox().size.height/2 - sprState5->getBoundingBox().size.height*0));
//    this->addChild(sprState5,4);

}

void GameUILayer::initTop()
{
    auto operationBG = Sprite::create("xui_bushu.png");
    this->addChild(operationBG,8);
    operationBG->setPosition(VisibleRect::top() - Vec2(0, operationBG->getBoundingBox().size.height/2));
    
    m_labelOperation = LabelAtlas::create("0", "bushu_number.png", 22, 33, '0');
    this->addChild(m_labelOperation,11);
    m_labelOperation->setPosition(Vec2(318, 1095));
    m_labelOperation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_labelOperation1 = LabelAtlas::create("0", "bushu_number.png", 22, 33, '0');
    this->addChild(m_labelOperation1,11);
    m_labelOperation1->setPosition(Vec2(318, 1095));
    m_labelOperation1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_labelOperation1->setOpacity(0);
    m_labelOperation1->setScale(1.2);
    
    auto scoreBG = Sprite::create("xui_defen.png");
    this->addChild(scoreBG,8);
    scoreBG->setPosition(VisibleRect::leftTop() + Vec2(scoreBG->getBoundingBox().size.width/2, -scoreBG->getBoundingBox().size.height/1.2));
    
    m_labelScore = LabelAtlas::create("0", "xindefen_number.png", 16, 22, '0');
    this->addChild(m_labelScore,8);
    m_labelScore->setPosition(Vec2(103, 1111));
    m_labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    DataCenter::getInstance()->setPosStar(m_labelOperation->getPosition());
    
    auto *imageItem = MenuItemImage::create("xin_zanting01.png", "xin_zanting02.png", CC_CALLBACK_1(GameUILayer::pauseMenu, this));
    imageItem->setPosition(VisibleRect::rightTop() - Point(imageItem->getBoundingBox().size.width/2 , imageItem->getBoundingBox().size.height/2 ));
    
    auto menu = Menu::create(imageItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 8);
    
    auto levelBg = Sprite::create("xui_guanqia.png");
    this->addChild(levelBg, 9);
    levelBg->setPosition(VisibleRect::rightBottom() - Vec2(levelBg->getBoundingBox().size.width/2, -154));
    
    auto labelLevel = LabelAtlas::create("1", "xinguanqia_number.png", 12, 16, '0');
    this->addChild(labelLevel, 9);
    labelLevel->setPosition(levelBg->getPosition() + Vec2(12,-48));
    labelLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    __String * strLev = __String::createWithFormat("%d" ,GameUIData::getInstance()->getCurNormalMission());
    labelLevel->setString(strLev->getCString());
    
    auto coinBg = Sprite::create("xui_longbi.png");
    this->addChild(coinBg, 9);
    coinBg->setPosition(VisibleRect::leftBottom() + Vec2(levelBg->getBoundingBox().size.width/2, 154));
    
    auto labelCoin = LabelAtlas::create("9999", "xinguanqia_number.png", 12, 16, '0');
    this->addChild(labelCoin, 9);
    labelCoin->setPosition(coinBg->getPosition() + Vec2(6,-50));
    labelCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

//    auto skillBG = Sprite::create("xui_skill.png");
//    this->addChild(skillBG,11);
//    skillBG->setPosition(Vec2(320, 720));
    
    Progress *pro5 = Progress::create("xui_lvkx01.png", "xui_lvkx02.png",true);
    this->addChild(pro5, 8);
    pro5->setPosition(25,516);
    pro5->setProgress(30);
    
    float heigh = pro5->getHeight();
    heigh = heigh/10.0;
    Vec2 starPos = pro5->getPosition() - Vec2(0, 5*heigh);
    
    for (int i = 1; i < 6; i++)
    {
        if (i < 5)
        {
            auto levBG = Sprite::create("xui_lvkuang01.png");
            this->addChild(levBG, 9);
            levBG->setPosition(starPos + Vec2(0, heigh * 2 * i));
        }
        
        
        __String *str = __String::createWithFormat("xui_lv%d.png",i);
        auto lev = Sprite::create(str->getCString());
        this->addChild(lev,10);
        lev->setPosition(starPos + Vec2(0, heigh * 2 * i - 3));
    }
    auto levAchive = Sprite::create("xui_lvkuang02.png");
    this->addChild(levAchive, 9);
    levAchive->setPosition(starPos + Vec2(0, heigh * 2));
    
//    Progress *pro6 = Progress::create("xui_jindu01.png", "xui_jindu02.png",false);
//    this->addChild(pro6, 11);
//    pro6->setPosition(320,670);
//    pro6->setProgress(40);
    
    Progress *pro7 = Progress::create("ui_jindu01.png", "ui_jindu02.png",false);
    this->addChild(pro7, 8);
    pro7->setPosition(pro7->getBoundingBox().size.width/2,1075);
    pro7->setProgress(40);
    
    if (DataCenter::getInstance()->getTimeLimit())
    {
        m_labelScoreTarget = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
        this->addChild(m_labelScoreTarget,8);
        m_labelScoreTarget->setPosition(Vec2(200, 1077));
        m_labelScoreTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_labelScoreTarget->setScale(1.2);

    }
}

//void GameUILayer::initTop()
//{
//    Armature *stepBg = Armature::create("bushu_ui");
//    this->addChild(stepBg,8);
//    stepBg->getAnimation()->playWithIndex(0);
//    stepBg->setPosition(VisibleRect::top() - Vec2(-105, stepBg->getBoundingBox().size.height/2 + 8));
//    
//    m_labelOperation = LabelAtlas::create("0", "bushu_number.png", 22, 33, '0');
//    this->addChild(m_labelOperation,8);
//    m_labelOperation->setPosition(Vec2(423, 1082));
//    m_labelOperation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    
//    m_labelOperation1 = LabelAtlas::create("0", "bushu_number.png", 22, 33, '0');
//    this->addChild(m_labelOperation1,8);
//    m_labelOperation1->setPosition(Vec2(423, 1082));
//    m_labelOperation1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    m_labelOperation1->setOpacity(0);
//    m_labelOperation1->setScale(1.2);
//    
//    m_labelScore = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
//    this->addChild(m_labelScore,8);
//    m_labelScore->setPosition(Vec2(533, 1080));
//    m_labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    
//    DataCenter::getInstance()->setPosStar(m_labelOperation->getPosition());
//    
//    auto *imageItem = MenuItemImage::create("ui_zanting01.png", "ui_zanting02.png", CC_CALLBACK_1(GameUILayer::pauseMenu, this));
//    imageItem->setPosition(VisibleRect::rightTop() - Point(imageItem->getBoundingBox().size.width/2 , imageItem->getBoundingBox().size.height/2 ));
//    
//    auto menu = Menu::create(imageItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 8);
//    
//    auto levelBg = Sprite::create("ui_gkdiban.png");
//    this->addChild(levelBg, 11);
//    levelBg->setPosition(VisibleRect::right() - Vec2(levelBg->getBoundingBox().size.width/2, -124));
//    
//    auto labelLevel = LabelAtlas::create("1", "guanqia_number.png", 12, 16, '0');
//    this->addChild(labelLevel, 11);
//    labelLevel->setPosition(levelBg->getPosition() + Vec2(23, 6));
//    labelLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    
//    
//    auto coinBg = Sprite::create("ui_longbidiban.png");
//    this->addChild(coinBg, 11);
//    coinBg->setPosition(VisibleRect::left() + Vec2(levelBg->getBoundingBox().size.width/2, 124));
//    
//    auto labelCoin = LabelAtlas::create("9999", "guanqia_number.png", 12, 16, '0');
//    this->addChild(labelCoin, 11);
//    labelCoin->setPosition(coinBg->getPosition() + Vec2(4, 6));
//    labelCoin->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    
//    auto coin = Sprite::create("ui_longbi.png");
//    this->addChild(coin, 11);
//    coin->setPosition(coinBg->getPosition() - Vec2(45, -10));
//    
//    
//    m_targetBG = Armature::create("guanqiamubiao_ui");
//    this->addChild(m_targetBG, 8);
//    m_targetBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//    m_targetBG->setPosition(VisibleRect::leftTop() + Vec2(m_targetBG->getBoundingBox().size.width/2 + 10, 0));
//    m_targetBG->getAnimation()->playWithIndex(0);
//    
//    if (!DataCenter::getInstance()->getTimeLimit())
//    {
//        float distance = 40;
//        for (int i = 0; i < COLLECT_ARRAY; i++)
//        {
//            m_sprTarget[i] = Sprite::create("red.png");
//            m_sprTarget[i]->setScale(0.45);
//            m_sprTarget[i]->setPosition(m_targetBG->getPosition() - Point( m_sprTarget[i]->getBoundingBox().size.height/1.5, distance * i + 65));
//            this->addChild(m_sprTarget[i], 8);
//            m_sprTarget[i]->setVisible(false);
//            
//            m_labelTarget[i] = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
//            m_labelTarget[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
//            m_labelTarget[i]->setPosition(m_targetBG->getPosition() - Point( -m_sprTarget[i]->getBoundingBox().size.height/1.5, distance * i + 52));
//            this->addChild(m_labelTarget[i], 9);
//            m_labelTarget[i]->setVisible(false);
//            
//            m_sprAchieve[i] = Sprite::create("wancheng_ui.png");
//            m_sprAchieve[i]->setPosition(m_targetBG->getPosition() - Point( -m_sprTarget[i]->getBoundingBox().size.height/1.5, distance * i + 62));
//            this->addChild(m_sprAchieve[i], 8);
//            m_sprAchieve[i]->setVisible(false);
//        }
//        
//        
////        Progress *pro1 = Progress::create("ui_nuqi01.png", "ui_nuqi02.png");
////        this->addChild(pro1, 8);
////        pro1->setPosition(320,705);
////        pro1->setProgress(100);
////        
////        Progress *pro2 = Progress::create("ui_nuqi01.png", "ui_nuqi02.png");
////        this->addChild(pro2, 8);
////        pro2->setPosition(60,741);
////        pro2->setProgress(80);
////        
////        Progress *pro3 = Progress::create("ui_nuqi01.png", "ui_nuqi02.png");
////        this->addChild(pro3, 8);
////        pro3->setPosition(188,718);
////        pro3->setProgress(50);
////        
////        Progress *pro4 = Progress::create("ui_nuqi01.png", "ui_nuqi02.png");
////        this->addChild(pro4, 8);
////        pro4->setPosition(450,718);
////        pro4->setProgress(20);
////        
////        Progress *pro5 = Progress::create("ui_nuqi01.png", "ui_nuqi02.png");
////        this->addChild(pro5, 8);
////        pro5->setPosition(580,741);
////        pro5->setProgress(60);
////        
////        Progress *pro6 = Progress::create("ui_hpboss01.png", "ui_hpboss02.png");
////        this->addChild(pro6, 8);
////        pro6->setPosition(315,1040);
////        pro6->setProgress(40);
//        
//    }
//    else
//    {
//        m_labelScoreTarget = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
//        this->addChild(m_labelScoreTarget,8);
//        m_labelScoreTarget->setPosition(m_targetBG->getPosition() - Vec2(0, m_targetBG->getBoundingBox().size.height/2.4));
//        m_labelScoreTarget->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        m_labelScoreTarget->setScale(1.2);
//    }
//    
//    Progress *pro7 = Progress::create("ui_jindu01.png", "ui_jindu02.png");
//    this->addChild(pro7, 8);
//    pro7->setPosition(530,1055);
//    pro7->setProgress(40);
//    
////    auto star1 = Sprite::create("ui_star02.png");
////    this->addChild(star1, 18);
////    star1->setPosition(pro7->getPosition() - Vec2(20, 0));
////    
////    auto star2 = Sprite::create("ui_star02.png");
////    this->addChild(star2, 18);
////    star2->setPosition(pro7->getPosition() + Vec2(20, 0));
////    
////    auto star3 = Sprite::create("ui_star02.png");
////    this->addChild(star3, 18);
////    star3->setPosition(pro7->getPosition() + Vec2(60, 0));
//}

void GameUILayer::targetNumAndDisplay(int index,GemType type, int num)
{
    m_labelTarget[index]->setVisible(true);
    m_sprTarget[index]->setVisible(true);
    
    __String *str = __String::createWithFormat("%d",num);
    m_labelTarget[index]->setString(str->getCString());
    
    switch (type)
    {
        case empty:
            m_sprTarget[index]->setTexture("icefloor.png");
            break;
        case red:
            m_sprTarget[index]->setTexture("red.png");
            break;
        case yellow:
            m_sprTarget[index]->setTexture("yellow.png");
            break;
        case blue:
            m_sprTarget[index]->setTexture("blue.png");
            break;
        case green:
            m_sprTarget[index]->setTexture("green.png");
            break;
        case purple:
            m_sprTarget[index]->setTexture("purple.png");
            break;
        case white:
            m_sprTarget[index]->setTexture("white.png");
            break;
        case ice:
            m_sprTarget[index]->setTexture("ice.png");
            break;
        case dragontooth:
            m_sprTarget[index]->setTexture("tooth.png");
            break;
        default:
            break;
    }
    
    DataCenter::getInstance()->initMapPoint(type, m_sprTarget[index]->getPosition());
}

void GameUILayer::addBlackLayer(LAYER_TYPE type)
{
    //添加一个半灰色的层
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    this->addChild(backLayerColor,99);
    
    auto BG = Armature::create("ui_db1");
    BG->getAnimation()->playWithIndex(1);
    BG->setPosition(VisibleRect::center());
    
    backLayerColor->addChild(BG,1);
    
    //添加向下触摸屏蔽
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);
    
    
    
    switch (type)
    {
        case layerPause:
        {
            
        }
            break;
        case layerTime:
        {
            BG->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(GameUILayer::addTimeDown, this,backLayerColor) );
        }
            break;
        default:
            break;
    }

}

void GameUILayer::pauseMenu(Ref *pSender)
{
    pauseGame();
    //添加一个半灰色的层
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    this->addChild(backLayerColor,99);
    
    auto BG = Armature::create("ui_db1");
    BG->getAnimation()->playWithIndex(0);
    BG->setPosition(VisibleRect::center());
    backLayerColor->addChild(BG,1);

    auto cloud = Armature::create("ui_zanting");
    cloud->getAnimation()->playWithIndex(0);
    cloud->setPosition(VisibleRect::center() + Vec2(0, 30));
    backLayerColor->addChild(cloud,2);
    
    
        
    //添加向下触摸屏蔽
    auto callback = [](Touch * ,Event *)
    {
        return true;
    };
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = callback;
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);
    
    auto *labelItemRestar = MenuItemImage::create("icon_chonglai.png", "icon_chonglai2.png", bind(&GameUILayer::restarGame, this, std::placeholders::_1, backLayerColor));
    
    auto *labelItemQuit = MenuItemImage::create("icon_zhucaidan.png", "icon_zhucaidan2.png", CC_CALLBACK_1(GameUILayer::quitGame, this));
    
    auto *labelItemContinue = MenuItemImage::create("icon_jixu.png", "icon_jixu2.png", bind(&GameUILayer::continueGame, this, std::placeholders::_1, backLayerColor));
    
    auto *labelItemCha = MenuItemImage::create("icon_guanbi01_1.png", "icon_guanbi01_2.png", bind(&GameUILayer::continueGame, this, std::placeholders::_1, backLayerColor));
    labelItemCha->setOpacity(0);
    labelItemCha->runAction(FadeIn::create(1));
    auto menu = Menu::create(labelItemQuit,labelItemRestar,labelItemContinue, NULL);
    menu->setPosition(VisibleRect::center() - Vec2(0, 85));
    menu->alignItemsHorizontallyWithPadding(100);
    backLayerColor->addChild(menu,2);
    
    auto menu2 = Menu::create(labelItemCha, NULL);
    menu2->setPosition(cloud->getPosition() + Vec2(265, 65));
    backLayerColor->addChild(menu2,2);
}


void GameUILayer::quitGame(Ref *pSender)
{
    resumeGame();
    DataCenter::getInstance()->setWinnerMode(false);
    DataCenter::getInstance()->destroyMapInfo();
    DataCenter::getInstance()->setTimeLimit(false);
    TDStageLayer::getInstance()->destroyInstance();
    SCENE_CHANGE_NORMAL(SceneState::UIGameMain);
}

void GameUILayer::continueGame(Ref *pSender, Node *pNode)
{
    resumeGame();
    pNode->removeFromParentAndCleanup(true);
}

void GameUILayer::add5Step(Ref *pSender, Node *pNode)
{
    m_iOperationNum += 5;
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    if (m_labelOperation->getOpacity())
    {
        m_labelOperation->setString(str->getCString());
    }
    else
    {
        m_labelOperation1->setString(str->getCString());
    }
    pNode->removeFromParentAndCleanup(true);
}

void GameUILayer::restarGame(Ref *pSender,Node* pNode)
{
    resumeGame();
    if (DataCenter::getInstance()->getTimeLimit())
    {
        pNode->removeFromParentAndCleanup(true);
        gameLayerFadeOut(m_pGameLayer , 0.8);
        unschedule(schedule_selector(GameUILayer::limitTime));
        this->runAction(Sequence::create(DelayTime::create(0.9), CallFunc::create(CC_CALLBACK_0(GameUILayer::initTimeLimit, this)), NULL));
    }
    else
    {
        TDStageLayer::getInstance()->destroyInstance();
        SCENE_CHANGE_NORMAL(SceneState::UIGameMissionSet);
    }
}

void GameUILayer::resumeGame()
{
    Director::getInstance()->resume();
}

void GameUILayer::pauseGame()
{
    Director::getInstance()->pause();
}

void GameUILayer::updateOperationNum(Ref *obj)
{
    if (!DataCenter::getInstance()->getTimeLimit())
    {
        float time = 0;
        if (m_bWinGame)
        {
            time = 0.2;
        }
        else
        {
            time = 0.3;
        }
        if (m_labelOperation->getOpacity())
        {
            m_labelOperation->runAction(Sequence::create(ScaleTo::create(time/2, 1.5),ScaleTo::create(time/2, 0.5) , CallFuncN::create(CC_CALLBACK_1(GameUILayer::hideOperation, this)), NULL));
            m_labelOperation1->runAction(Sequence::create(DelayTime::create(time/2) , Spawn::create(FadeIn::create(time/2),ScaleTo::create(time/2, 1), NULL), NULL));
            
            m_iOperationNum--;
            __String *str = __String::createWithFormat("%d",m_iOperationNum);
            m_labelOperation1->setString(str->getCString());

        }
        else
        {
            m_labelOperation1->runAction(Sequence::create(ScaleTo::create(time/2, 1.5),ScaleTo::create(time/2, 0.5) , CallFuncN::create(CC_CALLBACK_1(GameUILayer::hideOperation, this)), NULL));
            m_labelOperation->runAction(Sequence::create(DelayTime::create(time/2) , Spawn::create(FadeIn::create(time/2),ScaleTo::create(time/2, 1), NULL), NULL));
            
            m_iOperationNum--;
            __String *str = __String::createWithFormat("%d",m_iOperationNum);
            m_labelOperation->setString(str->getCString());

        }
    }
}

void GameUILayer::operationAnimaOver()
{
    m_iOperationNum--;
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    m_labelOperation->setString(str->getCString());
}

void GameUILayer::hideOperation(Node *pNode)
{
    pNode->setOpacity(0);
    pNode->setScale(1.2);
}

void GameUILayer::failGame()
{
    if (DataCenter::getInstance()->getTimeLimit())
    {
        LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
        this->addChild(backLayerColor,99);
        
        auto callback = [](Touch * ,Event *)
        {
            return true;
        };
        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = callback;
        listener->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);
        
        auto BG = Armature::create("ui_db1");
        BG->getAnimation()->playWithIndex(0);
        BG->setPosition(VisibleRect::center());
        backLayerColor->addChild(BG,1);
        
        auto word = Armature::create("ui_shijianyongjin");
        word->getAnimation()->playWithIndex(0);
        word->setPosition(Vec2::ZERO);
        BG->addChild(word,1);
        
        BG->runAction(Sequence::create(DelayTime::create(1),FadeOut::create(1),CallFuncN::create(CC_CALLBACK_1(GameUILayer::failLayer, this , backLayerColor)), NULL));
    }
    else
    {
        addBlackLayer(layerTime);
    }
    
//    Director::getInstance()->pause();
//    
//    //添加一个半灰色的层
//    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
//    this->addChild(backLayerColor,99);
//    //添加向下触摸屏蔽
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);
//    
//    auto *labelItemRestar = MenuItemLabel::create(Label::createWithSystemFont("重新挑战", "",30 ), bind(&GameUILayer::restarGame, this, std::placeholders::_1, backLayerColor));
//    labelItemRestar->setPosition(Vec2::ZERO);
//    
//    auto *labelItemQuit = MenuItemLabel::create(Label::createWithSystemFont("升级宠物", "",30 ), CC_CALLBACK_1(GameUILayer::quitGame, this));
//    labelItemQuit->setPosition(Vec2::ZERO);
//    
//    
//    auto menu = Menu::create(labelItemRestar,labelItemQuit, NULL);
//    menu->setPosition(VisibleRect::center());
//    menu->alignItemsHorizontallyWithPadding(20);
//    backLayerColor->addChild(menu);

}

void GameUILayer::addTimeDown(Node *pSender, Node *pLayer)
{
    auto time = Armature::create("djs_01");
    time->getAnimation()->playWithIndex(0);
    time->setPosition(VisibleRect::center() + Vec2(0, 230));
    time->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(GameUILayer::failLayer, this,pLayer) );
    pLayer->addChild(time,2);
    
    auto words = Armature::create("ui_jia501");
    words->getAnimation()->playWithIndex(0);
    words->setPosition(VisibleRect::center() + Vec2(0, 30));
    pLayer->addChild(words,2);

    auto add = Armature::create("jiawu_icon");
    pLayer->addChild(add,2);
    add->setPosition(VisibleRect::center() - Vec2(100, 50));
    add->getAnimation()->play("Animation1");
    
    auto *labelItemRestar = MenuItemImage::create("icon_queding.png", "icon_queding2.png", bind(&GameUILayer::add5Step, this, std::placeholders::_1, pLayer));
    labelItemRestar->setPosition(Vec2::ZERO);

    
    auto menu = Menu::create(labelItemRestar, NULL);
    menu->setPosition(VisibleRect::center() + Vec2(50, -50));
    pLayer->addChild(menu,2);

}

 void GameUILayer::failLayer(Node* pSender , Node * pLayer)
{
    pLayer->removeAllChildren();
    
    auto BG = Armature::create("ui_db1");
    BG->getAnimation()->playWithIndex(1);
    BG->setPosition(VisibleRect::center());
    pLayer->addChild(BG,1);
    
    auto fail = Armature::create("ui_cgsb01");
    fail->getAnimation()->playWithIndex(0);
    fail->setPosition(VisibleRect::center() + Vec2(0, 50));
    pLayer->addChild(fail,2);

    auto *labelItemRestar = MenuItemImage::create("icon_cxtz.png", "icon_cxtz2.png", bind(&GameUILayer::restarGame, this, std::placeholders::_1, pLayer));
    
    auto *labelItemQuit = MenuItemImage::create("icon_jinhua.png", "icon_jinhua2.png", CC_CALLBACK_1(GameUILayer::quitGame, this));
    
    auto menu = Menu::create(labelItemQuit,labelItemRestar,NULL);
    menu->setPosition(VisibleRect::center() - Vec2(0, 65));
    menu->alignItemsHorizontallyWithPadding(100);
    pLayer->addChild(menu,2);

}

void GameUILayer::matchOver(Ref *obj)
{
    if (!DataCenter::getInstance()->getTimeLimit())
    {
        limitOperatiomNum();
    }
}

void GameUILayer::limitOperatiomNum()
{
    if (m_bWinGame)
    {
        //过关条件达成;
        TDStageLayer::getInstance()->targetFinish();
        
        if (m_iOperationNum == 0)
        {
            if (!m_pGameLayer->beforeWinnerMode())
            {
                if (m_pGameLayer)
                {
                    gameLayerFadeOut(m_pGameLayer , 0.8);
                }
                this->runAction(Sequence::create(DelayTime::create(0.9),CallFunc::create(CC_CALLBACK_0(GameUILayer::addWinLayer, this)), NULL));
            }
            else
            {
                __String *str = __String::createWithFormat("%d",m_iOperationNum);
                NotificationCenter::getInstance()->postNotification(kMSG_WinnerModeStart, str);
            }
//            DataCenter::getInstance()->setWinnerMode(true);
//            MessageBox("YOU WIN !!!", "Congratulate");
        }
        else
        {
            if (!DataCenter::getInstance()->getWinnerMode())
            {
                m_pGameLayer->setTouchDisable();
                DataCenter::getInstance()->setWinnerMode(true);
                auto BG = Armature::create("ui_db1");
                BG->getAnimation()->playWithIndex(1);
                BG->setPosition(VisibleRect::center());
                this->addChild(BG,11);
                BG->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(GameUILayer::completeWords, this) );
            }
            else
            {
                __String *str = __String::createWithFormat("%d",m_iOperationNum);
                NotificationCenter::getInstance()->postNotification(kMSG_WinnerModeStart, str);
            }
        }
    }
    else
    {
        if (m_iOperationNum == 0)
        {
            failGame();
        }
        if (!DataCenter::getInstance()->getTimeLimit())
        {
            //TD接口;
            CCLOG("m_iOneScore = %d",m_iOneScore);
            TDStageLayer::getInstance()->soldierFactory(m_iOneScore);
            m_iOneScore = 0;
        }
    }

}

void GameUILayer::completeWords(Node *pSender)
{
    GemAction::getInstance().playEffectMusic(NULL, "uiwin.mp3");
    auto BG = Armature::create("ui_mbdc01");
    BG->getAnimation()->playWithIndex(0);
    BG->setPosition(Vec2::ZERO);
    pSender->addChild(BG,1);
    
    pSender->runAction(Sequence::create(DelayTime::create(1.5) ,FadeOut::create(1), CallFuncN::create(CC_CALLBACK_1(GameUILayer::winnerMode, this) ), NULL));
}

void GameUILayer::winnerMode(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    NotificationCenter::getInstance()->postNotification(kMSG_WinnerModeStart, str);
}

void GameUILayer::limitTime(float dt)
{
    m_iOperationNum--;
    __String *str = __String::createWithFormat("%d",m_iOperationNum);
    m_labelOperation->setString(str->getCString());
    
    if (m_iOperationNum == 0)
    {
        failGame();
        unschedule(schedule_selector(GameUILayer::limitTime));
    }
//    Director::getInstance()->getScheduler()->pauseTarget(this);
//    Director::getInstance()->getScheduler()->resumeTarget(this);
}

void GameUILayer::gameLayerFadeOut(Node *pSender  ,float times)
{
    if (pSender)
    {
        pSender->runAction(FadeOut::create(times));
    }
    Vector<Node*> children = pSender->getChildren();
    
    if (children.size() > 0)
    {
        for (int i = 0; i < children.size(); i++)
        {
            gameLayerFadeOut(children.at(i),times);
        }
    }
}

void GameUILayer::gameLayerFadeIn(Node *pSender  ,float times)
{
    if (pSender)
    {
        pSender->runAction(FadeIn::create(times));
    }
    Vector<Node*> children = pSender->getChildren();
    
    if (children.size() > 0)
    {
        for (int i = 0; i < children.size(); i++)
        {
            gameLayerFadeIn(children.at(i),times);
        }
    }
}

//void GameUILayer::gameLayerSetOpacity(Node *pSender)
//{
//    if (pSender)
//    {
//        pSender->setOpacity(0);
//    }
//    Vector<Node*> children = pSender->getChildren();
//    
//    if (children.size() > 0)
//    {
//        for (int i = 0; i < children.size(); i++)
//        {
//            gameLayerSetOpacity(children.at(i));
//        }
//    }
//}


void GameUILayer::addWinLayer()
{
    LayerColor* backLayerColor = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChild(backLayerColor,99);

    auto BG = Armature::create("ui_db1");
    BG->getAnimation()->playWithIndex(1);
    BG->setPosition(VisibleRect::center() - Vec2(0, 370));
    backLayerColor->addChild(BG,1);
    
    auto fail = Armature::create("ui_diwen1");
    fail->getAnimation()->playWithIndex(0);
    fail->setPosition(VisibleRect::center() - Vec2(0, 96));
    backLayerColor->addChild(fail,2);
    
    auto score = Armature::create("ui_fs");
    score->getAnimation()->playWithIndex(0);
    score->setPosition(VisibleRect::center() - Vec2(200, 330));
    backLayerColor->addChild(score,2);
    
    auto reward = Armature::create("ui_ggjl01");//ui_fz01
    reward->getAnimation()->playWithIndex(0);
    reward->setPosition(VisibleRect::center() - Vec2(200, 390));
    backLayerColor->addChild(reward,2);
    
    auto star1 = Armature::create("ui_fz01");//ui_fz01
    star1->getAnimation()->playWithIndex(0);
    star1->setPosition(VisibleRect::center() - Vec2(175, 153));
    backLayerColor->addChild(star1,2);
    
    auto star2 = Armature::create("ui_fz01");//ui_fz01
    star2->getAnimation()->playWithIndex(0);
    star2->setPosition(VisibleRect::center() + Vec2(-2, 25));
    backLayerColor->addChild(star2,2);
    
    auto star3 = Armature::create("ui_fz01");//ui_fz01
    star3->getAnimation()->playWithIndex(0);
    star3->setPosition(VisibleRect::center() - Vec2(-180, 153));
    backLayerColor->addChild(star3,2);
    
    //添加向下触摸屏蔽
//    auto callback = [](Touch * ,Event *)
//    {
//        return true;
//    };
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = callback;
//    listener->setSwallowTouches(true);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,backLayerColor);


    auto *labelItemRestar = MenuItemImage::create("icon_chonglai.png", "icon_chonglai2.png", bind(&GameUILayer::restarGame, this, std::placeholders::_1, backLayerColor));
    
    auto *labelItemQuit = MenuItemImage::create("icon_zhucaidan.png", "icon_zhucaidan2.png", CC_CALLBACK_1(GameUILayer::quitGame, this));
    
    auto *labelItemContinue = MenuItemImage::create("icon_xiayiguan2.png", "icon_xiayiguan2.png", CC_CALLBACK_1(GameUILayer::quitGame, this));
    
    auto *labelItemCha = MenuItemImage::create("icon_guanbi01_1.png", "icon_guanbi01_2.png", CC_CALLBACK_1(GameUILayer::quitGame, this));
    labelItemCha->setOpacity(0);
    labelItemCha->runAction(FadeIn::create(1));
    auto menu = Menu::create(labelItemQuit,labelItemRestar,labelItemContinue, NULL);
    menu->setPosition(VisibleRect::center() - Vec2(0, 480));
    menu->alignItemsHorizontallyWithPadding(150);
    backLayerColor->addChild(menu,2);
    
    auto menu2 = Menu::create(labelItemCha, NULL);
    menu2->setPosition(BG->getPosition() + Vec2(265, 95));
    backLayerColor->addChild(menu2,2);

    __String *str = __String::createWithFormat("%d",m_iScoreNum);
    LabelAtlas * labelScore = LabelAtlas::create("0", "defen_number.png", 16, 22, '0');
    labelScore->setString(str->getCString());
    backLayerColor->addChild(labelScore,2);
    labelScore->setPosition(Vec2(VisibleRect::center().x, score->getPositionY()));
    labelScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelScore->setScale(2);
}

void GameUILayer::updateCollect(Ref *obj)
{
    __String *str = (__String*)obj;
    int iMsg = str->intValue();
    
    int times = iMsg/1000;
    m_iScoreNum += times * 10;
    m_iOneScore += times * 10;
    __String *score = __String::createWithFormat("%d",m_iScoreNum);
    m_labelScore->setString(score->getCString());
    m_labelScore->setScale(1);
    m_labelScore->stopAllActions();
    m_labelScore->runAction(Sequence::create(ScaleTo::create(0.1, 1.2), ScaleTo::create(0.1, 1),ScaleTo::create(0.1, 1.2),ScaleTo::create(0.1, 1),NULL));

    if (!DataCenter::getInstance()->getTimeLimit())
    {
        GemType type = (GemType)(iMsg%100);
        int index = m_iArrTargetIndex[type];
        if (m_iArrTargetNum[type] > 0)
        {
            m_iArrTargetNum[type]--;
            
            //更新Boss身上的目标值;
            TDStageLayer::getInstance()->updateBossTag(m_iArrTargetNum[type],index);
            
            if (m_iArrTargetNum[type] == 0)
            {
//                m_labelTarget[index]->setVisible(false);
                m_bComplete[index] = true;
//                m_sprAchieve[index]->setVisible(true);
//                m_sprAchieve[index]->setScale(4);
//                m_sprAchieve[index]->runAction(ScaleTo::create(0.2, 1));
            }
            else
            {
//                __String *str = __String::createWithFormat("%d",m_iArrTargetNum[type]);
//                m_labelTarget[index]->setString(str->getCString());
            }
        }
        
        m_bWinGame = true;
        
        for (int i = 0; i < COLLECT_ARRAY; i++)
        {
            if (m_bComplete[i] == false)
            {
                m_bWinGame = false;
            }
        }

    }
    else
    {
        if (m_iScoreNum >= m_iScoreTarget)
        {
            m_iLevel++;
            m_iScoreTarget = DataCenter::getInstance()->getMapInfo()->getPassScore(m_iLevel);
            m_iOperationNum += DataCenter::getInstance()->getMapInfo()->getPassTime();
            m_iOperationNum = m_iOperationNum < 60 ? m_iOperationNum : 60;
            __String *str = __String::createWithFormat("%d",m_iOperationNum);
            m_labelOperation->setString(str->getCString());
            
            str = __String::createWithFormat("%d",m_iScoreTarget);

            m_labelScoreTarget->setString(str->getCString());
            
            unschedule(schedule_selector(GameUILayer::limitTime));
            
            showLevel();
        }
    }
}

void GameUILayer::onEnter()
{
    Layer::onEnter();
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::updateOperationNum), kMSG_UpdateMoves, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::matchOver), kMSG_MatchDown, NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameUILayer::updateCollect), kMSG_ShowCollecte, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(GameUILayer::bossDied), csMSG_BOSSDIED, NULL);
}

void GameUILayer::onExit()
{
    NotificationCenter::getInstance()->removeObserver(this, kMSG_UpdateMoves);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_MatchDown);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_ShowCollecte);
    
    NotificationCenter::getInstance()->removeObserver(this, csMSG_BOSSDIED);
    
    Layer::onExit();
}

void GameUILayer::addBoss()
{
    TDStageLayer::getInstance()->bossFactory(1);
}

void GameUILayer::bossDied(Ref *obj)
{
    //当前所有Boss都已干掉;
    CCLOG("All Bosses are Deaded..............");
}
