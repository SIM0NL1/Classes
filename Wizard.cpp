#include "Gem.h"

Wizard::Wizard()
{
    _state = 2;
    _type = wizard;
    _GemKind = SpecialGem;
}

bool Wizard::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "darkwizard_0.png");
    
    auto sp = Sprite::create("darklight_0.png");
    
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
    sp->runAction(RepeatForever::create(actionH));
    sp->setPosition(Vec2(_spr->getBoundingBox().size.width/2, _spr->getBoundingBox().size.height/2));
    _spr->addChild(sp,-1);
    
    return true;
}

bool Wizard::canMove()
{
    return false;
}

bool Wizard::equal(Gem* gem)
{
    return false;
}

void Wizard::beforeMatch(int i,int j)
{
    kEmptyFunction;
}
void Wizard::affected(int count)
{
    
}

void Wizard::afterMatch(int i, int j)
{
    createFog(i , j ,1);
}

void Wizard::changeState(int state)
{

}

void Wizard::idle()
{
    
}

void Wizard::createFog(int i ,int j , int distance)
{
    if(i-distance>=0&&_gemStoneMatrix[i-distance][j]&&_gemStoneMatrix[i-distance][j]->getGemType() != fog)
    {
        Gem *temp = _gemStoneMatrix[i-distance][j];
        _gemStoneMatrix[i-distance][j] = Gem::createFixGem(fog,this->getParent(),temp->getPosition(),true);
        temp->removeFromParentAndCleanup(true);
    }
    else if(i+distance<kMatrixWidth&&_gemStoneMatrix[i+distance][j]&&_gemStoneMatrix[i+distance][j]->getGemType() != fog)
    {
        Gem *temp = _gemStoneMatrix[i+distance][j];
        _gemStoneMatrix[i+distance][j] = Gem::createFixGem(fog,this->getParent(),temp->getPosition(),true);
        temp->removeFromParentAndCleanup(true);
    }
    else if(j-distance>=0&&_gemStoneMatrix[i][j-distance]&&_gemStoneMatrix[i][j-distance]->getGemType() != fog)
    {
        Gem *temp = _gemStoneMatrix[i][j-distance];
        _gemStoneMatrix[i][j-distance] = Gem::createFixGem(fog,this->getParent(),temp->getPosition(),true);
        temp->removeFromParentAndCleanup(true);
    }
    else if(j+distance<kMatrixWidth&&_gemStoneMatrix[i][j+distance]&&_gemStoneMatrix[i][j+distance]->getGemType() != fog)
    {
        Gem *temp = _gemStoneMatrix[i][j+distance];
        _gemStoneMatrix[i][j+distance] = Gem::createFixGem(fog,this->getParent(),temp->getPosition(),true);
        temp->removeFromParentAndCleanup(true);
    }
    else
    {
        if (distance == kMatrixWidth)
        {
            return;
        }
        createFog(i , j ,distance + 1);
    }
}
