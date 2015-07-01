//
//  GameUILayer.h
//  Dragon
//
//  Created by B081mac on 15/4/14.
//
//

#ifndef __Dragon__GameUILayer__
#define __Dragon__GameUILayer__

#include <iostream>
#include "cocos2d.h"
#include "DataTools.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace std;

#define COLLECT_ARRAY 3

typedef enum
{
    layerPause=0,
    layerTime,
    layerWin
}LAYER_TYPE;

class GameUILayer : public Layer
{
public:
    
    GameUILayer();
    ~GameUILayer();
    
    static Scene* gameScene();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void initBG();
    
    void initTop();
    
    void initData();
    
    void targetNumAndDisplay(int index,GemType type , int num);
    
    void pauseMenu(Ref* pSender);
    
    void continueGame(Ref* pSender,Node* pNode);
    
    void restarGame(Ref* pSender,Node* pNode);
    
    void quitGame(Ref* pSender);
    
    void resumeGame();
    
    void pauseGame();
    
    void updateOperationNum(Ref* obj);
    
    void operationAnimaOver();
    
    void hideOperation(Node* pNode);
    
    void matchOver(Ref* obj);
    
    void updateCollect(Ref* obj);
    
    void failGame();
    
    void limitOperatiomNum();
    
    void limitTime(float dt);
    
    void addBlackLayer(LAYER_TYPE type);
    
    void addTimeDown(Node* pSender , Node * pLayer);
    
    void failLayer(Node* pSender , Node * pLayer);
    
    void add5Step(Ref *pSender, Node *pNode);
    
    void winnerMode(Node *pSender);
    
    void completeWords(Node *pSender);
    
    void gameLayerFadeOut(Node *pSender ,float times);
    
    void gameLayerFadeIn(Node *pSender  ,float times);
    
//    void gameLayerSetOpacity(Node *pSender);
    
    void addWinLayer();
    
    void showTarget();
    
    void displayTarget(GemType type , int num ,Node * pLayer ,int index);
    
    void removeTargetLayer(Node* pSender , Node * pLayer);
    
    void initTimeLimit();
    
    //添加Boss;
    void addBoss();
    //Boss死亡;
    void bossDied(Ref *obj);
    
    void showLevel();
    
    void removeLevel(Node *pSender ,Node * pLayer);
    
    void beforeStarLimit(Node *pSender , int times);
    
    CREATE_FUNC(GameUILayer);
    
private:
    GameLayer *m_pGameLayer;
    
    bool m_bWinGame;
    
    int m_iOperationNum;
    int m_iScoreNum;
    int m_iOneScore;
    int m_iLevel;
    int m_iScoreTarget;
    map <GemType , int> m_mapTarget;
    
    LabelAtlas *m_labelOperation;
    LabelAtlas *m_labelOperation1;
    LabelAtlas *m_labelScore;
    LabelAtlas *m_labelScoreTarget;
    
    LabelAtlas *m_labelTarget[COLLECT_ARRAY];
    Sprite *m_sprTarget[COLLECT_ARRAY];
    Sprite *m_sprAchieve[COLLECT_ARRAY];
    Armature *m_targetBG;
    bool m_bComplete[COLLECT_ARRAY];  //finished
    
    int m_iArrTargetNum[maxgem];
    int m_iArrTargetIndex[maxgem];
};


#endif /* defined(__Dragon__GameUILayer__) */
