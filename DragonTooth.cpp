#include "Gem.h"

DragonTooth::DragonTooth()
{
    _state = 0;
    _type = dragontooth;
    _GemKind = SpecialGem;
}

bool DragonTooth::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "tooth.png");
    
    return true;
}

bool DragonTooth::canMove()
{
    return true;
}
bool DragonTooth::equal(Gem *fruit)
{
    return false;
}

void DragonTooth::beforeMatch(int i, int j)
{
    if(_mapInfo->isCollectPosition(i, j))
    {
        _state=-1;
        
        AnimationWraper aw2(NULL,e_aid_normal_explode,e_priority_normal_explode);
        
        _animationWraperVector->push_back(aw2);
    }
}

