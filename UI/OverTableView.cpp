//
//  OverTableView.cpp
//  author:Simon
//
//  Created by 5agame on 15/5/14.
//
//

#include "OverTableView.h"
#include "GameFunctions.h"

#include "GameChallengeMission.h"
#include "GameUIData.h"

OverTableView::OverTableView()
{
    //屏幕大小默认值;
    window_size = Size(640.f*2-1,1136.f);
    //单元个数默认值;
    cellNum = ci_MapNum;
    //地图镜像;
    fileMap = false;
    m_pMengBan = nullptr;
	vec_normalMission.clear();
}

OverTableView::~OverTableView()
{
	vec_normalMission.clear();
}

// on "init" you need to initialize your instance
bool OverTableView::init()
{
    //初始化;
    if ( !Layer::init() ) return false;
    
    tableView = TableView::create(this,window_size);
    //展开方向;
    tableView->setDirection(TableView::Direction::VERTICAL);
    //设置填充，需要优化;
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //追加 托管;
    tableView->setDelegate(this);
    addChild(tableView);
    tableView->reloadData();
    tableView->setBounceable(true);
    tableView->cellAtIndex(0);
    return true;
}

//设定单元格大小，宽，高;
Size OverTableView::cellSizeForTable(TableView *table){
    return window_size;
}

//设计单元格内容;
TableViewCell* OverTableView::tableCellAtIndex(TableView *table,ssize_t idx)
{
	CCLOG(" ~(^OO^)~  %d",idx);
    std::string id = StringUtils::format("%ld", idx+1);
    TableViewCell *cell = table->dequeueCell();

	cell = new TableViewCell();
	cell->autorelease();
	cell->setLocalZOrder(idx+1);
	string filename = StringUtils::format("mapbg/map_00%ld.png",ci_MapNum-(idx)%ci_MapNum);

	Sprite* bg_left = Sprite::create(RESOURCE(filename));
	bg_left->setFlippedX(false);
	bg_left->setAnchorPoint(Point(0, 0));
	bg_left->setPosition(Vec2::ZERO);
	cell->addChild(bg_left);
	if (!idx)
	{
		Sprite* expect = Sprite::create(RESOURCE("mapbg/map_jqqd.png"));
		expect->setAnchorPoint(Vec2(0.5f,0.5f));
		expect->setPosition(Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height-300));
		bg_left->addChild(expect,Z_First);
	}

	Sprite* bg_right = Sprite::create(RESOURCE(filename));
	bg_right->setFlippedX(true);
	bg_right->setAnchorPoint(Point(0, 0));
	bg_right->setPosition(Vec2(639,0));
	cell->addChild(bg_right);
	if (!idx)
	{
		Sprite* expect = Sprite::create(RESOURCE("mapbg/map_jqqd.png"));
		expect->setAnchorPoint(Vec2(0.5f,0.5f));
		expect->setPosition(Vec2(GLB_SIZE.width*0.5,GLB_SIZE.height-300));
		bg_right->addChild(expect,Z_First);
	}

	// ID部分
	auto *label_left = Label::createWithSystemFont(id.c_str(),"Arial",20);
	label_left->setAnchorPoint(Point(0, 0));
	label_left->setPosition(Point(50, 0));
	label_left->setColor(Color3B(0,0,0));
	//cell->addChild(label_left);

	auto *label_right = Label::createWithSystemFont(id.c_str(),"Arial",20);
	label_right->setAnchorPoint(Point(0, 0));
	label_right->setPosition(Point(50+639, 0));
	label_right->setColor(Color3B(0,0,0));
	//cell->addChild(label_right);
	//布置普通关卡;
	if (idx>=ci_MapNum-ci_NormalMissionNum/10)	//如果130关，就是13，但必须是10的整数倍;
	{
		int missionId=10*ci_MapNum-idx*10;	//本单元最大关卡ID;
		for (int i=0;i<10;++i)
		{
			GameNormalMission* mission_left = GameNormalMission :: create();
			mission_left->m_nMissionId = missionId;
			mission_left->setTag(1000+missionId);
			mission_left->setAnchorPoint(Vec2::ZERO);
			//取得关卡ID对应的坐标;
			Vec2 temp=GameUIData::getInstance()->getNormalMissionPos(missionId);
			mission_left->setPosition(temp);
			cell->addChild(mission_left);
			vec_normalMission.push_back(mission_left);
			mission_left->setMissionPorperty(missionId);
			mission_left->missionShow(missionId);
			GameFunctions::getInstance()->vertexZ(&mission_left,false);
			--missionId;
		}
	}

	// 特殊关卡蒙灰;
	m_pMengBan = Sprite :: create(RESOURCE("overlay_map.png"));
	m_pMengBan->setAnchorPoint(Vec2::ZERO);
	m_pMengBan->setPosition(Vec2(GLB_SIZE.width,0));
	m_pMengBan->setOpacity(120);
	cell->addChild(m_pMengBan,Z_Second);
	// 设置混合模式;
	BlendFunc cbl = {GL_DST_COLOR,GL_ONE_MINUS_SRC_ALPHA};
	m_pMengBan->setBlendFunc(cbl);

	auto pFog = Sprite::create(RESOURCE("wuqi_ddt.png"));
	pFog->setAnchorPoint(Vec2::ZERO);
	pFog->setPosition(Vec2(GLB_SIZE.width,0));
	cell->addChild(pFog,Z_Third);

	//布置特殊关卡;
	if (idx>=ci_MapNum-ci_ChallengeMissionNum/5)
	{
		int missionId=5*ci_MapNum-idx*5;	//本单元最大关卡ID;
		for (int i=0;i<5;++i)
		{
			GameChallengeMission* mission_right = GameChallengeMission::create();
			mission_right->setTag(missionId);
			mission_right->setAnchorPoint(Vec2::ZERO);
			//取得关卡ID对应的坐标;
			Vec2 temp=GameUIData::getInstance()->getChallengeMissionPos(missionId);
			mission_right->setPosition(Vec2(temp.x+639,temp.y));
			cell->addChild(mission_right);
			mission_right->setMissionPorperty(missionId);
			mission_right->missionShow(missionId);

			--missionId;
		}
	}
    return cell;
}

// 单元格数量;
ssize_t OverTableView::numberOfCellsInTableView(TableView *table)
{
    return cellNum;
}

// 触摸事件;
void OverTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
    //log("第%zi个单元格;", cell->getIdx());
    //tableView->setContentOffset(Vec2(0,tableView->getContentOffset().y-20.f));
}

void OverTableView::scrollViewDidScroll(extension::ScrollView* view)
{
    Point now = tableView->getContentOffset();
    if (now.y<-window_size.height*(cellNum-1))
    {
        tableView->setContentOffset(Vec2(0,-window_size.height*(cellNum-1)));
    }
    else if (now.y>0)
    {
        tableView->setContentOffset(Vec2(0,0));
    }
}

void OverTableView::scrollViewForDistance( float distance /*= -1*/ )
{
    if (distance==-1)
    {
        tableView->setContentOffset(Vec2(0,(ci_MapNum-1)*GLB_SIZE.height));//tableView->getContentSize().height
    }
    else
    {
        tableView->setContentOffsetInDuration(Vec2(0.f,tableView->getContentOffset().y+distance),1.f);	// 地图上移;
    }
}


