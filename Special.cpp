#include "Gem.h"

Special::Special()
{
    _state = 0;
    _type = special;
    _GemKind = SpecialGem;
    _specialCount++;
}

bool Special::createGemSprite(Node *layer, Point point,bool visible)
{
    Gem::createGemSpriteByName(layer, point, visible, "scale.png");
    
    return true;
}

bool Special::canMove()
{
    return true;
}

bool Special::equal(Gem* gem)
{
    return false;
}

void Special::beforeMatch(int i,int j)
{
    kEmptyFunction;
}
void Special::affected(int count)
{
    if (_GemKind == NormalGem) return;
    if(_state<0) return;
    
    _state--;
    
}

void Special::changeState(int state)
{
    if (_state < 0)
    {
        _state = 0;
        
        vector<GemType> vecType;
        
        MyPoint myPoint = getCurrentIndex(this->getPosition());
        int i = myPoint.x,j=myPoint.y;
        
        if (i < kMatrixWidth && j < kMatrixWidth)
        {
            if(i-1>=0&&_gemStoneMatrix[i-1][j])
            {
                vecType.push_back(_gemStoneMatrix[i-1][j]->getGemType());
            }
            if(i+1<kMatrixWidth&&_gemStoneMatrix[i+1][j])
            {
                vecType.push_back(_gemStoneMatrix[i+1][j]->getGemType());
            }
            if(j-1>=0&&_gemStoneMatrix[i][j-1])
            {
                vecType.push_back(_gemStoneMatrix[i][j-1]->getGemType());
            }
            if(j+1<kMatrixWidth&&_gemStoneMatrix[i][j+1])
            {
                vecType.push_back(_gemStoneMatrix[i][j+1]->getGemType());
            }
        }
        GemType fType ;
        
        while (1)
        {
            int index = arc4random()%_mapInfo->getGemType().size();
            fType = (GemType)_mapInfo->getGemType().at(index);
            
            vector<GemType>::iterator result = find( vecType.begin(), vecType.end(), fType);
            if (result != vecType.end())
            {
                continue;
            }
            else
            {
                break;
            }

        }
        
        Gem *temp = _gemStoneMatrix[i][j];
        _gemStoneMatrix[i][j] = Gem::createFixGem(fType,this->getParent(),temp->getPosition(),true);
        temp->removeFromParentAndCleanup(true);
        _specialCount = 0;
        
        int random = arc4random()%100;
        if (random < 10)//10
        {
            _gemStoneMatrix[i][j]->setNextSkill(SkillTurnTen);
        }
        else if(random < 30)//30
        {
            _gemStoneMatrix[i][j]->setNextSkill(SkillTen);
        }
        else
        {
            if (random%2)
            {
                _gemStoneMatrix[i][j]->setNextSkill(SkillLeft);
            }
            else
            {
                _gemStoneMatrix[i][j]->setNextSkill(SkillRight);
            }
        }
        
        _gemStoneMatrix[i][j]->setSkillSpr();
    }
}

void Special::idle()
{
   
}
