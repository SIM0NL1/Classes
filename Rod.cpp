#include "Gem.h"

Rod::Rod()
{
    _state = 0;
    _type = rod;
    _GemKind = SpecialGem;
}

bool Rod::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "rod.png");
    
    return true;
}

bool Rod::canMove()
{
    return true;
}
bool Rod::equal(Gem *fruit)
{
    return false;
}

void Rod::beforeMatch(int i, int j)
{
    if(_mapInfo->isCollectPosition(i, j))
    {
        _state=-1;
        
        AnimationWraper aw2(NULL,e_aid_normal_explode,e_priority_normal_explode);
        
        _animationWraperVector->push_back(aw2);
    }
}

