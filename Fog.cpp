#include "Gem.h"

Fog::Fog()
{
    _state = 0;
    _type = fog;
    _GemKind = SpecialGem;
}

bool Fog::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "darklight_0.png");
    
    auto animation = Animation::create();
    
    for (int i = 0; i < 5; i++)
    {
        __String *file = __String::createWithFormat("darklight_%d.png",i);
        
        auto sprite = Sprite::create(file->getCString());
        animation->addSpriteFrame(sprite->getSpriteFrame());
    }
    
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    auto actionH = Animate::create(animation);
    _spr->runAction(RepeatForever::create(actionH));
    
    return true;
}

bool Fog::canMove()
{
    return false;
}

bool Fog::equal(Gem* gem)
{
    return false;
}

void Fog::beforeMatch(int i,int j)
{
    kEmptyFunction;
}
void Fog::affected(int count)
{
    if(_state<0) return;
    _state--;
    
}

void Fog::changeState(int state)
{
    if (_state < 0)
    {
        removeGem();
    }
}

void Fog::idle()
{
   
}
