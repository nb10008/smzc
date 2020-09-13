//--------------------------------------------------------------------------------
//!\file script_mgr.h
//!\author Aslan
//!
//!\date 2008-09-01
//! last 2008-09-01
//!
//!\brief Lua脚本管理器，主要负责各种脚本的组织、生成及调用
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//--------------------------------------------------------------------------------
#pragma once

extern "C"
{
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"
#include "../lua/src/lstate.h"
};
#include "mutex.h"
#include "event_mgr.h"
#include "../ServerDefine/activity_define.h"
#include "../WorldDefine/penalty_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/loot.h"

class Unit;
class Creature;
class Role;
class CActivity;
class ScriptMgr;
class Skill;

struct tagMapTriggerInfo;
struct tagMapAreaInfo;

enum ERoleReviveType;

void RegisterTraceCmd();

//-------------------------------------------------------------------------------
// 任务脚本事件枚举
//-------------------------------------------------------------------------------
enum EScriptQuestEvent
{
	ESQE_On_Accept			=	0,			// 任务接取
	ESQE_On_Complete		=	1,			// 任务完成
	ESQE_On_Cancel			=	2,			// 任务取消
	ESQE_On_Creature_Kill	=	3,			// 杀死一只怪物
	ESQE_On_CheckAccept		=	4,			// 检测接取
	ESQE_On_CheckComplete	=	5,			// 检测交
	ESQE_On_NPC_Talk		=	6,			// 和NPC对话
	ESQE_On_Init			=	7,			// 任务初始化
	ESQE_On_Dlg_Default		=	8,			// 服务器可控对话框缺省事件
	ESQE_On_Invest			=	9,			// 调查地物		
	ESQE_OnClientCallback	=	10,			// 活动，读取客户端反馈用
	ESQE_On_BQComplete		=	11,			// 任务板任务完成
	ESQE_End				=	12,
};

//---------------------------------------------------------------------------------
// 怪物脚本事件枚举
//---------------------------------------------------------------------------------
enum EScriptCreatureEvent
{
	// 日常事件
	ESCAE_On_Load			=	0,			// 载入
	ESCAE_On_Respawn		=	1,			// 重生
	ESCAE_On_Enter_Combat	=	2,			// 进入战斗
	ESCAE_On_Leave_Combat	=	3,			// 离开战斗
	ESCAE_On_Die			=	4,			// 死亡
	ESCAE_On_Timer			=	5,			// 定时触发
	ESCAE_On_Invest			=	6,			// 被调查
	ESCAE_On_Talk			=	7,			// 被说话

	// AI事件
	ESCAE_On_UpdateAI		=	8,			// 更新AI状态机
	ESCAE_On_UpdateCurAI	=	9,			// 更新当前AI状态
	ESCAE_On_EnterCurAI		=	10,			// 进入当前AI状态
	ESCAE_On_LeaveCurAI		=	11,			// 离开当前AI状态
	ESCAE_On_EventCurAI		=	12,			// 当前AI状态事件触发
	ESCAE_On_Disappear		=	13,			// 怪物消失出发

	ESCAE_On_BeAttacked		=	14,			// 怪物在被攻击

	ESCAE_On_IsShopOpened	=	15,			// 商店是否可以买卖
	ESCAE_On_CanShopping	=   16,	        // 玩家是否可以在商店购买物品装备
    ESCAE_On_OnShopping     =   17,         //玩家购买了商品 

	ESCAE_On_BePulled		=	18,			// 拉怪状态
	
	ESCAE_End				=	19,
};

//---------------------------------------------------------------------------------
// 玩家脚本
//---------------------------------------------------------------------------------
enum EScriptRoleEvent
{
	ESRE_On_Online				=	0,			// 上线
	ESRE_On_FirstOnline			=	1,			// 第一次上线
	ESRE_On_IntoWorld			=	2,			// 进入游戏世界
	ESRE_On_FirstIntoWorld		=	3,			// 第一次进入游戏世界
	ESRE_On_EnterMap			=	4,			// 进入地图
	ESRE_IsDeadPenalty			=   5,			// 是否对玩家进行死亡惩罚
	ESRE_On_LevelChange			=	6,			// 等级提升
	ESRE_On_OpenChest			=	7,			// 开启宝箱
	ESRE_On_StopChest			=	8,			// 停止开启，产生随机物品
	ESRE_On_AgainChest			=	9,			// 再开一次
	ESRE_On_GetItem				=	10,			// 得到宝箱物品
	ESRE_On_GetMarriage			=	11,			// 结婚
	ESRE_On_GetMarriageCheck	=	12,			// 结婚检查
	ESRE_On_BreakMarriage		=	13,			// 离婚
	ESRE_On_BreakMarriageCheck	=	14,			// 离婚检查
	ESRE_IsEquipLootPenalty		=   15,			// 装备掉落惩罚
	ESRE_IsExpPenalty			=	16,			// 经验惩罚
	ESRE_IsInjuryPenalty		=	17,			// 内伤惩罚
	ESRE_IsMoralityPenalty		=	18,			// 道德惩罚
	ESRE_IsHostilityPenalty		=	19,			// 戾气惩罚
	ESRE_IsCommercePenalty		=	20,			// 跑商商银惩罚
	ESRE_GetRoleFishingExp		=	21,			// 钓鱼经验（离线用）
	ESRE_CanCancelPenalty		=	22,			// 是否能免除死亡惩罚
	ESRE_On_CompleteCommerce	=	23,			// 完成跑商
	ESRE_WaitSummon				=	24,			// 激活守护石
	ESRE_CancelSummon			=	25,			// 取消召唤
	ESRE_UseJingWuPoint			=	26,			// 使用精武点数
	ESRE_HandleMaster			=	27,			// 拜师
	ESRE_IsTrainFinished		=	28,			// 龙之试炼系统--判断完成试炼
	ESME_OnObtainItem			=	29,			// 玩家获得一件道具的事件接口
	ESME_OnSilverIncrease		=	30,			// 玩家游戏币增加的事件接口
	ESME_OnObtainTitle			=	31,			// 玩家获得一个称号的事件接口
	ESME_OnItemConsolidate		=	32,			// 玩家完成一次装备强化的事件接口
	ESME_OnActiveLonghunSuccess	=	33,			// 玩家激活龙魂成功的事件接口
	ESRE_OnTrainFinished		=	34,			// 玩家完成一个试炼

	ESRE_OnFabaoAttChange		=	35,			// 法宝属性变化：升阶、灵槽重置、五行相生重置
   
	ESRE_OnRoleJoinToArenaRoom  =   36,         // 角色加入竞技场房间
    ESRE_OnRoleCreateArenaRoom  =   37,         // 角色创建竞技场房间
    ESRE_OnRoleAgreeToBeInvited =   38,        // 角色同意被邀请加入房间
	
	ESRE_OnReadyToSoaring		=	39,			// 开始飞升，通知脚本，要飞升了，接飞升任务
	ESRE_OnCompletePracticeEquipment	=	40,			// 炼器成功标志
	ESRE_OnCompleteCastingEquipment		=	41,			// 熔铸成功标志

	ESRE_On_ForceBreakMarriage		=	42,			// 强制离婚
	ESRE_On_ForceBreakMarriageCheck	=	43,			// 强制离婚检查
	
	ESRE_OnChaseHolderDie			=	44,		// 夺宝奇兵活动携带宝箱者被杀死时触发
	ESRE_ForceLootChase				=	45,		// 宝箱强制掉落(夺宝奇兵)
	ESRE_OnMakeFriend				=	46,		// 增加好友
	ESRE_OnLearnSkill				=	47,		// 玩家学会新技能
	ESRE_OnEquipConsolidateLevelUp	=	48,		// 装备强化等级提升
	ESRE_OnProduceEquip			=	49,			// 制造装备
	ESRE_OnDigHoleOnEquip		=	50,		// 玩家开凿镶嵌孔
	ESRE_OnCutStone				=	51,		// 玩家切割宝石原石
	ESRE_OnDressEquip			=	52,		// 玩家穿着装备
	ESRE_OnBrandEquip			=	53,		// 对装备使用圣印
	ESRE_OnAddItemToBag			=	54,		// 背包中获得物品
	ESRE_OnGodStrengthenSuccess	=	55,		// 神铸成功后的事件
	ESRE_OnUseGoldStone			=	56,			// 使用点金石

	ESRE_OnCreateGuild			=	57,			// 创建公会

	ESRE_CanAddEnemy			=	58,			// 判断是否可以加入仇敌列表
	ESRE_OnKillMonster			=	59,			// 杀怪事件

	ESRE_CondenseUpgrade	= 60,	//神格提升
	ESRE_GetClergy			= 61,	//晋升神职
	ESRE_Transform			= 62,	//使用变身技能
	ESRE_GetRareArea		= 63,	//获得稀有领域
	ESRE_WinRareArea		= 64,	//抢夺稀有领域
	ESRE_ActivateArea		= 65,	//自选领域激活
	ESRE_IsCanRobRareArea	= 66,	//是否可以进行稀有领域抢夺

	ESRE_CanUseDakNPC		= 67,	// 是否可以使用驿站NPC
	ESRE_CanUseEarthTemple	= 68,	// 是否可以使用土地庙

	ESRE_CanCalCrit			= 69,	// 是否计算暴击
	ESRE_RoleLogout			= 70,	// 角色下线 

	ESRE_On_StartChest				=	71,			// 开宝箱开始播动画

	ESRE_On_FashionSuitNumChg		= 72, // 角色衣橱内集齐时装的套数改变
	ESRE_On_WardLevelUp = 73,			// 衣橱升级
	ESRE_Quest_IncSilver = 74,				// 完成任务后获得金钱
	ESRE_BuyItem_DecSilver = 75,			// 通过购买道具消耗金币
	ESRE_Strengthen_DecSilver = 76,			// 通过提升装备强化等级消耗金币
	ESRE_Compose_DecSilver = 77,			// 通过合成消耗金币
	ESRE_Can_StrengthenEquip = 78,			// 玩家强化装备时其成功率判定
	ESRE_RoleGetBoardQuestReward = 79, // 角色领取任务版任务的奖励
	ESRE_EquipGetGrowAtt = 80, // 装备获得成长属性
	ESRE_EquipGrowGetStar = 81, // 装备成长生成新的星星

	ESRE_AccountReactived = 85, // 玩家被回捞
	
	ESRE_GetSoulCrystalRenew = 86, // 重置魂晶属性

	ESRE_MapChangeWithScriptEquip = 87,	// 玩家穿着 MapLimit为1的装备切换地图
	ESRE_TryToWareScriptEquip = 88,				// 玩家试图穿着 MapLimit为1的装备
	//ESRE_RewardReactive = 87, // 介绍人领取回捞奖励
	ESRE_UseDiamondAddAndEquipGrowIM = 89,	//使用刷成长id以及宝石增幅道具
	ESRE_RefreshBaoXiang = 90,	//刷新战场结算奖励
	ESRE_GetHolyDevourNum = 91, //获得所装备圣灵的吞噬次数

	ESRE_End,
};

enum ERoleFabaoAttEvent
{
	RFAE_LevelUp		=	0,	// 法宝升阶
	RFAE_HoleAttReset	=	1,	// 法宝灵槽属性重置
	RFAE_WuxingXSReset	=	2,	// 法宝五行相生重置
};

//---------------------------------------------------------------------------------
// 地图脚本
//---------------------------------------------------------------------------------
enum EScriptMapEvent
{
	ESME_OnInit					=	0,			// 初始化时
	ESME_OnTimer				=	1,			// 定时更新
	ESME_OnPlayerEnter			=	2,			// 玩家进入
	ESME_OnPlayerLeave			=	3,			// 玩家离开，不包括下线
	ESME_OnCreatureDie			=	4,			// 生物死亡
	ESME_OnRoleDie				=	5,			// 玩家死亡
	ESME_OnEnterTrigger			=	6,			// 进入触发器
	ESME_OnEnterArea			=	7,			// 进入区域
	ESME_CanInviteJoinTeam		=	8,			// 是否允许邀请组队
	ESME_CanLeaveTeam			=	9,			// 是否允许离开队伍
	ESME_CanChangeLeader		=	10,			// 是否能移交队长
	ESME_On_Revive				=	11,			// 玩家复活	
	ESME_CanEnterWhenOnline		=   12,			// 玩家上线时是否能计入地图
	ESME_GetExportMapAndCoord	=	13,			// 得到玩家离开当前地图后的地图ＩＤ和坐标
	ESME_GetOnePerfectMap		=   14,			// 找到最佳的副本实例
	ESME_CanEnter				=	15,			// 玩家是否能进入该地图
	ESME_FriendEnemy			=	16,			// 两个对象间的敌我关系
	ESME_CanKickMember			=	17,			// 是否允许踢掉队友
	ESME_OnCreatureDisappear	=	18,			// 生物消失
	ESME_Safeguard				=	19,			// 是否允许玩家开启保护模式
	ESME_CanUseItem				=	20,			// 是否允许使用物品
	ESME_CanUseSkill			=	21,			// 是否允许使用技能
	ESME_CityBroadCast			=	22,			// 城市广播
	ESME_OnLeaveArea			=	23,			// 玩家离开某一区域
	ESME_OnCreatePvPMap			=	24,			// 竞技场-创建一个新的竞技场地图
	ESME_OnPlayerLeaveW			=	25,			// 玩家离开，包括下线
	ESME_ExperienceInstance		=   26,			// 玩家进入体验副本事件

	ESME_End					=	27,
};

//---------------------------------------------------------------------------------
// 固定活动脚本
//---------------------------------------------------------------------------------
enum EScriptActEvent
{
	ESAE_OnInit				=	0,			// 初始化时
	ESAE_OnTimer			=	1,			// 定时更新
	ESAE_OnStart			=	2,			// 活动开始
	ESAE_OnEnd				=	3,			// 活动结束
	ESAE_OnTimerMin			=	4,			// 活动每分钟更新
	ESAE_OnDefaultRequest	=	5,			// 客户端触发服务器脚本的缺省消息 
	ESAE_Broad				=	6,			// 活动广播
	ESAE_FinishEudemonSummon	=	7,		// 完成守护神召唤
	ESAE_OnClientCallback		= 8,		// 活动，读取客户端反馈用
	ESAE_OnActOpenClose	= 9,				// 控制台开启或关闭活动
	ESAE_End				=	10,
};

//---------------------------------------------------------------------------------
// 游戏世界事件脚本
//---------------------------------------------------------------------------------
enum EScriptWorldEvent
{
	ESWE_Adventure			=	0,			// 奇遇产生
	ESWE_OnCreatePvPMap3v3  =	1,          // 创建竞技场 
	ESWE_OnLoot				=	2,			// 掉落
	ESWE_BeKill_DropSilver	=	3,			// 怪物被击杀后掉落金钱
	ESWE_CreateItem			=	4,			// 生成一个新道具
	ESWE_DelItem			=	5,			// 物品消失（被点化或者被删除或者时间到或者出售，不包括直接交易和摆摊交易）
	ESWE_ExchangeItem		=	6,			// 物品被交易（通过摆摊售卖）
	
	ESWE_GetSoulCrystalQlty = 7,	// 获得魂晶品级
	ESWE_GetSoulCrystalSkillID = 8, // 获得魂晶技能id
	ESWE_GetSoulCrystalAtt = 9, // 获得魂晶属性id

    ESWE_End =	 10,
};

//---------------------------------------------------------------------------------
// 物品事件脚本
//---------------------------------------------------------------------------------
enum EScriptItemEvent
{
	ESIE_CanUse									=	0,			// 物品是否可用
	ESIE_Use											=	1,			// 物品使用
	ESIE_CanDelGroundItem					=	2,			// 掉在地上的物品是否可以删除
	ESIE_PutDownItem							=	3,			// 丢弃物品
	ESIE_CanPickUpItem						=	4,			// 判断是否可以捡起物品
	ESIE_PickUpItem								=	5,			// 捡起物品时触发
	ESIE_CanPutDown							= 6,			// 判断物品是否可以丢弃
	ESIE_CanDelItemOnRoleInit			= 7,			// 角色初始化时候，检查背包中物品需要删除（0表示需要删除， 1表示不需要）
	ESIE_Get		=	8,			// 物品获得

	ESIE_End
};

//---------------------------------------------------------------------------------
// 技能事件脚本
//---------------------------------------------------------------------------------
enum EScriptSkillEvent
{
	ESSE_CanCast			=	0,			// 技能是否可用
	ESSE_Cast				=	1,			// 技能使用	
	ESSE_SealSkill			=	2,			// 封印
	ESSE_CancelSkill		=	3,			// 取消技能使用

	ESSE_OnSkillChange			=	4,			// 玩家技能改变，学会，等级变化等

	ESSE_Cast2SingleTarget	=	5,			// 使用单目标的技能

	ESSE_End
};

enum EScriptMotionEvent
{
	ESMOTION_Motion         =   0,          // 个性动作播放
	ESMOTION_End
};

enum EScriptBuffEvent
{
	ESBUFF_Destroy			=	0,			// Buff结束事件

	ESBUFF_End,
};

//---------------------------------------------------------------------------------
// 通用脚本类
//---------------------------------------------------------------------------------
template<INT nSize>
class Script
{
public:
	VOID RegisterFunc(INT nIndex, const CHAR* szFunc);
	VOID Destroy();

	CONST CHAR* operator[] (INT ndx)const 
	{
		if( ndx >= 0 && ndx < nSize )
			return m_szFunc[ndx];
		return NULL;
	}

protected:
	Script();
	~Script();

protected:
	CHAR*	m_szFunc[nSize];		// 脚本函数字符串
};

template<INT nSize>
inline Script<nSize>::Script()
{
	ZeroMemory(m_szFunc, sizeof(m_szFunc));
}

template<INT nSize>
inline Script<nSize>::~Script()
{
	Destroy();
}

template<INT nSize>
inline VOID Script<nSize>::RegisterFunc(INT nIndex, const CHAR* szFunc)
{
	if( nIndex < 0  || nIndex >= nSize ) return;
	if( !P_VALID(szFunc) ) return;

	if( P_VALID(m_szFunc[nIndex]) )
	{
		TObjRef<Util> pUtil = "Util";
		IMSG(_T("%s is Registerred twice\r\n"), pUtil->AnsiToUnicode(szFunc));
		free(m_szFunc[nIndex]);
	}

	m_szFunc[nIndex] = _strdup(szFunc);
}

template<INT nSize>
inline VOID Script<nSize>::Destroy()
{
	for(INT n = 0; n < nSize; ++n)
	{
		if( P_VALID(m_szFunc[n]) )
		{
			free(m_szFunc[n]);
			m_szFunc[n] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------
// 个性动作脚本
//--------------------------------------------------------------------------------
class MotionScript : public Script<ESMOTION_End>
{
	friend class ScriptMgr;
private:
	~MotionScript() {}
public:
	void OnMotion(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwDestRoleID,DWORD dwMotionID)const;

};
//--------------------------------------------------------------------------------
// buff脚本
//--------------------------------------------------------------------------------
class BuffScript : public Script<ESBUFF_End>
{
	friend class ScriptMgr;
private:
	~BuffScript() {}
public:
	void OnDestroy(DWORD dwMapID,DWORD dwInstanceID,DWORD dwRoleID,DWORD dwBuffID)const;

};
//---------------------------------------------------------------------------------
// 任务脚本
//---------------------------------------------------------------------------------
class QuestScript : public Script<ESQE_End>
{
	friend class ScriptMgr;
private:
	~QuestScript() {}
public:
	INT  CheckAccept(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;
	INT  CheckComplete(UINT16 u16QuestID, Role* pRole, Creature* pNPC) const;

	VOID OnInit(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnAccept(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnBoardQuestComplete(UINT16 u16QuestID, Role* pOwner, Creature* pNPC) const;
	VOID OnCancel(UINT16 u16QuestID, Role* pOwner) const;
	VOID OnCreatureKill(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
	VOID OnNPCTalk(UINT16 u16QuestID, Role* pOwner, DWORD dwNPCID, DWORD dwNPCTypeID) const;
	VOID OnDlgDefault(UINT16 u16QuestID, Role* pOwner, DWORD dwDlgOption) const;
	VOID OnInvest(UINT16 u16QuestID, Role* pOwner, DWORD dwCreatureTypeID) const;
	VOID OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const;

	
};

//----------------------------------------------------------------------------------
// 怪物AI脚本
//----------------------------------------------------------------------------------
class CreatureScript : public Script<ESCAE_End>
{
	friend class ScriptMgr;
private:
	~CreatureScript() {}
public:
	VOID OnLoad(Creature* pCreature) const;
	VOID OnTalk(Creature* pCreature, Role* pRole, INT nIndex=-1) const;
	VOID OnRespawn(Creature* pCreature) const;
	VOID OnEnterCombat(Creature* pCreature) const;
	VOID OnLeaveCombat(Creature* pCreature) const;
	VOID OnDie(Creature* pCreature, Unit* pKiller, BOOL& bRemove) const;
	VOID OnInvest(Creature* pCreature, Role* pScr) const;

	VOID OnUpdateAI(Creature* pCreature) const;
	VOID OnUpdateCurAI(Creature* pCreature) const;
	VOID OnEnterCurAI(Creature* pCreature) const;
	VOID OnLeaveCurAI(Creature* pCreature) const;
	VOID OnDisappear(Creature* pCreature) const;
	BOOL OnEventCurAI(Creature* pCreature) const;

	BOOL IsShopOpened(Creature* pCreature,Role * player,Map* pMap) const;
	BOOL CanShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const;
	VOID OnShopping(Creature* pCreature,Role * player,Map* pMap,DWORD dwShopID,DWORD dwItemTypeID ) const;

	VOID OnBeAttacked(Creature* pCreature, Unit* pSrc) const;

	VOID OnBePulled(Creature * pCreature,const Vector3 & Pos,BOOL result) const;
};

//-------------------------------------------------------------------------------------
// 玩家脚本
//-------------------------------------------------------------------------------------
class RoleScript : public Script<ESRE_End>
{
	friend class ScriptMgr;
private:
	~RoleScript() {}
public:
	VOID	OnRoleOnline(Role* pRole) const;
	VOID	OnRoleFirstOnline(Role* pRole) const;
	VOID	OnRoleIntoWorld(Role* pRole) const;
	VOID	OnRoleFirstIntoWorld(Role* pRole) const;
	VOID	OnRoleEnterMap(Role* pRole) const;
	BOOL	IsDeadPenalty(Role* pRole, EDeadPenaltyMode eMode) const;
	VOID	OnRoleLevelChange(Role* pRole) const;
	VOID	OnOpenChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnStopChest(Role* pRole, DWORD dwChestID, DWORD dwKeyID) const;
	VOID	OnAgainChest(Role* pRole) const;
	VOID	OnGetItem(Role* pRole, DWORD dwChestID, DWORD dwItemID, INT nNum) const;
	VOID	OnStartChest(Role* pRole) const;
	VOID	OnGetMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnGetMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	VOID	OnBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	VOID	OnForceBreakMarriage(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	DWORD	OnForceBreakMarriageCheck(Role* pRole, DWORD dwApplicantRoleID,DWORD dwTargetRoleID) const;
	INT		OnGetRoleFishingExp(Role* pRole) const;
	DWORD	CanCancelPenalty(Role* pRole, Unit* pKiller, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommerce) const;
	VOID	OnCompleteCommerce(Role* pRole,DWORD dwTaelProgress) const;
	DWORD	OnWaitSummon(Role* pRole, DWORD dwCreatureID, BOOL& bRet) const;
	DWORD	OnCancelSummon(Role* pRole, DWORD dwCreatureID) const;
	VOID	OnUseJingWuPoint(Role* pRole, INT nPoint) const;
	INT		IsTrainFinished(Role* pRole, DWORD dwTrainID) const;									// 龙之试炼系统--完成试炼
	VOID	OnObtainItem(Role* pRole, DWORD dwItemID, EItemCreateMode eCreateMode) const;			// 玩家获得一件道具的事件接口
	VOID	OnSilverChange(Role* pRole, DWORD dwSilverIncrease) const;								// 玩家游戏币增加的事件接口
	VOID	OnObtainTitle(Role* pRole, DWORD dwTitleID) const;										// 玩家获得一个称号的事件接口
	VOID	OnItemConsolidate(Role* pRole, DWORD dwEquiptID, EAllConsolidateType eConsolidateType,
	INT nConsolidateLevel, BOOL bSuccess) const;							// 玩家完成一次装备强化的事件接口
	VOID	OnActiveLonghunSuccess(Role* pRole, DWORD dwEquiptID, DWORD dwLongHunSkillID) const;	// 玩家激活龙魂成功的事件接口
	VOID	OnTrainFinished(Role* pRole) const;														// 玩家完成一个试炼

	VOID	OnHandleMaster(Role* pMaster, Role* pApprentice) const;

	VOID	OnFabaoAttChange(Role* pOwner,CONST tagFabao * pFabao,INT nWhat,DWORD dwOther) const;

	VOID    OnRoleJoinToArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanJoin)const;
	VOID    OnRoleCreateArenaRoom(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,BOOL& bCanCreate)const;
	VOID    OnRoleAgreeToBeInvited(Role* pRole,DWORD dwRoomType,DWORD dwRoomId,DWORD dwDestRoleID,BOOL& bCanBeInvited)const;

	VOID	OnReadyToSoaring(Role * pRole) const;
	VOID	OnCompletePracticeEquipment(Role* pRole,DWORD dwItemTypeID) const;
	VOID	OnCompleteCastingEquipment(Role* pRole,DWORD dwItemTypeID,INT nCastingLevel) const;

	VOID	OnChaseHolderDie(Role * pRole, DWORD dwKillerID) const;
	VOID	ForceLootChase(Role * pRole) const;
	VOID	OnMakeFriend(Role * pRole, DWORD dwFriendID) const;
	VOID	OnLearnSkill(Role * pRole, DWORD dwSkillID) const;
	VOID	OnEquipConsolidateLevelUp(Role * pRole, DWORD dwTypeID, BYTE byBeforeLvl, BYTE byAfterLvl) const;
	VOID	OnProduceEquip(Role * pRole, DWORD dwTypeID, BYTE byQuality) const;
	VOID	OnDigHoleOnEquip(Role * pRole, DWORD dwTypeID, BOOL bSuccess, BYTE byHoleNum) const;
	DWORD	OnCutStone(Role * pRole, DWORD dwTypeID, DWORD dwNewStoneTypeID, DWORD dwDestTypeID) const;
	VOID	OnDressEquip(Role * pRole, bool bEquipOrNot, DWORD dwTypeID, BYTE byEquipPos, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const;
	VOID	OnBrandEquip(Role * pRole, DWORD dwTypeID,  DWORD dwBrandItemTypeID, BOOL bSuccess, BYTE byBrandLevel) const;
	VOID	OnAddItemToBag	(Role * pRole, DWORD dwTypeID) const;
	// 神铸成功事件
	VOID	OnGodStrengthenSuccess(Role * pRole,tagEquip * pEquip) const;

	DWORD	OnUseGoldStone(Role * pRole,	tagItem* pItem) const;

	VOID	OnCreateGuild(Role * pRole,DWORD dwGuildID) const;
	DWORD	CanAddEnemy(Role * pRole,DWORD dwKillerID) const;
	DWORD	OnKillMonster(Role * pRole, Creature * pCreature) const ;

	VOID	OnRoleCondenseUpgrade(Role* pRole) const;
	VOID	OnRoleGetClergy(Role* pRole) const;
	VOID	OnRoleGetRareArea(Role* pRole, DWORD dwAreaID) const;
	VOID	OnRoleWinRareArea(Role* pRole, DWORD dwRoleID, DWORD dwAreaID) const;
	VOID	OnRoleActivateArea(Role* pRole, DWORD dwAreaID) const;
	BOOL	IsCanRobRareArea(Role* pRole) const;

	BOOL	CanUseDakNPC(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const;
	BOOL	CanUseEarthTemple(Role* pRole, DWORD dwDestMapID, FLOAT x, FLOAT y, FLOAT z) const;

	BOOL	CanCalCrit(Role* pRole, DWORD dwTargetID) const;
	VOID	OnRoleLogout(Role* pRole) const;

	VOID	OnRoleWardLevelup(Role* pRole) const;

	VOID	OnCompleteFashionSuitNumChange(Role* pRole) const;

	// 完成任务后获得金钱
	VOID IncSilverByQuest(Role* pRole, DWORD dwQuestID, INT nSilver) const;

	// 通过购买道具消耗金币
	VOID DecSilverByBuyItem(Role* pRole, DWORD dwItemID, INT nSilver) const;

	// 通过提升装备强化等级消耗金币
	VOID DecSilverByStrengthen(Role* pRole, DWORD dwTypeID, INT nQuality, INT nLevel, INT nSilver ) const;

	// 通过合成消耗金币
	VOID DecSilverByCompose( Role* pRole, DWORD dwTypeID, INT nSilver ) const;

	DWORD CanStrengthenEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow, INT nQuality, INT nLevel) const;

	DWORD OnRoleEquipGetGrowID(Role* pRole, DWORD dwTypeID, INT nQlty) const;

	BYTE OnRoleEquipGrow(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
		BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM) const;

	VOID OnGetBoardQuestReward(Role*, UINT16, UINT16, INT, INT) const;

	VOID RenewSoulCrystalAtt(Role* pRole, DWORD dwTypeID, BYTE byQlty, INT* nAtt, OUT DWORD& dwErrorCode, OUT INT* nRetAttID) const;

	VOID AccountSuccessReactive(Role*) const;

	DWORD OnRoleMapChangeWithScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow,DWORD dwMapID) const;
	DWORD OnRoleTryToWareScriptEquip(Role* pRole, DWORD dwTypeID, DWORD dwSerialIDHigh, DWORD dwSerialIDLow) const;

	DWORD OnUseDiamondAddAndEquipGrowIM(Role* pRole, DWORD dwTypeID, INT64 n64EquipID) const;
	//DWORD RewardReactive(Role* pRole, BYTE byRewardLevel) const;
	DWORD OnRefreshBaoXiang(DWORD dwScole, DWORD &dwItemID1, DWORD &dwItemID2, DWORD &dwItemID3) const;
	VOID OnGetHolyDevourNum(Role* pRole, DWORD dwDevourNum) const;
};

//-------------------------------------------------------------------------------------
// 地图脚本
//-------------------------------------------------------------------------------------
class MapScript : public Script<ESME_End>
{
	friend class ScriptMgr;
private:
	~MapScript() {}
public:
	VOID	OnInit(Map* pMap) const;
	VOID	OnTimer(Map* pMap, INT nMilliSec) const;
	VOID	OnPlayerEnter(Role* pRole, Map* pMap) const;
	VOID	OnPlayerLeave(Role* pRole, Map* pMap, BOOL bLogout) const;
	VOID	OnCreatureDie(Creature* pCreature, Unit* pKiller, Map* pMap) const;
	VOID	OnRoleDie(Role* pRole, Unit* pKiller, Map* pMap) const;
	VOID	Revive(Role* pRole, ERoleReviveType eType, INT &nReviveHP, INT &nReviveMP, FLOAT &fx, FLOAT &fy, FLOAT &fz, DWORD &dwRebornMapID) const;
	VOID	OnEnterTrigger(Role* pRole, tagMapTriggerInfo* pTrigger, Map* pMap) const;
	VOID	OnEnterArea(Role* pRole, tagMapAreaInfo* pArea, Map* pMap) const;
	VOID	OnLeaveArea(Role* pRole, DWORD dwCurScriptArea, Map* pMap) const;
	INT		CanInviteJoinTeam(Map* pMap) const;
	INT		CanLeaveTeam(Map* pMap) const;
	INT		CanChangeLeader(Map* pMap) const;
	INT		CanKickMember(Map* pMap) const;
	DWORD	FriendEnemy(Map* pMap, Unit* pSrc, Unit* pTarget, BOOL& bIgnore) const;
	VOID	OnCreatureDisappear(Creature* pCreature, Map* pMap) const;
	INT		CanSetSafeguard(Map* pMap) const;
	BOOL	CanUseSkill(Map* pMap, DWORD dwTypeID) const;
	BOOL	CanUseItem(Map* pMap, DWORD dwTypeID) const;
	BOOL	OnCityBroadCast(DWORD dwBroadType,DWORD dwPara1 = GT_INVALID,DWORD dwPara2 = GT_INVALID\
							,DWORD dwPara3 = GT_INVALID,DWORD dwPara4 = GT_INVALID,DWORD dwPara5 = GT_INVALID) const;
	BOOL	OnCheckRoleLevel(DWORD dwMapID, DWORD dwInstanceID, DWORD dwRoleID) const;
    
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	// 竞技场-创建一个新的竞技场地图
	BOOL	OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const;

	// 脚本创建的副本独有接口
	INT		GetExportMapAndCoord(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	CanTakeOverWhenOnline(Role* pRole, DWORD& dwOutMapID, Vector3& vOut) const;
	VOID	GetOnePerfectMap(Role* pRole, DWORD &dwInstanceID) const;
	INT		CanEnter(Role* pRole) const;
};

//-------------------------------------------------------------------------------------
// 活动脚本
//-------------------------------------------------------------------------------------
class ActScript : public Script<ESAE_End>
{
	friend class ScriptMgr;
private:
	~ActScript() {}
public:
	VOID OnInit(DWORD dwActID) const;
	VOID OnTimer(DWORD dwActID, INT nSec) const;
	VOID OnTimerMin(DWORD dwActID) const;
	VOID OnActStart(DWORD dwActID) const;
	VOID OnActEnd(DWORD dwActID) const;
	VOID OnDefaultRequest(DWORD dwActID, Role* pRole, DWORD	dwEventType) const;
	VOID BroadActivityState(EActBroadMode eState) const;
	VOID OnClientCallback(INT nMsgID,Role* pPlayer,DWORD param1,DWORD param2,DWORD param3,DWORD param4) const;
	VOID FinishEudemonSummon() const;
	VOID OnActOpenClose(DWORD dwActID, DWORD dwIsOpen) const;
};

//-------------------------------------------------------------------------------------
// 物品脚本
//-------------------------------------------------------------------------------------
class ItemScript : public Script<ESIE_End>
{
	friend class ScriptMgr;
private:
	~ItemScript() {}
public:
	BOOL CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, BOOL &bIgnore) const;
	BOOL CanUseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, DWORD dwDesRoleID, BOOL &bIgnore) const;
	// Jason 2010-12-3 v1.3.0 注意，返回0时，走正常的使用，并扣除使用次数（1次）的流程，返回1则不扣使用次数。
	INT UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID) const;
	VOID UseItem(Map* pMap, DWORD dwTypeID, DWORD dwTargetID, DWORD dwDesRoleID) const;
	INT GroundItemCanDel(DWORD dwTypeID, INT64 n64Serial) const;
	VOID PutDownItem(DWORD dwTypeID) const;
	INT		CanPickUpItem(DWORD dwItemID, DWORD dwRoleID) const;
	VOID	OnPickUpItem(DWORD dwItemID, DWORD dwRoleID) const;
	INT		ItemCanPutDown(DWORD dwTypeID, INT64 n64Serial) const;
	BOOL	CanDeleteItem(DWORD dwTypeID, INT64 n64Serial) const;
	VOID GetItem(Map* pMap, DWORD dwTypeID, DWORD dwRoleID, EItemCreateMode eGetMode, DWORD n32High, DWORD n32Low) const;
};

//-------------------------------------------------------------------------------------
// 技能脚本
//-------------------------------------------------------------------------------------
class SkillScript : public Script<ESSE_End>
{
	friend class ScriptMgr;
private:
	~SkillScript () {}
public:
	DWORD CanCastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwDstUnitID) const;
	VOID CastSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, BOOL &bIgnore) const;
	VOID CastSingleTargetSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, BOOL &bIgnore) const;
	VOID SealSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const;
	VOID CancelSkill(Map* pMap, DWORD dwSkillID, DWORD dwOwnerID, DWORD dwTargetID, DWORD dwTargetTypeID) const;

	VOID OnSkillChange(Map* pMap,DWORD dwRoleID,DWORD dwSkillID,INT nPrevVal,INT nCurVal) const;
};


//-------------------------------------------------------------------------------------
// 游戏世界事件脚本
//-------------------------------------------------------------------------------------
class WorldScript : public Script<ESWE_End>
{
	friend class ScriptMgr;
private:
	~WorldScript() {}
public:
	VOID OnAdventure(Role* pRole) const;
	BOOL OnLoot(DWORD dwSrcID, DWORD dwTargetID, ELootType eType, DWORD dwTypeID, DWORD dwSetID = 0) const;

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	// 竞技场-创建一个新的竞技场地图
	BOOL	OnCreatePvPMap3v3(DWORD dwMapID, DWORD dwInstanceID, INT16 nRoomID, DWORD* pdwPlayerRoleID, INT16 nPlayerCnt) const;


	// 生成一个新道具
	VOID CreateItem( DWORD dwMapID, DWORD dwInstanceID, DWORD dwTypeID, INT nQuality ) const;

	//物品消失（被点化或者被删除或者时间到或者出售，不包括直接交易和摆摊交易）
	VOID DelItem( DWORD dwDelType, DWORD dwTypeID, INT nQuality ) const;

	// 物品被交易（通过摆摊售卖）
	VOID ExchangeItem( Map* pMap, DWORD dwRoleID, DWORD dwBuyRoleID, DWORD dwTypeID, INT nQuality, INT nSilver ) const;

	//怪物被击杀后掉落金钱
	VOID DropSilver(Creature * pCreature, DWORD dwRoleID, DWORD dwType, INT nSilver) const;

	// 魂晶相关
	VOID GetSoulCrystalQlty(DWORD dwTypeID, BYTE & Qlty) const ;
	DWORD GetSoulCrystalSkillID(DWORD dwTypeID, BYTE byQlty) const;
	VOID GetSoulCrystalAtt(DWORD dwRoleID, DWORD dwTypeID, BYTE byQlty, OUT INT* nRetAttID, bool bFirstCreate = true, INT nRateAdd1 = 0, INT nRateAdd2 = 0,
		INT* nAttID = 0) const ;
};

//-------------------------------------------------------------------------------------
// 脚本管理器
//-------------------------------------------------------------------------------------
class ScriptMgr : public EventMgr<ScriptMgr>
{
public:
	//---------------------------------------------------------------------------------
	// 初始化和销毁
	//---------------------------------------------------------------------------------
	BOOL Init();
	VOID Update();
	VOID Destroy();
	
	//---------------------------------------------------------------------------------
	// 注册脚本函数
	//---------------------------------------------------------------------------------
	VOID RegisterCreatureEvent(DWORD dwID, EScriptCreatureEvent eEvent, const CHAR* szFunction);
	VOID RegisterQuestEvent(UINT16 u16QuestID, EScriptQuestEvent eEvent, const CHAR* szFunction);
	VOID RegisterRoleEvent(EScriptRoleEvent eEvent, const CHAR* szFunction);
	VOID RegisterMapEvent(const CHAR* szMapName, EScriptMapEvent eEvent, const CHAR* szFunction);
	VOID RegisterActEvent(DWORD dwActID, EScriptActEvent eEvent, const CHAR* szFunction);
	VOID RegisterWorldEvent(EScriptWorldEvent eEvent, const CHAR* szFunction);
	VOID RegisterItemEvent(DWORD dwTypeID, EScriptItemEvent eEvent, const CHAR* szFunction);
	VOID RegisterSkillEvent(DWORD dwTypeID, EScriptSkillEvent eEvent, const CHAR* szFunction);	
	VOID RegisterMotionEvent(EScriptMotionEvent eEvent, const CHAR* szFunction);
	VOID RegisterBuffEvent(EScriptBuffEvent eEvent, const CHAR* szFunction);

	//---------------------------------------------------------------------------------
	// 生成脚本对象
	//---------------------------------------------------------------------------------
	const CreatureScript*	GetCreatureScript(DWORD dwCreatureID)	{ return m_mapCreatureScript.Peek(dwCreatureID); }
	const QuestScript*		GetQuestScript(UINT16 u16QuestID)		{ return m_mapQusetScript.Peek(u16QuestID); }
	const MapScript*		GetMapScript(DWORD dwMapID)				{ return m_mapMapScript.Peek(dwMapID); }
	const RoleScript*		GetRoleScript()							{ return m_pRoleScript; }
	const ActScript*		GetActScript(DWORD dwActID)				{ return m_mapActScript.Peek(dwActID); }
	const WorldScript*		GetWorldScript()						{ return m_pWorldScript; }
	const ItemScript*		GetItemScript(DWORD dwTypeID)			{ return m_mapItemScript.Peek(dwTypeID); }
	const SkillScript*		GetSkillScript(DWORD dwTypeID)			{ return m_mapSkillScript.Peek(dwTypeID); }
	const MotionScript*     GetMotionScript()                       { return m_pMotionScript; }
	const BuffScript*		GetBuffScript()							{ return m_pBuffScript; }
	//---------------------------------------------------------------------------------
	// 调用脚本
	//---------------------------------------------------------------------------------
	VOID	CallScriptFunction(CHAR* szFunction, CHAR* szFormat, ...);

	//---------------------------------------------------------------------------------
	// 生成脚本锁
	//---------------------------------------------------------------------------------
	DWORD	CreateScriptMutex();

	//---------------------------------------------------------------------------------
	// 锁住解除某个脚本锁
	//---------------------------------------------------------------------------------
	VOID	LockScriptMutex(DWORD dwMutexID);
	VOID	UnLockScriptMutex(DWORD dwMutexID);

	//---------------------------------------------------------------------------------
	// 获取全局变量
	//---------------------------------------------------------------------------------
	template<typename T>
	VOID	GetGlobal(LPCSTR strVarName, OUT T &res);

private:
	//---------------------------------------------------------------------------------
	// 注册异步事件函数
	//---------------------------------------------------------------------------------
	static VOID	RegisterScriptEventFunc();

	//---------------------------------------------------------------------------------
	// 异步事件处理函数
	//---------------------------------------------------------------------------------
	VOID	OnReload(DWORD dwSenderID, LPVOID pEventMessage);

	//---------------------------------------------------------------------------------
	// 初始化
	//---------------------------------------------------------------------------------
	VOID	LoadScripts();
	VOID	LoadScriptsFromDir(LPCTSTR szDir, std::vector<tstring>& luaFiles, BOOL bFirst=FALSE);
	VOID	RegisterCoreFunctions();

	//-----------------------------------------------------------------------------------
	// 销毁
	//-----------------------------------------------------------------------------------
	VOID	DestroyScripts();
	VOID	DestroyAllStates();
	VOID	UnloadScripts();

	//-----------------------------------------------------------------------------------
	// 重新加载脚本
	//-----------------------------------------------------------------------------------
	VOID	Reload();
	
	//-----------------------------------------------------------------------------------
	// 得到一个线程状态
	//-----------------------------------------------------------------------------------
	lua_State* GetThreadState();
	lua_State* CreateThreadState(DWORD dwThreadID);

	//-----------------------------------------------------------------------------------
	// 错误显示
	//-----------------------------------------------------------------------------------
	VOID ErrMsg(lua_State* pState);
public:
	VOID MemMsg();
	
private:
	typedef TMap<UINT16, QuestScript*>			QuestScriptMap;
	typedef TMap<DWORD, CreatureScript*>		CreatureScriptMap;
	typedef TMap<DWORD, MapScript*>				MapScriptMap;
	typedef TMap<DWORD, ActScript*>				ActScriptMap;
	typedef TMap<DWORD, ItemScript*>			ItemScriptMap;
	typedef TMap<DWORD, SkillScript*>			SkillScriptMap;

private:
	TObjRef<Util>					m_pUtil;
	Mutex							m_Lock;					// 用于生成线程状态的锁

	lua_State*						m_pMasterState;			// 主状态
	TSafeMap<DWORD, lua_State*>		m_mapThreadState;		// 各个线程状态

	TMap<DWORD, Mutex*>				m_mapScriptMutex;		// 脚本锁，用于需要锁定运行的脚本函数
	DWORD							m_dwScriptMutexIDGen;	// 脚本锁ID生成器

	QuestScriptMap					m_mapQusetScript;		// 任务脚本表（任务ID---脚本）
	CreatureScriptMap				m_mapCreatureScript;	// 怪物脚本表（怪物ID——脚本）
	MapScriptMap					m_mapMapScript;			// 地图脚本表（地图属性ID——脚本）
	ActScriptMap					m_mapActScript;			// 活动脚本表（活动ID－－脚本）
	ItemScriptMap					m_mapItemScript;		// 物品脚本表（物品TypeID－－脚本)
	SkillScriptMap					m_mapSkillScript;		// 技能脚本
    
	MotionScript*                   m_pMotionScript;        // 个性动作触发事件脚本
	BuffScript*						m_pBuffScript;			// Buff触发事件脚本
	WorldScript*					m_pWorldScript;			// 游戏世界脚本
	RoleScript*						m_pRoleScript;			// 玩家脚本
	DWORD							m_dwMaxPcallTime;		// 脚本调用的最长时间
};


//-----------------------------------------------------------------------------------
// 生成脚本锁
//-----------------------------------------------------------------------------------
inline DWORD ScriptMgr::CreateScriptMutex()
{
	Mutex* pMutex = new Mutex;

	++m_dwScriptMutexIDGen;
	m_mapScriptMutex.Add(m_dwScriptMutexIDGen, pMutex);

	return m_dwScriptMutexIDGen;
}

//------------------------------------------------------------------------------------
// 锁住某个脚本锁
//------------------------------------------------------------------------------------
inline VOID ScriptMgr::LockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Acquire();
	}
}

//-------------------------------------------------------------------------------------
// 解除某个脚本锁
//-------------------------------------------------------------------------------------
inline VOID ScriptMgr::UnLockScriptMutex(DWORD dwMutexID)
{
	Mutex* pMutex = m_mapScriptMutex.Peek(dwMutexID);

	if( P_VALID(pMutex) )
	{
		pMutex->Release();
	}
}

//---------------------------------------------------------------------------------
// 获取全局变量
//---------------------------------------------------------------------------------
template<typename INT>
VOID ScriptMgr::GetGlobal(LPCSTR strVarName, OUT INT &res)
{
	lua_State* pThreadState = GetThreadState();
	if( !P_VALID(pThreadState) ) return;

	m_Lock.Acquire();

	lua_getglobal(pThreadState, strVarName);
	if( lua_isnil(pThreadState, 1) )
	{
		IMSG(_T("Script Error: unknown Global Int Val%s\r\n"), m_pUtil->AnsiToUnicode(strVarName));
		lua_pop(pThreadState, 1);
		return;
	}
	res = luaL_checkint(pThreadState, 1);

	lua_pop(pThreadState, 1);

	m_Lock.Release();
}

extern ScriptMgr g_ScriptMgr;

#define ParseINT64(n64ID, nHigh, nLow) \
{\
	const INT32 nMask	= 0xFFFFFFFF; \
	nHigh	= (INT32)((n64ID >> 32) & nMask); \
	nLow	= (INT32)(n64ID & nMask); \
}