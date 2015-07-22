//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/21;
//@功能描述①: 角色系统展示界面;
//@功能描述②: 用于角色展示，选择，升级，查看等功能;
//@功能描述③: 场景类，提供切换、调用接口,支持弹窗;
//**************************************************************

#ifndef __RoleDisplay_H__
#define __RoleDisplay_H__

#include "GameFunctions.h"

class RoleDisplay : public Layer
{
public:
	CREATE_FUNC(RoleDisplay);
	void showUI();

private:
	RoleDisplay();
	static Scene* createScene();
	virtual bool init();

};

#endif