#ifndef __DRAGONEGGTRIPLE_GEM_H__
#define __DRAGONEGGTRIPLE_GEM_H__

#include <iostream>
#include "cocos2d.h"
/*#include "cocos-ext.h"*/
#include <vector>
#include "DataTools.h"
#include "GemAction.h"
#include "mapInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

USING_NS_CC ;
/*USING_NS_CC_EXT;*/
using namespace std;


class Gem: public Node
{
protected:
    Label*                 _countLabel;
    
    Sprite*                      _labelBackGround;
    
    Vector<FiniteTimeAction*> _vecActions;
    
    static Gem                ***_gemStoneMatrix;
    
    static MapInfo              *_mapInfo;
    
    static   Node*             _particleNode;//效果节点
    
    static   Node*             _mapNode;//地图节点
    
    static int                  **_mapMatrix;
    
    static vector<AnimationWraper>* _animationWraperVector;
    
    static vector<MyPoint>*         _emptyPointVector;
    
    static vector<ConnectionArea>* _connectionAreaVector;
    
    static int                  _grassguyCount;
    
    static vector<Gem*>         _vecRemoveGem;
    
    GemType                     _type;
    
    GemSkill                    _skill;
    
    GemSkill                    _nextskill;
    
    bool                        _autoSkill;
    
    int                         _state;  //对于特殊元素，state值根据元素不同有不同含义，区别对待,对于所有元素， -1时为可消除
    
    int                         _frozen;  //冰冻 0无冰冻，-1可消
    
    int                         _restrain;//束缚 0无束缚，-1可消
    
    int                         _roots;//束缚 0无束缚，-1可消
    
    int                         _chain;//束缚 0无束缚，-1可消
    
    int                         _iceFloor;
    
    ChangeState                 _change;//0，代表本轮无变化 1,代表本轮相连，2，代表本轮被影响，
    
    int                         _score;//收集元素加倍数,默认为1
    
    bool                        _sick;
    
    int                         _flag;
    
    int                         _scoreSum;//得分提示
    
    GemKind                     _GemKind;
    
    Sprite*                     _spr;// bi中读取的元素
    
    Sprite*                     _frozenSprite;//冰冻节点
    
    Sprite*                     _rootsSprite;
    
    Sprite*                     _chainSprite;
    
    Armature*                     _reminderSprite;
    
    Sprite*                     _rangeSprite;
    
    Sprite*                     _starSprite;
    
    Sprite*                     _skillSprite;
    
    Node*                       _restrainNode;//束缚节点
    
    Color3B                     _particleColor1;
    
    void                        createGemSpriteByName( Node *layer, Point point,bool visible,const char* name);
    
    bool                        _skillAnimation;
    
    int                         _skAnimaID;
    
    bool                        _sameAndSkill;
    
    vector<Gem*>                _vecRightGem;
    
    vector<Gem*>                _vecLeftGem;
    
    vector<Gem*>                _vecUpGem;
    
    vector<Gem*>                _vecDownGem;
    
    int                         _endGem;
    
    bool                        _removeAlready;
    
public:
    Gem();
    
    ~Gem();
    
    GemType                     getGemType();//取水果类型返回
    
    void                        setGemType();
    
    GemSkill                    getGemSkill();
    
    void                        setGemSkill(GemSkill sk);
    
    void                        setNextSkill(GemSkill sk);
    
    void                        changeSkill(){_skill = _nextskill; _nextskill = SkillNull;}
    
    GemSkill                    getNextSkill(){return _nextskill ;}
    
    bool                        getAutoSkill();
    
    void                        setAutoSkill(bool b);
    
    bool                        getRemoveAlready(){return _removeAlready;}
    
    void                        setRemoveAlready(bool b){_removeAlready = b;}
    
    void                        setSkillSpr();
    
    void                        displaySkill(Node *pSender);
    
    void                        sameDisplay(Node *pSender);
    
    void                        removeSkillSpr();
    
    void                        showSkillSpr();
    
    void                        breakIceFloor(int i,int j);
    
    void                        setSkillAnimation(bool b);
    
    bool                        getSkillAnimation(){return _skillAnimation;}
    
    void                        showSkillAnimation(Node *pSender);
    
    void                        setSkAnimaByID(int index  ){_skAnimaID = index ;}
    
    int                         getSkAnimaByID( ){return _skAnimaID ;}
    
    void                        setSameAndSkill(bool b  ){_sameAndSkill = b ;}
    
    bool                        getSameAndSkill(){return _sameAndSkill ;}
    
    void                        showSkAnimaByID();
    
    int                        getEndGem(){return _endGem;}
    
    void                        setEndGem(int s){_endGem = s;}
    
    bool                        getSick(){return _sick;}
    
    virtual void                setSick(bool s);
    
    void                        setChange(ChangeState cs){_change = cs;}
    
    void                        addChange(int count=1);
    
    ChangeState                 getChange(){return _change;}
    
    int                         getState(){return _state;}
    
    void                        setFlag(int f){_flag = f;}
    
    int                         getFlag(){return _flag;}
    
    void                        setState(int s){_state=s;}
    
    virtual void                addScore(int count,bool isAffect);//如果count为0,则清空score,否则累加
    
    int                         getScore(){return _score;}
    
    void                        setScore(int s){_score = s;}
    
    void                        addScore(int s){_score+=s;}
    
    void                        setScoreSum(int s){_scoreSum = s;}
    
    int                         getScoreSum(){return _scoreSum;}
    
    void                        showScore();
    
    void                        hideScore();
    
    void                        addCollectGemScore();
    
    void                        addCollectGemScoreUpdate();
    
    virtual bool                equal(Gem* gem);
    
    bool                        hasSkill(Gem * gem );
    
    static Gem*               createRandomGem(Node *layer,Point point,bool visible=true);
    
    static Gem*               createFixGem(GemType type,Node *layer,Point point,bool visible=true);//创建固定水果，根据type产生 i,j让水果知道自己在那
    
    static Sprite*              createNodeFromFile(Node *layer,Point point,const char* name);
    
    virtual bool                createGemSprite(Node *layer,Point point,bool visible)=0;
    
    static  void                setMapInfo(Node* particleNode,Node* mapNode,Gem ***matrix,MapInfo* mapInfo,vector<AnimationWraper>* aw,vector<MyPoint>* pVector,vector<ConnectionArea>* connectVector,int** mapMatrix);
    
    static void                 frozenerAndRestrainerPerform();
    
    void                        addFrozen(int frozen);
    
    void                        addRoots(int roots);
    
    void                        addChain(int chain);
    
    void                        addRestrain();
    
    GemKind                   getGemKind(){return _GemKind;}
    
    int                         getFrozen(){return _frozen;}
    int                         getRestrain(){return _restrain;}
    
    void                        addCollectGem();
    
    virtual bool                canMove(); //元素 能否移动
    
    virtual void                removeCollect(const Point& dist,float time,bool playMusic);
    
    virtual void                removeNoCollect(bool playMusic);
    
    void                        removeGem();
    
    static int                  getUniqueNumber(int max);
    
    MyPoint                     getCurrentIndex(const Point& point);
    
    long                         numbersOfSprRunningAction();
    
    Color3B                   getColor3B(){return _particleColor1;}
    //===================================================================
    virtual void                highLight();//提示时动作
    
    virtual void                stopHighLight();//停止提示
    
    virtual void                idle();//空闲时动作
    
    virtual void                selected();//选中时动作
    
    virtual void                unselected();//选中时动作
    
    virtual void                fallDownStart(); //下落开始
    
    virtual void                fallDownEnd(Node *pSende , bool playMusic); //下落结束
    
    void                        sendFallDownEndMSG();
    
    virtual void                zhayan();
    
    virtual void                becomeSick();//生病
    
    virtual void                becomeWell();//病好
    
    void                        bright();//消除时候或者在肥沃土地上时候执行的被影响时间轴
    
    void                        brightCallback();
    
    void                        frozenOff();
    
    void                        frozenOffCallback();
    
    void                        rootsOff();
    
    void                        rootsOffCallback();
    
    void                        chainOff();
    
    void                        chainOffCallback();
    
    void                        restrainOff();
    
    void                        restrainOffCallback();
    
    void                        showRange();
    
    void                        hideRange();
    //===================================================================
    virtual void                stopSpritAction(){this->stopAllActions();}//停止所有动作
    
    void                        swap(Direction direction,bool reverse,bool isStart, CallFunc* callbackAction);
    
    void                        swapCallback();
    
    void                        addAction( FiniteTimeAction* action);
    
    void                        fallDownToEnd(bool playMusic);
    
    void                        clearAllAction();
    
    void                        runAnimition( Node* sender,void* animitionName);
    
    void                        showSprite(){this->setVisible(true);}
    
    void                        hideSprite(){this->setVisible(false);}
    
    long                         getActionVectorSize(){return _vecActions.size();}
    
    //====================================================================
    //消除效果
    void                        explosionParticle();
    
    void                        fiveMatchEffect( Node* sender);
    
    void                        fourMatchEffect( Node* sender);
    
    void                        winnerModeStart();
    
    void                        winnerMode();
    
    //=====================================================================
    
    virtual void                explode(MyPoint &myPoint,MyPoint &distPoint,int count,int index);//元素消除
    
    virtual void                affected(int count);//被影响
    
    virtual void                beforeMatch(int i,int j);//消除之前预处理
    
    virtual void                afterMatch(int i,int j);//消除之后收尾
    
    virtual void                growOnFertile(int i,int j);
    
    //道具
    void                        propSingle();
    
    void                        playExplodeRandomMusic( Node* sender);
    
    void                        showSelf();
    
    void                        hideSelf();
    
    void                        removeSelf(Node *pSender);
    
    void                        removeSameAndSkill();
    
    void                        removeSkillNode(Node *pSender);
    
    void                        starBright(Node *pSender);
    
    const char *                brightName(Gem *gem);
    
    void                        updateState();
    
    void                        addHudun();
    
    void                        explodeAll(Node *pSender);
    
    void                        gemBright(Node *pSender);
    
    void                        gemBrightStar(Node *pSender);
    
    void                        gemBrightOver(Node *pSender ,Gem *gem);
    
    void                        dealWithSkill();
    
    void                        dealWithGem();
    
    void                        nextSkill(Node *pSender);
    
    void                        removeSameGem(Node *pSender ,GemType type);
    
    void                        removeSameSkill();
    
    void                        createSkill();
    
    void                        triggerSkill();
    
    void                        beforeExplode();
    
    void                        matchAll();
    
    void                        afterExplode();
    
    void                        sameSkillOver(Node *pSender ,GemType type);
    
    void                        aroundAnimation(Node *pSender);
};

class RedGem : public Gem
{
public:
    RedGem();
    bool                        createGemSprite(Node *layer, Point point,bool visible);
};

class BlueGem : public Gem
{
public:
    BlueGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class GreenGem : public Gem
{
public:
    GreenGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class WhiteGem : public Gem
{
public:
    WhiteGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class YellowGem : public Gem
{
public:
    YellowGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class PurpleGem : public Gem
{
public:
    PurpleGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

class OrangeGem : public Gem
{
public:
    OrangeGem();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
};

 class Ice : public Gem
 {
 public:
 	Ice(GemType type);
 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
 	bool                        canMove();
 	bool                        equal(Gem* Gem);
 	void                        setSick(bool s){}
 	void                        highLight(){};
 	void                        addScore(int count,bool isAffect){}
 	void                        idle();
 	void                        beforeMatch(int i,int j);
 	void                        affected(int count);
 	void                        changeState(int state);
 	void                        selected(){};
 };

//
 class DragonTooth : public Gem
 {
 public:
 	DragonTooth();
 	bool                        createGemSprite( Node *layer,  Point point,bool visible);
 	bool                        canMove();
 	bool                        equal(Gem* Gem);
 	void                        beforeMatch(int i,int j);
 	void                        setSick(bool s){}
 	void                        addScore(int count,bool isAffect){}
 	void                        idle(){}
};

class Bottle : public Gem
{
public:
    Bottle(GemType type);
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
    bool                        canMove();
    bool                        equal(Gem* Gem);
    void                        beforeMatch(int i,int j);
    void                        setSick(bool s){}
    void                        addScore(int count,bool isAffect){}
    void                        idle(){}
};

class Rod : public Gem
{
public:
    Rod();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
    bool                        canMove();
    bool                        equal(Gem* Gem);
    void                        beforeMatch(int i,int j);
    void                        setSick(bool s){}
    void                        addScore(int count,bool isAffect){}
    void                        idle(){}
};

class Pumpkin : public Gem
{
public:
    Pumpkin();
    bool                        createGemSprite( Node *layer,  Point point,bool visible);
    bool                        canMove();
    bool                        equal(Gem* Gem);
    void                        beforeMatch(int i,int j);
    void                        setSick(bool s){}
    void                        addScore(int count,bool isAffect){}
    void                        idle(){}
};

#endif  //__DRAGONEGGTRIPLE_GEM_H__