#pragma once
#include "Loong_CommonDef.h"
class CLoongMergeLogicCtrl
{
public:
	CLoongMergeLogicCtrl();
	~CLoongMergeLogicCtrl(void);

	//各种set get
public:
	//合并类型
	VOID SetMergeType(EDBMergeType eMergeType)		{ m_eMergeType = eMergeType;}

	//是否进行数据清理
	VOID SetIfCleanData(BOOL bDataClean)		{ m_bDataClean = bDataClean; }

	// 生存时间
	VOID SetSurviveDay(INT nSurviveDay)		{ m_nItemDelEquipDelSurviveDay = nSurviveDay; }

	//源2的游戏世界名
	VOID SetSrc2WorldName(tstring strSrc2WorldName)	{ m_strSrc2WorldName = strSrc2WorldName;}

	//源1的account表最大num
	VOID SetSrc1LoginMaxNum(DWORD dwSrc1LoginMaxNum)		{ m_dwSrc1LoginMaxNum = dwSrc1LoginMaxNum;}

	//源1roledata表最大num
	VOID SetSrc1RoleMaxNum(DWORD dwSrc1RoleMaxNum)		{ m_dwSrc1RoleMaxNum = dwSrc1RoleMaxNum;}

	//源1petdata表最大num
	VOID SetSrc1PetMaxNum(DWORD dwSrc1PetMaxNum)		{ m_dwSrc1PetMaxNum = dwSrc1PetMaxNum;}

	//内部物品最大序号
	VOID SetSrc1ItemMaxSerialInternal(INT64 n64ItemMaxSerialInternal)
	{
		m_n64Src1ItemMaxSerialInternal	=	n64ItemMaxSerialInternal;
		m_n64ItemCurSerialInternal		=	n64ItemMaxSerialInternal;
	}

	//外部物品最小序号
	VOID SetSrc1ItemMinSerialOutter(INT64 n64ItemMinSerialOutter)	
	{
		m_n64Src1ItemMinSerialOutter	=	n64ItemMinSerialOutter;
		m_n64ItemCurSerialOutter		=	n64ItemMinSerialOutter;
	}

	//划拨物品最小序号
	VOID SetSrc1ItemMinSerialBill(INT64 n64ItemMinSerialBill)
	{
		m_n64Src1ItemMinSerialBill	= n64ItemMinSerialBill;
		m_n64ItemCurSerialBill		= n64ItemMinSerialBill;
	}

	//得到所有源1的 旧id
	const std::vector <DWORD> &GetAllOldSrc1AccountID()		{ return m_vecOldSrc1AccountID; }

	//源1有，源2也有的用户。旧id
	const std::map <DWORD ,DWORD> &GetOldSrc1VsSrc2()		{ return m_mpOldSrc1VsSrc2; }
	//源2有，源1也有的用户。旧id
	const std::map <DWORD ,DWORD> &GetOldSrc2VsSrc1()		{ return m_mpOldSrc2VsSrc1; }

	//源2独有的用户集合。 旧id
	const std::vector <DWORD> &GetOldSrc2OnlyAccountID()		{ return m_vecOldSrc2OnlyAccountID; }

	//得到所有帮派id
	const std::set <DWORD> &GetAllOldGuildID()			{ return m_setOldGuildID; }

	//得到玩家补偿
	const std::vector <tagRoleCompensation> &GetRoleCompensation() { return m_vecRoleCompensation; }

	//得到新的外部生产物品64位id
	INT64 GetNewItemSerialOutter()	{ return --m_n64ItemCurSerialOutter; }

	//得到充值账号
	std::set<string>& GetCANameSet() { return m_setCAName; }

	//得到源1角色名
	std::set<tstring>& GetSrc1RoleName() { return m_setSrc1RoleName; }

	//得到源1角色的crc
	std::set<DWORD>& GetSrc1RoleNameCrc() { return m_setSrc1RoleNameCrc; }

	//得到源1神职角色ID
	std::set<DWORD>& GetSrc1ClergyRoleID() { return m_setSrc1ClergyRoleID; }
	//得到源2神职角色ID 
	std::set<DWORD>& GetSrc2ClergyRoleID() { return m_setSrc2ClergyRoleID; }

public:
	//login 如果跨大区 先调这个方法
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_Account*> &vecSrc1,
						std::vector <tagTWrap_Longin_Account*> &vecSrc2,
						std::vector <tagTWrap_Longin_Account*> &vecTar);
	//login worldforbid表
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc1,
						std::vector <tagTWrap_Longin_WorldForbid*> &vecSrc2,
						std::vector <tagTWrap_Longin_WorldForbid*> &vecTar);
	//login 黑名单
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_BlackList*> &vecSrc1,
						std::vector <tagTWrap_Longin_BlackList*> &vecSrc2,
						std::vector <tagTWrap_Longin_BlackList*> &vecTar);
	//login 防沉迷
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc1,
						std::vector <tagTWrap_Longin_FatigueTime*> &vecSrc2,
						std::vector <tagTWrap_Longin_FatigueTime*> &vecTar);
	//login 全局角色名字
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_RoleName*> &vecSrc1,
						std::vector <tagTWrap_Longin_RoleName*> &vecSrc2,
						std::vector <tagTWrap_Longin_RoleName*> &vecTar);
	//login 全局帮派名字
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_GuildName*> &vecSrc1,
						std::vector <tagTWrap_Longin_GuildName*> &vecSrc2,
						std::vector <tagTWrap_Longin_GuildName*> &vecTar);

	//login 全局家族名
	BOOL Loong_LoginMerge(std::vector <tagTWrap_Longin_FamilyName*> &vecSrc1,
						std::vector <tagTWrap_Longin_FamilyName*> &vecSrc2,
						std::vector <tagTWrap_Longin_FamilyName*> &vecTar);
	
public:
	//loong 如果没有跨大区 先调这个方法
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc1,
						std::vector <tagTWrap_Loong_AccountCommon*> &vecSrc2,
						std::vector <tagTWrap_Loong_AccountCommon*> &vecTar);

	//role data
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleData*> &vecSrc1,
						std::vector <tagTWrap_Loong_RoleData*> &vecSrc2,
						std::vector <tagTWrap_Loong_RoleData*> &vecTar);

	//帮派
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Guild*> &vecSrc1,
						std::vector <tagTWrap_Loong_Guild*> &vecSrc2,
						std::vector <tagTWrap_Loong_Guild*> &vecTar);

	//item 内部调下面三个方法
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
						std::vector <tagTWrap_Loong_Item*> &vecSrc2,
						std::vector <tagTWrap_Loong_Item*> &vecTar);
	//item 实际处理方法
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Item*> &vecSrc1,
						std::vector <tagTWrap_Loong_Item*> &vecSrc2,
						std::vector <tagTWrap_Loong_Item*> &vecTar,
						ETBItemType emTBType);
	//itembaibao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemBaiBao*> &vecTar);
	//itemdel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemDel*> &vecTar);
	//item_cdtime
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemCDTime*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemCDTime*> &vecTar);
	//buff
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Buff*> &vecSrc1,
						std::vector <tagTWrap_Loong_Buff*> &vecSrc2,
						std::vector <tagTWrap_Loong_Buff*> &vecTar);

	//BlackList
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BlackList*> &vecSrc1,
						std::vector <tagTWrap_Loong_BlackList*> &vecSrc2,
						std::vector <tagTWrap_Loong_BlackList*> &vecTar);

	//enemy
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Enemy*> &vecSrc1,
						std::vector <tagTWrap_Loong_Enemy*> &vecSrc2,
						std::vector <tagTWrap_Loong_Enemy*> &vecTar);

	//equip 实际处理方法
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
						std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
						std::vector <tagTWrap_Loong_Equip*> &vecTar,
						ETBEquipType emTBType);

	//equip
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Equip*> &vecSrc1,
						std::vector <tagTWrap_Loong_Equip*> &vecSrc2,
						std::vector <tagTWrap_Loong_Equip*> &vecTar);

	//EquipDel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_EquipDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_EquipDel*> &vecTar);

	//EquipBaiBao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_EquipBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_EquipBaiBao*> &vecTar);

	//氏族
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ClanData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ClanData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ClanData*> &vecTar);

	//名人堂
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActClanTreasure*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActClanTreasure*> &vecTar);
	
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc1,
						std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecSrc2,
						std::vector <tagTWrap_Loong_FameHallEnterSnap*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc1,
						std::vector <tagTWrap_Loong_RepRstTime*> &vecSrc2,
						std::vector <tagTWrap_Loong_RepRstTime*> &vecTar);

	//friend
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Friend*> &vecSrc1,
						std::vector <tagTWrap_Loong_Friend*> &vecSrc2,
						std::vector <tagTWrap_Loong_Friend*> &vecTar);

	//friendship
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc1,
						std::vector <tagTWrap_Loong_FriendshipSave*> &vecSrc2,
						std::vector <tagTWrap_Loong_FriendshipSave*> &vecTar);

	//GroupPurchase
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc1,
						std::vector <tagTWrap_Loong_GroupPurchase*> &vecSrc2,
						std::vector <tagTWrap_Loong_GroupPurchase*> &vecTar);

	//GuildMem
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildMem*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildMem*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildMem*> &vecTar);

	//item_needlog
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc1,
						std::vector <tagTWrap_Loong_ItemNeedLog*> &vecSrc2,
						std::vector <tagTWrap_Loong_ItemNeedLog*> &vecTar);

	//log_baibao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_LogBaiBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_LogBaiBao*> &vecTar);
	//RoleDel
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_RoleDel*> &vecSrc1,
						std::vector <tagTWrap_Loong_RoleDel*> &vecSrc2,
						std::vector <tagTWrap_Loong_RoleDel*> &vecTar);

	//pet_data
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_PetData*> &vecSrc1,
						std::vector <tagTWrap_Loong_PetData*> &vecSrc2,
						std::vector <tagTWrap_Loong_PetData*> &vecTar);
	//pet skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_PetSkill*> &vecSrc1,
						std::vector <tagTWrap_Loong_PetSkill*> &vecSrc2,
						std::vector <tagTWrap_Loong_PetSkill*> &vecTar);
	//skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Skill*> &vecSrc1,
						std::vector <tagTWrap_Loong_Skill*> &vecSrc2,
						std::vector <tagTWrap_Loong_Skill*> &vecTar);
	//task
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_Task*> &vecSrc1,
						std::vector <tagTWrap_Loong_Task*> &vecSrc2,
						std::vector <tagTWrap_Loong_Task*> &vecTar);
	//taskdone
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_TaskDone*> &vecSrc1,
						std::vector <tagTWrap_Loong_TaskDone*> &vecSrc2,
						std::vector <tagTWrap_Loong_TaskDone*> &vecTar);

	//title
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Title*> &vecSrc1,
						std::vector <tagTWrap_Loong_Title*> &vecSrc2,
						std::vector <tagTWrap_Loong_Title*> &vecTar);

	//vip_stall
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_VipStall*> &vecSrc1,
						std::vector <tagTWrap_Loong_VipStall*> &vecSrc2,
						std::vector <tagTWrap_Loong_VipStall*> &vecTar);

	//VisitingCard
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc1,
						std::vector <tagTWrap_Loong_VisitingCard*> &vecSrc2,
						std::vector <tagTWrap_Loong_VisitingCard*> &vecTar);

	//ybaccount
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_YBAccount*> &vecSrc1,
						std::vector <tagTWrap_Loong_YBAccount*> &vecSrc2,
						std::vector <tagTWrap_Loong_YBAccount*> &vecTar);

	//yuanbaoorder
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc1,
						std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecSrc2,
						std::vector <tagTWrap_Loong_YuanBaoOrder*> &vecTar);

	//bill item
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillItem*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillItem*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillItem*> &vecTar);
	//bill yuanbao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillYuanBao*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillYuanBao*> &vecTar);
	//bill yuanbao log
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc1,
						std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecSrc2,
						std::vector <tagTWrap_Loong_BillYuanBaoLog*> &vecTar);
	//guild_commodity
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildCommodity*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildCommodity*> &vecTar);
	//guild_skill
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildSkill*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildSkill*> &vecTar);
	//vip_netbar
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc1,
						std::vector <tagTWrap_Loong_VipNetBar*> &vecSrc2,
						std::vector <tagTWrap_Loong_VipNetBar*> &vecTar);
	//guild_upgrade
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc1,
						std::vector <tagTWrap_Loong_GuildFacilities*> &vecSrc2,
						std::vector <tagTWrap_Loong_GuildFacilities*> &vecTar);
	//commerce_rank
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc1,
						std::vector <tagTWrap_Loong_CommerceRank*> &vecSrc2,
						std::vector <tagTWrap_Loong_CommerceRank*> &vecTar);
	//activity
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActivityData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActivityData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActivityData*> &vecTar);
	//left msg
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc1,
						std::vector <tagTWrap_Loong_LeftMsg*> &vecSrc2,
						std::vector <tagTWrap_Loong_LeftMsg*> &vecTar);

	//外部链接
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc1,
						std::vector <tagTWrap_Loong_ExternalLink*> &vecSrc2,
						std::vector <tagTWrap_Loong_ExternalLink*> &vecTar);

	//name history
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_NameHistory*> &vecSrc1,
						std::vector <tagTWrap_Loong_NameHistory*> &vecSrc2,
						std::vector <tagTWrap_Loong_NameHistory*> &vecTar);

	//city
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_City*> &vecSrc1,
						std::vector <tagTWrap_Loong_City*> &vecSrc2,
						std::vector <tagTWrap_Loong_City*> &vecTar);

	//Master
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Master*> &vecSrc1,
						std::vector <tagTWrap_Loong_Master*> &vecSrc2,
						std::vector <tagTWrap_Loong_Master*> &vecTar);

	//short cut
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc1,
						std::vector <tagTWrap_Loong_ShortCutBar*> &vecSrc2,
						std::vector <tagTWrap_Loong_ShortCutBar*> &vecTar);

	//试炼状态
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_TrainState*> &vecSrc1,
						std::vector <tagTWrap_Loong_TrainState*> &vecSrc2,
						std::vector <tagTWrap_Loong_TrainState*> &vecTar);

	//ActiveRankData
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc1,
						std::vector <tagTWrap_Loong_ActiveRankData*> &vecSrc2,
						std::vector <tagTWrap_Loong_ActiveRankData*> &vecTar);

	//fabao
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Fabao*> &vecSrc1,
						std::vector <tagTWrap_Loong_Fabao*> &vecSrc2,
						std::vector <tagTWrap_Loong_Fabao*> &vecTar);

	// F-Plan 1.5.0 SoulCrystal
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc1,
		std::vector <tagTWrap_Loong_SoulCrystal*> &vecSrc2,
		std::vector <tagTWrap_Loong_SoulCrystal*> &vecTar);

	// F-Plan 1.5.0 account_reactive, account_reactiver
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc1,
		std::vector <tagTWrap_Loong_AccountReactive*> &vecSrc2,
		std::vector <tagTWrap_Loong_AccountReactive*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc1,
		std::vector <tagTWrap_Loong_AccountReactiver*> &vecSrc2,
		std::vector <tagTWrap_Loong_AccountReactiver*> &vecTar);

	//ForceBreakOutApprentice
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc1,
						std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecSrc2,
						std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecTar);

	//任务板
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc1,
						std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecSrc2,
						std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecTar);
	
	//帮派宣战
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc1,
						std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecSrc2,
						std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecTar);

	//角色在线时间统计
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc1,
						std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecSrc2,
						std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecTar);

	//妖精重生数据
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc1,
						std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecSrc2,
						std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecTar);

	//神之竞标
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_GodBid*> &vecSrc1,
						std::vector <tagTWrap_Sheng_GodBid*> &vecSrc2,
						std::vector <tagTWrap_Sheng_GodBid*> &vecTar);

	//砸蛋
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc1,
						std::vector <tagTWrap_Sheng_EggInfo*> &vecSrc2,
						std::vector <tagTWrap_Sheng_EggInfo*> &vecTar);

	//衣橱
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc1,
						std::vector <tagTWrap_Sheng_Wardrobe*> &vecSrc2,
						std::vector <tagTWrap_Sheng_Wardrobe*> &vecTar);

	//领域
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_Area*> &vecSrc1,
		std::vector <tagTWrap_Sheng_Area*> &vecSrc2,
		std::vector <tagTWrap_Sheng_Area*> &vecTar);

	//稀有领域
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Sheng_RareArea*> &vecSrc1,
		std::vector <tagTWrap_Sheng_RareArea*> &vecSrc2,
		std::vector <tagTWrap_Sheng_RareArea*> &vecTar);

	//Family
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_Family*> &vecSrc1,
												 std::vector <tagTWrap_Loong_Family*> &vecSrc2,
												 std::vector <tagTWrap_Loong_Family*> &vecTar);
	//FamilyMem
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyMem*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyMem*> &vecTar);
	//FamilyQuset
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyQuset*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyQuset*> &vecTar);
	//FamilyRoleSprite
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecTar);
	//FamilySprite
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc1,
												 std::vector <tagTWrap_Loong_FamilySprite*> &vecSrc2,
												 std::vector <tagTWrap_Loong_FamilySprite*> &vecTar);
	//SpriteRequirement
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc1,
												 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecSrc2,
												 std::vector <tagTWrap_Loong_SpriteRequirement*> &vecTar);

	//GmServerData
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_GmServerData*> &vecSrc1,
												 std::vector <tagTWrap_Loong_GmServerData*> &vecSrc2,
												 std::vector <tagTWrap_Loong_GmServerData*> &vecTar);


	//UseKeyCode_roleID
	BOOL  Loong_LoongMerge(std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc1,
		std::vector <tagTWrap_Loong_UseKeyCode*> &vecSrc2,
		std::vector <tagTWrap_Loong_UseKeyCode*> &vecTar);

	// F-Plan 1.6.0 holy
	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_Holy*> &vecSrc1,
		std::vector <tagTWrap_Loong_Holy*> &vecSrc2,
		std::vector <tagTWrap_Loong_Holy*> &vecTar);

	BOOL Loong_LoongMerge(std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc1,
		std::vector <tagTWrap_Loong_HolyEquip*> &vecSrc2,
		std::vector <tagTWrap_Loong_HolyEquip*> &vecTar);


public:
	//干掉所有中间数据 重新初始化变量
	VOID ClearMemory();

private:
	
	// 脚本数据特殊更新
	VOID UpdateSpecialActivityData(DWORD dwActID, tagActivityData& stAData);

	// 移动活动数据
	VOID MoveActivityData(DWORD dwActID, std::vector<tagTWrap_Loong_ActivityData*>::iterator itSrc,
		std::vector<tagTWrap_Loong_ActivityData*>::iterator itTar, INT nValidPos, BOOL bSameContainer);

	//过滤活动数据
	VOID FilterActivityData(std::vector <tagTWrap_Loong_ActivityData*> &vecData);

	//更新roleid 供源2使用
	VOID Update_Loong_RoleID(DWORD &dwRoleID);

	//更新物品64位id
	VOID Update_Loong_Item64SerialNum(INT64 &n64Serial);

	//转换成小写后的crc32
	DWORD LowerCrc32(LPCTSTR str,DWORD strLen);

	//玩家数据过滤
	VOID Loong_RoleDataFilter(std::vector<tagTWrap_Loong_RoleData*> & vecTar);

	//清理roledata表
	ETBRoleRemoveType Loong_RoleDataDoHouseKeeping(std::vector <tagTWrap_Loong_RoleData*>::iterator itTar,
												tagDWORDTime dwCurTm,
												BOOL bFirstRole);

	//检查该role是否为脏数据
	BOOL IsDirtyRoleData(DWORD dwRoleID);

public:
	std::vector <tagRoleCompensation> m_vecRoleCompensation; //玩家补偿

	std::set <string> m_setCAName; //曾经充值过的账号

	std::set <tstring> m_setSrc1RoleName;		//源1的角色名字

	std::set<DWORD> m_setSrc1RoleNameCrc;		//源1角色名字的crc

	std::set<DWORD> m_setSrc1ClergyRoleID;		//源1神职角色ID
	std::set<DWORD> m_setSrc2ClergyRoleID;		//源2神职角色ID

//合服用对照表
private:
	//std::map<tagItemNumKey ,INT64> m_mpOldItemNumvsNewItemNum;//新旧item序号对照表

	std::map <tagAccountIDKey,DWORD> m_mpOldAccountVsNewAccount; //跨大区合并需要 旧的accountid与新的accountid的对照表

	std::map <tagGuildIDKey,DWORD> m_mpOldGuildIDvsNewGuildID; //帮派id

	std::map <tagFamilyIDKey,DWORD> m_mpOldFamilyIDvsNewFamilyID; //家族id(目前先不考虑有重复crc的情况)

	std::map <DWORD ,DWORD> m_mpAccountIDvsWareSize;//新的accountid和仓库size的对照表

	stdext::hash_map <DWORD ,ETBRoleRemoveType> m_mpRoleIDvsRemoveType; //新的roleid和其remove类型的对照表

	std::set <DWORD> m_setOldGuildID; //所有老的帮派id

	std::map <DWORD ,DWORD> m_mpAccountIDvsRoleIDforCB; //用于处理“合服时被删除角色”角色仓库中绑定物品 改绑定(change bind)使用

	//m_vecSrc1AccountID、m_mpSrc1VsSrc2、m_vecSrc2OnlyAccountID 是用来进行account级转换的对照表
	std::vector <DWORD> m_vecOldSrc1AccountID; //源1的用户集合。 旧id

	std::map <DWORD ,DWORD> m_mpOldSrc1VsSrc2; //源1有，源2也有的用户。旧id
											//key:代表源1的AccountID。value:代表与源1对应的同一用户,在源2中的AccountID。
	std::map <DWORD ,DWORD> m_mpOldSrc2VsSrc1; 	//key:代表源2的AccountID。value:代表与源2对应的同一用户,在源1中的AccountID。
			
	std::vector <DWORD> m_vecOldSrc2OnlyAccountID; //源2独有的用户集合。 旧id

	std::set <DWORD> m_setChangeNameRoleID; //改名角色 的roleid 存更新后的

//合服用中间数据
private:

	std::vector <tagTWrap_Loong_Item> m_vecOverItem; //超出物品 最终写入item_baibao

	std::set <INT64> m_setOverEquipSerialNum;	//超出武器的序号

	std::vector <tagTWrap_Loong_Equip> m_vecOverEquip; //超出装备 最终写入equip_baibao

	std::set <DWORD> m_setCAccountID; //曾经充值过的账号id

	std::set <DWORD> m_mapGuildLeader; //所有帮主 新的roleid

	stdext::hash_set <INT64> m_setDirtyItem;	//脏的物品

	stdext::hash_set <INT64> m_setItemDelEquipDelSurvive;	//干净的itemdel equipdel
private:

	INT		m_nItemDelEquipDelSurviveDay; //保留多少天物品删除备份和装备删除备份

	BOOL	m_bDataClean; //是否进行数据清理

	EDBMergeType m_eMergeType;

	tstring		m_strSrc2WorldName; //源2世界名

	INT64		m_n64Src1ItemMaxSerialInternal;		//源1 内部生成物品		这个号从500000000000往上加 不包括500000000000
	INT64		m_n64Src1ItemMinSerialOutter;		//源1 外部生成物品		这个号从400000000000往下减 包括400000000000
	INT64		m_n64Src1ItemMinSerialBill;			//源1 直充生成物品最小序号	这个号从300000000000往下减 不包括300000000000

	INT64		m_n64ItemCurSerialInternal;			//当前内部生成物品		这个号从500000000000往上加 不包括500000000000
	INT64		m_n64ItemCurSerialOutter;			//当前外部生成物品		这个号从400000000000往下减 包括400000000000
	INT64		m_n64ItemCurSerialBill;				//当前直充生成物品最小序号	这个号从300000000000往下减 不包括300000000000

	DWORD		m_dwSrc1LoginMaxNum;				//src1 login account表 id最大数

	DWORD		m_dwSrc1RoleMaxNum;					//源1玩家最大数 用于roleid追加

	DWORD		m_dwSrc1PetMaxNum;					//源1 宠物最大petid			这个号从3000000001往上加　不包括3000000001

//	DWORD		m_dwSrc1VipStallMaxNum;				//源1 vip摊位最大数

	DWORD		m_dwSrc1WorldNameCrc;				//源1 游戏世界名的crc

	CRITICAL_SECTION		m_ContainerLock;					//多线程合并时操作容器时需要加锁

private:
	vector<tagFamilyNeedChangeName*>	m_vecNeedChangeFamilyName;

public:
	vector<tagFamilyNeedChangeName*>* GetNeedChangNameFamily(){ return &m_vecNeedChangeFamilyName; }

	VOID  Update_Loong_FamilyNameCRC(DWORD &dwFamilyID);
};


extern string Loong_CheckAccountName(CHAR szAccountName[X_SHORT_NAME]);
