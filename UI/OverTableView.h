//
//  OverTableView.h
//  author:Simon
//
//  Created by 5agame on 15/5/14.
//
//

#ifndef __XXL__OverTableView__
#define __XXL__OverTableView__

#include <iostream>
#include "cocos2d.h"
#include <extensions/cocos-ext.h>

USING_NS_CC;
using namespace extension;

#include "GameNormalMission.h"

class OverTableView : public Layer,public TableViewDataSource,public TableViewDelegate
{   
public:

	CREATE_FUNC(OverTableView);
	virtual bool init();

	TableView* tableView;

	//TableViewDataSource的抽象方法
	virtual Size cellSizeForTable(TableView* table);						//单元大小;
	virtual TableViewCell* tableCellAtIndex(TableView* table,ssize_t idx);  //编辑单元;
	virtual ssize_t numberOfCellsInTableView(TableView* table);				//单元个数;

	//TableViewDelegate的抽象方法
	virtual void tableCellTouched(TableView* table,TableViewCell* cell);

	//TableViewDelegate继承自ScrollView的抽象方法
	virtual void scrollViewDidScroll(extension::ScrollView* view);
	virtual void scrollViewDidZoom(extension::ScrollView* view){};

	CC_SYNTHESIZE(Size,window_size,WindowSize);
	CC_SYNTHESIZE(ssize_t,cellNum,CellNum);
	CC_SYNTHESIZE(bool,fileMap,FlipMap);
	// view移动，默认是移动到底部;
	void scrollViewForDistance(float distance = -1);
	vector<GameNormalMission*> vec_normalMission;

protected:
	OverTableView();
	~OverTableView();
    Sprite* m_pMengBan;      // 特殊关卡蒙灰;
};

#endif /* defined(__XXL__OverTableView__) */
