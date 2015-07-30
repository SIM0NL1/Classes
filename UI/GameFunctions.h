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

//内联函数-计算坐标;
inline cocos2d::Vec2 vecSub (const cocos2d::Vec2& v1,const cocos2d::Vec2& v2,float scale) {return Vec2(v1.x-scale*v2.x,v1.y-scale*v2.y);}
inline cocos2d::Vec2 vecSub (const cocos2d::Vec2& v1,const cocos2d::Vec2& v2) {return Vec2(v1.x-v2.x,v1.y-v2.y);}
inline cocos2d::Vec2 vecAdd (const cocos2d::Vec2& v1,const cocos2d::Vec2& v2) {return Vec2(v1.x+v2.x,v1.y+v2.y);}
inline cocos2d::Vec2 vecMid (const cocos2d::Vec2& v1) {return Vec2(v1.x*0.5f,v1.y*0.5f);}
inline cocos2d::Vec2 vecMid (const cocos2d::Vec2& v1,const cocos2d::Vec2& v2) {return Vec2(v1.x*0.5f+v2.x*0.5f,v1.y*0.5f+v2.y*0.5f);}
inline cocos2d::Vec2 vecMid (const Node* sender) {return Vec2(sender->getContentSize().width*0.5f,sender->getContentSize().height*0.5f);}

//函数接收调用的函数指针宏;
typedef std::function<void()> parameter_0_type;
typedef std::function<void(int&)> parameter_1_int;

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
