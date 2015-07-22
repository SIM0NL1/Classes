#include "Gem.h"

Special::Special()
{
    _state = 0;
    _type = special;
    _GemKind = SpecialGem;
    _specialCount++;
}

bool Special::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "scale.png");
    
    return true;
}

bool Special::canMove()
{
    return true;
}

bool Special::equal(Gem* gem)
{
    return false;
}

void Special::beforeMatch(int i,int j)
{
    kEmptyFunction;
}
void Special::affected(int count)
{
    if(_state<0) return;
    _state--;
    
}

void Special::changeState(int state)
{
    if (_state < 0)
    {
        //状态改变。。。
    }
}

void Special::idle()
{
   
}
