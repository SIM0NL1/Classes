//
//  GameChallengeMap.cpp
//  author:Simon
//	
//  Created by 5agame on 15/5/15.
//	特殊关卡的大地图UI
//

#include "GameChallengeMap.h"
#include "GameChallengeMission.h"
#include "GameUIData.h"

GameChallengeMap::GameChallengeMap()
{

}

GameChallengeMap::~GameChallengeMap()
{

}

bool GameChallengeMap::init()
{
	do 
	{
		CC_BREAK_IF(!OverTableView::init());
		this->setFlipMap(true);
		this->scrollViewForDistance();
		return true;
	} while (false);
	log("Fun GameChallengeMap :: init ERROR");
	return false;
}

TableViewCell* GameChallengeMap::tableCellAtIndex(TableView* table,ssize_t idx)
{
	std::string id = StringUtils::format("%d", idx+1);
	TableViewCell *cell = table->dequeueCell();

	if (1) {
		cell = new TableViewCell();
		cell->autorelease();

		Sprite* bg = Sprite::create(RESOURCE("map_002.png"));
		if (idx%2) {
			bg->setTexture(RESOURCE("map_001.png"));
		}
		bg->setFlippedX(fileMap);

		bg->setAnchorPoint(Point(0, 0));
		bg->setTextureRect(Rect(0,0,window_size.width,window_size.height));

		cell->addChild(bg);

		// ID部分
		auto *label = Label::createWithSystemFont(id.c_str(), "Arial", 20);
		label->setAnchorPoint(Point(0, 0));
		label->setPosition(Point(50, 0));
		label->setColor(Color3B(0,0,0));
		cell->addChild(label);

		if (idx>=(12-ci_NormalMissionNum/10))	//如果130关，就是13，但必须是10的整数倍
		{
			int missionId=120-idx*10;	//本单元最大关卡ID
			for (int i=0;i<10;++i)
			{
				GameChallengeMission* mission = GameChallengeMission::create();
				mission->setTag(missionId);
				mission->setAnchorPoint(Vec2::ZERO);
				//取得关卡ID对应的坐标
				Vec2 temp=GameUIData::getInstance()->getNormalMissionPos(missionId);
				mission->setPosition(temp);
				cell->addChild(mission);

				mission->setMissionPorperty(missionId);
				mission->missionShow(missionId);

				--missionId;
			}
		}

	}

	return cell;
}

