//
//  DataCenter.cpp
//  Dragon
//
//  Created by B081mac on 15/4/14.
//
//

#include "DataCenter.h"

static DataCenter * s_ShareDataCenter = nullptr;

DataCenter::DataCenter()
:m_iAddSteps(0)
,m_bWinnerMode(false)
,m_bTimeLimit(false)
{
    m_vecRemoveGem.clear();
}

DataCenter::~DataCenter()
{
    
}

DataCenter* DataCenter::getInstance()
{
    if (!s_ShareDataCenter)
    {
        s_ShareDataCenter = new (std::nothrow) DataCenter();
        CCASSERT(s_ShareDataCenter, "FATAL: Not enough memory");
    }
    return s_ShareDataCenter;
}

void DataCenter::initMapInfo(int level)
{
    m_pMapInfo = new (std::nothrow) MapInfo();
    
    if (m_bTimeLimit)
    {
        m_pMapInfo->getJsonData();
    }
    else
    {
        m_pMapInfo->getJsonData(level);
    }
    
}

MapInfo* DataCenter::getMapInfo()
{
    return  m_pMapInfo;
}

void DataCenter::destroyMapInfo()
{
    if (m_pMapInfo)
    {
        delete m_pMapInfo;
    }
}

void DataCenter::setAddSteps(int steps)
{
    m_iAddSteps = steps;
}

int DataCenter::getAddSteps()
{
    return m_iAddSteps;
}

void DataCenter::setPosStar(Point pos)
{
    m_posStar = pos;
}

Point DataCenter::getPosStar()
{
    return m_posStar;
}

void DataCenter::setWinnerMode(bool b)
{
    m_bWinnerMode = b;
}

bool DataCenter::getWinnerMode()
{
    return m_bWinnerMode;
}

void DataCenter::setTimeLimit(bool b)
{
    m_bTimeLimit = b;
}

bool DataCenter::getTimeLimit()
{
    return m_bTimeLimit;
}

void DataCenter::initMapPoint(GemType type, Point pos)
{
    if (type >= babydragon)
    {
        m_mapGemPoint.insert(make_pair(type , pos));
    }
}

Point DataCenter::getGemPoint(GemType type)
{
    std::map<GemType, Point>::iterator iter;
    iter = m_mapGemPoint.find(type);
    if (iter != m_mapGemPoint.end())
    {
        return iter->second;
    }
    else
    {
        return Point(-1, -1);
    }

}
