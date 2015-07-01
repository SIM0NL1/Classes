//
//	GameChallengeMission.h
//	Author:Simon
//	Date:  2015.5.21
//	本类作为特殊关卡节点父类
//	定义关卡基本属性：继承layout 坐标，状态，动画，按钮，图片，数字
//

#ifndef __GameChallengeMission_H__
#define __GameChallengeMission_H__

#include "GameFunctions.h"

class GameChallengeMission : public Layout
{
public:

	CREATE_FUNC(GameChallengeMission);
	Button *m_btnCandle;
	Label* m_labMissionId;

	//设置关卡UI
	void initMission();
	//设置关卡状态:正常/禁用
	void setMissionState(GameMissionState state);
	//关卡开启
	void missionOpen();
	//当前关卡
	void missionNow();
	//按钮监听回调的统一接口
	void BtnCall(Ref* pSender,Widget::TouchEventType type);
	//设置按钮是否可以点击
	void setBtnTouch(bool flag);
	//蜡烛按钮事件
	void onBtnCandle();
	//关卡进度，状态调用接口
	void setMissionPorperty(int id);
	//关卡星级调用接口，星星数量需要在配置文件中获取
	void setMissionStartNum(int missionId,int startNum);
	void missionShow(int missionId);

private:
	GameChallengeMission();
	~GameChallengeMission();
	virtual bool init();
	int missionStartNum;
	Size m_size;
};

#endif