//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: 程序框架定义实现
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "Loong.h"
//-------------------------------------------------------------------------------------------------------
// 注册需要数据库操作的命令
//-------------------------------------------------------------------------------------------------------
VOID CLoong::RegisterDBCmd()
{
	m_MsgCmdMgr.Register("N_Certification",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InitWorldOK),			_T(""));
	m_MsgCmdMgr.Register("NDBC_WorldShutDown",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::WorldShutDown),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LoongWorldInitOK",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoongWorldInitOK),	_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadAllRoleInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllRoleInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadSimRole",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadSimRole),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadRole",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadRole),			_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateRole",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateRole),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DelRole",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelRole),				_T(""));
	m_MsgCmdMgr.Register("NDBC_ResumeRole",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::ResumeRole),			_T(""));
	//m_MsgCmdMgr.Register("NDBC_RecordDel",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteRecord),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveRole",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveRole),			_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveRenameInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeRoleName),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeRoleGuild",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeRoleGuild),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearRoleDressMdID",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeRoleDressMdID),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveOneRoleInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveRoleInfo),		_T(""));

	// 帐号通用属性相关
	m_MsgCmdMgr.Register("NDBC_SafeCodeSet",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SetSafeCode),			_T(""));
	m_MsgCmdMgr.Register("NDBC_SafeCodeReset",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ResetSafeCode),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SafeCodeResetCancel",(NETMSGPROC)m_Trunk.sfp2(&CLoong::CancelSafeCodeReset), _T(""));
	m_MsgCmdMgr.Register("NDBC_WareSizeUpdate",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateWareSize),		_T(""));
	m_MsgCmdMgr.Register("NDBC_WareMoneyUpdate",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateWareMoney),		_T(""));
	m_MsgCmdMgr.Register("NDBC_BaiBaoYBUpdate",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateBaiBaoYuanBao), _T(""));
	m_MsgCmdMgr.Register("NDBC_BagYBUpdate",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateBagYuanBao),	_T(""));
	m_MsgCmdMgr.Register("NDBC_BagGoldUpdate",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateBagGold),	_T(""));
	m_MsgCmdMgr.Register("NDBC_PersonalSet",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdatePersonalSet),	_T(""));

	m_MsgCmdMgr.Register("NDBC_ChangeBagPsd",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeBagPsd),		_T(""));

	// 好友
	m_MsgCmdMgr.Register("NDBC_InsertFriend",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertFriend),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateFriend",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFriend),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFriend",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFriend),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DelAllFriRel",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteAllFriRel),		_T(""));

	m_MsgCmdMgr.Register("NDBC_InsertFriVal",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertFriendship),	_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateFriVal",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFriendship),	_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFriVal",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFriendship),	_T(""));
	m_MsgCmdMgr.Register("NDBC_DelAllFriValRel",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteAllFriValRel),	_T(""));

	// 黑名单
	m_MsgCmdMgr.Register("NDBC_InsertBlack",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertBlack),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteBlack",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteBlack),			_T(""));

	m_MsgCmdMgr.Register("NDBC_InsertEnemy",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertEnemy),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteEnemy",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteEnemy),			_T(""));

	// 取所有仇敌列表中有角色ID为dwRoleID的角色的ID
	m_MsgCmdMgr.Register("NDBC_GetRoleListOfEnemy",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetRoleListOfEnemy),	_T(""));

	m_MsgCmdMgr.Register("NDBC_GetItemInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetItemInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadItemNeedLog",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadItemNeedLog),		_T(""));

	// 任务
	m_MsgCmdMgr.Register("NDBC_AcceptQuest",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::AcceptQuest),			_T(""));
	m_MsgCmdMgr.Register("NDBC_CompleteQuest",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CompleteQuest),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DailyCompleteQuest", (NETMSGPROC)m_Trunk.sfp2(&CLoong::DailyCompleteQuest),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearDailyCompleteQuest", (NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearDailyCompleteQuest),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DiscardQuest",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DiscardQuest),		_T(""));

	// 技能
	m_MsgCmdMgr.Register("NDBC_AddSkill",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertSkill),			_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveSkill",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteSkill),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateSkill",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateSkill),			_T(""));

	// 物品装备
	m_MsgCmdMgr.Register("NDBC_NewItem",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertItem),			_T(""));
	m_MsgCmdMgr.Register("NDBC_NewEquip",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertEquip),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DelItem",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelItem),				_T(""));
	m_MsgCmdMgr.Register("NDBC_DelEquip",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelEquip),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DelEquipFromRole",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelEquipFromRole),	_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateEquipAtt",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateEquipSpec),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateEquipSingleAtt",(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateEquipSingleAtt),		_T(""));
	m_MsgCmdMgr.Register("NDBC_EquipSignature",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveEquipSignature),	_T(""));
	// Jason 2010-4-14 v2.0.0 法宝相关
	m_MsgCmdMgr.Register("NDBC_NewFabao",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertFabao),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateFabaoAtt",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFabaoSpec),			_T(""));
	// 魂晶相关
	m_MsgCmdMgr.Register("NDBC_NewSoulCrystal",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertSoulCrystal),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateSoulCrystal",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateSoulCrystalSpec),			_T(""));

	// wcy 2012-03-31 圣灵相关
	m_MsgCmdMgr.Register("NDBC_NewHoly",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertHoly),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateHolyAtt",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolySpec),			_T(""));
	m_MsgCmdMgr.Register("NDBC_HolyCoValue",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolyCoValue),			_T(""));

	// wcy 2012-04-18 圣纹相关
	m_MsgCmdMgr.Register("NDBC_NewHolyEquip",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertHolyEquip),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateHolyEquipAtt",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolyEquipSpec),			_T(""));

	// 百宝袋(item_baibao表)中物品
	m_MsgCmdMgr.Register("NDBC_LoadBaiBao",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadNewItem2BaiBao),	_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadBaiBaoYuanBao",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadBaiBaoYuanBaoInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_NewBaiBaoItem",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertBaiBaoItem),	_T(""));
	m_MsgCmdMgr.Register("NDBC_NewBaiBaoEquip",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertBaiBaoEquip),	_T(""));
	m_MsgCmdMgr.Register("NDBC_DelBaiBaoItem",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelBaiBaoItem),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DelBaiBaoEquip",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelBaiBaoEquip),		_T(""));

	// 百宝袋Log信息
	m_MsgCmdMgr.Register("NDBC_AddBaiBaoLog",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertBaibaoLog),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadBaiBaoLog",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetBaibaoLogInfo),	_T(""));

	// 称号
	m_MsgCmdMgr.Register("NDBC_TitleInsert",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertTitle),			_T(""));
	m_MsgCmdMgr.Register("NDBC_TitleDelete",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveTitle),			_T(""));

	// 角色名贴
	m_MsgCmdMgr.Register("NDBC_VCardChg",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateVCard),			_T(""));
	m_MsgCmdMgr.Register("NDBC_GetOffLineVCard",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadVCard),			_T(""));
	

	// 名人堂
	m_MsgCmdMgr.Register("NDBC_FameHallInitStart",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadFameHallInit),	_T(""));
	m_MsgCmdMgr.Register("NDBC_GetRepRankList",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadRepRank),			_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertFameHallSnap", (NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertFameHallEnterSnap), _T(""));
	m_MsgCmdMgr.Register("NDBC_InsertActClanTreasure", (NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertActTreasure),_T(""));
	m_MsgCmdMgr.Register("NDBC_UpDateRepRstTime",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRepRstTime),	_T(""));

	// 元宝交易
	m_MsgCmdMgr.Register("NDBC_LoadAllYBAccount",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllYBAccount),	_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadAllOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllYBOrder),		_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertAccount",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertYBAccount),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SynAccountYB",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateAccountYB),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SynAccountSilver",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateAccountSilver), _T(""));
	m_MsgCmdMgr.Register("NDBC_SynSubmitSell",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateSubmitSell),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SynSubmitBuy",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateSubmitBuy),		_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertOrder),			_T(""));
	m_MsgCmdMgr.Register("NDBC_CompleteOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CompleteOrder),		_T(""));
	m_MsgCmdMgr.Register("NDBC_RefreshOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::RefreshOrder),		_T(""));
	m_MsgCmdMgr.Register("NDBC_GetRoleYBOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetRoleYBOrder),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteYBOrder",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteYBOrder),		_T(""));

	// 帮派属性相关
	
	m_MsgCmdMgr.Register("NDBC_LoadAllEmptyGuild",  (NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllEmptyGuild),_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadAllGuild",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllGuild),		_T(""));
	m_MsgCmdMgr.Register("NDBC_GuildInitOK",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CheckGuildInitOK),	_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateGuild",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateGuild),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DismissGuild",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DismissGuild),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeGuildTenet",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeGuildTenet),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAllGuild",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAllGuild),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveGuildAtt",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveGuildAtt),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeGuildName",    (NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeGuildName),		_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertGuildName",    (NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertGuildName),		_T(""));

	// 帮派成员属性相关
	m_MsgCmdMgr.Register("NDBC_LoadAllGuildMember",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllGuildMember),		_T(""));
	m_MsgCmdMgr.Register("NDBC_JoinGuild",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::JoinGuild),				_T(""));
	m_MsgCmdMgr.Register("NDBC_LeaveGuild",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LeaveGuild),				_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeGuildPos",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeGuildPos),			_T(""));
	m_MsgCmdMgr.Register("NDBC_GuildWarePri",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GuildWarePrivilege),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeContrib",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeMemberContribution),_T(""));
	// Jason 2010-3-11 v1.4.0
	m_MsgCmdMgr.Register("NDBC_ChangeExploit",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GuildMemberChangeExploit),_T(""));

	// 帮派仓库相关
	m_MsgCmdMgr.Register("NDBC_LoadGuildWareItems",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGuildWareItems),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveGuildWare",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveGuildWareItems),		_T(""));

	// 帮派设施相关
	m_MsgCmdMgr.Register("NDBC_LoadFacilitiesInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGuildFacilities),		_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateFacilities",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertGuildFacilities),	_T(""));
	m_MsgCmdMgr.Register("NDBC_FacilitiesUpgrade",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateGuildFacilities),	_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveAllFacilities",(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveAllFacilities),		_T(""));

	// 帮派技能相关
	m_MsgCmdMgr.Register("NDBC_LoadGuildSkillInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGuildSkillInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateGuildSkill",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertGuildSkillInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveGuildSkill",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveGuildSkillInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeResearchSkill",(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeResearchGuildSkill),_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveGuildSkill",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveGuildSkillInfo),	_T(""));

	// 帮派跑商相关
	m_MsgCmdMgr.Register("NDBC_LoadCommodity",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGuildCommerceInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadCommerceRank",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadCommerceRankInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateCommodity",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateCommodityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangeTael",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateCommodityTael),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveCommodity",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveCommodityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveCommodity",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveCommodityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SetCommendation",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SetCommendation),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateCommerceRank",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateCommerceRankInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveCommerceRank",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveCommerceRankInfo),	_T(""));

	// 宠物相关
	m_MsgCmdMgr.Register("NDBC_GetPetInfo",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetPetInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DelPetSoul",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeletePet),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdatePetSoul",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdatePet),			_T(""));
	m_MsgCmdMgr.Register("NDBC_CreatePetSoul",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreatePet),			_T(""));

	m_MsgCmdMgr.Register("NDBC_InsertPetSkill",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertPetSkill),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DelPetSkill",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelPetSkill),			_T(""));

	//m_MsgCmdMgr.Register("NDBC_PetEquip",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::PetEquip),			_T(""));
	// 商城相关
	m_MsgCmdMgr.Register("NDBC_GetAllGPInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllGPInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_AddGPInfo",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::AddGPInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveGPInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveGPInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveGuildGPInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveGuildGPInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateGPInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateGPInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateGPTime",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateGPTime),		_T(""));
    m_MsgCmdMgr.Register("NDBC_LoadLimitItemInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadLimitItemInfo),		_T(""));
	 m_MsgCmdMgr.Register("NDBC_UpdateLimitItemInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveLimitItemInfo),		_T(""));

	// VIP摊位相关
	m_MsgCmdMgr.Register("NDBC_GetAllVIPStallInfo",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetAllVIPStallInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_ApplyVIPStall",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ApplyVIPStall),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateVIPStall",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateVIPStall),		_T(""));

	m_MsgCmdMgr.Register("NDBC_UpdateVNBPlayer",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateVNBPlayer),		_T(""));
	m_MsgCmdMgr.Register("NDBC_GetVNBData",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadVNBData),			_T(""));
	m_MsgCmdMgr.Register("NDBC_Orange_AddBonusPoint",(NETMSGPROC)m_Trunk.sfp2(&CLoong::AddBonusPoint),			_T(""));

	// 开启宝箱相关
	m_MsgCmdMgr.Register("NDBC_UpdateTreasureSum",  (NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateTreasureSum),	_T(""));

	// 固定活动
	m_MsgCmdMgr.Register("NDBC_LoadActivityData",   (NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadActivityData),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveActivityData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveActivityData),	_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearMergedScriptData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearMergedScriptData),	_T(""));

	//结婚离婚
	m_MsgCmdMgr.Register("NDBC_RoleGetMarriage",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RoleGetMarriage),	_T(""));
	m_MsgCmdMgr.Register("NDBC_RoleBreakMarriage",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RoleBreakMarriage),	_T(""));


	// 城市系统
	m_MsgCmdMgr.Register("NDBC_LoadCityInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadCityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateCityInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateCityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveCityAtt",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveCityInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAppointedAtt",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateCityAtt),		_T(""));
	m_MsgCmdMgr.Register("NDBC_GetGuildClanValue",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetGuildClanValue),	_T(""));

	// 在这里注册新的函数
	//m_MsgCmdMgr.Register("",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::),			_T(""));
	
	// 实力排行榜
	m_MsgCmdMgr.Register("NDBC_InitRankings",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InitRankings),				_T(""));

	// 神职竞选相关
	m_MsgCmdMgr.Register("NDBC_CandidateRecvClergy",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeCandidateRecvFlag),		_T(""));
	m_MsgCmdMgr.Register("NDBC_RequestInitClergyRank",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadClergyRank),				_T(""));
	m_MsgCmdMgr.Register("NDBC_Ask4CandidateCount",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetClergyCandidateCount),		_T(""));
	m_MsgCmdMgr.Register("NDBC_CancelPreClergy",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CancelPreClergy),				_T(""));

	// log相关
	m_MsgCmdMgr.Register("NDBC_LogSilver",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogSilver),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogYuanBao",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogYuanBao),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogExVolume",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogExVolume),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogVIPPoint",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogVIPPoint),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogTimeStat",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogTimeStat),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogItem",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogItem),				_T(""));
	m_MsgCmdMgr.Register("NDBC_LogMallSell",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogMallSell),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogMallSellPack",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogMallSellPack),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LogItemTimes",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogItemTimes),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LogFund",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogFund),				_T(""));
	m_MsgCmdMgr.Register("NDBC_LogMaterial",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogMaterial),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LogReputation",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogReputation),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LogGodBid",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogGodBid),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LogBattlefield",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogBattlefield),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LogAccountOnlineTime",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogAccountOnlineTime),		_T(""));
	
	// 留言相关
	m_MsgCmdMgr.Register("NDBC_LoadLeftMsg",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadLeftMsg),			_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertLeftMsg",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertLeftMsg),		_T(""));
//	m_MsgCmdMgr.Register("NDBC_ReplaceLeftMsg",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateLeftMsg),		_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearLeftMsg",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearLeftMsg),		_T(""));

	// 保存玩家经验和点
	m_MsgCmdMgr.Register("NDBC_UpdateRoleAtt",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRoleAtt),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateRoleAttPoint",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRoleAttPoint),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateRoleTalentPoint",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRoleTalentPoint),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateRoleItemTransport",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRoleItemTransport),		_T(""));

	// 压力测试专用
	m_MsgCmdMgr.Register("NDBC_TestLoadRole",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::TestLoadRole),		_T(""));
	m_MsgCmdMgr.Register("NDBC_TestLoadRoleItem",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::TestLoadRoleItem),	_T(""));
	m_MsgCmdMgr.Register("NDBC_TestSaveItem",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::TestSaveItem),		_T(""));
	m_MsgCmdMgr.Register("NDBC_TestSaveEquip",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::TestSaveEquip),		_T(""));

	// Gm命令
	m_MsgCmdMgr.Register("NDBC_LogGMCmd",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogGMCmd),			_T(""));

	// 宠物
	m_MsgCmdMgr.Register("NDBC_LogPet",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogPet),				_T(""));

	// 妖精生育
	m_MsgCmdMgr.Register("NDBC_LogFairyBirth",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogFairyBirth),				_T(""));

	// 角色在线时间log
	m_MsgCmdMgr.Register("NDBC_LogRoleOnlineInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogRoleOnlineInfo),		_T(""));

	//  游戏数据log，角色log
// 	m_MsgCmdMgr.Register("NDBC_LogWorldData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogWorldData),			_T(""));
// 	m_MsgCmdMgr.Register("NDBC_RequestLogWorldData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadWorldData),			_T(""));
// 	m_MsgCmdMgr.Register("NDBC_LogRoleData",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogRolelogData),			_T(""));
// 	m_MsgCmdMgr.Register("NDBC_RequestLogRoleData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadRolelogData),			_T(""));

	//m_MsgCmdMgr.Register("NDBC_ReplaceItemCDTime", (NETMSGPROC)m_Trunk.sfp2(&CLoong::ReplaceItemCDTime), _T(""));

	//external links
	m_MsgCmdMgr.Register("NDBC_GetExternalLinks",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadExtLinks),		_T("external link"));
	//离线挂机相关处理
	m_MsgCmdMgr.Register("NDBC_UpdateAccountOfflineRewardInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::OfflineExpReward),		_T("Offline Experience"));

	//仙界修行 
	m_MsgCmdMgr.Register("NDBC_SaveMiraclePrcNum",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveMiraclePrcNum),		_T(""));

	// 师徒
	m_MsgCmdMgr.Register("NDBC_LoadMasterApprenticeData",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadMasterApprenticeData),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAllMasterApprenticeData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAllMasterApprenticeData),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveOneMasterApprenticeData",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveOneMasterApprenticeData), _T(""));
	m_MsgCmdMgr.Register("NDBC_InsertMasterApprenticeData",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertMasterApprenticeData),	_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteAnApprentice",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteAnApprentice),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadForceBreakOutApprenticeList",(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadForceBreakOutApprenticeList),	_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertForceBreakOutApprentice",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertForceBreakOutApprentice),	_T(""));
	m_MsgCmdMgr.Register("NDBC_RemoveForceBreakOutApprentice",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::RemoveForceBreakOutApprentice),	_T(""));
	m_MsgCmdMgr.Register("NDBC_ModifyApprenticeJingWuPoint",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ModifyApprenticeJingWuPoint),	_T(""));

	//竞技场
	
	m_MsgCmdMgr.Register("NDBC_SaveSerialValToDB",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveSerialVal),			    _T(""));
	m_MsgCmdMgr.Register("NDBC_SaveArenaDataToDB",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveArenaData),			    _T(""));
    m_MsgCmdMgr.Register("NDBC_GetSerialVal",				    (NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadArenaSystemSerialVal),    _T(""));

	//选秀活动
	m_MsgCmdMgr.Register("NDBC_SaveShowPlayerData",          	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveShowPlayerData),       	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveShowPlayerPoint",	        (NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveShowPlayerPoint),         _T(""));
	m_MsgCmdMgr.Register("NDBC_SaveVotePlayerPoint",		    (NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveVotePlayerPoint),	        _T(""));
	m_MsgCmdMgr.Register("NDBC_GetShowPlayerData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadShowPlayerData),			_T(""));
	m_MsgCmdMgr.Register("NDBC_GetVotePlayerData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadVotePlayerData),			_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveShowPlayerDataCount",        (NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveShowPlayerDataCount),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveVotePlayerDataCount",	    (NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveVotePlayerDataCount),		_T(""));
		
    //活动排行
    m_MsgCmdMgr.Register("NDBC_GetActiveRankData",              (NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadActivetyRankData),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveActiveRankData",	            (NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveActivityRankData),		_T(""));
	
	// Jason 2010-1-27 v1.3.2 离线奖励/玩家回归
	m_MsgCmdMgr.Register("NDBC_LoadAccountInfo",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAccountInfo),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAccountInfo",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaceAccountInfo),				_T(""));

	m_MsgCmdMgr.Register("NDBC_LockItem",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveItemLockInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_BindItem",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveItemBindInfo),			_T(""));

	//m_MsgCmdMgr.Register("NDBC_RoleSpeak",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveRoleSpeak),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveShortCut_QuickBar",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveShortCut_QuickBar),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadShortCut_QuickBar",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadShortCut_QuickBar),		_T(""));
	

	// 龙之试炼系统
	m_MsgCmdMgr.Register("NDBC_SaveOpenTrainState",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveOpenTrainState),			_T(""));

	m_MsgCmdMgr.Register("NDBC_CheckName",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::CheckName),					_T(""));

	// 易容师改变容貌和发型
	m_MsgCmdMgr.Register("NDBC_SaveAvatarFaceAndHair",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAvatarFaceAndHair),		_T(""));

	// 公会战争
	m_MsgCmdMgr.Register("NDBC_ClearExpireGuildWarDeclare",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearExpireGuildWarDeclare),  _T(""));
	m_MsgCmdMgr.Register("NDBC_InsertGuildWarDeclare",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertGuildWarDeclare),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SetGuildWarDeclareEnded",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SetGuildWarDeclareEnded),		_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadGuildWarDeclare",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGuildWarDeclare),			_T(""));

	// 战场
	m_MsgCmdMgr.Register("NDBC_LoadBattleFieldAttackSignUpInfo",(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadBattleFieldAttackSignUpInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadAllicanceGuildInfo",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllicanceGuildInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertIntoAttackSignUpList",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertIntoAttackSignUpList),	_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFromAttackSignUpList",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFromAttackSignUpList),			_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearAttackSignUpList",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearAttackSignUpList		),			_T(""));
	m_MsgCmdMgr.Register("NDBC_ModifyBidFund",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::ModifyBidFund				),			_T(""));
	m_MsgCmdMgr.Register("NDBC_InsertIntoAllianceList",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertIntoAllianceList	),			_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearAllianceList",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearAllianceList			),			_T(""));
	m_MsgCmdMgr.Register("NDBC_ConfirmAlliance",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::ConfirmAlliance			),			_T(""));

	// 统计游戏世界地图在线人数清单
	m_MsgCmdMgr.Register("NDBC_WorldMapOlInfo",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::WorldMapOnlineInfo		),			_T(""));

	m_MsgCmdMgr.Register("NDBC_SaveOnlineTimeEachMonth",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveOnlineTimeEachMonth),		_T(""));
	m_MsgCmdMgr.Register("NDBC_QuestBoardSaveAllQuest",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::QuestBoardSaveAllQuest),		_T(""));
	m_MsgCmdMgr.Register("NDBC_QuestBoardSaveOneQuest",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::QuestBoardSaveOneQuest),		_T(""));
	m_MsgCmdMgr.Register("NDBC_SyncCanRankFlag",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SyncCanRankFlag),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveMediaOnlineNum",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveMediaOnlineNum),			_T(""));

	// 1.2.1 妖精心情
	m_MsgCmdMgr.Register("NDBC_FabaoMoodChanged",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFabaoMood),				_T(""));
	// 1.2.2 妖精伤害类型转换
	m_MsgCmdMgr.Register("NDBC_UpdateFabaoEleInjuryType",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFabaoEleInjuryType),	_T(""));
	// 1.2.2 妖精重生数据保存
	m_MsgCmdMgr.Register("NDBC_UpdateFabaoNirVanaData",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFabaoNirVanaData),		_T(""));
	// 1.2.2 妖精重生数据获取
	m_MsgCmdMgr.Register("NDBC_GetFabaoNirVanaData",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::GetFabaoNirVanaData),			_T(""));

	// 神之竞标
	m_MsgCmdMgr.Register("NDBC_LoadGodBidInfo",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadGodBidInfo),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveOneGodBidInfo",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveOneGodBidInfo),			_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearGodBidInfo",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearGodBidInfo),				_T(""));

	m_MsgCmdMgr.Register("NDBC_UpdateFabaoGivenBirthTimes",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFabaoBirthTimes),		_T(""));

	// 砸金蛋
	m_MsgCmdMgr.Register("NDBC_SaveEggInfo",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveEggInfo),					_T(""));
	m_MsgCmdMgr.Register("NDBC_ClearEggInfo",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearEggInfo),				_T(""));

	m_MsgCmdMgr.Register("NDBC_LoadOneRoleInfo",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadOneRoleInfo),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveWardrobeLayerInfo",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveWardrobeLayerInfo),		_T(""));

	m_MsgCmdMgr.Register("NDBC_LoadGlobalLimitedItem",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllRareArea),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveGlobalLimitedItem",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveRareItem),				_T(""));
	m_MsgCmdMgr.Register("NDBC_UnSaveGlobalLimitedItem",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UnSaveRareItem),				_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateGlobalLimitedItem",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRareArea),				_T(""));

	//神系统领域相关
	m_MsgCmdMgr.Register("NDBC_DeleteRoleArea",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteRoleArea),			_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateRoleArea",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateRoleArea),			_T(""));

	//删除一个buff
	m_MsgCmdMgr.Register("NDBC_DelRoleOneBuff",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::DelRoleOneBuff),			_T(""));

	m_MsgCmdMgr.Register("NDBC_InsertBuff2DB", (NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertBuffDir),_T("InsertBuffDir"));

	//家族妖精
	m_MsgCmdMgr.Register("NDBC_LoadFamilySprite",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadFamilySprite),						_T(""));
	m_MsgCmdMgr.Register("NDBC_CreateFamilySprite",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateFamilySprite),						_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateFamilySprite",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFamilySprite),						_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFamilySprite",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFamilySprite),						_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAllFamilySprite",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAllFamilySprite),						_T(""));

	m_MsgCmdMgr.Register("NDBC_CreateFamilySpriteRequirement",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateFamilySpriteRequirement),			_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateFamilySpriteRequirement",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateFamilySpriteRequirement),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFamilySpriteRequirement",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFamilySpriteRequirement),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteALLFamilySpriteRequirement",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteALLFamilySpriteRequirement),		_T(""));

	
	m_MsgCmdMgr.Register("NDBC_CreateRoleSpriteData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateRoleSpriteData),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateRoleSpriteData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRoleSpriteData),		_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteRoleSpriteData",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteRoleSpriteData),		_T(""));
	
	// 家族任务相关消息处理
	m_MsgCmdMgr.Register("NDBC_RenewFamilyQuest",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::RenewFamilyQuest),			_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveFamilyQuestDetail",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveFamilyQuest),				_T(""));
	m_MsgCmdMgr.Register("NDBC_ChangFamilyQuestIssueState",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::ChangeFamilyQuestIssueState), _T(""));
	m_MsgCmdMgr.Register("NDBC_LoadFamilyQuest",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadFamilyQuest),				_T(""));
	m_MsgCmdMgr.Register("NDBC_RequestFamilyQuest",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::RequestFamilyQuest),			_T(""));
	m_MsgCmdMgr.Register("NDBC_DeleteFamilyQuest",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::DeleteFamilyQuest),			_T(""));

	m_MsgCmdMgr.Register("NDBC_CreateFamily",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::CreateFamily),				_T(""));
	m_MsgCmdMgr.Register("NDBC_DismissFamily",				(NETMSGPROC)m_Trunk.sfp2(&CLoong::DismissFamily),				_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadAllFamily",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadAllFamily),				_T(""));
	m_MsgCmdMgr.Register("NDBC_LoadFamilyMember",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadFamilyMember),	_T(""));
	m_MsgCmdMgr.Register("NDBC_FamilyInitOK",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::FamilyInitOK),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAllFamily",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAllFamily),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveFamilyAtt",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveFamilyAtt),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveFamilyActive",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveFamilyActive),		_T(""));
	m_MsgCmdMgr.Register("NDBC_JoinFamily",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::JoinFamily),					_T(""));
	m_MsgCmdMgr.Register("NDBC_LeaveFamily",					(NETMSGPROC)m_Trunk.sfp2(&CLoong::LeaveFamily),				_T(""));
	m_MsgCmdMgr.Register("NDBC_SetLeader",						(NETMSGPROC)m_Trunk.sfp2(&CLoong::SetLeader),					_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveRoleFamilyQuestRewarded",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveRoleFamilyQuestRewarded),	_T(""));

	m_MsgCmdMgr.Register("NDBC_ClearBloodBrother",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::ClearTeatherInfo),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveAccountSpec",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveAccountSpec),	_T(""));

	m_MsgCmdMgr.Register("NDBC_TigerProfit",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::LoadTigerProfit),	_T(""));
	m_MsgCmdMgr.Register("NDBC_SaveTigerProfit",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::SaveTigerProfit),	_T(""));

	m_MsgCmdMgr.Register("NDBC_AccountReactiveCode",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::AccountReactiveOrNot),	_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateRewardReactiveLevel",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateRewardReactiveLevel),	_T(""));

	//新Key码
	m_MsgCmdMgr.Register("NDBC_KeyCodeAddGift",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::OnNDBC_KeyCodeAddGift),	_T(""));
	//领取元宝代币
	m_MsgCmdMgr.Register("NDBC_ReceiveYuanBao",			(NETMSGPROC)m_Trunk.sfp2(&CLoong::OnNDBC_ReceiveYuanBao),	_T(""));
	//元神
	m_MsgCmdMgr.Register("NDBC_InsertHolySoulInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::InsertHolySoulInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateHolySoulInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolySoulInfo),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateHolySoulExpAtt",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolySoulExpAtt),		_T(""));
	m_MsgCmdMgr.Register("NDBC_UpdateHolySoulAttInfo",		(NETMSGPROC)m_Trunk.sfp2(&CLoong::UpdateHolySoulAttInfo),		_T(""));

	//元神装备强化
	m_MsgCmdMgr.Register("NDBC_LogHolyEquipCon",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogHolyEquipCon),			_T(""));

	// 角色PK杀人记Log
	m_MsgCmdMgr.Register("NDBC_LogKillrole",	(NETMSGPROC)m_Trunk.sfp2(&CLoong::LogKillrole),			_T(""));

}
