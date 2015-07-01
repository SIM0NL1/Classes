#include "MapInfo.h"
#include "cocostudio/CocoStudio.h"
#include <time.h>

MapInfo::MapInfo(void)
{
    m_iArrMatrix = (int **)malloc(sizeof(int*)*kMatrixWidth);
    for(int i = 0;i<kMatrixWidth;i++)
    {
        m_iArrMatrix[i] = (int*)malloc(sizeof(int)*kMatrixWidth);
    }
    
    m_iArrGemMap = (int **)malloc(sizeof(int*)*kMatrixWidth);
    for(int i = 0;i<kMatrixWidth;i++)
    {
        m_iArrGemMap[i] = (int*)malloc(sizeof(int)*kMatrixWidth);
    }
}


MapInfo::~MapInfo(void)
{
    if(0 != m_iArrMatrix)
    {
        for(int i = 0;i<kMatrixWidth;i++)
        {
            free(m_iArrMatrix[i]);
        }
    }
    free(m_iArrMatrix);
    
    if(0 != m_iArrGemMap)
    {
        for(int i = 0;i<kMatrixWidth;i++)
        {
            free(m_iArrGemMap[i]);
        }
    }
    free(m_iArrGemMap);
}

bool MapInfo::getJsonData(int iLev)
{
    string filename="MapInfoTest.json";
    rapidjson::Document doc;
    //判断文件是否存在
    if(!FileUtils::getInstance()->isFileExist(filename))
    {
        log("json file is not find [%s]",filename.c_str());
        return false;
    }
    //读取文件数据，初始化doc
    std::string data=FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //判断读取成功与否 和 是否为数组类型
    if (doc.HasParseError() || !doc.IsArray())
    {
        log("get json data err!");
        return false;
    }
    
    if (iLev<doc.Size())
    {
        rapidjson::Value &v=doc[iLev];
        int index = 1;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecGemType.push_back((GemType)v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                GemType type = (GemType)v[index][i].GetInt();
                int num = v[index][i + 1].GetInt();
                
                m_mapTarget.insert(make_pair(type , num));
                i++;
            }
        }
        index++;
        
        m_iOperationNum = v[index++].GetInt();
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecFrozen1.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecFrozen2.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecRoots1.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecRoots2.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecChain1.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecChain2.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecIceFloor1.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecIceFloor2.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecLeaf.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                int iX = i/kMatrixWidth;
                int iY = i%kMatrixWidth;
                m_iArrMatrix[iX][iY] = v[index][i].GetInt();
            }
        }
    }
    else
    {
        log("yue jie ");
        return false;
    }
    //从第2行开始，因为第一行是属性
//    for(unsigned int i=1;i<doc.Size();i++)
//    {
//        //逐个提取数组元素（声明的变量必须为引用）
//        rapidjson::Value &v=doc[i];
//        
//        int id;//ID
//        string name;//名称
//        int hp;//血量
//        int Defense;//防御力
//        //        int attack;//攻击力
//        //        int passable;//是否可穿透（通行）
//        
//        //按下标提取
//        int a=0;
//        id=v[a++].GetInt();
//        name=v[a++].GetString();
//        hp=v[a++].GetInt();
//        Defense=v[a++].GetInt();
//        //        attack=v[a++].GetInt();
//        if (v[a].IsArray())
//        {
//            for (int i = 0; i < v[a].Capacity(); i ++)
//            {
//                CCLOG("%d" ,v[a][i].GetInt());
//            }
//        }
//        
//        CCLOG("ID:%d,Name:%s,Blood:%d,Atrack:%d",
//              id,name.c_str(),hp,Defense);
//    }

    return true;

}

bool MapInfo::getJsonData()
{
    string filename="TimeLimitData.json";
    rapidjson::Document doc;
    //判断文件是否存在
    if(!FileUtils::getInstance()->isFileExist(filename))
    {
        log("json file is not find [%s]",filename.c_str());
        return false;
    }
    //读取文件数据，初始化doc
    std::string data=FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //判断读取成功与否 和 是否为数组类型
    if (doc.HasParseError() || !doc.IsArray())
    {
        log("get json data err!");
        return false;
    }
    int iLev = 1;
    if (iLev<doc.Size())
    {
        rapidjson::Value &v=doc[iLev];
        int index = 0;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecGemType.push_back((GemType)v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecPassScore.push_back(v[index][i].GetInt());
            }
        }
        index++;
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                m_vecPassReward.push_back(v[index][i].GetInt());
            }
        }
        index++;

        
        m_iPassTime = v[index++].GetInt();
        
        if (v[index].IsArray())
        {
            for (int i = 0; i < v[index].Capacity(); i++)
            {
                int iX = i/kMatrixWidth;
                int iY = i%kMatrixWidth;
                m_iArrMatrix[iX][iY] = v[index][i].GetInt();
            }
        }
    }
    else
    {
        log("yue jie ");
        return false;
    }
    return true;
}


int** MapInfo::getMatrixMap()
{
    return m_iArrMatrix;
}

int** MapInfo::getGemMap()//生成元素地图的矩阵
{
	for(int i = 0;i<kMatrixWidth;i++)
	{
		for(int j = 0;j<kMatrixWidth;j++)
		{
            if (m_iArrMatrix[i][j] == Random /*|| m_iArrMatrix[i][j] == IceFloor || m_iArrMatrix[i][j] == IceFloor1*/)
            {
                m_iArrGemMap[i][j] = arc4random()%m_vecGemType.size() + 1;
            }
            else if (m_iArrMatrix[i][j] == Empty)
            {
                m_iArrGemMap[i][j] = empty;
            }
            else /*if (MAP_TEST[i][j] > 0)*/
            {
                m_iArrGemMap[i][j] = m_iArrMatrix[i][j];
            }
		}
	}
	return m_iArrGemMap;
}

int MapInfo::isFrozenInMap(int i, int j)
{
    int position = i * kMatrixWidth + j;
    if (m_vecFrozen1.size())
    {
        vector<int>::iterator result = find( m_vecFrozen1.begin(), m_vecFrozen1.end(), position );
        if (result != m_vecFrozen1.end())
        {
            return 1;
        }
    }
    if (m_vecFrozen2.size())
    {
        vector<int>::iterator result = find( m_vecFrozen2.begin(), m_vecFrozen2.end(), position );
        if (result != m_vecFrozen2.end())
        {
            return 2;
        }
    }

    return 0;
}

int MapInfo::isRootsInMap(int i, int j)
{
    int position = i * kMatrixWidth + j;
    if (m_vecRoots1.size())
    {
        vector<int>::iterator result = find( m_vecRoots1.begin(), m_vecRoots1.end(), position );
        if (result != m_vecRoots1.end())
        {
            return 1;
        }
    }
    if (m_vecRoots2.size())
    {
        vector<int>::iterator result = find( m_vecRoots2.begin(), m_vecRoots2.end(), position );
        if (result != m_vecRoots2.end())
        {
            return 2;
        }
    }
    
    return 0;
}

int MapInfo::isChainInMap(int i, int j)
{
    int position = i * kMatrixWidth + j;
    if (m_vecChain1.size())
    {
        vector<int>::iterator result = find( m_vecChain1.begin(), m_vecChain1.end(), position );
        if (result != m_vecChain1.end())
        {
            return 1;
        }
    }
    if (m_vecChain2.size())
    {
        vector<int>::iterator result = find( m_vecChain2.begin(), m_vecChain2.end(), position );
        if (result != m_vecChain2.end())
        {
            return 2;
        }
    }
    
    return 0;
}

int MapInfo::isIceFloorInMap(int i, int j)
{
    int position = i * kMatrixWidth + j;
    if (m_vecIceFloor1.size())
    {
        vector<int>::iterator result = find( m_vecIceFloor1.begin(), m_vecIceFloor1.end(), position );
        if (result != m_vecIceFloor1.end())
        {
            return 1;
        }
    }
    if (m_vecIceFloor2.size())
    {
        vector<int>::iterator result = find( m_vecIceFloor2.begin(), m_vecIceFloor2.end(), position );
        if (result != m_vecIceFloor2.end())
        {
            return 2;
        }
    }
    return 0;
}

int MapInfo::isLeafInMap(int i, int j)
{
    int position = i * kMatrixWidth + j;
    if (m_vecLeaf.size())
    {
        vector<int>::iterator result = find( m_vecLeaf.begin(), m_vecLeaf.end(), position );
        if (result != m_vecLeaf.end())
        {
            return 1;
        }
    }
    return 0;
}


bool MapInfo::isCollectPosition(int i, int j)
{
    if (j == 0)
    {
        return true;
    }
    else
    {
        if (m_iArrMatrix[i][j - 1] == 0)
        {
            return true;
        }
    }
    return false;
}

vector <GemType> MapInfo::getGemType()
{
    return m_vecGemType;
}

map <GemType , int> MapInfo::getMapTarget()
{
    return m_mapTarget;
}
int MapInfo::getOperationNum()
{
    return m_iOperationNum;
}

bool MapInfo::isMapTop(int i, int j)
{
    if (j < kMatrixWidth - 1)
    {
        if (m_iArrMatrix[i][j + 1] == NoneInEdge || m_iArrMatrix[i][j + 1] == NoneInMiddle)
        {
            return true;
        }
    }
    else if (j == kMatrixWidth - 1)
    {
        return true;
    }
    return false;
}

bool MapInfo::isMapBottom(int i, int j)
{
    if (j >= 1)
    {
        if (m_iArrMatrix[i][j-1] == 0)
        {
            return true;
        }
    }
    else if(j == 0)
    {
        return true;
    }
    return false;
}

int MapInfo::getPassScore(int index)
{
    if (index < m_vecPassScore.size())
    {
        return m_vecPassScore.at(index);
    }
    else
    {
        return 250*(index + 1)*(index + 1) + 750*(index + 1) + 1000;
    }
}

int MapInfo::getPassReward(int index)
{
    if (index < m_vecPassReward.size())
    {
        return m_vecPassReward.at(index);
    }
    else
    {
        return 200*(index + 1) - 600;
    }

}

int MapInfo::getPassTime()
{
    return m_iPassTime;
}
