#ifndef __DRAGONEGGTRIPLE_MAPMATRIX_H__
#define __DRAGONEGGTRIPLE_MAPMATRIX_H__

#include <iostream>
#include "cocos2d.h"
#include "Gem.h"
#include "GemAction.h"
#include <vector>
#include "MapInfo.h"

USING_NS_CC;

class MapMatrix
{
public:
    
    MapMatrix(int **mm,Gem ***fm,int count,MapInfo* mapinfo);
    
    ~MapMatrix();
    
    std::vector<ConnectionArea> connectionAreaVector;
    
    std::vector<PotentialArea> potentialAreaVector;
    
    void  firstTimeInit(int *fTypeArray,int count,Layer* layer);//首次初始化确保无相连
    
    void  findConnection();//找相连区域，放入队列
    
    void  findPotential();//找潜在相连区域，放入队列
    
    void  fillEmptyNew(Layer* layer,int i,int j);
    
    void  changeGemState(int step,std::map<GemType,int>& levelMap);
    
    SpriteBatchNode*  createMapFrame(Node* mapNode);//画边框
    
    void  clearCounter();
    
    void  markeMatrixChange();
    
    void  dealSick();
    
    void  findCenterPoint(MyPoint& selectPoint,MyPoint& nextPoint);
    
    void  findParticlePoint(MyPoint& selectPoint,MyPoint& nextPoint);
    
    void  tigerEatAnimal(MyPoint& _selectPoint,MyPoint& _nextPoint);
    
    void  matchAllGem(vector<AnimationWraper>* animationWraperVector ,int &continueTimes );
    
    void  growOnFertile();
    
    void  afterMatch();
    
    void  beforeMatchGem();
    
    void  renewGemCount();
    
    void  dealFallDown(Layer* layer);
    
    void  beforeMatchGem(vector<AnimationWraper>* animationWraperVector,MyPoint& selectPoint,MyPoint& nextPoint);
    
    void  dealFallDownColumn(Layer* layer ,int row);
    
private:
    MapInfo* _mapInfo;
    int **_mapMatrix;//地图矩阵
    Gem ***_gemMatrix;//元素矩阵
    int edgeLength;//维度
    int _newGemCounter[kMatrixWidth];
    int _newGemCounterFromMiddle[kMatrixWidth];
    int fallDownFlagArray[kMatrixWidth];
};

class Disjoinset
{
public:
    static Disjoinset &getInstance()
    {
        static Disjoinset instance;
        
        return instance;
    }
    
    int     pre[kMatrixWidth*kMatrixWidth];
    int     rank[kMatrixWidth*kMatrixWidth];
    
    void    makeset();
    int     find(int x);
    void    unionone(int a,int b);
    
    
};

#endif  //__DRAGONEGGTRIPLE_MAPMATRIX_H__