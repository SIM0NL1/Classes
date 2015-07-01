//**************************************************************
//@创建者:   Simon;
//@创建时间: 2015/05/29;
//@功能描述①: ;
//@功能描述②: ;
//@功能描述③: ;
//**************************************************************

#ifndef __GameEnter_H__
#define __GameEnter_H__

#include "GameFunctions.h"

class GameEnter : public Layer
{
public:
    GameEnter();
    ~GameEnter();
    
    static Scene* createScene();
    CREATE_FUNC(GameEnter);
    virtual bool init();
    void enterGame();
    //加载数据和资源;
    void loadJsonData();
    void loadArmature();
	void loadMyArmature();
};

#endif