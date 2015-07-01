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

class GameFunctions : public Ref
{
public:
	static GameFunctions* getInstance();
	static void deleteGameFunctions();
	const char* readResourcesPath(string fileName);
	string g_sChineseName;
    CC_SYNTHESIZE(float,verticalOffset,VerticalOffset);

    template<typename T>
    void vertexZ(T** pSender,bool flag=true)
    {
        if (flag)
        {
            //(*pSender)->setPositionZ(Z_Second);
        }
        else
        {
            //(*pSender)->setPositionZ(Z_First);
        }
        
    }

    
private:
	GameFunctions();
	virtual ~GameFunctions();
	static GameFunctions* m_self;
	string m_sFilePath;
};
#endif
