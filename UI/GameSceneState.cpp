//
//	GameSceneState.cpp
//	Author:Simon
//	Date:  2015.5.22
//	本类作为场景切换的记录;
//

#include "GameSceneState.h"
#include "GameEnter.h"
#include "GameMain.h"
#include "GameMissionSet.h"
#include "GameUIData.h"
#include "../GameUILayer.h"
#include "GameMusicControl.h"
#include "RoleDisplay.h"

GameSceneState* GameSceneState::m_self;
GameSceneState::GameSceneState()
{
	m_statePrevious = SceneState :: UIGameNone;		//祖状态;
	m_stateLast = SceneState :: UIGameNone;			//父状态;
	m_stateNow = SceneState :: UIGameNone;			//现状态;
}

GameSceneState::~GameSceneState()
{

}

GameSceneState* GameSceneState::getInstance()
{
	if (!m_self)
	{
		m_self = new GameSceneState();
	}
	return m_self;
}

void GameSceneState::destroyInstance()
{
	if (m_self)
	{
		delete(m_self);
	}
	m_self = nullptr;
}

void GameSceneState::switchScene(SceneState state,float t/*=0*/)
{
	Scene* scene = nullptr;
	switch (state)
	{
        case SceneState::UIGameEnter:
        {
			scene = CREATE_SCENE(GameEnter);
			break;
		}
        case SceneState::UIGameMain:
        {
			scene = CREATE_SCENE(GameMain)
			break;
		}
        case SceneState::UIGameMissionSet:
		{
			scene = CREATE_SCENE(GameMissionSet);
			break;
		}
        case SceneState::DDGameUILayer:
        {
            scene = GameUILayer::gameScene();
            break;
        }
		case SceneState::UIRoleDisplay:
		{
			scene = CREATE_SCENE(RoleDisplay);
			break;
		}
        default:break;
	}

	//更新状态;
	m_statePrevious = m_stateLast;
	m_stateLast = m_stateNow;
	m_stateNow = state;

	replaceScene(scene,t);
}

void GameSceneState::replaceScene(Scene* scene,float t)
{
    replaceSpecial();
	if (t)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(t,scene));
	}
	else
	{
		Director::getInstance()->replaceScene(scene);
	}
}

SceneState GameSceneState::getPreviousState() const
{
	return m_statePrevious;
}

SceneState GameSceneState::getLastState() const
{
	return m_stateLast;
}

SceneState GameSceneState::getNowState() const
{
	return m_stateNow;
}

//切换场景时,需要做的一些特殊处理;
void GameSceneState::replaceSpecial()
{
    if (m_stateLast == SceneState::UIGameMain)
    {
		//关卡定位,地图模式定位;
        float offset = g_pGameMain->m_pPageView->pTableView->tableView->getContentOffset().y;
        GameUIData::getInstance()->setVerticalGps(offset);
        int index = g_pGameMain->m_pPageView->getCurPageIndex();
        GameUIData::getInstance()->setVerticalIndex(index);
    }
    else if (m_stateLast == SceneState::DDGameUILayer)
    {
		//背景音乐切换;
        GameMusicControl::getInstance()->musicOn();
    }
}
