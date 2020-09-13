#pragma once
#include "Singleton.h"
#include "Loong_CommonDef.h"



class CLoongLoongDBCtrl: public Singleton<CLoongLoongDBCtrl>
{
	typedef VOID (CLoongLoongDBCtrl::*Fun_pFormat)(Beton::MyStream*,EDBType);
	typedef BOOL (CLoongLoongDBCtrl::*Fun_pFormat3)(Beton::MyStream*,LPVOID&,EDBType);
	typedef BOOL (CLoongLoongDBCtrl::*Fun_pFormat4)(Beton::MyStream*,LPVOID&,LPCSTR,EDBType);
	
	//拼accountid形式的
	typedef VOID (CLoongLoongDBCtrl::*Fun_pAccountFormat)(Beton::MyStream*,DWORD,EDBType,ETBQueryExtraOption);

	//拼分页查询的
	typedef VOID (CLoongLoongDBCtrl::*Fun_pPagingFormat)(Beton::MyStream*,INT64,EDBType);



	typedef VOID (CLoongLoongDBCtrl::*Fun_pProcRes)(LPVOID&, Beton::QueryResult*);
private:
	//模版方法
	// 读取数据库信息(有结果集)
	BOOL Load(OUT LPVOID pData,EDBType eDBType,
		Fun_pFormat pFunFormat, Fun_pProcRes pFunProcRes);

	//按用户ID加载数据
	BOOL AccountLoad(OUT LPVOID pData,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption,
		Fun_pAccountFormat pFunFormat, Fun_pProcRes pFunProcRes);

	//分页load
	EPagingState PagingLoad(OUT LPVOID pData,INT64 &n64Limit,EDBType eDBType,
		Fun_pPagingFormat pFunFormat, Fun_pProcRes pFunProcRes);


	// 插入数据库操作模版方法
	BOOL Insert( LPVOID pData, EDBType eDBType,Fun_pFormat3 pFunFormat);
	BOOL Insert( LPVOID pData, EDBType eDBType,LPCSTR pszTable,Fun_pFormat4 pFunFormat);

	//特殊的删除操作，单写
	BOOL DeleteRarearea( EDBType eDBType );
	BOOL DaleteArea( EDBType eDBType );
	BOOL DeleteSkill( EDBType eDBType );
	BOOL DeleteBuff( EDBType eDBType );
	BOOL DeleteItem( EDBType eDBType );

public:
	CLoongLoongDBCtrl(void);
	~CLoongLoongDBCtrl(void){ Destroy(); }


	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(tagConfigDetail stConfig[EM_DB_TYPE_END]);
	VOID	Destroy();

private:

	DataBase		m_LoongDB[EM_DB_TYPE_END];	// Beton数据库

public:
	//得到roledata表最大序列号
	DWORD GetRoleDataMaxNum(EDBType eDBType);

	DWORD GetPetDataMaxNum(EDBType eDBType);

	//划拨物品最小序号
	INT64 GetItemMinSerialBill(EDBType eDBType);

	//内部物品最大序号
	INT64 GetMaxSerialInternal(EDBType eDBType);

	//外部物品最小序号
	INT64 GetMinSerialOutter(EDBType eDBType);

	//创建补偿物品
	BOOL CreateItem(INT64 n64SerialNum, DWORD ItemTypdID, DWORD AccountID, DWORD RoleID);

	//删除掉稀有领域相关数据
	BOOL DeleteRareareaData(EDBType eDBType);

private:
	BOOL GetMinItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial,EDBType eDBType, INT64 n64LowLimit = GT_INVALID);
	BOOL GetMaxItemSerialInTable(LPCSTR szTable, INT64& n64ItemSerial,EDBType eDBType,INT64 n64UpLimit = GT_INVALID);

public:
	//Item 按account读取
	BOOL Loong_AccountQuery(std::vector <tagTWrap_Loong_Item*>&, DWORD dwAccountID, EDBType eDBType, ETBQueryExtraOption eEOption = EM_TBQ_EOPTION_NULL);
	BOOL Loong_AccountQuery(std::vector <tagTWrap_Loong_RoleData*>&, DWORD dwAccountID, EDBType eDBType, ETBQueryExtraOption eEOption = EM_TBQ_EOPTION_NULL);


public:
	//equip paging query
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_Equip*>&,INT64,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_EquipDel*>&,INT64,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_TaskDone*>&,INT64,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_Task*>&,INT64,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_ItemDel*>&,INT64,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_Fabao*>&,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_VisitingCard*>&,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_BoardQuestSaveData*>&,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>&vecData,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_FabaoNirVanaData*>&vecData,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_GodBid*>&vecData,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_EggInfo*>&vecData,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_Area*>&vecData,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Sheng_RareArea*>&vecData,INT64 n64Limit,EDBType eDBType);
	// F-Plan 1.5.0
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_SoulCrystal*>&vecData,INT64 n64Limit,EDBType eDBType);
	// 分页查询 train_state
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_TrainState*>&,INT64 n64Limit,EDBType eDBType);
	// 分页查询 skill
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_Skill*>&,INT64 n64Limit,EDBType eDBType);
	// 分页查询 shortcut_quickbar
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_ShortCutBar*>&,INT64 n64Limit,EDBType eDBType);	

	// F-Plan 1.6.0
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_Holy*>&,INT64 n64Limit,EDBType eDBType);
	EPagingState Loong_PagingQuery(std::vector <tagTWrap_Loong_HolyEquip*>&,INT64 n64Limit,EDBType eDBType);
	

public:
/************************ 读取相关 **************************/
	BOOL Loong_Query(std::vector <tagTWrap_Loong_AccountCommon*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_BlackList*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_Buff*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_ClanData*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_Enemy*>&,EDBType eDBType);

//	BOOL Loong_Query(std::vector <tagTWrap_Loong_Equip*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_EquipBaiBao*>&,EDBType eDBType);

//	BOOL Loong_Query(std::vector <tagTWrap_Loong_EquipDel*>&,EDBType eDBType);

	//氏族珍宝激活表
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ActClanTreasure*>&,EDBType eDBType);

	//氏族进入快照
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FameHallEnterSnap*>&,EDBType eDBType);

	//氏族声望重置时间快照
	BOOL Loong_Query(std::vector <tagTWrap_Loong_RepRstTime*>&,EDBType eDBType);

	//好友列表(单向)
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Friend*>&,EDBType eDBType);

	//友好度
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FriendshipSave*>&,EDBType eDBType);

	//团购
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GroupPurchase*>&,EDBType eDBType);

	//帮派
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Guild*>&,EDBType eDBType);

	//帮派成员
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GuildMem*>&,EDBType eDBType);

	//Item
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Item*>&,EDBType eDBType);

	//Item baibao
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ItemBaiBao*>&,EDBType eDBType);

	//Item del
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ItemDel*>&,EDBType eDBType);

	//item cd time
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ItemCDTime*>&,EDBType eDBType);

	//item_needlog
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ItemNeedLog*>&,EDBType eDBType);

	//log baibao
	BOOL Loong_Query(std::vector <tagTWrap_Loong_LogBaiBao*>&,EDBType eDBType);

	//role del
	BOOL Loong_Query(std::vector <tagTWrap_Loong_RoleDel*>&,EDBType eDBType);

	//roledata
//	BOOL Loong_Query(std::vector <tagTWrap_Loong_RoleData*>&,EDBType eDBType);

	//roledata del
	BOOL Loong_Query(std::vector <tagTWrap_Loong_RoleDataDel*>&,EDBType eDBType);

	////skill
	//BOOL Loong_Query(std::vector <tagTWrap_Loong_Skill*>&,EDBType eDBType);

	//task
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Task*>&,EDBType eDBType);

	//task done
	BOOL Loong_Query(std::vector <tagTWrap_Loong_TaskDone*>&,EDBType eDBType);

	//title
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Title*>&,EDBType eDBType);

	//元宝
	BOOL Loong_Query(std::vector <tagTWrap_Loong_YBAccount*>&,EDBType eDBType);

	//元宝交易订单
	BOOL Loong_Query(std::vector <tagTWrap_Loong_YuanBaoOrder*>&,EDBType eDBType);

	//bill_item
	BOOL Loong_Query(std::vector <tagTWrap_Loong_BillItem*>&,EDBType eDBType);
	//bill_yuanbao
	BOOL Loong_Query(std::vector <tagTWrap_Loong_BillYuanBao*>&,EDBType eDBType);
	//bill_yuanbao_log
	BOOL Loong_Query(std::vector <tagTWrap_Loong_BillYuanBaoLog*>&,EDBType eDBType);
	//pet_data
	BOOL Loong_Query(std::vector <tagTWrap_Loong_PetData*>&,EDBType eDBType);
	//pet skill
	BOOL Loong_Query(std::vector <tagTWrap_Loong_PetSkill*>&,EDBType eDBType);
	//vip_stall
	BOOL Loong_Query(std::vector <tagTWrap_Loong_VipStall*>&,EDBType eDBType);
	//guild_commodity
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GuildCommodity*>&,EDBType eDBType);
	//guild_skill
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GuildSkill*>&,EDBType eDBType);
	//vip_netbar
	BOOL Loong_Query(std::vector <tagTWrap_Loong_VipNetBar*>&,EDBType eDBType);
	//guild_upgrade
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GuildFacilities*>&,EDBType eDBType);
	//commerce_rank
	BOOL Loong_Query(std::vector <tagTWrap_Loong_CommerceRank*>&,EDBType eDBType);
	//activity
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ActivityData*>&,EDBType eDBType);
	//left msg
	BOOL Loong_Query(std::vector <tagTWrap_Loong_LeftMsg*>&,EDBType eDBType);

	//1.3.1新增
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ExternalLink*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_NameHistory*>&,EDBType eDBType);

	BOOL Loong_Query(std::vector <tagTWrap_Loong_City*>&,EDBType eDBType);

	//1.3.2新增
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Master*>&,EDBType eDBType);

	//快捷栏
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ShortCutBar*>&,EDBType eDBType);

	//划拨日志
	BOOL Loong_QueryCentralbillingLog(std::set <string>& setData,EDBType eDBType);

	//ActiveRankData
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ActiveRankData*> &vecData,EDBType eDBType);

	//ForceBreakOutApprentice
	BOOL Loong_Query(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*> &vecData,EDBType eDBType);

	// 帮派宣战
	BOOL Loong_Query(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecData,EDBType eDBType);

	// 衣橱
	BOOL Loong_Query(std::vector <tagTWrap_Sheng_Wardrobe*> &vecData,EDBType eDBType);

	//roledata表
	BOOL Loong_QueryRoleDataName(std::set <tstring>& setData,EDBType eDBType);
	BOOL Loong_QueryRoleDataNameCrc(std::set <DWORD>& setData,EDBType eDBType);

	//神职角色ID
	BOOL Loong_QueryClergyRoleID(std::set <DWORD>& setData,EDBType eDBType);

	//Family
	BOOL Loong_Query(std::vector <tagTWrap_Loong_Family*> &vecData,EDBType eDBType);
	//FamilyMem
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FamilyMem*> &vecData,EDBType eDBType);
	//FamilyQuset
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FamilyQuset*> &vecData,EDBType eDBType);
	//FamilyRoleSprite
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecData,EDBType eDBType);
	//FamilySprite
	BOOL Loong_Query(std::vector <tagTWrap_Loong_FamilySprite*> &vecData,EDBType eDBType);
	//SpriteRequirement
	BOOL Loong_Query(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecData,EDBType eDBType);

	//GmServerData
	BOOL Loong_Query(std::vector <tagTWrap_Loong_GmServerData*>&,EDBType eDBType);

	// F-Plan 1.5.0  account_reactive, account_reactiver
	BOOL Loong_Query(std::vector <tagTWrap_Loong_AccountReactive*> &vecData,EDBType eDBType);
	BOOL Loong_Query(std::vector <tagTWrap_Loong_AccountReactiver*> &vecData,EDBType eDBType);

	// F-Plan 1.5.12  UseKeyCode_roleID
	BOOL Loong_Query(std::vector <tagTWrap_Loong_UseKeyCode*> &vecData,EDBType eDBType);

/************************ 写入相关 **************************/
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_AccountCommon*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_BlackList*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Buff*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ClanData*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Enemy*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Equip*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_EquipBaiBao*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_EquipDel*>&,EDBType eDBType);

	//氏族珍宝激活表
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ActClanTreasure*>&,EDBType eDBType);

	//氏族进入快照
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FameHallEnterSnap*>&,EDBType eDBType);

	//氏族声望重置时间快照
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_RepRstTime*>&,EDBType eDBType);

	//好友列表(单向)
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Friend*>&,EDBType eDBType);

	//友好度
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FriendshipSave*>&,EDBType eDBType);

	//团购
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GroupPurchase*>&,EDBType eDBType);

	//帮派
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Guild*>&,EDBType eDBType);

	//帮派成员
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GuildMem*>&,EDBType eDBType);

	//Item
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Item*>&,EDBType eDBType);

	//Item baibao
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ItemBaiBao*>&,EDBType eDBType);

	//Item del
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ItemDel*>&,EDBType eDBType);

	//item cd time
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ItemCDTime*>&,EDBType eDBType);

	//item_needlog
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ItemNeedLog*>&,EDBType eDBType);

	//log baibao
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_LogBaiBao*>&,EDBType eDBType);

	//role del
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_RoleDel*>&,EDBType eDBType);

	//roledata
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_RoleData*>&,EDBType eDBType);

	//roledata del
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_RoleDataDel*>&,EDBType eDBType);

	//skill
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Skill*>&,EDBType eDBType);

	//task
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Task*>&,EDBType eDBType);

	//task done
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_TaskDone*>&,EDBType eDBType);

	//title
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Title*>&,EDBType eDBType);

	//名帖
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_VisitingCard*>&,EDBType eDBType);

	//元宝
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_YBAccount*>&,EDBType eDBType);

	//元宝交易订单
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_YuanBaoOrder*>&,EDBType eDBType);

	//bill_item
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_BillItem*>&,EDBType eDBType);
	//bill_yuanbao
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_BillYuanBao*>&,EDBType eDBType);
	//bill_yuanbao_log
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_BillYuanBaoLog*>&,EDBType eDBType);
	//pet_data
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_PetData*>&,EDBType eDBType);
	//pet_skill
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_PetSkill*>&,EDBType eDBType);
	//vip_stall
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_VipStall*>&,EDBType eDBType);
	//guild_commodity
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GuildCommodity*>&,EDBType eDBType);
	//guild_skill
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GuildSkill*>&,EDBType eDBType);
	//vip_netbar
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_VipNetBar*>&,EDBType eDBType);
	//guild_upgrade
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GuildFacilities*>&,EDBType eDBType);
	//commerce_rank
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_CommerceRank*>&,EDBType eDBType);
	//activity
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ActivityData*>&,EDBType eDBType);
	//留言
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_LeftMsg*>&,EDBType eDBType);

	//1.3.1 新增
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ExternalLink*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_NameHistory*>&,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_City*>&,EDBType eDBType);

	//1.3.2 新增
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Master*>&,EDBType eDBType);

	//快捷栏
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ShortCutBar*>&,EDBType eDBType);

	//修炼状态
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_TrainState*>&,EDBType eDBType);

	//ActiveRankData
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ActiveRankData*>&,EDBType eDBType);

	//fabao
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Fabao*>&,EDBType eDBType);

	// F-Plan 1.5.0 SoulCrystal
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_SoulCrystal*>&,EDBType eDBType);

	// F-Plan 1.5.0 account_reactive
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_AccountReactive*>&,EDBType eDBType);
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_AccountReactiver*>&,EDBType eDBType);

	//ForceBreakOutApprentice
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>&,EDBType eDBType);

	// boardquest
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_BoardQuestSaveData*> &vecData,EDBType eDBType);

	// 帮派宣战
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_GuildWarDeclare*> &vecData,EDBType eDBType);

	// 角色在线时间表
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_CurMonthOnlineTime*> &vecData,EDBType eDBType);

	// 妖精重生数据
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_FabaoNirVanaData*> &vecData,EDBType eDBType);

	// 神之竞标
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_GodBid*> &vecData,EDBType eDBType);

	// 砸蛋
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_EggInfo*> &vecData,EDBType eDBType);

	// 衣橱
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_Wardrobe*> &vecData,EDBType eDBType);

	// 领域
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_Area*> &vecData,EDBType eDBType);

	// 稀有领域
	BOOL Loong_Insert(std::vector <tagTWrap_Sheng_RareArea*> &vecData,EDBType eDBType);

	//Family
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Family*> &vecData,EDBType eDBType);
	//FamilyMem
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FamilyMem*> &vecData,EDBType eDBType);
	//FamilyQuset
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FamilyQuset*> &vecData,EDBType eDBType);
	//FamilyRoleSprite
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FamilyRoleSprite*> &vecData,EDBType eDBType);
	//FamilySprite
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_FamilySprite*> &vecData,EDBType eDBType);
	//SpriteRequirement
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_SpriteRequirement*> &vecData,EDBType eDBType);

	BOOL Loong_Insert(std::vector <tagTWrap_Loong_GmServerData*> &vecData,EDBType eDBType);

	// F-Plan 1.5.12
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_UseKeyCode*> &vecData,EDBType eDBType);

	// F-Plan 1.6.0 holy
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_Holy*>&,EDBType eDBType);
	BOOL Loong_Insert(std::vector <tagTWrap_Loong_HolyEquip*>&,EDBType eDBType);


//account query
private:
	VOID FormatAccountQueryItem(Beton::MyStream *pStream,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption);
	VOID FormatAccountQueryRoleData(Beton::MyStream *pStream,DWORD dwAccountID,EDBType eDBType,ETBQueryExtraOption eEOption);

//paging query
private:
	VOID FormatPagingQueryEquip(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryEquipDel(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryTaskDone(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryTask(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryItemDel(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryFabao(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryVisitingCard(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryBoardQuest(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryCurMonthOnlineTime(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryFabaoNirVanaData(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryGodBid(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryEggInfo(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryArea(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	VOID FormatPagingQueryRareArea(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType);
	// F-Plan 1.5.0
	VOID FormatPagingQuerySoulCrystal(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	// 分页查询train_state
	VOID FormatPagingQueryTrainState(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	// 分页查询skill
	VOID FormatPagingQuerySkill(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	// 分页查询 shortcut_quickbar
	VOID FormatPagingQueryShortCutBar(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	// F-Plan 1.6.0
	VOID FormatPagingQueryHoly(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);
	VOID FormatPagingQueryHolyEquip(Beton::MyStream *pStream,INT64 n64Limit,EDBType eDBType);	
private:

	//格式化sql语句
	VOID FormatQueryAccountCommon(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryBlackList(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryBuff(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryClanData(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryEnemy(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryEquipBaiBao(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryActClanTreasure(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryFameHallEnterSnap(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryRepRstTime(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryFriendSave(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryFriendshipSave(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGroupPurchase(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGuild(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGuildMem(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryItemBaiBao(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryItemCDTime(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryItemNeedLog(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryLogBaiBao(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryRoleDel(Beton::MyStream *pStream,EDBType eDBType);
//	VOID FormatQueryRoleData(Beton::MyStream *pStream,EDBType eDBType);
//	VOID FormatQuerySkill(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryTask(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryTaskDone(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryTitle(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryYBAccount(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryYuanBaoOrder(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryBillItem(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryBillYuanbao(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryBillYuanbaoLog(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryPetData(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryPetSkill(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryVipStall(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGuildCommodity(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGuildSkill(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryVipBar(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGuildUpgrade(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryCommerceRank(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryActivity(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryLeftMsg(Beton::MyStream *pStream,EDBType eDBType);
	//1.3.1新增
	VOID FormatQueryExternalLink(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryNameHistory(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryCity(Beton::MyStream *pStream,EDBType eDBType);
	//1.3.2新增
	VOID FormatQueryMaster(Beton::MyStream *pStream,EDBType eDBType);
	//用于删号处理
	VOID FormatQueryCentralbillingLog(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryShortCutBar(Beton::MyStream *pStream,EDBType eDBType);
	//2.0 世博
	VOID FormatQueryActiveRankData(Beton::MyStream *pStream,EDBType eDBType);
	//2.2.1
	VOID FormatQueryForceBreakOutApprentice(Beton::MyStream *pStream,EDBType eDBType);

	//FPlan1.0.0
	VOID FormatQueryGuildWarDeclare(Beton::MyStream *pStream,EDBType eDBType);

	VOID FormatQueryRoleDataName(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryRoleDataNameCrc(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryClergyRoleID(Beton::MyStream *pStream,EDBType eDBType);

	//FPlan1.3.6
	VOID FormatQueryWardrobe(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryGmServerData(Beton::MyStream *pStream,EDBType eDBType);

	// F-Plan 1.5.0 account_reactive, account_reactiver
	VOID FormatQueryAccountReactive(Beton::MyStream *pStream,EDBType eDBType);
	VOID FormatQueryAccountReactiver(Beton::MyStream *pStream,EDBType eDBType);

	// F-Plan 1.5.12 UseKeyCode_roleID
	VOID FormatQueryUseKeyCode(Beton::MyStream *pStream,EDBType eDBType);	

	VOID ProcResQueryGmServerData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	BOOL FormatInsertGmServerData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);

	//格式化sql语句
	BOOL FormatInsertAccountCommon(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBlackList(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBuff(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertClanData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertEnemy(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertEquip(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType);
	BOOL FormatInsertActClanTreasure(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertFameHallEnterSnap(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertRepRstTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertFriendSave(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertFriendshipSave(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGroupPurchase(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuild(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuildMem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertItem(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType);
	BOOL FormatInsertItemCDTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertItemNeedLog(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertLogBaiBao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertRoleDel(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertRoleData(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType);
	BOOL FormatInsertSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertTask(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertTaskDone(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertTitle(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertVisitingCard(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertYBAccount(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertYuanBaoOrder(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBillItem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBillYuanbao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBillYuanbaoLog(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertPetData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertPetSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertVipStall(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuildCommodity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuildSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertVipBar(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuildUpgrade(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertCommerceRank(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertActivity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertLeftMsg(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertExternalLink(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertNameHistory(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertCity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertMaster(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertShortCutBar(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertTrainState(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertActiveRankData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertFabao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertForceBreakOutApprentice(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertBoardQuest(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGuildWarDeclare(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertCurMonthOnlineTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertFabaoNirVanaData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertGodBid(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertEggInfo(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertWardrobe(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertArea(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertRareArea(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	// F-Plan 1.5.0 SoulCrystal
	BOOL FormatInsertSoulCrystal(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	// F-Plan 1.5.0 account_reactive, account_reactiver
	BOOL FormatInsertAccountReactive(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertAccountReactiver(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);

	// F-Plan 1.5.12 UseKeyCode_roleID
	BOOL FormatInsertUseKeyCode(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);	

	// F-Plan 1.6.0 
	BOOL FormatInsertHoly(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	BOOL FormatInsertHolyEquip(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	
	//Family
	VOID FormatQueryFamily(Beton::MyStream *pStream,EDBType eDBType);
	//FamilyMem
	VOID FormatQueryFamilyMem(Beton::MyStream *pStream,EDBType eDBType);
	//FamilyQuset
	VOID FormatQueryFamilyQuset(Beton::MyStream *pStream,EDBType eDBType);
	//FamilyRoleSprite
	VOID FormatQueryFamilyRoleSprite(Beton::MyStream *pStream,EDBType eDBType);
	//FamilySprite
	VOID FormatQueryFamilySprite(Beton::MyStream *pStream,EDBType eDBType);
	//SpriteRequirement
	VOID FormatQuerySpriteRequirement(Beton::MyStream *pStream,EDBType eDBType);
	//Family
	BOOL FormatInsertFamily(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	//FamilyMem
	BOOL FormatInsertFamilyMem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	//FamilyQuset
	BOOL FormatInsertFamilyQuset(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	//FamilyRoleSprite
	BOOL FormatInsertFamilyRoleSprite(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	//FamilySprite
	BOOL FormatInsertFamilySprite(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);
	//SpriteRequirement
	BOOL FormatInsertSpriteRequirement(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType);

	//查询结果操作
	VOID ProcResQueryAccountCommon(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBlackList(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBuff(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryClanData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryEnemy(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryEquip(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryActClanTreasure(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryFameHallEnterSnap(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRepRstTime(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryFriendSave(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryFriendshipSave(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGroupPurchase(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuild(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuildMem(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryItem(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryItemCDTime(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryItemNeedLog(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryLogBaiBao(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRoleDel(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRoleData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQuerySkill(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryTask(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryTaskDone(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryTitle(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryVisitingCard(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryYBAccount(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryYuanBaoOrder(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBillItem(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBillYuanbao(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBillYuanbaoLog(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryPetData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryPetSkill(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryVipStall(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuildCommodity(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuildSkill(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryVipBar(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuildUpgrade(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryCommerceRank(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryActivity(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryLeftMsg(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryExternalLink(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryNameHistory(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryCity(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryMaster(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryCentralbillingLog(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryShortCutBar(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryTrainState(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRoleDataName(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRoleDataNameCrc(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryActiveRankData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryFabao(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryForceBreakOutApprentice(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryBoardQuest(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGuildWarDeclare(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryCurMonthOnlineTime(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryFabaoNirVanaData(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryGodBid(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryEggInfo(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryWardrobe(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryArea(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryRareArea(OUT LPVOID &pData, Beton::QueryResult *pRes);
	// F-Plan 1.5.0
	VOID ProcResQuerySoulCrystal(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryAccountReactive(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryAccountReactiver(OUT LPVOID &pData, Beton::QueryResult *pRes);

	// F-Plan 1.5.12
	VOID ProcResQueryUseKeyCode(OUT LPVOID &pData, Beton::QueryResult *pRes);

	// F-Plan 1.6.0 
	VOID ProcResQueryHoly(OUT LPVOID &pData, Beton::QueryResult *pRes);
	VOID ProcResQueryHolyEquip(OUT LPVOID &pData, Beton::QueryResult *pRes);	
	
	
	//Family	
	VOID ProcResQueryFamily(OUT LPVOID &pData, Beton::QueryResult *pRes);
	//FamilyMem	
	VOID ProcResQueryFamilyMem(OUT LPVOID &pData, Beton::QueryResult *pRes);
	//FamilyQuset	
	VOID ProcResQueryFamilyQuset(OUT LPVOID &pData, Beton::QueryResult *pRes);
	//FamilyRoleSprite	
	VOID ProcResQueryFamilyRoleSprite(OUT LPVOID &pData, Beton::QueryResult *pRes);
	//FamilySprite	
	VOID ProcResQueryFamilySprite(OUT LPVOID &pData, Beton::QueryResult *pRes);
	//SpriteRequirement	
	VOID ProcResQuerySpriteRequirement(OUT LPVOID &pData, Beton::QueryResult *pRes);
};

//Beton回调函数
VOID LoongDBCallBack(DataBase* pDB, INT nReason, INT nParam);

#define sLoongBeton CLoongLoongDBCtrl::getSingleton()

