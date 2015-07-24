#include "Gem.h"
#include "UI/GameUIData.h"
#include <time.h>

MapInfo*									Gem::_mapInfo = NULL;
Node*										Gem::_particleNode = NULL;
Gem***                                      Gem::_gemStoneMatrix=NULL;
int**										Gem::_mapMatrix = NULL;
Node*										Gem::_mapNode = NULL;
vector<AnimationWraper>*                    Gem::_animationWraperVector = NULL;
vector<MyPoint>*                            Gem::_emptyPointVector = NULL;
vector<ConnectionArea>*                     Gem::_connectionAreaVector=NULL;
int											Gem::_specialCount = 0;
vector<Gem*>                                Gem::_vecRemoveGem = {};


Gem::Gem()
:_type((GemType)0)
,_state(0)
,_frozen(0)
,_restrain(0)
,_roots(0)
,_chain(0)
,_change(NoChange)
,_score(5)
,_sick(false)
,_GemKind(NormalGem)
,_frozenSprite(NULL)
,_rootsSprite(NULL)
,_chainSprite(NULL)
,_restrainNode(NULL)
,_skillSprite(NULL)
,_flag(0)
,_scoreSum(0)
,_countLabel(NULL)
,_labelBackGround(NULL)
,_skill(SkillNull)
,_nextskill(SkillNull)
,_autoSkill(false)
,_skillAnimation(false)
,_skAnimaID(0)
,_sameAndSkill(false)
,_endGem(false)
,_removeAlready(false)
{
    _vecLeftGem.clear();
    _vecRightGem.clear();
    _vecUpGem.clear();
    _vecDownGem.clear();
}
Gem::~Gem()
{
    clearAllAction();
    this->removeAllChildrenWithCleanup(true);
    if (_vecActions.size())
    {
        _vecActions.clear();
    }
}

Gem* Gem::createRandomGem(Node* layer,Point point,bool visible)
{
    int index = 0;
    
    GemType fType;
    
    if(GameUIData::getInstance()->getVerticalIndex())
    {
        if (_specialCount == 0 )
        {
            fType = special;
        }
        else
        {
            index = arc4random()%_mapInfo->getGemType().size();
            fType = (GemType)_mapInfo->getGemType().at(index);
        }
    }
    else
    {
        /*srand ((unsigned)time(nullptr));*/
        index = arc4random()%_mapInfo->getGemType().size();
        fType = (GemType)_mapInfo->getGemType().at(index);
    }
    Gem *gem = createFixGem(fType,layer,point,visible);
    return gem;
}

void Gem::setMapInfo(Node *particleNode,Node* mapNode, Gem ***matrix, MapInfo *mapInfo,vector<AnimationWraper>* aw,vector<MyPoint>* pvector,vector<ConnectionArea>* connectVector,int** mapMatrix)
{
    _particleNode = particleNode;
    
    _mapNode = mapNode;
    
    _gemStoneMatrix = matrix;
    
    _mapInfo = mapInfo;
    
    _mapMatrix = mapMatrix;
    
    _animationWraperVector = aw;
    
    _emptyPointVector = pvector;
    
    _connectionAreaVector = connectVector;
    
    _specialCount = 0;
}

void Gem::addCollectGemScore()
{
//    int param = _type*1000+_score;
    
//    NotificationCenter::getInstance()->postNotification(kMSG_UpdateCollecte,(Ref*)param);
}

void Gem::addCollectGemScoreUpdate()
{
//    NotificationCenter::getInstance()->postNotification(kMSG_ShowCollecte,(Ref*)_type);
}

Gem* Gem::createFixGem(GemType type,Node *layer,Point point,bool visible)
{
    Gem *gem = NULL;
    
    switch (type) {
        case empty:
            return NULL;
            break;
        case red:
            gem = new RedGem();
            break;
        case blue:
            gem = new BlueGem();
            break;
        case green:
            gem = new GreenGem();
            break;
        case white:
            gem = new WhiteGem();
            break;
        case yellow:
            gem = new YellowGem();
            break;
        case purple:
            gem = new PurpleGem();
            break;
        case special:
            gem = new Special();
            break;
        case fog:
            gem = new Fog();
            break;
        case ice:
        case ice1:
        case shield:
        case shield1:
        case shield2:
        case shield3:
        case shield4:
        case babydragon:
        case magicscrolls:
        case lifttree:
        case stormwind:
            gem = new Ice(type);
            break;
        case dragontooth:
            gem = new DragonTooth();
            break;
        case pumpkin:
            gem = new Pumpkin();
            break;
        case rod:
            gem = new Rod();
            break;
        case redbottle:
        case redbottle1:
        case redbottle2:
        case redbottle3:
        case yellowbottle:
        case yellowbottle1:
        case yellowbottle2:
        case yellowbottle3:
        case bluebottle:
        case bluebottle1:
        case bluebottle2:
        case bluebottle3:
        case greenbottle:
        case greenbottle1:
        case greenbottle2:
        case greenbottle3:
        case purplebottle:
        case purplebottle1:
        case purplebottle2:
        case purplebottle3:
        case whitebottle:
        case whitebottle1:
        case whitebottle2:
        case whitebottle3:
            gem = new Bottle(type);
            break;
        case wizard:
            gem = new Wizard();
            break;
        default:
            break;
    }
    
    if(gem&&gem->createGemSprite(layer, point,visible))
    {
        gem->autorelease();
        
        return gem;
    }
    CC_SAFE_DELETE(gem);
    
    return NULL;
}
void Gem::removeCollect(const Point& dist,float time,bool playMusic)
{
    Action* flyAction = GemAction::getInstance().getCollectGemAction(this->getPosition(),dist,time);
    
    this->setZOrder(10);
    
    
    this->runAction(Sequence::create(/*CallFunc::create(CC_CALLBACK_0(Gem::explosionParticle, this)),explodeRandomMusic,*/CallFunc::create(CC_CALLBACK_0(Gem::addCollectGemScore, this)),flyAction,CallFunc::create(CC_CALLBACK_0(Gem::addCollectGem, this)),CallFunc::create(CC_CALLBACK_0(Gem::hideSprite, this)),CallFunc::create(CC_CALLBACK_0(Gem::removeGem, this)),NULL));
}

void Gem::removeNoCollect(bool playMusic)
{
    MyPoint myPoint = getCurrentIndex(this->getPosition());
    int i = myPoint.x,j=myPoint.y;
    
    if (i < kMatrixWidth && j < kMatrixWidth)
    {
        if(i-1>=0&&_gemStoneMatrix[i-1][j])
        {
            _gemStoneMatrix[i-1][j]->changeState(0);
        }
        if(i+1<kMatrixWidth&&_gemStoneMatrix[i+1][j])
        {
            _gemStoneMatrix[i+1][j]->changeState(0);
        }
        if(j-1>=0&&_gemStoneMatrix[i][j-1])
        {
            _gemStoneMatrix[i][j-1]->changeState(0);
        }
        if(j+1<kMatrixWidth&&_gemStoneMatrix[i][j+1])
        {
            _gemStoneMatrix[i][j+1]->changeState(0);
        }
    }
    
    
    if(_frozen>0||_restrain>0||_roots>0||_chain>0)
    {
        if(_frozen>0)
        {
            _frozen--;
            frozenOff();
        }
        else if(_restrain>0)
        {
            _restrain--;
            restrainOff();
        }
        else if(_roots>0)
        {
            _roots--;
            rootsOff();
        }
        else if(_chain>0)
        {
            _chain--;
            chainOff();
        }
        
        return;
    }
    
    
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    if (_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y))
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _mapNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        auto spr = (Sprite*)_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y);
        int iFlag = spr->getLocalZOrder();
        if (iFlag == 1)
        {
            spr->setZOrder(-1);
            NotificationCenter::getInstance()->postNotification("display_score",this);
            
            _mapNode->removeChildByTag(mp.x * kMatrixWidth + mp.y , true);
            
            int msg = 1000*10;
            __String *str = __String::createWithFormat("%d",msg);
            NotificationCenter::getInstance()->postNotification(kMSG_ShowCollecte,str );
        }
        if(iFlag == 2)
        {
            spr->setTexture("icefloor.png");
            spr->setLocalZOrder(1);
        }
    }
    
    if (_state == -1 || (_type == all && _state == all))
    {
        _removeAlready = true;
        this->runAction(Sequence::create(CallFuncN::create(CC_CALLBACK_1(Gem::playExplodeRandomMusic, this)),CallFunc::create(CC_CALLBACK_0(Gem::explosionParticle, this)),CallFunc::create(CC_CALLBACK_0(Gem::addCollectGem, this)),CallFunc::create(CC_CALLBACK_0(Gem::removeSkillSpr, this)),CallFunc::create(CC_CALLBACK_0(Gem::hideSprite, this)),CallFunc::create(CC_CALLBACK_0(Gem::removeGem, this)),NULL));
    }
}

void Gem::createSkill()
{
    //直接形成。。。
//    for(int i=0;i<kMatrixWidth;i++)
//    {
//        for (int j=0;j<kMatrixWidth; j++)
//        {
//            if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
//            {
//                _gemStoneMatrix[i][j]->setSkillSpr();
//                
//            }
//        }
//    }
    
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    for( it =_connectionAreaVector->begin();it!=_connectionAreaVector->end();it++)
    {
        j = it->startPoint.y;
        i = it->startPoint.x;
        
        if (it->direction == Up)
        {
            if (it->isInCross)
            {
                for ( ;j < it->startPoint.y + it->count; j++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove() && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
                        
                    
                        _gemStoneMatrix[i][j]->beforeExplode();
                    
                        continue;
                    }
                    
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }


                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
                continue;
            }
            if (it->count == 4)
            {
                for ( ;j < it->startPoint.y + it->count; j++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
                        
            
                        _gemStoneMatrix[i][j]->beforeExplode();
                
                        
                        continue;
                    }
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }

                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
            }
            if (it->count >= 5)
            {
                for ( ;j < it->startPoint.y + it->count; j++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
                        
                        _gemStoneMatrix[i][j]->beforeExplode();
                   
                        
                        continue;
                    }
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }

                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
            }
        }
        else
        {
            
            if (it->isInCross)
            {
                for ( ;i < it->startPoint.x + it->count; i++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
//                        if (_gemStoneMatrix[i][j]->getGemSkill())
//                        {
                            _gemStoneMatrix[i][j]->beforeExplode();
//                        }
                        continue;
                    }
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }

                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
                continue;
            }
            if (it->count == 4)
            {
                for ( ;i < it->startPoint.x + it->count; i++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
                        
                        _gemStoneMatrix[i][j]->beforeExplode();
                        
                        continue;
                    }
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }

                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
            }
            if (it->count >= 5)
            {
                for ( ;i < it->startPoint.x + it->count; i++)
                {
                    if (_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->canMove()  && _gemStoneMatrix[i][j]->getNextSkill())
                    {
                        _gemStoneMatrix[i][j]->setSkillSpr();
                        _gemStoneMatrix[i][j]->setZOrder(2);
                        
                        _gemStoneMatrix[i][j]->beforeExplode();
                    
                        continue;
                    }
                    if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                    {
                        _gemStoneMatrix[i][j]->beforeExplode();
                        continue;
                    }

                    _gemStoneMatrix[i][j]->gemMoveCenter(Vec2(it->centerPoint.x * kElementSize + kStartX + kElementSize/2, it->centerPoint.y * kElementSize + kStartY + kElementSize/2));
                }
            }
            
        }
    }
    
    
    this->runAction(Sequence::create(DelayTime::create(0),CallFunc::create(CC_CALLBACK_0(Gem::matchAll, this)), NULL));
}

void Gem::gemMoveCenter(Point pos)
{
    if (canMove())
    {
        pos = pos - this->getPosition() ;
        _state = -1;
        _spr->runAction(Sequence::create(MoveTo::create(0.25, pos),CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, _spr)),NULL));
        gemMoveOver();
    }
    else
    {
        beforeExplode();
    }
}

void Gem::gemMoveOver()
{
    NotificationCenter::getInstance()->postNotification("display_score",this);
    
    MyPoint myPoint = getCurrentIndex(this->getPosition());
    int i = myPoint.x,j=myPoint.y;
    
    
    if (i < kMatrixWidth && j < kMatrixWidth)
    {
        if(i-1>=0&&_gemStoneMatrix[i-1][j])
        {
            _gemStoneMatrix[i-1][j]->changeState(0);
        }
        if(i+1<kMatrixWidth&&_gemStoneMatrix[i+1][j])
        {
            _gemStoneMatrix[i+1][j]->changeState(0);
        }
        if(j-1>=0&&_gemStoneMatrix[i][j-1])
        {
            _gemStoneMatrix[i][j-1]->changeState(0);
        }
        if(j+1<kMatrixWidth&&_gemStoneMatrix[i][j+1])
        {
            _gemStoneMatrix[i][j+1]->changeState(0);
        }
    }
    
    
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    if (_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y))
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _mapNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        auto spr = (Sprite*)_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y);
        int iFlag = spr->getLocalZOrder();
        if (iFlag == 1)
        {
            spr->setZOrder(-1);
            NotificationCenter::getInstance()->postNotification("display_score",this);
            
            _mapNode->removeChildByTag(mp.x * kMatrixWidth + mp.y , true);
            
            int msg = 1000*10;
            __String *str = __String::createWithFormat("%d",msg);
            NotificationCenter::getInstance()->postNotification(kMSG_ShowCollecte,str );
        }
        if(iFlag == 2)
        {
            spr->setTexture("icefloor.png");
            spr->setLocalZOrder(1);
        }
    }

    
    this->runAction(Sequence::create(DelayTime::create(0.26),CallFunc::create(CC_CALLBACK_0(Gem::removeGem, this)),NULL));
//    removeNoCollect(false);
}

void Gem::triggerSkill()
{
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    
    bool trigger = false;
    
    for( it =_connectionAreaVector->begin();it!=_connectionAreaVector->end();it++)
    {
        j = it->startPoint.y;
        i = it->startPoint.x;
        
        if (it->direction == Up)
        {
            for ( ;j < it->startPoint.y + it->count; j++)
            {
                if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                {
                    trigger = true;
                    _gemStoneMatrix[i][j]->beforeExplode();
                }
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
                if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemSkill() && (_gemStoneMatrix[i][j]->getState() == -1 || _gemStoneMatrix[i][j]->getNextSkill()))
                {
                    trigger = true;
                    _gemStoneMatrix[i][j]->beforeExplode();
                }
                
            }
        }
    }

    
    if (!trigger)
    {
        matchAll();
    }
}

void Gem::matchAll()
{
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    for( it =_connectionAreaVector->begin();it!=_connectionAreaVector->end();it++)
    {
        j = it->startPoint.y;
        i = it->startPoint.x;
        
        if ( (it->isInCross || it->count > 3) && GameUIData::getInstance()->getVerticalIndex() == 0 )
        {
            continue;
        }
        
        if (it->direction == Up)
        {
            for ( ;j < it->startPoint.y + it->count; j++)
            {
                if (_gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getRemoveAlready())
                {
                    _gemStoneMatrix[i][j]->removeNoCollect(false);
                }
                
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
                if (_gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getRemoveAlready())
                {
                    _gemStoneMatrix[i][j]->removeNoCollect(false);
                }
            }
        }
    }

}

void Gem::beforeExplode()
{
    if (_skill)
    {
        removeSkillSpr();
        dealWithSkill();
        dealWithGem();
    }
    else
    {
        this->removeNoCollect(false);
    }
}

void Gem::dealWithSkill()
{
    GemAction::getInstance().playEffectMusic(NULL,"bigboom.mp3");
    if (_type == all)
    {
        MyPoint allPos = getCurrentIndex(this->getPosition());
        _gemStoneMatrix[allPos.x][allPos.y]->setState(all);
        _gemStoneMatrix[allPos.x][allPos.y]->removeNoCollect(false);
        _gemStoneMatrix[allPos.x][allPos.y] = NULL;

        
        GemType fType = empty;
        int i,j,max=0;
        while(fType == empty)
        {
            do
            {
                i = arc4random()%kMatrixWidth;
                
                j = arc4random()%kMatrixWidth;
                
            }while(!_gemStoneMatrix[i][j]);
            
            if(_gemStoneMatrix[i][j] && _gemStoneMatrix[i][j]->getGemType() < all)
            {
                fType = _gemStoneMatrix[i][j]->getGemType();
            }
        }
        
        Point startPoint = this->getPosition();
        Sprite* spriteArray[kMatrixWidth*kMatrixWidth]={0};
        MyPoint distArray[kMatrixWidth*kMatrixWidth];
        
        for(i=0;i<kMatrixWidth;i++)
        {
            for (j=0;j<kMatrixWidth; j++)
            {
                if(_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getGemType()==fType && _gemStoneMatrix[i][j]->getState() != -1)
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
            Gem* distGem =_gemStoneMatrix[distArray[i].x][distArray[i].y];
//            distGem->setState(-1);
            Sequence* seq=NULL;
            ccBezierConfig config;
            Point controlPoint = GemAction::getInstance().getControlPoint(startPoint, distGem->getPosition(),(Director::getInstance()->getWinSize().width)*kParticleBezierScale);
            config.controlPoint_1 = controlPoint;
            config.controlPoint_2 = controlPoint;
            config.endPosition = distGem->getPosition();
            
            if(i==max-1)
            {
                CallFuncN* callback = CallFuncN::create(CC_CALLBACK_1(Gem::removeSameGem, distGem,fType));
                seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),callback,NULL);
            }
            else
            {
                seq = Sequence::create(Spawn::create(RotateTo::create(kSparkleFlyTime, 60),BezierTo::create(kSparkleFlyTime, config),NULL),CallFuncN::create(CC_CALLBACK_1(Gem::fiveMatchEffect, distGem)),NULL);
            }
            
            spriteArray[i]->runAction(seq);
        }
        
        return ;
    }
    
    __String *str = __String::create("");
    switch (_skill)
    {
        case SkillHorizontal:
        case SkillVerticl:
        case SkillCross:
        case SkillThreeCross:
        case SkillLeft:
        case SkillRight:
        case SkillTen:
            str = __String::create("hengxiao_effect");
            break;
        case SkillAround2:
        case SkillAround1:
        case SkillAround4:
        case SkillAround3:
        
        case SkillTurnTen:
            str = __String::create("jiugong03_effect");
            break;
        default:
            break;
    }
    Armature *arm = Armature::create(str->getCString());
    arm->getAnimation()->playWithIndex(0);
    _particleNode->addChild(arm);
    if (_skill == SkillVerticl)
    {
        arm->setRotation(90);
    }
    if (_skill == SkillLeft)
    {
        arm->setRotation(45);
    }
    if (_skill == SkillRight)
    {
        arm->setRotation(135);
    }
    if (_skill == SkillCross || _skill == SkillTen)
    {
        Armature *arm1 = Armature::create("hengxiao_effect");
        arm1->getAnimation()->playWithIndex(0);
        _particleNode->addChild(arm1);
        arm1->setRotation(90);
        arm1->setPosition(this->getPosition());
        
        arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
    }
    
    if (_skill == SkillThreeCross)
    {
        
        Armature *arm1 = Armature::create("hengxiao_effect");
        arm1->getAnimation()->playWithIndex(0);
        _particleNode->addChild(arm1);
        arm1->setRotation(90);
        arm1->setPosition(this->getPosition());
        
        arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        
        MyPoint mp = getCurrentIndex(this->getPosition());
        
        int x = mp.x;
        int y = mp.y;
        
        if (y - 1 >=0 && _gemStoneMatrix[x][y-1])
        {
            Armature *arm1 = Armature::create("hengxiao_effect");
            arm1->getAnimation()->playWithIndex(0);
            _particleNode->addChild(arm1);
            arm1->setPosition(_gemStoneMatrix[x][y-1]->getPosition());
            
            arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        }
        if (y + 1 < kMatrixWidth && _gemStoneMatrix[x][y+1])
        {
            Armature *arm1 = Armature::create("hengxiao_effect");
            arm1->getAnimation()->playWithIndex(0);
            _particleNode->addChild(arm1);
            arm1->setPosition(_gemStoneMatrix[x][y+1]->getPosition());
            
            arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        }
        if (x - 1 >= 0 && _gemStoneMatrix[x-1][y])
        {
            Armature *arm1 = Armature::create("hengxiao_effect");
            arm1->getAnimation()->playWithIndex(0);
            _particleNode->addChild(arm1);
            arm1->setPosition(_gemStoneMatrix[x-1][y]->getPosition());
            arm1->setRotation(90);
            arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        }
        if (x + 1 < kMatrixWidth && _gemStoneMatrix[x+1][y])
        {
            Armature *arm1 = Armature::create("hengxiao_effect");
            arm1->getAnimation()->playWithIndex(0);
            _particleNode->addChild(arm1);
            arm1->setPosition(_gemStoneMatrix[x+1][y]->getPosition());
            arm1->setRotation(90);
            arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        }
        
    }
    
    arm->setPosition(this->getPosition());
    setSkAnimaByID(0);
    
    if ((_skill >= SkillAround1 && _skill <= SkillAround4) || _skill == SkillTurnTen )
    {
        _spr->setOpacity(150);
        arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::aroundAnimation, this));;
    }
    else
    {
        arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
        arm->setTag(888);
        this->runAction(Sequence::create(/*DelayTime::create(0.2),*/CallFuncN::create(CC_CALLBACK_1(Gem::gemBright, this)), NULL));
    }
    
    if (_skill == SkillAround1 || _skill == SkillAround3)
    {
        this->setState(1);
    }
}

void Gem::dealWithGem()
{
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    float time = 0.4;
    
    if (_skill == SkillHorizontal || _skill == SkillCross)
    {
        for (int i = 1; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y] && !_gemStoneMatrix[mp.x - i][mp.y]->getNextSkill() && _gemStoneMatrix[mp.x - i][mp.y]->getState() != -1 && _gemStoneMatrix[mp.x - i][mp.y]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x - i][mp.y]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x - i][mp.y]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x - i][mp.y]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x - i][mp.y]->getGemSkill() /*&& !_gemStoneMatrix[mp.x - i][mp.y]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[mp.x - i][mp.y]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x - i][mp.y])), NULL));
                    }
                }
            }
            
            if (mp.x + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y] && !_gemStoneMatrix[mp.x + i][mp.y]->getNextSkill()  && _gemStoneMatrix[mp.x + i][mp.y]->getState() != -1 && _gemStoneMatrix[mp.x + i][mp.y]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x + i][mp.y]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x + i][mp.y]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x + i][mp.y]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x + i][mp.y]->getGemSkill() /*&& !_gemStoneMatrix[mp.x + i][mp.y]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[mp.x + i][mp.y]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x + i][mp.y])), NULL));
                    }
                }
            }
        }
        
    }
    
    if (_skill == SkillVerticl || _skill == SkillCross)
    {
        for (int i = 1; i < kMatrixWidth; i++)
        {
            if (mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x][mp.y - i] && !_gemStoneMatrix[mp.x][mp.y - i]->getNextSkill() && _gemStoneMatrix[mp.x][mp.y - i]->getState() != -1 && _gemStoneMatrix[mp.x][mp.y - i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x][mp.y - i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x][mp.y - i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x][mp.y - i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x][mp.y - i]->getGemSkill() /*&& !_gemStoneMatrix[mp.x][mp.y - i]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[mp.x][mp.y - i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x][mp.y - i])), NULL));
                    }
                }
            }
            
            if (mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x][mp.y + i] && !_gemStoneMatrix[mp.x][mp.y + i]->getNextSkill() && _gemStoneMatrix[mp.x][mp.y + i]->getState() != -1 && _gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x][mp.y + i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x][mp.y + i]->explode(mp, mp, 0, 0);
                    
                    if (_gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() /*&& !_gemStoneMatrix[mp.x][mp.y + i]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[mp.x][mp.y + i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x][mp.y + i])), NULL));
                    }
                }
            }
        }
    }
    if (_skill == SkillAround2 || _skill == SkillAround1)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-1)&&(i <= mp.x+1)&&(j >= mp.y-1)&&(j <= mp.y+1)&&_gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getNextSkill() && _gemStoneMatrix[i][j]->getState() != -1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround3 && _gemStoneMatrix[i][j]->getGemType() < dragontooth)
                {
                    
                    _gemStoneMatrix[i][j]->explode(mp, mp, 0, 0);
                    
                    if (_gemStoneMatrix[i][j]->getGemSkill() /*&& !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[i][j])), NULL));
                    }
                }
            }
        }
        if (_skill == SkillAround2 && !_sameAndSkill)
        {
            this->setState(0);
            this->setGemSkill(SkillAround1);
        }
    }
    
    if (_skill == SkillThreeCross)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if (( ((i >= mp.x-1) && (i <= mp.x+1)) || ((j >= mp.y-1) && (j <= mp.y+1)) ) &&_gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getNextSkill() && _gemStoneMatrix[i][j]->getState() != -1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround3 && _gemStoneMatrix[i][j]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[i][j]->explode(mp, mp, 0, 0);;
                    
                    if (_gemStoneMatrix[i][j]->getGemSkill() /*&& !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[i][j])), NULL));
                    }
                }
            }
        }
    }
    
    if (_skill == SkillAround4 || _skill == SkillAround3)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&& _gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getNextSkill() && _gemStoneMatrix[i][j]->getState() != -1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround3 && _gemStoneMatrix[i][j]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[i][j]->explode(mp, mp, 0, 0);
                    
                    if (_gemStoneMatrix[i][j]->getGemSkill() /*&& !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                    {
                        _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[i][j])), NULL));
                    }
                }
            }
        }
        if (_skill == SkillAround4)
        {
            this->setState(0);
            this->setGemSkill(SkillAround3);
        }
    }
    
    if (_skill == SkillLeft)
    {
        for (int i = 1; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0 && mp.y + i < kMatrixWidth)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y + i] && !_gemStoneMatrix[mp.x - i][mp.y + i]->getNextSkill() && _gemStoneMatrix[mp.x - i][mp.y + i]->getState() != -1 && _gemStoneMatrix[mp.x - i][mp.y + i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x - i][mp.y + i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x - i][mp.y + i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x - i][mp.y + i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x - i][mp.y + i]->getGemSkill() )
                    {
                        _gemStoneMatrix[mp.x - i][mp.y + i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x - i][mp.y + i])), NULL));
                    }
                }
            }
            
            if (mp.x + i < kMatrixWidth && mp.y - i >= 0)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y - i] && !_gemStoneMatrix[mp.x + i][mp.y - i]->getNextSkill()  && _gemStoneMatrix[mp.x + i][mp.y - i]->getState() != -1 && _gemStoneMatrix[mp.x + i][mp.y - i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x + i][mp.y - i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x + i][mp.y - i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x + i][mp.y - i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x + i][mp.y - i]->getGemSkill())
                    {
                        _gemStoneMatrix[mp.x + i][mp.y - i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x + i][mp.y - i])), NULL));
                    }
                }
            }
        }
        
    }

    if (_skill == SkillRight)
    {
        for (int i = 1; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0 && mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y - i] && !_gemStoneMatrix[mp.x - i][mp.y - i]->getNextSkill() && _gemStoneMatrix[mp.x - i][mp.y - i]->getState() != -1 && _gemStoneMatrix[mp.x - i][mp.y - i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x - i][mp.y - i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x - i][mp.y - i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x - i][mp.y - i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x - i][mp.y - i]->getGemSkill() )
                    {
                        _gemStoneMatrix[mp.x - i][mp.y - i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x - i][mp.y - i])), NULL));
                    }
                }
            }
            
            if (mp.x + i < kMatrixWidth && mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y + i] && !_gemStoneMatrix[mp.x + i][mp.y + i]->getNextSkill()  && _gemStoneMatrix[mp.x + i][mp.y + i]->getState() != -1 && _gemStoneMatrix[mp.x + i][mp.y + i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x + i][mp.y + i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x + i][mp.y + i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x + i][mp.y + i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x + i][mp.y + i]->getGemSkill())
                    {
                        _gemStoneMatrix[mp.x + i][mp.y + i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x + i][mp.y + i])), NULL));
                    }
                }
            }
        }
    }
    
    if (_skill == SkillTen)
    {
        for (int i = 1; i < 3; i++)
        {
            if (mp.x - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y] && !_gemStoneMatrix[mp.x - i][mp.y]->getNextSkill() && _gemStoneMatrix[mp.x - i][mp.y]->getState() != -1 && _gemStoneMatrix[mp.x - i][mp.y]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x - i][mp.y]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x - i][mp.y]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x - i][mp.y]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x - i][mp.y]->getGemSkill())
                    {
                        _gemStoneMatrix[mp.x - i][mp.y]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x - i][mp.y])), NULL));
                    }
                }
            }
            
            if (mp.x + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y] && !_gemStoneMatrix[mp.x + i][mp.y]->getNextSkill()  && _gemStoneMatrix[mp.x + i][mp.y]->getState() != -1 && _gemStoneMatrix[mp.x + i][mp.y]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x + i][mp.y]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x + i][mp.y]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x + i][mp.y]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x + i][mp.y]->getGemSkill())
                    {
                        _gemStoneMatrix[mp.x + i][mp.y]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x + i][mp.y])), NULL));
                    }
                }
            }
        }
        
        for (int i = 1; i < 3; i++)
        {
            if (mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x][mp.y - i] && !_gemStoneMatrix[mp.x][mp.y - i]->getNextSkill() && _gemStoneMatrix[mp.x][mp.y - i]->getState() != -1 && _gemStoneMatrix[mp.x][mp.y - i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x][mp.y - i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x][mp.y - i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x][mp.y - i]->explode(mp, mp, 0, 0);
                    if (_gemStoneMatrix[mp.x][mp.y - i]->getGemSkill())
                    {
                        _gemStoneMatrix[mp.x][mp.y - i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x][mp.y - i])), NULL));
                    }
                }
            }
            if (mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x][mp.y + i] && !_gemStoneMatrix[mp.x][mp.y + i]->getNextSkill() && _gemStoneMatrix[mp.x][mp.y + i]->getState() != -1 && _gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() != SkillAround1 && _gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() != SkillAround3 && _gemStoneMatrix[mp.x][mp.y + i]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[mp.x][mp.y + i]->explode(mp, mp, 0, 0);
                    
                    if (_gemStoneMatrix[mp.x][mp.y + i]->getGemSkill() )
                    {
                        _gemStoneMatrix[mp.x][mp.y + i]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[mp.x][mp.y + i])), NULL));
                    }
                }
            }
        }
    }
    

    if (_skill == SkillTurnTen)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if (i == mp.x || j == mp.y)
                {
                    continue;
                }
                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&& _gemStoneMatrix[i][j] && !_gemStoneMatrix[i][j]->getNextSkill() && _gemStoneMatrix[i][j]->getState() != -1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround1 && _gemStoneMatrix[i][j]->getGemSkill() != SkillAround3 && _gemStoneMatrix[i][j]->getGemType() < dragontooth)
                {
                    _gemStoneMatrix[i][j]->explode(mp, mp, 0, 0);
                    
                    if (_gemStoneMatrix[i][j]->getGemSkill() )
                    {
                        _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(Gem::nextSkill, _gemStoneMatrix[i][j])), NULL));
                    }
                }
            }
        }
    }


}

void Gem::nextSkill(Node *pSender)
{
    Gem* gem = (Gem*)pSender;
    gem->beforeExplode();

}

void Gem::addCollectGem()
{
//    map<GemType, int> map_Target = _mapInfo->getMapTarget();
//    map<GemType, int>::iterator iter;
//    iter = map_Target.find(_type);
//    if (iter != map_Target.end())
//    {
        int msg = 1000 * _score + _type;
        __String *str = __String::createWithFormat("%d",msg);
        NotificationCenter::getInstance()->postNotification(kMSG_ShowCollecte,str );
//    }
}

void Gem::removeGem()
{
    _state = 0;
    int count = 0;
    for(int i=0;i<kMatrixWidth;i++)
    {
        for (int j=0;j<kMatrixWidth; j++)
        {
            if (_gemStoneMatrix[i][j]&&_gemStoneMatrix[i][j]->getState() == -1)
            {
                count++;
            }
        }
    }
    
    CCLOG("dingdong === %d",count);
    
    MyPoint mp = getCurrentIndex(this->getPosition());
    _gemStoneMatrix[mp.x][mp.y] = NULL;
    
    //   想办法 解决
//    this->clearAllAction();
//    
//    this->removeFromParentAndCleanup(true);
    
//    if(_type == all)
//    {
//        this->clearAllAction();
//        this->removeFromParentAndCleanup(true);
//    }
//    else
//    {
        vector<Gem*> ::iterator result = find(_vecRemoveGem.begin(), _vecRemoveGem.end(), this);
        if (result == _vecRemoveGem.end())
        {
            _vecRemoveGem.push_back(this);
        }
//    }
    
    if (count == 0 && _type != all)
    {
        afterExplode();
        CCLOG("vecRemoveGem = %lu" ,_vecRemoveGem.size());
        for (int i = 0; i < _vecRemoveGem.size(); i++)
        {
            Gem *gem = (Gem*)_vecRemoveGem.at(i);
            if (gem)
            {
                gem->removeFromParentAndCleanup(true);
                gem = NULL;
            }
            
        }
        _vecRemoveGem.clear();
        CCLOG("vecRemoveGem = %lu" ,_vecRemoveGem.size());
    }
}

void Gem::createGemSpriteByName(Node *layer,Point point,bool visible,const char* name)
{
    _spr = Sprite::create(name);
    
    
    this->addChild(_spr);
    
    this->setPosition(point);
    
    this->setVisible(visible);
    
    layer->addChild(this);
    
    _reminderSprite = Armature::create("effect_xuanzhong");
    this->addChild(_reminderSprite);
    
    _reminderSprite->getAnimation()->playWithIndex(0);
    
    _reminderSprite->setVisible(false);
    
    
    _rangeSprite = Sprite::create("effect_range01.png");
    this->addChild(_rangeSprite);
    
    auto animation2 = Animation::create();
    
    for (int i = 1; i < 6; i++)
    {
        __String *file = __String::createWithFormat("effect_range0%d.png",i);
        
        auto sprite = Sprite::create(file->getCString());
        animation2->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    animation2->setDelayPerUnit(0.05);
    animation2->setRestoreOriginalFrame(true);
    auto action2 = Animate::create(animation2);
    _rangeSprite->runAction(RepeatForever::create(action2));
    _rangeSprite->setVisible(false);
    
    if(_GemKind==SpecialGem)
    {
        return;
    }
    
    _skillSprite = Sprite::create("item_heng01.png");
    this->addChild(_skillSprite,1);
    _skillSprite->setVisible(false);
    
    _starSprite = Sprite::create("star_effect.png");
    _spr->addChild(_starSprite);
    _starSprite->setVisible(false);
    
    addScore(0,false);
}

Sprite* Gem::createNodeFromFile(Node *father,Point point,const char* name)
{
    Sprite *spr = Sprite::create(name);
    spr->setPosition(point);
    father->addChild(spr);
    return spr;
}
void Gem::addFrozen(int frozen)
{
    if(_frozen==0)
    {
        if (frozen == 1)
        {
            _frozenSprite = Gem::createNodeFromFile(this, Point(0,0) ,"frozen.png");
        }
        if (frozen == 2)
        {
            _frozenSprite = Gem::createNodeFromFile(this, Point(0,0) ,"frozen1.png");
        }
        _frozen = frozen;
    }
}

void Gem::addRoots(int roots)
{
    if(_roots==0)
    {
        if (roots == 1)
        {
            _rootsSprite = Gem::createNodeFromFile(this, Point(0,0) ,"roots.png");
        }
        if (roots == 2)
        {
            _rootsSprite = Gem::createNodeFromFile(this, Point(0,0) ,"roots1.png");
        }
        _roots = roots;
    }
}

void Gem::addChain(int chain)
{
    if(_chain==0)
    {
        if (chain == 1)
        {
            _chainSprite = Gem::createNodeFromFile(this, Point(0,0) ,"chain.png");
        }
        if (chain == 2)
        {
            _chainSprite = Gem::createNodeFromFile(this, Point(0,0) ,"chain1.png");
        }
        _chain = chain;
    }
}

void Gem::addRestrain()
{
    if(_restrain==0)
    {
        
        _restrain = 1;
    }
}
void Gem::addChange(int count)
{
    if(_change!=Eliminate)
    {
        _change = (ChangeState)(_change+count);
    }
}

void Gem::setGemType()
{
    if (_skill == SkillAllSame)
    {
        _skillSprite->setVisible(false);
        setAutoSkill(false);
        _type = all;
    }
}

void Gem::setNextSkill(GemSkill sk)
{
    if (_skill != SkillAround1 && _skill != SkillAround3)
    {
        _nextskill = sk;
    }
    else
    {
        _nextskill = SkillNull;
    }
}

GemType Gem::getGemType()
{
    return _type;
}

GemSkill Gem::getGemSkill()
{
    return _skill;
}

void Gem::setGemSkill(GemSkill sk)
{
    _skill = sk;

    if (!_skill || _skill == SkillAround1)
    {
        _skillSprite->setVisible(false);
    }
}

void Gem::setAutoSkill(bool b)
{
    _spr->setOpacity(200);
    _autoSkill = b;
    _skillSprite->setVisible(false);
}

bool Gem::getAutoSkill()
{
    return _autoSkill;
}

void Gem::displaySkill(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    
    auto animation = Animation::create();
    __String *str = __String::create("");
    switch (_nextskill/*_skill*/)
    {
        case SkillHorizontal:
        case SkillVerticl:
        case SkillLeft:
        case SkillRight:
        {
            str = __String::create("item_heng01.png");
            for (int i = 1; i < 8; i++)
            {
                __String *file = __String::createWithFormat("item_heng0%d.png",i);
                
                auto sprite = Sprite::create(file->getCString());
                animation->addSpriteFrame(sprite->getSpriteFrame());
            }
            animation->setDelayPerUnit(0.2);
            animation->setRestoreOriginalFrame(true);
        }
            break;
        case SkillAround2:
        {
            str = __String::create("item_jiugong01.png");
            for (int i = 1; i < 8; i++)
            {
                __String *file = __String::createWithFormat("item_jiugong0%d.png",i);
                
                auto sprite = Sprite::create(file->getCString());
                animation->addSpriteFrame(sprite->getSpriteFrame());
            }
            animation->setDelayPerUnit(0.2);
            animation->setRestoreOriginalFrame(true);
        }
            break;
        case SkillTen:
        {
            str = __String::create("item_shizi01.png");
            for (int i = 1; i < 8; i++)
            {
                __String *file = __String::createWithFormat("item_shizi0%d.png",i);
                
                auto sprite = Sprite::create(file->getCString());
                animation->addSpriteFrame(sprite->getSpriteFrame());
            }
            animation->setDelayPerUnit(0.2);
            animation->setRestoreOriginalFrame(true);
        }
            break;
        case SkillTurnTen:
        {
            str = __String::create("item_fanshizi01.png");
            for (int i = 1; i < 8; i++)
            {
                __String *file = __String::createWithFormat("item_fanshizi0%d.png",i);
                
                auto sprite = Sprite::create(file->getCString());
                animation->addSpriteFrame(sprite->getSpriteFrame());
            }
            animation->setDelayPerUnit(0.2);
            animation->setRestoreOriginalFrame(true);
        }
            break;

        default:
            break;
    }
    
    if (/*_skill*/_nextskill == SkillAllSame)
    {
        _spr->setVisible(false);
        Armature *armature1 = Armature::create( "tongsexiao");
        this->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
    }
    else
    {
        _skillSprite->stopAllActions();
        auto action = Animate::create(animation);
        _skillSprite->setVisible(true);
        _skillSprite->setTexture(str->getCString());
        _skillSprite->setRotation(0);
        if (_nextskill/*_skill*/ == SkillVerticl)
        {
            _skillSprite->setRotation(90);
        }
        if (_nextskill/*_skill*/ == SkillLeft)
        {
            _skillSprite->setRotation(45);
        }

        if (_nextskill/*_skill*/ == SkillRight)
        {
            _skillSprite->setRotation(135);
        }
        
        if (_nextskill/*_skill*/ == SkillTurnTen)
        {
            _skillSprite->setRotation(90);
        }


        _skillSprite->runAction(RepeatForever::create(action));
        _spr->setZOrder(2);
    }
}

void Gem::sameDisplay(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    setSkillSpr();
}

void Gem::setSkillSpr()
{
    Armature *armature1 = Armature::create( "jvqi_effect");
    this->addChild(armature1,-1);
    armature1->getAnimation()->playWithIndex(0);
    armature1->setPosition(Vec2::ZERO);
    armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::displaySkill, this) );
}

void Gem::removeSkillSpr()
{
    if (_skillSprite && !_nextskill)
    {
        _skillSprite->setVisible(false);
    }
}

void Gem::showSkillSpr()
{
    if (_skill && _skillSprite  )
    {
        _skillSprite->setVisible(true);
    }
}

bool Gem::canMove()
{
    if (_frozen==0&&_restrain==0&&_chain==0&&_roots==0)
    {
        return true;
    }
    return false;
}

void Gem::showRange()
{
    _rangeSprite->setVisible(true);
}

void Gem::hideRange()
{
    _rangeSprite->setVisible(false);
}

void Gem::showSelf()
{
    this->setVisible(true);
}

void Gem::hideSelf()
{
    this->setVisible(false);
}


void Gem::highLight()
{
    this->setZOrder(1);
    _reminderSprite->setVisible(true);
}
void Gem::stopHighLight()
{
    this->setZOrder(0);
    _reminderSprite->setVisible(false);

}
void Gem::idle()
{
    if(_sick||_frozen>0) return;
    
    if(_spr->getNumberOfRunningActions()>0||this->getNumberOfRunningActions()>0) return;
    
    if (_type >= all) return;
    
    float fX = 70 * CCRANDOM_0_1();
    float fY = 70 * CCRANDOM_0_1();
    
    Image *image = new Image();
    
    switch (_type)
    {
        case red:
            image->initWithImageFile("red.png");
            break;
        case yellow:
            image->initWithImageFile("yellow.png");
            break;
        case blue:
            image->initWithImageFile("blue.png");
            break;
        case green:
            image->initWithImageFile("green.png");
            break;
        case purple:
            image->initWithImageFile("purple.png");
            break;
        case white:
            image->initWithImageFile("white.png");
            break;
        default:
            break;
    }
    unsigned char *data = image->getData();
    
    if (data)
    {
        int pa = 4 * ((image->getHeight() - (int)(fX) - 1) * image->getWidth() + (int)(fY)) + 3;
        unsigned int ap = data[pa];
        
        if (ap > 200)
        {
            _starSprite->setVisible(true);
            _starSprite->setPosition(Vec2(fX, fY));
            
            
            _starSprite->runAction(Sequence::create(Spawn::create(Sequence::create(ScaleTo::create(0.8, 1.4),ScaleTo::create(0.8, 0), NULL),RotateBy::create(1.6, 80), NULL),CallFunc::create(CC_CALLBACK_0(Gem::zhayan, this)),NULL));
        }
    }
    
    
    delete image;

}
void Gem::selected()
{
    if(_frozen>0||_restrain>0||_chain>0||_roots>0)
    {
        return;
    }

    _reminderSprite->setVisible(true);
}

void Gem::unselected()
{
    if(_frozen>0||_restrain>0||_chain>0||_roots>0)
    {
        return;
    }
    _reminderSprite->setVisible(false);
    
    
}

void Gem::zhayan()
{
    _starSprite->setScale(1);
    _starSprite->setVisible(false);
    _starSprite->setPosition(Vec2(0, 0));
}
void Gem::becomeSick()
{
}
void Gem::becomeWell()
{
}
void Gem::fallDownStart()
{
//    zhayan();
    
    if(_sick) return ;
    
//    _spr->setScaleX(0.7);
}
void Gem::fallDownEnd(Node *pSende , bool playMusic)
{
//    clearAllAction();
    Sequence *seq = NULL;
    
    seq = Sequence::create(/*ScaleTo::create(0.1, 1.5 ,0.7),ScaleTo::create(0.1,0.8,1.3),ScaleTo::create(0.1 , 1.2 ,0.9),ScaleTo::create(0.1 ,1)JumpBy::create(0.1, Point(0,0), kElementSize/8, 1),*/JumpBy::create(0.15, Point(0,0), kElementSize/9, 1),JumpBy::create(0.09, Point(0,0), kElementSize/15, 1),CallFunc::create(CC_CALLBACK_0(Gem::sendFallDownEndMSG, this)),NULL);
    
    this->runAction(seq);
}
void Gem::sendFallDownEndMSG()
{
    NotificationCenter::getInstance()->postNotification(kMSG_GemFallDown);
//    clearAllAction();
}
void Gem::bright()
{
    GemAction::getInstance().setBright(_spr);
    
//    Sequence* seq = Sequence::create(ScaleTo::create(kBrightScaleTime, 1.3),ScaleTo::create(kBrightScaleTime, 1),CallFunc::create(this,callfunc_selector(Gem::brightCallback)),NULL);
    
    Sequence* seq = Sequence::create(/*ScaleTo::create(kBrightScaleTime, 1.3),ScaleTo::create(kBrightScaleTime, 0.5)*/DelayTime::create(0.04),CallFunc::create(CC_CALLBACK_0(Gem::brightCallback, this)),NULL);
    
    this->runAction(seq);
}
void Gem::brightCallback()
{
    GemAction::getInstance().removeBright(_spr);
}
void Gem::hideScore()
{
    
}
void Gem::showScore()
{
    
}
void Gem::frozenOff()
{
    if(!_frozenSprite) return;
    
    if (_frozen == 0)
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        frozenOffCallback();
        NotificationCenter::getInstance()->postNotification("display_score",this);
    }
    else if(_frozen == 1)
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        _frozenSprite->setTexture("frozen.png");
        NotificationCenter::getInstance()->postNotification("display_score",this);
    }
}

void Gem::rootsOff()
{
    if (!_rootsSprite) {
        return;
    }
    
    if (_roots == 0)
    {
        rootsOffCallback();
    }
    else if(_roots == 1)
    {
        _rootsSprite->setTexture("roots.png");
    }
}

void Gem::chainOff()
{
    if (!_chainSprite) {
        return;
    }
    
    if (_chain == 0)
    {
        Armature *armature1 = Armature::create( "effect_tiechuangsui");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        
        chainOffCallback();
    }
    else if(_chain == 1)
    {
        Armature *armature1 = Armature::create( "effect_tiechuangsui");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        _chainSprite->setTexture("chain.png");
    }
}


void Gem::restrainOff()
{
    if(!_restrainNode) return;
}

void Gem::frozenOffCallback()
{
    _frozenSprite->removeFromParentAndCleanup(true);
    _frozenSprite = NULL;
}

void Gem::rootsOffCallback()
{
    _rootsSprite->removeFromParentAndCleanup(true);
    _rootsSprite = NULL;
}

void Gem::chainOffCallback()
{
    _chainSprite->removeFromParentAndCleanup(true);
    _chainSprite = NULL;
}

void Gem::restrainOffCallback()
{
    _restrainNode->removeFromParentAndCleanup(true);
}
void Gem::fallDownToEnd(bool playMusic)
{
//    GemAction::getInstance().playEffectMusic(NULL, "dropdown.mp3");
    if (_vecActions.size() == 0)
    {
        return;
    }
    
    
    FiniteTimeAction* afterMoveAnimation = CallFuncN::create(CC_CALLBACK_1(Gem::fallDownEnd, this , playMusic));
    
    addAction(afterMoveAnimation);
    
    this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(Gem::fallDownStart, this)),Sequence::create(_vecActions),NULL));
    
}

void Gem::addAction(FiniteTimeAction* action)
{
    _vecActions.pushBack(action);
}

void Gem::runAnimition(Node* sender,void *animitionName)
{
    
}

void Gem::swap(Direction direction, bool reverse,bool isSelected,CallFunc* callbackAction)
{
    FiniteTimeAction *reverseAction,*swapAction/*,*selectScale1,*selectScale2,*nextScale1,*nextScale2*/,*delayTime;
    
    swapAction = GemAction::getInstance().getGemChangeAction(direction);
    
    delayTime = DelayTime::create(0.1);
    
    GemAction* caller = &(GemAction::getInstance());
    
    if(reverse)
    {
        if(isSelected)
        {
            reverseAction =EaseExponentialIn::create(GemAction::getInstance().getGemChangeAction(Direction(Down-direction)));
            this->runAction(Sequence::create(swapAction,/*CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3")),*/delayTime,reverseAction,callbackAction,CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3")),NULL));
        }
        else
        {
            this->setZOrder(-1);
            
            reverseAction = EaseExponentialIn::create(GemAction::getInstance().getGemChangeAction(Direction(Down-direction)));
            
            this->runAction(Sequence::create(swapAction,/*CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3")),*/delayTime,reverseAction,CallFunc::create(CC_CALLBACK_0(Gem::swapCallback, this)),callbackAction,CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3")),NULL));
        }
    }
    else
    {
        if(isSelected)
        {
            
            this->runAction(Sequence::create(swapAction  ,callbackAction,/*CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3")),*/NULL));
        }
        else
        {
            this->setZOrder(-1);
            
            this->runAction(Sequence::create(swapAction,CallFunc::create(CC_CALLBACK_0(Gem::swapCallback, this)),callbackAction/*,CallFuncN::create(CC_CALLBACK_1(GemAction::playEffectMusic,caller,"change.mp3"))*/,NULL));
        }
    }
}

void Gem::swapCallback()
{
    this->setZOrder(0);
}
void Gem::clearAllAction()
{
    if (_vecActions.size())
    {
        _vecActions.clear();
    }
}
//单个消除效果
void Gem::explosionParticle()
{
    NotificationCenter::getInstance()->postNotification("display_score",this);
    __String *str = __String::create("red_suilie.plist");
    switch (_type)
    {
        case red:
            str = __String::create("red_suilie.plist");
            break;
        case blue:
            str = __String::create("blue_suilie.plist");
            break;
        case green:
            str = __String::create("green_suilie.plist");
            break;
        case purple:
            str = __String::create("purple_suilie.plist");
            break;
        case white:
            str = __String::create("white_suilie.plist");
            break;
        case yellow:
            str = __String::create("yellow_suilie.plist");
            break;
        default:
            break;
    }

    if (_type == all )
    {
        Armature *armature1 = Armature::create( "tsx_eff");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
    }
    else
    {
        ParticleSystemQuad *partic1 = ParticleSystemQuad::create(str->getCString());
        _particleNode->addChild(partic1,11);
        partic1->setPosition(this->getPosition());
        partic1->setAutoRemoveOnFinish(true);
        
        ParticleSystemQuad *partic2 = ParticleSystemQuad::create("suilie2.plist");
        _particleNode->addChild(partic2,10);
        partic2->setPosition(this->getPosition());
        partic2->setAutoRemoveOnFinish(true);
        
//        partic2->setStartColor(Color4F(_particleColor1));
//        partic2->setEndColor(Color4F(_particleColor1));
        
        
//        Armature *armature1 = Armature::create( "effect_suilie");
//        _particleNode->addChild(armature1);
//        armature1->getAnimation()->playWithIndex(0);
//        armature1->setPosition(this->getPosition());
//        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        
    }
    this->setZOrder(1);
    
    _spr->setVisible(false);
}

void Gem::aroundAnimation(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
    
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    if (_skill == SkillAround1 || _skill == SkillAround2)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-1)&&(i <= mp.x+1) && (j >= mp.y-1) && (j <= mp.y+1) && _gemStoneMatrix[i][j])
                {
                    if (i == mp.x && j == mp.y)
                    {
                        continue;
                    }
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[i][j]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
        }
    }
    if (_skill == SkillAround3 || _skill == SkillAround4)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&&_gemStoneMatrix[i][j])
                {
                    if (i == mp.x && j == mp.y)
                    {
                        continue;
                    }
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[i][j]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
        }
    }
    
    if (_skill == SkillTen)
    {
        for (int i = 1; i < 3; i++)
        {
            if (mp.x - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y])
                {
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[mp.x - i][mp.y]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
            
            if (mp.x + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y])
                {
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[mp.x + i][mp.y]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
        }
        
        for (int i = 1; i < 3; i++)
        {
            if (mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x][mp.y - i])
                {
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[mp.x][mp.y - i]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
            if (mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x][mp.y + i])
                {
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[mp.x][mp.y + i]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
        }
    }
    
    if (_skill == SkillTurnTen)
    {
        for(int i=0;i<kMatrixWidth ;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if (i == mp.x || j == mp.y)
                {
                    continue;
                }

                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&& _gemStoneMatrix[i][j] )
                {
                    Armature *arm1 = Armature::create("jiugong04_effect");
                    arm1->getAnimation()->playWithIndex(0);
                    _particleNode->addChild(arm1);
                    arm1->setPosition(_gemStoneMatrix[i][j]->getPosition());
                    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::showSkillAnimation, this));
                }
            }
        }
    }

    
    Armature *arm1 = Armature::create("jiugong04_effect");
    arm1->getAnimation()->playWithIndex(0);
    _particleNode->addChild(arm1);
    arm1->setPosition(_gemStoneMatrix[mp.x][mp.y]->getPosition());
    arm1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::gemBright, this));
//    this->runAction(Sequence::create(DelayTime::create(0.4),CallFuncN::create(CC_CALLBACK_1(Gem::gemBright, this)), NULL));
    arm1->setTag(999);
}

void Gem::gemBright(Node *pSender)
{
    CCLOG("pSender->getTag() = %d" , pSender->getTag());
//    pSender->removeFromParentAndCleanup(true);
    
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    if (_skill == SkillHorizontal || _skill == SkillCross)
    {
        for (int i = 0; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y] /*&& _gemStoneMatrix[mp.x - i][mp.y]->getState() == -1  && !_gemStoneMatrix[mp.x - i][mp.y]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[mp.x - i][mp.y]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x - i][mp.y])), NULL));
                }
            }
            
            if (mp.x + i < kMatrixWidth)
            {
                if (_gemStoneMatrix[mp.x + i][mp.y] /*&& _gemStoneMatrix[mp.x + i][mp.y]->getState() == -1 && !_gemStoneMatrix[mp.x + i][mp.y]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[mp.x + i][mp.y]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x + i][mp.y])), NULL));
                }
            }
        }
        
    }
    
    if (_skill == SkillVerticl || _skill == SkillCross)
    {
        for (int i = 0; i < kMatrixWidth; i++)
        {
            if (mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x][mp.y - i] /*&& _gemStoneMatrix[mp.x][mp.y - i]->getState() == -1 && !_gemStoneMatrix[mp.x][mp.y - i]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[mp.x][mp.y - i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y - i])), NULL));
                }
            }
            
            if (mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x][mp.y + i] /*&& _gemStoneMatrix[mp.x][mp.y + i]->getState() == -1 && !_gemStoneMatrix[mp.x][mp.y + i]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[mp.x][mp.y + i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y + i])), NULL));
                }
            }
        }
        
    }
    
    if (_skill == SkillThreeCross)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if (( ((i >= mp.x-1) && (i <= mp.x+1)) || ((j >= mp.y-1) && (j <= mp.y+1)) ) &&_gemStoneMatrix[i][j] /*&& _gemStoneMatrix[i][j]->getState() == -1 && !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                {
                   _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[i][j])), NULL));
                }
            }
        }
    }
    
    if (_skill == SkillAround2 || _skill == SkillAround1)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-1)&&(i <= mp.x+1)&&(j >= mp.y-1)&&(j <= mp.y+1)&&_gemStoneMatrix[i][j] /*&& _gemStoneMatrix[i][j]->getState() == -1 && !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[i][j])), NULL));
                }
            }
        }
        if (_skill == SkillAround1 && _state == 1)
        {
            _state = -1;
            _skill = SkillNull;
            _gemStoneMatrix[mp.x][mp.y]->runAction(Sequence::create(DelayTime::create(0),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y])), NULL));
        }
    }
    
    if (_skill == SkillAround4 || _skill == SkillAround3)
    {
        for(int i=0;i<kMatrixWidth;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&&_gemStoneMatrix[i][j] /*&& _gemStoneMatrix[i][j]->getState() == -1 && !_gemStoneMatrix[i][j]->getSameAndSkill()*/)
                {
                    _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[i][j])), NULL));
                }
            }
        }
        if (_skill == SkillAround3 && _state == 1)
        {
            _state = -1;
            _skill = SkillNull;
            _gemStoneMatrix[mp.x][mp.y]->runAction(Sequence::create(DelayTime::create(0),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y])), NULL));
        }
    }
    
    if (_skill == SkillLeft)
    {
        for (int i = 0; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0 && mp.y + i < kMatrixWidth)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y + i])
                {
                    _gemStoneMatrix[mp.x - i][mp.y + i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x - i][mp.y + i])), NULL));
                }
            }
            
            if (mp.x + i < kMatrixWidth && mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x + i][mp.y - i])
                {
                    _gemStoneMatrix[mp.x + i][mp.y - i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x + i][mp.y - i])), NULL));
                }
            }
        }

    }
    
    if (_skill == SkillRight)
    {
        for (int i = 0; i < kMatrixWidth; i++)
        {
            if (mp.x - i >= 0 && mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y - i])
                {
                    _gemStoneMatrix[mp.x - i][mp.y - i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x - i][mp.y - i])), NULL));
                }
            }
            
            if (mp.x + i < kMatrixWidth && mp.y + i < kMatrixWidth)
            {
                if (_gemStoneMatrix[mp.x + i][mp.y + i])
                {
                    _gemStoneMatrix[mp.x + i][mp.y + i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x + i][mp.y + i])), NULL));
                }
            }
        }
        
    }

    if (_skill == SkillTen)
    {
        for (int i = 0; i < 3; i++)
        {
            if (mp.x - i >= 0)
            {
                if (_gemStoneMatrix[mp.x - i][mp.y])
                {
                    if (_gemStoneMatrix[mp.x - i][mp.y])
                    {
                        _gemStoneMatrix[mp.x - i][mp.y]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x - i][mp.y])), NULL));
                    }
                }
            }
            
            if (mp.x + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x + i][mp.y])
                {
                    if (_gemStoneMatrix[mp.x + i][mp.y])
                    {
                        _gemStoneMatrix[mp.x + i][mp.y]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x - i][mp.y])), NULL));
                    }
                }
            }
        }
        
        for (int i = 0; i < 3; i++)
        {
            if (mp.y - i >= 0)
            {
                if (_gemStoneMatrix[mp.x][mp.y - i])
                {
                    if (_gemStoneMatrix[mp.x][mp.y - i])
                    {
                        _gemStoneMatrix[mp.x][mp.y - i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y - i])), NULL));
                    }
                }
            }
            if (mp.y + i < kMatrixWidth)
            {
                
                if (_gemStoneMatrix[mp.x][mp.y + i])
                {
                    if (_gemStoneMatrix[mp.x][mp.y + i])
                    {
                        _gemStoneMatrix[mp.x][mp.y + i]->runAction(Sequence::create(DelayTime::create(0.05 * i),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[mp.x][mp.y + i])), NULL));
                    }
                }
            }
        }
    }

    if (_skill == SkillTurnTen)
    {
        for(int i=0;i<kMatrixWidth ;i++)
        {
            for (int j=0;j<kMatrixWidth; j++)
            {
                if (i == mp.x || j == mp.y)
                {
                    continue;
                }
                
                if ((i >= mp.x-2)&&(i <= mp.x+2)&&(j >= mp.y-2)&&(j <= mp.y+2)&& _gemStoneMatrix[i][j] )
                {
                    int times = abs(i - mp.x) + abs(j - mp.y) - 1;
                    _gemStoneMatrix[i][j]->runAction(Sequence::create(DelayTime::create(0.05 * times),CallFuncN::create(CC_CALLBACK_1(Gem::gemBrightStar, _gemStoneMatrix[i][j])), NULL));
                }
            }
        }

    }

}

void Gem::gemBrightStar(Node *pSender)
{
    Gem* gem = (Gem*)pSender;
    

    if (gem->getGemType() < all)
    {
        Armature *arm = Armature::create(brightName(gem));
        arm->getAnimation()->playWithIndex(0);
        gem->addChild(arm);
        arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::gemBrightOver, gem,gem));
    }
    
}

void Gem::gemBrightOver(Node *pSender,Gem *gem)
{
    if (gem->getState() != -1)
    {
        pSender->removeFromParentAndCleanup(true);
    }
    
    gem->removeNoCollect(false);  //依次 消除
    
    matchAll();  // 要解决 
}

void Gem::removeSelf(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
}

void Gem::removeSameGem(Node *pSender ,GemType type)
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for (int j=0; j<kMatrixWidth; j++)
        {
            if(_gemStoneMatrix[i][j]/*&&_gemStoneMatrix[i][j]->getState() == -1*/ && _gemStoneMatrix[i][j]->getGemType() == type)
            {
                _gemStoneMatrix[i][j]->beforeExplode();
            }
        }
    }
}

void Gem::afterExplode()
{
    NotificationCenter::getInstance()->postNotification("after_match");
}

void Gem::sameSkillOver(Node *pSender ,GemType type)
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for (int j=0; j<kMatrixWidth; j++)
        {
            if(_gemStoneMatrix[i][j]&& _gemStoneMatrix[i][j]->getGemType() == type && _gemStoneMatrix[i][j]->getNextSkill() == SkillNull)
            {
                _gemStoneMatrix[i][j]->beforeExplode();
            }
        }
    }
    this->runAction(Sequence::create(DelayTime::create(1.5),CallFunc::create(CC_CALLBACK_0(Gem::afterExplode, this)), NULL));
}

void Gem::removeSameSkill()
{

    beforeExplode();
}

void Gem::fiveMatchEffect(Node* sender)
{
    Sprite* spr = (Sprite*)sender;
    
//    spr->removeFromParentAndCleanup(true);

    spr->setOpacity(0);
    spr->runAction(Sequence::create(DelayTime::create(0.15),CallFuncN::create(CC_CALLBACK_1(Gem::removeSelf, this)), NULL));
    
    if (!_sameAndSkill)
    {
        MyPoint mp = getCurrentIndex(this->getPosition());
        explode(mp, mp, 0, 0);
    }
    if (_nextskill)
    {
        if (_type == all)
        {
            return;
        }
        Armature *arm = Armature::create(brightName(this));
        arm->getAnimation()->playWithIndex(0);
        this->addChild(arm);
        arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::sameDisplay, this));
    }
}

//四消效果
void Gem::fourMatchEffect(Node* sender)
{
    
}
void Gem::addScore(int count,bool isAffect)
{
    
}

bool Gem::hasSkill(Gem *gem)
{
    if (_skill)
    {
        if (gem != NULL && gem->canMove() && gem->getGemType()<dragontooth && _skill == SkillAllSame)
        {
            return true;
        }
        return (gem != NULL && gem->getGemSkill());
    }
    return false;
}

void Gem::breakIceFloor(int i, int j)
{
    
}

bool Gem::equal(Gem *gem)
{
    return (gem!=NULL&&_type==gem->getGemType() && _type < all);
}

//设置生病和病好，生成动画加入队列
void Gem::setSick(bool sick)
{
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//三个元素相连，针对每个调用此方法，出入聚合中心点及是否变病态                                                        //
//1,判断_state，_frozen,_restrain                                                                           //
//1)冰冻或者束缚大于0，则减一，执行动画                                                                         //
//2)冰冻或束缚等于0，减一，判断是不是可消元素，将动画加入队列                                                      //
//3)聚合根据聚合目的地，创建聚合动画,加入队列，如果本聚合元素下一动作为可消除，则将飞走动作加入队列                      //
//4)病态的将得分设置为0,如果之前是非病态，调用变病态动画                                                         //
//5)相连元素其中有一个是病态的，则本排所有元素变为病态                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Gem::explode(MyPoint &myPoint,MyPoint &distPoint,int count,int index)
{
    int i = myPoint.x,j=myPoint.y;
    
    if(i-1>=0&&_gemStoneMatrix[i-1][j])
    {
        _gemStoneMatrix[i-1][j]->affected(count);
    }
    if(i+1<kMatrixWidth&&_gemStoneMatrix[i+1][j])
    {
        _gemStoneMatrix[i+1][j]->affected(count);
    }
    if(j-1>=0&&_gemStoneMatrix[i][j-1])
    {
        _gemStoneMatrix[i][j-1]->affected(count);
    }
    if(j+1<kMatrixWidth&&_gemStoneMatrix[i][j+1])
    {
        _gemStoneMatrix[i][j+1]->affected(count);
    }
    
    //冰冻或者束缚
    if(_frozen>0||_restrain>0||_roots>0||_chain>0)
    {
        return;
    }
    
    //普通元素，非冰冻/束缚
    //单个消除不对周围产生影响
    if(-1!=_state)//判断是否已经被消除，不能同时消除两次
    {
        if(index==1)
        {
            setFlag(1);
        }
        
        _state=-1;
    }
}
//1,判断自己是不是相连，如果相连元素则返回
//2,判断是不是病态，病态元素返回
//3,设置change字段为被影响
//4,普通元素改变累积值
//5,固定元素和派生元素改变状态，并执行动画
//6,变成病态
void Gem::affected(int count)
{
    
}

void Gem::beforeMatch(int i, int j)
{
}
void Gem::afterMatch(int i, int j)
{
    if(_change<=Eliminate&&_state!=-1)
    {
        _change = NoChange;
    }
    if(_change>=Changed)
    {
        _change = NoChange;
    }
}
void Gem::growOnFertile(int i, int j)
{
    
}

int Gem::getUniqueNumber(int max)
{
    static int flag[kMatrixWidth*kMatrixWidth] = {0};
    
    if(max==-1)
    {
        for(int i=0;i<kMatrixWidth*kMatrixWidth;i++)
        {
            flag[i] = i;
        }
        
        return 0;
    }
    else
    {
        if(max<1) return flag[0];
        
        int t = arc4random()%max;
        
        int result = flag[t];
        
        flag[t] = flag[max-1];
        
        return result;
    }
}

MyPoint Gem::getCurrentIndex(const Point& point)
{
    return MyPoint((point.x-kStartX)/kElementSize,(point.y-kStartY)/kElementSize);
}

long Gem::numbersOfSprRunningAction()
{
    return _spr->getNumberOfRunningActions();
}

void Gem::winnerModeStart()
{
    int random = arc4random()%2;
    if (random)
    {
        _nextskill = SkillHorizontal;
    }
    else
    {
        _nextskill = SkillVerticl;
    }
    setSkillSpr();
}
void Gem::winnerMode()
{
    
}

void Gem::propSingle()
{
    
}

void Gem::playExplodeRandomMusic(Node* sender)
{
    GemAction::getInstance().playEffectMusic(NULL,"boom.mp3");
}

void Gem::showSkillAnimation(Node *pSender)
{
    pSender->removeFromParentAndCleanup(true);
}

void Gem::starBright(Node *pSender)
{
    Gem* gem = (Gem*)pSender;
    Armature *arm = Armature::create(brightName(gem));
    arm->getAnimation()->playWithIndex(0);
    gem->addChild(arm);
    arm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this));
}

const char* Gem::brightName(Gem *gem)
{
    __String *str = __String::create("");
    
    switch (gem->getGemType())
    {
        case red:
            str = __String::create("red_liang");
            break;
        case blue:
            str = __String::create("blue_liang");
            break;
        case green:
            str = __String::create("green_liang");
            break;
        case purple:
            str = __String::create("purple_liang");
            break;
        case white:
            str = __String::create("white_liang");
            break;
        case yellow:
            str = __String::create("yellow_liang");
            break;
        default:
            break;
    }
    return str->getCString();
}

void Gem::addHudun()
{
    Armature *armature = Armature::create( "hudun_effect");
    _particleNode->addChild(armature);
    armature->getAnimation()->playWithIndex(0);
    armature->setPosition(this->getPosition());
    armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
}

void Gem::explodeAll(Node *pSender)
{
//    if (pSender)
//    {
//        Sprite* spr = (Sprite*)pSender;
//        spr->setOpacity(0);
//        spr->runAction(Sequence::create(DelayTime::create(0.15),CallFuncN::create(CC_CALLBACK_1(Gem::removeSelf, this)), NULL));
//    }
    MyPoint mp = getCurrentIndex(this->getPosition());
    
    explode(mp, mp, 0, 0);
    
    if(_frozen>0||_restrain>0||_roots>0||_chain>0)
    {
        if(_frozen>0)
        {
            _frozen--;
            frozenOff();
        }
        else if(_restrain>0)
        {
            _restrain--;
            restrainOff();
        }
        else if(_roots>0)
        {
            _roots--;
            rootsOff();
        }
        else if(_chain>0)
        {
            _chain--;
            chainOff();
        }
        
        return;
    }
    
    if (_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y))
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _mapNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        auto spr = (Sprite*)_mapNode->getChildByTag(mp.x * kMatrixWidth + mp.y);
        int iFlag = spr->getLocalZOrder();
        if (iFlag == 1)
        {
            spr->setZOrder(-1);
            NotificationCenter::getInstance()->postNotification("display_score",this);
            
            _mapNode->removeChildByTag(mp.x * kMatrixWidth + mp.y , true);
            
            int msg = 1000*10;
            __String *str = __String::createWithFormat("%d",msg);
            NotificationCenter::getInstance()->postNotification(kMSG_ShowCollecte,str );
        }
        if(iFlag == 2)
        {
            spr->setTexture("icefloor.png");
            spr->setLocalZOrder(1);
        }
    }
    
    if (_state == -1 || (_type == all && _state == all))
    {
        _emptyPointVector->push_back(mp);
        _removeAlready = true;
        this->runAction(Sequence::create(CallFuncN::create(CC_CALLBACK_1(Gem::playExplodeRandomMusic, this)),CallFunc::create(CC_CALLBACK_0(Gem::explosionParticle, this)),CallFunc::create(CC_CALLBACK_0(Gem::addCollectGem, this)),CallFunc::create(CC_CALLBACK_0(Gem::removeSkillSpr, this)),CallFunc::create(CC_CALLBACK_0(Gem::hideSprite, this)),CallFuncN::create(CC_CALLBACK_1(Gem::removeSelf, this)),NULL));
    }
    
}