#include "StdAfx.h"

#include "wx/wx.h"
#include "servermergetoolapp.h"
#include "Loong_LoongDBCtrl.h"



VOID CLoongLoongDBCtrl::FormatQueryAccountCommon(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("account_common",
		"AccountID,		AccountName,		SafeCodeCrc,		ResetTime,		BagPsdCrc,"
		"BaiBaoYuanBao,	WareSize,			WareSilver,			PersonalSet,"	
		//1.3.2 新增
		"LastLoginTime,	LastLogoutTime,		LastReceiveDailyOfflineRewardTime,	LastReceiveRegressionTime,"	
		
		"EMacc");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定的账号
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}	
}
VOID CLoongLoongDBCtrl::FormatQueryBlackList(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("blacklist","RoleID,BlackID");
}
VOID CLoongLoongDBCtrl::FormatQueryBuff(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("buff", 
		"RoleID,"			"SrcUnitID,"		"SrcSkillID,"			"ItemTypeID,"		"ItemSerialID,"		
		"BuffID,"			"CurTick,"			"buff.Level,"				"CurLapTimes,"		"EffectSkillID");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的buff
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");	
	}
}
VOID CLoongLoongDBCtrl::FormatQueryClanData(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("clan_data", "RepXuanYuan,	ConXuanYuan,		ActCntXuanYuan,"
									"RepShenNong,	ConShenNong,		ActCntShenNong,"
									"RepFuXi,		ConFuXi,			ActCntFuXi,"
									"RepSanMiao,	ConSanMiao,			ActCntSanMiao,"
									"RepJiuLi,		ConJiuLi,			ActCntJiuLi,"
									"RepYueZhi,		ConYueZhi,			ActCntYueZhi,"
									"RepNvWa,		ConNvWa,			ActCntNvWa,"
									"RepGongGong,	ConGongGong,		ActCntGongGong,"
									"FameMask,		RepRstTimeStamp,	RoleID,"
									"TotalFame,		FamePm,				RemoveFlag"	);
}
//enemy
VOID CLoongLoongDBCtrl::FormatQueryEnemy(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("enemy","RoleID,EnemyID");
}

// equip
VOID CLoongLoongDBCtrl::FormatQueryEquip(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("equip",
		"SerialNum,"			"Quality,"			"PotValUsed,"			"MinUseLevel,"			"MaxUseLevel,"	
		"WuHun,"				"MinDmg,"			"MaxDmg,"				"Armor,"				"PotVal,"		
		"RoleAttEffect,"		"PosyTimes,"		"PosyEffect,"			"EngraveTimes,"			"EngraveAtt,"
		"HoleNum,"				"HoleGemID,"		"BrandLevel,"			"DerateVal,"			"LongFuLevel,"
		"WuXing,"				"CanCut,"			"LongInnerID,"			"LongOuterID,"			"SpecAtt,"	
		"Appearance,"			"Rein,"				"Savvy,"				"Fortune,"				"ColorID,"	
		"QltyModPct,"			"QltyModPctEx,"		"AttALimMod,"			"AttALimModPct,"		"FlareVal,"
		"PotValModPct,"			"PotIncTimes,"		"PurpleQltyIdfPct,"	
		//1.3.2 新增
		"Signed,"				"Signature,"		"SignRoleID,"
		//2.2.0新增
		"SoaringFlag,"			"CastingTimes,"
		//FPlan1.0.0
		"RoleAtt2EffectCount,"	"ConsolidateLevel,"	"ConsolidateAttEffect,"	"LastGodStrengthenLevel,""GodStrengthenLevel,"
		"CurGodStrengthenExp,"	"GodStrengthenTime," 

		// F-Plan 1.5.0
		"GrowID,"  "GrowValue,"  "GrowLevel,"  "GrowTimes,"  "GrowUseIMTimes,"  "GrowStars,"  "GrowAtt," "DiamondLevel"
		);
	/*
	//排序分页
	pStream->FillString(" join ( select SerialNum from equip");
	pStream->FillString(" order by SerialNum desc limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
	pStream->FillString(")");

	pStream->FillString("as equip2 using(SerialNum)");
	*/

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的equip
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");	
	}
}

VOID CLoongLoongDBCtrl::FormatQueryEquipBaiBao(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("equip_baibao",
		"SerialNum,"	"Quality,"			"PotValUsed,"		"MinUseLevel,"	"MaxUseLevel,"	
		"WuHun,"		"MinDmg,"			"MaxDmg,"			"Armor,"		"PotVal,"		
		"RoleAttEffect,""PosyTimes,"		"PosyEffect,"		"EngraveTimes,"	"EngraveAtt,"
		"HoleNum,"		"HoleGemID,"		"BrandLevel,"		"DerateVal,"	"LongFuLevel,"
		"WuXing,"		"CanCut,"			"LongInnerID,"		"LongOuterID,"	"SpecAtt,"	
		"Appearance,"	"Rein,"				"Savvy,"			"Fortune,"		"ColorID,"	
		"QltyModPct,"	"QltyModPctEx,"		"AttALimMod,"		"AttALimModPct,""FlareVal,"
		"PotValModPct," "PotIncTimes,"		"PurpleQltyIdfPct,"
		//1.3.2 新增
		"Signed,"		"Signature,"		"SignRoleID,"
		//2.2.0新增
		"SoaringFlag,"  "CastingTimes,"
		//FPlan1.0.0
		"RoleAtt2EffectCount,"	"ConsolidateLevel,"	"ConsolidateAttEffect,"	"LastGodStrengthenLevel,""GodStrengthenLevel,"
		"CurGodStrengthenExp,"	"GodStrengthenTime," 

		// F-Plan 1.5.0
		"GrowID,"  "GrowValue,"  "GrowLevel,"  "GrowTimes,"  "GrowUseIMTimes,"  "GrowStars,"  "GrowAtt," "DiamondLevel"
		);

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的equip_baibao
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}

}

VOID CLoongLoongDBCtrl::FormatQueryActClanTreasure(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("famehall_act_treasure",
		"TreasureID,"	"ActNameID," "RoleID," "ClanType," "ActTime");
}
//氏族进入快照
VOID CLoongLoongDBCtrl::FormatQueryFameHallEnterSnap(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("famehall_entersnap",
		"RoleID,"	"EnterNameID," "ClanType," "EnterTime");
}
//氏族声望重置时间快照
VOID CLoongLoongDBCtrl::FormatQueryRepRstTime(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("famehall_rep_rst_timestamp",
		"ClanType," "ResetTimeStamp");
}
//好友
VOID CLoongLoongDBCtrl::FormatQueryFriendSave(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("friend",
		"RoleID," "FriendID," "GroupID");
}
//好友度
VOID CLoongLoongDBCtrl::FormatQueryFriendshipSave(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("friendship",
		"RoleID," "FriendID," "FriVal");
}
//团购
VOID CLoongLoongDBCtrl::FormatQueryGroupPurchase(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("group_purchase",
		"GuildID," "RoleID," "MallID," "ItemPrice," "ParticipatorNum," "RequireNum,"
		"RemainTime," "ParticipatorList");
}
//帮派
VOID CLoongLoongDBCtrl::FormatQueryGuild(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild",
		"ID,"			"Name,"			"FounderNameID,"	"SpecState,"		"Level,"
		"HoldCity0,"	"HoldCity1,"	"HoldCity2,"		"Fund,"				"Material,"
		"Rep,"			"DailyCost,"	"Peace,"			"Rank,"				"Tenet,"
		"Symbol,"		"CreateTime,"	"GroupPurchase,"	"LeaderID,"			"RemainSpreadTimes,"
		"Commendation,"
		//FPlan 1.2.2
		"DailyRewardTakenTimes");
}
//帮派成员
VOID CLoongLoongDBCtrl::FormatQueryGuildMem(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild_member",
		"RoleID,"	"GuildID,"		"GuildPos,"		"TotalContrib,"		"CurContrib,"
		"Exploit,"	"Salary,"		"JoinTime,"		"CanUseGuildWare");
}

//item baibao
VOID CLoongLoongDBCtrl::FormatQueryItemBaiBao(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("item_baibao",
		"SerialNum,"	"Num,"				"TypeID,"		"Bind,"				"LockStat,"
		"UseTimes,"		"FirstGainTime,"	"CreateMode,"	"CreateID,"			"CreatorID,"	
		"CreateTime,"	"OwnerID,"			"AccountID,"	"ContainerTypeID,"	"Suffix,"
		"NameID,"		"UnlockTime,"
		// Fplan 1.2.1
		"ItemSpecVal1,"	"ItemSpecVal2");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定的账号的item_baibao
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}

//item cd time
VOID CLoongLoongDBCtrl::FormatQueryItemCDTime(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("item_cdtime",
		"RoleID," "CDTime");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的物品&装备冷却时间
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}
//item need log
VOID CLoongLoongDBCtrl::FormatQueryItemNeedLog(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("item_needlog",
		"TypeID," "NeedLog," "MinQlty");
}
//log baibao
VOID CLoongLoongDBCtrl::FormatQueryLogBaiBao(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("log_baibao", "AccountID,RoleID,LogTime,n16Type,TypeID,LeaveWords");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的百宝袋记录
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}
//role del
VOID CLoongLoongDBCtrl::FormatQueryRoleDel(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("role_del", "AccountID,RoleID,RoleName,IP,DeleteTime");
}


//task
VOID CLoongLoongDBCtrl::FormatQueryTask(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("task","TaskID,StartTime,TaskFlag,Monster0Num,Monster1Num,Monster2Num,"
		"Monster3Num,Monster4Num,Monster5Num,ScriptData, DynamicTarget, RoleID, ChuandaoCount");
}
//task done
VOID CLoongLoongDBCtrl::FormatQueryTaskDone(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("task_done", "TaskID,Times,StartTime,RoleID");
}
//title
VOID CLoongLoongDBCtrl::FormatQueryTitle(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("title", "ID,Count,RoleID,TimeRemain");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的角色称号
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}
//元宝
VOID CLoongLoongDBCtrl::FormatQueryYBAccount(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("ybaccount", "RoleID,YuanBao,Gold,Silver,bSellOrder,bBuyOrder");
}
//元宝交易订单
VOID CLoongLoongDBCtrl::FormatQueryYuanBaoOrder(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("yuanbaoorder", "dwID,RoleID,OrderType,Price,Number,OrderMode,StartTime,EndTime,AvgPrice,DealNum");
}
//bill item
VOID CLoongLoongDBCtrl::FormatQueryBillItem(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("bill_item", "token_id,account_id,item_id,item_num");
}
//bill yuan bao
VOID CLoongLoongDBCtrl::FormatQueryBillYuanbao(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("bill_yuanbao", "token_id,account_id,yuanbao");
}
//bill yuan bao log
VOID CLoongLoongDBCtrl::FormatQueryBillYuanbaoLog(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("bill_yuanbao_log", "token_id,account_id,yuanbao,time");
}

//pet_data
VOID CLoongLoongDBCtrl::FormatQueryPetData(Beton::MyStream *pStream,EDBType eDBType)
{

	pStream->SetSelect("pet_data",
		//	0			1			2			3
		"pet_id,		pet_name,	master_id,	type_id,"
		//	4			5			6			7
		"quality,		aptitude,	potential,	cur_spirit,"
		//	8			9			10			11
		"cur_exp,		step,		grade,		talent_count,	"
		//	12			13			14
		"wuxing_energy,	pet_state,	pet_lock,	pet_data.RemoveFlag,"

		"pet_value,		pet_pm,		birthday,	live,"
		
		"lifeadded");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的宠物
		//pStream->SetWhere();
		//pStream->FillString(" master_id in (select RoleID from roledata where AccountID in (select AccountID from chose_account) ) ");	

		pStream->FillString("  left join roledata on pet_data.master_id = roledata.RoleID ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}
//pet skill
VOID CLoongLoongDBCtrl::FormatQueryPetSkill(Beton::MyStream *pStream,EDBType eDBType)
{
	//			
	pStream->SetSelect("pet_skill", "petskill_typeid,	petid,	para1,	para2");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的宠物技能
		pStream->FillString("  left join pet_data on pet_skill.petid = pet_data.pet_id ");
		pStream->FillString("  left join roledata on pet_data.master_id = roledata.RoleID ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");


		//pStream->FillString("petid in (select pet_id from pet_data where master_id in (select RoleID from roledata where AccountID in (select AccountID from chose_account) ) )");	
	}	
}

//vip_stall
VOID CLoongLoongDBCtrl::FormatQueryVipStall(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("vip_stall", "StallID,RoleID,RemainTime");
}
//guild_commodity
VOID CLoongLoongDBCtrl::FormatQueryGuildCommodity(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild_commodity", "role_id,guild_id,role_level,tael,goods");
}
//guild_skill
VOID CLoongLoongDBCtrl::FormatQueryGuildSkill(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild_skill", 
									"guild_id,"		"skill_id,"		"progress,"		"level,"	"researching,"
									//FPlan1.0.0
									"active");
}
//vip_netbar
VOID CLoongLoongDBCtrl::FormatQueryVipBar(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("vip_netbar", "accountid,login_time");
}
//guild_upgrade
VOID CLoongLoongDBCtrl::FormatQueryGuildUpgrade(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild_upgrade",
		"guild_id,"			"type,"			"level,"			"progress,"		"item_type_1,"
		"item_neednum_1,"	"item_type_2,"	"item_neednum_2,"	"item_type_3,"	"item_neednum_3,"
		"item_type_4,"		"item_neednum_4");
}
//commerce_rank
VOID CLoongLoongDBCtrl::FormatQueryCommerceRank(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("commerce_rank",
		"role_id,"			"guild_id,"			"times,"			"tael");
}
//activity
VOID CLoongLoongDBCtrl::FormatQueryActivity(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("activity",
							"dwID,"			"ScriptData,"
							// FPlan1.3.0
							"ScriptData1,"	"ScriptData2,"	"ScriptData3,"	"ScriptData4");
}
//留言
VOID CLoongLoongDBCtrl::FormatQueryLeftMsg(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("left_msg","msg_id,"	"roleid," "msg_data");
}

//外部链接表
VOID CLoongLoongDBCtrl::FormatQueryExternalLink(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("external_links","LinkName,"	"LinkURL");
}
//记录改名记录的表
VOID CLoongLoongDBCtrl::FormatQueryNameHistory(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("name_history",
						"AccountID,"	"RoleID,"	"name_1,"	"name_2,"	"name_3,"
						"name_4,"		"name_5,"	"name_6,"	"name_7,"	"name_8,"
						"name_9,"		"name_10");
}
//城市
VOID CLoongLoongDBCtrl::FormatQueryCity(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("city",
		"id,"				"guild_id,"		"defence,"		"eudemon_tally,"	"tax_rate,"
		"tax_rate_time,"	"taxation,"		"prolificacy,"	"signup_list");
}

//master
VOID CLoongLoongDBCtrl::FormatQueryMaster(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("Master",
		"dwID,"				"JingWuPoint,"		"QinWuPoint,"	"LevelUpBonus,"		"RewardNum,"	"ApprenticeData");
}

//centralbilling
VOID CLoongLoongDBCtrl::FormatQueryCentralbillingLog(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("centralbilling_log",	"USERNAME");
}

//roledata
VOID CLoongLoongDBCtrl::FormatQueryRoleDataName(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("roledata",	"RoleName");
}

//roledatacrc
VOID CLoongLoongDBCtrl::FormatQueryRoleDataNameCrc(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("roledata",	"RoleNameCrc");
}

// 神职角色ID
VOID CLoongLoongDBCtrl::FormatQueryClergyRoleID(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("Clergy_Mars", "RoleID");
	pStream->FillString(" WHERE RecvOrNot = 1");
	pStream->FillString(" UNION ");
	pStream->SetSelect("Clergy_Apollo", "RoleID");
	pStream->FillString(" WHERE RecvOrNot = 1");
	pStream->FillString(" UNION ");
	pStream->SetSelect("Clergy_Rabbi", "RoleID");
	pStream->FillString(" WHERE RecvOrNot = 1");
	pStream->FillString(" UNION ");
	pStream->SetSelect("Clergy_Peace", "RoleID");
	pStream->FillString(" WHERE RecvOrNot = 1");
}


//short cut bar
VOID CLoongLoongDBCtrl::FormatQueryShortCutBar(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("shortcut_quickbar",	"RoleID,"	"quickbardata");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的角色快捷栏
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}


//ActiveRankData
VOID CLoongLoongDBCtrl::FormatQueryActiveRankData(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("activity_rankdata", "role_id, role_name,role_level,datatype1,datatype2,datatype3,datatype4,datatype5,datatype6,datatype7,datatype8");
}


//force_break_out_apprentice
VOID CLoongLoongDBCtrl::FormatQueryForceBreakOutApprentice(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("force_break_out_apprentice", "RoleID, MasterID,BreakoutInsertTime");
}

//guild_war_declare
VOID CLoongLoongDBCtrl::FormatQueryGuildWarDeclare(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("guild_war_declare", "AttackGuildID,DefenceGuildID,WarDeclareTime,WarEnded");
}

//衣橱
VOID CLoongLoongDBCtrl::FormatQueryWardrobe(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("wardrobe", "wardrobe.RoleID,LayerID,wardrobe.Level,State,PlayActStartTime,PlayActFriendID");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定的账号
				
		//pStream->FillString("RoleID in (select RoleID from roledata where AccountID in (select AccountID from chose_account) )");

		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}	
}

VOID CLoongLoongDBCtrl::FormatQueryGmServerData(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("gm_server_data", "data_value");
}

VOID CLoongLoongDBCtrl::FormatQuerySkill(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("skill","RoleID,ID,BiddenLevel,SelfLevel,Proficiency,CoolDown,active_time");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的skill数据
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}

}

VOID CLoongLoongDBCtrl::FormatQueryFabao(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("fabao",		"SerialNum,"		"NativeIntelligence,"	"Stage,"				"PracticeValue,"	"AttTypes,"
		"AttGrowings0,"		"AttGrowings,"			"WuxingType,"			"WuxingHoles,"		"WuxingBeadIDs,"
		"SlotAttTypes,"		"SlotAttValue,"			"0,"					"UseLevel,"			"SlotNum,"
		"SlotLingzhuMatch,"	"SlotMacthNdx,"
		//FPlan 1.2.1
		"Mood,"				"MoodTicks,"			"MoodBuffID,"				"EleInjuryType,"	"EleInjury,"
		"EleResisFir,"		"EleResisWater,"		"EleResisEarth,"			"EleResisWind,"		"EleResisBright,"
		"EleResisDark,"		"DamageUpgradeTimes,"	"ResistanceUpgradeTimes,"	"FairySkill1,"		"FairySkill2,"
		"FairySkill3,"		"FairySkill4,"			"FairySkill5,"				"UnsureSkill,"
		//FPlan 1.2.2
		"StoredExpOpen,"	"CurStoredExp,"
		//FPlan 1.3.0
		"FabaoStatus,"		"StatusChangeTime,"		"BirthValue,"				"BirthTimes,"		"LevelBirth,"
		"IsGivenBirthTimes," "MaxIntelligence");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的法宝
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString("chose_account.AccountID is not null");	
	}
}

VOID CLoongLoongDBCtrl::FormatQuerySoulCrystal(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("soulcrystal",		"SerialNum,"		"SoulLevel,"	"SoulQlty,"				"SoulSkillID,"	"SoulAttID1,"
		"SoulAttID2,"		"SoulAttID3,"			"SoulAttID4,"			"SoulAttID5,"		"SoulAttID6,"
		"SoulAttID7,"		"SoulAttID8");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的魂晶SoulCrystal
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString("chose_account.AccountID is not null");	
	}
}

VOID CLoongLoongDBCtrl::FormatQueryFabaoNirVanaData(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("fabao_extend_his",
		"SerialNum,"		"fabao_extend_his.AccountID,"		"RoleID,"				"Stage,"					"EleInjuryType,"
		"EleInjury,"		"EleResisFire,"		"EleResisWater,"		"EleResisEarth,"			"EleResisWind,"
		"EleResisBright,"	"EleResisDark,"		"DamageUpgradeTimes,"	"ResistanceUpgradeTimes"	);

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定的账号
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");	
	}
}

VOID CLoongLoongDBCtrl::FormatQueryArea(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("area", "AreaID,RoleID,Activation,CDTime");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的领域数据
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}
}

VOID CLoongLoongDBCtrl::FormatQueryVisitingCard(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("visiting_card", "visiting_card.Level,			Job,	MateRoleID,		FactionID,		Position,"
		"Visibility,	visiting_card.Sex,	Constellation,	ChineseZodiac,	Area,"
		"Province,		City,	HeadUrl,		Signature,		NameHistory,"
		"RoleID");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的名帖
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->FillString("  left join chose_account using(AccountID) ");
		pStream->SetWhere();
		pStream->FillString(" chose_account.AccountID is not null");
	}

}
// F-Plan 1.5.0 account_reactive, account_reactiver
VOID CLoongLoongDBCtrl::FormatQueryAccountReactive(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("account_reactive", "accountid, keycode, reward, rewardlevel");
}

VOID CLoongLoongDBCtrl::FormatQueryAccountReactiver(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("account_reactiver", "accountid, keycode, level, exp");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//按用户查询item表
VOID CLoongLoongDBCtrl::FormatAccountQueryItem(Beton::MyStream *pStream, DWORD dwAccountID, EDBType eDBType, ETBQueryExtraOption eEOption)
{
	pStream->SetSelect("item",
		"SerialNum,"	"Num,"				"TypeID,"		"Bind,"				"LockStat,"
		"UseTimes,"		"FirstGainTime,"	"CreateMode,"	"CreateID,"			"CreatorID,"	
		"CreateTime,"	"OwnerID,"			"AccountID,"	"ContainerTypeID,"	"Suffix,"
		"NameID,"		"UnlockTime,"		
		// Fplan 1.2.1
		"ItemSpecVal1,"	"ItemSpecVal2");

	pStream->SetWhere();

	pStream->FillString("AccountID = ") << dwAccountID;

	pStream->FillString(" AND ");

	if(eEOption == EM_TBQ_EOPTION_GUILDWARE)
	{
		pStream->FillString(" ContainerTypeID = ") << EICT_GuildWare;
	}
	else
	{
		pStream->FillString(" ContainerTypeID != ") << EICT_GuildWare;
	}

}

//按用户查询roledata表
VOID CLoongLoongDBCtrl::FormatAccountQueryRoleData(Beton::MyStream *pStream, DWORD dwAccountID, EDBType eDBType,  ETBQueryExtraOption eEOption)
{
	pStream->SetSelect("roledata", 
		"RoleName,"				"RoleNameCrc,"					"DisplaySet,"					"Sex,"					"HairModelID,"
		"HairColorID,"			"FaceModelID,"					"FaceDetailID,"					"DressModelID,"			"MapID,"	
		"X,"					"Y,"							"Z,"							"FaceX,"				"FaceY,"	
		"FaceZ,"				"Class,"						"ClassEx,"						"Level,"				"ExpCurLevel,"	
		"HP,"					"MP,"							"AttPtAvail,"					"TalentPtAvail,"		"PhysiqueAdded,"	
		"StrengthAdded,"		"PneumaAdded,"					"InnerforceAdded,"				"TechniqueAdded,"		"AgilityAdded,"	
		"Injury,"				"Vitality,"						"Knowledge,"					"Morality,"				"Culture,"	
		"Credit,"				"Identity,"						"VIPPoint,"						"BagSize,"				"BagGold,"	
		"BagSilver,"			"BagYuanBao,"					"GuildID,"						"0,"					"0,"	
		"0,"					"CreateTime,"					"LoginTime,"					"LogoutTime,"			"OnlineTime,"	
		"CurOnlineTime,"		"Rage,"							"Endurance,"					"SGFlag,"				"CloseSGTime,"

		"TalentType1,"			"TalentType2,"					"TalentType3,"					"TalentType4,"			"TalentVal1,"		
		"TalentVal2,"			"TalentVal3,"					"TalentVal4,"					"RebornMapID,"			"TotalTax,"			
		"Morale,"				"GetMallFreeTime,"				"RemoteOpenSet,"				"CurTitleID,"			"ExchangeVolume,"	
		"RoleID,"				"AvatarEquip,"					"RemoveFlag,"					"RemoveTime,"			"AccountID,"		
		"ScriptData,"			"TreasureSum,"					"StallLevel,"					"StallDailyExp,"		"StallCurExp,"	
		"StallLastTime,"

		"Hostility,"			"DeadUnSetSafeGuardCountDown,"	"ItemTransportMapID,"			"ItemTransportX,"		"ItemTransportZ,"
		"ItemTransportY,"		"Strength," 					"StrengthNum,"					"LoverID,"				"HaveWedding,"	
		"LastLessingTime,"		"LastLessingLevel,"				"NeedPrisonRevive,"				"LevelPm,"				"EquipValue,"
		"EquipPm,"				"LastLessingLoongDate,"			"CurrentDayOnlineTime,"			"OneDayFirstLoginedTime,""OnlineRewardPerDayRecTime,"
		"LastUpgrade,"			"WorkedLevel,"					"OfflineExperienceRewardFlag,"	"Spirit,"				"FixSpirit,"
		"ConsumptiveStrength,"	"KillCount,"					"AdvanceStrength,"				"MasterID,"				"JingWuPoint,"
		"QinWuPoint,"			"0,"							"MARelation,"

		"TrainStateTable,"		"WuXun,"						"WuJi,"							"LastLessingLevelEx,"	"GodMiraclePoints,"
		
		"TrainStateTransfered," "Weary,"
		
		"SoaringValue,"			"SoaringSkillLearnTimes,"		"Toughness,"
		//FPlan 1.0.0
		"ConsolidateTimes,"		"CompleteRefreshTime,"			"IMRefreshTime,"				"RoleState,"			"FollowPetPocketValue,"
		"RidingPetPocketValue,"	"VipLevel,"						"VipStartTime,"					"FreeWorldTalkTimes,"	"CanRankFlag,"			
		"YaoJingValue,"
		//FPlan 1.2.0
		"Buy50LvlItemFlag,"				"Buy60LvlItemFlag,"				"Buy70LvlItemFlag,"				"Buy80LvlItemFlag,"
		//FPlan 1.2.1
		"YListCompleteRefreshTime,"		"YListIMRefreshTime,"
		//FPlan 1.2.2
		"TakeGuildRewardTime,"			"ExpPilularUseTimes,"
		//FPlan 1.3.0
		"PickupModeSetting,"
		//FPlan 1.3.3
		"PlayerBack,"					"PlayerBackDays,"				"VipMaxDays,"
		//FPlan 1.3.6
		"UseConstraintsMaxPKSafeGuardLevel,"	"BePlayActLayerID,"		"BePlayActLevel,"
		//FPlan 1.4.0
		"GodHead,"				"GodFaith,"						"GodCondenced,"					"Clergy,"				"PreClergy,"
		"ClergyMarsPoint,"		"ClergyApolloPoint,"			"ClergyRabbiPoint,"				"ClergyPeacePoint,"		"ClergyCandidateType,"
		"Clergy4SeniorOrJunior,"
		//FPlan 1.4.1
		"KeyCodeRewarded,"
		//FPlan 1.4.3
		"FamilyID,"
		//FPlan 1.4.4
		"TrainDate,"			"TrainNum,"
		//FPlan 1.4.6
		"award_point,"			"award_flag,"

		//FPlan 1.4.8
		"BrotherEndTime,"		"BrotherTeacherID,"

		// FPlan 1.4.11
		"PulseRemainTime,"		"TigerTime,"
		// FPlan 1.5.0
		"god_soul,"			"monster_soul,"			"god_point,"		"monster_point,"		"flower_num,"		"egg_num"
		);

	pStream->SetWhere();

	pStream->FillString("AccountID = ") << dwAccountID;

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//分页查询
VOID CLoongLoongDBCtrl::FormatPagingQueryEquip(Beton::MyStream *pStream, INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("equip",
		"SerialNum,"			"Quality,"			"PotValUsed,"			"MinUseLevel,"			"MaxUseLevel,"	
		"WuHun,"				"MinDmg,"			"MaxDmg,"				"Armor,"				"PotVal,"		
		"RoleAttEffect,"		"PosyTimes,"		"PosyEffect,"			"EngraveTimes,"			"EngraveAtt,"
		"HoleNum,"				"HoleGemID,"		"BrandLevel,"			"DerateVal,"			"LongFuLevel,"
		"WuXing,"				"CanCut,"			"LongInnerID,"			"LongOuterID,"			"SpecAtt,"	
		"Appearance,"			"Rein,"				"Savvy,"				"Fortune,"				"ColorID,"	
		"QltyModPct,"			"QltyModPctEx,"		"AttALimMod,"			"AttALimModPct,"		"FlareVal,"
		"PotValModPct,"			"PotIncTimes,"		"PurpleQltyIdfPct,"	
		//1.3.2 新增
		"Signed,"				"Signature,"		"SignRoleID,"
		//2.2.0新增
		"SoaringFlag,"			"CastingTimes,"
		//FPlan1.0.0
		"RoleAtt2EffectCount,"	"ConsolidateLevel,"	"ConsolidateAttEffect,"	"LastGodStrengthenLevel,""GodStrengthenLevel,"
		"CurGodStrengthenExp,"	"GodStrengthenTime," 

		// F-Plan 1.5.0
		"GrowID,"  "GrowValue,"  "GrowLevel,"  "GrowTimes,"  "GrowUseIMTimes,"  "GrowStars,"  "GrowAtt," "DiamondLevel"
		);
	/*
	//排序分页
	pStream->FillString(" join ( select SerialNum from equip");
	pStream->FillString(" order by SerialNum desc limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
	pStream->FillString(")");

	pStream->FillString("as equip2 using(SerialNum)");
	*/

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的魂晶SoulCrystal
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->SetWhere();
		pStream->FillString("AccountID in (select AccountID from chose_account)");	
	}

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

VOID CLoongLoongDBCtrl::FormatPagingQueryEquipDel(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("equip_del",
		"SerialNum,"	"Quality,"			"PotValUsed,"		"MinUseLevel,"	"MaxUseLevel,"	
		"WuHun,"		"MinDmg,"			"MaxDmg,"			"Armor,"		"PotVal,"		
		"RoleAttEffect,""PosyTimes,"		"PosyEffect,"		"EngraveTimes,"	"EngraveAtt,"
		"HoleNum,"		"HoleGemID,"		"BrandLevel,"		"DerateVal,"	"LongFuLevel,"
		"WuXing,"		"CanCut,"			"LongInnerID,"		"LongOuterID,"	"SpecAtt,"	
		"Appearance,"	"Rein,"				"Savvy,"			"Fortune,"		"ColorID,"	
		"QltyModPct,"	"QltyModPctEx,"		"AttALimMod,"		"AttALimModPct,""FlareVal,"
		"PotValModPct," "PotIncTimes,"		"PurpleQltyIdfPct," 
		//1.3.2 新增
		"Signed,"		"Signature,"		"SignRoleID,"
		//2.2.0新增
		"SoaringFlag,"	"CastingTimes,"
		//FPlan1.0.0
		"RoleAtt2EffectCount,"	"ConsolidateLevel,"	"ConsolidateAttEffect,"	"LastGodStrengthenLevel,""GodStrengthenLevel,"
		"CurGodStrengthenExp,"	"GodStrengthenTime," 

		// F-Plan 1.5.0
		"GrowID,"  "GrowValue,"  "GrowLevel,"  "GrowTimes,"  "GrowUseIMTimes,"  "GrowStars,"  "GrowAtt," "DiamondLevel"
 		);

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//fabao
VOID CLoongLoongDBCtrl::FormatPagingQueryFabao(Beton::MyStream *pStream,  INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("fabao",		"SerialNum,"		"NativeIntelligence,"	"Stage,"				"PracticeValue,"	"AttTypes,"
									"AttGrowings0,"		"AttGrowings,"			"WuxingType,"			"WuxingHoles,"		"WuxingBeadIDs,"
									"SlotAttTypes,"		"SlotAttValue,"			"0,"					"UseLevel,"			"SlotNum,"
									"SlotLingzhuMatch,"	"SlotMacthNdx,"
									//FPlan 1.2.1
									"Mood,"				"MoodTicks,"			"MoodBuffID,"				"EleInjuryType,"	"EleInjury,"
									"EleResisFir,"		"EleResisWater,"		"EleResisEarth,"			"EleResisWind,"		"EleResisBright,"
									"EleResisDark,"		"DamageUpgradeTimes,"	"ResistanceUpgradeTimes,"	"FairySkill1,"		"FairySkill2,"
									"FairySkill3,"		"FairySkill4,"			"FairySkill5,"				"UnsureSkill,"
									//FPlan 1.2.2
									"StoredExpOpen,"	"CurStoredExp,"
									//FPlan 1.3.0
									"FabaoStatus,"		"StatusChangeTime,"		"BirthValue,"				"BirthTimes,"		"LevelBirth,"
									"IsGivenBirthTimes," "MaxIntelligence");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的法宝
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->SetWhere();
		pStream->FillString("AccountID in (select AccountID from chose_account)");	
	}
	

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//分页查询
VOID CLoongLoongDBCtrl::FormatPagingQueryTaskDone(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("task_done", "TaskID,Times,StartTime,RoleID");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//分页查询
VOID CLoongLoongDBCtrl::FormatPagingQueryTask(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("task","TaskID,StartTime,TaskFlag,Monster0Num,Monster1Num,Monster2Num,"
		"Monster3Num,Monster4Num,Monster5Num,ScriptData, DynamicTarget, RoleID, ChuandaoCount");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//分页查询
VOID CLoongLoongDBCtrl::FormatPagingQueryItemDel(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("item_del",
		"SerialNum,"	"Num,"				"TypeID,"		"Bind,"				"LockStat,"
		"UseTimes,"		"FirstGainTime,"	"CreateMode,"	"CreateID,"			"CreatorID,"	
		"CreateTime,"	"OwnerID,"			"AccountID,"	"ContainerTypeID,"	"Suffix,"
		"NameID,"		"UnlockTime,"
		// Fplan 1.2.1
		"ItemSpecVal1,"	"ItemSpecVal2");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//名帖
VOID CLoongLoongDBCtrl::FormatPagingQueryVisitingCard(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("visiting_card", "Level,			Job,	MateRoleID,		FactionID,		Position,"
		"Visibility,	Sex,	Constellation,	ChineseZodiac,	Area,"
		"Province,		City,	HeadUrl,		Signature,		NameHistory,"
		"RoleID");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// 读取角色任务板任务的SQL语句格式化
VOID CLoongLoongDBCtrl::FormatPagingQueryBoardQuest(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("quest_board", "RoleID,QuestID,BoardID,State,BoardType");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// 角色在线时间统计
VOID CLoongLoongDBCtrl::FormatPagingQueryCurMonthOnlineTime(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("role_online_time_each_month", 
									"AccountID,"	"RoleID,"	"January,"	"February,"		"March,"
									"April,"		"May,"		"June,"		"July,"			"August,"
									"September,"	"October,"	"November,"	"December,"		"Year,"
									//FPlan 1.2.2
									"LastUpdateTime");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// 妖精重生
VOID CLoongLoongDBCtrl::FormatPagingQueryFabaoNirVanaData(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("fabao_extend_his",
		"SerialNum,"		"AccountID,"		"RoleID,"				"Stage,"					"EleInjuryType,"
		"EleInjury,"		"EleResisFire,"		"EleResisWater,"		"EleResisEarth,"			"EleResisWind,"
		"EleResisBright,"	"EleResisDark,"		"DamageUpgradeTimes,"	"ResistanceUpgradeTimes"	);

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定的账号
		pStream->SetWhere();
		pStream->FillString("AccountID in (select AccountID from chose_account)");	
	}

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// 神之竞标
VOID CLoongLoongDBCtrl::FormatPagingQueryGodBid(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("god_bid", "RoleID,Bidden,BidType,BidID");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// 砸蛋
VOID CLoongLoongDBCtrl::FormatPagingQueryEggInfo(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("egg_broker",	"RoleID,"
										"CurLayer,	ColourEggPos1,	ColourEggPos2,	BrokePos1,	ItemID1,"
										"BrokePos2,	ItemID2,		BrokePos3,		ItemID3,	BrokePos4,"
										"ItemID4,	BrokePos5,		ItemID5,		BrokePos6,	ItemID6,"
										"BrokePos7,	ItemID7");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//领域
VOID CLoongLoongDBCtrl::FormatPagingQueryArea(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("area", "AreaID,RoleID,Activation,CDTime");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

//稀有领域
VOID CLoongLoongDBCtrl::FormatPagingQueryRareArea(Beton::MyStream *pStream,INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("rarearea", 
		"AreaID,"	"ItemID,"	"DataID,"	"State,"	"LeaveTime,"
		"RecordTime");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// F-Plan 1.5.0 SoulCrystal
VOID CLoongLoongDBCtrl::FormatPagingQuerySoulCrystal(Beton::MyStream *pStream,  INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("soulcrystal",		"SerialNum,"		"SoulLevel,"	"SoulQlty,"				"SoulSkillID,"	"SoulAttID1,"
		"SoulAttID2,"		"SoulAttID3,"			"SoulAttID4,"			"SoulAttID5,"		"SoulAttID6,"
		"SoulAttID7,"		"SoulAttID8");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的魂晶SoulCrystal
		pStream->FillString("  left join item using(SerialNum) ");
		pStream->SetWhere();
		pStream->FillString("AccountID in (select AccountID from chose_account)");	
	}

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// train_state
VOID CLoongLoongDBCtrl::FormatPagingQueryTrainState(Beton::MyStream *pStream,  INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("train_state",	"RoleID,"	"TrainID,"	"TrainState,"	"Completeness");

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}

// skill
VOID CLoongLoongDBCtrl::FormatPagingQuerySkill(Beton::MyStream *pStream,  INT64 n64Limit, EDBType eDBType)
{
	pStream->SetSelect("skill","RoleID,ID,BiddenLevel,SelfLevel,Proficiency,CoolDown,active_time");

	if ( eDBType == EM_DB_SRC_TWO )
	{
		// 只从源2中抽取 chose_account 中指定账号的skill数据
		pStream->FillString("  left join roledata using(RoleID) ");
		pStream->SetWhere();
		pStream->FillString("AccountID in (select AccountID from chose_account)");	
	}

	pStream->FillString(" limit ")<<n64Limit;
	pStream->FillString(",")<<DB_PAGING;//分页
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL CLoongLoongDBCtrl::FormatInsertAccountCommon(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_AccountCommon*>);
	std::vector <tagTWrap_Loong_AccountCommon*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("account_common");
		pStream->FillString("AccountID=") << (*it)->dwAccountID;
		pStream->FillString(",AccountName='").FillString((*it)->szAccountName, pCon).FillString("'");
		pStream->FillString(",SafeCodeCrc=") << (*it)->stOthers.sSafeCode.dwSafeCodeCrc;
		pStream->FillString(",ResetTime=") << (*it)->stOthers.sSafeCode.dwTimeReset;
		pStream->FillString(",BagPsdCrc=") << (*it)->stOthers.dwBagPsdCrc;
		pStream->FillString(",BaiBaoYuanBao=") << (*it)->stOthers.nBaiBaoYuanBao;
		pStream->FillString(",WareSize=") << (*it)->stOthers.n16WareSize;
		pStream->FillString(",WareSilver=") << (*it)->stOthers.n64WareSilver;
		pStream->FillString(",PersonalSet='").FillBlob(&(*it)->stOthers.stPersonalSet,sizeof(tagDWPlayerPersonalSet),pCon).FillString("'");

		//1.3.2新增
		//"LastLoginTime,	LastLogoutTime,		LastReceiveDailyOfflineRewardTime,	LastReceiveRegressionTime"	);
		pStream->FillString(",LastLoginTime=") << (*it)->timeLastLogin;
		pStream->FillString(",LastLogoutTime=") << (*it)->timeLastLogout;
		pStream->FillString(",LastReceiveDailyOfflineRewardTime=") << (*it)->timeLastReceiveDailyOfflineReward;
		pStream->FillString(",LastReceiveRegressionTime=") << (*it)->timeLastReceiveRegression;


		//1.4.2
		pStream->FillString(",EMacc=") << (BYTE)(*it)->stOthers.eSpec;
		


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}

	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
BOOL CLoongLoongDBCtrl::FormatInsertBlackList(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_BlackList*>);
	std::vector <tagTWrap_Loong_BlackList*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("blacklist");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",BlackID=") << (*it)->dwBlackID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CLoongLoongDBCtrl::FormatInsertBuff(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Buff*>);
	std::vector <tagTWrap_Loong_Buff*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("buff");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",SrcUnitID=") << (*it)->stOthers.dwSrcUnitID;
		pStream->FillString(",SrcSkillID=") << (*it)->stOthers.dwSrcSkillID;
		pStream->FillString(",ItemTypeID=") << (*it)->stOthers.dwItemTypeID;
		pStream->FillString(",ItemSerialID=") << (*it)->stOthers.n64Serial;

		pStream->FillString(",BuffID=") << (*it)->stOthers.dwBuffID;
		pStream->FillString(",CurTick=") << (*it)->stOthers.nPersistTick;
		pStream->FillString(",Level=") << (*it)->stOthers.n8Level;
		pStream->FillString(",CurLapTimes=") << (*it)->stOthers.n8CurLapTimes;

		if((*it)->stOthers.n8ModifierNum > 0)
		{
			pStream->FillString(",EffectSkillID='");
			pStream->FillBlob((*it)->pcBlob, (*it)->stOthers.n8ModifierNum * sizeof(DWORD), pCon);
			pStream->FillString("'");
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
BOOL CLoongLoongDBCtrl::FormatInsertClanData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ClanData*>);
	std::vector <tagTWrap_Loong_ClanData*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("clan_data");
		pStream->FillString("RoleID=")		<< (*it)->dwRoleID;
		pStream->FillString(",RepXuanYuan=")		<< (*it)->stOthers.m_nReputation[ECLT_XuanYuan];
		pStream->FillString(",ConXuanYuan=")	<< (*it)->stOthers.m_nContribution[ECLT_XuanYuan];
		pStream->FillString(",ActCntXuanYuan=")	<< (*it)->stOthers.m_n8ActCount[ECLT_XuanYuan];

		pStream->FillString(",RepShenNong=")	<< (*it)->stOthers.m_nReputation[ECLT_ShenNong];
		pStream->FillString(",ConShenNong=")	<< (*it)->stOthers.m_nContribution[ECLT_ShenNong];
		pStream->FillString(",ActCntShenNong=")	<< (*it)->stOthers.m_n8ActCount[ECLT_ShenNong];

		pStream->FillString(",RepFuXi=")		<< (*it)->stOthers.m_nReputation[ECLT_FuXi];
		pStream->FillString(",ConFuXi=")		<< (*it)->stOthers.m_nContribution[ECLT_FuXi];
		pStream->FillString(",ActCntFuXi=")		<< (*it)->stOthers.m_n8ActCount[ECLT_FuXi];

		pStream->FillString(",RepSanMiao=")		<< (*it)->stOthers.m_nReputation[ECLT_SanMiao];
		pStream->FillString(",ConSanMiao=")		<< (*it)->stOthers.m_nContribution[ECLT_SanMiao];
		pStream->FillString(",ActCntSanMiao=")	<< (*it)->stOthers.m_n8ActCount[ECLT_SanMiao];

		pStream->FillString(",RepJiuLi=")		<< (*it)->stOthers.m_nReputation[ECLT_JiuLi];
		pStream->FillString(",ConJiuLi=")		<< (*it)->stOthers.m_nContribution[ECLT_JiuLi];
		pStream->FillString(",ActCntJiuLi=")	<< (*it)->stOthers.m_n8ActCount[ECLT_JiuLi];

		pStream->FillString(",RepYueZhi=")		<< (*it)->stOthers.m_nReputation[ECLT_YueZhi];
		pStream->FillString(",ConYueZhi=")		<< (*it)->stOthers.m_nContribution[ECLT_YueZhi];
		pStream->FillString(",ActCntYueZhi=")	<< (*it)->stOthers.m_n8ActCount[ECLT_YueZhi];

		pStream->FillString(",RepNvWa=")		<< (*it)->stOthers.m_nReputation[ECLT_NvWa];
		pStream->FillString(",ConNvWa=")		<< (*it)->stOthers.m_nContribution[ECLT_NvWa];
		pStream->FillString(",ActCntNvWa=")		<< (*it)->stOthers.m_n8ActCount[ECLT_NvWa];

		pStream->FillString(",RepGongGong=")	<< (*it)->stOthers.m_nReputation[ECLT_GongGong];
		pStream->FillString(",ConGongGong=")	<< (*it)->stOthers.m_nContribution[ECLT_GongGong];
		pStream->FillString(",ActCntGongGong=")	<< (*it)->stOthers.m_n8ActCount[ECLT_GongGong];

		pStream->FillString(",FameMask=")		<< (*it)->stOthers.m_u16FameMask;
		pStream->FillString(",RepRstTimeStamp=")<< (*it)->stOthers.m_dwtLastResetTime;

		//1.3.1新增	"TotalFame,		FamePm,				RemoveFlag"	);
		pStream->FillString(",TotalFame=")		<< (*it)->stOthers.m_dwTotalRep;
		pStream->FillString(",FamePm=")			<< (*it)->stOthers.m_nRepPm;
		pStream->FillString(",RemoveFlag=")		<< (*it)->bRemoveFlag;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//enemy
BOOL CLoongLoongDBCtrl::FormatInsertEnemy(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Enemy*>);
	std::vector <tagTWrap_Loong_Enemy*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("enemy");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",EnemyID=") << (*it)->dwEnemyID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//武器 songg
BOOL CLoongLoongDBCtrl::FormatInsertEquip(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Equip*>);
	std::vector <tagTWrap_Loong_Equip*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert(pszTable);
		pStream->FillString("SerialNum=") << (*it)->n64SerialNum;
		pStream->FillString(",Quality=") << (*it)->stOthers.equipSpec.byQuality;
		pStream->FillString(",MinUseLevel=") << (*it)->stOthers.equipSpec.byMinUseLevel;
		pStream->FillString(",MaxUseLevel=") << (*it)->stOthers.equipSpec.byMaxUseLevel;
		pStream->FillString(",AttALimMod=") << (*it)->stOthers.equipSpec.n16AttALimMod;
		pStream->FillString(",AttALimModPct=") << (*it)->stOthers.equipSpec.n16AttALimModPct;

		pStream->FillString(",WuHun=") << (*it)->stOthers.equipSpec.n16WuHun;
		pStream->FillString(",MinDmg=") << (*it)->stOthers.equipSpec.n16MinDmg;
		pStream->FillString(",MaxDmg=") << (*it)->stOthers.equipSpec.n16MaxDmg;
		pStream->FillString(",Armor=") << (*it)->stOthers.equipSpec.n16Armor;
		pStream->FillString(",PotVal=") << (*it)->stOthers.equipSpec.nPotVal;
		pStream->FillString(",PotValUsed=") << (*it)->stOthers.equipSpec.nPotValUsed;

		pStream->FillString(",PosyTimes=") << (*it)->stOthers.equipSpec.byPosyTimes;
		pStream->FillString(",EngraveTimes=") << (*it)->stOthers.equipSpec.byEngraveTimes;
		pStream->FillString(",HoleNum=") << (*it)->stOthers.equipSpec.byHoleNum;
		pStream->FillString(",BrandLevel=") << (*it)->stOthers.equipSpec.byBrandLevel;
		pStream->FillString(",LongFuLevel=") << (*it)->stOthers.equipSpec.byLongfuLevel;

		pStream->FillString(",CanCut=") << (*it)->stOthers.equipSpec.bCanCut;
		pStream->FillString(",LongInnerID=") << (*it)->stOthers.equipSpec.dwLongHunInnerID;
		pStream->FillString(",LongOuterID=") << (*it)->stOthers.equipSpec.dwLongHunOuterID;
		pStream->FillString(",SpecAtt=") << (*it)->stOthers.equipSpec.bySpecAtt;
		pStream->FillString(",FlareVal=") << (*it)->stOthers.equipSpec.byFlareVal;
		pStream->FillString(",QltyModPct=") << (*it)->stOthers.equipSpec.n16QltyModPct;
		pStream->FillString(",QltyModPctEx=") << (*it)->stOthers.equipSpec.n16QltyModPctEx;
		pStream->FillString(",PotValModPct=") << (*it)->stOthers.equipSpec.n16PotValModPct;
			
		// 时装属性
		pStream->FillString(",Appearance=") << (*it)->stOthers.equipSpec.n16Appearance;
		pStream->FillString(",Rein=") << (*it)->stOthers.equipSpec.byRein;
		pStream->FillString(",Savvy=") << (*it)->stOthers.equipSpec.bySavvy;
		pStream->FillString(",Fortune=") << (*it)->stOthers.equipSpec.byFortune;
		pStream->FillString(",ColorID=") << (*it)->stOthers.equipSpec.n8ColorID;

		pStream->FillString(",RoleAttEffect='").FillBlob((*it)->stOthers.equipSpec.nRoleAttEffect, sizeof((*it)->stOthers.equipSpec.nRoleAttEffect), pCon);
		pStream->FillString("',PosyEffect='").FillBlob((*it)->stOthers.equipSpec.PosyEffect, sizeof((*it)->stOthers.equipSpec.PosyEffect), pCon);
		pStream->FillString("',EngraveAtt='").FillBlob((*it)->stOthers.equipSpec.nEngraveAtt, sizeof((*it)->stOthers.equipSpec.nEngraveAtt), pCon);
		pStream->FillString("',HoleGemID='").FillBlob((*it)->stOthers.equipSpec.dwHoleGemID, sizeof((*it)->stOthers.equipSpec.dwHoleGemID), pCon);
		pStream->FillString("',DerateVal='").FillBlob((*it)->stOthers.equipSpec.nBrandAtt, sizeof((*it)->stOthers.equipSpec.nBrandAtt), pCon);
		pStream->FillString("',WuXing='").FillBlob((*it)->stOthers.equipSpec.byLongfuAtt, sizeof((*it)->stOthers.equipSpec.byLongfuAtt), pCon);
		pStream->FillString("'");


		//1.3.0新增
		pStream->FillString(",PotIncTimes=") << (*it)->stOthers.equipSpec.nPotIncTimes;
		pStream->FillString(",PurpleQltyIdfPct=") << (*it)->stOthers.equipSpec.dwPurpleQltyIdfPct;

		//1.3.2 新增
		//"Signed,"		"Signature,"		"SignRoleID");
		pStream->FillString(",Signed=") << (*it)->stOthers.equipSpec.bySigned;
		pStream->FillString(",SignRoleID=") << (*it)->stOthers.equipSpec.dwSignatruePlayerID;
		pStream->FillString(",Signature='").FillString((*it)->stOthers.equipSpec.szSignature).FillString("'");

		//2.2.0新增
		pStream->FillString(",SoaringFlag=") << (*it)->stOthers.equipSpec.bSoaringFlag;
		pStream->FillString(",CastingTimes=") << (*it)->stOthers.equipSpec.nCastingTimes;

		//FPlan1.0.0
		pStream->FillString(",RoleAtt2EffectCount=") << (*it)->stOthers.equipSpec.nRoleL2AttCount;
		pStream->FillString(",ConsolidateLevel=") << (*it)->stOthers.equipSpec.byConsolidateLevel;
		pStream->FillString(",ConsolidateAttEffect='").FillBlob((*it)->stOthers.equipSpec.ConsolidateEffect,
			sizeof((*it)->stOthers.equipSpec.ConsolidateEffect), pCon);
		pStream->FillString("'");
		pStream->FillString(",LastGodStrengthenLevel=")	<< (*it)->stOthers.equipSpec.byLastGodStrengthenLevel;
		pStream->FillString(",GodStrengthenLevel=")		<< (*it)->stOthers.equipSpec.byGodStrengthenLevel;
		pStream->FillString(",CurGodStrengthenExp=")	<< (*it)->stOthers.equipSpec.dwCurGodStrengthenExp;
		pStream->FillString(",GodStrengthenTime=")		<< ((DWORD)(*it)->stOthers.equipSpec.dwGodStrengthenTime);

		// F-Plan 1.5.0
		pStream->FillString(",GrowID=") << (*it)->stOthers.equipSpec.dwGrowTypeID;
		pStream->FillString(",GrowValue=") << (*it)->stOthers.equipSpec.dwGrowValue;
		pStream->FillString(",GrowLevel=") << (*it)->stOthers.equipSpec.byGrowLevel;
		pStream->FillString(",GrowTimes=") << (*it)->stOthers.equipSpec.nTotalGrowTimes;
		pStream->FillString(",GrowUseIMTimes=") << (*it)->stOthers.equipSpec.nTotalIMUseTimes;
		pStream->FillString(",GrowStars='").FillBlob((*it)->stOthers.equipSpec.eGrowStar, sizeof((*it)->stOthers.equipSpec.eGrowStar), pCon);
		pStream->FillString("',GrowAtt='").FillBlob((*it)->stOthers.equipSpec.nGrowAttValue, sizeof((*it)->stOthers.equipSpec.nGrowAttValue), pCon);
		pStream->FillString("'");
		pStream->FillString(",DiamondLevel=")<< (*it)->stOthers.equipSpec.byDiamondLevel;


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
BOOL CLoongLoongDBCtrl::FormatInsertActClanTreasure(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ActClanTreasure*>);
	std::vector <tagTWrap_Loong_ActClanTreasure*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("famehall_act_treasure");
		pStream->FillString("TreasureID=") << (*it)->u16TreasureID;
		pStream->FillString(",ActNameID=") << (*it)->dwActNameID;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",ClanType=") << (*it)->byClanType;
		pStream->FillString(",ActTime=") << (*it)->dwActTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//氏族进入快照
BOOL CLoongLoongDBCtrl::FormatInsertFameHallEnterSnap(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FameHallEnterSnap*>);
	std::vector <tagTWrap_Loong_FameHallEnterSnap*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("famehall_entersnap");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",EnterNameID=") << (*it)->dwEnterNameID;
		pStream->FillString(",ClanType=") << (*it)->byClanType;
		pStream->FillString(",EnterTime=") << (*it)->dwEnterTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//氏族声望重置时间快照
BOOL CLoongLoongDBCtrl::FormatInsertRepRstTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_RepRstTime*>);
	std::vector <tagTWrap_Loong_RepRstTime*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("famehall_rep_rst_timestamp");
		pStream->FillString("ClanType=") << (*it)->byClanType;
		pStream->FillString(",ResetTimeStamp=") << (*it)->dwRepRstTimeStamp;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//好友
BOOL CLoongLoongDBCtrl::FormatInsertFriendSave(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Friend*>);
	std::vector <tagTWrap_Loong_Friend*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("friend");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",FriendID=") << (*it)->stOthers.dwFriendID;
		pStream->FillString(",GroupID=") << (*it)->stOthers.nGroupID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//好友度
BOOL CLoongLoongDBCtrl::FormatInsertFriendshipSave(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FriendshipSave*>);
	std::vector <tagTWrap_Loong_FriendshipSave*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("friendship");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",FriendID=") << (*it)->stOthers.dwFriendID;
		pStream->FillString(",FriVal=") << (*it)->stOthers.nFriVal;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//团购
BOOL CLoongLoongDBCtrl::FormatInsertGroupPurchase(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GroupPurchase*>);
	std::vector <tagTWrap_Loong_GroupPurchase*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("group_purchase");
		pStream->FillString("GuildID=") << (*it)->stOthers.dwGuildID;
		pStream->FillString(",RoleID=") << (*it)->stOthers.dwRoleID;
		pStream->FillString(",MallID=") << (*it)->stOthers.dwMallID;
		pStream->FillString(",ItemPrice=") << (*it)->stOthers.nPrice;
		pStream->FillString(",ParticipatorNum=") << (*it)->stOthers.nParticipatorNum;
		pStream->FillString(",RequireNum=") << (*it)->stOthers.nRequireNum;
		pStream->FillString(",RemainTime=") << (*it)->stOthers.nRemainTime;

		//响应者列表，此时只有发起人
		pStream->FillString(",ParticipatorList='");
		pStream->FillBlob((*it)->pcBlob, (*it)->dwBlobSize, pCon);
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//帮派
BOOL CLoongLoongDBCtrl::FormatInsertGuild(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Guild*>);
	std::vector <tagTWrap_Loong_Guild*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild");
		pStream->FillString("ID=") << (*it)->stOthers.dwID;

		if(_tcsclen((*it)->stOthers.szName) != 0)
		{
			pStream->FillString(",Name='").FillBlob((*it)->stOthers.szName, (_tcsclen((*it)->stOthers.szName)+1) * sizeof(TCHAR), pCon);
			pStream->FillString("'");
		}

		pStream->FillString(",FounderNameID=") << (*it)->stOthers.dwFounderNameID;
		pStream->FillString(",SpecState=") << (*it)->stOthers.dwSpecState;
		pStream->FillString(",Level=") << (*it)->stOthers.byLevel;
		pStream->FillString(",HoldCity0=") << (*it)->stOthers.byHoldCity[0];
		pStream->FillString(",HoldCity1=") << (*it)->stOthers.byHoldCity[1];
		pStream->FillString(",HoldCity2=") << (*it)->stOthers.byHoldCity[2];
		pStream->FillString(",Fund=") << (*it)->stOthers.nFund;
		pStream->FillString(",Material=") << (*it)->stOthers.nMaterial;
		pStream->FillString(",Rep=") << (*it)->stOthers.nReputation;
		pStream->FillString(",DailyCost=") << (*it)->stOthers.nDailyCost;
		pStream->FillString(",Peace=") << (*it)->stOthers.n16Peace;
		pStream->FillString(",Rank=") << (*it)->stOthers.n16Rank;

		pStream->FillString(",LeaderID=") << (*it)->stOthers.dwLeaderRoleID;
		pStream->FillString(",RemainSpreadTimes=") << (*it)->stOthers.byAffairRemainTimes;
		pStream->FillString(",Commendation=") << (*it)->stOthers.bCommendation;

		if(_tcsclen((*it)->stOthers.szTenet) != 0)
		{
			pStream->FillString(",Tenet='").FillBlob((*it)->stOthers.szTenet, (_tcsclen((*it)->stOthers.szTenet)+1) * sizeof(TCHAR), pCon);
			pStream->FillString("'");
		}

		if(_tcsclen((*it)->stOthers.szSymbolURL) != 0)
		{
			pStream->FillString(",Symbol='").FillBlob((*it)->stOthers.szSymbolURL, (_tcsclen((*it)->stOthers.szSymbolURL)+1) * sizeof(TCHAR), pCon);
			pStream->FillString("'");
		}

		pStream->FillString(",CreateTime='").FillString((*it)->szDate);
		pStream->FillString("'");
		pStream->FillString(",GroupPurchase=") << (*it)->stOthers.nGroupPurchase;

		pStream->FillString(",DailyRewardTakenTimes='") ;
		pStream->FillBlob((LPVOID)(*it)->stOthers.wGuildPosTakeRewardTimes, sizeof((*it)->stOthers.wGuildPosTakeRewardTimes),pCon);
		pStream->FillString("'") ;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//帮派成员
BOOL CLoongLoongDBCtrl::FormatInsertGuildMem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GuildMem*>);
	std::vector <tagTWrap_Loong_GuildMem*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild_member");
		pStream->FillString("RoleID=") << (*it)->stOthers.sGuildMember.dwRoleID;
		pStream->FillString(",GuildID=") << (*it)->stOthers.dwGuildID;
		pStream->FillString(",GuildPos=") << (*it)->stOthers.sGuildMember.eGuildPos;
		pStream->FillString(",TotalContrib=") << (*it)->stOthers.sGuildMember.nTotalContribution;
		pStream->FillString(",CurContrib=") << (*it)->stOthers.sGuildMember.nContribution;	
		pStream->FillString(",Exploit=") << (*it)->stOthers.sGuildMember.nExploit;
		pStream->FillString(",Salary=") << (*it)->stOthers.sGuildMember.nSalary;

		pStream->FillString(",JoinTime='").FillString((*it)->szDate);
		pStream->FillString("'");

		pStream->FillString(",CanUseGuildWare=") << (*it)->stOthers.sGuildMember.bUseGuildWare;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//item songg
BOOL CLoongLoongDBCtrl::FormatInsertItem(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Item*>);
	std::vector <tagTWrap_Loong_Item*>::iterator it = p->begin();

	CHAR szTime[X_DATATIME_LEN + 1];

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert(pszTable);
		pStream->FillString("SerialNum=") << (*it)->stOthers.n64Serial;
		pStream->FillString(",TypeID=") << (*it)->stOthers.dwTypeID;

		pStream->FillString(",Num=") << (*it)->stOthers.n16Num;
		pStream->FillString(",Bind=") << (*it)->stOthers.byBind;
		pStream->FillString(",LockStat=") << (*it)->stOthers.bLock;
		pStream->FillString(",UseTimes=") << (*it)->stOthers.nUseTimes;

		pStream->FillString(",CreateMode=") << (*it)->stOthers.eCreateMode;
		pStream->FillString(",CreateID=") << (*it)->stOthers.dwCreateID;

		pStream->FillString(",CreatorID=") << (*it)->stOthers.dwCreatorID;
		pStream->FillString(",NameID=") << (*it)->stOthers.dwNameID;

		DwordTime2DataTime(szTime, sizeof(szTime), (*it)->stOthers.dwCreateTime);
		pStream->FillString(",CreateTime='").FillString(szTime);
		pStream->FillString("'");

		DwordTime2DataTime(szTime, sizeof(szTime), (*it)->stOthers.dw1stGainTime);
		pStream->FillString(",FirstGainTime='").FillString(szTime);
		pStream->FillString("'");

		pStream->FillString(",OwnerID=") << (*it)->stOthers.dwOwnerID;
		pStream->FillString(",AccountID=") << (*it)->stOthers.dwAccountID;
		pStream->FillString(",ContainerTypeID=") << (*it)->stOthers.eConType;
		pStream->FillString(",Suffix=") << (*it)->stOthers.n16Index;

		//1.3.2 新增
		pStream->FillString(",UnlockTime=") << (*it)->stOthers.dwUnlockTime;

		// FPlan1.2.1
		pStream->FillString(",ItemSpecVal1=") << (*it)->stOthers.dwSpecVal1;
		pStream->FillString(",ItemSpecVal2=") << (*it)->stOthers.dwSpecVal2;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}

	}
	return TRUE;

}
//item cd time
BOOL CLoongLoongDBCtrl::FormatInsertItemCDTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ItemCDTime*>);
	std::vector <tagTWrap_Loong_ItemCDTime*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("item_cdtime");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;

		if((*it)->vecCDTime.size())
		{
			pStream->FillString(",CDTime='");
			pStream->FillBlob(&(*it)->vecCDTime[0], sizeof(tagCDTime) * (*it)->vecCDTime.size(), pCon);	
			pStream->FillString("'");
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//item need log
BOOL CLoongLoongDBCtrl::FormatInsertItemNeedLog(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ItemNeedLog*>);
	std::vector <tagTWrap_Loong_ItemNeedLog*>::iterator it = p->begin();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("item_needlog");
		pStream->FillString("TypeID=") << (*it)->dwTypeID;
		pStream->FillString(",NeedLog=") << (*it)->bNeedLog;
		pStream->FillString(",MinQlty=") << (*it)->sMinQlty;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//item log baibao
BOOL CLoongLoongDBCtrl::FormatInsertLogBaiBao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_LogBaiBao*>);
	std::vector <tagTWrap_Loong_LogBaiBao*>::iterator it = p->begin();

	CHAR szTime[X_DATATIME_LEN + 1];
	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("log_baibao");
		pStream->FillString("AccountID=") << (*it)->dwAccountID;
		pStream->FillString(",RoleID=") << (*it)->stOthers.dwRoleID;

		// LoongDB设置时间
		if(DwordTime2DataTime(szTime, sizeof(szTime), (*it)->stOthers.dwTime))
		{
			pStream->FillString(",LogTime='").FillString(szTime);
			pStream->FillString("'");
		}

		pStream->FillString(",n16Type=") << (*it)->stOthers.n16Type;
		pStream->FillString(",TypeID=") << (*it)->stOthers.dwTypeID;

		//赠言
		pStream->FillString(",LeaveWords='");
		if((*it)->ptcWords != NULL)
		{
			INT nLen = (_tcsclen((*it)->ptcWords)+1) * sizeof(TCHAR);
			if (nLen > 0)
			{
				pStream->FillBlob((*it)->ptcWords, (*it)->stOthers.n16Size-sizeof(tagBaiBaoRecord)+sizeof((*it)->stOthers.szWords), pCon);
			}

		}
		pStream->FillString("'");
		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
//item role del
BOOL CLoongLoongDBCtrl::FormatInsertRoleDel(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_RoleDel*>);
	std::vector <tagTWrap_Loong_RoleDel*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("role_del");
		pStream->FillString("AccountID=") << (*it)->dwAccountID;
		pStream->FillString(",RoleName='").FillString((*it)->szRoleName, pCon);
		pStream->FillString("',RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",IP='").FillString((*it)->szIP);
		pStream->FillString("',DeleteTime='").FillString((*it)->szDeleteTime).FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
//roledata 
BOOL CLoongLoongDBCtrl::FormatInsertRoleData(Beton::MyStream *pStream,LPVOID &pData,LPCSTR pszTable,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_RoleData*>);
	std::vector <tagTWrap_Loong_RoleData*>::iterator it = p->begin();

	CHAR szTime[X_DATATIME_LEN + 1];
	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		tagRoleDataConst	*pRoleDataConst = &(*it)->stOthers.RoleDataConst;
		tagRoleDataSave		*pRoleDataSave	= &(*it)->stOthers.RoleDataSave;

		pStream->SetInsert(pszTable);

		pStream->FillString("RoleName='").FillString(pRoleDataConst->szRoleName, pCon);
		pStream->FillString("',RoleNameCrc=") << pRoleDataConst->dwRoleNameCrc;
		pStream->FillString(",Sex=") << pRoleDataConst->Avatar.bySex;
		pStream->FillString(",HairModelID=") << pRoleDataConst->Avatar.wHairMdlID;
		pStream->FillString(",HairColorID=") << pRoleDataConst->Avatar.wHairTexID;
		pStream->FillString(",FaceModelID=") << pRoleDataConst->Avatar.wFaceMdlID;
		pStream->FillString(",FaceDetailID=") << pRoleDataConst->Avatar.wFaceDetailTexID;
		pStream->FillString(",DressModelID=") << pRoleDataConst->Avatar.wDressMdlID;

		pStream->FillString(",DisplaySet=") << pRoleDataSave->DisplaySet;
		pStream->FillString(",MapID=") << pRoleDataSave->dwMapID;
		pStream->FillString(",X=") << pRoleDataSave->fCoordinate[0];			// 坐标
		pStream->FillString(",Y=") << pRoleDataSave->fCoordinate[1];
		pStream->FillString(",Z=") << pRoleDataSave->fCoordinate[2];
		pStream->FillString(",FaceX=") << pRoleDataSave->fFace[0];	// 朝向
		pStream->FillString(",FaceY=") << pRoleDataSave->fFace[1];
		pStream->FillString(",FaceZ=") << pRoleDataSave->fFace[2];

		pStream->FillString(",Class=") << pRoleDataSave->eClass;
		pStream->FillString(",ClassEx=") << pRoleDataSave->eClassEx;
		pStream->FillString(",Level=") << pRoleDataSave->nLevel;
		pStream->FillString(",ExpCurLevel=") << pRoleDataSave->nCurExp;
		pStream->FillString(",HP=") << pRoleDataSave->nHP;
		pStream->FillString(",MP=") << pRoleDataSave->nMP;

		pStream->FillString(",AttPtAvail=") <<pRoleDataSave->nAttPoint;
		pStream->FillString(",TalentPtAvail=") <<pRoleDataSave->nTalentPoint;

		pStream->FillString(",PhysiqueAdded=") <<pRoleDataSave->nAttPointAdd[ERA_Physique];
		pStream->FillString(",StrengthAdded=") <<pRoleDataSave->nAttPointAdd[ERA_Strength];
		pStream->FillString(",PneumaAdded=") <<pRoleDataSave->nAttPointAdd[ERA_Pneuma];
		pStream->FillString(",InnerforceAdded=") <<pRoleDataSave->nAttPointAdd[ERA_InnerForce];
		pStream->FillString(",TechniqueAdded=") <<pRoleDataSave->nAttPointAdd[ERA_Technique];
		pStream->FillString(",AgilityAdded=") <<pRoleDataSave->nAttPointAdd[ERA_Agility];

		pStream->FillString(",Injury=") <<pRoleDataSave->nInjury;
		pStream->FillString(",Vitality=") <<pRoleDataSave->nVitality;
		pStream->FillString(",Knowledge=") <<pRoleDataSave->nKnowledge;
		pStream->FillString(",Morality=") <<pRoleDataSave->nMorality;
		pStream->FillString(",Culture=") <<pRoleDataSave->nCulture	;
		pStream->FillString(",Credit=") <<pRoleDataSave->nCredit;
		pStream->FillString(",Identity=") <<pRoleDataSave->nIdentity;
		pStream->FillString(",VIPPoint=") <<pRoleDataSave->nVIPPoint;

		pStream->FillString(",BagSize=") <<pRoleDataSave->n16BagSize;
		pStream->FillString(",BagGold=") <<pRoleDataSave->nBagGold;
		pStream->FillString(",BagSilver=") <<pRoleDataSave->nBagSilver;
		pStream->FillString(",BagYuanBao=") <<pRoleDataSave->nBagYuanBao;

		pStream->FillString(",GuildID=") <<pRoleDataSave->dwGuildID;

		if(DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataSave->LoginTime))
		{
			pStream->FillString(",LoginTime='").FillString(szTime);
			pStream->FillString("'");

			DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataSave->LogoutTime);
			pStream->FillString(",LogoutTime='").FillString(szTime);
			pStream->FillString("'");

			DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataSave->CloseSafeGuardTime);
			pStream->FillString(",CloseSGTime='").FillString(szTime);
			pStream->FillString("'");

			DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataSave->dwTimeGetMallFree);
			pStream->FillString(",GetMallFreeTime='").FillString(szTime);
			pStream->FillString("'");

			DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataConst->CreateTime);
			pStream->FillString(",CreateTime='").FillString(szTime);
			pStream->FillString("'");
		}

		pStream->FillString(",OnlineTime=") << pRoleDataSave->nOnlineTime;
		pStream->FillString(",CurOnlineTime=") << pRoleDataSave->nCurOnlineTime;

		pStream->FillString(",Rage=") <<pRoleDataSave->nRage;
		pStream->FillString(",Endurance=") <<pRoleDataSave->nEndurance;
		pStream->FillString(",SGFlag=") <<pRoleDataSave->bSafeGuard;


		pStream->FillString(",TalentType1=") <<pRoleDataSave->talent[0].eType;
		pStream->FillString(",TalentType2=") <<pRoleDataSave->talent[1].eType;
// 		pStream->FillString(",TalentType3=") <<pRoleDataSave->talent[2].eType;
// 		pStream->FillString(",TalentType4=") <<pRoleDataSave->talent[3].eType;
		pStream->FillString(",TalentVal1=") <<pRoleDataSave->talent[0].nPoint;
		pStream->FillString(",TalentVal2=") <<pRoleDataSave->talent[1].nPoint;
// 		pStream->FillString(",TalentVal3=") <<pRoleDataSave->talent[2].nPoint;
// 		pStream->FillString(",TalentVal4=") <<pRoleDataSave->talent[3].nPoint;

		pStream->FillString(",RebornMapID=") <<pRoleDataSave->dwRebornMapID;
		pStream->FillString(",TotalTax=") <<pRoleDataSave->nTotalTax;
		pStream->FillString(",Morale=") <<pRoleDataSave->nMorale;

		pStream->FillString(",RemoteOpenSet=") <<pRoleDataSave->sRemoteOpenSet;
		pStream->FillString(",CurTitleID=") <<pRoleDataSave->u16ActiveTitleID;
		pStream->FillString(",ExchangeVolume=") <<pRoleDataSave->nExVolume;

		pStream->FillString(",RoleID=") <<(*it)->dwRoleID;

		pStream->FillString(",AvatarEquip='").FillBlob(&pRoleDataSave->AvatarEquip, SIZE_AVATAR_EQUIP, pCon).FillString("'");

		pStream->FillString(",RemoveFlag=") <<(*it)->bRemoveFlag;
		pStream->FillString(",RemoveTime='").FillString((*it)->szRemoveTime);
		pStream->FillString("'");
		pStream->FillString(",AccountID=") <<(*it)->dwAccountID;

		pStream->FillString(",ScriptData='");
		pStream->FillBlob(pRoleDataSave->Data.dwData, sizeof(DWORD)*ESD_Role, pCon);
		pStream->FillString("'");

		pStream->FillString(",TreasureSum=") <<pRoleDataSave->nTreasureSum;


		pStream->FillString(",StallLevel=") <<pRoleDataSave->byStallLevel;
		pStream->FillString(",StallDailyExp=") <<pRoleDataSave->nStallDailyExp;
		pStream->FillString(",StallCurExp=") <<pRoleDataSave->nStallCurExp;
		pStream->FillString(",StallLastTime=") <<pRoleDataSave->dwLastStallTime;


		pStream->FillString(",Hostility=") << pRoleDataSave->nHostility;
		pStream->FillString(",DeadUnSetSafeGuardCountDown=") << pRoleDataSave->nDeadUnSetSafeGuardCountDown;
		pStream->FillString(",ItemTransportMapID=") << pRoleDataSave->dwItemTransportMapID;
		pStream->FillString(",ItemTransportX=") << pRoleDataSave->fItemTransportX;
		pStream->FillString(",ItemTransportZ=") << pRoleDataSave->fItemTransportZ;
		pStream->FillString(",ItemTransportY=") << pRoleDataSave->fItemTransportY;

		//1.3.0 新增
		//"Strength," 		"StrengthNum,"	"LoverID,"				"HaveWedding,"
		//"LastLessingTime,"	"LastLessingLevel,"	"NeedPrisonRevive"
		pStream->FillString(",Strength=") << pRoleDataSave->nStrength;
		pStream->FillString(",StrengthNum=") << pRoleDataSave->nStrengthNum;
		pStream->FillString(",LoverID=") << pRoleDataSave->dwLoverID;
		pStream->FillString(",HaveWedding=") << pRoleDataSave->bHaveWedding;
		pStream->FillString(",LastLessingTime=") << pRoleDataSave->timeLastLessing;
		pStream->FillString(",LastLessingLevel=") << pRoleDataSave->nLessingLevel;
		pStream->FillString(",NeedPrisonRevive=") << pRoleDataSave->bNeedPrisonRevive;

		//1.3.1 新增
		//"LevelPm,"				"EquipValue,"
		//"EquipPm,"				"LastLessingLoongDate,"			"CurrentDayOnlineTime,"			"OneDayFirstLoginedTime,""OnlineRewardPerDayRecTime,"
		//"LastUpgrade,"			"WorkedLevel,"
		pStream->FillString(",LevelPm=") << pRoleDataSave->nLevelPm;
		pStream->FillString(",EquipValue=") << pRoleDataSave->nEquipValue;
		pStream->FillString(",EquipPm=") << pRoleDataSave->nEquipPm;
		pStream->FillString(",LastLessingLoongDate=") << pRoleDataSave->dateLastLessingLoong;
		pStream->FillString(",CurrentDayOnlineTime=") << pRoleDataSave->dwCurrentDayOnlineTime;
		pStream->FillString(",OneDayFirstLoginedTime=") << pRoleDataSave->timeOneDayFirstLogined;
		pStream->FillString(",OnlineRewardPerDayRecTime=") << pRoleDataSave->timeOnlineRewardPerDayRec;
		pStream->FillString(",LastUpgrade=") << pRoleDataSave->dwLastUpgrade;
		pStream->FillString(",WorkedLevel=") << (*it)->n64WorkedLevel;

	
		//1.3.2 新增
		//"OfflineExperienceRewardFlag,"	"Spirit,"				"FixSpirit,"
		//"ConsumptiveStrength,"			"KillCount,"					"AdvanceStrength,"				"MasterID,"				"JingWuPoint,"
		//"QinWuPoint");
		pStream->FillString(",OfflineExperienceRewardFlag=") << pRoleDataSave->bIsOpenOfflineReward	;
		pStream->FillString(",Spirit=") << pRoleDataSave->nSpirit;
		pStream->FillString(",FixSpirit=") << pRoleDataSave->dwFixSpiritCD;
		pStream->FillString(",ConsumptiveStrength=") << pRoleDataSave->nConsumptiveStrength;
		pStream->FillString(",KillCount=") << 	pRoleDataSave->nKillScore;
		pStream->FillString(",AdvanceStrength=") << pRoleDataSave->nAdvanceStrength;
		pStream->FillString(",MasterID=") << pRoleDataSave->dwMasterID;
		pStream->FillString(",JingWuPoint=") << 	pRoleDataSave->nJingWuPoint;
		pStream->FillString(",QinWuPoint=") << pRoleDataSave->nQinWuPoint;

		//1.4.0新增
		pStream->FillString(",MARelation=") << pRoleDataSave->nMARelation;

		//1.5.0新增	
		pStream->FillString(",TrainStateTable= '");
		pStream->FillBlob(&(*it)->stTrainStateTable,(*it)->dwTStateTblSize, pCon);
		pStream->FillString("'");
		pStream->FillString(",WuXun=") << pRoleDataSave->nWunXun;
		pStream->FillString(",WuJi=") << pRoleDataSave->nWunJi;
		pStream->FillString(",LastLessingLevelEx=") << pRoleDataSave->nLessingLevelEx;

		//1.5.1新增
		pStream->FillString(",GodMiraclePoints =") << pRoleDataSave->nGodMiraclePoints;

		//2.0新增
		pStream->FillString(",TrainStateTransfered=") << pRoleDataSave->byTrainStatesTransfered;
		pStream->FillString(",Weary=") << pRoleDataSave->nWeary;

		//2.2新增
		pStream->FillString(",SoaringValue=") << pRoleDataSave->nSoaringValue;
		pStream->FillString(",SoaringSkillLearnTimes=") << pRoleDataSave->nSoaringSkillLearnTimes;
		pStream->FillString(",Toughness=") << pRoleDataSave->nToughness;


		//FPlan 1.0.0
		//"ConsolidateTimes,"		"CompleteRefreshTime,"			"IMRefreshTime,"				"RoleState,"			"FollowPetPocketValue,"			//"RidingPetPocketValue,"	"VipLevel,"						"VipStartTime,"					"FreeWorldTalkTimes,"	"CanRankFlag,"					//"YaoJingValue");

		pStream->FillString(",ConsolidateTimes=") << pRoleDataSave->nConsolidateTimes;
		pStream->FillString(",CompleteRefreshTime=") << pRoleDataSave->nCompleteRefreshTime;
		pStream->FillString(",IMRefreshTime=") << pRoleDataSave->nIMRefreshTime;
		pStream->FillString(",RoleState=") << pRoleDataSave->dwRoleState;
		pStream->FillString(",FollowPetPocketValue=") << pRoleDataSave->u16FollowPetPocketValve;
		pStream->FillString(",RidingPetPocketValue=") << pRoleDataSave->u16RidingPetPocketValve;
		pStream->FillString(",VipLevel=") << pRoleDataSave->nVipLevel;
		pStream->FillString(",VipStartTime=") << pRoleDataSave->dwVipStartTime;
		pStream->FillString(",FreeWorldTalkTimes=") << pRoleDataSave->n16FreeWorldTalkTimes;
		pStream->FillString(",CanRankFlag=") << pRoleDataSave->byCanRankFlag;
		pStream->FillString(",YaoJingValue=") << pRoleDataSave->nYaoJingValue;

		//FPlan 1.2.0
		//"Buy50LvlItemFlag,"		"Buy60LvlItemFlag"				"Buy70LvlItemFlag"				"Buy80LvlItemFlag,"
		pStream->FillString(",Buy50LvlItemFlag=") << pRoleDataSave->byBuy50LvlItemFlag;
		pStream->FillString(",Buy60LvlItemFlag=") << pRoleDataSave->byBuy60LvlItemFlag;
		pStream->FillString(",Buy70LvlItemFlag=") << pRoleDataSave->byBuy70LvlItemFlag;
		pStream->FillString(",Buy80LvlItemFlag=") << pRoleDataSave->byBuy80LvlItemFlag;

		//FPlan 1.2.1
		//"YListCompleteRefreshTime," "YListIMRefreshTime"
		pStream->FillString(",YListCompleteRefreshTime=") << pRoleDataSave->nYellowListCompleteRefreshTime;
		pStream->FillString(",YListIMRefreshTime=") << pRoleDataSave->nYellowListIMRefreshTime;

		//FPlan 1.2.2
		//"TakeGuildRewardTime,"			"ExpPilularUseTimes"
		pStream->FillString(",TakeGuildRewardTime=") << (DWORD)pRoleDataSave->timeTakeGuildReward;
		pStream->FillString(",ExpPilularUseTimes=") << pRoleDataSave->dwExpPilularUseTimes;

		//FPlan 1.3.0
		//PickupModeSetting
		pStream->FillString(",PickupModeSetting=") << pRoleDataSave->dwPickupModeSetting;

		//FPlan 1.3.3
		pStream->FillString(",PlayerBack=") << pRoleDataSave->byPlayerBack;
		pStream->FillString(",PlayerBackDays=") << pRoleDataSave->byPlayerBackDays;
		pStream->FillString(",VipMaxDays=") << pRoleDataSave->dwVipMaxDays;

		//FPlan 1.3.6
		pStream->FillString(",UseConstraintsMaxPKSafeGuardLevel=") << pRoleDataSave->byUsingConstraintsPKSafeGuardLevel;
		pStream->FillString(",BePlayActLayerID=") << pRoleDataSave->dwBePlayActLayerID;
		pStream->FillString(",BePlayActLevel=") << pRoleDataSave->byBePlayActLevel;

		//FPlan 1.4.0
		pStream->FillString(",GodHead=") << pRoleDataSave->nGodhead;
		pStream->FillString(",GodFaith=") << pRoleDataSave->nGodFaith;
		pStream->FillString(",GodCondenced=") << pRoleDataSave->nGodCondense;
		pStream->FillString(",Clergy=") << pRoleDataSave->dwClergy;
		pStream->FillString(",PreClergy=") << pRoleDataSave->dwPreClergy;
		pStream->FillString(",ClergyMarsPoint=") << pRoleDataSave->dwClergyMarsPoint;
		pStream->FillString(",ClergyApolloPoint=") << pRoleDataSave->dwClergyApolloPoint;
		pStream->FillString(",ClergyRabbiPoint=") << pRoleDataSave->dwClergyRabbiPoint;
		pStream->FillString(",ClergyPeacePoint=") << pRoleDataSave->dwClergyPeacePoint;
		pStream->FillString(",ClergyCandidateType=") << pRoleDataSave->eClergyCandidate;
		pStream->FillString(",Clergy4SeniorOrJunior=") << (pRoleDataSave->eSeniorOrJunior);

		// FPlan 1.4.1
		pStream->FillString(",KeyCodeRewarded=") << pRoleDataSave->dwKeyCodeRewarded;

		// FPlan 1.4.3
		pStream->FillString(",FamilyID=") << pRoleDataSave->dwFamilyID;

		// FPlan 1.4.4
		pStream->FillString(",TrainDate=") << pRoleDataSave->dwTrainDate;
		pStream->FillString(",TrainNum=") << pRoleDataSave->nTrainNum;

		// FPlan 1.4.6
		pStream->FillString(",award_point=") << pRoleDataSave->nAwardPoint;
		pStream->FillString(",award_flag=") << pRoleDataSave->nAwardFlag;

		//// FPlan 1.4.8
		//DwordTime2DataTime(szTime, sizeof(szTime), pRoleDataSave->timeBloodBrother);
		pStream->FillString(",BrotherEndTime=") << pRoleDataSave->timeBloodBrother;
		//pStream->FillString("'");

		pStream->FillString(",BrotherTeacherID=") << pRoleDataSave->dwBrotherTeatherID;

		// FPlan 1.4.11
		pStream->FillString(",PulseRemainTime=") << pRoleDataSave->nPulseLearnRemainTime;
		pStream->FillString(",TigerTime=") << pRoleDataSave->nTigerTime;

		// FPlan 1.5.0
		pStream->FillString(",god_soul=") << pRoleDataSave->nGod;
		pStream->FillString(",monster_soul=") << pRoleDataSave->nMonster;
		pStream->FillString(",god_point=") << pRoleDataSave->nGodPoint;
		pStream->FillString(",monster_point=") << pRoleDataSave->nMonsterPoint;

		//鲜花鸡蛋排行by lib
		pStream->FillString(",flower_num=") << pRoleDataSave->nFlower;
		pStream->FillString(",egg_num=") << pRoleDataSave->nEgg;


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//skill
BOOL CLoongLoongDBCtrl::FormatInsertSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Skill*>);
	std::vector <tagTWrap_Loong_Skill*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("skill");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",ID=") << (*it)->stOthers.dwID;
		pStream->FillString(",BiddenLevel=") << (*it)->stOthers.nLearnLevel;
		pStream->FillString(",SelfLevel=") << (*it)->stOthers.nSelfLevel;
		pStream->FillString(",Proficiency=") << (*it)->stOthers.nProficiency;
		pStream->FillString(",CoolDown=") << (*it)->stOthers.nCoolDown;
		pStream->FillString(",active_time=") << (*it)->stOthers.dwActiveTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//task
BOOL CLoongLoongDBCtrl::FormatInsertTask(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Task*>);
	std::vector <tagTWrap_Loong_Task*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("task");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",TaskID=") << (*it)->stOthers.u16QuestID;
		pStream->FillString(",TaskFlag=") << (*it)->stOthers.bQuestFlag;
		pStream->FillString(",StartTime=") << (*it)->stOthers.dwAcceptTime;
		pStream->FillString(",ChuandaoCount=") << (*it)->stOthers.nChuandaoCount;

		for(INT32 i=0; i<QUEST_CREATURES_COUNT; ++i)
		{
			pStream->FillString(",Monster") << i;
			pStream->FillString("Num=") << (INT16)(*it)->stOthers.n16MonsterNum[i];
		}

		pStream->FillString(",ScriptData='");
		pStream->FillBlob((*it)->stOthers.Data.dwData, sizeof(DWORD)*ESD_Quest, pCon);
		pStream->FillString("'");

		if((*it)->stOthers.DynamicTarget.eTargetType != EQTT_NULL)
		{
			pStream->FillString(",DynamicTarget='");
			pStream->FillBlob(&(*it)->stOthers.DynamicTarget, sizeof(tagQuestDynamicTarget), pCon);
			pStream->FillString("'");
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//task done
BOOL CLoongLoongDBCtrl::FormatInsertTaskDone(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_TaskDone*>);
	std::vector <tagTWrap_Loong_TaskDone*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("task_done");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",TaskID=") << (*it)->stOthers.u16QuestID;
		pStream->FillString(",StartTime=") << (*it)->stOthers.dwStartTime;
		pStream->FillString(",Times=") << (*it)->stOthers.nTimes;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//title
BOOL CLoongLoongDBCtrl::FormatInsertTitle(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Title*>);
	std::vector <tagTWrap_Loong_Title*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("title");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",ID=") << (*it)->stOthers.u16TitleID;
		pStream->FillString(",Count=") << (*it)->stOthers.dwStateMark;
		pStream->FillString(",TimeRemain =") << (*it)->stOthers.dwTimeRemain;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//名帖
BOOL CLoongLoongDBCtrl::FormatInsertVisitingCard(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_VisitingCard*>);
	std::vector <tagTWrap_Loong_VisitingCard*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("visiting_card");
		pStream->FillString("Level=") << (INT32)(*it)->stOthers.dwLevel;
		pStream->FillString(",Job=") << (INT32)(*it)->stOthers.dwJob;
		pStream->FillString(",MateRoleID=") << (INT32)(*it)->stOthers.dwMateRoleID;
		pStream->FillString(",FactionID=") << (INT32)(*it)->stOthers.dwFactionID;
		pStream->FillString(",Position=") << (INT32)(*it)->stOthers.dwPosition;

		pStream->FillString(",Visibility=") << (*it)->stOthers.customVCardData.bVisibility;
		pStream->FillString(",Sex=") << (*it)->stOthers.customVCardData.bySex;
		pStream->FillString(",Constellation=") << (*it)->stOthers.customVCardData.byConstellation;
		pStream->FillString(",ChineseZodiac=") << (*it)->stOthers.customVCardData.byChineseZodiac;
		pStream->FillString(",Area=") << (*it)->stOthers.customVCardData.byArea;
		pStream->FillString(",Province=") << (*it)->stOthers.customVCardData.byProvince;
		pStream->FillString(",City='").FillString((*it)->stOthers.customVCardData.chCity, pCon).FillString("'");
		pStream->FillString(",HeadUrl='").FillString((*it)->tcHeadUrl, pCon).FillString("'");
		pStream->FillString(",Signature='").FillString((*it)->tcSignature, pCon).FillString("'");
		pStream->FillString(",NameHistory='").FillString((*it)->tcNameHistory, pCon).FillString("'");
		pStream->FillString(",RoleID=") << (*it)->stOthers.dwRoleID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
//元宝
BOOL CLoongLoongDBCtrl::FormatInsertYBAccount(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_YBAccount*>);
	std::vector <tagTWrap_Loong_YBAccount*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("ybaccount");

		pStream->FillString("YuanBao=") <<(*it)->nYuanBao;
		pStream->FillString(",Gold=") <<(*it)->nGold;
		pStream->FillString(",Silver=") <<(*it)->nSilver;
		pStream->FillString(",bSellOrder=") <<(*it)->bSellOrder;
		pStream->FillString(",bBuyOrder=") <<(*it)->bBuyOrder;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//bill item
BOOL CLoongLoongDBCtrl::FormatInsertBillItem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_BillItem*>);
	std::vector <tagTWrap_Loong_BillItem*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("bill_item");
		pStream->FillString("token_id='").FillString((*it)->szTokenID, pCon);
		pStream->FillString("',account_id=") << (*it)->dwAccountID;
		pStream->FillString(",item_id=") << (*it)->dwItemTypeID;
		pStream->FillString(",item_num=") << (*it)->nNum;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//bill_yuanbao
BOOL CLoongLoongDBCtrl::FormatInsertBillYuanbao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_BillYuanBao*>);
	std::vector <tagTWrap_Loong_BillYuanBao*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("bill_yuanbao");
		pStream->FillString("token_id='").FillString((*it)->szTokenID, pCon);
		pStream->FillString("',account_id=") << (*it)->dwAccountID;
		pStream->FillString(",yuanbao=") << (*it)->nNum;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
//bill_yuanbao_log
BOOL CLoongLoongDBCtrl::FormatInsertBillYuanbaoLog(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_BillYuanBaoLog*>);
	std::vector <tagTWrap_Loong_BillYuanBaoLog*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("bill_yuanbao_log");
		pStream->FillString("token_id='").FillString((*it)->szTokenID, pCon);
		pStream->FillString("',account_id=") << (*it)->dwAccountID;
		pStream->FillString(",yuanbao=") << (*it)->nNum;
		pStream->FillString(",time='").FillString((*it)->szTime, pCon).FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}
//元宝交易订单
BOOL CLoongLoongDBCtrl::FormatInsertYuanBaoOrder(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_YuanBaoOrder*>);
	std::vector <tagTWrap_Loong_YuanBaoOrder*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("yuanbaoorder");

		pStream->FillString("dwID=") <<(*it)->dwID;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",OrderType=") << (*it)->eYBOType;
		pStream->FillString(",Price=") << (*it)->nPrice;
		pStream->FillString(",Number=") << (*it)->nNum;
		pStream->FillString(",OrderMode=") << (*it)->eYBOMode;
		pStream->FillString(",StartTime=") << (*it)->dwStartTime;
		pStream->FillString(",EndTime=") << (*it)->dwEndTime;
		pStream->FillString(",AvgPrice=") << (*it)->nAvgPrice;
		pStream->FillString(",DealNum=") << (*it)->nDealNum;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;

}
//pet_skill
BOOL CLoongLoongDBCtrl::FormatInsertPetSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_PetSkill*>);
	std::vector <tagTWrap_Loong_PetSkill*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("pet_skill");

		pStream->FillString("petskill_typeid=")<<(*it)->stOthers.dwTypeID;// 原型id
		pStream->FillString(",petid=") <<(*it)->dwPetID;
		pStream->FillString(",para1=") << static_cast<DWORD>((*it)->stOthers.nPara1);// 冷却计时
		pStream->FillString(",para2=") << static_cast<DWORD>((*it)->stOthers.nPara2); // 使用计时

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;

}
//pet_data
BOOL CLoongLoongDBCtrl::FormatInsertPetData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_PetData*>);
	std::vector <tagTWrap_Loong_PetData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("pet_data");
		pStream->FillString("pet_name='").FillString((*it)->stOthers.tszName, pCon).FillString("'");
		pStream->FillString(",master_id=") <<(*it)->stOthers.dwMasterID;
		pStream->FillString(",type_id=") << (*it)->stOthers.dwProtoID;
		pStream->FillString(",quality=") << (*it)->stOthers.nQuality;
		pStream->FillString(",aptitude=") << (*it)->stOthers.nAptitude;
		pStream->FillString(",potential=") << (*it)->stOthers.nPotential;
		pStream->FillString(",cur_spirit=") << (*it)->stOthers.nSpirit;
		pStream->FillString(",wuxing_energy=") << (*it)->stOthers.nWuXingEnergy;
		pStream->FillString(",talent_count=") << (*it)->stOthers.nTalentCount;
		pStream->FillString(",pet_state=") << (*it)->stOthers.byPetState;
		pStream->FillString(",cur_exp=") << (*it)->stOthers.nExpCur;
		pStream->FillString(",step=") << (*it)->stOthers.nStep;
		pStream->FillString(",grade=") << (*it)->stOthers.nGrade;
		pStream->FillString(",pet_id=") << (*it)->stOthers.dwPetID;
		pStream->FillString(",pet_lock=") << (*it)->stOthers.bLocked;
		pStream->FillString(",RemoveFlag=") << (*it)->bRemoveFlag;
		pStream->FillString(",pet_value=") << (*it)->stOthers.dwPetValue;
		pStream->FillString(",pet_pm=") << (*it)->stOthers.nPetPm;

		//1.5.0新增
		pStream->FillString(",birthday=") << (*it)->stOthers.dwBornDate;
		pStream->FillString(",live=") << (*it)->stOthers.bLive;
		pStream->FillString(",lifeadded=") << (*it)->stOthers.nAddedLife;


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;

}

//vip_stall
BOOL CLoongLoongDBCtrl::FormatInsertVipStall(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_VipStall*>);
	std::vector <tagTWrap_Loong_VipStall*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("vip_stall");
		pStream->FillString("RoleID=") << (*it)->dwOwnerID;
		pStream->FillString(",RemainTime=") <<(*it)->nRemainTime;
		pStream->FillString(",StallID=") << (*it)->byStallIndex;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;

}
//guild_commodity
BOOL CLoongLoongDBCtrl::FormatInsertGuildCommodity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GuildCommodity*>);
	std::vector <tagTWrap_Loong_GuildCommodity*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild_commodity");
		pStream->FillString("role_id=") << (*it)->stOthers.dwRoleID;
		pStream->FillString(",guild_id=") <<(*it)->dwGuildID;
		pStream->FillString(",role_level=") << (*it)->stOthers.nLevel;
		pStream->FillString(",tael=") << (*it)->stOthers.nTael;

		pStream->FillString(",goods='");
		pStream->FillBlob((*it)->stOthers.sGoodInfo, (*it)->dwGoodInfoSize, pCon);
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//guild_skill
BOOL CLoongLoongDBCtrl::FormatInsertGuildSkill(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GuildSkill*>);
	std::vector <tagTWrap_Loong_GuildSkill*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild_skill");
		pStream->FillString("guild_id=") << (*it)->dwGuildID;
		pStream->FillString(",skill_id=") <<(*it)->stOthers.dwSkillID;
		pStream->FillString(",progress=") << (*it)->stOthers.n16Progress;
		pStream->FillString(",level=") << (*it)->stOthers.nLevel;
		pStream->FillString(",researching=") << (*it)->stOthers.bResearching;

		// FPlan1.0.0
		pStream->FillString(",active=") << (*it)->stOthers.bActive;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//vip_netbar
BOOL CLoongLoongDBCtrl::FormatInsertVipBar(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_VipNetBar*>);
	std::vector <tagTWrap_Loong_VipNetBar*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("vip_netbar");
		pStream->FillString("accountid=") << (*it)->dwAccountID;
		pStream->FillString(",login_time=") <<(*it)->dwLoginTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//guild_upgrade
BOOL CLoongLoongDBCtrl::FormatInsertGuildUpgrade(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GuildFacilities*>);
	std::vector <tagTWrap_Loong_GuildFacilities*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild_upgrade");
		pStream->FillString("guild_id=") << (*it)->dwGuildID;
		pStream->FillString(",type=") <<(*it)->eType;
		pStream->FillString(",level=") << (*it)->byLevel;
		pStream->FillString(",progress=") << (*it)->n16Progress;

		for (int n=1; n<=MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			pStream->FillString(",item_type_") << n;
			pStream->FillString("=") << (*it)->dwItemTypeID[n-1];
			pStream->FillString(",item_neednum_") << n;
			pStream->FillString("=") << (*it)->nItemNeed[n-1];
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//commerce_rank
BOOL CLoongLoongDBCtrl::FormatInsertCommerceRank(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_CommerceRank*>);
	std::vector <tagTWrap_Loong_CommerceRank*>::iterator it = p->begin();

	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("commerce_rank");
		pStream->FillString("role_id=") << (*it)->stOthers.dwRoleID;
		pStream->FillString(",guild_id=") <<(*it)->dwGuildID;
		pStream->FillString(",times=") << (*it)->stOthers.nTimes;
		pStream->FillString(",tael=") << (*it)->stOthers.nTael;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//activity
BOOL CLoongLoongDBCtrl::FormatInsertActivity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ActivityData*>);
	std::vector <tagTWrap_Loong_ActivityData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("activity");
		pStream->FillString("dwID=") << (*it)->dwID;

// 		pStream->FillString(",ScriptData='");
// 		pStream->FillBlob((*it)->stAData[0].Data.dwData, sizeof(DWORD)*ESD_Activity, pCon);
// 		pStream->FillString("'");
		
		//第一个脚本数据不存储 从第二个开始
		CHAR szTmp[X_SHORT_STRING] = {0};
		for(INT nLoop = 1; nLoop<ACTIVITY_DATA_NUM; ++nLoop)
		{
			// 如果有数据
			if((*it)->bHaveData[nLoop])
			{
				sprintf(szTmp, "ScriptData%d", nLoop);
				pStream->FillString(",");
				pStream->FillString(szTmp);
				pStream->FillString("='");

				pStream->FillBlob((*it)->stAData[nLoop].Data.dwData, sizeof(DWORD)*ESD_Activity, pCon);
				pStream->FillString("'");
			}
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//留言
BOOL CLoongLoongDBCtrl::FormatInsertLeftMsg(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_LeftMsg*>);
	std::vector <tagTWrap_Loong_LeftMsg*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("left_msg");
		pStream->FillString("msg_id=") << (*it)->dwMsgID;
		pStream->FillString(",roleid=") << (*it)->dwRoleID;

		pStream->FillString(",msg_data='");
		pStream->FillBlob((*it)->pcBlob, (*it)->dwBlobSize, pCon);
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//外部链接
BOOL CLoongLoongDBCtrl::FormatInsertExternalLink(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ExternalLink*>);
	std::vector <tagTWrap_Loong_ExternalLink*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("external_links");
		pStream->FillString("LinkName='").FillString((*it)->linkName, pCon).FillString("'");
		pStream->FillString(",LinkURL='").FillString((*it)->linkValue, pCon).FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//历史改名记录
BOOL CLoongLoongDBCtrl::FormatInsertNameHistory(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_NameHistory*>);
	std::vector <tagTWrap_Loong_NameHistory*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("name_history");
		pStream->FillString("AccountID=") << (*it)->dwAccountID;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;

		for(int iLoop = 0; iLoop<10; iLoop++)
		{
			CHAR szTmp[X_SHORT_STRING] = {0};
			sprintf( szTmp, "name_%d", iLoop+1 );
			pStream->FillString(",").FillString(szTmp).FillString("='").FillString(&(*it)->szRoleName[iLoop*X_SHORT_STRING],pCon).FillString("'");
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//城市
BOOL CLoongLoongDBCtrl::FormatInsertCity(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{

	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_City*>);
	std::vector <tagTWrap_Loong_City*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("city");
		pStream->FillString("id = ")<<(*it)->stOthers.sCityAtt.dwCityID;
		pStream->FillString(",guild_id = ")<<(*it)->stOthers.sCityAtt.dwGuildID;
		pStream->FillString(",defence = ")<<(*it)->stOthers.sCityAtt.nDefence;
		pStream->FillString(",eudemon_tally = ")<<(*it)->stOthers.sCityAtt.nEudemonTally;
		pStream->FillString(",tax_rate = ")<<(*it)->stOthers.sCityAtt.nTaxRate;
		pStream->FillString(",prolificacy = ")<<(*it)->stOthers.sCityAtt.nProlificacy;

		pStream->FillString(",signup_list ='");
		pStream->FillBlob( (*it)->pcSigupList,(*it)->stOthers.nSignUpNum * sizeof(tagCitySignUp), pCon );
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//master
BOOL CLoongLoongDBCtrl::FormatInsertMaster(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{	
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Master*>);
	std::vector <tagTWrap_Loong_Master*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("master");
		pStream->FillString("dwID = ") << (*it)->dwID;
		pStream->FillString(",JingWuPoint = ") << (*it)->nJingWuPoint;
		pStream->FillString(",QinWuPoint = ") << (*it)->nQinWuPoint;
		pStream->FillString(",LevelUpBonus = ") << (*it)->nLevelUpBonus;
		pStream->FillString(",RewardNum = ") << (*it)->nRewardNum;

		pStream->FillString(",ApprenticeData ='");
		pStream->FillBlob( (*it)->sApprentice.dwData,sizeof((*it)->sApprentice.dwData), pCon );
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	
	return TRUE;
}

//short cut bar
BOOL CLoongLoongDBCtrl::FormatInsertShortCutBar(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ShortCutBar*>);
	std::vector <tagTWrap_Loong_ShortCutBar*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("shortcut_quickbar");
		pStream->FillString("RoleID = ") << (*it)->dwRoleID;

		pStream->FillString(",quickbardata ='");
		pStream->FillBlob( (*it)->QuickBarData,sizeof((*it)->QuickBarData), pCon );
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//train_state
BOOL CLoongLoongDBCtrl::FormatInsertTrainState(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_TrainState*>);
	std::vector <tagTWrap_Loong_TrainState*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("train_state");
		pStream->FillString("RoleID = ") << (*it)->dwRoleID;
		pStream->FillString(",TrainID = ") << (*it)->stOthers.dwTrainID;
		pStream->FillString(",TrainState = ") << (*it)->stOthers.eTrainState;

		// FPlan1.0.0
		pStream->FillString(",Completeness = ") << (*it)->stOthers.nCompleteness;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}
//ActiveRankData
BOOL CLoongLoongDBCtrl::FormatInsertActiveRankData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ActiveRankData*>);
	std::vector <tagTWrap_Loong_ActiveRankData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("activity_rankdata");
		pStream->FillString("role_id=") << (*it)->dwRoleId;
		pStream->FillString(",role_name='").FillString((*it)->ucRoleName);
		pStream->FillString("'");

		pStream->FillString(",role_level=") << (*it)->n16RoleLevel;
		pStream->FillString(",datatype1=") << (*it)->nDataType[0];
		pStream->FillString(",datatype2=") << (*it)->nDataType[1];
		pStream->FillString(",datatype3=") << (*it)->nDataType[2];
		pStream->FillString(",datatype4=") << (*it)->nDataType[3];
		pStream->FillString(",datatype5=") << (*it)->nDataType[4];
		pStream->FillString(",datatype6=") << (*it)->nDataType[5];
		pStream->FillString(",datatype7=") << (*it)->nDataType[6];
		pStream->FillString(",datatype8=") << (*it)->nDataType[7];

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//fabao
BOOL CLoongLoongDBCtrl::FormatInsertFabao(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Fabao*>);
	std::vector <tagTWrap_Loong_Fabao*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("fabao");
		pStream->FillString("SerialNum=") << (*it)->n64Serial;
		pStream->FillString(",NativeIntelligence=") << (*it)->stOthers.n16NativeIntelligence;
		pStream->FillString(",Stage=") << (*it)->stOthers.n16Stage;
		pStream->FillString(",PracticeValue=") << (*it)->stOthers.nPracticeValue;

		pStream->FillString(",AttTypes ='");
		pStream->FillBlob( (*it)->stOthers.eAttType,sizeof((*it)->stOthers.eAttType), pCon );
		pStream->FillString("'");

		pStream->FillString(",AttGrowings0 ='");
		pStream->FillBlob( (*it)->stOthers.nAttGrowing0,sizeof((*it)->stOthers.nAttGrowing0), pCon );
		pStream->FillString("'");

		pStream->FillString(",AttGrowings ='");
		pStream->FillBlob( (*it)->stOthers.nAttGrowing,sizeof((*it)->stOthers.nAttGrowing), pCon );
		pStream->FillString("'");

		pStream->FillString(",WuxingType=") << (*it)->stOthers.eWuxing;

		pStream->FillString(",WuxingHoles ='");
		pStream->FillBlob( (*it)->stOthers.eWuxingHole,sizeof((*it)->stOthers.eWuxingHole), pCon );
		pStream->FillString("'");

		pStream->FillString(",WuxingBeadIDs ='");
		pStream->FillBlob( (*it)->stOthers.dwWuxingBeadID,sizeof((*it)->stOthers.dwWuxingBeadID), pCon );
		pStream->FillString("'");

		pStream->FillString(",SlotAttTypes ='");
		pStream->FillBlob( (*it)->stOthers.eSlotAttType,sizeof((*it)->stOthers.eSlotAttType), pCon );
		pStream->FillString("'");

		pStream->FillString(",SlotAttValue ='");
		pStream->FillBlob( (*it)->stOthers.nSlotAttValue,sizeof((*it)->stOthers.nSlotAttValue), pCon );
		pStream->FillString("'");

		pStream->FillString(",WuxingMatchReward ='");
		pStream->FillString("'");

		pStream->FillString(",UseLevel=") << (*it)->stOthers.nUseLevel;
		pStream->FillString(",SlotNum=") << (*it)->stOthers.nSlotNum;

		pStream->FillString(",SlotLingzhuMatch=") << (*it)->stOthers.bySlotLingzhuMatch;

		pStream->FillString(",SlotMacthNdx ='");
		pStream->FillBlob( (*it)->stOthers.bySlotMacthNdx,sizeof((*it)->stOthers.bySlotMacthNdx), pCon );
		pStream->FillString("'");

		//FPlan 1.2.1 妖精心情
		pStream->FillString(",Mood=") << (*it)->stOthers.nMood;
		pStream->FillString(",MoodTicks=") << (*it)->stOthers.nMoodTicks;
		pStream->FillString(",MoodBuffID=") << (*it)->stOthers.dwMoodBuffID;
		pStream->FillString(",EleInjuryType=") << (*it)->stOthers.eEleInjury;
		pStream->FillString(",EleInjury=") << (*it)->stOthers.nEleInjury;
		pStream->FillString(",EleResisFir=") << (*it)->stOthers.nEleResistance[EER_Fire];
		pStream->FillString(",EleResisWater=") << (*it)->stOthers.nEleResistance[EER_Water];
		pStream->FillString(",EleResisEarth=") << (*it)->stOthers.nEleResistance[EER_Earth];
		pStream->FillString(",EleResisWind=") << (*it)->stOthers.nEleResistance[EER_Wind];
		pStream->FillString(",EleResisBright=") << (*it)->stOthers.nEleResistance[EER_Bright];
		pStream->FillString(",EleResisDark=") << (*it)->stOthers.nEleResistance[EER_Dark];
		pStream->FillString(",DamageUpgradeTimes=") << (*it)->stOthers.byDamageUpgradeTimes;
		pStream->FillString(",ResistanceUpgradeTimes=") << (*it)->stOthers.byResistanceUpgradeTimes;
		pStream->FillString(",FairySkill1=") << (*it)->stOthers.dwFairySkill[0];
		pStream->FillString(",FairySkill2=") << (*it)->stOthers.dwFairySkill[1];
		pStream->FillString(",FairySkill3=") << (*it)->stOthers.dwFairySkill[2];
		pStream->FillString(",FairySkill4=") << (*it)->stOthers.dwFairySkill[3];
		pStream->FillString(",FairySkill5=") << (*it)->stOthers.dwFairySkill[4];
		pStream->FillString(",UnsureSkill=") << (*it)->stOthers.dwUnsureSkill;

		//FPlan 1.2.2
		//"StoredExpOpen,"	"CurStoredExp");
		pStream->FillString(",StoredExpOpen=") << (*it)->stOthers.bStoredExpOpen;
		pStream->FillString(",CurStoredExp=") << (*it)->stOthers.nCurStoredExp;

		//FPlan  1.3.0 Jason 2010-12-6 妖精繁殖相关属性
		pStream->FillString(",FabaoStatus=") << (*it)->stOthers.eFabaoStatus;
		pStream->FillString(",StatusChangeTime=") << (*it)->stOthers.dwStatusChangeTime;
		pStream->FillString(",BirthValue=") << (*it)->stOthers.nBirthValue;
		pStream->FillString(",BirthTimes=") << (*it)->stOthers.nBirthTimes;

		pStream->FillString  (",LevelBirth='");
		pStream->FillBlob((LPVOID)(*it)->stOthers.byBirthLevel, sizeof((*it)->stOthers.byBirthLevel),pCon);
		pStream->FillString  ("'");

		pStream->FillString(",IsGivenBirthTimes=") << (*it)->stOthers.byIsGivenBirthTimes;

		pStream->FillString(",MaxIntelligence=") << (*it)->stOthers.byMaxIntelligence;
		

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}


//ForceBreakOutApprentice
BOOL CLoongLoongDBCtrl::FormatInsertForceBreakOutApprentice(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>);
	std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{	
		pStream->SetInsert("force_break_out_apprentice");
		pStream->FillString("RoleID=") << (*it)->dwApprenticeID;
		pStream->FillString(",MasterID=") << (*it)->dwMasterID;
		pStream->FillString(",BreakoutInsertTime=") << (*it)->BreakoutInsertTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// 任务板数据
BOOL CLoongLoongDBCtrl::FormatInsertBoardQuest(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_BoardQuestSaveData*>);
	std::vector <tagTWrap_Sheng_BoardQuestSaveData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("quest_board");
		pStream->FillString("BoardID=") << (*it)->u16BoardID;
		pStream->FillString(",State=") << (*it)->eState;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",QuestID=") << (*it)->u16QuestID;
		pStream->FillString(",BoardType=") << (*it)->byType;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//GuildWarDeclare
BOOL CLoongLoongDBCtrl::FormatInsertGuildWarDeclare(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_GuildWarDeclare*>);
	std::vector <tagTWrap_Sheng_GuildWarDeclare*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("guild_war_declare");
		pStream->FillString("AttackGuildID=") << (*it)->dwAttackGuildID;
		pStream->FillString(",DefenceGuildID=") << (*it)->dwDefenceGuildID;
		pStream->FillString(",WarDeclareTime=") << (*it)->dwWarDeclareTime;

		INT nWarEnded = (*it)->bWarEnded?1:0;
		pStream->FillString(",WarEnded=") << nWarEnded;


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}


// 角色在线时间统计
BOOL CLoongLoongDBCtrl::FormatInsertCurMonthOnlineTime(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>);
	std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>::iterator it = p->begin();

	CHAR szTime[X_DATATIME_LEN + 1];
	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("role_online_time_each_month");
		pStream->FillString("AccountID=") << (*it)->dwAccountID;
		pStream->FillString(",RoleID=") << (*it)->dwRoleID;

		pStream->FillString(",January=") << (*it)->nJanuary;
		pStream->FillString(",February=") << (*it)->nFebruary;
		pStream->FillString(",March=") << (*it)->nMarch;
		pStream->FillString(",April=") << (*it)->nApril;
		pStream->FillString(",May=") << (*it)->nMay;
		pStream->FillString(",June=") << (*it)->nJune;
		pStream->FillString(",July=") << (*it)->nJuly;
		pStream->FillString(",August=") << (*it)->nAugust;
		pStream->FillString(",September=") << (*it)->nSeptember;
		pStream->FillString(",October=") << (*it)->nOctober;
		pStream->FillString(",November=") << (*it)->nNovember;
		pStream->FillString(",December=") << (*it)->nDecember;
		pStream->FillString(",Year=") << (*it)->byYear;

		// FPlan1.2.2
		DwordTime2DataTime(szTime, sizeof(szTime), (*it)->dwLastUpdateTm);
		pStream->FillString(",LastUpdateTime='").FillString(szTime);
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// 妖精重生
BOOL CLoongLoongDBCtrl::FormatInsertFabaoNirVanaData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_FabaoNirVanaData*>);
	std::vector <tagTWrap_Sheng_FabaoNirVanaData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("fabao_extend_his");	
		pStream->FillString("SerialNum = ") << (*it)->n64ID;
		pStream->FillString(",AccountID = ") << (*it)->dwAccountID;
		pStream->FillString(",RoleID = ") << (*it)->dwRoleID;
		pStream->FillString(",Stage = ") << (*it)->data.nStage;
		pStream->FillString(",EleInjuryType = ") << (*it)->data.nEleInjuryType;
		pStream->FillString(",EleInjury = ") << (*it)->data.nEleInjury;
		pStream->FillString(",EleResisFire = ") << (*it)->data.nEleResistance[EER_Fire];
		pStream->FillString(",EleResisWater = ") << (*it)->data.nEleResistance[EER_Water];
		pStream->FillString(",EleResisEarth = ") << (*it)->data.nEleResistance[EER_Earth];
		pStream->FillString(",EleResisWind = ") << (*it)->data.nEleResistance[EER_Wind];
		pStream->FillString(",EleResisBright = ") << (*it)->data.nEleResistance[EER_Bright];
		pStream->FillString(",EleResisDark = ") << (*it)->data.nEleResistance[EER_Dark];
		pStream->FillString(",DamageUpgradeTimes = ") << (*it)->data.byDamageUpgradeTimes;
		pStream->FillString(",ResistanceUpgradeTimes = ") << (*it)->data.byResistanceUpgradeTimes;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// 神之竞标
BOOL CLoongLoongDBCtrl::FormatInsertGodBid(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_GodBid*>);
	std::vector <tagTWrap_Sheng_GodBid*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("god_bid");	
		pStream->FillString("RoleID=")<<(*it)->dwRoleID;
		pStream->FillString(",Bidden=")<<(*it)->n64Bidden;
		pStream->FillString(",BidType=")<<(*it)->byBidType;
		pStream->FillString(",BidID=")<<(*it)->dwBidID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//砸蛋信息
BOOL CLoongLoongDBCtrl::FormatInsertEggInfo(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_EggInfo*>);
	std::vector <tagTWrap_Sheng_EggInfo*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("egg_broker");	
		pStream->FillString("RoleID=")<<(*it)->dwRoleID;
		pStream->FillString(",CurLayer=")<<(*it)->stOthers.nCurLayer;
		pStream->FillString(",ColourEggPos1=")<<(*it)->stOthers.n16ColourEggPos1;
		pStream->FillString(",ColourEggPos2=")<<(*it)->stOthers.n16ColourEggPos2;
		for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
		{
			char szItemID[100] = {0};
			char szBrokePos[100] = {0};
			sprintf(szItemID, ",ItemID%d=%d", nCnt+1, (*it)->stOthers.sBrokenEggInfo[nCnt].dwItemID);
			sprintf(szBrokePos, ",BrokePos%d=%d", nCnt+1, (*it)->stOthers.sBrokenEggInfo[nCnt].nBrokePos);
			pStream->FillString((const char*)szItemID);
			pStream->FillString((const char*)szBrokePos);
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

//衣橱
BOOL CLoongLoongDBCtrl::FormatInsertWardrobe(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_Wardrobe*>);
	std::vector <tagTWrap_Sheng_Wardrobe*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{


		pStream->SetInsert("wardrobe");
		pStream->FillString("RoleID=")<<(*it)->dwRoleID;
		pStream->FillString(",LayerID=")<<(*it)->dwLayerID;
		pStream->FillString(",Level=")<<(*it)->byLevel;
		pStream->FillString(",State=")<<(*it)->byState;
		pStream->FillString(",PlayActStartTime=")<<(*it)->dwPlayActStartTime;
		pStream->FillString(",PlayActFriendID=")<<(*it)->dwFriendID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// 领域
BOOL CLoongLoongDBCtrl::FormatInsertArea(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_Area*>);
	std::vector <tagTWrap_Sheng_Area*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("area");
		pStream->FillString("RoleID=") << (*it)->dwRoleID;
		pStream->FillString(",AreaID=") << (*it)->dwAreaID;
		pStream->FillString(",Activation=") << (*it)->bActivation;
		pStream->FillString(",CDTime=") << (*it)->dwCD;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// 稀有领域
BOOL CLoongLoongDBCtrl::FormatInsertRareArea(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Sheng_RareArea*>);
	std::vector <tagTWrap_Sheng_RareArea*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("rarearea");
		pStream->FillString("ItemID=") << (*it)->dwID;
		pStream->FillString(",AreaID=") << (*it)->dwAreaID;
		pStream->FillString(",DataID=") << (*it)->n64ID;
		pStream->FillString(",State=") << (INT)(*it)->eType;
		pStream->FillString(",LeaveTime=") << (*it)->nTime;
		pStream->FillString(",RecordTime=") << (*it)->dwRecordTime;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// F-Plan 1.5.0  SoulCrystal
BOOL CLoongLoongDBCtrl::FormatInsertSoulCrystal(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)

{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_SoulCrystal*>);
	std::vector <tagTWrap_Loong_SoulCrystal*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("soulcrystal");
		pStream->FillString("SerialNum=") << (*it)->n64SerialNum;
		pStream->FillString(",SoulLevel=") << (*it)->stOthers.SoulSpec.bySoulCrystalLvl;
		pStream->FillString(",SoulQlty=") << (*it)->stOthers.SoulSpec.bySoulCrystalQlty;
		pStream->FillString(",SoulSkillID=") << (*it)->stOthers.SoulSpec.dwStaticSkillID;
		for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d="), i+1);
			pStream->FillString(szTmp)<<(*it)->stOthers.SoulSpec.nSoulAttID[i];
		}

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

// F-Plan 1.5.0  account_reactive, account_reactiver
BOOL CLoongLoongDBCtrl::FormatInsertAccountReactive(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_AccountReactive*>);
	std::vector <tagTWrap_Loong_AccountReactive*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("account_reactive");
		pStream->FillString("accountid=") << (*it)->dwAccountID;
		pStream->FillString(",keycode=") << (*it)->stOthers.dwKey;
		pStream->FillString(",reward=") << (*it)->stOthers.nScore;
		pStream->FillString(",rewardlevel=") << (*it)->stOthers.byRewardLevel;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}

BOOL CLoongLoongDBCtrl::FormatInsertAccountReactiver(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_AccountReactiver*>);
	std::vector <tagTWrap_Loong_AccountReactiver*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("account_reactiver");
		pStream->FillString("accountid=") << (*it)->dwAccountID;
		pStream->FillString(",keycode=") << (*it)->stOthers.dwKeyCode;
		pStream->FillString(",level=") << (*it)->stOthers.byLevel;
		pStream->FillString(",exp=") << (*it)->stOthers.dwExp;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			// 释放连接
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}

	// 释放连接
	m_LoongDB[eDBType].ReturnConnection(pCon);

	return TRUE;
}


//-----------------------------------------------------------------------------------------------------------
//Family
VOID CLoongLoongDBCtrl::FormatQueryFamily(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family", 
		"FamilyID,"			"FamilyName,"			"LeaderID,"	"FounderID,"		"Active,"
		"CreateTime");
}
//-----------------------------------------------------------------------------------------------------------
//FamilyMem
VOID CLoongLoongDBCtrl::FormatQueryFamilyMem(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family_member", 
		"RoleID,"	"FamilyID,"		"JoinTime,"		"IsLeader,"		"BeRewarded");
}
//-----------------------------------------------------------------------------------------------------------
//FamilyQuset
VOID CLoongLoongDBCtrl::FormatQueryFamilyQuset(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family_quest","*");
}
//-----------------------------------------------------------------------------------------------------------
//FamilyRoleSprite
VOID CLoongLoongDBCtrl::FormatQueryFamilyRoleSprite(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family_role_sprite", 
		"FamilyID,"	"SpriteID,"	"RoleName,"	"Shili," "Gongming,"
		"RoleID," "Level," "Quality");
}
//-----------------------------------------------------------------------------------------------------------
//FamilySprite
VOID CLoongLoongDBCtrl::FormatQueryFamilySprite(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family_sprite", 
		"FamilyID,"	"Level,"	"Exp,"	"Name,"	"HP,"
		"EXAttack,"	"InAttack,"	"EXDefense,"	"InDefense,"	"EXAttackDeeper,"
		"InAttackDeeper,"	"EXAttackResistance,"	"InAttackResistance,"	"Toughness,"	"CritDes,"
		"ControleffectDeepen,"	"ControleffectResistance,"	"SlowingeffectDeepen,"	"SlowingeffectResistance,"	"FixedeffectDeepen,"
		"FixedeffectResistance,"	"AgingeffectDeepen,"	"AgingeffectResistance"	);
}
//-----------------------------------------------------------------------------------------------------------
//SpriteRequirement
VOID CLoongLoongDBCtrl::FormatQuerySpriteRequirement(Beton::MyStream *pStream,EDBType eDBType)
{
	pStream->SetSelect("family_sprite_requirement", 
		"FamilyID,"	"RequirementID,"	"Complete,"	"RoleName," "Result");
}
//-----------------------------------------------------------------------------------------------------------
//Family
BOOL CLoongLoongDBCtrl::FormatInsertFamily(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_Family*>);
	std::vector <tagTWrap_Loong_Family*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("family");
		pStream->FillString("FamilyID=")<<(*it)->stFamilyAtt.sFamilyAtt.dwFamilyID;
		pStream->FillString(",FamilyName='");
		pStream->FillString((*it)->stFamilyAtt.strFamilyName).FillString("'");
		pStream->FillString(",LeaderID=")<<(*it)->stFamilyAtt.sFamilyAtt.dwLeaderID;
		pStream->FillString(",FounderID=")<<(*it)->stFamilyAtt.sFamilyAtt.dwFounderID;
		pStream->FillString(",Active=")<<(*it)->stFamilyAtt.sFamilyAtt.nActive;

		char szDate[X_DATATIME_LEN + 1]  = {0};
		ZeroMemory(szDate, sizeof(szDate));
		DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, (*it)->stFamilyAtt.sFamilyAtt.dwCreateTime);
		pStream->FillString(",CreateTime='").FillString(szDate);
		pStream->FillString("'");


		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//FamilyMem
BOOL CLoongLoongDBCtrl::FormatInsertFamilyMem(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FamilyMem*>);
	std::vector <tagTWrap_Loong_FamilyMem*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{

		pStream->SetInsert("family_member");
		pStream->FillString("FamilyID=")<<(*it)->stFamilyMem.dwFamilyID;
		pStream->FillString(",RoleID=")<<(*it)->stFamilyMem.dwRoleID;
		pStream->FillString(",BeRewarded=")<<(*it)->stFamilyMem.bRewarded;
		pStream->FillString(",IsLeader=")<<(*it)->stFamilyMem.bLeader;

		char szDate[X_DATATIME_LEN + 1]  = {0};
		ZeroMemory(szDate, sizeof(szDate));
		DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, (*it)->stFamilyMem.dwJoinTime);
		pStream->FillString(",JoinTime='").FillString(szDate);
		pStream->FillString("'");

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//FamilyQuset
BOOL CLoongLoongDBCtrl::FormatInsertFamilyQuset(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FamilyQuset*>);
	std::vector <tagTWrap_Loong_FamilyQuset*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("family_quest");
		pStream->FillString("FamilyID=") << (*it)->stFamilyQuest.dwFamilyID;
		pStream->FillString(",LastQuestProcess=") << (*it)->stFamilyQuest.nProcess;
		pStream->FillString(",IssueFlag=") << (*it)->stFamilyQuest.bIssueState;
		pStream->FillString(",QuestID1=") << (*it)->stFamilyQuest.QuestData[0].dwQuestID;
		pStream->FillString(",QuestNum1=") << (*it)->stFamilyQuest.QuestData[0].nTargetCount;
		pStream->FillString(",QuestID2=") << (*it)->stFamilyQuest.QuestData[1].dwQuestID;
		pStream->FillString(",QuestNum2=") << (*it)->stFamilyQuest.QuestData[1].nTargetCount;
		pStream->FillString(",QuestID3=") << (*it)->stFamilyQuest.QuestData[2].dwQuestID;
		pStream->FillString(",QuestNum3=") << (*it)->stFamilyQuest.QuestData[2].nTargetCount;
		pStream->FillString(",QuestID4=") << (*it)->stFamilyQuest.QuestData[3].dwQuestID;
		pStream->FillString(",QuestNum4=") << (*it)->stFamilyQuest.QuestData[3].nTargetCount;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//FamilyRoleSprite
BOOL CLoongLoongDBCtrl::FormatInsertFamilyRoleSprite(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FamilyRoleSprite*>);
	std::vector <tagTWrap_Loong_FamilyRoleSprite*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("family_role_sprite");
		pStream->FillString("SpriteID=") << (*it)->stFamilyRoleSprite.dwSpriteType;
		pStream->FillString(",RoleName='").FillString((*it)->stFamilyRoleSprite.tszRoleName).FillString("'");;
		pStream->FillString(",Shili=") << (*it)->stFamilyRoleSprite.fShili;
		pStream->FillString(",Gongming=") << (*it)->stFamilyRoleSprite.fGongming;
		pStream->FillString(",Level=") << (*it)->stFamilyRoleSprite.un2Level;
		pStream->FillString(",Quality=") << (*it)->stFamilyRoleSprite.bQuality;
		pStream->FillString(",FamilyID=") << (*it)->dwFamilyID;
		pStream->FillString(",RoleID=") << (*it)->stFamilyRoleSprite.dwRoleID;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//FamilySprite
BOOL CLoongLoongDBCtrl::FormatInsertFamilySprite(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_FamilySprite*>);
	std::vector <tagTWrap_Loong_FamilySprite*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("family_sprite");
		pStream->FillString("FamilyID=") << (*it)->stFamilySprite.dwFamilyID;
		pStream->FillString(",Level=") << (*it)->stFamilySprite.un2Level;
		pStream->FillString(",Exp=") << (*it)->stFamilySprite.n8Exp;
		pStream->FillString(",Name='").FillString((*it)->stFamilySprite.tszName).FillString("'");
		pStream->FillString(",HP=") << (*it)->stFamilySprite.nAbility[EFSA_HP];
		pStream->FillString(",EXAttack=") << (*it)->stFamilySprite.nAbility[EFSA_EXAttack];
		pStream->FillString(",InAttack=") << (*it)->stFamilySprite.nAbility[EFSA_InAttack];					
		pStream->FillString(",EXDefense=") << (*it)->stFamilySprite.nAbility[EFSA_EXDefense];					
		pStream->FillString(",InDefense=") << (*it)->stFamilySprite.nAbility[EFSA_InDefense];					
		pStream->FillString(",EXAttackDeeper=") << (*it)->stFamilySprite.nAbility[EFSA_EXAttackDeeper];			
		pStream->FillString(",InAttackDeeper=") << (*it)->stFamilySprite.nAbility[EFSA_InAttackDeeper];			
		pStream->FillString(",EXAttackResistance=") << (*it)->stFamilySprite.nAbility[EFSA_EXAttackResistance];		
		pStream->FillString(",InAttackResistance=") << (*it)->stFamilySprite.nAbility[EFSA_InAttackResistance];		
		pStream->FillString(",Toughness=") << (*it)->stFamilySprite.nAbility[EFSA_Toughness];					
		pStream->FillString(",CritDes=") << (*it)->stFamilySprite.nAbility[EFSA_CritDes];					
		pStream->FillString(",ControleffectDeepen=") << (*it)->stFamilySprite.nAbility[EFSA_ControleffectDeepen];		
		pStream->FillString(",ControleffectResistance=") << (*it)->stFamilySprite.nAbility[EFSA_ControleffectResistance];	
		pStream->FillString(",SlowingeffectDeepen=") << (*it)->stFamilySprite.nAbility[EFSA_SlowingeffectDeepen];		
		pStream->FillString(",SlowingeffectResistance=") << (*it)->stFamilySprite.nAbility[EFSA_SlowingeffectResistance];	
		pStream->FillString(",FixedeffectDeepen=") << (*it)->stFamilySprite.nAbility[EFSA_FixedeffectDeepen];			
		pStream->FillString(",FixedeffectResistance=") << (*it)->stFamilySprite.nAbility[EFSA_FixedeffectResistance];		
		pStream->FillString(",AgingeffectDeepen=") << (*it)->stFamilySprite.nAbility[EFSA_AgingeffectDeepen];			
		pStream->FillString(",AgingeffectResistance=") << (*it)->stFamilySprite.nAbility[EFSA_AgingeffectResistance];

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//SpriteRequirement
BOOL CLoongLoongDBCtrl::FormatInsertSpriteRequirement(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_SpriteRequirement*>);
	std::vector <tagTWrap_Loong_SpriteRequirement*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("family_sprite_requirement");
		pStream->FillString("FamilyID=") << (*it)->dwFamilyID;
		pStream->FillString(",RequirementID=") << (*it)->stSpriteRequirement.dwRequirementID;
		pStream->FillString(",Complete=") << (*it)->stSpriteRequirement.bComplete;
		pStream->FillString(",RoleName='").FillString((*it)->stSpriteRequirement.tszRoleName).FillString("'");
		pStream->FillString(",Result=") << (*it)->stSpriteRequirement.nRet;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//GmServerData
BOOL CLoongLoongDBCtrl::FormatInsertGmServerData(Beton::MyStream *pStream,LPVOID &pData,EDBType eDBType)
{
	MTRANS_POINTER(p, pData, std::vector <tagTWrap_Loong_GmServerData*>);
	std::vector <tagTWrap_Loong_GmServerData*>::iterator it = p->begin();

	Beton::Connection* pCon = m_LoongDB[eDBType].GetFreeConnection();
	for(;it != p->end(); ++it)
	{
		pStream->SetInsert("gm_server_data");
		pStream->FillString("data_name='MinItemSerial'");
		pStream->FillString(",data_value=") << (*it)->n64Value;

		if(!m_LoongDB[eDBType].Execute(pStream))
		{
			m_LoongDB[eDBType].ReturnConnection(pCon);
			return FALSE;
		}
	}
	m_LoongDB[eDBType].ReturnConnection(pCon);
	return TRUE;
}

