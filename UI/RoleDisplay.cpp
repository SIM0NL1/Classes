//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/07/21;
//@功能描述①: 角色系统展示界面;
//@功能描述②: 用于角色展示，选择，升级，查看等功能;
//@功能描述③: 场景类，提供切换、调用接口,支持弹窗;
//**************************************************************

#include "RoleDisplay.h"

RoleDisplay::RoleDisplay()
{

}

Scene* RoleDisplay::createScene()
{
	do 
	{
		Scene* scene = Scene :: create();
		CC_BREAK_IF(!scene);

		RoleDisplay* layer = RoleDisplay::create();
		CC_BREAK_IF(!layer);

		scene->addChild(layer);

		return scene;
	} while (false);
	return nullptr;
}

bool RoleDisplay::init()
{
	do 
	{
		CC_BREAK_IF(!Layer::init());

		showUI();

		return true;
	} while (false);
	return false;
}

void RoleDisplay::showUI()
{
	//展示UI;

}

