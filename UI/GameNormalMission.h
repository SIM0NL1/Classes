//
//	GameNormalMission.h
//	Author:Simon
//	Date:  2015.5.18
//	本类作为普通关卡节点父类
//	定义关卡基本属性：继承layout 坐标，状态，动画，按钮，图片，数字
//

#ifndef __GameNormalMission_H__
#define __GameNormalMission_H__

#include "GameFunctions.h"

class GameNormalMission : public Layout
{
public:

	CREATE_FUNC(GameNormalMission);
	Button *m_btnDizuo,*m_btnStone;
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
	//底座按钮事件
	void onBtnDizuo();
	//巨石按钮事件
	void onBtnStone();
	//关卡进度，状态调用接口
	void setMissionPorperty(int id);
	//关卡星级调用接口，星星数量需要在配置文件中获取
	void setMissionStartNum(int missionId,int startNum);
	void missionShow(int missionId);

private:
	GameNormalMission();
	~GameNormalMission();
	virtual bool init();
	Size m_dizuoSize;
	Size m_stoneSize;
	int missionStartNum;
};

#endif