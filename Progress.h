//
//  Progress.h
//  Dragon
//
//  Created by B081mac on 15/5/15.
//
//

#ifndef __Dragon__Progress__
#define __Dragon__Progress__

#include <stdio.h>

#include "cocos2d.h"
USING_NS_CC;

class Progress : public Sprite
{
public:
    bool init(const char* background, const char* fillname, bool vertical);
    /*
     the inputs are SpriteFrame Names.
     they should be loaded into SpriteFrameCache before calling this.
     */
    static Progress* create(const char* background, const char* fill , bool vertical);
    
    void setFill(ProgressTimer* fill){_fill=fill;}
    
    void setProgress(float percentage){_fill->setPercentage(percentage);}
    
    float getHeight(){return _fHeight;}
    
private:
    ProgressTimer* _fill;
    float _fHeight;
};

#endif /* defined(__Dragon__Progress__) */
