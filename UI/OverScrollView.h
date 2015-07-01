//
//  OverScrollView.h
//  XXL
//
//  Created by 5agame on 15/5/14.
//
//

#ifndef __XXL__OverScrollView__
#define __XXL__OverScrollView__

#include "GameFunctions.h"
#include "OverTableView.h"
class OverScrollView;
class OverTableView;
class OverScrollViewDelegate
{
public:
    OverScrollViewDelegate(){};
    virtual ~OverScrollViewDelegate(){};
    virtual Size sizeForPerPage()=0;
    virtual void pageViewDidScroll(OverScrollView* scrollView){};
};
class OverScrollView : public extension::ScrollView
{
public:
    static OverScrollView* create(OverScrollViewDelegate* delegate);
    virtual bool init(OverScrollViewDelegate* delegate);
    virtual void setContentOffsetInDuration(Vec2 offset,float dt);
    virtual void setContentOffset(Vec2 offset);
    
    EventListenerTouchOneByOne* listener;
    int pageCount;
    
    Node* getPageAtIndex(int index);
    int getCurPageIndex();
    
    virtual bool onTouchBegan(Touch* touch,Event* unused_event);
    virtual void onTouchMoved(Touch* touch,Event* unused_event);
    virtual void onTouchEnded(Touch* touch,Event* unused_event);
    virtual void onTouchCancelled(Touch* touch,Event* unused_event);
    OverTableView* pTableView;
    Armature *qiehuan;
    bool _isChmap;
    Sprite* pChmap;
    void chmapEffect(float offset);
    void horizontalGPS(bool flag=true);
    void verticalGPS();
    void extractFun();
    void simulateBtn();
    
private:
    OverScrollView();
    ~OverScrollView();
    CC_SYNTHESIZE(OverScrollViewDelegate*,_delegate,Delegate);
    //根据手势滑动的距离和方向滚动图层;
    void adjustScrollView(float offset);
    void performedAnimatedScroll(float dt);
    Point m_startPoint,m_endPoint;
    float current_offset;
    int current_index;
 
    
};

#endif /* defined(__XXL__OverScrollView__) */