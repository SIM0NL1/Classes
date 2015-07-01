#include "Gem.h"

Bottle::Bottle(GemType type)
{
    _state = 0;
    _type = type;
    _GemKind = SpecialGem;
}

bool Bottle::createGemSprite(Node *layer, Point point,bool visible)
{
    
    __String *str = __String::create("");
    if (_type == redbottle || _type == yellowbottle || _type == bluebottle || _type == greenbottle || _type == purplebottle || _type == whitebottle)
    {
        _state = 3;
        str = __String::create("bottle.png");
    }
    else if (_type == redbottle1)
    {
        _state = 2;
        str = __String::create("bottle1_red.png");
    }
    else if (_type == yellowbottle1)
    {
        _state = 2;
        str = __String::create("bottle1_yellow.png");
    }
    else if (_type == bluebottle1)
    {
        _state = 2;
        str = __String::create("bottle1_blue.png");
    }
    else if (_type == greenbottle1)
    {
        _state = 2;
        str = __String::create("bottle1_green.png");
    }
    else if (_type == purplebottle1)
    {
        _state = 2;
        str = __String::create("bottle1_purple.png");
    }
    else if (_type == whitebottle1)
    {
        _state = 2;
        str = __String::create("bottle1_white.png");
    }
    else if (_type == redbottle2)
    {
        _state = 1;
        str = __String::create("bottle2_red.png");
    }
    else if (_type == yellowbottle2)
    {
        _state = 1;
        str = __String::create("bottle2_yellow.png");
    }
    else if (_type == bluebottle2)
    {
        _state = 1;
        str = __String::create("bottle2_blue.png");
    }
    else if (_type == greenbottle2)
    {
        _state = 1;
        str = __String::create("bottle2_green.png");
    }
    else if (_type == purplebottle2)
    {
        _state = 1;
        str = __String::create("bottle2_purple.png");
    }
    else if (_type == whitebottle2)
    {
        _state = 1;
        str = __String::create("bottle2_white.png");
    }
    else if (_type == redbottle3)
    {
        _state = 0;
        str = __String::create("bottle3_red.png");
    }
    else if (_type == yellowbottle3)
    {
        _state = 0;
        str = __String::create("bottle3_yellow.png");
    }
    else if (_type == bluebottle3)
    {
        _state = 0;
        str = __String::create("bottle3_blue.png");
    }
    else if (_type == greenbottle3)
    {
        _state = 0;
        str = __String::create("bottle3_green.png");
    }
    else if (_type == purplebottle3)
    {
        _state = 0;
        str = __String::create("bottle3_purple.png");
    }
    else if (_type == whitebottle3)
    {
        _state = 0;
        str = __String::create("bottle3_white.png");
    }
    
    Gem::createGemSpriteByName(layer, point, visible, str->getCString());
    
    
    
    return true;
}

bool Bottle::canMove()
{
    return false;
}
bool Bottle::equal(Gem *fruit)
{
    return false;
}

void Bottle::beforeMatch(int i, int j)
{
    if(_state<0) return;
    
    
    if(j + 1 < kMatrixWidth)
    {
        if (_gemStoneMatrix[i][j+1])
        {
            GemType type = (GemType)((_type - redbottle) / 5 + 1);
            if (_gemStoneMatrix[i][j+1]->getGemType() == type && _gemStoneMatrix[i][j+1]->getState() != -1)
            {
                _state-- ;
                MyPoint mp = MyPoint(i, j + 1);
                _gemStoneMatrix[i][j+1]->explode(mp, mp, 0, 0);
                
                AnimationWraper aw(NULL, e_aid_normal_explode, e_priority_normal_explode);
                
                _animationWraperVector->push_back(aw);
                
                __String *str = __String::create("");
                if (_state == 2)
                {
                    if (type == red )
                    {
                        str = __String::create("bottle1_red.png");
                    }
                    else if (type == yellow)
                    {
                        str = __String::create("bottle1_yellow.png");
                    }
                    else if (type == blue)
                    {
                        _state = 2;
                        str = __String::create("bottle1_blue.png");
                    }
                    else if (type == green)
                    {
                        str = __String::create("bottle1_green.png");
                    }
                    else if (type == purple)
                    {
                        str = __String::create("bottle1_purple.png");
                    }
                    else if (type == white)
                    {
                        str = __String::create("bottle1_white.png");
                    }
                }
                else if(_state == 1)
                {
                    if (type == red)
                    {
                        str = __String::create("bottle2_red.png");
                    }
                    else if (type == yellow)
                    {
                        str = __String::create("bottle2_yellow.png");
                    }
                    else if (type == blue)
                    {
                        str = __String::create("bottle2_blue.png");
                    }
                    else if (type == green)
                    {
                        str = __String::create("bottle2_green.png");
                    }
                    else if (type == purple)
                    {
                        str = __String::create("bottle2_purple.png");
                    }
                    else if (type == white)
                    {
                        str = __String::create("bottle2_white.png");
                    }

                }
                else if(_state == 0)
                {
                    if (type == red)
                    {
                        str = __String::create("bottle3_red.png");
                    }
                    else if (type == yellow)
                    {
                        str = __String::create("bottle3_yellow.png");
                    }
                    else if (type == blue)
                    {
                        str = __String::create("bottle3_blue.png");
                    }
                    else if (type == green)
                    {
                        str = __String::create("bottle3_green.png");
                    }
                    else if (type == purple)
                    {
                        str = __String::create("bottle3_purple.png");
                    }
                    else if (type == white)
                    {
                        str = __String::create("bottle3_white.png");
                    }
                }
                else if(_state == -1)
                {
                    if (type == red)
                    {
                        str = __String::create("bottle4_red.png");
                    }
                    else if (type == yellow)
                    {
                        str = __String::create("bottle4_yellow.png");
                    }
                    else if (type == blue)
                    {
                        str = __String::create("bottle4_blue.png");
                    }
                    else if (type == green)
                    {
                        str = __String::create("bottle4_green.png");
                    }
                    else if (type == purple)
                    {
                        str = __String::create("bottle4_purple.png");
                    }
                    else if (type == white)
                    {
                        str = __String::create("bottle4_white.png");
                    }
                }
                _spr->setTexture(str->getCString());
            }
        }
        
        if (_state == -1)
        {
            AnimationWraper aw2(NULL,e_aid_normal_explode,e_priority_normal_explode);
            
            _animationWraperVector->push_back(aw2);
        }
    }
}

