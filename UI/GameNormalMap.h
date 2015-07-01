//
//  GameNormalMap.h
//  author:Simon
//	
//  Created by 5agame on 15/5/15.
//	普通关卡的大地图UI
//

#ifndef __GameNormalMap_H__
#define __GameNormalMap_H__

#include "GameFunctions.h"
#include "OverTableView.h"

class GameNormalMission;
class GameNormalMap : public OverTableView
{
public:
	CREATE_FUNC(GameNormalMap);
	virtual bool init();
	virtual TableViewCell* tableCellAtIndex(TableView* table,ssize_t idx);  //编辑地图TableView单元
	void initMission();
	void normalActiveMissionBtnTouch(bool flag);		//because of the open mission's touch event swallow the tableview priority,so design this function;
private:
	GameNormalMap();
	~GameNormalMap();
	vector<GameNormalMission*> vec_normalMission;

};
extern GameNormalMap* g_pGameNormalMap;

#endif