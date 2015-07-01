//
//  GameFunctions.h
//  author:Simon
//
//  Created by 5agame on 15/5/14.
//  实现项目中公共的方法，这是一个工具类
//  单例实现

#include "GameFunctions.h"

GameFunctions* GameFunctions :: m_self = nullptr;

GameFunctions :: GameFunctions()
{
	m_sFilePath = "";
}

GameFunctions :: ~GameFunctions()
{
}

GameFunctions* GameFunctions :: getInstance()
{
	if (!m_self)
	{
		m_self = new GameFunctions();
	}
	return m_self;
}
void GameFunctions :: deleteGameFunctions()
{
	if (m_self)
	{
		delete(m_self);
	}
	m_self = nullptr;
}

const char* GameFunctions::readResourcesPath(string fileName)
{
	//设置资源路径统一接口，方便后期操作;
	GameFunctions::getInstance()->m_sFilePath="UI/"+fileName;		//防止资源路径修改;
	return GameFunctions::getInstance()->m_sFilePath.c_str();
}
