//
//  Progress.cpp
//  Dragon
//
//  Created by B081mac on 15/5/15.
//
//

#include "Progress.h"

bool Progress::init(const char* background, const char* fillname, bool vertical)
{
    this->initWithFile(background);
    ProgressTimer* fill = ProgressTimer::create(Sprite::create(fillname));
    this->setFill(fill);
    this->addChild(fill);
    
    _fHeight = this->getBoundingBox().size.height;
    
    fill->setType(ProgressTimer::Type::BAR);
    
    if (vertical)
    {
        fill->setMidpoint(Point(0.5,0));
        fill->setBarChangeRate(Point(0, 1.0));
    }
    else
    {
        fill->setMidpoint(Point(0,0.5));
        fill->setBarChangeRate(Point(1.0, 0));
    }
    
    
    fill->setPosition(this->getContentSize()/2);
    fill->setPercentage(100);
    return true;
}

Progress* Progress::create(const char* background, const char* fillname, bool vertical)
{
    Progress* progress = new Progress();
    if(progress && progress->init(background,fillname,vertical))
    {
        progress->autorelease();
        return progress;
    }
    else
    {
        delete progress;
        progress = NULL;
        return NULL;
    }
}