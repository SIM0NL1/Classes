#ifndef __DRAGONEGGTRIPLE_MAPINFO_H__
#define __DRAGONEGGTRIPLE_MAPINFO_H__

#include "cocos2d.h"
#include "DataTools.h"

using namespace std;
USING_NS_CC ;

class MapInfo :public Ref
{
public:
	MapInfo ();
	~MapInfo();

	int** getGemMap();
    int** getMatrixMap();
    
    bool getJsonData();
    
    bool getJsonData(int iLev);
    
    int isFrozenInMap(int i ,int j);
    
    int isRootsInMap(int i ,int j);
    
    int isChainInMap(int i ,int j);
    
    int isIceFloorInMap(int i ,int j);
    
    int isLeafInMap(int i ,int j);
    
    bool isMapTop(int i ,int j);
    
    bool isMapBottom(int i ,int j);
    
    vector <GemType> getGemType();
    map <GemType , int> getMapTarget();
    
    int getOperationNum();
    
    bool isCollectPosition(int i ,int j);
    
    int getPassScore(int index);
    
    int getPassReward(int index);
    
    int getPassTime();
    
private:
    
    vector <GemType> m_vecGemType;
    map <GemType , int> m_mapTarget;
    int m_iOperationNum;
    
    int **m_iArrMatrix;//地图矩阵
    int **m_iArrGemMap;//元素地图
    
    vector<int> m_vecFrozen1;
    vector<int> m_vecFrozen2;

    vector<int> m_vecRoots1;
    vector<int> m_vecRoots2;
    
    vector<int> m_vecChain1;
    vector<int> m_vecChain2;
    
    vector<int> m_vecIceFloor1;
    vector<int> m_vecIceFloor2;
    
    vector<int> m_vecLeaf;
    
    vector<int> m_vecPassScore;
    vector<int> m_vecPassReward;
    int m_iPassTime;
    
};

#endif  //__DRAGONEGGTRIPLE_MAPINFO_H__

