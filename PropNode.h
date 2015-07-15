//
//  PropNode.h
//  DragonGame
//
//  Created by B081mac on 15/7/14.
//
//

#ifndef __DragonGame__PropNode__
#define __DragonGame__PropNode__

#include <stdio.h>
#include "cocos2d.h"
#include "DataTools.h"
#include "GameLayer.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

USING_NS_CC;

#define PROP_TAG 33

class PropNode : public Node
{
private:
    PROP_TYPE m_ePropType;
    Armature *m_armProp;
    LabelAtlas *m_labProNum;
    GameLayer *m_pGameLayer;
public:
    PropNode();
    ~PropNode();
    
    static PropNode *create(PROP_TYPE type , GameLayer* layer);
    
    virtual bool init(PROP_TYPE type, GameLayer* layer);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void initProp();
    
    const char* getArmName();
    
    void stopAnimation();
};

#endif /* defined(__DragonGame__PropNode__) */
