//
//  GameFunctions.h
//  author:Simon
//
//  Created by 5agame on 15/5/14.
//  实现项目中公共的方法，这是一个工具类;
//  单例实现;

#ifndef XXL_GameFunctions_h
#define XXL_GameFunctions_h

#include "GameDefine.h"
#include "GameSceneState.h"

//切换场景;
#define SCENE_CHANGE_NORMAL(myscene) GameSceneState :: getInstance()->switchScene(myscene);
#define SCENE_CHANGE_FADE(myscene) GameSceneState :: getInstance()->switchScene(myscene,0.5f);

//读取资源;
#define RESOURCE(_STR_PATH_) GameFunctions::getInstance()->readResourcesPath(_STR_PATH_)
#define RESOURCETEST(_STR_PATH_) GameFunctions::getInstance()->readResourcesPathTest(_STR_PATH_)

//场景创建函数;
#define CREATE_SCENE(_CLASS_NAME_) _CLASS_NAME_::createScene();

class GameFunctions : public Ref
{
public:
	static GameFunctions* getInstance();
	static void deleteGameFunctions();
	const char* readResourcesPath(string fileName);
	const char* readResourcesPathTest(int fileName);
	string g_sChineseName;
    CC_SYNTHESIZE(float,verticalOffset,VerticalOffset);
	string soldierTypeName[5][5];
	//初始化不同角色和等级的士兵UI;
	void initSoldierType();
	//大地图界面按钮与关卡按钮回调屏蔽;
	CC_SYNTHESIZE(bool ,isUIBtnCallBack,IsUIBtnCallBack);

private:
	GameFunctions();
	virtual ~GameFunctions();
	static GameFunctions* m_self;
	string m_sFilePath;
};
#endif
