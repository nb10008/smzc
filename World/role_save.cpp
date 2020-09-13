//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_save.cpp
// author: 
// actor:
// data: 2008-11-19
// last:
// brief: 人物数据结构保存
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_log.h"
#include "../WorldDefine/currency_define.h"
#include "../ServerDefine/msg_holySoul.h"

#include "role.h"
#include "db_session.h"
#include "buff.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "family_sprite_mgr.h"
#include "IPGDragonfly.h"
#include "HolySoul.h"
//#include "ipg_session_cash.h"
//-------------------------------------------------------------------------------
// 保存玩家到DB
//-------------------------------------------------------------------------------
VOID Role::SaveToDB(BOOL bExit)
{
	if( _isnan(m_MoveData.m_vPos.x) || _isnan(m_MoveData.m_vPos.y) || _isnan(m_MoveData.m_vPos.z) )
	{
		ILOG->Write(_T("Error1!!!! Role's position Invalid, Role ID is %u, movestate is %d, nextstate is %d, startpos is <%f, %f, %f> endpos is <%f, %f, %f>\r\n\r\n"), 
			m_dwID, m_MoveData.m_eCurMove, m_MoveData.m_eNextPreMove, m_MoveData.m_vPosStart.x, m_MoveData.m_vPosStart.y, m_MoveData.m_vPosStart.z, 
			m_MoveData.m_vDest.x, m_MoveData.m_vDest.y, m_MoveData.m_vDest.z);
	}
	
	if (bExit)
	{	// 退出时保存一次当月在线时长
		//UpdateCurMonthOnlineTime(TRUE);
	}

	if(m_LogoutTime < m_LoginTime)
	{
		m_nOnlineTime += CalcTimeDiff(g_world.GetWorldTime(), m_LoginTime);
	}
	else
	{
		m_nOnlineTime += CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	}

	m_LogoutTime = g_world.GetWorldTime();
	m_nCurOnlineTime = CalcTimeDiff(m_LogoutTime, m_LoginTime);
	//m_nOnlineTime += m_nCurOnlineTime;
	CalTodayOnlineTime();

	// 计算并更新实力相关各值,同时通知客户端更新实力总值
													//
	InitStrength();									//		
													//		
// 	tagNS_RoleStrengthChange send;					//
// 	send.nStrengthValue = m_Strength.nStrength;		//
// 	SendMessage(&send, send.dwSize);				//
	//////////////////////////////////////////////////

	// 计算神职分值
	if(IsEquipTimeEquipment())
		CalClergyPoint();

	m_SaveRole.Init();
	tagNDBC_SaveRole *pSend = m_SaveRole;

	pSend->dwRoleID = m_dwID;

	// Jason 2009-12-7 神龙赐福
	if( m_timeLastLessing == 0 )
	{
		// 前8次神龙赐福，第一次，需要在玩家退出游戏世界时，保存已经在线的时间长度，放进m_timeLastLessing里面
		//  这样下次登录，才会继续走这个时间；而在普通的savepertick里面不做保存，否则可能会改变当前时间点（就是首次的时候）
		// 导致第一次时间不对（肯定多，或高于，或加倍）
		if( IsValidOfLessingLoong() && bExit )
			m_timeLastLessing = GetCurrentOnlineTime();
		//else if( m_nLessingLevelEx > 0 )
		//	m_timeLastLessing = CalTodayOnlineTime();
	}

	pSend->RoleData.timeLastLessing = m_timeLastLessing;//m_nLessingLevel > 0 ? m_timeLastLessing : 0;
	pSend->RoleData.nLessingLevel = m_nLessingLevel;
	// Jason 2010-3-12 v1.5.0
	pSend->RoleData.nLessingLevelEx = m_nLessingLevelEx;

	//--------------------------------------------------------------------------------------------------

	// 每日在线时长奖励
	pSend->RoleData.dateLastLessingLoong	= m_dateLastLessingLoong;
	pSend->RoleData.dwCurrentDayOnlineTime	= m_dwCurrentDayOnlineTime;
	pSend->RoleData.timeOneDayFirstLogined	= m_timeOneDayFirstLogined;
	pSend->RoleData.timeOnlineRewardPerDayRec		= m_timeOnlineRewardPerDayRec;
	// Jason 2010-1-10 v1.3.2 这个就不保存了，由另外的协议处理吧m_bIsOpenOfflineReward

	//--------------------------------------------------------------------------------------------------
	// Jason 2010-1-30 v1.3.2宠物栏阀
	pSend->RoleData.u16FollowPetPocketValve	= m_u16FollowPetPocketValve;
	pSend->RoleData.u16RidingPetPocketValve = m_u16RidingPetPocketValve;
	//--------------------------------------------------------------------------------------------------

	pSend->RoleData.LoginTime = m_LoginTime;
	pSend->RoleData.LogoutTime = m_LogoutTime;
	pSend->RoleData.nOnlineTime = m_nOnlineTime;
	pSend->RoleData.nCurOnlineTime = m_nCurOnlineTime;
	pSend->RoleData.dwTimeGetMallFree = m_LastGetMallFreeTime;

	// Jason 2010-4-12 
	pSend->RoleData.nGodMiraclePoints = m_nGodMiraclePoints;

	pSend->RoleData.DisplaySet = m_DisplaySet;
	pSend->RoleData.AvatarEquip = GetAvatarEquip();

	pSend->RoleData.bSpeakOff = m_bSpeakOff;
	pSend->RoleData.dwMapID = m_dwMapID;
	pSend->RoleData.dwRebornMapID = m_dwRebornMapID;
	pSend->RoleData.fCoordinate[0] = m_MoveData.m_vPos.x;
	pSend->RoleData.fCoordinate[1] = m_MoveData.m_vPos.y;
	pSend->RoleData.fCoordinate[2] = m_MoveData.m_vPos.z;
	pSend->RoleData.fFace[0] = m_MoveData.m_vFace.x;
	pSend->RoleData.fFace[1] = m_MoveData.m_vFace.y;
	pSend->RoleData.fFace[2] = m_MoveData.m_vFace.z;

	pSend->RoleData.eClass = m_eClass;
	pSend->RoleData.eClassEx = m_eClassEx;
	pSend->RoleData.nLevel = m_nLevel;
	pSend->RoleData.nCurExp = m_nCurLevelExp;
	pSend->RoleData.dwLastUpgrade = m_dwLastUpgrade;
	pSend->RoleData.nHP = GetAttValue(ERA_HP);
	pSend->RoleData.nMP = GetAttValue(ERA_MP);
	pSend->RoleData.nAttPoint = GetAttValue(ERA_AttPoint);
	pSend->RoleData.nTalentPoint = GetAttValue(ERA_TalentPoint);
	pSend->RoleData.nRage = GetAttValue(ERA_Rage);
	pSend->RoleData.nEndurance = GetAttValue(ERA_Endurance);

	IFASTCODE->MemCpy(pSend->RoleData.nAttPointAdd, m_nAttPointAdd, sizeof(m_nAttPointAdd));

	pSend->RoleData.nVitality = GetAttValue(ERA_Vitality);
	pSend->RoleData.nInjury = GetAttValue(ERA_Injury);
	pSend->RoleData.nKnowledge = GetAttValue(ERA_Knowledge);
	pSend->RoleData.nMorale = GetAttValue(ERA_Morale);
	pSend->RoleData.nMorality = GetAttValue(ERA_Morality);
	pSend->RoleData.nCulture = GetAttValue(ERA_Culture);
	pSend->RoleData.nHostility = GetAttValue(ERA_Hostility);

	//精力系统
	pSend->RoleData.nSpirit = GetAttValue(ERA_Spirit);
	pSend->RoleData.dwFixSpiritCD = m_dwFixSpiriteCDTime;

	//武勋
	pSend->RoleData.nWunXun = GetAttValue(ERA_WuXun);
	pSend->RoleData.nWunJi = GetAttValue(ERA_WuJi);

	//疲劳值
	pSend->RoleData.nWeary = GetAttValue(ERA_Weary);

	pSend->RoleData.nGodCondense = m_nAtt[ERA_God_Condenced];
	pSend->RoleData.nGodFaith		= m_nAtt[ERA_God_Faith];
	pSend->RoleData.nGodhead		= m_nAtt[ERA_God_Godhead];

	pSend->RoleData.nCredit = m_nCredit;
	pSend->RoleData.nIdentity = m_nIdentity;
	pSend->RoleData.nVIPPoint = GetCurMgr().GetVIPPoint();

	pSend->RoleData.nVipLevel = GetAttValue(ERA_VipLevel);
	pSend->RoleData.dwVipStartTime = m_dwVipStartTime;
	pSend->RoleData.dwVipMaxDays = m_dwVipMaxDays;
	pSend->RoleData.n16FreeWorldTalkTimes = m_n16FreeWorldTalkTimes;

	// 实力相关
	pSend->RoleData.nStrength = m_Strength.nStrength;
	pSend->RoleData.nEquipValue = m_Strength.nEquipValue;
	pSend->RoleData.nYaoJingValue = m_Strength.nFairy;
	pSend->RoleData.nAdvanceStrength = m_Strength.nAdvanceStrength;
	pSend->RoleData.nConsumptiveStrength = m_Strength.nConsumptiveStrength;
	pSend->RoleData.nKillScore = m_nKillScore;

	pSend->RoleData.n16BagSize = GetItemMgr().GetBagCurSize();
	pSend->RoleData.nBagGold = MSilver2DBGold(GetCurMgr().GetBagSilver());
	pSend->RoleData.nBagSilver = MSilver2DBSilver(GetCurMgr().GetBagSilver());
	INT32 nBagYuanBao = GetCurMgr().GetBagYuanBao();

	
	if(!g_world.IsUseIPGSystem())
	{
		pSend->RoleData.nBagYuanBao = nBagYuanBao;
	}
	else
	{
		pSend->RoleData.nBagYuanBao = 0;
	}
	//else
	//{
	//	g_world.WriteKroeaLog(_T("user exit, g_world.IsUseIPGSystem(). \r\n"));

	//	pSend->RoleData.nBagYuanBao = 0;

	//	if(bExit && nBagYuanBao > 0)
	//	{
	//		TCHAR tszTemp[32] = {0};
	//		wsprintf( tszTemp, _T("user exit, nBagYuanBao:%d. \r\n"),nBagYuanBao);
	//		g_world.WriteKroeaLog(tszTemp);

	//		TCHAR szName[32] ={0};
	//		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(GetID());
	//		if (!P_VALID(pRoleInfo))
	//		{
	//			_tcscpy( szName, pRoleInfo->szRoleName );
	//		}

	//		//发送到IPG
	//		//g_IpgCashSession.SendIncYuanbaoNum( GetSession()->GetSessionID(), GetID(), szName, nBagYuanBao, GetSex());

	//		IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
	//		if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
	//		{
	//			//char szUserID[USERID_LEN] = {0};
	//			////snprintf( szUserID, sizeof(szUserID), "%d", GetID() );
	//			//sprintf( szUserID, "%d", GetID() );

	//			char szUserIp[16] = {0};
	//			//snprintf( szUserIp, sizeof(szUserIp), "%d", GetSession()->GetGetIP() );
	//			sprintf( szUserIp, "%d", GetSession()->GetGetIP() );

	//			int nCash = 0;

	//			INOUT_RET ret;

	//			char szName1[32] ={0};
	//			strncpy(szName1, IUTIL->UnicodeToUnicode8(szName), _countof(szName1) - 1);
	//			szName1[32-1] = 0;

	//			g_world.WriteKroeaLog(_T("user exit, OutDFCashReq. \r\n"));

	//			pIPGDragonfly->OutDFCashReq( ret, GetSession()->GetAccount(), GetSession()->GetKrUserID(), szName1, szUserIp, 0, nBagYuanBao, GetSex(), "", FALSE );

	//			g_world.WriteKroeaLog(_T("user exit, OutDFCashReq end. \r\n"));
	//		}
	//	}
	//}

	pSend->RoleData.nExVolume = GetCurMgr().GetExchangeVolume();

	pSend->RoleData.dwGuildID = m_dwGuildID;
	pSend->RoleData.dwFamilyID = GetFamilyID();

	pSend->RoleData.bSafeGuard = (GetRoleState() & ERS_Safeguard);
	pSend->RoleData.dwRoleState = GetRoleState();
	pSend->RoleData.CloseSafeGuardTime = m_CloseSafeGuardTime;
	pSend->RoleData.nDeadUnSetSafeGuardCountDown = m_nDeadUnSetSafeGuardCountDown;
	pSend->RoleData.bNeedPrisonRevive = m_bNeedPrisonRevive;
	IFASTCODE->MemCpy(pSend->RoleData.talent, m_Talent, sizeof(pSend->RoleData.talent));

	// 对远端玩家公开选项设置
	pSend->RoleData.sRemoteOpenSet.bTitle	= GetTitleMgr()->Visibility();

	// 称号
	pSend->RoleData.u16ActiveTitleID		= GetTitleMgr()->GetActiviteTitle();

	pSend->RoleData.dwLoverID				= m_dwLoverID;
	pSend->RoleData.bHaveWedding			= m_bHaveWedding;
	pSend->RoleData.n16MiraclePracticeNum	= m_n16MiraclePracticeNum;

	pSend->RoleData.byBuy50LvlItemFlag = m_byBuy50LvlItemFlag ;
	pSend->RoleData.byBuy60LvlItemFlag = m_byBuy60LvlItemFlag ;
	pSend->RoleData.byBuy70LvlItemFlag = m_byBuy70LvlItemFlag ;
	pSend->RoleData.byBuy80LvlItemFlag = m_byBuy80LvlItemFlag ;

	pSend->RoleData.dwExpPilularUseTimes = GetExpPilularUseTimes();

	pSend->RoleData.byUsingConstraintsPKSafeGuardLevel = m_bUsingPkMinLevel;

	// 玩家脚本数据
	IFASTCODE->MemCpy(pSend->RoleData.Data.dwData, m_ScriptData.dwData, sizeof(DWORD)*ESD_Role);

	// 玩家摊位信息
	if (P_VALID(m_pStall))
	{
		m_pStall->Save2DB(&(pSend->RoleData));
	}
	
	// 玩家禁言状态
	pSend->RoleData.bSpeakOff = m_bSpeakOff;

	// 师徒相关
	pSend->RoleData.dwMasterID = m_dwMasterID;
	IFASTCODE->MemCpy(pSend->RoleData.ApprenticeData.dwData, m_ApprenticeData.dwData, sizeof(DWORD)*MAX_APPRENTICE_NUM);
	pSend->RoleData.nJingWuPoint = m_nJingWuPoint;
	pSend->RoleData.nQinWuPoint = m_nQinWuPoint;
	pSend->RoleData.nMARelation = m_nMARelation;

	// 试炼是否已经转移过数据
	pSend->RoleData.byTrainStatesTransfered = m_byTrainStateTransfered;

	// Jason 2010-5-24 v2.2.0飞升
	pSend->RoleData.nSoaringValue	= GetBaseAttValue(ERA_Soar);

	pSend->RoleData.nSoaringSkillLearnTimes	= m_nSoaringSkillLearnTimes;
	//pSend->RoleData.timeLearnSoaringSkill	= m_timeLearnSoaringSkill;

	pSend->RoleData.nToughness = GetBaseAttValue(ERA_Toughness);

	pSend->RoleData.nCompleteRefreshTime = m_pBoardQuestMgr->GetCompleteReFreshTime();
	pSend->RoleData.nYellowListCompleteRefreshTime = m_pYellowListMgr->GetCompleteReFreshTime();
	pSend->RoleData.nIMRefreshTime = m_pBoardQuestMgr->GetIMRefreshTime();
	pSend->RoleData.nYellowListIMRefreshTime = m_pYellowListMgr->GetIMRefreshTime();
	INT nBoardQuestNum = m_pBoardQuestMgr->GetRoleBoardQuest().Size();
	INT nYellowListQuestNum = m_pYellowListMgr->GetRoleBoardQuest().Size();
	pSend->RoleData.nBoardQuestNum = nBoardQuestNum+nYellowListQuestNum;
	// Jason 2010-6-12 v1.0.0
	pSend->RoleData.nConsolidateTimes = m_nConsolidateTimes;

	// 公会战场每日奖励
	pSend->RoleData.timeTakeGuildReward	= m_timeTakeGuildReward;

	// 保存拾取模式设置
	pSend->RoleData.dwPickupModeSetting = m_dwPickupModeSetting;
	// 保存玩家回归标志
	pSend->RoleData.byPlayerBack = (BYTE)m_bPlayerBack;
	pSend->RoleData.byPlayerBackDays = m_byPlayerBackDay;
	
	pSend->RoleData.dwBePlayActLayerID = GetWardrobe().GetBePlayActLayerID();
	pSend->RoleData.byBePlayActLevel = GetWardrobe().GetBePlayActLevel();

	// 竞选神职相关
	pSend->RoleData.dwClergyMarsPoint = m_dwClergyMarsPoint;
	pSend->RoleData.dwClergyApolloPoint = m_dwClergyApolloPoint;
	pSend->RoleData.dwClergyRabbiPoint = m_dwClergyRabbiPoint;
	pSend->RoleData.dwClergyPeacePoint = m_dwClergyPeacePoint;
	pSend->RoleData.eClergyCandidate = m_eClergyCandidate;
	pSend->RoleData.eSeniorOrJunior = m_eClergy4SeniorOrJunior;
	pSend->RoleData.dwClergy = m_dwClergy;
	pSend->RoleData.dwPreClergy = m_dwPrevClergy;

	// 保存身上的妖精
	SaveRoleFabaoChange();

	pSend->RoleData.dwKeyCodeRewarded = m_dwKeyCodeRewarded;

	pSend->RoleData.dwTrainDate = m_dwTrainDate;
	pSend->RoleData.nTrainNum = m_nTrainNum;
	pSend->RoleData.nAwardPoint = m_nAwardPoint;
	pSend->RoleData.nAwardFlag = m_nAwardFlag;

	pSend->RoleData.dwBrotherTeatherID = m_BrotherMgr.GetTeatherID();
	pSend->RoleData.timeBloodBrother = m_BrotherMgr.GetEndTime();
	pSend->RoleData.nPulseLearnRemainTime = m_nPulseLearnTimes;
	pSend->RoleData.nTigerTime = m_nTigerTimes;

	pSend->RoleData.nGod = m_nGod;
	pSend->RoleData.nMonster = m_nMonster;
	pSend->RoleData.nGodPoint = m_nGodPoint;
	pSend->RoleData.nMonsterPoint = m_nMonsterPoint;

	pSend->RoleData.nFlower = GetAttValue(ERA_FlowerNum);
	pSend->RoleData.nEgg = GetAttValue(ERA_EggsNum);
	pSend->RoleData.nHolyValue = GetAttValue(ERA_Holy);

	/*********************************************************************************************************
	*技能列表，状态列表，称号列表，物品列表，装备列表，好友列表，仇敌列表，任务列表，已完成任务列表，
	*物品冷却时间表，已开启试炼状态表
	*必须按顺序
	*********************************************************************************************************/
	LPVOID pData = pSend->RoleData.byData;

	// 1.技能列表
	pSend->RoleData.nSkillNum		= 0;
	SaveSkill2DB(pData, pData, pSend->RoleData.nSkillNum);

	// 2.状态列表
	pSend->RoleData.nBuffNum		= 0;
	SaveBuff2DB(pData, pData, pSend->RoleData.nBuffNum);

	// 3.称号条件列表
	pSend->RoleData.nTitleNum		= 0;
	GetTitleMgr()->SaveTitlesToDB(pData, pData, pSend->RoleData.nTitleNum);

	// 4.物品列表 5.装备列表
	pSend->RoleData.nItemNum		= 0;
	GetItemMgr().SaveItem2DB(pData, pData, pSend->RoleData.nItemNum, g_FamilySpriteMgr.FindRoleFamilySprite( GetFamilyID(), GetID()) );
	

	// 6.好友列表 -- 存: 实时,即单独发消息
	pSend->RoleData.nFriendNum		= 0;

	// 7.仇敌列表 -- 存: 实时,即单独发消息
	pSend->RoleData.nEnemyNum		= 0;

	// 8.任务列表
	pSend->RoleData.nQuestNum		= 0;
	SaveQuest2DB(pData, pData, pSend->RoleData.nQuestNum);

	// 9.已完成任务列表 -- 存: 实时,即单独发消息
	pSend->RoleData.nQuestDoneNum	= 0;

	// 10.物品冷却时间
	pSend->RoleData.nItemCDTimeNum	= 0;
	GetItemMgr().SaveCDTime2DB(pData, pData, pSend->RoleData.nItemCDTimeNum);

	// 11.氏族数据
	m_ClanData.Save2DB(pData, pData, pSend->RoleData.bClanDataChg);

	// 12.宠物
	GetPetPocket()->SaveToDB(pData, pData, pSend->RoleData.nPetsNum);

	// 13.师徒
	SaveMaster2DB(pData, pData);

	// 14.任务板
// 	SaveBoardQuest2DB(pData, pData);

	// 15.神系统领域
	pSend->RoleData.nAreaNum		= 0;
	SaveArea2DB(pData, pData, pSend->RoleData.nAreaNum);

	// 16.商城限量物品购买计数
	pSend->RoleData.nBuyMallItemLogCount = m_mapBuyMallItemCount.Size();
	if(pSend->RoleData.nBuyMallItemLogCount > 0)
		SaveBuyMallItemCount2DB(pData, pData, pSend->RoleData.nBuyMallItemLogCount);

	//元神
	/*pSend->RoleData.bIsHolySoulActivated = m_HolySoul.IsActivated();
	if ( m_HolySoul.IsActivated() )
	{
		SaveHolySoulInfo(pData,pData);
	}*/

	// 重新计算消息大小
	pSend->dwSize = (DWORD)((BYTE*)pData - (BYTE*)pSend);

	g_dbSession.Send(pSend, pSend->dwSize);

	// 重置保存倒计时
	ResetNeedSave2DBCD();

	if( _isnan(m_MoveData.m_vPos.x) || _isnan(m_MoveData.m_vPos.y) || _isnan(m_MoveData.m_vPos.z) )
	{
		ILOG->Write(_T("Error2!!!! Role's position Invalid, Role ID is %u, movestate is %d, nextstate is %d, startpos is <%f, %f, %f> endpos is <%f, %f, %f>\r\n\r\n"), 
			m_dwID, m_MoveData.m_eCurMove, m_MoveData.m_eNextPreMove, m_MoveData.m_vPosStart.x, m_MoveData.m_vPosStart.y, m_MoveData.m_vPosStart.z, 
			m_MoveData.m_vDest.x, m_MoveData.m_vDest.y, m_MoveData.m_vDest.z);
	}

	//快捷栏保存服务器
	SaveQuickBarData2DB();
	GetItemMgr().SaveMatingFabao2DB();

	//如果身上带有稀有领域需要将时间存库

	// 角色身上的log数据	//这个消息DB已经不处理了，不发了
// 	tagNDBC_LogRoleData roledata;
// 	roledata.dwRoleID = GetID();
// 	memcpy(roledata.dwData, m_dwRoleLogData, sizeof(DWORD)*DataCollection_Max_RoleData_Num);
// 	g_dbSession.Send(&roledata, roledata.dwSize);
}

//-------------------------------------------------------------------------------
// 保存技能
//-------------------------------------------------------------------------------
VOID Role::SaveSkill2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	INT32 i	= 0;
	Skill* pSkill = NULL;
	tagSkillSave *pSkillSave = (tagSkillSave*)pData;

	TMap<DWORD, Skill*>::TMapIterator iter = m_mapSkill.Begin();
	while(m_mapSkill.PeekNext(iter, pSkill))
	{
		// 如果是普通攻击技能，则不保存；变形技能也不需要保存
		if( ESSTE_Default == pSkill->GetTypeEx() || pSkill->IsExtraSkill() ) continue;

		// 如果没变过，不保存
		if (!pSkill->IsNeedSave()) continue;

		pSkill->InitSkillSave(&pSkillSave[i++]);
		pSkill->SetSaved();
	}

	nNum = i;
	pOutPointer = &pSkillSave[nNum];
}

//-------------------------------------------------------------------------------
// 保存Buff
//-------------------------------------------------------------------------------
VOID Role::SaveBuff2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	INT32		nSize = 0;
	BYTE		*pCurPointer = (BYTE*)pData;

	INT nBuffNum = 0;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		// 下线消失的buff
		if( m_Buff[n].Interrupt(EBIF_OffLine) ) continue;

		m_Buff[n].InitBuffSave((tagBuffSave*)pCurPointer, nSize);
		pCurPointer += nSize;
		++nBuffNum;
	}

	nNum = nBuffNum;

	pOutPointer = pCurPointer;
}

//---------------------------------------------------------------------------------
// 保存任务
//---------------------------------------------------------------------------------
VOID Role::SaveQuest2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	tagQuestSave* pQuestSave = (tagQuestSave*)pData;

	nNum = 0;

	for(INT n = 0; n < QUEST_MAX_COUNT; ++n)
	{
		if( FALSE == m_Quests[n].IsValid() )
			continue;

		m_Quests[n].InitQuestSave(&pQuestSave[nNum++]);
	}

	pOutPointer = &pQuestSave[nNum];
}

//---------------------------------------------------------------------------------
// 保存师徒
//---------------------------------------------------------------------------------
VOID Role::SaveMaster2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer)
{
	MTRANS_POINTER(pSave, pData, tagMasterApprentice2DB);

	pSave->dwID = m_dwMasterID;
	pSave->nJingWuPoint = m_nJingWuPoint;
	pSave->nQinWuPoint = m_nQinWuPoint;
	pSave->nLevelUpBonus = m_nLevelUpBonus;
	pSave->nRewardNum = m_nRewardNum;
	pSave->sApprentice = m_ApprenticeData;

	pOutPointer = (BYTE*)pSave + sizeof(tagMasterApprentice2DB);
}

// VOID Role::SaveBoardQuest2DB(IN LPVOID pData, OUT LPVOID &pOutPointer)
// {	
// 	MTRANS_POINTER(pSave, pData, tagBoardQuestSaveData);
// 	
// 	INT nNum = m_pBoardQuestMgr->GetRoleBoardQuest().Size();
// 	for(INT i=0; i<nNum; ++i)
// 	{
// 		UINT16 u16ID = QUEST_PANEL_START_TASK_ID+i;
// 		if (!P_VALID(m_pBoardQuestMgr->GetRoleBoardQuest().Peek(u16ID)))
// 		{
// 			continue;
// 		}
// 
// 		pSave->dwRoleID = GetID();
// 		pSave->u16QuestID = u16ID;
// 		pSave->u16BoardID = m_pBoardQuestMgr->GetRoleBoardQuest().Peek(u16ID)->u16BoardID;
// 		pSave->eState = m_pBoardQuestMgr->GetRoleBoardQuest().Peek(u16ID)->eState;
// 
// 		//pSave = pSave + sizeof(tagBoardQuestSaveData);
// 		++pSave;
// 	}
// 
// 	pOutPointer = pSave;
// }
//-------------------------------------------------------------------------------
// 保存神系统领域
VOID Role::SaveArea2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum)
{
	m_GodAreaMgr.Save2DB(pData,pOutPointer,nNum);
}
//-------------------------------------------------------------------------------

VOID Role::SaveBuyMallItemCount2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nOutNum)
{
	INT nSum = m_mapBuyMallItemCount.Size();
	if (nSum <= 0)
	{
		return;
	}

	tagBuyMallItemCountSaveData* p = (tagBuyMallItemCountSaveData*) pData;
	TMap<DWORD, INT>::TMapIterator it = m_mapBuyMallItemCount.Begin();
	DWORD dwTypeID=0;
	INT nNum = 0;
	INT i=0;
	while(m_mapBuyMallItemCount.PeekNext(it, dwTypeID, nNum))
	{
		if (!P_VALID(dwTypeID) || !P_VALID(nNum))
		{
			continue;
		}
		tagBuyMallItemCountSaveData* pTmp = p+i;
		if (!P_VALID(pTmp))
		{
			break;
		}
		
		//pTmp[i].dwRoleID = this->GetID();
		pTmp[i].dwTypeID = dwTypeID;
		pTmp[i].nNum = nNum;
		++i;
	}

	nOutNum = i;
	pOutPointer = &p[nOutNum];
}

//---------------------------------------------------------------------------------
// 保存元神信息
//---------------------------------------------------------------------------------
VOID Role::SaveHolySoulInfo(IN LPVOID pData, OUT LPVOID &pOutPointer)
{
	MTRANS_POINTER(pSave, pData, tagHolySoulInfoEx);

	m_HolySoul.SaveHolySoulInfoToDB(HolySoul::ESaveType_SystemUpdate,pSave);

	pOutPointer = (BYTE*)pSave + sizeof(tagHolySoulInfoEx);

}