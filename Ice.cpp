#include "Gem.h"

Ice::Ice(GemType type)
{
    _state = 2;
    _type = type;
    _GemKind = SpecialGem;
}

bool Ice::createGemSprite(Node *layer, Point point,bool visible)
{
    __String *str = __String::create("");
    if (_type == ice)
    {
        _state = 0;
        str = __String::create("ice.png");
    }
    else if (_type == ice1)
    {
        _state = 1;
        str = __String::create("ice1.png");
    }
    else if (_type == shield)
    {
        _state = 0;
        str = __String::create("shield.png");
    }
    else if (_type == shield1)
    {
        _state = 1;
        str = __String::create("shield1.png");
    }
    else if (_type == shield2)
    {
        _state = 2;
        str = __String::create("shield2.png");
    }
    else if (_type == shield3)
    {
        _state = 3;
        str = __String::create("shield3.png");
    }
    else if (_type == shield4)
    {
        _state = 4;
        str = __String::create("shield4.png");
    }
    if (_type == babydragon)
    {
        _state = 1;
        str = __String::create("dragonegg1.png");
    }
    else if (_type == magicscrolls)
    {
        _state = 1;
        str = __String::create("magicscrolls.png");
    }
    else if (_type == lifttree)
    {
        _state = 1;
        str = __String::create("lifetree.png");
    }
    else if (_type == stormwind)
    {
        _state = 1;
        str = __String::create("stormwind2.png");
    }
    Gem::createGemSpriteByName(layer, point, visible, str->getCString());
    
    return true;
}

bool Ice::canMove()
{
    return false;
}

bool Ice::equal(Gem* gem)
{
    return false;
}

void Ice::beforeMatch(int i,int j)
{
    kEmptyFunction;
}
void Ice::affected(int count)
{
    if(_state<0) return;
    
    _state--;
    // 冰块最终状态
    if(_state<0)
    {
        AnimationWraper aw1(this, e_aid_fish_match,e_priority_fish_changestate);
        
        aw1.state = 5;
        
        
        changeState(5);
    }
    else if(_state==0)
    {
        AnimationWraper aw(this, e_aid_fish_match, e_priority_fish_changestate);
        
        aw.state = 4;
        
        changeState(4);
    }
    else if(_state==1)
    {
        AnimationWraper aw(this, e_aid_fish_match, e_priority_fish_changestate);
        
        aw.state = 3;
        
        
        changeState(3);
    }
    else if(_state==2)
    {
        AnimationWraper aw(this, e_aid_fish_match, e_priority_fish_changestate);
        
        aw.state = 2;
        
        
        changeState(2);
    }
    else if(_state==3)
    {
        AnimationWraper aw(this, e_aid_fish_match, e_priority_fish_changestate);
        
        aw.state = 1;
        
        changeState(1);
    }
    
}

void Ice::changeState(int state)
{
    if (_type == ice1 || _type == ice)
    {
        Armature *armature1 = Armature::create( "effect_bingkuaisui");
        _particleNode->addChild(armature1);
        armature1->getAnimation()->playWithIndex(0);
        armature1->setPosition(this->getPosition());
        armature1->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_1(Gem::removeSelf, this) );
        if(state==4)
        {
            _spr->setTexture("ice.png");
            _type = ice;
        }
        else if(state==5)
        {
            removeGem();
        }
    }
    else if (_type >= shield && _type <= shield4)
    {
        if(state==1)
        {
            _spr->setTexture("shield3.png");
            _type = shield3;
        }
        else if(state==2)
        {
            _spr->setTexture("shield2.png");
            _type = shield2;
        }
        else if(state==3)
        {
            _spr->setTexture("shield1.png");
            _type = shield1;
        }
        else if(state==4)
        {
            _spr->setTexture("shield.png");
            _type = shield;
        }
        else if(state==5)
        {
            
        }
    }
    else if (_type == babydragon)
    {
        if(state==4)
        {
            _spr->setTexture("dragonegg.png");
        }
        else if(state==5)
        {
            auto animationC = Animation::create();
            
            for (int i = 0; i < 7; i++)
            {
                __String *file = __String::createWithFormat("babydragon_%d.png",i);
                
                auto sprite = Sprite::create(file->getCString());
                animationC->addSpriteFrame(sprite->getSpriteFrame());
            }
            
            animationC->setDelayPerUnit(0.12);
            animationC->setRestoreOriginalFrame(false);
            auto actionC = Animate::create(animationC);
            _spr->runAction(actionC);
        }
    }
    else if (_type == magicscrolls)
    {
        if(state==4)
        {
            _spr->setTexture("magicscrolls1.png");
        }
        else if(state==5)
        {
            _spr->setTexture("magicscrolls2.png");
        }
    }
    else if (_type == lifttree)
    {
        if(state==4)
        {
            _spr->setTexture("lifetree1.png");
        }
        else if(state==5)
        {
            _spr->setTexture("lifetree2.png");
        }
        
    }
    else if (_type == stormwind)
    {
        if(state==4)
        {
            _spr->setTexture("");
        }
        else if(state==5)
        {
            _spr->setTexture("");
        }
    }

}

void Ice::idle()
{
    if(_spr->getNumberOfRunningActions()>0||this->getNumberOfRunningActions()>0) return;
    if(_state==2)
    {
//        this->runAnimition(NULL, (void*)kIce_idle1);
    }
    if(_state==1)
    {
//        this->runAnimition(NULL, (void*)kIce_idle2);
    }
    if(_state==0)
    {
//        this->runAnimition(NULL, (void*)kIce_idle3);
    }
}
