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
	_mapSingleSoldier.clear();
	_mapAllSoldier.clear();
	_vecSingleBoss.clear();
	_mapAllBoss.clear();
	_mapShield.clear();
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

void GameUIData::writeMissionProgressData(JsonFileType fileType,int id,MissionPro& progress)
{
    rapidjson::Document readdoc;
    string data;
    switch (fileType)
    {
        case JsonFileType::NORMALMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission.json"));
            _vecNormalPro.at(id-1) = progress;
            break;
        case JsonFileType::CHALLENGEMISSION:
            data = FileUtils::getInstance()->getStringFromFile(RESOURCE("uidata/bigmapmission_ts.json"));
            _vecChallengePro.at(id-1) = progress;
            break;
        default:break;
    }
    readdoc.Parse<0>(data.c_str());
    if(readdoc.HasParseError() || !readdoc.IsArray())
    {
        CCLOG("GetParseError%s\n",readdoc.GetParseError());
    }
    
	rapidjson::Value &temp = readdoc[id];
	int j = 0;
	temp[j].SetInt(progress.id);
	temp[++j].SetInt(progress.start);
	temp[++j].SetInt(progress.score);
	temp[++j].SetInt(progress.state);

  //  FileUtils::getInstance()->getWritablePath().append("userdata.json");

    StringBuffer buffer;
    rapidjson::PrettyWriter<StringBuffer> writer(buffer);  
    readdoc.Accept(writer);
	FILE* file;
	switch (fileType)
	{
	case JsonFileType::NORMALMISSION:
		file = fopen(RESOURCE("uidata/bigmapmission.json"), "wb");
		break;
	case JsonFileType::CHALLENGEMISSION:
		file = fopen(RESOURCE("uidata/bigmapmission_ts.json"), "wb");
		break;
	default:break;
	}
    
    if (file)
    {
        fputs(buffer.GetString(), file);  
        fclose(file);
    }
    //CCLOG("%s",buffer.GetString()); 
    
    return;
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
        case JsonFileType::NORMALMISSION:return _vecNormalPro.at(id-1);break;
        case JsonFileType::CHALLENGEMISSION:return _vecChallengePro.at(id-1);break;
        default:return _vecNormalPro.at(id-1);break;	//只是为了消除警告;
    }
}

void GameUIData::readTDSoldierData()
{
	string filename = RESOURCE("uidata/td_soldier.json");
	rapidjson::Document tdDoc;
	//判断文件是否存在;
	if (!FileUtils::getInstance()->isFileExist(filename))
	{
		CCLOG("soldierdata json file is not find [%s]",filename.c_str());
		return;
	}
	string data = FileUtils::getInstance()->getStringFromFile(filename);
	tdDoc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//
	if (tdDoc.HasParseError() || !tdDoc.IsArray())
	{
		CCLOG("GetParseError%s\n",tdDoc.GetParseError());
		return;
	}
	_mapSingleSoldier.clear();
	_mapAllSoldier.clear();
	for (unsigned int i=1;i<tdDoc.Size();++i)
	{
		rapidjson::Value &temp = tdDoc[i];
		int j = 0;
		SoldierData dataTemp;
		int type = temp[j].GetInt();
		int level = temp[++j].GetInt();
		dataTemp.spend = temp[++j].GetInt();
		dataTemp.hp = temp[++j].GetInt();
		dataTemp.dps = temp[++j].GetInt();
		dataTemp.rate = temp[++j].GetInt();
		_mapSingleSoldier[level] = dataTemp;
		if (level==5)
		{
			_mapAllSoldier[type] = _mapSingleSoldier;
		}
	}
	for (int i=1;i<6;i++)
	{
		for(int j=1;j<6;j++)
		{
			CCLOG("type = %d, level = %d, spend = %d, hp = %d, dps = %d, rate = %d",i,j,_mapAllSoldier.at(i).at(j).spend,_mapAllSoldier.at(i).at(j).hp,_mapAllSoldier.at(i).at(j).dps,_mapAllSoldier.at(i).at(j).rate);
		}
	}
}

void GameUIData::readTDBossData()
{
	string filename = RESOURCE("uidata/td_boss.json");
	rapidjson::Document tdDoc;
	//判断文件是否存在;
	if (!FileUtils::getInstance()->isFileExist(filename))
	{
		CCLOG("Bossdata json file is not find [%s]",filename.c_str());
		return;
	}
	string data = FileUtils::getInstance()->getStringFromFile(filename);
	tdDoc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//
	if (tdDoc.HasParseError() || !tdDoc.IsArray())
	{
		CCLOG("GetParseError%s\n",tdDoc.GetParseError());
		return;
	}
	_vecSingleBoss.clear();
	_mapAllBoss.clear();
	for (unsigned int i=1;i<tdDoc.Size();++i)
	{
		rapidjson::Value &temp = tdDoc[i];
		int j = 0;
		BossData dataTemp;
		int missionId = temp[j].GetInt();
		dataTemp.type = temp[++j].GetInt();
		dataTemp.shield_1 = temp[++j].GetInt();
		dataTemp.shield_2 = temp[++j].GetInt();
		dataTemp.shield_3 = temp[++j].GetInt();
		dataTemp.hp = temp[++j].GetInt();
		dataTemp.rate = temp[++j].GetInt();
		dataTemp.dps = temp[++j].GetInt();
		dataTemp.range = temp[++j].GetInt();
		dataTemp.ai = temp[++j].GetString();
		dataTemp.animation = temp[++j].GetString();
		dataTemp.reward = temp[++j].GetInt();

		_mapAllBoss[missionId].push_back(dataTemp);

	}
	for (int i=1;i<=_mapAllBoss.size();i++)
	{
		for(int j=0;j<_mapAllBoss.at(i).size();j++)////
		{
			CCLOG("missionId=%d,type=%d,shield1=%d,shield2=%d,shield3=%d,hp=%d,rate=%d,dps=%d,range=%d,ai=%s,animation=%s,reward=%d",
				i,_mapAllBoss.at(i).at(j).type,_mapAllBoss.at(i).at(j).shield_1,_mapAllBoss.at(i).at(j).shield_2,_mapAllBoss.at(i).at(j).shield_3,_mapAllBoss.at(i).at(j).hp,_mapAllBoss.at(i).at(j).rate,_mapAllBoss.at(i).at(j).dps,_mapAllBoss.at(i).at(j).range,_mapAllBoss.at(i).at(j).ai.c_str(),_mapAllBoss.at(i).at(j).animation.c_str(),_mapAllBoss.at(i).at(j).reward);
		}
	}
}

void GameUIData::readTDShieldData()
{
	string filename = RESOURCE("uidata/td_shield.json");
	rapidjson::Document tdDoc;
	//判断文件是否存在;
	if (!FileUtils::getInstance()->isFileExist(filename))
	{
		CCLOG("ShieldData json file is not find [%s]",filename.c_str());
		return;
	}
	string data = FileUtils::getInstance()->getStringFromFile(filename);
	tdDoc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	//
	if (tdDoc.HasParseError() || !tdDoc.IsArray())
	{
		CCLOG("GetParseError%s\n",tdDoc.GetParseError());
		return;
	}
	_mapShield.clear();
	for (unsigned int i=1;i<tdDoc.Size();++i)
	{
		rapidjson::Value &temp = tdDoc[i];
		int j = 0;
		ShieldData dataTemp;
		dataTemp.id = temp[j].GetInt();
		dataTemp.type = temp[++j].GetInt();
		dataTemp.elementId = temp[++j].GetInt();
		dataTemp.num = temp[++j].GetInt();
		_mapShield[i] = dataTemp;
	}
	for (int i=1;i<=_mapShield.size();i++)
	{
		CCLOG("Shield situationId = %d, type = %d, elementId = %d, num = %d",_mapShield.at(i).id,_mapShield.at(i).type,_mapShield.at(i).elementId,_mapShield.at(i).num);
	}
}

SoldierData GameUIData::getSoldierData(int type,int level)
{
	 return _mapAllSoldier.at(type).at(level);
}

vector<BossData> GameUIData::getBossData(int missionId)
{
	return _mapAllBoss.at(missionId);
}

ShieldData GameUIData::getShieldData(int id)
{
	return _mapShield.at(id);
}

void GameUIData::writeData()
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document readdoc;
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		readdoc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(readdoc.HasParseError()||!readdoc.IsObject())
		{
			CCLOG("GetParseError%s\n",readdoc.GetParseError());
		}

		rapidjson::Value &var = readdoc;
		int normalPro = Json_Check_int32(var,"CurNormalMissionProgress");
		int challengePro = Json_Check_int32(var,"CurChallengeMissionProgress");
		int normalPlay = Json_Check_int32(var,"CurNormalMissionPlay");
		int challengePlay = Json_Check_int32(var,"CurChallengeMissionPlay");
		int longbiNum = Json_Check_int32(var,"CurLongBiNum");
		int diamondsNum = Json_Check_int32(var,"CurDiamondsNum");
		log("%d,%d,%d,%d,%d,%d",normalPro,challengePro,normalPlay,challengePlay,longbiNum,diamondsNum);
	}
	else
	{
		//生成Json文件，存储在getWriteablePath文件夹下;
		rapidjson::Document writedoc;
		rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();

		writedoc.SetObject();
		//json object  格式添加键值对;
		writedoc.AddMember("CurNormalMissionProgress",2,allocator);		//普通关卡当前进度;
		writedoc.AddMember("CurChallengeMissionProgress",0,allocator);	//挑战关卡当前进度;
		writedoc.AddMember("CurNormalMissionPlay",2,allocator);			//正在游戏的普通关卡;
		writedoc.AddMember("CurChallengeMissionPlay",0,allocator);		//正在游戏的副本关卡;
		writedoc.AddMember("CurLongBiNum",100,allocator);		//当前龙币数值;
		writedoc.AddMember("CurDiamondsNum",100,allocator);		//当前钻石数值;
		//数组里面包着对象;
//		rapidjson::Value array(rapidjson::kArrayType);
//		rapidjson::Value object(rapidjson::kObjectType);                
// 		object.AddMember("CurNormalMissionProgress",1,allocator);		//普通关卡当前进度;
// 		object.AddMember("CurChallengeMissionProgress",0,allocator);	//挑战关卡当前进度;
// 		object.AddMember("CurNormalMissionPlay",1,allocator);			//正在游戏的普通关卡;
// 		object.AddMember("CurChallengeMissionPlay",0,allocator);		//正在游戏的副本关卡;
// 		object.AddMember("CurLongBiNum",100,allocator);			//当前龙币数值;
// 		object.AddMember("CurDiamondsNum",100,allocator);		//当前钻石数值;
// 		array.PushBack(object,allocator);
// 		writedoc.AddMember("user",array,allocator);
		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		writedoc.Accept(writer);
		//system("delE:/Project/DND_Win/Resources/UI/uidata/userdata.json");
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}
		CCLOG("%s",buffer.GetString());
	}
}

void GameUIData::setIntegerForKey(const char* key,int value)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_int32(var,key))
		{
			var[key] = value;
		}
		else
		{
			doc.AddMember(key,value,allocator);
		}

		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}

	}
	else
	{
		MessageBox("ERROR Not Exist uidata.json ! setIntegerForKey","JsonError");
		CCLOG("ERROR Not Exist uidata.json ! setIntegerForKey");
		return;
	}
}

void GameUIData::setLongIntegerForKey(const char* key,unsigned int value)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_uint64(var,key))
		{
			var[key] = value;
		}
		else
		{
			doc.AddMember(key,value,allocator);
		}

		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}

	}
	else
	{
		MessageBox("ERROR Not Exist uidata.json ! setLongIntegerForKey","JsonError");
		CCLOG("ERROR Not Exist uidata.json ! setLongIntegerForKey");
		return;
	}
}

void GameUIData::setBooleanForKey(const char* key,bool value)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_bool(var,key))
		{
			var[key] = value;
		}
		else
		{
			doc.AddMember(key,value,allocator);
		}

		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}
	}
	else
	{
		MessageBox("ERROR Not Exist uidata.json ! setBooleanForKey","JsonError");
		CCLOG("ERROR Not Exist uidata.json ! setBooleanForKey");
		return;
	}
}

void GameUIData::setStringForKey(const char* key,string value)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_string(var,key))
		{
			var[key] = value.c_str();
		}
		else
		{
			doc.AddMember(key,value.c_str(),allocator);
		}

		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}
	}
	else
	{
		MessageBox("ERROR Not Exist uidata.json ! setStringForKey","JsonError");
		CCLOG("ERROR Not Exist uidata.json ! setStringForKey");
		return;
	}
}

void GameUIData::setFloatForKey(const char* key,float value)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_double(var,key))
		{
			var[key] = value;
		}
		else
		{
			doc.AddMember(key,value,allocator);
		}

		StringBuffer buffer;
		rapidjson::PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);
		FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
		if (file)
		{
			fputs(buffer.GetString(), file);
			fclose(file);
		}
	}
	else
	{
		MessageBox("ERROR Not Exist uidata.json ! setFloatForKey","JsonError");
		CCLOG("ERROR Not Exist uidata.json ! setFloatForKey");
		return;
	}
}

int GameUIData::getIntegerForKey(const char* key)
{
	return getIntegerForKey(key,0);
}

int GameUIData::getIntegerForKey(const char* key,int defaultValue)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_int32(var,key))
		{
			return Json_Check_int32(var,key);
		}
		else
		{
			doc.AddMember(key,defaultValue,allocator);
			StringBuffer buffer;
			rapidjson::PrettyWriter<StringBuffer> writer(buffer);
			doc.Accept(writer);
			FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
			if (file)
			{
				fputs(buffer.GetString(), file);
				fclose(file);
			}
			return defaultValue;
		}
	}
	else
	{
		CCLOG("ERROR Not Exist uidata.json ! getIntegerForKey");
		return 0;
	}
}

unsigned int GameUIData::getLongIntegerForKey(const char* key)
{
	return getLongIntegerForKey(key,0);
}

unsigned int GameUIData::getLongIntegerForKey(const char* key,unsigned int defaultValue)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}

		rapidjson::Value &var = doc;
		if (json_check_is_uint64(var,key))
		{
			return Json_Check_uint64(var,key);
		}
		else
		{
			doc.AddMember(key,defaultValue,allocator);
			StringBuffer buffer;
			rapidjson::PrettyWriter<StringBuffer> writer(buffer);
			doc.Accept(writer);
			FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
			if (file)
			{
				fputs(buffer.GetString(), file);
				fclose(file);
			}
			return defaultValue;
		}
	}
	else
	{
		CCLOG("ERROR Not Exist uidata.json ! getLongIntegerForKey");
		MessageBox("Not Exist uidata.json ! getLongIntegerForKey","Error");
		return 0;
	}
}

bool GameUIData::getBooleanForKey(const char* key)
{
	return getBooleanForKey(key,false);
}

bool GameUIData::getBooleanForKey(const char* key,bool defaultValue)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}
		rapidjson::Value &var = doc;
		if (json_check_is_bool(var,key))
		{
			return Json_Check_bool(var,key);
		}
		else
		{
			doc.AddMember(key,defaultValue,allocator);
			StringBuffer buffer;
			rapidjson::PrettyWriter<StringBuffer> writer(buffer);
			doc.Accept(writer);
			FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
			if (file)
			{
				fputs(buffer.GetString(), file);
				fclose(file);
			}
			return defaultValue;
		}
	}
	else
	{
		CCLOG("ERROR Not Exist uidata.json ! getBooleanForKey");
		return false;
	}
}

std::string GameUIData::getStringForKey(const char* key)
{
	return getStringForKey(key,"");
}

std::string GameUIData::getStringForKey(const char* key,string defaultValue)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}
		rapidjson::Value &var = doc;
		if (json_check_is_string(var,key))
		{
			return Json_Check_string(var,key);
		} 
		else
		{
			doc.AddMember(key,defaultValue.c_str(),allocator);
			StringBuffer buffer;
			rapidjson::PrettyWriter<StringBuffer> writer(buffer);
			doc.Accept(writer);
			FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
			if (file)
			{
				fputs(buffer.GetString(), file);
				fclose(file);
			}
			return defaultValue;
		}
	}
	else
	{
		CCLOG("ERROR Not Exist uidata.json ! getStringForKey");
		return "";
	}
}

float GameUIData::getFloatForKey(const char* key)
{
	return getFloatForKey(key,0);
}

float GameUIData::getFloatForKey(const char* key,float defaultValue)
{
	if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
		if(doc.HasParseError()||!doc.IsObject())
		{
			CCLOG("GetParseError%s\n",doc.GetParseError());
		}
		rapidjson::Value &var = doc;
		if (json_check_is_double(var,key))
		{
			return Json_Check_double(var,key);
		} 
		else
		{
			doc.AddMember(key,defaultValue,allocator);
			StringBuffer buffer;
			rapidjson::PrettyWriter<StringBuffer> writer(buffer);
			doc.Accept(writer);
			FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
			if (file)
			{
				fputs(buffer.GetString(), file);
				fclose(file);
			}
			return defaultValue;
		}
	}
	else
	{
		CCLOG("ERROR Not Exist uidata.json ! getFloatForKey");
		return 0;
	}
}

void GameUIData::removeDataForKey(const char* key)
{
    if(FileUtils::getInstance()->isFileExist(_JSON_PATH_))
    {
        rapidjson::Document doc;
        string data = FileUtils::getInstance()->getStringFromFile(_JSON_PATH_);
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if(doc.HasParseError()||!doc.IsObject())
        {
            CCLOG("GetParseError%s\n",doc.GetParseError());
        }
        doc.RemoveMember(key);
        StringBuffer buffer;
        rapidjson::PrettyWriter<StringBuffer> writer(buffer);
        doc.Accept(writer);
        FILE* file = fopen(_JSON_PATH_.c_str(), "wb");
        if (file)
        {
            fputs(buffer.GetString(), file);
            fclose(file);
        }
    }
    else
    {
        CCLOG("ERROR Not Exist uidata.json ! removeDataForKey");
        return;
    }
}

void GameUIData::setNormalMissionProgress(int num)
{
	normalMissionProgress = num;
	GameUIData::getInstance()->setIntegerForKey("CurNormalMissionProgress",num);
}

int GameUIData::getNormalMissionProgress() const
{
	return normalMissionProgress;
}

void GameUIData::setCurNormalMission(int num)
{
	curNormalMission = num;
	GameUIData::getInstance()->setIntegerForKey("CurNormalMissionPlay",num);
}

int GameUIData::getCurNormalMission() const
{
	return curNormalMission;
}

void GameUIData::setChallengeMissionProgress(int num)
{
	challengeMissionProgress = num;
	GameUIData::getInstance()->setIntegerForKey("CurChallengeMissionProgress",num);
}

int GameUIData::getChallengeMissionProgress() const
{
	return challengeMissionProgress;
}

void GameUIData::setCurChallengeMission(int num)
{
	curChallengeMission = num;
	GameUIData::getInstance()->setIntegerForKey("CurChallengeMissionPlay",num);
}

int GameUIData::getCurChallengeMission() const
{
	return curChallengeMission;
}

