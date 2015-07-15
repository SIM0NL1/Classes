//
//  PropNode.cpp
//  DragonGame
//
//  Created by B081mac on 15/7/14.
//
//

#include "PropNode.h"

PropNode::PropNode()
{
    
}
PropNode::~PropNode()
{
    
}

PropNode *PropNode::create(PROP_TYPE type, GameLayer* layer)
{
    PropNode *pRet = new(std::nothrow) PropNode();
    if (pRet && pRet->init(type,layer))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool PropNode::init(PROP_TYPE type, GameLayer* layer)
{
    if (!Node::init())
    {
        return false;
    }
    
//    auto draw = DrawNode::create();
//    addChild(draw, 10);
//    
//    
//    draw->drawCircle(getPosition(), 40, CC_DEGREES_TO_RADIANS(90), 30, false, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

    
    m_ePropType = type;
    m_pGameLayer = layer;
    
    initProp();
    
    EventListenerTouchOneByOne* listener_touch = EventListenerTouchOneByOne::create();
    listener_touch->onTouchBegan = CC_CALLBACK_2(PropNode::onTouchBegan,this);
    listener_touch->onTouchMoved = CC_CALLBACK_2(PropNode::onTouchMoved,this);
    listener_touch->onTouchEnded = CC_CALLBACK_2(PropNode::onTouchEnded,this);
    listener_touch->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener_touch, this);

    return true;
}

void PropNode::initProp()
{
    m_armProp = Armature::create(getArmName());
    this->addChild(m_armProp);
    m_armProp->setPosition(Vec2::ZERO);
    m_armProp->getAnimation()->play("Animation1");
    
    auto numBg = Sprite::create("ui_kuang03.png");
    m_armProp->addChild(numBg,10);
    numBg->setPosition(Vec2(-35,-25));
    
    m_labProNum = LabelAtlas::create("1", "shuliang02_number.png", 12, 16, '0');
    m_armProp->addChild(m_labProNum,10);
    m_labProNum->setPosition(numBg->getPosition());
    m_labProNum->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    __String* str = __String::createWithFormat("%d",m_ePropType);
    m_labProNum->setString(str->getCString());
    
}

const char* PropNode::getArmName()
{
    __String *str ;
    switch (m_ePropType)
    {
        case Prop_Hammer:
            str = __String::create("chuizi_icon");
            break;
        case Prop_Cross:
            str = __String::create("shizi_icon");
            break;
        case Prop_TNT:
            str = __String::create("lingxing_icon");
            break;
        case Prop_Magic:
            str = __String::create("mobang_icon");
            break;
        case Prop_Change:
            str = __String::create("nuqi_icon");
            break;
        case Prop_Add5:
            str = __String::create("jiawu_icon");
            break;
        default:
            break;
    }
    return str->getCString();
}

void PropNode::stopAnimation()
{
    Vec2 pos = m_armProp->getPosition();
    m_armProp->removeFromParentAndCleanup(true);
    m_armProp = NULL;
    
    m_armProp = Armature::create("jiawu_icon");
    this->addChild(m_armProp);
    m_armProp->setPosition(pos);
    m_armProp->getAnimation()->play("Animation1");
}

void PropNode::onEnter()
{
    Node::onEnter();
}
void PropNode::onExit()
{
    _eventDispatcher->removeEventListenersForTarget(this);
    Node::onExit();
}

bool PropNode::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (m_pGameLayer->getChildByTag(9999))
    {
        return false;
    }
    
    Point location = pTouch->getLocation();
    if ((int)location.distance(this->getPosition()) <= 40)
    {
        if (m_ePropType == Prop_Add5)
        {
            m_pGameLayer->matrixBright();
            m_armProp->getAnimation()->play("Animation2");
            m_armProp->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_0(PropNode::stopAnimation, this) );
            return false;
        }
        
        m_armProp->setOpacity(100);
        if (this->getChildByTag(PROP_TAG))
        {
            this->removeChildByTag(PROP_TAG);
        }
        auto propSpr = Armature::create(getArmName());
        
        this->getParent()->addChild(propSpr,120,PROP_TAG);
        propSpr->setPosition(this->getPosition() + Vec2(-20, 20));

        return true;
    }
    return false;
}
void PropNode::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point pos = pTouch->getLocation();
    auto sp = this->getParent()->getChildByTag(PROP_TAG);
    sp->setPosition(pos + Vec2(-20, 20));
    
    pos = m_pGameLayer->convertToNodeSpace(sp->getPosition());
    
    int a = pos.x;
    
    int b = pos.y;
    m_pGameLayer->setPropType(m_ePropType);
    m_pGameLayer->showPropRange(a, b);
    
}
void PropNode::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    m_armProp->setOpacity(255);
    
    
    m_pGameLayer->hidePropRange();
    auto prop = this->getParent()->getChildByTag(PROP_TAG);
    
    Point pos = m_pGameLayer->convertToNodeSpace(prop->getPosition());
        
    int a = pos.x;
        
    int b = pos.y;
    
    if (m_pGameLayer->useProp(a, b))
    {
        m_pGameLayer->propAnimation((Armature*)prop);
    }
    else
    {
        if (this->getParent()->getChildByTag(PROP_TAG))
        {
            this->getParent()->removeChildByTag(PROP_TAG);
        }
    }
    
}

void PropNode::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}