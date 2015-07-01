#include "Gem.h"

Pumpkin::Pumpkin()
{
    _state = 0;
    _type = pumpkin;
    _GemKind = SpecialGem;
}

bool Pumpkin::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "pumpkin.png");
    
    return true;
}

bool Pumpkin::canMove()
{
    return true;
}
bool Pumpkin::equal(Gem *fruit)
{
    return false;
}

void Pumpkin::beforeMatch(int i, int j)
{
    if(_mapInfo->isCollectPosition(i, j))
    {
        _state=-1;
        
        AnimationWraper aw2(NULL,e_aid_normal_explode,e_priority_normal_explode);
        
        _animationWraperVector->push_back(aw2);
    }
}

