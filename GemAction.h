#ifndef __DRAGONEGGTRIPLE_GEMACTION_H__
#define __DRAGONEGGTRIPLE_GEMACTION_H__

#include <iostream>
#include "cocos2d.h"
#include "Gem.h"
#include "DataTools.h"

USING_NS_CC;

class GemAction  :public Ref
{
public:
    
    
    static GemAction &getInstance()
    {
        static GemAction instance;
        return instance;
    }
    
    ActionInterval *getGemMoveAction(Direction direction);//下落移动
    
    FiniteTimeAction *getGemRenewAction(MyPoint distination);//无潜在更新所有
    
    FiniteTimeAction *getGemMoveToAction(MyPoint distination);//无延时移动
    
    ActionInterval *getGemChangeAction(Direction direction);//交换移动
    
    ActionInterval *getCollectGemAction(const Point& startPoint,const Point& distPoint,float time);
    
    ActionInterval *getConvergeMoveAction(Point point);
    
    Point           getControlPoint(Point point1,Point point2,int distance);
    Point           getControlPointSingle(Point point1,Point point2,int distance);
    
    static void        setBright(Node* sender);
    static void        removeBright(Node* sender);
    //音效
    // 	void playBackgroudMusic(CCNode* sender,void* data);
    //
    // 	void stopBackgroudMusic();
    //
    void playEffectMusic(Node* pSender ,std::string file);
    //
    // 	void switchBackgroudMusic();
    
    
private:
    
    GemAction ();
    ~GemAction ();
    static GLProgram* _glProgramBright;
    static GLProgram* _glProgramNormal;
    
    char* _musicPath;
    DISALLOW_COPY_AND_ASSIGN(GemAction);
};

#endif  //__DRAGONEGGTRIPLE_GEMACTION_H__

