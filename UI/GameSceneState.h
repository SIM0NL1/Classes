//
//	GameSceneState.h
//	Author:Simon
//	Date:  2015.5.22
//	本类作为场景切换的记录
//

#ifndef __GameSceneState_H__
#define __GameSceneState_H__

#include "GameFunctions.h"

class GameSceneState : public Ref
{
public:
	static GameSceneState* m_self;
	static GameSceneState* getInstance();
	void destroyInstance();
	void switchScene(SceneState state,float t=0);
	void replaceScene(Scene* scene,float t);
    // 切换场景时，需要做的一些特殊处理;
    void replaceSpecial();
	SceneState getPreviousState()const;
	SceneState getLastState()const;
	SceneState getNowState()const;

private:
	GameSceneState();
	~GameSceneState();
	SceneState m_statePrevious,m_stateLast,m_stateNow;
};

#endif