//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong.h
// author: Sxg
// actor:
// data: 2008-05-08
// last:
// brief: 程序框架定义
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/base_define.h"
//-----------------------------------------------------------------------------
#define ID_GAMESERVER		1
#define SLEEPTIME			100

//-----------------------------------------------------------------------------
struct tagNDBS_LoadRole;
struct tagNDBS_LoadSimRole;
struct tagNDBS_CreateRole;
struct tagNDBS_SaveRole;
struct tagNDBS_LoadBaiBao;
struct tagNDBS_LoadBaiBaoLog;
struct tagNDBS_GetAllGPInfo;

class RTSession;
class CLoongDB;
class CLoongRank;
#include "../serverdefine/mutex.h"
//-----------------------------------------------------------------------------
// 消息接收队列(比直接使用MsgQueueTS减少一次内存分配及拷贝)
//-----------------------------------------------------------------------------
class DBMsgQueue
{
	friend class RTSession;

public:
	DBMsgQueue()	{ m_hEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);}
	~DBMsgQueue()	{ CloseHandle(m_hEvent); }

public:
	// 添加消息到队列尾，返回是否添加成功
	VOID AddMsg(LPBYTE pMsg)
	{
		m_MsgQueue.PushBack(pMsg);
		
		// 激活线程
		::SetEvent(m_hEvent);
	}

	// 从队列头取消息，外边需负责释放内存
	LPBYTE	GetMsg()
	{
		return m_MsgQueue.PopFront();
	}

	// 取得事件，当队列中加入消息时，此事件被激活
	HANDLE	GetEvent() { return m_hEvent; }

	// 得到队列中的消息数目
	INT		GetMsgNum() { return m_MsgQueue.Size(); }

private:
	TSafeList<LPBYTE>	m_MsgQueue;
	HANDLE				m_hEvent;
};

//-----------------------------------------------------------------------------
// DB服务器框架类
//-----------------------------------------------------------------------------
class CLoong
{
public:
	CLoong();
	~CLoong();

public:
	// 初始化操作(需读取配置文件)
	BOOL Init(HINSTANCE hInst);
	// 是否需要关闭
	BOOL IsTerminate() { return m_bTerminate; }
	// 主循环
	VOID MainLoop();
	// 关闭线程
	VOID ShutDown() { InterlockedExchange((long*)(&m_bTerminate), TRUE); }

	BOOL IsConnect() { return !m_bGameServerLogout && m_bGameServerConnect; }

	VOID SendMsg(PVOID pSend, DWORD dwSize);

public:
	DWORD GetHeartbeat() const { return m_Status.dwHeartbeat; }


private: // 界面相关
	// 创建UI界面
	BOOL CreateUI(HINSTANCE hInst);
	// 更新UI界面
	VOID RefreshUI();

private: // 游戏服务器相关请求(参数dwDummy为满足NetCmdMgr处理要求)
	// 请求所有玩家简易信息
	DWORD LoadAllRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	// 请求角色基本信息(姓名、等级、外形和当前所在地图等)
	DWORD LoadSimRole(DWORD pDBMsg, DWORD dwDummy);
	// 请求加载单个角色simple信息
	DWORD LoadOneRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	// 请求角色信息(选定角色所有信息-- roledata, skill, buff, item, equip...)
	DWORD LoadRole(DWORD pDBMsg, DWORD dwDummy);
	//// 删除玩家的buff，功夫，item/equip等.用于清除数据库中玩家多余的记录
	//VOID DeleteRecord(DWORD pDBMsg, DWORD dwDummy);
	// 请求存储角色数据
	VOID SaveRole(DWORD pDBMsg, DWORD dwDummy);
	// 请求创建角色
	DWORD CreateRole(DWORD pDBMsg, DWORD dwDummy);
	// 请求删除角色
	DWORD DelRole(DWORD pDBMsg, DWORD dwDummy);
	// 请求恢复角色
	DWORD ResumeRole(DWORD pDBMsg, DWORD dwDummy);
	// 请求存储角色简易数据
	VOID SaveRoleInfo(DWORD pDBMsg, DWORD dwDummy);
	
	// 请求修改角色数据
	DWORD ChangeRoleName(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeRoleGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeRoleDressMdID(DWORD pDBMsg, DWORD dwDummy);

	// 初始化游戏世界
	DWORD InitWorldOK(DWORD pDBMsg, DWORD dwDummy);

	// 游戏世界关闭
	DWORD WorldShutDown(DWORD pDBMsg, DWORD dwDummy);
	
	// 向LoongWorld返回初始化完成
	DWORD LoongWorldInitOK(DWORD pDBMsg, DWORD dwDummy);

	// 读取物品64位序号等相关信息
	DWORD GetItemInfo(DWORD pDBMsg, DWORD dwDummy);	
	// 读取新物品(从item_baibao表中)到百宝袋中
	DWORD LoadNewItem2BaiBao(DWORD pDBMsg, DWORD dwDummy);
	// 读取百宝袋中元宝信息
	DWORD LoadBaiBaoYuanBaoInfo(DWORD pDBMsg, DWORD dwDummy);
	// 从item_baibao表中删除指定物品
	DWORD DelBaiBaoItem(DWORD pDBMsg, DWORD dwDummy);
	// 从item_baibao表中删除指定装备
	DWORD DelBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy);
	// 读取需要记录的物品TypeID
	DWORD LoadItemNeedLog(DWORD pDBMsg, DWORD dwDummy);
	// 读取所有玩家的元宝交易账户
	DWORD LoadAllYBAccount(DWORD pDBMsg, DWORD dwDummy);
	// 读取所有的元宝交易订单
	DWORD LoadAllYBOrder(DWORD pDBMsg, DWORD dwDummy);
	// 插入一个元宝交易账户
	DWORD InsertYBAccount(DWORD pDBMsg, DWORD dwDummy);
	// 更新元宝交易账户的元宝数量
	DWORD UpdateAccountYB(DWORD pDBMsg, DWORD dwDummy);
	// 更新元宝交易账户的金钱数量
	DWORD UpdateAccountSilver(DWORD pDBMsg, DWORD dwDummy);
	// 更新元宝交易账户出售订单的提交状态
	DWORD UpdateSubmitSell(DWORD pDBMsg, DWORD dwDummy);
	// 更新元宝交易账户收购订单的提交状态
	DWORD UpdateSubmitBuy(DWORD pDBMsg, DWORD dwDummy);
	// 插入元宝交易订单
	DWORD InsertOrder(DWORD pDBMsg, DWORD dwDummy);
	// 撤销或关闭元宝交易订单
	DWORD CompleteOrder(DWORD pDBMsg, DWORD dwDummy);
	// 更新元宝交易订单
	DWORD RefreshOrder(DWORD pDBMsg, DWORD dwDummy);
	// 查询一周内玩家交易订单
	DWORD GetRoleYBOrder(DWORD pDBMsg, DWORD dwDummy);
	// 删除数据库中时间超过七天的所有订单
	DWORD DeleteYBOrder(DWORD pDBMsg, DWORD dwDummy);

	// 百宝袋Log相关
	// 取得指定账号下Log信息
	DWORD GetBaibaoLogInfo(DWORD pDBMsg, DWORD dwDummy);
	// 插入一条Log记录
	DWORD InsertBaibaoLog(DWORD pDBMsg, DWORD dwDummy);
	
	// 帐号通用属性相关
	DWORD SetSafeCode(DWORD pDBMsg, DWORD dwDummy);
	DWORD ResetSafeCode(DWORD pDBMsg, DWORD dwDummy);
	DWORD CancelSafeCodeReset(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateWareSize(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateWareMoney(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBaiBaoYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBagYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateBagGold(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdatePersonalSet(DWORD pDBMsg, DWORD dwDummy);

	DWORD ChangeBagPsd(DWORD pDBMsg, DWORD dwDummy);

	// 读取固定活动数据
	DWORD LoadActivityData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveActivityData(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearMergedScriptData(DWORD pDBMsg, DWORD dwDummy);

	// 实力排行榜
	DWORD InitRankings(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadStrengthRankings();
	DWORD LoadLevelRankings();
	DWORD LoadFameRankings();
	DWORD LoadPetRankings();
	DWORD LoadEquipRankings();
    DWORD LoadWuXunRankings();
	DWORD LoadFlowerRankings();
	DWORD LoadEggRankings();
	DWORD UpdateRankings(DWORD pDBMsg, DWORD dwDummy);

	// 保存角色经验
	DWORD UpdateRoleAtt(DWORD pDBMsg, DWORD dwDummy);

	// 保存角色点
	DWORD UpdateRoleAttPoint(DWORD pDBMsg, DWORD dwDummy);

	// 保存角色天资点
	DWORD UpdateRoleTalentPoint(DWORD pDBMsg, DWORD dwDummy);

	//保存记录式传送符所保存的传送点信息
	DWORD UpdateRoleItemTransport( DWORD pDBMsg, DWORD dwDummy );
	//结婚
	DWORD RoleGetMarriage(DWORD pDBMsg, DWORD dwDummy);
	//离婚
	DWORD RoleBreakMarriage(DWORD pDBMsg, DWORD dwDummy);

	// 神职竞选
	DWORD LoadClergyRank(DWORD pDBMsg, DWORD);
	VOID LoadClergyMarsRank();
	VOID LoadClergyApolloRank();
	VOID LoadClergyRabbiRank();
	VOID LoadClergyPeaceRank();
	DWORD ChangeCandidateRecvFlag(DWORD pDBMsg, DWORD);
	DWORD GetClergyCandidateCount(DWORD pDBMsg, DWORD);
	DWORD CancelPreClergy(DWORD pDBMsg, DWORD);

	//稀有领域
	DWORD LoadAllRareArea(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaveRareItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD UnSaveRareItem(DWORD pDBMsg, DWORD dwDummy);

	//DWORD CreateRareArea(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateRareArea(DWORD pDBMsg,DWORD dwDummy);
	//DWORD DeleteRareArea(DWORD pDBMsg,DWORD dwDummy);

	//领域
	DWORD DeleteRoleArea(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateRoleArea(DWORD pDBMsg, DWORD dwDummy);
	//// 更新物品冷却时间
	//DWORD ReplaceItemCDTime(DWORD pDBMsg, DWORD dwDummy);
	
private: // 实时处理(为了处理上方便)
	// 好友
	DWORD InsertFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFriend(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAllFriRel(DWORD pDBMsg, DWORD dwDummy);

	// 友好度
	DWORD InsertFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFriendship(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAllFriValRel(DWORD pDBMsg, DWORD dwDummy);

	// 黑名单
	DWORD InsertBlack(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteBlack(DWORD pDBMsg, DWORD dwDummy);

	// 仇敌
	DWORD InsertEnemy(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteEnemy(DWORD pDBMsg, DWORD dwDummy);

	// 取所有仇敌列表中有角色ID为dwRoleID的角色的ID
	DWORD GetRoleListOfEnemy(DWORD pDBMsg, DWORD dwDummy);

	// 任务
	DWORD AcceptQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD CompleteQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DiscardQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearDailyCompleteQuest(DWORD pDBMsg, DWORD dwDummy);

	// 技能
	DWORD InsertSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateSkill(DWORD pDBMsg, DWORD dwDummy);
	
	// 物品&装备
	DWORD InsertItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelEquipFromRole(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateEquipSpec(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateEquipSingleAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveEquipSignature(DWORD pDBMsg, DWORD dwDummy);

	DWORD DelEquipdelByTime(CHAR *pszTime);
	DWORD DelItemdelByTime(CHAR *pszTime);
	// Jason 2010-4-14 v2.0.0 法宝相关
	DWORD InsertFabao(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateFabaoSpec(DWORD pDBMsg,DWORD dwDummy);
	// 魂晶
	DWORD InsertSoulCrystal(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateSoulCrystalSpec(DWORD pDBMsg,DWORD dwDummy);

	// wcy 2012-03-31 圣灵相关
	DWORD InsertHoly(DWORD pDBMsg,DWORD dwDummy);	
	DWORD UpdateHolySpec(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolyCoValue(DWORD pDBMsg,DWORD dwDummy);

	// wcy 2012-04-18 圣纹相关
	DWORD InsertHolyEquip(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolyEquipSpec(DWORD pDBMsg,DWORD dwDummy);
	

	// 百宝袋(item_baibao表)中物品
	DWORD InsertBaiBaoItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertBaiBaoEquip(DWORD pDBMsg, DWORD dwDummy);

	// 称号
	DWORD InsertTitle(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveTitle(DWORD pDBMsg, DWORD dwDummy);
	// 角色名帖
	DWORD UpdateVCard(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVCard(DWORD pDBMsg, DWORD dwDummy);

	// 声望、名人堂、氏族珍宝
	DWORD LoadRepRank(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFameHallInit(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertActTreasure(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertFameHallEnterSnap(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRepRstTime(DWORD pDBMsg, DWORD dwDummy);

	// 帮派属性相关
	
	DWORD LoadAllEmptyGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadAllGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD CheckGuildInitOK(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD DismissGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildTenet(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildName(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildName(DWORD pDBMsg, DWORD dwDummy);
	

	// 帮派成员属性相关
	DWORD LoadAllGuildMember(DWORD pDBMsg, DWORD dwDummy);
	DWORD JoinGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD LeaveGuild(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeGuildPos(DWORD pDBMsg, DWORD dwDummy);
	DWORD GuildWarePrivilege(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeMemberContribution(DWORD pDBMsg, DWORD dwDummy);
	// Jason 2010-3-11 v1.4.0帮派功勋变化
	DWORD GuildMemberChangeExploit(DWORD pDBMsg,DWORD dwDummy);

	// 帮派仓库
	DWORD LoadGuildWareItems(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildWareItems(DWORD pDBMsg, DWORD dwDummy);

	// 帮派设施
	DWORD LoadGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGuildFacilities(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveAllFacilities(DWORD pDBMsg, DWORD dwDummy);

	// 帮派技能
	DWORD LoadGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeResearchGuildSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGuildSkillInfo(DWORD pDBMsg, DWORD dwDummy);

	// 帮派跑商
	DWORD LoadGuildCommerceInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCommodityTael(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveCommodityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SetCommendation(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveCommerceRankInfo(DWORD pDBMsg, DWORD dwDummy);

	// 宠物相关
	DWORD GetPetInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreatePet(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeletePet(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdatePet(DWORD pDBMsg, DWORD dwDummy);

	DWORD InsertPetSkill(DWORD pDBMsg, DWORD dwDummy);
	DWORD DelPetSkill(DWORD pDBMsg, DWORD dwDummy);

	// 商城相关
	DWORD LoadAllGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD AddGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveGuildGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGPInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateGPTime(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadLimitItemInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveLimitItemInfo(DWORD pDBMsg, DWORD dwDummy);
	
	

	// VIP摊位相关
	DWORD GetAllVIPStallInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD ApplyVIPStall(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateVIPStall(DWORD pDBMsg, DWORD dwDummy);

	DWORD UpdateVNBPlayer(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVNBData(DWORD pDBMsg, DWORD dwDummy);
	DWORD AddBonusPoint(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadLeftMsg(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertLeftMsg(DWORD pDBMsg, DWORD dwDummy);
//	DWORD UpdateLeftMsg(DWORD pDBMsg, DWORD dwDummy);
	DWORD ClearLeftMsg(DWORD pDBMsg, DWORD dwDummy);

	//	开启宝箱计数相关
	DWORD UpdateTreasureSum(DWORD pDBMsg, DWORD dwDummy);

	// 城市系统
	DWORD LoadCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveCityInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateCityAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD GetGuildClanValue(DWORD pDBMsg, DWORD dwDummy);

	// 师徒
	DWORD LoadMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveOneMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertMasterApprenticeData(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteAnApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadForceBreakOutApprenticeList(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD RemoveForceBreakOutApprentice(DWORD pDBMsg, DWORD dwDummy);
	DWORD ModifyApprenticeJingWuPoint(DWORD pDBMsg, DWORD dwDummy);

	//竞技场
	DWORD SaveSerialVal(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveArenaData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadArenaSystemSerialVal(DWORD pDBMsg, DWORD dwDummy);

	//选秀活动
	DWORD SaveShowPlayerData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveShowPlayerPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveVotePlayerPoint(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveShowPlayerDataCount(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveVotePlayerDataCount(DWORD pDBMsg, DWORD dwDummy); 
	
	DWORD GetShowPlayerDataCount();
	DWORD GetVotePlayerDataCount();
	DWORD LoadShowPlayerData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadVotePlayerData(DWORD pDBMsg, DWORD dwDummy);

	//活动排行
	DWORD LoadActivetyRankData(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveActivityRankData(DWORD pDBMsg, DWORD dwDummy);

	//bloodbrother
	DWORD ClearTeatherInfo(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveAccountSpec(DWORD pDBMsg, DWORD dwDummy);

	DWORD AccountReactiveOrNot(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRewardReactiveLevel(DWORD, DWORD);

private: // log
	DWORD LogSilver(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogExVolume(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogVIPPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogTimeStat(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMallSell(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMallSellPack(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogItemTimes(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogGodBid(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogBattlefield(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogAccountOnlineTime(DWORD pDBMsg, DWORD dwDummy);

	DWORD LogFund(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogMaterial(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogReputation(DWORD pDBMsg, DWORD dwDummy);

	DWORD LogGMCmd(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogPet(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogFairyBirth(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogRoleOnlineInfo(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogWorldData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LogRolelogData(DWORD pDBMsg, DWORD dwDummy);

	// log 实际处理
	DWORD _LogSilver(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogYuanBao(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogExVolume(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogVIPPoint(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogTimeStat(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMallSell(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMallSellPack(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogItemTimes(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogGodBid(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogBattlefield(DWORD pDBMsg, DWORD dwDummy);
	// log 实际处理

	DWORD _LogFund(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogMaterial(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogReputation(DWORD pDBMsg, DWORD dwDummy);

	DWORD _LogGMCmd(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogPet(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogFairyBirth(DWORD pDBMsg, DWORD dwDummy);
	DWORD _LogRoleOnline(DWORD pDBMsg, DWORD dwDummy);

	//external links from table
	DWORD LoadExtLinks(DWORD pDBMsg, DWORD dwDummy);
	//离线挂机
	DWORD OfflineExpReward(DWORD pDBMsg,DWORD dwDummy);
	// Jason 2010-1-27 v1.3.2 离线奖励/玩家回归
	DWORD LoadAccountInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaceAccountInfo(DWORD pDBMsg,DWORD );


	//仙界修行
	DWORD SaveMiraclePrcNum(DWORD pDBMsg,DWORD dwDummy);

	DWORD SaveItemLockInfo(DWORD pDBMsg, DWORD);

	DWORD SaveItemBindInfo(DWORD pDBMsg, DWORD);
	//快捷栏保存读取
	DWORD SaveShortCut_QuickBar(DWORD pDBMsg, DWORD);
	DWORD LoadShortCut_QuickBar(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveRoleSpeak(DWORD pDBMsg, DWORD);

	// 保存已经开启的试炼状态
	DWORD SaveOpenTrainState(DWORD pDBMsg, DWORD dwDummy);

	DWORD CheckName(DWORD pDBMsg, DWORD);	

	DWORD SaveAvatarFaceAndHair(DWORD pDBMsg, DWORD);	

	// 公会战争
	DWORD ClearExpireGuildWarDeclare(DWORD pDBMsg, DWORD);	
	DWORD InsertGuildWarDeclare(DWORD pDBMsg, DWORD);	
	DWORD SetGuildWarDeclareEnded(DWORD pDBMsg, DWORD);	
	DWORD LoadGuildWarDeclare(DWORD pDBMsg, DWORD);	

	// 战场
	DWORD LoadBattleFieldAttackSignUpInfo(DWORD pDBMsg, DWORD);	
	DWORD LoadAllicanceGuildInfo(DWORD pDBMsg, DWORD);	
	DWORD InsertIntoAttackSignUpList(DWORD pDBMsg, DWORD);	
	DWORD DeleteFromAttackSignUpList(DWORD pDBMsg, DWORD);	
	DWORD ClearAttackSignUpList(DWORD pDBMsg, DWORD);
	DWORD ModifyBidFund(DWORD pDBMsg, DWORD);	
	DWORD InsertIntoAllianceList(DWORD pDBMsg, DWORD);
	DWORD ClearAllianceList(DWORD pDBMsg, DWORD);
	DWORD ConfirmAlliance(DWORD pDBMsg, DWORD);


	// 游戏世界各个地图人在线人数统计
	DWORD WorldMapOnlineInfo(DWORD pDBMsg, DWORD dwDummy);

	DWORD SaveOnlineTimeEachMonth(DWORD pDBMsg, DWORD dwDummy);
	DWORD QuestBoardSaveAllQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD QuestBoardSaveOneQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD SyncCanRankFlag(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveMediaOnlineNum(DWORD pDBMsg, DWORD dwDummy);

	// 更新妖精心情
	DWORD UpdateFabaoMood(DWORD pDBMsg,DWORD dwDummy);
	// 妖精元素伤害类型变化
	DWORD	UpdateFabaoEleInjuryType(DWORD pDBMsg,DWORD dwDummy);
	// 保存妖精重生数据
	DWORD UpdateFabaoNirVanaData(DWORD pMsg,DWORD);
	// 获取妖精重生数据
	DWORD GetFabaoNirVanaData(DWORD,DWORD);

	// 神之竞标
	DWORD LoadGodBidInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD SaveOneGodBidInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD ClearGodBidInfo(DWORD pDBMsg,DWORD dwDummy);

	DWORD UpdateFabaoBirthTimes(DWORD pDBMsg,DWORD dwDummy);

	// 砸金蛋
	DWORD SaveEggInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD ClearEggInfo(DWORD pDBMsg,DWORD dwDummy);

	// 衣橱
	DWORD SaveWardrobeLayerInfo(DWORD pDBMsg,DWORD dwDummy);

	DWORD DelRoleOneBuff(DWORD pDBMsg, DWORD dwDummy);
	DWORD InsertBuffDir(DWORD pDBMsg,DWORD);


	//家族妖精
	DWORD LoadFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	//DWORD LoadFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteALLFamilySpriteRequirement(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllFamilySprite(DWORD pDBMsg, DWORD dwDummy);
	DWORD CreateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);
	DWORD UpdateRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteRoleSpriteData(DWORD pDBMsg, DWORD dwDummy);

	// 家族任务
	DWORD RenewFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD ChangeFamilyQuestIssueState(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	//DWORD FamilyMemberRewarded(DWORD pDBMsg, DWORD dwDummy);

	// 家族相关
	DWORD CreateFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD DismissFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadAllFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadFamilyMember(DWORD pDBMsg, DWORD dwDummy);
	DWORD FamilyInitOK(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveAllFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyAtt(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveFamilyActive(DWORD pDBMsg, DWORD dwDummy);
	DWORD JoinFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD LeaveFamily(DWORD pDBMsg, DWORD dwDummy);
	DWORD SetLeader(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveRoleFamilyQuestRewarded(DWORD pDBMsg, DWORD dwDummy);
	DWORD RequestFamilyQuest(DWORD pDBMsg, DWORD dwDummy);
	DWORD DeleteFamilyQuest(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadWorldData(DWORD pDBMsg, DWORD dwDummy);
	DWORD LoadRolelogData(DWORD pDBMsg, DWORD dwDummy);

	DWORD LoadTigerProfit(DWORD pDBMsg, DWORD dwDummy);
	DWORD SaveTigerProfit(DWORD pDBMsg, DWORD dwDummy);

	//元神
	DWORD InsertHolySoulInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulInfo(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulExpAtt(DWORD pDBMsg,DWORD dwDummy);
	DWORD UpdateHolySoulAttInfo(DWORD pDBMsg,DWORD dwDummy);
	//元神装备强化
	DWORD LogHolyEquipCon(DWORD pDBMsg, DWORD dwDummy);	
	// 角色PK杀人记Log
	DWORD LogKillrole(DWORD pDBMsg, DWORD dwDummy);	

private: // 游戏服务器登陆/登出回调函数
	DWORD LoginCallBack(LPBYTE pByte, DWORD dwSize);
	DWORD LogoutCallBack(DWORD dwDummy);

private:
	// 读取文件, 初始化成员
	BOOL InitConfig();	

	// 注册需要数据库操作的命令
	VOID RegisterDBCmd();

	// 创建log表 -- 需同步处理，以免出现表还未创建，就向其中插入数据的情况产生
	VOID CreateCurDataLogTable();

	// 初始化游戏世界
	DWORD InitWorld(LPCTSTR szWorldName,BOOL bReload=FALSE);

	// 关闭游戏世界
	VOID TermWorld();

	VOID RegisterTestCmd(TObjRef<Console> pConsole);
	// 立即排行
	VOID RegisterRankingIns(TObjRef<Console> pConsole);
private: // 工作线程
	// 心跳
	DWORD ThreadBroadcast(LPVOID);

	// 排行榜
	DWORD ThreadRankings(LPVOID);

	// 获取游戏服务器消息
	DWORD ThreadReceiveMsg(LPVOID);

	// 处理服务器消息
	DWORD ThreadProcessMsg(LPVOID);

private: // 压测专用
	DWORD TestLoadRole(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestLoadRoleItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestSaveItem(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestSaveEquip(DWORD pDBMsg, DWORD dwDummy);
	DWORD TestDBReadFunction();
	DWORD RankingNow();
private:
	TSFPTrunk<CLoong>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<StreamServer>	m_pServer;				// 与游戏服务器的连接
	TObjRef<Thread>			m_pThread;				// 线程管理器

	volatile BOOL			m_bTerminate;			// 是否结束
	volatile BOOL			m_bSuspendFlag;		// 让线程全部等待的信号
	volatile LONG			m_lThreadNum;
	enum {EThreadRecv=0,EThreadProc,EThreadBroadcast,ETheadAll};
	server_def::Event 		m_ProcThreadSuspend[ETheadAll];	// 让处理、接收、广播线程等待，排行另有db链接，不用考虑
	server_def::Event		m_eventSuspended;
	VOID 					SuspendAllWorkingThread();
	VOID					ResumeAllWorkingThread();
	BOOL					WaitForWorkingThreadSuspend(INT nTimeout);
	VOID					NotifySuspended();

	volatile BOOL			m_bGameServerLogout;				// 当前游戏世界是否关闭
	volatile BOOL			m_bGameServerConnect;				// 当前是否有游戏世界连接

	TCHAR					m_szWorldName[X_SHORT_NAME];		// 游戏世界名
	//TCHAR					m_szServerName[X_LONG_STRING*2];	// 游戏服务器机器名称
	INT						m_nPort;							// 本服务器端口号
	DWORD					m_dwDBGoldenCode;					// 本服务器的金色代码
	DWORD					m_dwGameGoldenCode;					// 游戏服务器的金色代码

	CLoongDB*				m_pDB;					// 数据库记录操作类
	CLoongRank*				m_pDBRank;				// 排行榜数据库操作类
	BYTE					m_byRankingStartTime;	// 排行榜排行时间
	tagDWORDTime			m_dwLogTime;			// log用时间
	tagDWORDTime			m_dwLastRanking;		// 记录排行榜更新的最近时间
	CHAR					m_szLogFilePostfix[16];	// log用数据库表后缀

	DBMsgQueue				m_MsgQue;				// 待处理消息队列
	NetCmdMgr				m_MsgCmdMgr;			// 游戏服务器与本服务器的消息函数映射	

	CHAR					m_pBuffer[X_ROLE_BUFF_SIZE];	// 用于给游戏服务器返回消息

	tagNDBS_LoadSimRole*	m_pLoadSimRole;			// 反馈某个帐号下的基本角色信息,不要对该属性使用ZeroMemory
	tagNDBS_LoadRole*		m_pLoadRole;			// 反馈某个角色的具体数据，不要对该属性使用ZeroMemory
	tagNDBS_CreateRole*		m_pCreateRole;			// 反馈新创建的角色的基本信息
	tagNDBS_LoadBaiBao*		m_pLoadBaiBao;			// 反馈读取的百宝袋物品信息
	tagNDBS_LoadBaiBaoLog*	m_pLoadBaiBaoLog;		// 反馈读取的百宝袋物品Log信息
	
	CResGuard				m_Lock;

	tagDWORDTime			m_dwClergyRankTime;		// 最近一次尽享神职竞选的时间

private: // UI相关
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindow;
	TObjRef<GUISystem>		m_pGUI;
	IGUIRenderNative		m_Render;
	TObjRef<Console>		m_pConsole;

public: // 数据统计相关
	struct tagStatus
	{
		// 系统
		INT					nCpuNum;
		DWORD				dwTotalPhys;
		DWORD				dwAvailPhys;
		INT					nQuotaNonPagedPoolUsage;

		// 服务器状态
		volatile DWORD		dwHeartbeat;			// 心跳
		volatile DWORD		dwRecvMsgNum;			// 接收到的消息总数
		volatile DWORD		dwProcMsgNum;			// 处理的消息总数
		volatile DWORD		dwSendMsgNum;			// 已发送的消息总数

		DWORD				dwRecvMsgSz;			// 网络层接受到的总的消息大小
		DWORD				dwSendMsgSz;			// 网络层发送的总的消息大小

		volatile DWORD		dwUnRecvMsgNum;			// 网络层已接受，但上层未收取的消息数
		volatile DWORD		dwUnSendMsgNum;			// 上层已发送，但网络层未发出的消息数

		DWORD				dwReadNum;				// 读DB次数
		DWORD				dwWriteNum;				// 写DB次数

		DWORD				dwLastRecvNum;			// 最近一次收到的总消息数
		DWORD				dwLastProcNum;			// 最近一次处理的总消息数
		DWORD				dwLastSendNum;			// 最近一次发送的总消息数

		DWORD				dwLastRecvSz;			// 最近一次收到的消息大小
		DWORD				dwLastSendSz;			// 最近一次发送的消息大小
		
		DWORD				dwLastReadNum;			// 最近一次读DB次数
		DWORD				dwLastWriteNum;			// 最近一次写DB次数

		DWORD				dwTickRecvNum;			// 当前刷新间隔内收到的总消息数
		DWORD				dwTickProcNum;			// 当前刷新间隔内处理的总消息数
		DWORD				dwTickSendNum;			// 当前刷新间隔内发送的总消息数

		DWORD				dwTickRecvSz;			// 当前刷新间隔内收到的消息大小
		DWORD				dwTickSendSz;			// 当前刷新间隔内发送的消息大小

		DWORD				dwTickReadNum;			// 当前刷新间隔内读DB次数
		DWORD				dwTickWriteNum;			// 当前刷新间隔内写DB次数

		DWORD				dwLastTick;				// 最近一次刷新时间
		DWORD				dwInterval;				// 时间间隔(毫秒)

		DWORD				dwMaxSendedMsgSize;		// 发送的最大消息大小
		DWORD				dwMaxSendedMsgID;			// 发送的最大消息id

		// 版本相关
		DWORD				dwVerLoongDB;
		DWORD				dwVerBeton;
		DWORD				dwVerVEngine;
		DWORD				dwVerServerDefine;
		DWORD				dwVerXML;
		
		tagStatus()
		{
			Init();
		}
		
		VOID Init()
		{
			ZeroMemory(this, sizeof(*this));
			nCpuNum		= TObjRef<SysInfo>()->GetProcessorNum();
			dwLastTick	= GetTickCount();
		}
	};
	// 得到服务器状态信息
	const tagStatus* GetStatus();

private:
	tagStatus				m_Status;
	// Jason 2010-11-29
	volatile BOOL			m_bRankingNow;			// 是否立即排行
	volatile LONG			m_lIniTermWorldCounter;	// init和term world的技术器，防止不配对初始化和终止
	BOOL		m_bInited;
private:
	DWORD OnNDBC_KeyCodeAddGift(DWORD pDBMsg, DWORD dwDummy);
	DWORD OnNDBC_ReceiveYuanBao(DWORD pDBMsg, DWORD dwDummy);
};

