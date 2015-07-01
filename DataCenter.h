//
//  DataCenter.h
//  Dragon
//
//  Created by B081mac on 15/4/14.
//
//

#ifndef __Dragon__DataCenter__
#define __Dragon__DataCenter__

#include <iostream>
#include "cocos2d.h"
#include "DataTools.h"
#include "MapInfo.h"

USING_NS_CC;
using namespace std;

class  DataCenter :public Ref
{
public:
    DataCenter();
    ~DataCenter();
    static DataCenter* getInstance();
    
    void initMapInfo(int level);
    
    MapInfo* getMapInfo();
    
    void destroyMapInfo();
    
    void setAddSteps(int steps);
    
    int getAddSteps();
    
    void setPosStar(Point pos);
    
    Point getPosStar();
    
    void setWinnerMode(bool b);
    
    bool getWinnerMode();
    
    void setTimeLimit(bool b);
    
    bool getTimeLimit();
    
    void initMapPoint(GemType type , Point pos);
    
    Point getGemPoint(GemType type);
    
    map<GemType ,Point > getGemPointMap()
    {
        return m_mapGemPoint;
    }
    
    vector<Gem*> getRemoveGem()
    {
        return m_vecRemoveGem;
    }
protected:
    int m_iAddSteps;
    
    Point m_posStar;
    
    MapInfo *m_pMapInfo;
    
    bool m_bWinnerMode;
    
    bool m_bTimeLimit;
    
    map<GemType ,Point > m_mapGemPoint;
    
    vector<Gem*> m_vecRemoveGem;
    
};


#endif /* defined(__Dragon__DataCenter__) */
