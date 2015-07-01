//
//  GameChallengeMap.h
//  author:Simon
//	
//  Created by 5agame on 15/5/15.
//	特殊关卡的大地图UI
//

#ifndef __GameChallengeMap_H__
#define __GameChallengeMap_H__

#include "GameFunctions.h"
#include "OverTableView.h"

class GameChallengeMap : public OverTableView
{
public:
	CREATE_FUNC(GameChallengeMap);
	virtual bool init();
	virtual TableViewCell* tableCellAtIndex(TableView* table,ssize_t idx);  //编辑单元

private:
	GameChallengeMap();
	~GameChallengeMap();

};

#endif