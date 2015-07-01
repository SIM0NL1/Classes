//
//  GameUIData.cpp
//  author:Simon
//
//  Created by 5agame on 15/5/18.
//	Json生成，读取;
//

#include "GameUIData.h"
#include "GameDragonBase.h"
#include "SupportTool.h"
#include "GameFunctions.h"

GameUIData* GameUIData::m_self = nullptr;
GameUIData::GameUIData()
{
    missionpos.clear();
    normalMissionProgress = 1;
    challengeMissionProgress = 1;
    curNormalMission = 1;
    curChallengeMission = 1;
    vec_Role.clear();
    challengepos.clear();
    horizontalGps = 0;
    verticalGps = -1;
    verticalIndex = 0;
    _vecNormalPro.clear();
    _vecChallengePro.clear();
}

GameUIData::~GameUIData()
{
}

GameUIData* GameUIData::getInstance()
{
    if (!m_self)
    {
        m_self = new GameUIData();
    }
    return m_self;
}

void GameUIData::deleteInstance()
{
    if (m_self)
    {
        delete(m_self);
    }
    m_self = nullptr;
}

void GameUIData::writeData()
{
    //安全路径;
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("uidata.json");
    //const char* path  = RESOURCE("uidata/uidata.json");
    if(FileUtils::getInstance()->isFileExist(path))
    {
        rapidjson::Document readdoc;
        string data = FileUtils::getInstance()->getStringFromFile(path);
        readdoc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if(readdoc.HasParseError()||!readdoc.IsObject())
        {
            CCLOG("GetParseError%s\n",readdoc.GetParseError());
        }
        
        rapidjson::Value &var = readdoc;
        int normalPro = var["CurNormalMissionProgress"].GetInt();
        int challengePro = var["CurChallengeMissionProgress"].GetInt();
        int normalPlay = var["CurNormalMissionPlay"].GetInt();
        int challengePlay = var["CurChallengeMissionPlay"].GetInt();
        int longbiNum = var["CurLongBiNum"].GetInt();
        int diamondsNum = var["CurDiamondsNum"].GetInt();
        log("%d,%d,%d,%d,%d,%d",normalPro,challengePro,normalPlay,challengePlay,longbiNum,diamondsNum);
    }
    else
    {
        //生成Json文件，存储在getWriteablePath文件夹下;
        rapidjson::Document writedoc;
        writedoc.SetObject();
        rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
        //json object  格式添加键值对;
        writedoc.AddMember("CurNormalMissionProgress",1,allocator);		//普通关卡当前进度;
        writedoc.AddMember("CurChallengeMissionProgress",0,allocator);	//挑战关卡当前进度;
        writedoc.AddMember("CurNormalMissionPlay",1,allocator);			//正在游戏的普通关卡;
        writedoc.AddMember("CurChallengeMissionPlay",0,allocator);		//正在游戏的副本关卡;
        writedoc.AddMember("CurLongBiNum",100,allocator);		//当前龙币数值;
        writedoc.AddMember("CurDiamondsNum",100,allocator);		//当前钻石数值;
        
        StringBuffer buffer;
        rapidjson::Writer<StringBuffer> writer(buffer);  
        writedoc.Accept(writer);
        FILE* file = fopen(path.c_str(), "wb");
        if (file)
        {
            fputs(buffer.GetString(), file);  
            fclose(file);
        }
        CCLOG("%s",buffer.GetString());
    }
}

void GameUIData::readPosData(JsonFileType fileType)
{
    rapidjson::Document readdoc;
    string data;
    switch (fileType)
    {
        case JsonFileType::NORMALMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/missionposition.json"));
            missionpos.clear();
            break;
        case JsonFileType::CHALLENGEMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/missionposition_ts.json"));
            challengepos.clear();
            break;
        default:
            break;
    }
    readdoc.Parse<0>(data.c_str());
    if(readdoc.HasParseError() || !readdoc.IsArray())
    {
        CCLOG("GetParseError%s\n",readdoc.GetParseError());
    }
    
    for (unsigned int i=1;i<readdoc.Size();++i)
    {
        rapidjson::Value &temp = readdoc[i];
        int j = 0;
        Vec2 posTemp;
        posTemp.x=temp[j].GetInt();
        posTemp.y=temp[++j].GetInt();
        switch (fileType)
        {
            case JsonFileType::NORMALMISSION:missionpos.push_back(posTemp);break;
            case JsonFileType::CHALLENGEMISSION:challengepos.push_back(posTemp);break;
            default:break;
        }
    }
    return;
}

void GameUIData::readMissionProgressData(JsonFileType fileType)
{
    rapidjson::Document readdoc;
    string data;
    switch (fileType)
    {
        case JsonFileType::NORMALMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission.json"));
            _vecNormalPro.clear();
            break;
        case JsonFileType::CHALLENGEMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission_ts.json"));
            _vecChallengePro.clear();
            break;
        default:
            break;
    }
    readdoc.Parse<0>(data.c_str());
    if(readdoc.HasParseError() || !readdoc.IsArray())
    {
        CCLOG("GetParseError%s\n",readdoc.GetParseError());
    }
    
    for (unsigned int i=1;i<readdoc.Size();++i)
    {
        rapidjson::Value &temp = readdoc[i];
        int j = 0;
        MissionPro proTemp;
        proTemp.id = temp[j].GetInt();
        proTemp.start = temp[++j].GetInt();
        proTemp.score = temp[++j].GetInt();
        proTemp.state = temp[++j].GetInt();
        switch (fileType)
        {
            case JsonFileType::NORMALMISSION:_vecNormalPro.push_back(proTemp);break;
            case JsonFileType::CHALLENGEMISSION:_vecChallengePro.push_back(proTemp);break;
            default:break;
        }
    }
    return;
}

void GameUIData::writeMissionProgressData(JsonFileType fileType)
{
    rapidjson::Document readdoc;
    string data;
    switch (fileType)
    {
        case JsonFileType::NORMALMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission.json"));
            _vecNormalPro.clear();
            break;
        case JsonFileType::CHALLENGEMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission_ts.json"));
            _vecChallengePro.clear();
            break;
        default:
            break;
    }
    readdoc.Parse<0>(data.c_str());
    if(readdoc.HasParseError() || !readdoc.IsArray())
    {
        CCLOG("GetParseError%s\n",readdoc.GetParseError());
    }
    
    for (unsigned int i=1;i<readdoc.Size();++i)
    {
        rapidjson::Value &temp = readdoc[i];
        int j = 0;
        temp[j].SetInt(99);
        temp[++j].SetInt(99);
        temp[++j].SetInt(99);
        temp[++j].SetInt(99);	
    }
    
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);  
    readdoc.Accept(writer);
    FILE* file = fopen(RESOURCE("uidata/bigmapmission.json"), "wb");
    if (file)
    {
        fputs(buffer.GetString(), file);  
        fclose(file);
    }
    CCLOG("%s",buffer.GetString()); 
    
    return;
}

void GameUIData::setIntegerForKey(string key,int data)
{
    
}

void GameUIData::setBooleanForKey(string key,bool data)
{
    
}

void GameUIData::setStringForKey(string key,string data)
{
    
}

void GameUIData::setFloatForKey(string key,float data)
{
    
}

cocos2d::Vec2 GameUIData::getNormalMissionPos(int id)
{
    return missionpos.at(id-1);
}

cocos2d::Vec2 GameUIData::getChallengeMissionPos(int id)
{
    return challengepos.at(id-1);
}

void GameUIData::readRoleData()
{
    string filename = RESOURCE("uidata/roledata.json");
    rapidjson::Document doc;
    //判断文件是否存在;
    if (!FileUtils::getInstance()->isFileExist(filename))
    {
        CCLOG("roledata json file is not find [%s]",filename.c_str());
        return;
    }
    string data = FileUtils::getInstance()->getStringFromFile(filename);
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
    //
    if (doc.HasParseError() || !doc.IsArray())
    {
        CCLOG("GetParseError%s\n",doc.GetParseError());
        return;
    }
    
    //刨除第一行;
    for (unsigned int i=1;i<doc.Size();++i)
    {
        //每一行是一个value;
        rapidjson::Value &temp = doc[i];
        int j=0;		//
        GameDragonBase* pData = new GameDragonBase();
        pData->m_nDragonId = temp[j].GetInt();
        pData->m_sDragonChineseName = temp[++j].GetString();
        
        GameFunctions::getInstance()->g_sChineseName = pData->m_sDragonChineseName;
        
        pData->m_sEnglishName = temp[++j].GetString();
        
        //GameFunctions::getInstance()->g_sChineseName = pData->m_sEnglishName;
        
        pData->m_nDragonType = temp[++j].GetInt();
        pData->m_nUnlockType = temp[++j].GetInt();
        
        if (temp[++j].IsInt())
        {
            pData->m_aUnlockCondition.push_back(temp[j].GetInt());
            pData->m_aUnlockCondition.push_back(-7);
        }
        else if (temp[j].IsString())
        {
            string s = temp[j].GetString();
            string delim = "+";
            vector<string> str_temp = split(s,delim);
            for (size_t i=0;i<str_temp.size();++i)
            {
                pData->m_aUnlockCondition.push_back( atoi(str_temp.at(i).c_str()) );
            }
        }
        pData->m_nTrial = temp[++j].GetInt();
        pData->m_aPrice[0] = temp[++j].GetInt();
        pData->m_aPrice[1] = temp[++j].GetInt();
        pData->m_sAttribute = temp[++j].GetString();
        pData->m_nDragonLevel = temp[++j].GetInt();
        pData->m_sSkill	= temp[++j].GetString();
        pData->m_fImpair = temp[++j].GetDouble();
        pData->m_nSkillId = temp[++j].GetInt();
        pData->m_nSkillType = temp[++j].GetInt();
        pData->m_nCollect = temp[++j].GetInt();
        pData->m_sDescribe = temp[++j].GetString();
        pData->m_nSkillNum = temp[++j].GetInt();
        pData->m_nFeed = temp[++j].GetInt();
        pData->m_nUpGradeTotal = temp[++j].GetInt();
        pData->m_fDiscount = temp[++j].GetDouble();
        pData->m_sAnimate = temp[++j].GetString();
        pData->m_sImage = temp[++j].GetString();
        pData->m_sIcon = temp[++j].GetString();

        vec_Role.push_back(pData);
    }
}

//截取字符串;
vector<string> GameUIData::split(string str,string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    //
    size_t size=str.size();
    for(size_t i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

float GameUIData::getNormalMissionHeight()
{
    if (normalMissionProgress%10)
        return GLB_SIZE.height/2-(GLB_SIZE.height*(normalMissionProgress/10)+missionpos.at(normalMissionProgress-1).y);
    else
        return GLB_SIZE.height/2-(GLB_SIZE.height*((normalMissionProgress/10)-1)+missionpos.at(normalMissionProgress-1).y);
}

float GameUIData::getChallengeMissionHeight()
{
    if (challengeMissionProgress%5)
        return GLB_SIZE.height/2-(GLB_SIZE.height*(challengeMissionProgress/5)+challengepos.at(challengeMissionProgress-1).y);
    else
        return GLB_SIZE.height/2-(GLB_SIZE.height*((challengeMissionProgress/5)-1)+challengepos.at(challengeMissionProgress-1).y);
}

void GameUIData::initGPS()
{
    horizontalGps = getNormalMissionHeight();
}

MissionPro GameUIData::getMissionProgress(int id,JsonFileType fileType)
{
    switch (fileType)
    {
        case JsonFileType::NORMALMISSION:return _vecNormalPro.at(id);break;
        case JsonFileType::CHALLENGEMISSION:return _vecChallengePro.at(id);break;
        default:break;
    }
}



