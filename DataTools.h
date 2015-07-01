#ifndef __DRAGONEGGTRIPLE_DATATOOLS_H__
#define __DRAGONEGGTRIPLE_DATATOOLS_H__

#include "cocos2d.h"
USING_NS_CC ;

#define kElementSize   78.0f//128.0f //160.0f
#define kMatrixWidth 8
#define kStartX 0
#define kStartY 0
#define kMapZOrder -1
#define kGemZOrder 0
#define kParticleZOrder 10
#define kEdgeBlankDevidedSize 10//单侧边框占总宽度份数
#define kGameLayerStartMultiple 5
#define kGameLayerFlyTime 0.8
#define kRenewGemWaitTime 0.3
#define kBrightScaleTime 0.12
#define kExplodeMiddleTime 0.067//同一列中移除前后时间间隔
#define kRenewGemTime 0.5//重排持续时间

#define kChangeWaitTime 0.3//交换错误返回之前等待的时间
#define kChangeMoveTime 0.3//交换时移动的时间
#define kChangeScaleFirstStepTime 0.08//交换变化第一步
#define kchangeScaleSecondStepTime 0.06//交换变化第二步
#define kFallMoveTime 0.1//下落一格所需时间
#define kFallMoveCorner 1/7.0//斜向
#define KCollectMoveSpeed 1500//收集元素移动到目的地用时
#define kCollectMoveJumpHeight 200
#define kHighLightFrequnce 4//高亮提示频率
#define kConvergeMoveTime 0.3//聚合元素移动时间
#define kConvergeChangeTime 0.8//聚合元素状态变化时间
#define kRemoveScaleLargeTime 0.1//消除放大时间
#define kRemoveScaleSmallTime 0.1//消除缩小时间
#define kRemoveCollectDelayTime 0.4
#define kRemoveeNoCollectDelayTime 0.4
#define kParticleScaleParm 1.2
#define kHenhouseBirdflyTime 0.8
#define kTigerJumpTime 0.8

//各种动作间隔时间
#define kPriorityInterval_GrassguyDissapear 0.7
#define kPriorityInterval_SickChange 0.8
#define kTigerEatDelayTime 0.7
#define kFiveMatchDelayTime 0.7
#define kCrossMatchDelayTime 0.7
#define kExplodeSumShowTime 0.5
#define kSparkleFlyTime 0.8
#define kParticleBezierScale 0.6
#define kGenerateGrassTimeDelayTime 0.2
#define kFrozenAndRestrainOffTime 0.4
#define kWinnerModeStarFlyDelayTime 1

#define kPriority5_delaytime 0.9

#define kGameConfigNull "kong"

#define kAdsIntervalTime 120

//所有元素都有的动作
//上下左右移动，选中

//普通元素动作
//上下移动，左右移动，掉落开始/结束，提示,选中
//
//眨眼，空闲，变病态，病态变好，被影响

#define kZhayan "zhayan"//点击
#define kIdleAnimation "idle"//空闲
#define kBecomeSick "becomesick"  //生病
#define kBecomeWell "becomewell" //病好
#define kFrozenOff "frozenoff" //去掉冰冻
#define kRestrainOff "restrainoff" //去掉束缚
#define kFrozenon "frozenon"//增加冰冻
#define kResttainOn "restrainon"//增加束缚

//聚合类动画(青蛙，蝴蝶）
#define kButterfly_Converge1 "converge1"//蛹破裂动画
#define kButterfly_Converge2 "converge2"//蝴蝶出现动画
#define kFrog_Converge1 "converge1"
#define kFrog_Converge2 "converge2"
#define kFrog_Converge3 "converge3"
#define kFrog_Converge1_Appear1 "appear1"//没有草
#define kFrog_Converge2_Appear2 "appear2"//裂开

//grassguy
#define kGrassguy_Disappear "disappear"//消失
#define kGrassguy_Disappear_Waittime 1.5//消失时间

//鹦鹉窝
#define kHenhouse_EggAppear "eggappear"
#define kHenhouse_EggBroken "eggbroken"
#define kHenhouse_ParrotAppear "parrotappear"

//泡泡里的鱼
#define kFishinbubble_match1 "start"//消除一次
#define kFishinbubble_match2 "middle"//消除两次
#define kFishinbubble_match3 "last"//消除三次，终极状态
#define kFishinbubble_idle1  "idle1"
#define kFishinbubble_idle2  "idle2"
#define kFishinbubble_idle3  "idle3"

//老虎
#define KTiger_Appear "appear"//老虎出现
#define kTiger_Dizzy1 "dizzy"//老虎头上一个星
#define kTiger_JumpToAnimal "tigerjump" //老虎扑小动物
#define kTiger_EatAnimal  "tigereat" //老虎吃东西

//连续消除提示
#define kMatch_great "Great.ccbi"
#define kMatch_perfect "Perfect.ccbi"
#define kMatch_awesome "Awesome. bi"
#define kMatch_great_level 3
#define kMatch_perfect_level 4
#define kMatch_awesome_level 5

//效果(加在效果层上)
#define kAfterEliminateParticle1 "remove.ccbi"//单个消除之后效果1
#define kAfterEliminateParticle2 "disappear.ccbi"//单个消除之后效果2
#define kFourOrMoreMatchParticle "sixiaochu.ccbi" //四或五消除效果
#define kCrossMatchParticle "shizixiaochu.ccbi" //十字消除效果
#define kWinnerModeLine "line.ccbi"//英雄模式 b
#define kParticleStart "appear" //效果执行（为了方便回调）

#define kEmptyFunction " "

//nitofication
#define kMSG_GemFallDown  "msg_Gem_falldown"
#define kMSG_UpdateCollecte "msg_update_collect"
#define kMSG_ShowCollecte  "msg_show_collect"
#define kMSG_UpdateMoves "msg_update_moves"
#define kMSG_Add5Moves  "add_5_moves"
#define kMSG_UpdateStars "msg_updata_stars"
#define kMSG_MatchDown "msg_match_down"  // 匹配完成
#define kMSG_RenewAll "msg_renewall" //renew持续时间为 kRenewGemTime
#define kMSG_WinnerModeStart "msg_winner_mode_start"//英雄模式开启
#define kMSG_PropUseStart "msg_props_start"//使用道具
#define kMSG_PropUseCancel "msg_props_cancel" //道具取消
#define kMSG_PropUseOver "msg_props_over"//道具使用完毕
#define kMSG_PropUseNoCandidate "msg_props_nocandidate" //道具没有作用
#define kMSG_GameLayer_BeforeOutStage "msg_gamelayer_beforeout" //通知游戏结束，参数0失败，参数1成功
#define kMSG_GameLayer_AfterOutStage  "msg_gamelayer_afterout"//游戏结束，layer飞出之后通知前台切换界面
#define kMSG_GameLayer_LayerInitComplete "msg_gamelayer_complete"
#define kMSG_ClassGuid_PropSelectOver "msg_classguid_propselectover"
#define kMSG_ClassGuid_MoveGem      "msg_classguid_moveGem"
#define kMSG_BuyFiveSteps             "msg_buyfivesteps"
#define kMSG_BuyDimonds               "msg_buydimonds"
#define kMSG_CheckBtn               "msg_checkbtn"

#define kMSG_AvatarMenu               "msg_avatar_menu"
#define kMSG_FacebookLoginOk           "msg_fb_login_ok"
#define kMSG_RemoveGift     "msg_remove_gift"

//颜色
#define kColor1  Color3B(146,0,146)//红
#define kColor2  Color3B(255,252,0)//金黄
#define kColor3  Color3B(0,255,0)//纯绿
#define kColor4  Color3B(0,146,255)//纯蓝
#define kColor5  Color3B(255,145,0)//橙色
#define kColor6  Color3B(0,255,246)//蓝色
#define kColor7  Color3B(156,156,0)//紫色
//google analytic category
#define kGA_category1 "关卡数据"
#define kGA_category2 "关卡数据2"
#define kGA_Login  "登陆数据"
#define kGA_Facebook  "FACEBOOK数据"
#define kGA_buyLevelData "购买时关卡"

//音效开关
#define kMusic_Backgroud_Switch  "backgroud_switch"
#define kMusic_Effect_Switch  "effect_switch"

//音效文件

#define kIsReceiptNotification  "is_receipt_notification"
#define kResetData              "reset_data"

#define kExitLayerTag  246
#define kIsHaveDialogLayer "is_have_dialog_layer"
#define kUpdateFriendsNum  "update_friends_num"
#define kUpdateUIdate      "update_ui_date"
#define kAddProfileLayer   "add_profile_layer"
#define kAddInviteLayer   "add_invite_layer"
#define kAddFriut4GiveHeart "add_Gem_4_give_heart"
#define kTodayNumInYear    "today_num_in_year"
#define kInitGiftIsVisible  "init_gift_is_visible"
#define kDailyMissionMessage "daily_mission_message"
#define kUpdateGoodsType  "update_goods_type"

#define kUpdateFbMsgdate      "update_fbmsg_date"

#define kIsFirstPurcharAchieve "is_first_purchar_achieve"
#define kIsCustomBuyAnyAcount  "is_custom_buy_any_acount"


#define kMsgUpdataInviteTable "msg_updata_invite_table"
#define kMsgSendHeartProfile "msg_send_heart_profile"
#define kMsgSendHeartLeaderBoard "msg_send_heart_leaderboard"
#define kMsgSendHeartMessage "msg_send_heart_message"
#define kMsgIsAddRate2UI     "msg_is_add_rate_2_ui"

#define kBuyDiamondNoTouch "buy_diamond_no_touch"

#define HEARTTIME 1800

#define WORLD_MAP_UI_LAYER_TAG 123

class Gem;

struct MyPoint
{
    int x,y;
    MyPoint(){}
    MyPoint(int i,int j){x=i;y=j;}
    void setPosition(int i,int j){x=i;y=j;}
    void swap(MyPoint& p){int tempX = p.x; int tempY = p.y; p.setPosition(x, y); setPosition(tempX, tempY);}
    bool operator ==(const MyPoint& p ){if(p.x==x&&p.y==y) return true; else return false;}
    void reset(){x=-1;y=-1;}
    bool equal(MyPoint &point){if(x==point.x&&y==point.y) return true; else return false;}
    bool isOrigin(){return (x==0)&&(y==0);}
};

typedef enum
{
    e_priority_normal_becomesick=1,
    e_priority_normal_becomewell=1,
    e_priority_henhouse_changestate=2,
    e_priority_henhouse_birdfly=3,
    e_priority_normal_fertileaddscore=4,
    e_priority_grassguy_disappear=5,
    e_priority_grassguy_generategrass=5,
    e_priority_tiger_eatanimal=5,
    e_priority_normal_frozenoff=14,
    e_priority_normal_rootsoff=14,
    e_priority_normal_chainoff=14,
    e_priority_normal_restrainoff=14,
    e_priority_fish_changestate=14,
    e_priority_tiger_changestate=6,
    e_priority_winnermode_starfly=6,
    e_priority_winnermode_start=7,
    e_priority_colorfullegg_disappear=7,
    e_priority_fish_remove=7,
    e_priority_colorfullegg_fly=8,
    e_priority_particle_five2=13,
    e_priority_particle_five1=11,
    e_priority_particle_cross=10,
    e_priority_particle_four=10,
    e_priority_all_gem=10,
    e_priority_skill=12,
    e_priority_prop=10,
    e_priority_converge_move=11,
    e_priority_converge_changestate=12,
    e_priority_converge_remove=13,
    e_priority_normal_explode=14,
    e_priority_normal_explode_sum=15,
    e_priority_normal_affectaddscore=16,
    e_priority_beforematch=17,
}AnimationPriority;

typedef enum
{
    e_aid_grassguy_disappear,//grassguy消失
    e_aid_grassguy_generategrass,//生成5块草地
    e_aid_normal_becomesick,//正常元素生病
    e_aid_normal_becomewell,//正常元素病好
    e_aid_normal_explode,//正常元素消失
    e_aid_normal_addscore,//普通元素增加积累值
    e_aid_normal_frozenoff,//冰冻去掉
    e_aid_normal_rootsoff,//冰冻去掉
    e_aid_normal_chainoff,//冰冻去掉
    e_aid_normal_restrainoff,//束缚去掉
    e_aid_colorfullegg_disappear,//彩蛋消失
    e_aid_henhouse_threeeggs,//出现三个蛋
    e_aid_henhouse_eggbroken,//蛋碎
    e_aid_henhouse_birdappear,//鸟出现
    e_aid_henhouse_birdfly,//飞出鹦鹉
    e_aid_fish_match,//鱼被影响
    e_aid_fish_remove,//鱼移除
    e_aid_converge_move, //聚合元素移动
    e_aid_converge_changestate,//聚合元素改变状态
    e_aid_converge_remove,//聚合元素移除
    e_aid_tiger_eatanimal,//老虎吃小动物
    e_aid_tiger_changestate,//老虎改变状态
    e_aid_particle_five1,
    e_aid_particle_five2,
    e_aid_particle_four,
    e_aid_skill,
    e_aid_prop,
    e_aid_all_gem,
    e_aid_particle_cross,
    e_aid_normal_explode_sum,
    e_aid_winnermode_starfly,
    e_aid_winnermode_start,
    e_aid_beforematch,
}AnimationID;

struct AnimationWraper
{
    AnimationPriority priority;
    AnimationID animationID;
    Gem* node;
    Point dist1;
    Point dist2;
    Point dist3;
    int score;
    int state;
    int index1;
    int index2;
    bool isSingleAnimation;
    MyPoint myDist1;
    MyPoint myDist2;
    MyPoint myDist3;
    Gem*  Gem1;
    Gem*  Gem2;
    Gem*  Gem3;
    
    Gem*  GemToRemove1;
    Gem*  GemToRemove2;
    Gem*  GemToRemove3;
    
    int flagArray[10];
    int flagArrayLength;
    
    AnimationWraper(Gem* f,AnimationID aid,AnimationPriority p)
    :node(f)
    ,animationID(aid)
    ,priority(p)
    ,score(0)
    ,isSingleAnimation(false)
    {
        for(int i=0;i<6;i++)
        {
            flagArray[i]=0;
        }
    }
    void setDistination( Point& p1, Point& p2, Point& p3)
    {
        dist1 = p1;
        dist2 = p2;
        dist3 = p3;
    }
};

typedef enum
{
    empty = 0,
    red=1,
    yellow=2,
    blue=3,
    green=4,
    purple=5,
    white=6,
    all=7,
    extend1=8,//扩展
    extend2=9,//扩展
    extend3=10,//扩展
    ice=11,
    ice1=12,
    shield=13,
    shield1=14,
    shield2=15,
    shield3=16,
    shield4=17,
    babydragon=18,
    magicscrolls=19,
    lifttree=20,
    stormwind=21,
    dragontooth=22,
    rod=23,
    pumpkin=24,
    redbottle=30,
    redbottle1=31,
    redbottle2=32,
    redbottle3=33,
    redbottle4=34,
    yellowbottle=35,
    yellowbottle1=36,
    yellowbottle2=37,
    yellowbottle3=38,
    yellowbottle4=39,
    bluebottle=40,
    bluebottle1=41,
    bluebottle2=42,
    bluebottle3=43,
    bluebottle4=44,
    greenbottle=45,
    greenbottle1=46,
    greenbottle2=47,
    greenbottle3=48,
    greenbottle4=49,
    purplebottle=50,
    purplebottle1=51,
    purplebottle2=52,
    purplebottle3=53,
    purplebottle4=54,
    whitebottle=55,
    whitebottle1=56,
    whitebottle2=57,
    whitebottle3=58,
    whitebottle4=59,
    maxgem=60,
}GemType;

typedef enum
{
    NormalGem=0,
    SpecialGem,
}GemKind;

typedef enum
{
    Up=0,
    Left,
    Right,
    Down,
    LeftDown,
    RightDown,
    StandBy
}Direction;

typedef enum
{
    DeepEliminate=-2,
    Eliminate=-1,
    NoChange=0,
    Changed
} ChangeState;

typedef enum
{
    NoneInMiddle = -5,
    Empty=-4,
    IceFloor1=-3,
    IceFloor=-2,
    Random=-1,
    NoneInEdge=0
}MapType;

typedef enum
{
    pupa=0,//蛹
    bug//虫
} ButterflyType;

typedef enum
{
    littlefrog=0,//小青蛙
    frogwithtail,//长腿的蝌蚪
    tadpole//蝌蚪
} FrogType;

typedef enum
{
    Henhouse_Eggs=2,
    Henhouse_Eggbroken=1,
    Henhouse_Birds=0,
}HenhouseState;

typedef enum //道具
{
    PropNone=-1,
    PropSingle,
    PropAround,
    PropCross,
    PropChange,
    PropAddOne,
    PropRow,
    PropOneKind,
    PropCureSick,
}PropKind;


typedef enum
{
    BossAppear=0,
    BossHurt1,
    BossHurt2,
    BossDie
}BossState;

typedef enum
{
    Spring=30,
    Summer=80,
    Autumn=120,
    Winter=162,
}BackgroundType;

typedef enum
{
    SkillNull = 0,
    SkillHorizontal,
    SkillVerticl,
    SkillAround1,
    SkillAround2,
    SkillAllSame,
    SkillAround3,
    SkillAround4,
    SkillCross,
    SkillThreeCross
}GemSkill;

typedef enum //教程
{
    ClassNone=-1,
    ClassSanXiao,//三消
    ClassSiXiao,//四消
    ClassShiZiXiao,//十字消
    ClassWuXiao,//五消
    ClassFishinbubble,//泡泡里的鱼
    ClassButterfly,//聚合
    ClassRattan,//藤条
    ClassFertileLand,//肥沃土地
    ClassHenHouse,//派生
    ClassBarrenLand,//贫瘠土地
    ClassFrozen,//冰冻
    ClassSeed,//草地生成者
    ClassTiger,//老虎
    ClassColorfulLegg,//彩蛋
    
    ClassPropSingle=14,//道具
    ClassPropAddOne,
    ClassPropRow,
    ClassPropOneKind,
    ClassPropCureSick,
    
    ClassBuyProp=19,
    ClassBuyLife=20,
    
    ClassShareFaceBook
    
}ClassKind;

struct ConnectionArea//相连区域
{
    MyPoint startPoint;
    Direction direction;
    int count;
    
    MyPoint centerPoint;
    
    MyPoint particlePoint;
    
    bool isInCross;
    
    bool isContain(MyPoint& point)
    {
        if(point.x==-1)
        {
            return false;
        }
        if(direction==Up)
        {
            return (point.x==startPoint.x)&&(point.y-startPoint.y>=0)&&(point.y-startPoint.y<count);
        }
        else
        {
            return (point.y==startPoint.y)&&(point.x-startPoint.x>=0)&&(point.x-startPoint.x<count);
        }
    }
    
    bool isCross(const ConnectionArea& connectArea,MyPoint& crossPoint)
    {
        if(this==&connectArea)
        {
            return false;
        }
        for(int i=0;i<connectArea.count;i++)
        {
            if(connectArea.direction==Up)
            {
                MyPoint point(connectArea.startPoint.x,connectArea.startPoint.y+i);
                
                if(isContain(point))
                {
                    crossPoint.setPosition(point.x, point.y);
                    isInCross = true;
                    return true;
                }
            }
            else
            {
                MyPoint point(connectArea.startPoint.x+i,connectArea.startPoint.y);
                
                if(isContain(point))
                {
                    crossPoint.setPosition(point.x, point.y);
                    isInCross = true;
                    return true;
                }
            }
        }
        crossPoint.reset();
        
        return false;
    }
};

struct PotentialArea//潜在区域
{
    MyPoint movePoint;
    Direction diretion;
    MyPoint point1;
    MyPoint point2;
};
typedef enum
{
    BuyDimondFail=-1,
    BuyDimond1=0,
    BuyDimond2=1,
    BuyDimond3=2,
    BuyDimond4=3,
    BuyDimond5=4,
}BuyDimondMessage;

// static const int  MAP_TEST[kMatrixWidth][kMatrixWidth] = 
// {
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1},
// 	{-1, -1, -1, -1, -1, -1, -1, -1}
// };

static const int  MAP_TEST[kMatrixWidth][kMatrixWidth] =
{
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1}
};

//static const int  MAP_TEST[kMatrixWidth][kMatrixWidth] = 
//{
//    {0, 0, 0,  -1, -1, 0, 0, 0},
//    {0, 0, -1, -1, -1, -1, 0, 0},
//    {0,-1, -1, -1, -1, -1,-1, 0},
//    {-1,-1,-1, -1, -1, -1,-1,-1},
//    {-1,-1,-1, -1, -1, -1,-1,-1},
//    {0, -1,-1, -1, -1, -1,-1, 0},
//    {0, 0, -1, -1, -1, -1, 0, 0},
//    {0, 0, 0,  -1, -1, 0, 0, 0}
//};

#endif   //__DRAGONEGGTRIPLE_DATATOOLS_H__