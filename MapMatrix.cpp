#include "MapMatrix.h"


MapMatrix::MapMatrix(int **mm,Gem ***fm, int count,MapInfo* mapinfo)
:_mapMatrix(mm)
,_gemMatrix(fm)
,edgeLength(count)
,_mapInfo(mapinfo)
{
    int i,j;
    
    for(int i=0;i<kMatrixWidth;i++)
    {
        _newGemCounter[i] = 0;
        
        _newGemCounterFromMiddle[i] = 0;
        
        fallDownFlagArray[i]=0;   //填空
    }
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_mapMatrix[i][j]==NoneInMiddle||(j==kMatrixWidth-1&&_mapMatrix[i][j]!=NoneInEdge))
            {
                fallDownFlagArray[i]=1;
                continue;
            }
        }
    }
    
    int distance;
    
    for(i=0;i<kMatrixWidth;i++)
    {
        distance = kMatrixWidth;
        
        if(fallDownFlagArray[i]==0)
        {
            for(j=i+1;j<kMatrixWidth;j++)
            {
                if(fallDownFlagArray[j]==1&&distance>j-i+1)
                {
                    distance=j-i+1;
                    continue;
                }
            }
            for(j=i-1;j>=0;j--)
            {
                if(fallDownFlagArray[j]==1&&distance>i-j+1)
                {
                    distance=i-j+1;
                    continue;
                }
            }
            if(distance==kMatrixWidth)
            {
                fallDownFlagArray[i]=0;
            }
            else
            {
                fallDownFlagArray[i] = distance;
            }
        }
    }
    
}
MapMatrix::~MapMatrix()
{
    
}
void MapMatrix::findConnection()
{
    int i,j,_count;
    Gem* tempGem;
    
    connectionAreaVector.clear();
    
    for(i=0;i<edgeLength;i++)
    {
        for(j=0;j<edgeLength;j++)
        {
            if(!_gemMatrix[i][j])
            {
                continue;
            }
            else if(_gemMatrix[i][j]->getGemSkill() == SkillCross || _gemMatrix[i][j]->getGemSkill() == SkillThreeCross || _gemMatrix[i][j]->getGemSkill() == SkillAround4)
            {
                ConnectionArea ca;
                ca.startPoint.x = i;
                ca.startPoint.y = j;
                ca.count = 1;
                ca.direction = Up;
                connectionAreaVector.push_back(ca);
            }
            
        }
    }

    
//    for(i=0;i<edgeLength;i++)
//    {
//        for(j=0;j<edgeLength;j++)
//        {
//            if(!_gemMatrix[i][j])
//            {
//                continue;
//            }
//            else if(_gemMatrix[i][j]->getGemSkill() == SkillAround1)
//            {
//                ConnectionArea ca;
//                ca.startPoint.x = i;
//                ca.startPoint.y = j;
//                ca.count = 0;
//                ca.direction = Up;
//                connectionAreaVector.push_back(ca);
//            }
//            
//        }
//    }
//    
//    for(i=0;i<edgeLength;i++)
//    {
//        for(j=0;j<edgeLength;j++)
//        {
//            if(!_gemMatrix[i][j])
//            {
//                continue;
//            }
//            else if(_gemMatrix[i][j]->getAutoSkill())
//            {
//                ConnectionArea ca;
//                ca.startPoint.x = i;
//                ca.startPoint.y = j;
//                ca.count = 0;
//                ca.direction = Up;
//                connectionAreaVector.push_back(ca);
//            }
//            
//        }
//    }
    
    for(i=0;i<edgeLength;i++)
    {
        for(j=0;j<edgeLength;j++)
        {
            if(!_gemMatrix[i][j])
            {
                continue;
            }
            else
            {
                tempGem = _gemMatrix[i][j];
                
                int _count = 0;
                
                do{
                    j++;
                    _count++;
                }while(j<edgeLength&&tempGem->equal(_gemMatrix[i][j]));
                
                if(_count>=3)
                {
                    ConnectionArea ca;
                    ca.startPoint.x = i;
                    ca.startPoint.y = j - _count;
                    ca.count = _count;
                    ca.direction = Up;
                    connectionAreaVector.push_back(ca);
                }
                if(j+1>=edgeLength)
                {
                    break;
                }
                else
                {
                    j--;
                }
            }
            
        }
    }
    
    for(j=0;j<edgeLength;j++)
    {
        for(int i=0;i<edgeLength;i++)
        {
            if(!_gemMatrix[i][j])
            {
                continue;
            }
            else
            {
                tempGem = _gemMatrix[i][j];
                
                _count = 0;
                
                do{
                    i++;
                    _count++;
                }while(i<edgeLength&&tempGem->equal(_gemMatrix[i][j]));
                
                if(_count>=3)
                {
                    ConnectionArea ca;
                    ca.startPoint.x = i - _count;
                    ca.startPoint.y = j;
                    ca.count = _count;
                    ca.direction = Right;
                    connectionAreaVector.push_back(ca);
                }
                if(i+1>=edgeLength)
                {
                    break;
                }
                else
                {
                    i--;
                }
            }
        }
    }
}

void MapMatrix::findPotential()
{
    int i,j;
    
    Gem *f,*f1,*f2,*f3,*f4;
    
    potentialAreaVector.clear();
    
    for(i=0;i<edgeLength;i++)
    {
        for(j=0;j<edgeLength;j++)
        {
            if(!_gemMatrix[i][j]||(_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove()==false))
            {
                continue;
            }
            else
            {
                f = _gemMatrix[i][j];
                
                f1=f2=f3=f4 = NULL;
                
                if(i-1>=0&&j-1>=0&&_gemMatrix[i-1][j-1])
                    f1 = _gemMatrix[i-1][j-1];
                if(i-1>=0&&j+1<edgeLength&&_gemMatrix[i-1][j+1])
                    f2 = _gemMatrix[i-1][j+1];
                if(i+1<edgeLength&&j+1<edgeLength&&_gemMatrix[i+1][j+1])
                    f3 = _gemMatrix[i+1][j+1];
                if(i+1<edgeLength&&j-1>=0&&_gemMatrix[i+1][j-1])
                    f4 = _gemMatrix[i+1][j-1];
                if(f->equal(f1)&&f->equal(f2)&&_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove()&&_gemMatrix[i-1][j]->getGemSkill() < SkillAllSame)
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i,j);
                    pa.point1.setPosition(i-1, j-1);
                    pa.point2.setPosition(i-1, j+1);
                    pa.diretion = Left;
                    potentialAreaVector.push_back(pa);
                }
                if(f->equal(f2)&&f->equal(f3)&&_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove()&&_gemMatrix[i][j+1]->getGemSkill() < SkillAllSame)
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i, j);
                    pa.point1.setPosition(i-1, j+1);
                    pa.point2.setPosition(i+1, j+1);
                    pa.diretion = Up;
                    potentialAreaVector.push_back(pa);
                }
                if(f->equal(f3)&&f->equal(f4)&&_gemMatrix[i+1][j]&&_gemMatrix[i+1][j]->canMove()&&_gemMatrix[i+1][j]->getGemSkill() < SkillAllSame)
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i, j);
                    pa.point1.setPosition(i+1, j+1);
                    pa.point2.setPosition(i+1,j-1);
                    pa.diretion = Right;
                    potentialAreaVector.push_back(pa);
                }
                if(f->equal(f4)&&f->equal(f1)&&_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove()&&_gemMatrix[i][j-1]->getGemSkill() < SkillAllSame)
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i, j);
                    pa.point1.setPosition(i-1, j-1);
                    pa.point2.setPosition(i+1, j-1);
                    pa.diretion = Down;
                    potentialAreaVector.push_back(pa);
                }
                
            }
        }
    }
    
    for(i=0;i<edgeLength;i++)
    {
        for(j=0;j<edgeLength;j++)
        {
            if(!_gemMatrix[i][j])
            {
                continue;
            }
            else
            {
                f = _gemMatrix[i][j];
                
                if(j+1<edgeLength&&f->equal(_gemMatrix[i][j+1]))
                {
                    if(i+1<edgeLength&&j+2<edgeLength&&f->equal(_gemMatrix[i+1][j+2])&&_gemMatrix[i+1][j+2]->canMove()&&_gemMatrix[i+1][j+2]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove()&&_gemMatrix[i][j+2]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i+1, j+2);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Left;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(j+3<edgeLength&&f->equal(_gemMatrix[i][j+3])&&_gemMatrix[i][j+3]->canMove()&&_gemMatrix[i][j+3]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove()&&_gemMatrix[i][j+2]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i, j+3);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Down;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i-1>=0&&j+2<edgeLength&&f->equal(_gemMatrix[i-1][j+2])&&_gemMatrix[i-1][j+2]->canMove()&&_gemMatrix[i-1][j+2]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j+2]&&_gemMatrix[i][j+2]->canMove()&&_gemMatrix[i][j+2]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i-1, j+2);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Right;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    
                    if(i+1<edgeLength&&j-1>=0&&f->equal(_gemMatrix[i+1][j-1])&&_gemMatrix[i+1][j-1]->canMove()&&_gemMatrix[i+1][j-1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove()&&_gemMatrix[i][j-1]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i+1, j-1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Left;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(j-2>=0&&f->equal(_gemMatrix[i][j-2])&&_gemMatrix[i][j-2]->canMove()&&_gemMatrix[i][j-2]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove()&&_gemMatrix[i][j-1]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i, j-2);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Up;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i-1>=0&&j-1>=0&&f->equal(_gemMatrix[i-1][j-1])&&_gemMatrix[i-1][j-1]->canMove()&&_gemMatrix[i-1][j-1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i][j-1]&&_gemMatrix[i][j-1]->canMove()&&_gemMatrix[i][j-1]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i-1, j-1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i, j+1);
                            pa.diretion = Right;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                }
            }
        }
    }
    
    for(j=0;j<edgeLength;j++)
    {
        for(i=0;i<edgeLength;i++)
        {
            if(!_gemMatrix[i][j])
            {
                continue;
            }
            else
            {
                f = _gemMatrix[i][j];
                
                if(i+1<edgeLength&&f->equal(_gemMatrix[i+1][j]))
                {
                    if(i+2<edgeLength&&j-1>=0&&f->equal(_gemMatrix[i+2][j-1])&&_gemMatrix[i+2][j-1]->canMove()&&_gemMatrix[i+2][j-1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove()&&_gemMatrix[i+2][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i+2, j-1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Up;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i+3<edgeLength&&f->equal(_gemMatrix[i+3][j])&&_gemMatrix[i+3][j]->canMove()&&_gemMatrix[i+3][j]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove()&&_gemMatrix[i+2][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i+3, j);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Left;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i+2<edgeLength&&j+1<edgeLength&&f->equal(_gemMatrix[i+2][j+1])&&_gemMatrix[i+2][j+1]->canMove()&&_gemMatrix[i+2][j+1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i+2][j]&&_gemMatrix[i+2][j]->canMove()&&_gemMatrix[i+2][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i+2, j+1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Down;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    
                    if(i-1>=0&&j-1>=0&&f->equal(_gemMatrix[i-1][j-1])&&_gemMatrix[i-1][j-1]->canMove()&&_gemMatrix[i-1][j-1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove()&&_gemMatrix[i-1][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i-1, j-1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Up;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i-2>=0&&f->equal(_gemMatrix[i-2][j])&&_gemMatrix[i-2][j]->canMove()&&_gemMatrix[i-2][j]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove()&&_gemMatrix[i-1][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i-2, j);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Right;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                    if(i-1>=0&&j+1<edgeLength&&f->equal(_gemMatrix[i-1][j+1])&&_gemMatrix[i-1][j+1]->canMove()&&_gemMatrix[i-1][j+1]->getGemSkill() < SkillAllSame)
                    {
                        if(_gemMatrix[i-1][j]&&_gemMatrix[i-1][j]->canMove()&&_gemMatrix[i-1][j]->getGemSkill() < SkillAllSame)
                        {
                            PotentialArea pa;
                            pa.movePoint.setPosition(i-1, j+1);
                            pa.point1.setPosition(i, j);
                            pa.point2.setPosition(i+1, j);
                            pa.diretion = Down;
                            potentialAreaVector.push_back(pa);
                        }
                    }
                }
                
            }
        }
    }
    
    for(i=0;i<edgeLength;i++)
    {
        for(j=0;j<edgeLength;j++)
        {
            if(!_gemMatrix[i][j]||(_gemMatrix[i][j]&&_gemMatrix[i][j]->canMove()==false))
            {
                continue;
            }
            else if(_gemMatrix[i][j]->getGemSkill())
            {
                f = _gemMatrix[i][j];
                
                f1=f2=f3=f4 = NULL;
                
                if(i-1>=0 && _gemMatrix[i-1][j])
                {
                    f1 = _gemMatrix[i-1][j];
                }
                if(i+1<edgeLength&&_gemMatrix[i+1][j])
                {
                    f2 = _gemMatrix[i+1][j];
                }
                if(j-1>0 && _gemMatrix[i][j-1])
                {
                    f3 = _gemMatrix[i][j-1];
                }
                if(j+1<edgeLength && _gemMatrix[i][j+1])
                {
                    f4 = _gemMatrix[i][j+1];
                }
                
                if (f->hasSkill(f1))
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i,j);
                    pa.point1.setPosition(i-1, j);
                    pa.point2.setPosition(-1, -1);
                    pa.diretion = StandBy;
                    potentialAreaVector.push_back(pa);
                }
                if (f->hasSkill(f2))
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i,j);
                    pa.point1.setPosition(i+1, j);
                    pa.point2.setPosition(-1, -1);
                    pa.diretion = StandBy;
                    potentialAreaVector.push_back(pa);
                }
                if (f->hasSkill(f3))
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i,j);
                    pa.point1.setPosition(i, j-1);
                    pa.point2.setPosition(-1, -1);
                    pa.diretion = StandBy;
                    potentialAreaVector.push_back(pa);
                }
                if (f->hasSkill(f4))
                {
                    PotentialArea pa;
                    pa.movePoint.setPosition(i,j);
                    pa.point1.setPosition(i, j+1);
                    pa.point2.setPosition(-1, -1);
                    pa.diretion = StandBy;
                    potentialAreaVector.push_back(pa);
                }
                
            }
        }
    }

}

void MapMatrix::fillEmptyNew(Layer* layer, int i, int j)
{
    if(j+1>=kMatrixWidth||NoneInMiddle==_mapMatrix[i][j+1])//正上方达到断层或者外层
    {
        int* counterArray;
        
        if(j+1>=edgeLength)
        {
            counterArray = _newGemCounter;
        }
        else if(NoneInMiddle==_mapMatrix[i][j+1])
        {
            counterArray = _newGemCounterFromMiddle;
        }
        
        Gem *gem = Gem::createRandomGem(layer,Point(kStartX + i * kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);
        
        counterArray[i]++;
        
        for(int k=1;k<counterArray[i];k++)
        {
            ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);
            
            gem->addAction(action);
        }
        CallFunc* actionFunc = CallFunc::create(CC_CALLBACK_0(Gem::showSprite, gem));
        
        gem->addAction(actionFunc);
        
        ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);
        
        gem->addAction(actionDown);
        
        _gemMatrix[i][j] = gem;
        
        return;
    }
    
    if(_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove())//正上方元素不为空且可以移动
    {
        ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);
        
        _gemMatrix[i][j+1]->addAction(actionDown);
        
        _gemMatrix[i][j] = _gemMatrix[i][j+1];
        
        _gemMatrix[i][j+1] = NULL;
        
        fillEmptyNew(layer, i, j+1);
        
        return;
    }
    
    if(!_gemMatrix[i][j+1]&&_mapMatrix[i][j+1]!=0)//正上方为空并且可以有元素
    {
        fillEmptyNew(layer, i, j+1);
        
        if(_gemMatrix[i][j+1]&&_gemMatrix[i][j+1]->canMove())
        {
            ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(Down);
            
            _gemMatrix[i][j+1]->addAction(actionDown);
            
            _gemMatrix[i][j] = _gemMatrix[i][j+1];
            
            _gemMatrix[i][j+1] = NULL;
            
            fillEmptyNew(layer, i, j+1);
            
            return;
        }
    }
    //到此处还未填满，则从左上及右上填充
    
    if(i-1>=0)
    {
        if(_gemMatrix[i-1][j+1]&&_gemMatrix[i-1][j+1]->canMove())//左侧不为空并且可以移动
        {
            ActionInterval* actionRightDown = GemAction::getInstance().getGemMoveAction(RightDown);
            
            _gemMatrix[i-1][j+1]->addAction(actionRightDown);
            
            _gemMatrix[i][j] = _gemMatrix[i-1][j+1];
            
            _gemMatrix[i-1][j+1] = NULL;
            
            fillEmptyNew(layer, i-1, j+1);
            
            return;
        }
        if(!_gemMatrix[i-1][j+1]&&_mapMatrix[i-1][j+1]!=NoneInEdge&&_mapMatrix[i-1][j+1]!=NoneInMiddle)//左侧为空且可填充
        {
            fillEmptyNew(layer, i-1, j+1);
            
            if(_gemMatrix[i-1][j+1]&&_gemMatrix[i-1][j+1]->canMove())
            {
                ActionInterval* actionRightDown = GemAction::getInstance().getGemMoveAction(RightDown);
                
                _gemMatrix[i-1][j+1]->addAction(actionRightDown);
                
                _gemMatrix[i][j] = _gemMatrix[i-1][j+1];
                
                _gemMatrix[i-1][j+1] = NULL;
                
                fillEmptyNew(layer, i-1, j+1);
                
                return;
            }
        }
        if(_mapMatrix[i-1][j+1]==NoneInMiddle)//左上方为断层，递归出口，与正上方为断层同一级别
        {
            int* counterArray = _newGemCounterFromMiddle;
            
            Gem *gem = Gem::createRandomGem(layer,Point(kStartX + (i-1)* kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);
            
            counterArray[i]++;
            
            for(int k=1;k<counterArray[i];k++)
            {
                ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);
                
                gem->addAction(action);
            }
            CallFunc* actionFunc = CallFunc::create(CC_CALLBACK_0(Gem::showSprite, gem));
            
            gem->addAction(actionFunc);
            
            ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(RightDown);
            
            gem->addAction(actionDown);
            
            _gemMatrix[i][j] = gem;
            
            return;
        }
    }
    
    if(i+1<kMatrixWidth)
    {
        if(_gemMatrix[i+1][j+1]&&_gemMatrix[i+1][j+1]->canMove())
        {
            ActionInterval* actionLeftDown = GemAction::getInstance().getGemMoveAction(LeftDown);
            
            _gemMatrix[i+1][j+1]->addAction(actionLeftDown);
            
            _gemMatrix[i][j] = _gemMatrix[i+1][j+1];
            
            _gemMatrix[i+1][j+1] = NULL;
            
            fillEmptyNew(layer, i+1, j+1);
            
            return;
        }
        if(!_gemMatrix[i+1][j+1]&&_mapMatrix[i+1][j+1]!=NoneInEdge&&_mapMatrix[i+1][j+1]!=NoneInMiddle)//右侧为空且可以填充
        {
            //填充自身(i,j)
            fillEmptyNew(layer,i+1,j+1);
            
            if(_gemMatrix[i+1][j+1]&&_gemMatrix[i+1][j+1]->canMove())
            {
                ActionInterval* actionLeftDown = GemAction::getInstance().getGemMoveAction(LeftDown);
                
                _gemMatrix[i+1][j+1]->addAction(actionLeftDown);
                
                _gemMatrix[i][j] = _gemMatrix[i+1][j+1];
                
                _gemMatrix[i+1][j+1] = NULL;
                
                fillEmptyNew(layer, i+1, j+1);
                
                return;
            }
        }
        if(_mapMatrix[i+1][j+1]==NoneInMiddle)//右上为断层，可以填充
        {
            int* counterArray = _newGemCounterFromMiddle;
            
            Gem *gem = Gem::createRandomGem(layer,Point(kStartX + (i+1)* kElementSize + kElementSize/2, kStartY + (j+1) * kElementSize + kElementSize/2),false);
            
            counterArray[i]++;
            
            for(int k=1;k<counterArray[i];k++)
            {
                ActionInterval* action =GemAction::getInstance().getGemMoveAction(StandBy);
                
                gem->addAction(action);
            }
            CallFunc* actionFunc = CallFunc::create(CC_CALLBACK_0(Gem::showSprite, gem));
            
            gem->addAction(actionFunc);
            
            ActionInterval* actionDown = GemAction::getInstance().getGemMoveAction(LeftDown);
            
            gem->addAction(actionDown);
            
            _gemMatrix[i][j] = gem;
            
            return;
        }
    }
    
}
//拼接地图框
SpriteBatchNode* MapMatrix::createMapFrame(Node* mapNode)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "map.plist" );
    
    SpriteBatchNode* batchNode = SpriteBatchNode::create("map.png",1000);
    
    batchNode->setPosition(Vec2::ZERO);
    
    mapNode->addChild(batchNode,-1);
    
    int i,j;
    
    int tm[kMatrixWidth+2][kMatrixWidth+2]={0};
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(NoneInEdge==_mapMatrix[i][j]||NoneInMiddle==_mapMatrix[i][j])
            {
                tm[i+1][j+1]=0;
            }
            else
            {
                tm[i+1][j+1]=1;
            }
        }
    }
    Sprite* backspr = NULL;
    
    for(i=1;i<kMatrixWidth+1;i++)
    {
        for(j=1;j<kMatrixWidth+1;j++)
        {
            if(tm[i][j])
            {
                if(i%2==0)
                {
                    if(j%2==0)
                    {
                        backspr= Sprite::createWithSpriteFrameName("battlemap_5_dark.png");
                    }
                    else
                    {
                        backspr= Sprite::createWithSpriteFrameName("battlemap_5_light.png");
                    }
                }
                else
                {
                    if(j%2==0)
                    {
                        backspr= Sprite::createWithSpriteFrameName("battlemap_5_light.png");
                    }
                    else
                    {
                        backspr= Sprite::createWithSpriteFrameName("battlemap_5_dark.png");
                    }
                }
//                backspr->setScale(60.0/70);
                backspr->setPosition(Point(kStartX+(i-1+0.5)*kElementSize,kStartY+(j-1+0.5)*kElementSize));
                
                batchNode->addChild(backspr);
            }
        }
    }
//    for(i=1;i<kMatrixWidth+1;i++)
//    {
//        for(j=1;j<kMatrixWidth+1;j++)
//        {
//            if(tm[i][j])
//            {
//                if(!tm[i-1][j]&&!tm[i][j-1])//左下命中
//                {
//                    if(tm[i-1][j-1])
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back2x_1.png");
//                        
//                        spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));
//                        
//                        spr->setTag(13);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                    else
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back1_4.png");
//                        
//                        spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));
//                        
//                        spr->setTag(4);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                }
//                if (!tm[i-1][j]&&!tm[i][j+1])//左上命中
//                {
//                    if(tm[i-1][j+1])
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back2x_2.png");
//                        
//                        spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));
//                        
//                        spr->setTag(14);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                    else
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back1_1.png");
//                        
//                        spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));
//                        
//                        spr->setTag(1);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                    
//                }
//                if (!tm[i+1][j]&&!tm[i][j-1])//右下命中
//                {
//                    if(!tm[i+1][j-1])
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back1_3.png");
//                        
//                        spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ (j-1) *kElementSize));
//                        
//                        spr->setTag(3);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                    
//                }
//                if (!tm[i+1][j]&&!tm[i][j+1])//右上命中
//                {
//                    if(!tm[i+1][j+1])
//                    {
//                        Sprite* spr = Sprite::createWithSpriteFrameName("back1_2.png");
//                        
//                        spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ j *kElementSize));
//                        
//                        spr->setTag(2);
//                        
//                        spr->setScale(60.0/128);
//                        
//                        batchNode->addChild(spr);
//                    }
//                    
//                }
//            }
//            else
//            {
//                if(tm[i-1][j]&&tm[i][j-1]&&tm[i-1][j-1])//左下命中
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back3_2.png");
//                    
//                    spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+(j-1)*kElementSize));
//                    
//                    spr->setTag(10);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                if (tm[i-1][j]&&tm[i][j+1]&&tm[i-1][j+1])//左上命中
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back3_3.png");
//                    
//                    spr->setPosition(Point(kStartX+(i-1)*kElementSize,kStartY+ j *kElementSize));
//                    
//                    spr->setTag(11);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                if (tm[i+1][j]&&tm[i][j-1]&&tm[i+1][j-1])//右下命中
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back3_1.png");
//                    
//                    spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ (j-1) *kElementSize));
//                    
//                    spr->setTag(9);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                if (tm[i+1][j]&&tm[i][j+1]&&tm[i+1][j+1])//右上命中
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back3_4.png");
//                    
//                    spr->setPosition(Point(kStartX+ i *kElementSize,kStartY+ j *kElementSize));
//                    
//                    spr->setTag(12);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//            }
//        }
//    }
//    
//    int jIndex,iIndex,maxSize,k;
//    
//    for(i=1;i<=kMatrixWidth+1;i++)
//    {
//        for (j=1;j<=kMatrixWidth+1;j++)
//        {
//            jIndex = j;
//            maxSize = 0;
//            
//            while(tm[i][jIndex]&&!tm[i-1][jIndex])
//            {
//                jIndex++;
//                maxSize++;
//            }
//            if(maxSize>=2)
//            {
//                for(k=1;k<maxSize;k++)
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back2_2.png");
//                    
//                    spr->setPosition(Point(kStartX+ (i-1) *kElementSize,kStartY+ (j-1+k) *kElementSize));
//                    
//                    spr->setTag(6);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                
//                j = jIndex;
//                continue;
//            }
//        }
//    }
//    for(i=1;i<=kMatrixWidth+1;i++)
//    {
//        for (j=1;j<=kMatrixWidth+1;j++)
//        {
//            jIndex = j;
//            maxSize =0;
//            while(!tm[i][jIndex]&&tm[i-1][jIndex])
//            {
//                jIndex++;
//                maxSize++;
//            }
//            if(maxSize>=2)
//            {
//                for(k=1;k<maxSize;k++)
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back2_4.png");
//                    
//                    spr->setPosition(Point(kStartX+ (i-1) *kElementSize,kStartY+ (j-1+k) *kElementSize));
//                    
//                    spr->setTag(8);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                j=jIndex;
//                continue;
//            }
//        }
//    }
//    
//    for(j=1;j<=kMatrixWidth+1;j++)
//    {
//        for (i=1;i<=kMatrixWidth+1;i++)
//        {
//            iIndex = i;
//            maxSize = 0;
//            
//            while(tm[iIndex][j]&&!tm[iIndex][j-1])
//            {
//                iIndex++;
//                maxSize++;
//            }
//            if(maxSize>=2)
//            {
//                for(k=1;k<maxSize;k++)
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back2_1.png");
//                    
//                    spr->setPosition(Point(kStartX+ (i-1+k) *kElementSize,kStartY+ (j-1) *kElementSize));
//                    
//                    spr->setTag(5);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                i=iIndex;
//                continue;
//            }
//        }
//    }
//    for(j=1;j<=kMatrixWidth+1;j++)
//    {
//        for (i=1;i<=kMatrixWidth+1;i++)
//        {
//            iIndex = i;
//            maxSize =0;
//            while(!tm[iIndex][j]&&tm[iIndex][j-1])
//            {
//                iIndex++;
//                maxSize++;
//            }
//            if(maxSize>=2)
//            {
//                for(k=1;k<maxSize;k++)
//                {
//                    Sprite* spr = Sprite::createWithSpriteFrameName("back2_3.png");
//                    
//                    spr->setPosition(Point(kStartX+ (i-1+k) *kElementSize,kStartY+ (j-1) *kElementSize));
//                    
//                    spr->setTag(7);
//                    
//                    spr->setScale(60.0/128);
//                    
//                    batchNode->addChild(spr);
//                }
//                i=iIndex;
//                continue;
//            }
//        }
//    }
    return batchNode;
}

//根据递归函数得到的动作队列，加入回调并运行

void MapMatrix::clearCounter()
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        _newGemCounter[i] = 0;
        
        _newGemCounterFromMiddle[i] = 0;
    }
}

void MapMatrix::markeMatrixChange()
{
//    vector<ConnectionArea>::iterator it;
//    
//    int i,j;
//    //处理元素，生成动画队列
//    
//    //预处理，对所有相连元素标记
//    for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
//    {
//        if (it->direction == Up)
//        {
//            for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
//            {
//                if(_gemMatrix[i][j])
//                {
//                    _gemMatrix[i][j]->setChange(Eliminate);
//                }
//            }
//        }
//        else
//        {
//            for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
//            {
//                if(_gemMatrix[i][j])
//                {
//                    _gemMatrix[i][j]->setChange(Eliminate);
//                }
//            }
//        }
//    }
//    //标记周围影响元素，并可记录影响次数
//    for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
//    {
//        if (it->direction == Up)
//        {
//            for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
//            {
//                if(!_gemMatrix[i][j]->canMove())//如果本元素被冰冻或者被束缚，则不能影响周围元素
//                {
//                    continue;
//                }
//                if(j==it->startPoint.y)
//                {
//                    if(j-1>=0&&_gemMatrix[i][j-1])
//                    {
//                        _gemMatrix[i][j-1]->addChange();
//                    }
//                }
//                else if(j==it->startPoint.y+it->count-1)
//                {
//                    if(j+1<kMatrixWidth&&_gemMatrix[i][j+1])
//                    {
//                        _gemMatrix[i][j+1]->addChange();
//                    }
//                }
//                
//                if(i-1>=0&&_gemMatrix[i-1][j])
//                {
//                    _gemMatrix[i-1][j]->addChange();
//                }
//                if(i+1<kMatrixWidth&&_gemMatrix[i+1][j])
//                {
//                    _gemMatrix[i+1][j]->addChange();
//                }
//            }
//        }
//        else
//        {
//            for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
//            {
//                if(!_gemMatrix[i][j]->canMove())//如果本元素被冰冻或者被束缚，则不能影响周围元素
//                {
//                    continue;
//                }
//                if(i ==it->startPoint.x)
//                {
//                    if(i-1>=0&&_gemMatrix[i-1][j])
//                    {
//                        _gemMatrix[i-1][j]->addChange();
//                    }
//                }
//                else if(i==it->startPoint.x+it->count-1)
//                {
//                    if(i+1<kMatrixWidth&&_gemMatrix[i+1][j])
//                    {
//                        _gemMatrix[i+1][j]->addChange();
//                    }
//                }
//                
//                if(j-1>=0&&_gemMatrix[i][j-1])
//                {
//                    _gemMatrix[i][j-1]->addChange();
//                }
//                if(j+1<kMatrixWidth&&_gemMatrix[i][j+1])
//                {
//                    _gemMatrix[i][j+1]->addChange();
//                }
//            }
//        }
//    }
    
}

void Disjoinset::makeset()
{
    for(int i=0;i<kMatrixWidth*kMatrixWidth;i++)
    {
        pre[i] = i;
        rank[i]= 0;
    }
}

int Disjoinset::find(int x)
{
    if(x!=pre[x])
    {
        pre[x]=find(pre[x]);
    }
    return pre[x];
}

void Disjoinset::unionone(int a, int b)
{
    int t1 = find(a);
    
    int t2 = find(b);
    
    if(rank[t1]>rank[t2])
    {
        pre[t2] = t1;
    }
    else
    {
        pre[t1] = t2;
    }
    
    if(rank[t1]==rank[t2])
    {
        rank[t2]++;
    }
}

void MapMatrix::dealSick()
{
//    int i,j,k,l;
//    
//    int sickGem[kMatrixWidth*kMatrixWidth] = {0};
//    
//    bool hasSick = false;
//    
//    for(k=0;k<connectionAreaVector.size();k++)
//    {
//        ConnectionArea ca = connectionAreaVector[k];
//        
//        if (ca.direction == Up)
//        {
//            for (j = ca.startPoint.y,i = ca.startPoint.x; j < ca.startPoint.y + ca.count; j++)
//            {
//                if(_gemMatrix[i][j]->getSick())
//                {
//                    sickGem[k]=1;
//                    hasSick = true;
//                }
//            }
//        }
//        else
//        {
//            for (i = ca.startPoint.x,j = ca.startPoint.y; i < ca.startPoint.x + ca.count; i++)
//            {
//                if(_gemMatrix[i][j]->getSick())
//                {
//                    sickGem[k]=1;
//                    hasSick = true;
//                }
//            }
//        }
//    }
//    
//    if(!hasSick) return;
//    
//    MyPoint crossPoint;
//    
//    Disjoinset ds = Disjoinset::getInstance();
//    
//    ds.makeset();
//    
//    GemType type;
//    
//    for(i=0;i<connectionAreaVector.size();i++)
//    {
//        MyPoint startPoint1 = connectionAreaVector[i].startPoint;
//        
//        type = _gemMatrix[startPoint1.x][startPoint1.y]->getGemType();
//        
//        for(j=i+1;j<connectionAreaVector.size();j++)
//        {
//            MyPoint startPoint2 = connectionAreaVector[j].startPoint;
//            
//            if(_gemMatrix[startPoint2.x][startPoint2.y]->getGemType()!=type)
//            {
//                continue;
//            }
//            
//            if(connectionAreaVector[i].isCross(connectionAreaVector[j], crossPoint))
//            {
//                ds.unionone(i, j);
//            }
//        }
//    }
//    
//    for(k=0;k<connectionAreaVector.size();k++)
//    {
//        if(sickGem[k]==1)
//        {
//            int index = ds.pre[k];
//            
//            for(l=0;l<connectionAreaVector.size();l++)
//            {
//                if(ds.pre[l]==index)
//                {
//                    ConnectionArea ca = connectionAreaVector[l];
//                    
//                    MyPoint startPoint = ca.startPoint;
//                    
//                    if (ca.direction == Up)
//                    {
//                        for (j = startPoint.y,i = startPoint.x; j < startPoint.y + ca.count; j++)
//                        {
//                            _gemMatrix[i][j]->setSick(true);
//                        }
//                    }
//                    else
//                    {
//                        for (i = startPoint.x,j = startPoint.y; i < startPoint.x + ca.count; i++)
//                        {
//                            _gemMatrix[i][j]->setSick(true);
//                        }
//                    }
//                }
//            }
//        }
//    }
}


void MapMatrix::findCenterPoint(MyPoint &selectPoint, MyPoint &nextPoint)
{
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    
    //处理所有相连的条，找到中心位置（可能为移动点，也可能为头结点，重置相交标志为空）
    for(it=connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
    {
        it->isInCross=false;
        
        if(it->isContain(selectPoint)||it->isContain(nextPoint))
        {
            if(it->isContain(selectPoint))
            {
                it->centerPoint.setPosition(selectPoint.x,selectPoint.y);
            }
            if(it->isContain(nextPoint))
            {
                it->centerPoint.setPosition(nextPoint.x,nextPoint.y);
            }
        }
        else//不包含两个移动点,则查找本区域内移动的点，设置为中心点
        {
            if (it->direction == Up)
            {
                for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
                {
                    if(_gemMatrix[i][j]->getActionVectorSize()>0)
                    {
                        it->centerPoint.setPosition(i, j);
                        break;
                    }
                    if(j==it->startPoint.y+it->count-1)
                    {
                        it->centerPoint.setPosition(it->startPoint.x, it->startPoint.y);
                    }
                }
            }
            else
            {
                for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
                {
                    if(_gemMatrix[i][j]->getActionVectorSize()>0)
                    {
                        it->centerPoint.setPosition(i, j);
                        break;
                    }
                    if(i==it->startPoint.x+it->count-1)
                    {
                        it->centerPoint.setPosition(it->startPoint.x, it->startPoint.y);
                    }
                }
            }
        }
        
    }
}

void MapMatrix::tigerEatAnimal(MyPoint& _selectPoint,MyPoint& _nextPoint)
{
    for (int i=0;i<kMatrixWidth; i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getGemType()/*==tiger*/)
            {
                // 				Tiger* tiger = (Tiger*)_gemMatrix[i][j];
                // 
                // 				MyPoint point(i,j);
                // 
                // 				tiger->eatAnimal(point,_selectPoint,_nextPoint );
            }
        }
    }
}

void MapMatrix::matchAllGem(vector<AnimationWraper>* animationWraperVector ,int &continueTimes)
{
    
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    
    //执行所有相连元素的explode函数，其内部会调用周围元素的影响函数affect
    for( it =connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
    {
        j = it->startPoint.y;
        i = it->startPoint.x;
        
        if (it->direction == Up)
        {
            for ( ;j < it->startPoint.y + it->count; j++)
            {
                MyPoint point(i,j);
                
                _gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
                                            it->startPoint.y+it->count-j);
            }
            
        }
        else
        {
            for (; i < it->startPoint.x + it->count; i++)
            {
                MyPoint point(i,j);
                
                _gemMatrix[i][j]->explode(point,it->centerPoint,it->count-2,
                                                it->startPoint.x+it->count-i);
                
            }
        }
        
        if (it->count >= 5)
        {
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setState(0);
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setNextSkill(SkillAllSame);
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setGemType();
        }
        else if(it->isInCross)
        {
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setState(0);
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setNextSkill(SkillAround2);
        }
        else if(it->count == 4)
        {
            _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setState(0);
            if (it->direction == Up)
            {
                _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setNextSkill(SkillHorizontal);
            }
            else
            {
                _gemMatrix[it->centerPoint.x][it->centerPoint.y]->setNextSkill(SkillVerticl);
            }
        }
    }

}

void MapMatrix::renewGemCount()
{
    for(int i=0;i<kMatrixWidth;i++)
    {
        for(int j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j]&&_gemMatrix[i][j]->getChange()!=Eliminate)
            {
                _gemMatrix[i][j]->addScore(0, false);
            }
        }
    }
}
void MapMatrix::growOnFertile()
{
    int i,j;
    //肥沃土地增加
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->growOnFertile(i, j);
            }
        }
    }
}

void MapMatrix::afterMatch()
{
    int i,j;
    
    for(i=0;i<kMatrixWidth;i++)
    {
        for(j=0;j<kMatrixWidth;j++)
        {
            if(_gemMatrix[i][j])
            {
                _gemMatrix[i][j]->afterMatch(i, j);
            }
        }
    }
}

void MapMatrix::beforeMatchGem()
{
    
}

void MapMatrix::dealFallDown(Layer* layer)
{
    int i,j,k;
    
    for(k=0;k<kMatrixWidth;k++)
    {
        for(i=0;i<kMatrixWidth;i++)
        {
            if(fallDownFlagArray[i]==k)
            {
                for(j=0;j<kMatrixWidth;j++)
                {
                    if(_mapMatrix[i][j]!=NoneInEdge&&_mapMatrix[i][j]!=NoneInMiddle&&_gemMatrix[i][j]==NULL)
                    {
                        fillEmptyNew(layer, i, j);
                    }
                }
            }
        }
    }
}

void MapMatrix::dealFallDownColumn(Layer *layer, int row)
{
    int j,k;
    
    for(k=0;k<kMatrixWidth;k++)
    {
        if(fallDownFlagArray[row]==k)
        {
            for(j=0;j<kMatrixWidth;j++)
            {
                if(_mapMatrix[row][j]!=NoneInEdge && _mapMatrix[row][j]!=NoneInMiddle && _gemMatrix[row][j]==NULL)
                {
                    fillEmptyNew(layer, row, j);
                }
            }
        }
    }
}


void MapMatrix::beforeMatchGem(vector<AnimationWraper>* animationWraperVector,MyPoint& selectPoint,MyPoint& nextPoint)
{
    findCenterPoint(selectPoint, nextPoint);
    
    /*	dealSick();*/
    
    int i,j,k,l,sum=0;
    
    int flagArray[kMatrixWidth*kMatrixWidth]={0};
    int sumFlagArray[kMatrixWidth*kMatrixWidth]={0};
    
    Disjoinset ds = Disjoinset::getInstance();
    
    ds.makeset();
    
    MyPoint crossPoint;
    
    vector<ConnectionArea>::iterator it;
    
    for(i = 0;i<connectionAreaVector.size();i++)
    {
        MyPoint startPoint1 = connectionAreaVector[i].startPoint;
        
        GemType ftype = _gemMatrix[startPoint1.x][startPoint1.y]->getGemType();
        
        for (j=0; j<connectionAreaVector.size(); j++)
        {
            MyPoint startPoint2 = connectionAreaVector[j].startPoint;
            
            if(_gemMatrix[startPoint2.x][startPoint2.y]->getGemType()!=ftype)
            {
                continue;
            }
            if(connectionAreaVector[i].isCross(connectionAreaVector[j], crossPoint))
            {
                ds.unionone(i, j);
                
                connectionAreaVector[i].centerPoint.setPosition(crossPoint.x, crossPoint.y);
                
                connectionAreaVector[j].centerPoint.setPosition(crossPoint.x, crossPoint.y);
            }
        }
    }
    
    findParticlePoint(selectPoint, nextPoint);//初始化粒子效果节点
    
    //找五连及十字
    MyPoint particlePoint;
    int  unionSize = 0;
    for(i=0;i<connectionAreaVector.size();i++)
    {
        //已经被处理过
        if(1==flagArray[i])
        {
            continue;
        }
        //取父节点
        int rootIndex = ds.pre[i];
        
        unionSize = 0;
        
        //找同一个集合中是否有大于5相连的出现
        for(j=0;j<connectionAreaVector.size();j++)
        {
            if(0==flagArray[j]&&ds.pre[j]==rootIndex)
            {
                //大于等于5个的相连，生成动画
                if(connectionAreaVector[j].count>=5)
                {
                    
                }
            }
        }
        
        //找同一集合中的十字相连
        for(j=0;j<connectionAreaVector.size();j++)
        {
            if(0==flagArray[j]&&ds.pre[j]==rootIndex)
            {
                unionSize++;
            }
        }
        
        if(unionSize>=2)
        {
            particlePoint = connectionAreaVector[rootIndex].centerPoint;
            
            
            MyPoint myCrossPoint;
            
            for(k=0;k<connectionAreaVector.size();k++)
            {
                if(ds.pre[k]==rootIndex)
                {
                    flagArray[k]=1;
                    
                    if(k!=rootIndex&&connectionAreaVector[k].isCross(connectionAreaVector[rootIndex], myCrossPoint)&&myCrossPoint.equal(particlePoint))
                    {
                        
                    }
                }
            }
            
        }
    }
    //找4连
    for(j=0;j<connectionAreaVector.size();j++)
    {
        if(0==flagArray[j])
        {
            if(connectionAreaVector[j].count==4)
            {
                particlePoint = connectionAreaVector[j].particlePoint;
                
                flagArray[j]=1;
            }
        }
    }
    //生成收集元素总和动画,其中5连及聚合的不算
    for(i=0;i<connectionAreaVector.size();i++)
    {
        MyPoint startPoint = connectionAreaVector[i].startPoint;
        
        GemKind kind = _gemMatrix[startPoint.x][startPoint.y]->getGemKind();
        
        if(sumFlagArray[i]==0&&kind==NormalGem&&!_gemMatrix[startPoint.x][startPoint.y]->getSick())
        {
            int index = ds.pre[i];
            
            for(j=0;j<connectionAreaVector.size();j++)
            {
                if(ds.pre[j]==index)
                {
                    ConnectionArea ca = connectionAreaVector[j];
                    
                    if (ca.direction == Up)
                    {
                        for (l = ca.startPoint.y,k = ca.startPoint.x; l < ca.startPoint.y + ca.count; l++)
                        {
                            if(_gemMatrix[k][l]->canMove())
                            {
                                sum+=_gemMatrix[k][l]->getScore();
                            }
                        }
                    }
                    else
                    {
                        for (k = ca.startPoint.x,l = ca.startPoint.y; k < ca.startPoint.x + ca.count; k++)
                        {
                            if(_gemMatrix[k][l]->canMove())
                            {
                                sum+=_gemMatrix[k][l]->getScore();
                            }
                        }
                    }
                    
                    sumFlagArray[j]=1;
                }
            }
            if(sum>0)
            {
                MyPoint centerPoint = connectionAreaVector[index].centerPoint;
                
                if(_gemMatrix[centerPoint.x][centerPoint.y]/*&&_mapInfo->getPassLevelCCNode(_gemMatrix[centerPoint.x][centerPoint.y]->getGemType())!=NULL*/)
                {
                    _gemMatrix[centerPoint.x][centerPoint.y]->setScoreSum(sum);
                }
                
                sum=0;
            }
        }
    }
}

void MapMatrix::findParticlePoint(MyPoint &selectPoint, MyPoint &nextPoint)
{
    vector<ConnectionArea>::iterator it;
    
    int i,j;
    
    //处理所有相连的条，找到中心位置（可能为移动点，也可能为头结点，重置相交标志为空）
    for(it=connectionAreaVector.begin();it!=connectionAreaVector.end();it++)
    {
        /*		it->isInCross=false;*/
        
        if(it->isContain(selectPoint)||it->isContain(nextPoint))
        {
            if(it->isContain(selectPoint))
            {
                it->particlePoint.setPosition(selectPoint.x,selectPoint.y);
            }
            if(it->isContain(nextPoint))
            {
                it->particlePoint.setPosition(nextPoint.x,nextPoint.y);
            }
        }
        else//不包含两个移动点,则查找本区域内移动的点，设置为中心点
        {
            if (it->direction == Up)
            {
                for (j = it->startPoint.y,i = it->startPoint.x; j < it->startPoint.y + it->count; j++)
                {
                    if(it->count>=5)
                    {
                        it->particlePoint.setPosition(i,it->startPoint.y+2);
                    }
                    else if(it->count<5)
                    {
                        it->particlePoint.setPosition(i, it->startPoint.y+1);
                    }
                }
            }
            else
            {
                for (i = it->startPoint.x,j = it->startPoint.y; i < it->startPoint.x + it->count; i++)
                {
                    if(it->count>=5)
                    {
                        it->particlePoint.setPosition(it->startPoint.x+2,j);
                    }
                    else if(it->count<5)
                    {
                        it->particlePoint.setPosition(it->startPoint.x+1, j);
                    }
                }
            }
        }
        
    }
}