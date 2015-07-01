//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/06/03;
//@功能描述①: GameMissionSet.h;
//@功能描述②: 从关卡进入游戏，配置数据，配置角色;
//@功能描述③: UI功能类;
//**************************************************************

#ifndef __GameMissionSet_H__
#define __GameMissionSet_H__

#include "GameFunctions.h"

enum GameMissionSetBtnTag
{
	GameMissionSetBtnStart,
	GameMissionSetBtnExit,
};
class GameMissionSet : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(GameMissionSet);
	void showUI();
	Button *m_btnStart,*m_btnExit;
	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	void onBtnStart();
	void onBtnExit();


private:
	GameMissionSet();
	virtual ~GameMissionSet();
	virtual bool init();
};

#endif