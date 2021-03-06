//
//  DuplicateLayer.cpp
//  DragonGame
//
//  Created by B081mac on 15/7/9.
//
//

#include "DuplicateLayer.h"
#include "DuplicateLayer.h"
#include "SwallowLayer.h"
#include "DataCenter.h"
#include "Progress.h"
#include <math.h>
#include <time.h>


DuplicateLayer::DuplicateLayer()
:_isMoveDone(true)
,_isStartMove(false)
,_isFallDownEnd(false)
,_fallGemCount(0)
,_edgeBlankDevidedSize(10)
,_isWinnerModeStart(false)
,_propkind(PropNone)
,_skillType(SkillNull)
,_magicCallBack(NULL)
,_propCar(NULL)
,_propStarstNode(NULL)
,_winnerModeStarsNode(NULL)
,_matchCollectTimes(0)
,_matchRepeatReminder(NULL)
,_continueMatchTimes(0)
,_hightLightSwitch(true)
,_bossNode(NULL)
,_upIndex(NULL)
,_hammerSprite(NULL)
,_aroundSprite(NULL)
,_add5stepSprite(NULL)
,_crossSprite(NULL)
,_onekindSprite(NULL)
,_removeGemSwitch(true)
,_matchDownMSGSwitch(false)
,_disappearByOder(false)
,_explodeAll(false)
{
    highLightMovePoint.setPosition(-1, -1);
    highLightPoint1.setPosition(-1, -1);
    highLightPoint2.setPosition(-1, -1);
    _selectPoint.reset();
    _nextPoint.reset();
}

DuplicateLayer::~DuplicateLayer()
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->removeFromParentAndCleanup(true);
                _gemMatrix[i][j]=NULL;
            }
        }
    }
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        free(_gemMatrix[i]);
        free(_mapMatrix[i]);
    }
    free(_gemMatrix);
    free(_mapMatrix);
    
    if(_mapLayer)
    {
        delete _mapLayer;
    }
    
    if(_particleNode)
    {
        _particleNode->removeFromParentAndCleanup(true);
    }
    if(_winnerModeStarsNode)
    {
        _winnerModeStarsNode->removeFromParentAndCleanup(true);
    }
    if(_propStarstNode)
    {
        _propStarstNode->removeFromParentAndCleanup(true);
    }
    if(_mapNode)
    {
        _mapNode->removeFromParentAndCleanup(true);
    }
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void DuplicateLayer::initPropSprite()
{
    _hammerSprite = Armature::create("chuizi_icon");
    this->addChild(_hammerSprite);
    _hammerSprite->setPosition(this->convertToNodeSpace(Vec2(74 , 39)));
    _hammerSprite->getAnimation()->play("Animation1");
    
    auto numBg = Sprite::create("ui_kuang03.png");
    _hammerSprite->addChild(numBg,10);
    numBg->setPosition(Vec2(-35,-25));
    
    auto labelLevel = LabelAtlas::create("1", "shuliang02_number.png", 12, 16, '0');
    _hammerSprite->addChild(labelLevel,10);
    labelLevel->setPosition(numBg->getPosition());
    labelLevel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _crossSprite = Armature::create("shizi_icon");
    this->addChild(_crossSprite);
    _crossSprite->setPosition(this->convertToNodeSpace(Vec2(172 , 39)));
    _crossSprite->getAnimation()->play("Animation1");
    
    auto numBg1 = Sprite::create("ui_kuang03.png");
    _crossSprite->addChild(numBg1,10);
    numBg1->setPosition(Vec2(-35,-25));
    auto labelLevel1 = LabelAtlas::create("2", "shuliang02_number.png", 12, 16, '0');
    _crossSprite->addChild(labelLevel1,10);
    labelLevel1->setPosition(numBg1->getPosition());
    labelLevel1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _aroundSprite = Armature::create("lingxing_icon");
    this->addChild(_aroundSprite);
    _aroundSprite->setPosition(this->convertToNodeSpace(Vec2(270 , 39)));
    _aroundSprite->getAnimation()->play("Animation1");
    
    auto numBg2 = Sprite::create("ui_kuang03.png");
    _aroundSprite->addChild(numBg2,10);
    numBg2->setPosition(Vec2(-35,-25));
    auto labelLevel2 = LabelAtlas::create("3", "shuliang02_number.png", 12, 16, '0');
    _aroundSprite->addChild(labelLevel2,10);
    labelLevel2->setPosition(numBg2->getPosition());
    labelLevel2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _onekindSprite = Armature::create("mobang_icon");
    this->addChild(_onekindSprite);
    _onekindSprite->setPosition(this->convertToNodeSpace(Vec2(369 , 39)));
    _onekindSprite->getAnimation()->play("Animation1");
    
    auto numBg3 = Sprite::create("ui_kuang03.png");
    _onekindSprite->addChild(numBg3,10);
    numBg3->setPosition(Vec2(-35,-25));
    auto labelLevel3 = LabelAtlas::create("4", "shuliang02_number.png", 12, 16, '0');
    _onekindSprite->addChild(labelLevel3,10);
    labelLevel3->setPosition(numBg3->getPosition());
    labelLevel3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _changeSprite = Armature::create("nuqi_icon");
    this->addChild(_changeSprite);
    _changeSprite->setPosition(this->convertToNodeSpace(Vec2(468 , 39)));
    _changeSprite->getAnimation()->play("Animation1");
    
    auto numBg4 = Sprite::create("ui_kuang03.png");
    _changeSprite->addChild(numBg4,10);
    numBg4->setPosition(Vec2(-35,-25));
    auto labelLevel4 = LabelAtlas::create("5", "shuliang02_number.png", 12, 16, '0');
    _changeSprite->addChild(labelLevel4,10);
    labelLevel4->setPosition(numBg4->getPosition());
    labelLevel4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _add5stepSprite = Armature::create("jiawu_icon");
    this->addChild(_add5stepSprite);
    _add5stepSprite->setPosition(this->convertToNodeSpace(Vec2(570 , 39)));
    _add5stepSprite->getAnimation()->play("Animation1");
    
    auto numBg5 = Sprite::create("ui_kuang03.png");
    _add5stepSprite->addChild(numBg5,10);
    numBg5->setPosition(Vec2(-35,-25));
    auto labelLevel5 = LabelAtlas::create("6", "shuliang02_number.png", 12, 16, '0');
    _add5stepSprite->addChild(labelLevel5,10);
    labelLevel5->setPosition(numBg5->getPosition());
    labelLevel5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto skillBG = Sprite::create("xui_skill.png");
    this->addChild(skillBG,8);
    skillBG->setPosition(this->convertToNodeSpace(Vec2(320 , 720)));
    
    Progress *pro6 = Progress::create("xui_jindu01.png", "xui_jindu02.png",false);
    this->addChild(pro6, 11);
    pro6->setPosition(skillBG->getPosition() - Vec2(0, 50));
    pro6->setProgress(40);
}

bool DuplicateLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _mapInfo = DataCenter::getInstance()->getMapInfo();
    
    //
    int** tempMatrix = _mapInfo->getMatrixMap();
    
    _mapMatrix = (int**)malloc(sizeof(int*)*kMatrixWidth);
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        _mapMatrix[i] = (int*)malloc(sizeof(int)*kMatrixWidth);
        
        for(int j=0;j<kMatrixWidth;j++)
        {
            _mapMatrix[i][j]=tempMatrix[i][j];
        }
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    float screenRate = winSize.height/winSize.width;
    
    if(screenRate>1.7)
    {
        _edgeBlankDevidedSize = 29;
    }
    else if(screenRate<1.4)
    {
        _edgeBlankDevidedSize = 15;
    }
    else
    {
        _edgeBlankDevidedSize = 29;
    }
    
    CCLOG("======%f",screenRate);
    
    float edgeBlank = winSize.width/_edgeBlankDevidedSize;
    
    this->setContentSize(Size(winSize.width-edgeBlank*2,winSize.width-edgeBlank*2));//原来 Y 为 heigh
    
    this->setAnchorPoint(Point(0, 0));
    
    _scaleFactor = (winSize.width-edgeBlank*2)/(kElementSize*kMatrixWidth);
    
    this->setScale(_scaleFactor);
    
    _startPoint=calculateStartPoint(edgeBlank, winSize, _scaleFactor);
    
    _particleNode = Node::create();//粒子效果节点,z=1
    
    _mapNode = Node::create();//地图节点z=-1
    
    Point startPoint =Point(-winSize.width*kGameLayerStartMultiple,(winSize.height-(winSize.width-edgeBlank*2))/2);
    
    Point endPoint = _startPoint;//ccp(edgeBlank,(winSize.height-(winSize.width-edgeBlank*2))/2);
    
    this->addChild(_particleNode,kParticleZOrder);
    
    this->addChild(_mapNode,kMapZOrder);
    
    _gemMatrix = (Gem***)malloc(sizeof(Gem**)*kMatrixWidth);
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        _gemMatrix[i] = (Gem**)malloc(sizeof(Gem*)*kMatrixWidth);
        
        for(int j=0;j<kMatrixWidth;j++)
        {
            _gemMatrix[i][j]=NULL;
        }
    }
    
    int **fruitMap = _mapInfo->getGemMap();
    
    _mapLayer = new MapMatrix(_mapMatrix, _gemMatrix, kMatrixWidth,_mapInfo);
    
    _mapLayer->createMapFrame(_mapNode);
    
    _mapNode->setVisible(false);
    _particleNode->setVisible(false);
    
    //    auto spr = Sprite::create("map01_zhegai.png");
    //    spr->setAnchorPoint(Vec2(0, 0));
    //    _mapNode->addChild(spr);
    
    Gem::setMapInfo(_particleNode,_mapNode, _gemMatrix, _mapInfo,&_animationWraperVector,&_emptyPointVector,&(_mapLayer->connectionAreaVector),_mapMatrix);
    
    for(int i = 0;i<kMatrixWidth;i++)
    {
        for(int j = 0;j<kMatrixWidth;j++)
        {
            GemType type =(GemType)fruitMap[i][j];
            /*			CCLOG("%d",fruitMap[i][j]);*/
            _gemMatrix[i][j] = Gem::createFixGem(type,this,Point(kStartX + i * kElementSize + kElementSize/2, kStartY + j * kElementSize + kElementSize/2),true);
            if(_gemMatrix[i][j])
                _gemMatrix[i][j]->setVisible(false);
        }
    }
    if(/*_mapInfo->getClassKind()==ClassNone||(_mapInfo->getClassKind()>=ClassPropSingle&&_mapInfo->getClassKind()<=ClassPropCureSick)*/true)
    {
        this->firstTimeInit();
    }
    //放鸟窝，冰冻，藤条
    for (int i=0; i<kMatrixWidth; i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if (!_gemMatrix[i][j])
            {
                continue;
            }
            _gemMatrix[i][j]->addFrozen(_mapInfo->isFrozenInMap(i,j));
            
            _gemMatrix[i][j]->addRoots(_mapInfo->isRootsInMap(i,j));
            
            _gemMatrix[i][j]->addChain(_mapInfo->isChainInMap(i,j));
            
            if (_mapInfo->isIceFloorInMap(i, j) == 1)
            {
                auto sprIceFloor = Sprite::create("icefloor.png");
                sprIceFloor->setPosition(Point(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize));
                sprIceFloor->setTag(i*kMatrixWidth + j);
                _mapNode->addChild(sprIceFloor);
                
                sprIceFloor->setLocalZOrder(1);
            }
            if (_mapInfo->isIceFloorInMap(i, j) == 2)
            {
                auto sprIceFloor = Sprite::create("icefloor1.png");
                sprIceFloor->setPosition(Point(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize));
                sprIceFloor->setTag(i*kMatrixWidth + j);
                _mapNode->addChild(sprIceFloor);
                
                sprIceFloor->setLocalZOrder(2);
            }
            
            if (_mapInfo->isLeafInMap(i, j))
            {
                auto sprIceFloor = Sprite::create("leaf.png");
                sprIceFloor->setPosition(Point(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize));
                sprIceFloor->setTag(i*kMatrixWidth + j);
                _particleNode->addChild(sprIceFloor);
            }
            if (_mapInfo->isMapTop(i, j))
            {
                Armature *armature = Armature::create( "xialuojiantou");
                _particleNode->addChild(armature,20);
                armature->getAnimation()->playWithIndex(0);
                armature->setPosition(kStartX+(i+0.5)*kElementSize,kStartY+(j+1)*kElementSize + 10);
            }
            if (_mapInfo->isMapBottom(i, j))
            {
                Armature *armature = Armature::create( "xialuojiantou");
                _particleNode->addChild(armature,20);
                armature->getAnimation()->playWithIndex(0);
                armature->setPosition(kStartX+(i+0.5)*kElementSize,kStartY+(j)*kElementSize + 10);
            }
            
        }
    }
    
    //
    
    this->setPosition(endPoint);
    
    this->setTouchDisable();
    
    
    this->runAction(Sequence::create(DelayTime::create(kGameLayerFlyTime),/*CallFunc::create(CC_CALLBACK_0(DuplicateLayer::appear, this)),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::shining, this)),*/NULL));
    
    _listener_touch = EventListenerTouchOneByOne::create();
    _listener_touch->onTouchBegan = CC_CALLBACK_2(DuplicateLayer::onTouchBegan,this);
    _listener_touch->onTouchMoved = CC_CALLBACK_2(DuplicateLayer::onTouchMoved,this);
    _listener_touch->onTouchEnded = CC_CALLBACK_2(DuplicateLayer::onTouchEnded,this);
    _listener_touch->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener_touch, this);
    
    return true;
}

void DuplicateLayer::onEnter()
{
    Layer::onEnter();
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::winnerMode), kMSG_WinnerModeStart, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::outStage),kMSG_GameLayer_BeforeOutStage, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::classGuidMove),kMSG_ClassGuid_MoveGem, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::gemFallNotification),kMSG_GemFallDown, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::buyFiveSteps),kMSG_BuyFiveSteps, NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::afterExplode), "after_match", NULL);
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(DuplicateLayer::displayScore), "display_score", NULL);
}

void DuplicateLayer::onExit()
{
    NotificationCenter::getInstance()->removeObserver(this, kMSG_WinnerModeStart);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_PropUseStart);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_PropUseCancel);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_GameLayer_BeforeOutStage);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_ClassGuid_MoveGem);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_GemFallDown);
    NotificationCenter::getInstance()->removeObserver(this, kMSG_BuyFiveSteps);
    NotificationCenter::getInstance()->removeObserver(this, "after_match");
    NotificationCenter::getInstance()->removeObserver(this, "display_score");
    _eventDispatcher->removeEventListenersForTarget(this);
    
    Layer::onExit();
}

bool DuplicateLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    stopHighLight();
    
    _isMoveDone = true;
    
    _location = pTouch->getLocation();
    
    int a = _location.x-kStartX-_startPoint.x;
    
    int b = _location.y-kStartY-_startPoint.y;
    
    int i = a/(kElementSize*_scaleFactor);
    int j = b/(kElementSize*_scaleFactor);
    
    
    if (_propkind == PropChange)
    {
        if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove())
        {
            if (i == _selectPoint.x)
            {
                if (j == _selectPoint.y - 1)
                {
                    _nextPoint.setPosition(i, j);
                    changeGem(Down);
                }
                if (j == _selectPoint.y + 1)
                {
                    _nextPoint.setPosition(i, j);
                    changeGem(Up);
                }
            }
            if (j == _selectPoint.y)
            {
                if (i == _selectPoint.x - 1)
                {
                    _nextPoint.setPosition(i, j);
                    changeGem(Left);
                }
                if (i == _selectPoint.x + 1)
                {
                    _nextPoint.setPosition(i, j);
                    changeGem(Right);
                }
            }
        }
        return false;
    }
    
    if(/*(b<0||a<0) && */_propkind == PropNone)
    {
        __String *str = __String::create("");
        if (_hammerSprite->getBoundingBox().containsPoint(Vec2(a, b)))
        {
            str = __String::create("chuizi_icon");
            _propkind = PropSingle;
            _hammerSprite->setOpacity(100);
        }
        if (_aroundSprite->getBoundingBox().containsPoint(Vec2(a, b)))
        {
            str = __String::create("lingxing_icon");
            _propkind = PropAround;
            _aroundSprite->setOpacity(100);
        }
        if (/*_crossSprite->getBoundingBox()*/RectApplyAffineTransform(Rect(-40, -40, 62, 62),_crossSprite->getNodeToParentAffineTransform()).containsPoint(Vec2(a, b)))
        {
            
            str = __String::create("shizi_icon");
            _propkind = PropCross;
            _crossSprite->setOpacity(100);
        }
        if (_changeSprite->getBoundingBox().containsPoint(Vec2(a, b)))
        {
            str = __String::create("nuqi_icon");
            _propkind = PropChange;
            _changeSprite->setOpacity(100);
        }
        if (_onekindSprite->getBoundingBox().containsPoint(Vec2(a, b)))
        {
            str = __String::create("mobang_icon");
            _propkind = PropOneKind;
            _onekindSprite->setOpacity(100);
        }
        
        if (_add5stepSprite->getBoundingBox().containsPoint(Vec2(a, b)))
        {
            matrixBright();
            _add5stepSprite->getAnimation()->play("Animation2");
            _add5stepSprite->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(DuplicateLayer::resetAnimation, this) );
        }
        
        
        if (_propkind > PropNone)
        {
            _selectPoint.reset();
            //            stopHighLight();
            
            if (this->getChildByTag(PROP_TAG))
            {
                this->removeChildByTag(PROP_TAG);
            }
            auto propSpr = Armature::create(str->getCString());
            this->addChild(propSpr,120,PROP_TAG);
            propSpr->setPosition(Vec2(a , b) + Vec2(-20, 20));
            if (_propkind == PropOneKind)
            {
                propSpr->setAnchorPoint(Vec2(0.3, 0.7));
            }
            return true;
        }
        //        return false;
    }
    
    MyPoint mp = MyPoint(-1, -1);
    if(!_selectPoint.equal(mp))
    {
        mp = MyPoint(i,j);
        if (_selectPoint.equal(mp))
        {
            _isMoveDone = false;
            _isMoveDone = false;
            
            _gemMatrix[i][j]->selected();
            
            _selectPoint.setPosition(i, j);
            
            return true;
        }
        
        
        if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove()&&_gemMatrix[_selectPoint.x][_selectPoint.y])
        {
            _gemMatrix[_selectPoint.x][_selectPoint.y]->unselected();
            if ((i == _selectPoint.x && (j == _selectPoint.y - 1 || j == _selectPoint.y + 1)) || (j == _selectPoint.y && (i == _selectPoint.x - 1 || i == _selectPoint.x + 1)))
            {
                _hightLightSwitch = false;
                stopHighLight();
                
                setTouchDisable();
                _nextPoint.setPosition(i, j);
                if (i == _selectPoint.x && j == _selectPoint.y - 1)
                {
                    changeGem(Down);
                    
                }
                else if (i == _selectPoint.x && j == _selectPoint.y + 1)
                {
                    changeGem(Up);
                    
                }
                
                else if (j == _selectPoint.y && i == _selectPoint.x - 1)
                {
                    changeGem(Left);
                    
                }
                else if (j == _selectPoint.y && i == _selectPoint.x + 1)
                {
                    changeGem(Right);
                }
                return true;
            }
            else
            {
                _selectPoint.reset();
            }
        }
    }
    
    if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove())
    {
        _isMoveDone = false;
        
        _gemMatrix[i][j]->selected();
        
        _selectPoint.setPosition(i, j);
        
        return true;
    }
    else
    {
        _isMoveDone = true;
        
        return false;
    }
}

void DuplicateLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if (_propkind > PropNone)
    {
        stopHighLight();
        Point pos = pTouch->getLocation();
        auto sp = this->getChildByTag(PROP_TAG);
        sp->setPosition(Vec2(pos.x-kStartX-_startPoint.x, pos.y-kStartY-_startPoint.y) + Vec2(-20, 20));
        
        int a = sp->getPositionX();
        
        int b = sp->getPositionY();
        
        if (a < 0 || b < 0)
        {
            hidePropRange();
        }
        
        else
        {
            int i = a/(kElementSize*_scaleFactor);
            int j = b/(kElementSize*_scaleFactor);
            if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j])
            {
                showPropRange(i, j);
            }
            else
            {
                hidePropRange();
            }
        }
        
        return;
    }
    MyPoint mp = MyPoint(-1, -1);
    
    if (!_selectPoint.equal(mp)&&_gemMatrix[_selectPoint.x][_selectPoint.y])
    {
        _gemMatrix[_selectPoint.x][_selectPoint.y]->unselected();
    }
    else
    {
        return;
    }
    
    int a = _location.x-kStartX-_startPoint.x;
    
    int b = _location.y-kStartY-_startPoint.y;
    
    if(b<0||a<0) return;
    
    int i = a/(kElementSize*_scaleFactor);
    int j = b/(kElementSize*_scaleFactor);
    
    Point moveDirection = pTouch->getLocation();
    
    float distance = _location.getDistance(moveDirection);//ccpDistance(_location, moveDirection);
    
    if(distance<kElementSize*_scaleFactor*0.2)
    {
        return;
    }
    if (!_isMoveDone)
    {
        
        if (fabs(moveDirection.x - _location.x) > fabs(moveDirection.y - _location.y))
        {
            if (moveDirection.x - _location.x > 0)
            {
                i += 1;
                _moveDirection = Right;
            }
            else
            {
                i -= 1;
                _moveDirection = Left;
            }
        }
        else
        {
            if (moveDirection.y - _location.y > 0)
            {
                j += 1;
                _moveDirection = Up;
            }
            else
            {
                j -= 1;
                _moveDirection = Down;
            }
        }
        
        if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth)
        {
            _isMoveDone = true;
            
            _nextPoint.setPosition(i, j);
        }
        else
        {
            return;
        }
        
        Gem *sGem = _gemMatrix[_selectPoint.x][_selectPoint.y];
        
        Gem *nGem = _gemMatrix[_nextPoint.x][_nextPoint.y];
        
        if (sGem && nGem && sGem->canMove() && nGem->canMove())
        {
            _hightLightSwitch = false;
            
            stopHighLight();
            
            setTouchDisable();
            
            changeGem(_moveDirection);
        }
        else if (sGem && nGem && sGem->canMove() && !nGem->canMove())
        {
            
            stopHighLight();
            
            setTouchDisable();
            
            MoveBy *action;
            
            switch (_moveDirection)
            {
                case Up:
                    action = MoveBy::create(0.2, Vec2(0,kElementSize/3.5));
                    break;
                case Down:
                    action = MoveBy::create(0.2, Vec2(0,-kElementSize/3.5));
                    break;
                case Left:
                    action = MoveBy::create(0.2, Vec2(-kElementSize/3.5,0));
                    break;
                case Right:
                    action = MoveBy::create(0.2, Vec2(kElementSize/3.5,0));
                    break;
                default:
                    break;
            }
            sGem->runAction(Sequence::create(action,action->reverse(),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::setTouchEnable, this)), NULL));
        }
    }
}

void DuplicateLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (_propkind > PropNone)
    {
        hidePropRange();
        auto prop = (Armature*)this->getChildByTag(PROP_TAG);
        Point pos = prop->getPosition();
        
        int a = pos.x;
        
        int b = pos.y;
        
        if(b>=0 && a>=0)
        {
            int i = a/(kElementSize*_scaleFactor);
            int j = b/(kElementSize*_scaleFactor);
            CCLOG(" i = %d  j = %d",i ,j);
            
            if(i>=0&&i<kMatrixWidth&&j>=0&&j<kMatrixWidth&&_gemMatrix[i][j])
            {
                if (_propkind == PropChange )
                {
                    if (_gemMatrix[i][j]->canMove())
                    {
                        _changeSprite->setOpacity(255);
                        _selectPoint.setPosition(i, j);
                    }
                }
                else
                {
                    setTouchDisable();
                    MyPoint point = MyPoint(i, j);
                    
                    if(!propUse(point))
                    {
                        setTouchEnable();
                    }
                    else
                    {
                        GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
                        _propPosition = point;
                        
                        propAnimation(point);
                        return;
                    }
                }
            }
        }
        MyPoint mp = MyPoint(-1, -1);
        if(_propkind != PropChange || _selectPoint.equal(mp))
        {
            _propkind = PropNone;
        }
        this->removeChildByTag(PROP_TAG,true);
        
        _hammerSprite->setOpacity(255);
        _aroundSprite->setOpacity(255);
        _crossSprite->setOpacity(255);
        _onekindSprite->setOpacity(255);
        _changeSprite->setOpacity(255);
    }
    
}

void DuplicateLayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
}

void DuplicateLayer::changeGem(Direction direction)
{
    if(_propkind == PropChange)
    {
        _matchDownMSGSwitch = true;
        
        Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];
        
        _gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];
        
        _gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;
        
        
        CallFunc* callbackAction = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::beforeMatch, this));
        
        _gemMatrix[_nextPoint.x][_nextPoint.y]->swap(direction, false, false,callbackAction);
        
        _gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-direction),false, true, NULL);
        
        _isStartMove = true;
        
        return;
    }
    PotentialArea potentialArea;
    
    bool canChange = false,grassguyMatch=false;
    
    _continueMatchTimes = 0;
    
    if ((_gemMatrix[_nextPoint.x][_nextPoint.y]->getGemType() <= all && _gemMatrix[_selectPoint.x][_selectPoint.y]->getGemType() <= all) && (_gemMatrix[_selectPoint.x][_selectPoint.y]->hasSkill(_gemMatrix[_nextPoint.x][_nextPoint.y]) || _gemMatrix[_nextPoint.x][_nextPoint.y]->hasSkill(_gemMatrix[_selectPoint.x][_selectPoint.y])))
    {
        _matchDownMSGSwitch = true;
        
        Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];
        
        _gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];
        
        _gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;
        
        
        CallFunc* callbackAction = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::skillGemChange, this));
        
        _gemMatrix[_nextPoint.x][_nextPoint.y]->swap(direction, false, false,callbackAction);
        
        _gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-direction),false, true, NULL);
        
        _isStartMove = true;
        
        NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
        
        return;
    }
    
    
    for(int i=0;i<_mapLayer->potentialAreaVector.size();i++)
    {
        potentialArea = _mapLayer->potentialAreaVector[i];
        
        bool selectRight =potentialArea.movePoint.x == _selectPoint.x&&potentialArea.movePoint.y == _selectPoint.y&&direction==potentialArea.diretion;
        
        
        bool nextRight = potentialArea.movePoint.x == _nextPoint.x&&potentialArea.movePoint.y == _nextPoint.y&&Direction(Down-direction) == potentialArea.diretion;
        
        if(selectRight||nextRight)
        {
            canChange = true;
            break;
        }
    }
    
    if(grassguyMatch)
    {
        _matchDownMSGSwitch = true;
        
        Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];
        
        _gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];
        
        _gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;
        
        
        _gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-direction), false,true, NULL);
        
        _isStartMove = true;
        
        NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
    }
    else if(canChange)
    {
        _matchDownMSGSwitch = true;
        
        Gem *tempGem = _gemMatrix[_selectPoint.x][_selectPoint.y];
        
        _gemMatrix[_selectPoint.x][_selectPoint.y] = _gemMatrix[_nextPoint.x][_nextPoint.y];
        
        _gemMatrix[_nextPoint.x][_nextPoint.y] = tempGem;
        
        CallFunc* callbackAction = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::beforeMatch, this));
        
        _gemMatrix[_nextPoint.x][_nextPoint.y]->swap(direction, false, true,callbackAction);
        
        _gemMatrix[_selectPoint.x][_selectPoint.y]->swap(Direction(Down-direction),false, false, NULL);
        
        _isStartMove = true;
        
        NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
    }
    else
    {
        CallFunc* callbackAction = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::findPotentialOrRenew, this));
        
        _gemMatrix[_selectPoint.x][_selectPoint.y]->swap(direction,true,true,callbackAction);
        
        _gemMatrix[_nextPoint.x][_nextPoint.y]->swap(Direction(Down-direction), true,false,NULL);
        
        _hightLightSwitch = true;
        
        _selectPoint.reset();
    }
}

void DuplicateLayer::hidePropRange()
{
    for (int i = 0; i < kMatrixWidth; i++)
    {
        for (int j = 0; j < kMatrixWidth; j ++)
        {
            if (_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->hideRange();
            }
            
        }
    }
}

void DuplicateLayer::showPropRange(int i, int j)
{
    hidePropRange();
    
    _gemMatrix[i][j]->showRange();
    if(_propkind == PropAround)
    {
        if(i - 1 >= 0)
        {
            if (j + 1 < kMatrixWidth && _gemMatrix[i-1][j+1])
            {
                _gemMatrix[i-1][j+1]->showRange();
            }
            if (_gemMatrix[i-1][j])
            {
                _gemMatrix[i-1][j]->showRange();
            }
            if (j - 1 >= 0 && _gemMatrix[i-1][j-1])
            {
                _gemMatrix[i-1][j-1]->showRange();
            }
        }
        if (i + 1 < kMatrixWidth)
        {
            if (j + 1 < kMatrixWidth && _gemMatrix[i+1][j+1])
            {
                _gemMatrix[i+1][j+1]->showRange();
            }
            if (_gemMatrix[i+1][j])
            {
                _gemMatrix[i+1][j]->showRange();
            }
            if (j - 1 >= 0 && _gemMatrix[i+1][j-1])
            {
                _gemMatrix[i+1][j-1]->showRange();
            }
        }
        if (j - 1 >=0 && _gemMatrix[i][j-1])
        {
            _gemMatrix[i][j-1]->showRange();
        }
        if (j + 1 < kMatrixWidth && _gemMatrix[i][j+1])
        {
            _gemMatrix[i][j+1]->showRange();
        }
        if (i-2 >= 0 && _gemMatrix[i-2][j])
        {
            _gemMatrix[i-2][j]->showRange();
            
        }
        if (i+2 < kMatrixWidth && _gemMatrix[i+2][j])
        {
            _gemMatrix[i+2][j]->showRange();
            
        }
        if (j-2 >= 0 && _gemMatrix[i][j-2])
        {
            _gemMatrix[i][j-2]->showRange();
        }
        if (j+2 < kMatrixWidth && _gemMatrix[i][j+2])
        {
            _gemMatrix[i][j+2]->showRange();
        }
        
        
    }
    if(_propkind == PropSingle)
    {
        _gemMatrix[i][j]->showRange();
    }
    if(_propkind == PropCross)
    {
        for (int a = 0; a < kMatrixWidth; a++)
        {
            if (a == i)
            {
                continue;
            }
            if (_gemMatrix[a][j])
            {
                _gemMatrix[a][j]->showRange();
            }
        }
        
        for (int b = 0; b < kMatrixWidth; b++)
        {
            if (b == j)
            {
                continue;
            }
            if (_gemMatrix[i][b])
            {
                _gemMatrix[i][b]->showRange();
            }
        }
        
    }
}

void DuplicateLayer::appear()
{
    _mapNode->setVisible(true);
    _particleNode->setVisible(true);
    for (int i=0; i<kMatrixWidth; i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if (_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->setVisible(true);
                _gemMatrix[i][j]->setScale(0);
                _gemMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0.3) , ScaleTo::create(0.3, 1), NULL));
            }
        }
    }
    this->runAction(Sequence::create(DelayTime::create(0.4) , CallFunc::create(CC_CALLBACK_0(DuplicateLayer::shining, this)), NULL));
}

void DuplicateLayer::highLightGem(float dt)
{
    _mapLayer->findPotential();
    
    if (_propkind > PropNone)
    {
        return;
    }
    
    MyPoint mp = MyPoint(-1, -1);
    if(!_selectPoint.equal(mp))
    {
        _gemMatrix[_selectPoint.x][_selectPoint.y]->unselected();
        _selectPoint.reset();  //待定
    }
    
    if(!_mapLayer->potentialAreaVector.empty()&&_hightLightSwitch)
    {
        highLightMovePoint = _mapLayer->potentialAreaVector[0].movePoint;
        highLightPoint1 = _mapLayer->potentialAreaVector[0].point1;
        highLightPoint2 = _mapLayer->potentialAreaVector[0].point2;
        
        _gemMatrix[highLightMovePoint.x][highLightMovePoint.y]->highLight();
        _gemMatrix[highLightPoint1.x][highLightPoint1.y]->highLight();
        if (highLightPoint2.x >= 0 && highLightPoint2.y >= 0)
        {
            _gemMatrix[highLightPoint2.x][highLightPoint2.y]->highLight();
        }
        
    }
}
void DuplicateLayer::stopHighLight()
{
    if(!_mapLayer->potentialAreaVector.empty())
    {
        highLightMovePoint = _mapLayer->potentialAreaVector[0].movePoint;
        highLightPoint1 = _mapLayer->potentialAreaVector[0].point1;
        highLightPoint2 = _mapLayer->potentialAreaVector[0].point2;
        
        _gemMatrix[highLightMovePoint.x][highLightMovePoint.y]->stopHighLight();
        _gemMatrix[highLightPoint1.x][highLightPoint1.y]->stopHighLight();
        if (highLightPoint2.x >= 0 && highLightPoint2.y >= 0)
        {
            _gemMatrix[highLightPoint2.x][highLightPoint2.y]->stopHighLight();
        }
    }
}

void DuplicateLayer::renewAnimation(cocos2d::Node *pSender)
{
    Sprite *spr = Sprite::create("effect_suilie01.png");
    auto animation = Animation::create();
    
    for (int i = 1; i < 13; i++)
    {
        __String *file = __String::createWithFormat("effect_suilie0%d.png",i);
        
        auto sprite = Sprite::create(file->getCString());
        animation->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    animation->setDelayPerUnit(0.05);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    this->addChild(spr);
    spr->setPosition(pSender->getPosition());
    spr->runAction(Sequence::create(action,CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this)), NULL));
}

void DuplicateLayer::renewAllGem()
{
    NotificationCenter::getInstance()->postNotification(kMSG_RenewAll);
    
    MyPoint pointArray[kMatrixWidth*kMatrixWidth];
    
    Gem* gemArray[kMatrixWidth*kMatrixWidth];
    
    int i,j,max_size=0;
    
    for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
    {
        pointArray[i].setPosition(0, 0);
        gemArray[i]=NULL;
    }
    
    for(j=0;j<kMatrixWidth;j++)
    {
        for(i=0;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove() && _gemMatrix[i][j]->getGemType()<all)
            {
                gemArray[max_size] = _gemMatrix[i][j];
                pointArray[max_size] = MyPoint(i,j);
                max_size++;
            }
        }
    }
    while(_mapLayer->potentialAreaVector.empty())
    {
        do{
            for(i=max_size;i>0;i--)
            {
                int index = arc4random()%i;
                
                MyPoint p = pointArray[index];
                
                _gemMatrix[p.x][p.y] = gemArray[i-1];
                
                pointArray[index].swap(pointArray[i-1]);
            }
            
            _mapLayer->findConnection();
            
        }while(!_mapLayer->connectionAreaVector.empty());
        
        _mapLayer->findPotential();
    }
    
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    backLayerColor->setContentSize(Size(1000, 2000));
    backLayerColor->setPosition(Vec2(-100, -200));
    this->addChild(backLayerColor,99);
    
    auto callback = CallFuncN::create(
                                      [&](Node *pSender)
                                      {
                                          pSender->removeFromParentAndCleanup(true);
                                      } );
    backLayerColor->runAction(Sequence::create(DelayTime::create(1.5) , callback->clone() ,NULL));
    
    Armature *armature = Armature::create( "buju_effect");
    this->getParent()->addChild(armature,100);
    armature->getAnimation()->playWithIndex(0);
    armature->setPosition(Vec2(Director::getInstance()->getWinSize().width/2, 500));
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this) );
    
    FiniteTimeAction* action = NULL;
    
    for(i =0;i<max_size;i++)
    {
        if(i==0)
        {
            action = Sequence::create(DelayTime::create(kRenewGemTime + 0.2),ScaleTo::create(kRenewGemTime, 0),GemAction::getInstance().getGemRenewAction(pointArray[i]),ScaleTo::create(kRenewGemTime, 1),NULL);
        }
        else
        {
            action = Sequence::create(DelayTime::create(kRenewGemTime + 0.2),ScaleTo::create(kRenewGemTime, 0),GemAction::getInstance().getGemRenewAction(pointArray[i]),ScaleTo::create(kRenewGemTime, 1),NULL);
        }
        
        if(max_size-1==i)
        {
            FiniteTimeAction* actionWithCallback = Sequence::create(CallFunc::create(CC_CALLBACK_0(DuplicateLayer::addShieldLayer, this)),action,DelayTime::create(0.5),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::removeShieldLayer, this)),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::beforeMatch, this)),NULL);
            
            gemArray[i]->runAction(actionWithCallback);
            
            return;
        }
        gemArray[i]->runAction(action);
    }
}
void DuplicateLayer::findPotentialOrRenew()
{
    _mapLayer->findPotential();
    
    if(_mapLayer->potentialAreaVector.empty())
    {
        renewAllGem();
        return ;
    }
    else
    {
        this->setTouchEnable();
        return ;
    }
}


void DuplicateLayer::setTouchEnable()
{
    this->removeChildByTag(9999);
    
    this->unschedule(schedule_selector(DuplicateLayer::highLightGem));
    
    this->unschedule(schedule_selector(DuplicateLayer::runIdleAnimation));
    
    this->schedule(schedule_selector(DuplicateLayer::highLightGem),4);
    
    this->schedule(schedule_selector(DuplicateLayer::runIdleAnimation), 4);
    
}
void DuplicateLayer::setTouchDisable()
{
    if (!this->getChildByTag(9999))
    {
        SwallowLayer *noTouchLayer = SwallowLayer::create(); // 屏蔽层
        
        this->addChild(noTouchLayer,999,9999);
    }
    
    this->unschedule(schedule_selector(DuplicateLayer::highLightGem));
    
    this->unschedule(schedule_selector(DuplicateLayer::runIdleAnimation));
}

void DuplicateLayer::stopRemindAndIdle()
{
    this->unschedule(schedule_selector(DuplicateLayer::highLightGem));
    
    this->unschedule(schedule_selector(DuplicateLayer::runIdleAnimation));
}
void DuplicateLayer::runIdleAnimation(float dt)
{
    int i,j,k=0;
    
    while(k<25)
    {
        do
        {
            i = arc4random()%kMatrixWidth;
            
            j = arc4random()%kMatrixWidth;
            
        }while(!_gemMatrix[i][j]);
        
        if(_gemMatrix[i][j])
        {
            _gemMatrix[i][j]->idle();
        }
        
        k++;
    }
}

void DuplicateLayer::flyToSame(MyPoint allPos , MyPoint samePos ,GemSkill skill)
{
    _continueMatchTimes = 1;
    Point startPoint = _gemMatrix[allPos.x][allPos.y]->getPosition();
    GemType type = _gemMatrix[samePos.x][samePos.y]->getGemType();
    
    if (_gemMatrix[allPos.x][allPos.y] && _gemMatrix[allPos.x][allPos.y]->getGemType() == all)
    {
        _gemMatrix[allPos.x][allPos.y]->setState(all);
        _gemMatrix[allPos.x][allPos.y]->removeNoCollect(false);
        _gemMatrix[allPos.x][allPos.y] = NULL;
    }
    
    int i,j,max=0;
    
    Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};
    MyPoint distArray[kMatrixWidth*kMatrixWidth];
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for (j=0; j<kMatrixWidth; j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==type)
            {
                Sprite* spr = Sprite::create("sparkle1.png");
                spr->setPosition(startPoint);
                _particleNode->addChild(spr);
                spriteArray[max] = spr;
                distArray[max++].setPosition(i, j);
                
                ParticleSystemQuad *partic1 = ParticleSystemQuad::create("tongsexiao.plist");
                spr->addChild(partic1,10);
                partic1->setPosition(Vec2(spr->getBoundingBox().size.width/2,spr->getBoundingBox().size.height/2));
                
                if (_gemMatrix[i][j]->canMove())
                {
                    switch (skill)
                    {
                        case SkillHorizontal:
                        case SkillVerticl:
                        {
                            int random = arc4random();
                            if (random%2 == 0)
                            {
                                _gemMatrix[i][j]->setNextSkill(SkillHorizontal);
                            }
                            else
                            {
                                _gemMatrix[i][j]->setNextSkill(SkillVerticl);
                            }
                            _gemMatrix[i][j]->setSameAndSkill(true);
                        }
                            break;
                        case SkillAround2:
                        {
                            _gemMatrix[i][j]->setNextSkill(SkillAround2);
                            _gemMatrix[i][j]->setSameAndSkill(true);
                        }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    _gemMatrix[i][j]->explode(distArray[max - 1], distArray[max - 1], 0, 0);
                }
            }
        }
    }
    
    
    for (i=0; i<max; i++)
    {
        Gem* distGem =_gemMatrix[distArray[i].x][distArray[i].y];
        Sequence* seq=NULL;
        ccBezierConfig config;
        Point controlPoint = GemAction::getInstance().getControlPoint(startPoint, distGem->getPosition(),(Director::getInstance()->getWinSize().width)*kParticleBezierScale);
        config.controlPoint_1 = controlPoint;
        config.controlPoint_2 = controlPoint;
        config.endPosition = distGem->getPosition();
        
        if(i==max-1)
        {
            ActionInstant * callback;
            if (skill)
            {
                callback = CallFuncN::create(CC_CALLBACK_1(Gem::sameSkillOver, distGem,type));
            }
            else
            {
                callback = CallFuncN::create(CC_CALLBACK_1(Gem::removeSameGem, distGem,type));
            }
            seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),callback,NULL);
        }
        else
        {
            seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),NULL);
        }
        
        spriteArray[i]->runAction(seq);
    }
}

void DuplicateLayer::explodeSameAnimation(AnimationWraper aw , CallFuncN* callback)
{
    Point startPoint = aw.dist1;
    GemType type = aw.node->getGemType();
    
    int a = (startPoint.x-kStartX)/kElementSize;
    int b = (startPoint.y-kStartY)/kElementSize;
    
    if (_gemMatrix[a][b] && _gemMatrix[a][b]->getGemType() == all)
    {
        _gemMatrix[a][b]->removeNoCollect(false);
        _gemMatrix[a][b] = NULL;
    }
    
    
    int flagMatrix[kMatrixWidth][kMatrixWidth] = {0};
    
    int i,j,max=0;
    
    Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};
    MyPoint distArray[kMatrixWidth*kMatrixWidth];
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for (j=0;j<kMatrixWidth; j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==type)
            {
                flagMatrix[i][j]=1;
            }
        }
    }
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for (j=0; j<kMatrixWidth; j++)
        {
            if(flagMatrix[i][j]==1)
            {
                Sprite* spr = Sprite::create("sparkle1.png");
                spr->setPosition(startPoint);
                _particleNode->addChild(spr);
                spriteArray[max] = spr;
                distArray[max++].setPosition(i, j);
                
                ParticleSystemQuad *partic1 = ParticleSystemQuad::create("tongsexiao.plist");
                spr->addChild(partic1,10);
                partic1->setPosition(Vec2(spr->getBoundingBox().size.width/2,spr->getBoundingBox().size.height/2));
            }
        }
    }
    
    for (i=0; i<max; i++)
    {
        Gem* distGem =_gemMatrix[distArray[i].x][distArray[i].y];
        
        Sequence* seq=NULL;
        ccBezierConfig config;
        Point controlPoint = GemAction::getInstance().getControlPoint(startPoint, distGem->getPosition(),(Director::getInstance()->getWinSize().width)*kParticleBezierScale);
        config.controlPoint_1 = controlPoint;
        config.controlPoint_2 = controlPoint;
        config.endPosition = distGem->getPosition();
        
        if(i==max-1 && callback)
        {
            if (aw.node->getGemSkill())
            {
                seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),callback,CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),NULL);
                
            }
            else
            {
                seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),NULL);
                
                this->runAction(Sequence::create(DelayTime::create(1 + kSparkleFlyTime),callback, NULL));
                
            }
            
        }
        else
        {
            seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),NULL);
        }
        
        spriteArray[i]->runAction(seq);
    }
    
}

void DuplicateLayer::fiveMatchEffect(int index,MyPoint& point,CallFuncN* callback)
{
    //1，播放聚集能量粒子效果,同时本排元素高亮放大
    //2，找到屏幕中可以被影响的元素，生成光点飞向他们。
    //3，击中元素播放高亮放大效果
    //4，聚合类不播放该粒子效果
    
    vector<ConnectionArea>::iterator it;
    
    ConnectionArea ca = _mapLayer->connectionAreaVector[index];
    MyPoint particlePoint = ca.particlePoint;
    Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();
    
    GemType type = _gemMatrix[particlePoint.x][particlePoint.y]->getGemType();
    
    int flagMatrix[kMatrixWidth][kMatrixWidth] = {0};
    
    int i,j,max=0;
    
    //    Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};
    //
    //    MyPoint distArray[kMatrixWidth*kMatrixWidth];
    //地图中与5连相同种类元素，并且是可以移动的元素标记（非冰冻，束缚）
    for(i=0;i<kMatrixWidth;i++)
    {
        for (j=0;j<kMatrixWidth; j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()==type&&_gemMatrix[i][j]->canMove())
            {
                flagMatrix[i][j]=1;
            }
        }
    }
    
    for(it=_mapLayer->connectionAreaVector.begin();it!=_mapLayer->connectionAreaVector.end();it++)
    {
        if (it->direction == Up)
        {
            for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
            {
                if(_gemMatrix[i][j]->getGemType()==type)
                {
                    flagMatrix[i][j]=0;
                }
            }
        }
        else
        {
            for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
            {
                if(_gemMatrix[i][j]->getGemType()==type)
                {
                    flagMatrix[i][j]=0;
                }
            }
        }
    }
    //高亮闪
    if (ca.direction == Up)
    {
        for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
        {
            _gemMatrix[i][j]->bright();
            
        }
    }
    else
    {
        for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
        {
            _gemMatrix[i][j]->bright();
        }
    }
    
    if(max==0)
    {
        this->runAction(callback);
    }
}

void DuplicateLayer::fourMatchEffect(int index,CallFuncN *callback)
{
    //1,播放粒子效果
    //2,飞出光点
    vector<ConnectionArea>::iterator it;
    
    ConnectionArea ca = _mapLayer->connectionAreaVector[index];
    
    MyPoint particlePoint = ca.particlePoint;
    
    Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();
    
    Gem* fruitArray[kMatrixWidth]={0};
    
    int i,j,max=0;
    
    //高亮闪
    if (ca.direction == Up)
    {
        for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=ca.startPoint.x,j=0;j<ca.startPoint.y;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[max++] = _gemMatrix[i][j];
            }
        }
        for(i=ca.startPoint.x,j=ca.startPoint.y+ca.count;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[max++] = _gemMatrix[i][j];
            }
        }
    }
    else
    {
        for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=0,j=ca.startPoint.y;i<ca.startPoint.x;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[max++] = _gemMatrix[i][j];
            }
        }
        for(i=ca.startPoint.x+ca.count,j=ca.startPoint.y;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[max++] = _gemMatrix[i][j];
            }
        }
    }
    if(max==0)
    {
        this->runAction(Sequence::create(DelayTime::create(kSparkleFlyTime),callback,NULL));
    }
    this->runAction(Sequence::create(/*DelayTime::create(kSparkleFlyTime),*/callback,NULL));
}
void DuplicateLayer::crossMatchEffect(int index1,int index2, CallFuncN* callback)
{
    vector<ConnectionArea>::iterator it;
    
    ConnectionArea ca1 = _mapLayer->connectionAreaVector[index1];
    
    ConnectionArea ca2 = _mapLayer->connectionAreaVector[index2];
    
    MyPoint particlePoint = ca1.centerPoint;
    
    Point startPoint = _gemMatrix[particlePoint.x][particlePoint.y]->getPosition();
    
    int fruitArray[kMatrixWidth][kMatrixWidth]={0};
    
    int i,j;
    
    int max = 0;
    
    if (ca1.direction == Up)
    {
        for (j = ca1.startPoint.y,i = ca1.startPoint.x; j < ca1.startPoint.y + ca1.count; j++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=ca1.startPoint.x,j=0;j<ca1.startPoint.y;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j] = 1;
                max++;
            }
        }
        for(i=ca1.startPoint.x,j=ca1.startPoint.y+ca1.count;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j]= 1;
                max++;
            }
        }
    }
    else
    {
        for (i = ca1.startPoint.x,j = ca1.startPoint.y; i < ca1.startPoint.x + ca1.count; i++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=0,j=ca1.startPoint.y;i<ca1.startPoint.x;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j] = 1;
                max++;
            }
        }
        for(i=ca1.startPoint.x+ca1.count,j=ca1.startPoint.y;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j] = 1;
                max++;
            }
        }
    }
    
    if (ca2.direction == Up)
    {
        for (j = ca2.startPoint.y,i = ca2.startPoint.x; j < ca2.startPoint.y + ca2.count; j++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=ca2.startPoint.x,j=0;j<ca2.startPoint.y;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j]=1;
                max++;
            }
        }
        for(i=ca2.startPoint.x,j=ca2.startPoint.y+ca2.count;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j]=1;
                max++;
            }
        }
    }
    else
    {
        for (i = ca2.startPoint.x,j = ca2.startPoint.y; i < ca2.startPoint.x + ca2.count; i++)
        {
            _gemMatrix[i][j]->bright();
        }
        for(i=0,j=ca2.startPoint.y;i<ca2.startPoint.x;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j]=1;
                max++;
            }
        }
        for(i=ca2.startPoint.x+ca2.count,j=ca2.startPoint.y;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemKind()==NormalGem&&_gemMatrix[i][j]->canMove())
            {
                fruitArray[i][j]=1;
                max++;
            }
        }
    }
    if(max==0)
    {
        this->runAction(Sequence::create(DelayTime::create(kSparkleFlyTime),callback,NULL));
    }
    this->runAction(Sequence::create(/*DelayTime::create(kSparkleFlyTime),*/callback,NULL));
}

void DuplicateLayer::firstTimeInit()
{
    MyPoint pointArray[kMatrixWidth*kMatrixWidth];
    
    Gem* fruitArray[kMatrixWidth*kMatrixWidth];
    
    int i,j,max_size=0;
    
    for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
    {
        pointArray[i].setPosition(0, 0);
        
        fruitArray[i]=NULL;
    }
    
    for(j=0;j<kMatrixWidth;j++)
    {
        for(i=0;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j]&&(_gemMatrix[i][j]->getGemKind()==NormalGem/*||_gemMatrix[i][j]->getGemType()==butterfly||_gemMatrix[i][j]->getGemType()==chicken*/))
            {
                fruitArray[max_size] = _gemMatrix[i][j];
                
                pointArray[max_size] = MyPoint(i,j);
                
                max_size++;
            }
        }
    }
    
    _mapLayer->findConnection();
    
    while(!_mapLayer->connectionAreaVector.empty())
    {
        do{
            for(i=max_size;i>0;i--)
            {
                int index = arc4random()%i;
                
                MyPoint p = pointArray[index];
                
                _gemMatrix[p.x][p.y] = fruitArray[i-1];
                
                pointArray[index].swap(pointArray[i-1]);
            }
            
            _mapLayer->findPotential();
            
        }while(_mapLayer->potentialAreaVector.empty());
        
        _mapLayer->findConnection();
    }
    
    for(i =0;i<max_size;i++)
    {
        FiniteTimeAction* action = GemAction::getInstance().getGemMoveToAction(pointArray[i]);
        
        fruitArray[i]->runAction(action);
    }
}

void DuplicateLayer::magicAnimation(Node* pSender)
{
    if(_gemMatrix[_propPosition.x][_propPosition.y]&&_gemMatrix[_propPosition.x][_propPosition.y]->getGemSkill()==SkillNull&&_gemMatrix[_propPosition.x][_propPosition.y]->canMove()&&_gemMatrix[_propPosition.x][_propPosition.y]->getGemKind()==NormalGem)
    {
        _skillType = SkillNull;
        int random = arc4random()%100;
        
        if (random < 15)//15
        {
            _skillType = SkillAllSame;
        }
        else if(random < 55)//55
        {
            _skillType = SkillAround2;
        }
        else
        {
            int index = arc4random()%2;
            if (index)
            {
                _skillType = SkillVerticl;
            }
            else
            {
                _skillType = SkillHorizontal;
            }
        }
        
        pSender->removeFromParentAndCleanup(true);
        _gemMatrix[_propPosition.x][_propPosition.y]->setNextSkill(_skillType);
        _gemMatrix[_propPosition.x][_propPosition.y]->setSkillSpr();
        
        _gemMatrix[_propPosition.x][_propPosition.y]->setGemSkill(_skillType);
        _gemMatrix[_propPosition.x][_propPosition.y]->setGemType();
        
        _mapLayer->findPotential();
    }
    
    else
    {
        if (_gemMatrix[_propPosition.x][_propPosition.y])
        {
            _gemMatrix[_propPosition.x][_propPosition.y]->addHudun();
        }
    }
    _onekindSprite->setOpacity(255);
    _skillType = SkillNull;
    _propkind = PropNone;
    setTouchEnable();
}

void DuplicateLayer::skillAnimation(CallFuncN *callback)
{
    this->runAction(Sequence::create(DelayTime::create(2.3),callback,NULL));
}

void DuplicateLayer::propAnimation(CallFuncN* callback)  // 技能 延时
{
    auto prop = (Armature*)this->getChildByTag(PROP_TAG);
    prop->getAnimation()->play("Animation2");
    
    if (_propkind == PropOneKind)
    {
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::magicAnimation, this) );
        return;
    }
    
    if (_propkind == PropAround)
    {
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::aroundAnimation, this) );
    }
    else
    {
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this) );
        if (_propkind == PropCross)
        {
            prop->runAction(Sequence::create(DelayTime::create(2),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::crossAnimation, this)), NULL));
        }
    }
    float times = 0;
    
    switch (_propkind)
    {
        case PropSingle:
            times = 0.5;
            break;
        case PropCross:
            times = 0.8;
            break;
        case PropChange:
            times = 0.5;
            break;
        case PropAround:
            times = 1;
            break;
        default:
            break;
    }
    this->runAction(Sequence::create(DelayTime::create(times),callback,NULL));
}


void DuplicateLayer::propAnimation(MyPoint mp)
{
    auto prop = (Armature*)this->getChildByTag(PROP_TAG);
    prop->getAnimation()->play("Animation2");
    
    if (_propkind == PropOneKind)
    {
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::magicAnimation, this) );
    }
    else if (_propkind == PropAround)
    {
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::aroundAnimation, this) );
    }
    else if (_propkind == PropCross)
    {
        prop->runAction(Sequence::create(DelayTime::create(0.8),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::crossAnimation, this)), NULL));
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this) );
    }
    else
    {
        prop->runAction(Sequence::create(DelayTime::create(0.4),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::hammerAnimation, this)), NULL));
        prop->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this) );
    }
    
    
}

void DuplicateLayer::hammerAnimation()
{
    if(_gemMatrix[_propPosition.x][_propPosition.y]->getGemKind()==NormalGem)
    {
        _gemMatrix[_propPosition.x][_propPosition.y]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[_propPosition.x][_propPosition.y]->beforeExplode();
        //        GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
        
        this->runAction(Sequence::create(MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)), MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)),MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)),NULL));
    }
    else if(_gemMatrix[_propPosition.x][_propPosition.y]->getGemType()>=ice && _gemMatrix[_propPosition.x][_propPosition.y]->getGemType()<=stormwind)
    {
        _gemMatrix[_propPosition.x][_propPosition.y]->affected(1);
        //        GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
        
        this->runAction(Sequence::create(MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)), MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)),MoveBy::create(0.05, Vec2(0, 5)),MoveBy::create(0.05, Vec2(0, -5)),NULL));
    }
    else if(_gemMatrix[_propPosition.x][_propPosition.y]->getGemType()>=dragontooth && _gemMatrix[_propPosition.x][_propPosition.y]->getGemType()<=pumpkin)
    {
        _gemMatrix[_propPosition.x][_propPosition.y]->addHudun();
        beforeMatch();
    }
    
    if(_gemMatrix[_propPosition.x][_propPosition.y] && _gemMatrix[_propPosition.x][_propPosition.y]->getState()!=-1)
    {
        _gemMatrix[_propPosition.x][_propPosition.y]->afterMatch(_propPosition.x, _propPosition.y);
        afterMatch();
    }
}

void DuplicateLayer::crossAnimation()
{
    //    GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
    for (int i = 0; i < kMatrixWidth; i++)
    {
        if (i == _propPosition.x)
        {
            continue;
        }
        if (_gemMatrix[i][_propPosition.y])
        {
            _gemMatrix[i][_propPosition.y]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i][_propPosition.y]->beforeExplode();
        }
    }
    
    for (int j = 0; j < kMatrixWidth; j++)
    {
        if (j == _propPosition.y)
        {
            continue;
        }
        if (_gemMatrix[_propPosition.x][j])
        {
            _gemMatrix[_propPosition.x][j]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[_propPosition.x][j]->beforeExplode();
        }
    }
    
    
    _gemMatrix[_propPosition.x][_propPosition.y]->explode(_propPosition, _propPosition, 0, 0);
    _gemMatrix[_propPosition.x][_propPosition.y]->beforeExplode();
    
    _propkind = PropNone;
    _crossSprite->setOpacity(255);
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    backLayerColor->setContentSize(Size(1000, 2000));
    backLayerColor->setPosition(Vec2(-100, -200));
    this->getParent()->addChild(backLayerColor,99);
    
    auto callback = CallFuncN::create(
                                      [&](Node *pSender)
                                      {
                                          pSender->removeFromParentAndCleanup(true);
                                      } );
    backLayerColor->runAction(Sequence::create(DelayTime::create(0.8) , callback->clone() ,NULL));
    this->getParent()->runAction(Sequence::create(MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),NULL));
}

void DuplicateLayer::aroundAnimation(Node* pSender)
{
    _aroundSprite->setOpacity(255);
    
    
    //    GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
    int i = _propPosition.x;
    int j = _propPosition.y;
    if(i - 1 >= 0)
    {
        if (j + 1 < kMatrixWidth && _gemMatrix[i-1][j+1])
        {
            _gemMatrix[i-1][j+1]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i-1][j+1]->beforeExplode();
        }
        if (_gemMatrix[i-1][j])
        {
            _gemMatrix[i-1][j]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i-1][j]->beforeExplode();
        }
        if (j - 1 >= 0 && _gemMatrix[i-1][j-1])
        {
            _gemMatrix[i-1][j-1]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i-1][j-1]->beforeExplode();
        }
    }
    if (i + 1 < kMatrixWidth)
    {
        if (j + 1 < kMatrixWidth && _gemMatrix[i+1][j+1])
        {
            _gemMatrix[i+1][j+1]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i+1][j+1]->beforeExplode();
        }
        if (_gemMatrix[i+1][j])
        {
            _gemMatrix[i+1][j]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i+1][j]->beforeExplode();
        }
        if (j - 1 >= 0 && _gemMatrix[i+1][j-1])
        {
            _gemMatrix[i+1][j-1]->explode(_propPosition, _propPosition, 0, 0);
            _gemMatrix[i+1][j-1]->beforeExplode();
        }
    }
    if (j - 1 >=0 && _gemMatrix[i][j-1])
    {
        _gemMatrix[i][j-1]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i][j-1]->beforeExplode();
    }
    if (j + 1 < kMatrixWidth && _gemMatrix[i][j+1])
    {
        _gemMatrix[i][j+1]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i][j+1]->beforeExplode();
    }
    if (i-2 >= 0 && _gemMatrix[i-2][j])
    {
        _gemMatrix[i-2][j]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i-2][j]->beforeExplode();
    }
    if (i+2 < kMatrixWidth && _gemMatrix[i+2][j])
    {
        _gemMatrix[i+2][j]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i+2][j]->beforeExplode();
    }
    if (j-2 >= 0 && _gemMatrix[i][j-2])
    {
        _gemMatrix[i][j-2]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i][j-2]->beforeExplode();
    }
    if (j+2 < kMatrixWidth && _gemMatrix[i][j+2])
    {
        _gemMatrix[i][j+2]->explode(_propPosition, _propPosition, 0, 0);
        _gemMatrix[i][j+2]->beforeExplode();
    }
    
    _gemMatrix[i][j]->explode(_propPosition, _propPosition, 0, 0);
    _gemMatrix[i][j]->beforeExplode();
    
    _propkind = PropNone;
    auto prop = (Armature*)this->getChildByTag(PROP_TAG);
    LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
    backLayerColor->setContentSize(Size(1000, 2000));
    backLayerColor->setPosition(Vec2(-100, -200));
    this->getParent()->addChild(backLayerColor,99);
    
    auto sprAnim = Sprite::create("effect_jiugong01.png");
    this->addChild(sprAnim,100);
    sprAnim->setPosition(Vec2(prop->getPosition()));
    auto animation = Animation::create();
    
    for (int i = 1; i < 9; i++)
    {
        __String *file = __String::createWithFormat("effect_jiugong0%d.png",i);
        
        auto sprite = Sprite::create(file->getCString());
        animation->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    auto action = Animate::create(animation);
    
    auto callback = CallFuncN::create(
                                      [&](Node *pSender)
                                      {
                                          pSender->removeFromParentAndCleanup(true);
                                      } );
    
    sprAnim->runAction(Sequence::create(action , callback ,NULL));
    backLayerColor->runAction(Sequence::create(DelayTime::create(0.9) , callback->clone() ,NULL));
    this->getParent()->runAction(Sequence::create(MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),MoveTo::create(0.025, Vec2(-25,0)) , MoveTo::create(0.025, Vec2(0,0)), MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(-25,0)) ,MoveTo::create(0.025, Vec2(0,0)) ,MoveTo::create(0.025, Vec2(25,0)) ,MoveTo::create(0.025, Vec2(0,0)),NULL));
    
    prop->removeFromParentAndCleanup(true);
    
}

bool DuplicateLayer::sameGemVector()
{
    int times = 0;
    int j = kMatrixWidth - 1;
    
    while (j >= 0)
    {
        for(int i = 0;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][j] && _gemMatrix[i][j]->getSameAndSkill() && _gemMatrix[i][j]->getState() != -1)
            {
                _gemMatrix[i][j]->setState(-1);
                _gemMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0.3 + 0.2 * times),CallFunc::create(CC_CALLBACK_0(Gem::removeSameSkill, _gemMatrix[i][j])), NULL));
                times ++;
            }
        }
        j--;
    }
    if (times>0)
    {
        return true;
    }
    return false;
}

bool DuplicateLayer::autoSkill()
{
    bool resule = false;
    for(int i=0;i<kMatrixWidth;i++)
    {
        for (int j=0;j<kMatrixWidth; j++)
        {
            if(_gemMatrix[i][j]&&(_gemMatrix[i][j]->getGemSkill() == SkillAround1 || _gemMatrix[i][j]->getGemSkill() == SkillAround3))
            {
                MyPoint mp = MyPoint(i, j);
                _gemMatrix[i][j]->explode(mp, mp, 0, 0);
                _gemMatrix[i][j]->beforeExplode();
                resule = true;
                //                return true;   //一个个 爆炸
            }
        }
    }
    return resule;
}

//移动之后，和填空之后，均会执行beforematch函数
void DuplicateLayer::beforeMatch()
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&& _gemMatrix[i][j]->getNextSkill() && _gemMatrix[i][j]->getGemSkill() != SkillAround1 && _gemMatrix[i][j]->getGemSkill() != SkillAround3)
            {
                _gemMatrix[i][j]->setGemSkill(_gemMatrix[i][j]->getNextSkill());
                _gemMatrix[i][j]->setNextSkill(SkillNull);
                _gemMatrix[i][j]->setGemType();
            }
        }
    }
    
    
    _mapLayer->findConnection();
    
    
    if(!_mapLayer->connectionAreaVector.empty())
    {
        //标记所有相连的为Eliminate,标记周围的为Changed,标记changed采用累加形式，派生者根据累加次数调整状态
        _mapLayer->markeMatrixChange();
    }
    //处理特殊元素（窝，蛋）及病态元素
    //    _mapLayer->beforeMatchGem();
    
    //    if(_isMoveDone&&_selectPoint.x!=-1&&_nextPoint.x!=-1)
    //    {
    //        _mapLayer->tigerEatAnimal(_selectPoint,_nextPoint);
    //    }
    
    if(_mapLayer->connectionAreaVector.empty())
    {
        if (autoSkill())
        {
            return;
        }
        else if (sameGemVector())
        {
            return;
        }
        
        afterMatch();
        if (!DataCenter::getInstance()->getWinnerMode())
        {
            _mapLayer->afterMatch();
        }
        
        return;
    }
    else
    {
        _continueMatchTimes++;
        
        _mapLayer->beforeMatchGem(&_animationWraperVector, _selectPoint, _nextPoint);
        //清空积累值
        if(_isStartMove&&_selectPoint.x!=-1&&_selectPoint.y!=-1&&!DataCenter::getInstance()->getWinnerMode())
        {
            _mapLayer->renewGemCount();
        }
        //        _mapLayer->matchAllGem(&_animationWraperVector,_continueMatchTimes);
        
        
        playContinueMatchMusic(_continueMatchTimes);
        
        dealGemBeforeExplode();
    }
}
void DuplicateLayer::afterMatch()
{
    int i,j;
    
    _fallGemCount=0;
    
    _explodeAll = false;
    
    //清理移用于标记的移动点
    _selectPoint.reset();
    
    _nextPoint.reset();
    
    //所有被清除的点在_emptyPointVector中,将地图中其所在位置设置为空
    
    for(i=0;i<_emptyPointVector.size();i++)
    {
        MyPoint p = _emptyPointVector[i];
        
        _gemMatrix[p.x][p.y]=NULL;
    }
    //对所有非空元素清空动作序列
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->stopHighLight();
                _gemMatrix[i][j]->clearAllAction();
                _gemMatrix[i][j]->setVisible(true);
            }
        }
    }
    _mapLayer->dealFallDown(this);
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getActionVectorSize()>0)
            {
                _fallGemCount++;
            }
        }
    }
    //如果此轮没有空出现，则进入等待输入流程
    if(_fallGemCount==0)
    {   //根据联消次数不同，播放不同动画
        
        _mapLayer->findConnection();
        
        if(!_mapLayer->connectionAreaVector.empty())
        {
            _continueMatchTimes++;
            
            _mapLayer->beforeMatchGem(&_animationWraperVector, _selectPoint, _nextPoint);
            //清空积累值
            if(_isStartMove&&_selectPoint.x!=-1&&_selectPoint.y!=-1&&!DataCenter::getInstance()->getWinnerMode())
            {
                _mapLayer->renewGemCount();
            }
            _mapLayer->matchAllGem(&_animationWraperVector,_continueMatchTimes);
            
            _mapLayer->afterMatch();
            
            playContinueMatchMusic(_continueMatchTimes);
            
            _mapLayer->beforeMatchGem();
   
            return;
        }
        
        if(_continueMatchTimes>=kMatch_great_level&&_propkind==PropNone&&!DataCenter::getInstance()->getWinnerMode()/*&&!_mapInfo->getHeroMode()*/)
        {
            CCLOG("%d",_continueMatchTimes);
            if(_continueMatchTimes>=kMatch_awesome_level)
            {
                _matchRepeatReminder = Armature::create( "unbelievable_effect");
            }
            else if(_continueMatchTimes>=kMatch_perfect_level)
            {
                _matchRepeatReminder = Armature::create( "excellent_effect");
            }
            else if(_continueMatchTimes>=kMatch_great_level)
            {
                _matchRepeatReminder = Armature::create( "nice_effect");
            }
            _matchRepeatReminder->setPosition(Director::getInstance()->getWinSize().width/2, 500);
            _matchRepeatReminder->getAnimation()->playWithIndex(0);
            _matchRepeatReminder->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(DuplicateLayer::matchRepeatCallback, this) );
            
            this->getParent()->addChild(_matchRepeatReminder,99);
            
            _matchCollectTimes = 0;
            _continueMatchTimes = 0;
            return;
        }
        
        _matchCollectTimes = 0;
        
        
        if(/*!_mapInfo->getHeroMode()&&_mapInfo->getRestStep()>0*/!DataCenter::getInstance()->getWinnerMode())
        {
            _hightLightSwitch = true;
            
            findPotentialOrRenew();
        }
        
        if(_propkind!=PropNone)
        {
            propUseOver();
        }
        
        //已经开始移动，并且没有使用道具
        if(_isStartMove&&_propkind==PropNone&&_matchDownMSGSwitch)
        {
            for(i=0;i<kMatrixWidth;i++)
            {
                for(j=0;j<kMatrixWidth;j++)
                {
                    if(_gemMatrix[i][j])
                    {
                        _gemMatrix[i][j]->setScore(5);
                    }
                }
            }
            
            _matchDownMSGSwitch = false;
            
            CCNotificationCenter::getInstance()->postNotification(kMSG_MatchDown);
        }
    }
    else//有空出现，填空，填空完成后执行回调
    {
        _isFallDownEnd = false;
        
        _isStartMove = true;
        
        if(_fallGemCount<=0)
        {
            CallFunc* callback = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::gemFallCallback, this));
            this->runAction(callback);
        }
        else
        {
            this->fallDownToEnd(NULL, 0);
        }
        
        _emptyPointVector.clear();
    }
}

void DuplicateLayer::dealGemBeforeExplode()
{
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    
    bool triggerSkill = false;
    
    for( it = _mapLayer->connectionAreaVector.begin();it!=_mapLayer->connectionAreaVector.end();it++)
    {
        j = it->startPoint.y;
        i = it->startPoint.x;
        
        if (it->direction == Up)
        {
            for ( ;j < it->startPoint.y + it->count; j++)
            {
                MyPoint point(i,j);
                
                _gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
                                          it->startPoint.y+it->count-j);
                int index = _continueMatchTimes < 5 ? _continueMatchTimes:5;
                
                _gemMatrix[i][j]->setScore(index * 2);
                
                if (_gemMatrix[i][j]->getGemSkill())
                {
                    triggerSkill = true;
                }
            }
            
        }
        else
        {
            for (; i < it->startPoint.x + it->count; i++)
            {
                MyPoint point(i,j);
                
                _gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
                                          it->startPoint.x+it->count-i);
                int index = _continueMatchTimes < 5 ? _continueMatchTimes:5;
                
                _gemMatrix[i][j]->setScore(index * 2);
                
                if (_gemMatrix[i][j]->getGemSkill())
                {
                    triggerSkill = true;
                }
            }
        }
        
        MyPoint temp;
        
        if (it->direction == Up)
        {
            if (it->centerPoint.y + 1 < it->startPoint.y + it->count)
            {
                temp = MyPoint(it->centerPoint.x, it->centerPoint.y + 1);
            }
            else
            {
                temp = MyPoint(it->centerPoint.x, it->centerPoint.y - 1);
            }
        }
        else
        {
            if (it->centerPoint.x + 1 < it->startPoint.x + it->count)
            {
                temp = MyPoint(it->centerPoint.x + 1, it->centerPoint.y);
            }
            else
            {
                temp = MyPoint(it->centerPoint.x - 1, it->centerPoint.y);
            }
        }
        
        
        
        if (triggerSkill)
        {
            it =_mapLayer->connectionAreaVector.begin();
            j = it->startPoint.y;
            i = it->startPoint.x;
            _gemMatrix[i][j]->triggerSkill();
        }
        else
        {
            for( it =_mapLayer->connectionAreaVector.begin();it!=_mapLayer->connectionAreaVector.end();it++)
            {
                j = it->startPoint.y;
                i = it->startPoint.x;
                
                if (it->direction == Up)
                {
                    for ( ;j < it->startPoint.y + it->count; j++)
                    {
                        _gemMatrix[i][j]->removeNoCollect(false);
                    }
                    
                }
                else
                {
                    for (; i < it->startPoint.x + it->count; i++)
                    {
                        if (it->isInCross)
                        {
                            MyPoint mp = MyPoint(i, j);
                            if (mp.equal(it->centerPoint))
                            {
                                continue;
                            }
                        }
                        _gemMatrix[i][j]->removeNoCollect(false);
                        
                    }
                }
            }
        }
    }
}

void DuplicateLayer::gemFallNotification(Ref *obj)
{
    gemFallCallback();
}

void DuplicateLayer::gemFallCallback()
{
    if(_isFallDownEnd) return;
    
    _fallGemCount--;
    
    if(_fallGemCount<=0 && !_explodeAll)
    {
        _isFallDownEnd = true;
        
        this->runAction(CallFunc::create(CC_CALLBACK_0(DuplicateLayer::beforeMatch, this)));
    }
}

Point DuplicateLayer::calculateStartPoint(float edgeWidth,Size& winSize,float scaleFactor)
{
    int leftIndex=kMatrixWidth,rightIndex=0,upIndex=0,downIndex=kMatrixWidth;
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_mapMatrix[i][j]!=NoneInEdge&&_mapMatrix[i][j]!=NoneInMiddle)
            {
                if(i<leftIndex)
                {
                    leftIndex = i;
                }
                if(i>rightIndex)
                {
                    rightIndex = i;
                }
                if(j>upIndex)
                {
                    upIndex = j;
                }
                if(j<downIndex)
                {
                    downIndex = j;
                }
            }
        }
    }
    int startX = kMatrixWidth-rightIndex-leftIndex-1;
    int startY = kMatrixWidth-upIndex-downIndex-1;
    
    auto skillBG = Sprite::create("map01_003.png");
    return Point(edgeWidth+startX*0.5*scaleFactor*kElementSize,skillBG->getBoundingBox().size.height/1.5+startY*0.5*scaleFactor*kElementSize);
}

void DuplicateLayer::addShieldLayer()
{
    SwallowLayer *noTouchLayer = SwallowLayer::create();
    
    this->addChild(noTouchLayer,-129,9999);
}

void DuplicateLayer::removeShieldLayer()
{
    this->removeChildByTag(9999);
}

void DuplicateLayer::matrixBright()
{
    Vector<FiniteTimeAction*> actionArray;
    
    int i,k;
    
    for(k=1;k<=kMatrixWidth;k++)
    {
        for(i=0;i<k;i++)
        {
            if(_gemMatrix[i][i+kMatrixWidth-k])
            {
                CallFunc* callAction1 = CallFunc::create(CC_CALLBACK_0(Gem::bright, _gemMatrix[i][i+kMatrixWidth-k]));
                actionArray.pushBack(callAction1);
            }
        }
        
        actionArray.pushBack(DelayTime::create(0.04));
    }
    
    for(k=kMatrixWidth-1;k>=1;k--)
    {
        for(i=kMatrixWidth-k;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][i-kMatrixWidth+k])
            {
                CallFunc* callAction2 = CallFunc::create(CC_CALLBACK_0(Gem::bright, _gemMatrix[i][i-kMatrixWidth+k]));
                
                actionArray.pushBack(callAction2);
            }
        }
        actionArray.pushBack(DelayTime::create(0.04));
    }
    this->runAction(Sequence::create(actionArray));
}

void DuplicateLayer::shining()
{
    initExplodeFlyTime();//初始化收集元素飞行时间
    
    Vector<FiniteTimeAction*> actionArray;
    
    int i,j,k;
    
    for(k=1;k<=kMatrixWidth;k++)
    {
        for(i=0;i<k;i++)
        {
            if(_gemMatrix[i][i+kMatrixWidth-k])
            {
                CallFunc* callAction1 = CallFunc::create(CC_CALLBACK_0(Gem::bright, _gemMatrix[i][i+kMatrixWidth-k]));
                actionArray.pushBack(callAction1);
            }
        }
        
        actionArray.pushBack(DelayTime::create(0.02));
    }
    
    for(k=kMatrixWidth-1;k>=1;k--)
    {
        for(i=kMatrixWidth-k;i<kMatrixWidth;i++)
        {
            if(_gemMatrix[i][i-kMatrixWidth+k])
            {
                CallFunc* callAction2 = CallFunc::create(CC_CALLBACK_0(Gem::bright, _gemMatrix[i][i-kMatrixWidth+k]));
                
                actionArray.pushBack(callAction2);
            }
        }
        actionArray.pushBack(DelayTime::create(0.02));
    }
    
    actionArray.pushBack(DelayTime::create(0.3));
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j])
            {
                
            }
        }
    }
    
    actionArray.pushBack(DelayTime::create(3*kBrightScaleTime));
    
    CallFunc* callback = NULL;
    
    if(/*_mapInfo->getHard()*/false)
    {
        callback = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::bossAppear, this));
        
        actionArray.pushBack(callback);
    }
    else
    {
        callback = CallFunc::create(CC_CALLBACK_0(DuplicateLayer::beforeMatch, this));
        
        actionArray.pushBack(callback);
    }
    
    actionArray.pushBack(CallFunc::create(CC_CALLBACK_0(DuplicateLayer::addClassLayer, this)));
    
    this->runAction(Sequence::create(actionArray));
}

bool DuplicateLayer::beforeWinnerMode()
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j] && _gemMatrix[i][j]->getGemSkill())
            {
                return true;
            }
        }
    }
    return false;
}

void DuplicateLayer::winnerMode(Ref *obj)
{
    __String *str = (__String*)obj;
    
    bool isBreak = false;
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j] && _gemMatrix[i][j]->getGemSkill())
            {
                MyPoint mp = MyPoint(i, j);
                _gemMatrix[i][j]->explode(mp,mp , 0, 0);
                _gemMatrix[i][j]->beforeExplode();
                isBreak = true;
            }
        }
    }
    
    if (isBreak)
    {
        _matchDownMSGSwitch = true;
        return;
    }
    
    if(_winnerModeStarsNode==NULL)
    {
        _winnerModeStarsNode = Node::create();
        
        this->addChild(_winnerModeStarsNode,kParticleZOrder);
        
        winnerModeShiningInBackgroud(NULL, 0);
    }
    
    this->setTouchDisable();
    
    if(!_isWinnerModeStart)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for(int j=0;j<kMatrixWidth;j++)
            {
                if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getSick())
                {
                    _gemMatrix[i][j]->setSick(false);
                }
            }
        }
        _isWinnerModeStart = true;
    }
    
    //生成winnermode动画
    //1,根据当前的moves值随机找到不大于10个的位置，生成飞过去动画,更新moves
    //2,地图中被选中的元素执行横排/竖排消除,生成动画,调用这些元素的explode函数
    //3,执行runallanimation.
    
    int flagArray[kMatrixWidth*kMatrixWidth]={0};
    int tempArray[kMatrixWidth*kMatrixWidth]={0};
    
    int i,j,max=0,index,k,l;
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getState()!=-1&&_gemMatrix[i][j]->canMove()&&_gemMatrix[i][j]->getGemKind()==NormalGem)
            {
                flagArray[i*kMatrixWidth+j]=1;
            }
        }
    }
    //取出所有可以替换的位置
    for(i=0;i<kMatrixWidth*kMatrixWidth;i++)
    {
        if(flagArray[i]==1)
        {
            tempArray[max++]=i;
        }
    }
    
    
    int moves = str->intValue()>10?10:str->intValue();
    
    moves = moves>max?max:moves;
    
    Gem::getUniqueNumber(-1);
    //所有位置生成动画
    AnimationWraper aw(NULL,e_aid_winnermode_starfly,e_priority_winnermode_starfly);
    
    if(max<=0)
    {
        aw.flagArrayLength = 0;
    }
    else
    {
        aw.flagArrayLength = moves;
    }
    
    for(i=0;i<moves&&max>0;i++)
    {
        index = Gem::getUniqueNumber(max--);
        
        aw.flagArray[i]=tempArray[index];
        
        MyPoint p(tempArray[index]/kMatrixWidth,tempArray[index]%kMatrixWidth);
        
        _gemMatrix[p.x][p.y]->explode(p,p, 0, 0 );
    }
    
    _matchDownMSGSwitch = true;
    winnerModeStarfly(aw);
}

void DuplicateLayer::winnerModeStarfly(AnimationWraper aw)
{
    Point startPosition = this->convertToNodeSpace(DataCenter::getInstance()->getPosStar()/* + Point(0,CCDirector::sharedDirector()->getWinSize().height)*/);
    
    for(int i=0;i<aw.flagArrayLength;i++)
    {
        int index = aw.flagArray[i];
        
        MyPoint distPoint(index/kMatrixWidth,index%kMatrixWidth);
        
        ParticleSystemQuad *sparkle = ParticleSystemQuad::create("feilv.plist");
        
        Point endPosition = _gemMatrix[distPoint.x][distPoint.y]->getPosition();
        
        float t1 = startPosition.y-endPosition.y;
        float t2 = startPosition.x-endPosition.x;
        double tan = (atan(t2/t1)*360)/(2*3.14159);
        sparkle->setZOrder(1);
        sparkle->setPosition(startPosition - Vec2(0, 40));
        sparkle->setAnchorPoint(Point(0.5,0));
        sparkle->setRotation(tan);
        sparkle->setVisible(false);
        sparkle->setScale(0.6);
        _particleNode->addChild(sparkle);
        
        
        Sequence* seq=NULL;
        
        if(i==aw.flagArrayLength-1)
        {
            CallFuncN* callback = CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeExplode, this , aw));
            seq = Sequence::create(DelayTime::create(i*0.2),CCCallFuncN::create(CC_CALLBACK_1(DuplicateLayer::showSparkle, this)),MoveTo::create(kPriority5_delaytime-0.3, _gemMatrix[distPoint.x][distPoint.y]->getPosition()),/*playEffectAction,*/CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sparkle)),CallFunc::create(CC_CALLBACK_0(Gem::winnerModeStart,_gemMatrix[distPoint.x][distPoint.y])),callback,NULL);
        }
        else
        {
            seq = Sequence::create(DelayTime::create(i*0.2),CCCallFuncN::create(CC_CALLBACK_1(DuplicateLayer::showSparkle, this)),MoveTo::create(kPriority5_delaytime-0.3, _gemMatrix[distPoint.x][distPoint.y]->getPosition()),/*playEffectAction,*/CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sparkle)),CallFunc::create(CC_CALLBACK_0(Gem::winnerModeStart, _gemMatrix[distPoint.x][distPoint.y]) ),NULL);
        }
        
        sparkle->runAction(seq);
    }
    if(aw.flagArrayLength==0)
    {
        CCLOG("flagArrayLength = %d" , aw.flagArrayLength);
    }
    
}

void DuplicateLayer::winnerModeExplode(Node *pSender, AnimationWraper aw)
{
    for(int i=0;i<aw.flagArrayLength;i++)
    {
        int index = aw.flagArray[i];
        
        MyPoint distPoint(index/kMatrixWidth,index%kMatrixWidth);
                
        _gemMatrix[distPoint.x][distPoint.y]->runAction(Sequence::create(DelayTime::create(1.1 + 0.2 * i) ,CallFunc::create(CC_CALLBACK_0(Gem::changeSkill, _gemMatrix[distPoint.x][distPoint.y])) ,CallFunc::create(CC_CALLBACK_0(Gem::beforeExplode, _gemMatrix[distPoint.x][distPoint.y])) ,NULL));
    }
}

void DuplicateLayer::propUseStart(Ref* object)
{
    _matchDownMSGSwitch = true;
    
    int t =  0;//= (int)object;
    
    _propkind = (PropKind)t;
    
    stopRemindAndIdle();//停止提示和空闲动作
    
    if(_propkind==PropAddOne||_propkind==PropCureSick)
    {
        MyPoint p(0,0);
        
        propUse(p);
    }
    else
    {
        if(_propStarstNode!=NULL)
        {
            _propStarstNode->setVisible(true);
            return;
        }
        else
        {
            _propStarstNode = Node::create();
            
            this->addChild(_propStarstNode,kParticleZOrder);
        }
        
        propShiningInBackgroud(NULL,0);
    }
    
}

void DuplicateLayer::skillGemChange()
{
    
    GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
    
    GemSkill firstSkill = _gemMatrix[_selectPoint.x][_selectPoint.y]->getGemSkill();
    GemSkill secondSkill = _gemMatrix[_nextPoint.x][_nextPoint.y]->getGemSkill();
    
    _gemMatrix[_selectPoint.x][_selectPoint.y]->setGemSkill(SkillNull);
    _gemMatrix[_nextPoint.x][_nextPoint.y]->setGemSkill(SkillNull);
    _gemMatrix[_selectPoint.x][_selectPoint.y]->setNextSkill(SkillNull);
    _gemMatrix[_nextPoint.x][_nextPoint.y]->setNextSkill(SkillNull);
    
    if (firstSkill == SkillAllSame && secondSkill == SkillAllSame)
    {
        explodeAllGem(_gemMatrix[_nextPoint.x][_nextPoint.y]);
        return;
    }
    else if(firstSkill == SkillAllSame || secondSkill == SkillAllSame)
    {
        GemSkill skill = SkillNull;
        
        MyPoint allPonit;
        MyPoint normalPonit;
        if (firstSkill)
        {
            if (firstSkill == SkillAllSame)
            {
                _gemMatrix[_selectPoint.x][_selectPoint.y]->setGemSkill(SkillNull);
                allPonit = _selectPoint;
                normalPonit = _nextPoint;
            }
            else
            {
                skill = firstSkill;
                allPonit = _nextPoint;
                normalPonit = _selectPoint;
            }
        }
        if (secondSkill)
        {
            if (secondSkill == SkillAllSame)
            {
                _gemMatrix[_nextPoint.x][_nextPoint.y]->setGemSkill(SkillNull);
                allPonit = _nextPoint;
                normalPonit = _selectPoint;
            }
            else
            {
                skill = secondSkill;
                allPonit = _selectPoint;
                normalPonit = _nextPoint;
            }
        }
        
        flyToSame(allPonit, normalPonit,skill);
        return;
        
    }
    else
    {
        if ((firstSkill < SkillAround1 && secondSkill < SkillAround1))
        {
            _gemMatrix[_selectPoint.x][_selectPoint.y]->setGemSkill(SkillNull);
            _gemMatrix[_nextPoint.x][_nextPoint.y]->setGemSkill(SkillCross);
            
            beforeMatch();
            return;
        }
        
        if((firstSkill < SkillAround1 && secondSkill == SkillAround2) || (firstSkill == SkillAround2 && secondSkill < SkillAround1))
        {
            _gemMatrix[_selectPoint.x][_selectPoint.y]->setGemSkill(SkillNull);
            _gemMatrix[_nextPoint.x][_nextPoint.y]->setGemSkill(SkillThreeCross);
            
            beforeMatch();
            return;
        }
        
        if (firstSkill == SkillAround2 && secondSkill == SkillAround2)
        {
            _gemMatrix[_selectPoint.x][_selectPoint.y]->setGemSkill(SkillNull);
            _gemMatrix[_nextPoint.x][_nextPoint.y]->setGemSkill(SkillAround4);
            
            beforeMatch();
            return;
        }
        
    }
    
}

bool DuplicateLayer::propUse(MyPoint &point)
{
    if (_gemMatrix[point.x][point.y])
    {
        return true;
    }
    return false;
    
    
    switch (_propkind)
    {
        case PropSingle:
        {
            if(_gemMatrix[point.x][point.y])
            {
                if(_gemMatrix[point.x][point.y]->getGemKind()==NormalGem)
                {
                    _gemMatrix[point.x][point.y]->explode(point, point, 0, 0);
                    
                }
                else if(_gemMatrix[point.x][point.y]->getGemType()>=ice && _gemMatrix[point.x][point.y]->getGemType()<=stormwind)
                {
                    _gemMatrix[point.x][point.y]->affected(1);
                    
                }
                else if(_gemMatrix[point.x][point.y]->getGemType()>=dragontooth && _gemMatrix[point.x][point.y]->getGemType()<=pumpkin)
                {
                    
                    _gemMatrix[point.x][point.y]->addHudun();
                    return false;
                }
                
                if(_gemMatrix[point.x][point.y]->getState()!=-1)
                {
                    _gemMatrix[point.x][point.y]->afterMatch(point.x, point.y);
                }
            }
            else
            {
                return false;
            }
        }
            break;
        case PropAround:
        {
            if(_gemMatrix[point.x][point.y])
            {
                int i = point.x;
                int j = point.y;
                if(i - 1 >= 0)
                {
                    if (j + 1 < kMatrixWidth && _gemMatrix[i-1][j+1])
                    {
                        MyPoint mp(i-1,j+1);
                        _gemMatrix[i-1][j+1]->explode(mp, mp, 0, 0);
                    }
                    if (_gemMatrix[i-1][j])
                    {
                        MyPoint mp(i-1,j);
                        _gemMatrix[i-1][j]->explode(mp, mp, 0, 0);
                    }
                    if (j - 1 >= 0 && _gemMatrix[i-1][j-1])
                    {
                        MyPoint mp(i-1,j-1);
                        _gemMatrix[i-1][j-1]->explode(mp, mp, 0, 0);
                    }
                }
                if (i + 1 < kMatrixWidth)
                {
                    if (j + 1 < kMatrixWidth && _gemMatrix[i+1][j+1])
                    {
                        MyPoint mp(i+1,j+1);
                        _gemMatrix[i+1][j+1]->explode(mp, mp, 0, 0);
                    }
                    if (_gemMatrix[i+1][j])
                    {
                        MyPoint mp(i+1,j);
                        _gemMatrix[i+1][j]->explode(mp, mp, 0, 0);
                    }
                    if (j - 1 >= 0 && _gemMatrix[i+1][j-1])
                    {
                        MyPoint mp(i+1,j-1);
                        _gemMatrix[i+1][j-1]->explode(mp, mp, 0, 0);
                    }
                }
                if (j - 1 >=0 && _gemMatrix[i][j-1])
                {
                    MyPoint mp(i,j-1);
                    _gemMatrix[i][j-1]->explode(mp, mp, 0, 0);
                }
                if (j + 1 < kMatrixWidth && _gemMatrix[i][j+1])
                {
                    MyPoint mp(i,j+1);
                    _gemMatrix[i][j+1]->explode(mp, mp, 0, 0);
                }
                if (i-2 >= 0 && _gemMatrix[i-2][j])
                {
                    MyPoint mp(i-2,j);
                    _gemMatrix[i-2][j]->explode(mp, mp, 0, 0);
                    
                }
                if (i+2 < kMatrixWidth && _gemMatrix[i+2][j])
                {
                    MyPoint mp(i+2,j);
                    _gemMatrix[i+2][j]->explode(mp, mp, 0, 0);
                    
                }
                if (j-2 >= 0 && _gemMatrix[i][j-2])
                {
                    MyPoint mp(i,j-2);
                    _gemMatrix[i][j-2]->explode(mp, mp, 0, 0);
                }
                if (j+2 < kMatrixWidth && _gemMatrix[i][j+2])
                {
                    MyPoint mp(i,j+2);
                    _gemMatrix[i][j+2]->explode(mp, mp, 0, 0);
                }
                
                
                _gemMatrix[i][j]->explode(point, point, 0, 0);
                
                
                if(_gemMatrix[point.x][point.y]->getState()!=-1)
                {
                    _gemMatrix[point.x][point.y]->afterMatch(point.x, point.y);
                }
                
            }
            else
            {
                return false;
            }
        }
            break;
        case PropCross:
        {
            if(_gemMatrix[point.x][point.y])
            {
                for (int i = 0; i < kMatrixWidth; i++)
                {
                    if (i == point.x)
                    {
                        continue;
                    }
                    if (_gemMatrix[i][point.y])
                    {
                        MyPoint mp(i,point.y);
                        _gemMatrix[i][point.y]->explode(mp, mp, 0, 0);
                    }
                }
                
                for (int j = 0; j < kMatrixWidth; j++)
                {
                    if (j == point.y)
                    {
                        continue;
                    }
                    if (_gemMatrix[point.x][j])
                    {
                        MyPoint mp(point.x,j);
                        _gemMatrix[point.x][j]->explode(mp, mp, 0, 0);
                    }
                }
                
                
                _gemMatrix[point.x][point.y]->explode(point, point, 0, 0);
                
                
                if(_gemMatrix[point.x][point.y]->getState()!=-1)
                {
                    _gemMatrix[point.x][point.y]->afterMatch(point.x, point.y);
                }
                
            }
            else
            {
                return false;
            }
        }
            break;
        case PropOneKind:
        {
            if(_gemMatrix[point.x][point.y]&&_gemMatrix[point.x][point.y]->getGemSkill()==SkillNull&&_gemMatrix[point.x][point.y]->canMove()&&_gemMatrix[point.x][point.y]->getGemKind()==NormalGem)
            {
                _skillType = SkillNull;
                int random = arc4random()%100;
                
                if (random < 15)//15
                {
                    _skillType = SkillAllSame;
                }
                else if(random < 55)//55
                {
                    _skillType = SkillAround2;
                }
                else
                {
                    int index = arc4random()%2;
                    if (index)
                    {
                        _skillType = SkillVerticl;
                    }
                    else
                    {
                        _skillType = SkillHorizontal;
                    }
                }
            }
            else
            {
                if (_gemMatrix[point.x][point.y])
                {
                    _gemMatrix[point.x][point.y]->addHudun();
                }
                return false;
            }
        }
            break;
        default:
            break;
    }
    
    
    
}
void DuplicateLayer::propRemoveRow(float dt )
{
    Point point = _propCar->getPosition();
    
    MyPoint p((point.x-kStartX)/kElementSize,(point.y-kStartY)/kElementSize);
    
    if(p.x>=0&&p.x<kMatrixWidth&&p.y>=0&&p.y<kMatrixWidth&&_gemMatrix[p.x][p.y]&&_gemMatrix[p.x][p.y]->getGemKind()==NormalGem&&_gemMatrix[p.x][p.y]->canMove())
    {
        Point dist = _explodeDistArray[_gemMatrix[p.x][p.y]->getGemType()];
        
        if(!dist.equals(Point(-1,-1))&&!_gemMatrix[p.x][p.y]->getSick())
        {
            _gemMatrix[p.x][p.y]->removeCollect(dist,getExplodeFlyTime(p),false);
        }
        else
        {
            _gemMatrix[p.x][p.y]->removeNoCollect(false);
        }
        
        _gemMatrix[p.x][p.y]=NULL;
        
        _emptyPointVector.push_back(p);
    }
}
void DuplicateLayer::propUnschedule()
{
    unschedule(schedule_selector(DuplicateLayer::propRemoveRow));
}
void DuplicateLayer::propUseCancel()
{
    _propkind = PropNone;
    
    if(_propStarstNode)
    {
        _propStarstNode->setVisible(false);
    }
}

void DuplicateLayer::propUseOver()
{
    _propkind = PropNone;
    
    if(_propStarstNode)
    {
        _propStarstNode->setVisible(false);
    }
    
    NotificationCenter::getInstance()->postNotification(kMSG_PropUseOver);
}

void DuplicateLayer::propShiningInBackgroud(Node* sender,int data)
{
    
    int flag = data;
    
    int flag1 = flag/10;
    int flag2 = flag%10;
    
    int i,j;
    
    Point point;
    
    int flagMatrix[kMatrixWidth+2][kMatrixWidth+2]={0};
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(0==_mapMatrix[i][j]||NoneInMiddle==_mapMatrix[i][j])
            {
                flagMatrix[i+1][j+1]=0;
            }
            else
            {
                flagMatrix[i+1][j+1]=1;
            }
        }
    }
    
    for(int i=0;i<kMatrixWidth+2;i++)
    {
        for(int j=0;j<kMatrixWidth+2;j++)
        {
            int index = i*kMatrixWidth+j;
            
            if(flagMatrix[i][j]==0&&index%3==flag1)
            {
                //左侧不为空
                if(i-1>=0&&flagMatrix[i-1][j])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
  
                }
                //下不为空
                if(j-1>=0&&flagMatrix[i][j-1])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                  }
                //右侧不为空
                if(i+1<kMatrixWidth+2&&flagMatrix[i+1][j])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                  
                }
                //上不为空
                if(j+1<kMatrixWidth+2&&flagMatrix[i][j+1])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
           
                }
            }
        }
    }
    if(flag1==0&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::propShiningInBackgroud, this ,1)),NULL));
    }
    else if(flag1==0&&flag2==1)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::propShiningInBackgroud, this ,10)),NULL));
    }
    else if(flag1==1&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::propShiningInBackgroud, this ,11)),NULL));
    }
    else if(flag1==1&&flag2==1)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::propShiningInBackgroud, this ,20)),NULL));
    }
    else if(flag1==2&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::propShiningInBackgroud, this ,21)),NULL));
    }
}

void DuplicateLayer::winnerModeShiningInBackgroud(cocos2d::Node *sender, int data)
{
    
    int flag = data;
    
    int flag1 = flag/10;
    int flag2 = flag%10;
    
    int i,j;
    
    Point point;
    
    int flagMatrix[kMatrixWidth+2][kMatrixWidth+2]={0};
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(0==_mapMatrix[i][j]||NoneInMiddle==_mapMatrix[i][j])
            {
                flagMatrix[i+1][j+1]=0;
            }
            else
            {
                flagMatrix[i+1][j+1]=1;
            }
        }
    }
    
    for(int i=0;i<kMatrixWidth+2;i++)
    {
        for(int j=0;j<kMatrixWidth+2;j++)
        {
            int index = i*kMatrixWidth+j;
            
            if(flagMatrix[i][j]==0&&index%3==flag1)
            {
                //左侧不为空
                if(i-1>=0&&flagMatrix[i-1][j])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                 
                }
                //下不为空
                if(j-1>=0&&flagMatrix[i][j-1])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                }
                //右侧不为空
                if(i+1<kMatrixWidth+2&&flagMatrix[i+1][j])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.75)*kElementSize);
                    }
                }
                //上不为空
                if(j+1<kMatrixWidth+2&&flagMatrix[i][j+1])
                {
                    if(flag2==0)
                    {
                        point = Point(kStartX+(i-0.75)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                    else
                    {
                        point = Point(kStartX+(i-0.25)*kElementSize,kStartY+(j-0.25)*kElementSize);
                    }
                }
            }
        }
    }
    if(flag1==0&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeShiningInBackgroud, this , 1)),NULL));
        
    }
    else if(flag1==0&&flag2==1)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeShiningInBackgroud, this , 10)),NULL));
    }
    else if(flag1==1&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeShiningInBackgroud, this , 11)),NULL));
    }
    else if(flag1==1&&flag2==1)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeShiningInBackgroud, this , 20)),NULL));
    }
    else if(flag1==2&&flag2==0)
    {
        this->runAction(Sequence::create(DelayTime::create(0.2),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::winnerModeShiningInBackgroud, this , 1)),NULL));
    }
}

void DuplicateLayer::outStage(Ref* object)
{
    this->setTouchDisable();
    
    int flag = 0;//= (int)object
    
    Point startPosition = this->getPosition();
    
    Point endPosition =Point(Director::getInstance()->getWinSize().width*kGameLayerStartMultiple,startPosition.y);
    
    Sequence* seq = Sequence::create(CCEaseBackIn::create(MoveTo::create(kGameLayerFlyTime,endPosition)),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::sendOutStageMessage, this)),/*CallFunc::create(caller,callfunc_selector(GemAction::stopBackgroudMusic)),*/CallFunc::create(CC_CALLBACK_0(DuplicateLayer::removeDuplicateLayer, this)),NULL);

    if(_propStarstNode)
    {
        _propStarstNode->removeAllChildrenWithCleanup(true);
    }
    if(_winnerModeStarsNode)
    {
        _winnerModeStarsNode->removeAllChildrenWithCleanup(true);
    }
    
    if(flag==0)
    {
        this->runAction(seq);
        
        return;
    }
    else
    {
        Vector<FiniteTimeAction*> array ;
        
        int i,j;
        
        for(i=0;i<kMatrixWidth;i++)
        {
            for(j=0;j<kMatrixWidth;j++)
            {
                if(_gemMatrix[i][j])
                {
                    CallFunc* action = CallFunc::create(CC_CALLBACK_0(Gem::highLight, _gemMatrix[i][j]));
                    array.pushBack(action);
                }
            }
            array.pushBack(DelayTime::create(0.05));
        }
        array.pushBack(DelayTime::create(1));
        
        array.pushBack(seq);
        
        this->runAction(Sequence::create(array));
    }
}

void DuplicateLayer::sendOutStageMessage()
{
    NotificationCenter::getInstance()->postNotification(kMSG_GameLayer_AfterOutStage);
}
void DuplicateLayer::matchRepeatCallback()
{
    
    _matchRepeatReminder->removeFromParentAndCleanup(true);
}

void DuplicateLayer::playContinueMatchMusic(int matchTimes)
{
}

void DuplicateLayer::initExplodeFlyTime()
{
    map<GemType ,Point > mapGemPoint = DataCenter::getInstance()->getGemPointMap();
    map<GemType,Point>::iterator it;
    
    float sum=0,count=1;
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            Point startPoint = Point(kStartX+(i+0.5)*kElementSize,kStartY+(j+0.5)*kElementSize);
            
            for(it = mapGemPoint.begin();it != mapGemPoint.end();it++)
            {
                Point dist = DataCenter::getInstance()->getGemPoint(it->first);
                
                Point tempPoint = Point(dist.x,dist.y+CCDirector::getInstance()->getWinSize().height);
                
                Point endPoint = this->convertToNodeSpace(tempPoint);
                
                sum+=startPoint.getDistance( endPoint);
                
                count++;
            }
            
            float time = (sum/count)/KCollectMoveSpeed;
            
            _explodeFlyTime[i][j]=time;
        }
    }
    
}
float DuplicateLayer::getExplodeFlyTime(MyPoint &point)
{
    float time;
    
    if(point.x>=0&&point.y>=0&&point.x<kMatrixWidth&&point.y<kMatrixWidth)
    {
        time = _explodeFlyTime[point.x][point.y];
    }
    else
    {
        time = _explodeFlyTime[0][0];
    }
    
    return time;
}
void DuplicateLayer::removeGem(Node *sender, int data)
{
    int t = data;
    
    int i = t/kMatrixWidth,j=t%kMatrixWidth;
    
    Action* seq=NULL;
    
    if(j<0)
    {
        
        return;
    }
    int next=0;
    float time = 0.05;
    
    if(i==kMatrixWidth-1&&j>=0)
    {
        next = j-1;
    }
    else if(i<kMatrixWidth)
    {
        next = (i+1)*kMatrixWidth+j;
    }
    if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getState()<0)
    {
        Point dist = DataCenter::getInstance()->getGemPoint(_gemMatrix[i][j]->getGemType());
        
        MyPoint p(i,j);
        
        if(!dist.equals(Point(-1,-1)))
        {
            _matchCollectTimes+=_gemMatrix[i][j]->getScore();
            
            _gemMatrix[i][j]->removeCollect(dist, getExplodeFlyTime(p),_gemMatrix[i][j]->getFlag()==1);
        }
        else
        {
            _gemMatrix[i][j]->removeNoCollect(_gemMatrix[i][j]->getFlag()==1);
        }
        
        _emptyPointVector.push_back(p);
        
        if (_disappearByOder)
        {
            seq = Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::removeGem, this ,next)),NULL);
        }
        else
        {
            seq = Sequence::create(CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::removeGem, this ,next)),NULL); // 一起消失
        }
        
    }
    else
    {
        seq = CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::removeGem, this ,next));
    }
    
    this->runAction(seq);
}

void DuplicateLayer::addClassLayer()
{
    
}
void DuplicateLayer::classGuidMove(cocos2d::Ref *obj)
{
    
}
void DuplicateLayer::removeDuplicateLayer()
{
    this->removeFromParentAndCleanup(true);
}

void DuplicateLayer::showSparkle(cocos2d::Node *sender)
{
    if(sender)
    {
        NotificationCenter::getInstance()->postNotification(kMSG_UpdateMoves);
        sender->setVisible(true);
    }
}
void DuplicateLayer::fallDownToEnd(cocos2d::Node *sender,int data)
{
    int j = data;
    for(int i=0;i<kMatrixWidth;i++)
    {
        if(_gemMatrix[i][j])
        {
            _gemMatrix[i][j]->fallDownToEnd(true);
        }
    }
    if(j+1<kMatrixWidth)
    {
        this->runAction(Sequence::create(DelayTime::create(0.015),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::fallDownToEnd, this,(j+1))),NULL));
    }
    else
    {
        GemAction::getInstance().playEffectMusic(NULL, "dropdown.mp3"); //方案1
        _mapLayer->clearCounter();
    }
}

void DuplicateLayer::bossAppear()
{
    
}

void DuplicateLayer::bossAppearCallback()
{
    if(_bossNode)
    {
        _bossNode->removeFromParentAndCleanup(true);
    }
    
    NotificationCenter::getInstance()->postNotification(kMSG_GameLayer_LayerInitComplete);
    
    beforeMatch();
}
void DuplicateLayer::buyFiveSteps(Ref *obj)
{
    _hightLightSwitch = true;
    findPotentialOrRenew();
}

void DuplicateLayer::removeNodeFromParent(Node *pSender)
{
    _hammerSprite->setOpacity(255);
    
    pSender->removeFromParentAndCleanup(true);
    _propkind = PropNone;
}

void DuplicateLayer::resetAnimation()
{
    Vec2 pos = _add5stepSprite->getPosition();
    _add5stepSprite->removeFromParentAndCleanup(true);
    _add5stepSprite = NULL;
    
    _add5stepSprite = Armature::create("jiawu_icon");
    this->addChild(_add5stepSprite);
    _add5stepSprite->setPosition(pos);
    _add5stepSprite->getAnimation()->play("Animation1");
}

void DuplicateLayer::explodeAllGem(Gem *gem)
{
    int i,j;
    for(i=0;i<kMatrixWidth;i++)
    {
        for (j=kMatrixWidth - 1; j>= 0; j--)
        {
            if(_gemMatrix[i][j] && _gemMatrix[i][j]->getGemType() < dragontooth)
            {
                _gemMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0.3), CallFuncN::create(CC_CALLBACK_1(Gem::explodeAll, _gemMatrix[i][j])), NULL));
            }
        }
    }
    
    this->runAction(Sequence::create(DelayTime::create(1.2),CallFunc::create(CC_CALLBACK_0(DuplicateLayer::afterMatch, this)), NULL));
}

void DuplicateLayer::fallDownColumn(Node *sender, int row)
{
    this->runAction(Sequence::create(DelayTime::create(0.8),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::beforeFallDown, this,0 , row)),NULL));
}

void DuplicateLayer::beforeFallDown(Node *sender, int data, int row)
{
    int i;
    
    _fallGemCount=0;
    
    for(i=0;i<_emptyPointVector.size();i++)
    {
        MyPoint p = _emptyPointVector[i];
        
        _gemMatrix[p.x][p.y]=NULL;
    }
    
    
    _emptyPointVector.clear();
    
    _mapLayer->dealFallDownColumn(this, row);
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getActionVectorSize()>0)
            {
                _fallGemCount++;
            }
        }
    }
    
    _isFallDownEnd = false;
    
    _isStartMove = true;
    
    fallDownEndColumn(NULL, data,  row);
    
}

void DuplicateLayer::fallDownEndColumn(Node *sender, int data, int row)
{
    int j = data;
    
    if(_gemMatrix[row][j])
    {
        _gemMatrix[row][j]->fallDownToEnd(true);
    }
    if(j+1<kMatrixWidth)
    {
        this->runAction(Sequence::create(DelayTime::create(0.015),CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::fallDownEndColumn, this,(j+1) , row)),NULL));
    }
    else
    {
        GemAction::getInstance().playEffectMusic(NULL, "dropdown.mp3"); //方案1
        _mapLayer->clearCounter();
    }
}


void DuplicateLayer::afterExplode(Ref *obj)
{
    afterMatch();
}

void DuplicateLayer::displayScore(cocos2d::Ref *obj)
{
    Gem *gem = (Gem*)obj;
    Point pos = gem->getPosition();
    
    MyPoint mp = gem->getCurrentIndex(gem->getPosition());
    if (_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y) && _mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y)->getZOrder() == -1)
    {
        auto arm = Armature::create("jiafen");
        _particleNode->addChild(arm);
        arm->setPosition(pos + Vec2(0, 30));
        arm->getAnimation()->playWithIndex(4);
    }
    else
    {
        this->runAction(Sequence::create( CallFuncN::create(CC_CALLBACK_1(DuplicateLayer::addScore, this,pos,gem->getScore())), NULL));
    }
}

void DuplicateLayer::addScore(Node *sender, Point pos , int animID)
{
    auto arm = Armature::create("jiafen");
    _particleNode->addChild(arm);
    arm->setPosition(pos);
    
    int index = _continueMatchTimes - 1 ;
    if (animID%2 == 0)
    {
        
        index = index < 4 ? index :4;
    }
    else
    {
        index = 5;
    }
    
    if(index <0)index = 0;
    arm->getAnimation()->playWithIndex(index);
    
    arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(DuplicateLayer::removeNodeFromParent, this) );
}
