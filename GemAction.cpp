#include "GemAction.h"
#include <math.h>
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

GLProgram* GemAction::_glProgramBright = NULL;
GLProgram* GemAction::_glProgramNormal = NULL;

GemAction::GemAction()
{
    _glProgramBright = new GLProgram();
    
    _glProgramBright->initWithFilenames("bright.vsh", "bright.fsh");
    
    _glProgramBright->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);   // 暂时注销
    _glProgramBright->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    _glProgramBright->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    _glProgramBright->link();
    _glProgramBright->updateUniforms();
    
    _glProgramNormal = new GLProgram();
    
    _glProgramNormal->initWithFilenames("bright.vsh", "normal.fsh");
    
    _glProgramNormal->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);   // 暂时注销
    _glProgramNormal->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    _glProgramNormal->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    _glProgramNormal->link();
    _glProgramNormal->updateUniforms();
    
    /*	_musicPath = kMusic_Backgroud_MapLayer;*/
}
GemAction::~GemAction()
{
    _glProgramBright->release();
    _glProgramNormal->release();
}
ActionInterval* GemAction::getGemMoveAction(Direction direction)
{
    
    ActionInterval *action = NULL;
    
    switch (direction)
    {
        case Up:
            action = MoveBy::create(kFallMoveTime, Point(0, kElementSize));
            break;
        case Down:
            action = MoveBy::create(kFallMoveTime, Point(0, -kElementSize));
            break;
        case Left:
            action = MoveBy::create(kFallMoveTime, Point( -kElementSize,0));
            break;
        case Right:
            action = MoveBy::create(kFallMoveTime, Point(kElementSize , 0));
            break;
        case RightDown:
            action = MoveBy::create(kFallMoveCorner, Point(kElementSize, -kElementSize));
            break;
        case LeftDown:
            action = MoveBy::create(kFallMoveCorner, Point(-kElementSize, -kElementSize));
            break;
        case StandBy:
            action = DelayTime::create(kFallMoveTime);
            break;
        default:
            action = DelayTime::create(kFallMoveTime);
            break;
    }
//    auto act = Sequence::create(action,DelayTime::create(kFallMoveTime/2), NULL);
    return action;
}


FiniteTimeAction* GemAction::getGemRenewAction(MyPoint distination)
{
    return JumpTo::create(kRenewGemWaitTime,Point(distination.x*kElementSize+kStartX+kElementSize/2,distination.y*kElementSize+kStartY+kElementSize/2), kElementSize/3, 2);
}
FiniteTimeAction* GemAction::getGemMoveToAction(MyPoint distination)
{
    return MoveTo::create(0,Point(distination.x*kElementSize+kStartX+kElementSize/2,distination.y*kElementSize+kStartY+kElementSize/2));
}
ActionInterval* GemAction::getGemChangeAction(Direction direction)
{
    
    ActionInterval *action = NULL;
    
    switch (direction)
    {
        case Up:
            action = EaseBackOut::create(MoveBy::create(kChangeMoveTime, Point(0, kElementSize)));
            break;
        case Down:
            action = EaseBackOut::create(MoveBy::create(kChangeMoveTime, Point(0, -kElementSize)));
            break;
        case Left:
            action = EaseBackOut::create(MoveBy::create(kChangeMoveTime, Point( -kElementSize,0)));
            break;
        case Right:
            action = EaseBackOut::create(MoveBy::create(kChangeMoveTime, Point(kElementSize , 0)));
            break;
        default:
            break;
    }
    
    return action;
}

ActionInterval* GemAction::getCollectGemAction(const Point& startPoint,const Point& distPoint,float time)
{
    
    ccBezierConfig config;
    Point controlPoint = getControlPoint(startPoint, distPoint, kElementSize);//ccp(startPoint.x+kElementSize,startPoint.y+kElementSize);
    config.controlPoint_1 = controlPoint;
    config.controlPoint_2 = controlPoint;
    config.endPosition = distPoint;
    
    return Spawn::create(Sequence::create(ScaleTo::create(0.04, 1.3),ScaleTo::create(time-0.04, 0.8),NULL),BezierTo::create(time, config),NULL);
    
}

ActionInterval* GemAction::getConvergeMoveAction(Point point)
{
    return MoveTo::create(kConvergeMoveTime,point);
}

void GemAction::setBright(Node* node)
{
    node->setGLProgram(_glProgramBright);
//     	Vector<Node*> vec_children = node->getChildren();
//    
//     	for (int i = 0; i < vec_children.size(); i++)
//     	{
//     		vec_children.at(i)->setGLProgram(_glProgramBright);
//     	}
    
    // 	CCObject* obj;
    //
    // 	CCARRAY_FOREACH(node->getChildren(), obj)
    // 	{
    // 		((CCNode*)obj)->setShaderProgram(_glProgramBright);
    // 	}
    
    
}

void GemAction::removeBright(Node* node)
{
    
    node->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
//    node->setGLProgram(_glProgramNormal);                //方法二
    
//     	Vector<Node*> vec_children = node->getChildren();
//    
//     	for (int i = 0; i < vec_children.size(); i++)
//     	{
//     		vec_children.at(i)->setGLProgram(_glProgramNormal);
//     	}
    // 	CCObject* obj;
    // 	CCARRAY_FOREACH(node->getChildren(), obj)
    // 	{
    // 		((CCNode*)obj)->setShaderProgram(_glProgramNormal);
    // 	}
}

Point GemAction::getControlPoint(Point point1, Point point2,int k)
{
    static bool flag=false;
    
    float distance = point1.getDistance(point2);/*ccpDistance(point1, point2);*/
    
    if(point1.x==point2.x)
    {
        point2.setPoint(point2.x+10, point2.y);
    }
    else if(point1.y==point2.y)
    {
        point2.setPoint(point2.x, point2.y+10);
    }
    
    float sin = fabs(point1.y-point2.y)/distance;
    float cos = fabs(point1.x-point2.x)/distance;
    float m = (point1.x+point2.x)/2;
    float n = (point1.y+point2.y)/2;
    float b = sin*k+(point1.y+point2.y)/2;
    float a = cos*k+(point1.x+point2.x)/2;
    //printf("sin=%4.2f,cos=%4.2f,m=%4.2f,n=%4.2f,a=%4.2f,b=%4.2f,k=%d\n",sin,cos,m,n,a,b,k);
    if(flag)
    {
        flag = false;
        
        return Point(b-n+m,m-a+n);
    }
    else
    {
        flag = true;
        return Point(n-b+m,a-m+n);
    }
}

Point GemAction::getControlPointSingle(Point point1, Point point2,int k)
{
    float distance = point1.getDistance(point2);/*ccpDistance(point1, point2)*/;
    
    if(point1.x==point2.x)
    {
        point2.setPoint(point2.x+10, point2.y);
    }
    else if(point1.y==point2.y)
    {
        point2.setPoint(point2.x, point2.y+10);
    }
    
    float sin = fabs(point1.y-point2.y)/distance;
    float cos = fabs(point1.x-point2.x)/distance;
    float m = (point1.x+point2.x)/2;
    float n = (point1.y+point2.y)/2;
    float b = sin*k+(point1.y+point2.y)/2;
    float a = cos*k+(point1.x+point2.x)/2;
    //printf("sin=%4.2f,cos=%4.2f,m=%4.2f,n=%4.2f,a=%4.2f,b=%4.2f,k=%d\n",sin,cos,m,n,a,b,k);
    
    return Point(b-n+m,m-a+n);
}

 void GemAction::playEffectMusic(Node* pSender ,string file)
 {
     SimpleAudioEngine::getInstance()->playEffect(file.c_str());
 }
//
// void GemAction::playBackgroudMusic(cocos2d::CCNode *sender, void *data)
// {
// 	_musicPath = (char*)data;
// 
// 	if(CCUserDefault::sharedUserDefault()->getBoolForKey(kMusic_Backgroud_Switch))
// 	{
// 		stopBackgroudMusic();
// 
// 		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_musicPath,true);
// 	}
// }
// 
// void GemAction::stopBackgroudMusic()
// {
// 	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
// }
// void GemAction::switchBackgroudMusic()
// {
// 	if(CCUserDefault::sharedUserDefault()->getBoolForKey(kMusic_Backgroud_Switch))
// 	{
// 		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_musicPath,true);
// 	}
// 	else
// 	{
// 		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
// 	}
// }