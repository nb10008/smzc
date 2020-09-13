//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_init.cpp
// author: 
// actor:
// data: 2008-8-11
// last:
// brief: 初始化人物数据结构
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/master_apprentice.h"
#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_virgin_online.h"
#include "../WorldDefine/msg_activity.h"
#include "../WorldDefine/RoleDefine.h"
#include "../ServerDefine/msg_item.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/family_define.h"
#include "..\ServerDefine\family_define.h"
#include "../ServerDefine/msg_holySoul.h"

#include "role.h"
#include "att_res.h"
#include "skill.h"
#include "quest.h"
#include "quest_mgr.h"
#include "map_creator.h"
#include "script_mgr.h"
#include "title_mgr.h"
#include "pet_pocket.h"
#include "vip_netbar.h"
#include "guild.h"
#include "guild_mgr.h"
#include "quest_board.h"
#include "item_creator.h"
#include "family.h"
#include "family_join.h"
#include "Family_mgr.h"
#include "family_sprite.h"
#include "family_sprite_mgr.h"
#include "HolySoul.h"


//-------------------------------------------------------------------------------
// 初始化玩家
//-------------------------------------------------------------------------------
VOID Role::Init(const tagRoleDataLoad* pData)
{
	ASSERT( P_VALID(pData) );

	const tagRoleDataConst* pDataConst = &pData->RoleDataConst;
	const tagRoleDataSave* pDataSave = &pData->RoleDataSave;

	// 初始化玩家脚本
	m_pScript = g_ScriptMgr.GetRoleScript();

	// 玩家脚本数据
	IFASTCODE->MemCpy(m_ScriptData.dwData, pDataSave->Data.dwData, sizeof(DWORD)*ESD_Role);

	// 称号管理器
	m_pTitleMgr		= new TitleMgr;
	// Jason 2010-3-30
	if( P_VALID(m_pTitleMgr) )
		m_pTitleMgr->Resize(MAX_TITLE_NUM_EX);

	// 新建宠物带
	m_pPetPocket	= new PetPocket;
	
	// 任务板管理器
	m_pBoardQuestMgr = new BoardQuestMgr(this);
	m_pBoardQuestMgr->Init(this->GetScriptData(RewardBoardQuestLvlIdx), EQBT_Reward);

	m_pYellowListMgr = new BoardQuestMgr(this);
	m_pYellowListMgr->Init(this->GetScriptData(YellowListBoardQuestLvlIdx), EQBT_YellowList);
	
	m_n16YellowListAcceptTimes = this->GetScriptData(TodayYellowListQuestTimesIdx);

	// 这里可以判断是否是今天的第一次登陆
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (pDataSave->LoginTime.year == dwCurTime.year &&
		pDataSave->LoginTime.month == dwCurTime.month &&
		pDataSave->LoginTime.day == dwCurTime.day)
	{
		m_bTodayFirstLogin = FALSE;
	}
	else
	{
		m_bTodayFirstLogin = TRUE;
	}

	// 初始化常规属性
	IFASTCODE->MemCpy(&m_Avatar, &pDataConst->Avatar, sizeof(m_Avatar));
	m_DisplaySet = pDataSave->DisplaySet;
	m_dwRebornMapID = pDataSave->dwRebornMapID;
	m_CreatTime = pDataConst->CreateTime;
	m_LoginTime = g_world.GetWorldTime();		// 重置上线时间
	
	//--------------------------------------------------------------------------------------------------

	// Jason 2010-1-8 每日在线奖励相关
	m_dateLastLessingLoong	= pDataSave->dateLastLessingLoong;
	if( (DWORD)m_dateLastLessingLoong == 0 )
		m_dateLastLessingLoong = m_LoginTime;

	m_LogoutTime = pDataSave->LogoutTime;
	m_timeRealLogout = m_LogoutTime;


	m_timeOnlineRewardPerDayRec = pDataSave->timeOnlineRewardPerDayRec;
	m_timeOneDayFirstLogined = pDataSave->timeOneDayFirstLogined;
	m_dwCurrentDayOnlineTime = pDataSave->dwCurrentDayOnlineTime;
	if( m_timeOneDayFirstLogined.day != m_LoginTime.day || m_timeOneDayFirstLogined.month != m_LoginTime.month || m_timeOneDayFirstLogined.year != m_LoginTime.year  ) // 换天了
	{
		// 隔天退出的情况
		if( m_LogoutTime.day == m_LoginTime.day && 
			m_LogoutTime.month == m_LoginTime.month &&
			m_LogoutTime.year == m_LoginTime.year ) 
		{
			// 隔天退出了，但是第二天没有领取过的时候，只清零m_nLessingLevelEx，当天累计在线时间m_timeLastLessing不清零，免得重新计时
			if( m_timeOnlineRewardPerDayRec.day !=	m_LoginTime.day || m_timeOnlineRewardPerDayRec.month != m_LoginTime.month || m_timeOnlineRewardPerDayRec.year != m_LoginTime.year   )
				m_nLessingLevelEx = 0;//这玩意有可能使用户多领奖励哟
		}
		// 不是跨天退出情况，就把这些时间相关量清零
		else  
		{
			m_dwCurrentDayOnlineTime = 0;
			m_nLessingLevelEx = 0;
			m_timeLastLessing = 0;
		}
		m_timeOneDayFirstLogined = m_LoginTime;
	}
	
	//m_timeOnlineRewardPerDayRec = pDataSave->timeOnlineRewardPerDayRec;

	// 2010-1-15 离线挂机相关
	m_bIsOpenOfflineReward = pDataSave->bIsOpenOfflineReward;

	//--------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------
	// Jason 2010-1-30 v1.3.2宠物栏阀
	m_u16PetPocketValve = pDataSave->u16FollowPetPocketValve+pDataSave->u16RidingPetPocketValve	;
	m_u16FollowPetPocketValve = pDataSave->u16FollowPetPocketValve;
	m_u16RidingPetPocketValve = pDataSave->u16RidingPetPocketValve;
	//--------------------------------------------------------------------------------------------------

	//m_LogoutTime = pDataSave->LogoutTime;
	m_nOnlineTime = pDataSave->nOnlineTime;
	m_nCurOnlineTime = pDataSave->nCurOnlineTime;
	m_dwLastUpgrade = pDataSave->dwLastUpgrade;
	m_eClass = pDataSave->eClass;
	m_eClassEx = pDataSave->eClassEx;
	m_nKillScore = pDataSave->nKillScore;
	m_bHasLeftMsg = pDataSave->bHasLeftMsg;
	// Jason 2010-4-12 
	m_nGodMiraclePoints = pDataSave->nGodMiraclePoints;

	// 初始化上次获取商城免费物品时间
	SetLastGetMallFreeTime(pDataSave->dwTimeGetMallFree);

	// 根据行囊是否加密设置行囊限制
	m_RoleStateEx.SetState(ERSE_BagPsdExist);
	if(GetSession() && !GetSession()->IsHaveBagPsd())
	{
		m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
		m_RoleStateEx.SetState(ERSE_BagPsdPass);
	}

	// 初始化称号选项
	m_pTitleMgr->InitOpts(this, pDataSave->u16ActiveTitleID, pDataSave->sRemoteOpenSet.bTitle);

	// 精力道具CD时间
	m_dwFixSpiriteCDTime = pDataSave->dwFixSpiritCD;

	// PK保护相关
	m_CloseSafeGuardTime = pDataSave->CloseSafeGuardTime;
	m_nDeadUnSetSafeGuardCountDown = pDataSave->nDeadUnSetSafeGuardCountDown;

	m_bNeedPrisonRevive = pDataSave->bNeedPrisonRevive;		//需要牢狱复活

	m_bUsingPkMinLevel = pDataSave->byUsingConstraintsPKSafeGuardLevel;

	// 师徒相关
	m_dwMasterID = pDataSave->dwMasterID;
	m_nMARelation = pDataSave->nMARelation;
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(m_dwID);
	if(P_VALID(pInfo)) m_nMARelation = pInfo->nMARelation; // 使用最新的 
	//IFASTCODE->MemCpy(&m_ApprenticeData.dwData, pDataSave->ApprenticeData.dwData, sizeof(m_ApprenticeData.dwData));
	//for (int i=0; i!=MAX_APPRENTICE_NUM; ++i)
	//{
	//	if (m_ApprenticeData.dwData[i] != 0)
	//	{
	//		m_mapApprentice.Add(m_ApprenticeData.dwData[i], m_ApprenticeData.dwData[i]);
	//	}
	//}
	if(m_dwMasterID != GT_INVALID)
		m_nJingWuPoint = pDataSave->nJingWuPoint;				// 精武点数
	//m_nQinWuPoint  = pDataSave->nQinWuPoint;				// 勤武点数

	IFASTCODE->MemCpy(m_Talent, pDataSave->talent, sizeof(m_Talent));

	// Jason 2010-7-14 v1.0.0
	m_RoleState.InitState(0);
	if( pDataSave->dwRoleState & ERS_PK )
	{
		SetRoleState(ERS_PK,FALSE);
	}
	// 练级模式
	else if( pDataSave->dwRoleState & ERS_PeaceModel )
	{
		SetRoleState(ERS_PeaceModel,FALSE);
	}
	else if( pDataSave->dwRoleState & ERS_PKEX )
	{
		SetRoleState(ERS_PKEX,FALSE);
	}

	INT nMax_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
	if(g_world.IsPkConstraintsOpen())
	{
		nMax_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
	}

	if(pDataSave->bSafeGuard && pDataSave->nLevel <= nMax_PK_SafeGuard_Level)
	{
		SetRoleState(ERS_Safeguard, FALSE);
	}

	m_byBuy50LvlItemFlag = pDataSave->byBuy50LvlItemFlag;
	m_byBuy60LvlItemFlag = pDataSave->byBuy60LvlItemFlag;
	m_byBuy70LvlItemFlag = pDataSave->byBuy70LvlItemFlag;
	m_byBuy80LvlItemFlag = pDataSave->byBuy80LvlItemFlag;
	GetWardrobe().SetBePlayActLayerID(pDataSave->dwBePlayActLayerID);
	GetWardrobe().SetBePlayActLevel(pDataSave->byBePlayActLevel);

	// 帮派
	m_dwGuildID = pDataSave->dwGuildID;
	m_dwFamilyID = pDataSave->dwFamilyID;
	if (GT_VALID(m_dwGuildID))
	{
		// 检查玩家是否在帮派中
		Guild* pGuild	= g_guildMgr.GetGuild(m_dwGuildID);
		if (!P_VALID(pGuild) || !P_VALID(pGuild->GetMember(GetID())))
		{
			SetGuildID(GT_INVALID);
		}
		else
		{
			// 跑商状态
			GuildCommodity* pCommodity = pGuild->GetGuildCommerce().GetCommodity(m_dwID);
			if (P_VALID(pCommodity))
			{
				SetRoleState(ERS_Commerce, FALSE);
			}
		}
	}
	
	m_nPulseLearnTimes = pData->RoleDataSave.nPulseLearnRemainTime;
	m_nTigerTimes = pData->RoleDataSave.nTigerTime;

	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		// 如果跨天，重置每日接取天之行会任务榜任务次数
		ResetYellowListAcceptTimes();

		// 如果跨天，重置角色使用经验丹次数
		ResetExpPilularUseTimes();

		// 重置经脉学习次数
		ResetPulseCanLearnNum();
	}

	// 读取并设置人物数据库属性
	InitAtt(pDataSave);

	//if( pDataSave->timeLearnSoaringSkill.year != m_LoginTime.year || pDataSave->timeLearnSoaringSkill.month != m_LoginTime.month || pDataSave->timeLearnSoaringSkill.day != m_LoginTime.day  )
	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		m_nSoaringSkillLearnTimes	= m_nCLearnSoaringSkillTotalTimes;
		//m_timeLearnSoaringSkill		= m_LoginTime;
	}
	else
	{
		m_nSoaringSkillLearnTimes	= pDataSave->nSoaringSkillLearnTimes;
		//m_timeLearnSoaringSkill		= pDataSave->timeLearnSoaringSkill;
	}

	// 任务板相关
	m_pBoardQuestMgr->SetCompleteRefreshTime(pDataSave->nCompleteRefreshTime);
	m_pBoardQuestMgr->SetIMRefreshTime(pDataSave->nIMRefreshTime);
	m_pYellowListMgr->SetCompleteRefreshTime(pDataSave->nYellowListCompleteRefreshTime);
	m_pYellowListMgr->SetIMRefreshTime(pDataSave->nYellowListIMRefreshTime);

	// 初始化角色实力值相关的基础值，需要计算的在后面计算
	//m_Strength.nAdvanceStrength = pData->RoleDataSave.nAdvanceStrength;
	m_Strength.nConsumptiveStrength = pData->RoleDataSave.nConsumptiveStrength;
	m_nStrengthNum = pData->RoleDataSave.nStrengthNum;
	m_nEquipPm = pDataSave->nEquipPm;

	// Jason 2010-6-12 v1.0.0
	m_nConsolidateTimes = pDataSave->nConsolidateTimes;

	m_timeTakeGuildReward = pDataSave->timeTakeGuildReward	;

	// 检查是否需要更新进阶实力值
	CheckRoleAdvanceStrength();

	// 读取拾取模式设置
	m_dwPickupModeSetting = pDataSave->dwPickupModeSetting;

	// 读取玩家回归标志
	m_bPlayerBack = pDataSave->byPlayerBack;
	if(!g_world.IsPlayerBack())
	{
		m_bPlayerBack = FALSE;
		m_byPlayerBackDay = 0;
	}
	m_byPlayerBackDay = pDataSave->byPlayerBackDays;

	// 神职竞选相关
	m_dwClergyMarsPoint = pDataSave->dwClergyMarsPoint;
	m_dwClergyApolloPoint = pDataSave->dwClergyApolloPoint;
	m_dwClergyRabbiPoint = pDataSave->dwClergyRabbiPoint;
	m_dwClergyPeacePoint = pDataSave->dwClergyPeacePoint;
	m_eClergyCandidate = pDataSave->eClergyCandidate;
	m_eClergy4SeniorOrJunior = pDataSave->eSeniorOrJunior;
	m_dwClergy = pDataSave->dwClergy;
	m_dwPrevClergy = pDataSave->dwPreClergy;

	m_dwKeyCodeRewarded = pDataSave->dwKeyCodeRewarded;

	m_dwTrainDate = pDataSave->dwTrainDate;
	m_nTrainNum = pDataSave->nTrainNum;
	m_nAwardPoint = pDataSave->nAwardPoint;
	m_nAwardFlag = pDataSave->nAwardFlag;

	m_nGod = pDataSave->nGod;
	m_nMonster = pDataSave->nMonster;
	m_nGodPoint = pDataSave->nGodPoint;
	m_nMonsterPoint = pDataSave->nMonsterPoint;

	UpdateFamilyTrainNum();

	// todo：读取技能列表，装备列表，buff列表，并计算各自对人物属性的影响
	/*********************************************************************************************************
	*技能列表，状态列表，称号列表，称号条件列表，物品列表，装备列表，好友列表，仇敌列表，任务列表，已完成任务列表，角色名帖
	*师徒，同门， 已开启试炼状态表
	*必须按顺序读取(该顺序和tagRoleDataSave中对应)
	*********************************************************************************************************/

	const BYTE *pDBData = &pDataSave->byData[0];	// 列表数据指针,该指针需在读取函数中移动,即pDBData的值是变化的

	// 初始化技能列表
	InitSkill(pDBData, pDataSave->nSkillNum);

	// 初始化状态列表
	InitBuff(pDBData, pDataSave->nBuffNum);

	// 初始化称号列表
	m_pTitleMgr->InitTitles(pDBData, pDataSave->nTitleNum);

	// 计算玩家的离线分钟数，以便扣除圣灵默契值
	/*DWORD dwOffSecond = CalcTimeDiff(m_LoginTime,pDataSave->LogoutTime);
	INT nOffMinute =(INT)(dwOffSecond / (DWORD)60);*/
	// 初始化物品装备列表
	InitItem(pDBData, pDataSave->nItemNum);

	// 初始化好友仇敌列表
	InitFriend(pDBData, pDataSave->nFriendNum);
	
	// 仇敌列表相关消息
	InitEnemy(pDBData, pDataSave->nEnemyNum);

	// 初始化玩家当前任务列表
	InitCurrentQuest(pDBData, pDataSave->nQuestNum);

	// 初始化玩家完成任务列表
	InitCompleteQuest(pDBData, pDataSave->nQuestDoneNum);

	// 初始化玩家完成每日任务列表
	InitDailyCompleteQuest(pDBData, pDataSave->nDailyQuestDoneNum);

	// 初始化物品冷却时间
	InitItemCDTime(pDBData, pDataSave->nItemCDTimeNum);

	// 初始化好友度
	InitFriendValue(pDBData, pDataSave->nFriendshipNum);

	// 初始化黑名单
	InitBlackList(pDBData, pDataSave->nBlackNum);

	// 初始化角色名贴
	m_VCard.Init(pDBData, this);

	// 初始化氏族数据
	m_ClanData.Init(pDBData, this);

	// 初始化宠物带
	m_pPetPocket->Init(pDBData, pDataSave->nPetsNum, this);

	// 初始化师徒
	InitMaster(pDBData);

	// 初始化同门
	InitClassMates(pDBData);

	// 初始化已开启试炼状态表
	InitTrainState(pDBData, pDataSave->nOpenTrainCnt);

	// 任务板任务
	m_pBoardQuestMgr->Init(pDBData, pDataSave->nBoardQuestNum, this, FALSE);
	m_pYellowListMgr->Init(pDBData, pDataSave->nBoardQuestNum, this, TRUE);

	// 角色当前月在线时长
	//InitCurMonthOnlineTime(pDBData, pDataSave->nMonthOnlineTimeCnt);

	// 砸金蛋
	m_EggBroker.InitEggInfoFromDB(pDBData, pDataSave->nEggInfoCnt);

	// 衣橱
	m_Wardrobe.InitWardrobe(pDBData, pDataSave->nWardrobeLayerCnt, this);

	// 初始化神系统领域列表
	InitArea(pDBData, pDataSave->nAreaNum);

	// 初始化购买商城物品计数
	InitBuyMallItemCount(pDBData, pDataSave->nBuyMallItemLogCount);

	//初始化家族妖精属性加乘
	InitAbilityFromFamilySprite();

	// 神职竞选
	LoginClergyVoteResultCheck();

	if( GetBaseAttValue(ERA_Soar) == ESV_NotCompletedSoaring && !IsHaveQuest(40001) && !IsHaveDoneQuest(40001) )
	{
		SetBaseAttValue(ERA_Soar,ESV_NotSoaring);
	}
	//初始化元神信息
	//if ( pDataSave->bIsHolySoulActivated )
	//{
	//	DWORD dwOffSecond = CalcTimeDiff(m_LoginTime,pDataSave->LogoutTime);
	//	INT nOffHour =(INT)(dwOffSecond / (DWORD)3600);
	//	InitHolySoulInfo(pDBData,nOffHour);
	//}

	// 所有内容已经加载完毕，重新计算人物初始当前属性(这里包括计算实力系统相关的各个值)
	CalInitAtt();

	// 计算所有状态
	CalInitState();

	g_VipNetBarMgr.PlayerNotify(GetSession()->GetSessionID());

	// 初始化角色开宝箱相关状态
	InitChestState();

	m_BrotherMgr.InitData(pDataSave->dwBrotherTeatherID,pDataSave->timeBloodBrother);

	// 如果大于30级，升级时不需要检测关闭pk保护
// 	INT Max_PK_SafeGuard_Level = SAFE_GUARD_FORCE_LEVEL;
// 	if( g_world.IsPkConstraintsOpen()  && IsUsingPKConstraintsSafeGuradMinLevel() )
// 	{
// 		Max_PK_SafeGuard_Level = g_world.GetPKMiniLevel();
// 	}
// 	if ( m_nLevel > Max_PK_SafeGuard_Level )
// 	{
// 		m_bLevelUpClosedSafeGuard = TRUE;
// 	}

	tagNS_RoleInitOk send;
	GetSession()->SendMessage(&send, send.dwSize);

	//UpdateBlessingOfLoong();// Jason
	// Jason 12-8-2007采用拉模式
	//NotifyClientIniLessingOfLoong();

	// Jason v1.3.1外部链接
	g_dbSession.SendMsgToPlayerForExtLinks  (this);


	if( g_VipNetBarMgr.IsOpen() )
	{
		g_VipNetBarMgr.RecReward(this);
	}

	SynPickupModeSetting2Client();
}

//----------------------------------------------------------------------------------------------
// 初始化数据库中中的人物属性
//----------------------------------------------------------------------------------------------
VOID Role::InitAtt(const tagRoleDataSave* pDataSave)
{
	// 各个一级属性的投点
	IFASTCODE->MemCpy(m_nAttPointAdd, pDataSave->nAttPointAdd, sizeof(m_nAttPointAdd));

	// 所有属性附上默认值
	for(INT n = 0; n < ERA_End; n++)
	{
		m_nBaseAtt[n] = g_attRes.GetAttDefRole(n);
	}

	// 设置一些保存在数据库中的属性（一些不随基础值改变的属性，还缺少一个士气）
	m_nAtt[ERA_HP]			=	pDataSave->nHP;
	m_nAtt[ERA_MP]			=	pDataSave->nMP;
	m_nAtt[ERA_Rage]		=	pDataSave->nRage;
	m_nAtt[ERA_Vitality]	=	pDataSave->nVitality;
	m_nAtt[ERA_Endurance]	=	pDataSave->nEndurance;
	m_nAtt[ERA_Knowledge]	=	pDataSave->nKnowledge;
	m_nAtt[ERA_Injury]		=	pDataSave->nInjury;
	m_nAtt[ERA_Morale]		=	pDataSave->nMorale;
	m_nAtt[ERA_Morality]	=	pDataSave->nMorality;
	m_nAtt[ERA_Culture]		=	pDataSave->nCulture;
	m_nAtt[ERA_AttPoint]	=	pDataSave->nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	pDataSave->nTalentPoint;
	m_nAtt[ERA_Hostility]	=	pDataSave->nHostility;
	//m_nAtt[ERA_Shili]		=	pDataSave->nStrength;		// 实力值
	m_nAtt[ERA_Spirit]		=	pDataSave->nSpirit;			// 精力值
	m_nAtt[ERA_WuXun]		=	pDataSave->nWunXun;			// 武勋值
	m_nAtt[ERA_WuJi]		=	pDataSave->nWunJi;			// 武技值
	//神系统属性
	m_nAtt[ERA_God_Condenced] = pDataSave->nGodCondense;
	m_nAtt[ERA_God_Faith]		= pDataSave->nGodFaith;
	m_nAtt[ERA_God_Godhead]	= pDataSave->nGodhead;
	//鲜花鸡蛋
	m_nAtt[ERA_FlowerNum]		=	pDataSave->nFlower;			// 鲜花
	m_nAtt[ERA_EggsNum]			=	pDataSave->nEgg;			// 鸡蛋
	m_nAtt[ERA_Holy]			=	pDataSave->nHolyValue;		// 圣灵值

	if( pDataSave->LogoutTime.year != m_LoginTime.year || pDataSave->LogoutTime.month != m_LoginTime.month || pDataSave->LogoutTime.day != m_LoginTime.day )
	{
		m_nAtt[ERA_Weary]	=	0;							// 疲劳值清0
	}
	else
	{
		m_nAtt[ERA_Weary]	=	pDataSave->nWeary;			// 疲劳值
	}
	m_nAtt[ERA_VipLevel]	= pDataSave->nVipLevel;	// 角色vip等级

	m_nLevel		= pDataSave->nLevel;					// 等级
	m_nLevelPm      = pDataSave->nLevelPm;					// 等级排名
	m_dwLastUpgrade = pDataSave->dwLastUpgrade;				// 最近一次升级时间
	m_nCurLevelExp	= pDataSave->nCurExp;					// 当前升级经验

	m_eClass	= pDataSave->eClass;						// 职业
	m_eClassEx	= pDataSave->eClassEx;						// 职业扩展
	m_nCredit	= pDataSave->nCredit;						// 信用度
	m_nIdentity = pDataSave->nIdentity;						// 身份
	m_nVIPPoint = pDataSave->nVIPPoint;						// 会员积分
	m_nTreasureSum = pDataSave->nTreasureSum;				//开启宝箱计数

	// 通过人物等级计算对应的一级属性
	const tagLevelUpEffect* pLevelUp = g_attRes.GetLevelUpEffect(pDataSave->nLevel);

	SetBaseAttValue(ERA_Physique, pLevelUp->n16RoleAtt[ERA_Physique]);
	SetBaseAttValue(ERA_Strength, pLevelUp->n16RoleAtt[ERA_Strength]);
	SetBaseAttValue(ERA_Pneuma, pLevelUp->n16RoleAtt[ERA_Pneuma]);
	SetBaseAttValue(ERA_InnerForce, pLevelUp->n16RoleAtt[ERA_InnerForce]);
	SetBaseAttValue(ERA_Technique, pLevelUp->n16RoleAtt[ERA_Technique]);
	SetBaseAttValue(ERA_Agility, pLevelUp->n16RoleAtt[ERA_Agility]);

	// 设置某些二级属性
	SetBaseAttValue(ERA_MaxHP,			pLevelUp->n16HP);
	SetBaseAttValue(ERA_MaxMP,			pLevelUp->n16MP);
	SetBaseAttValue(ERA_MaxVitality,	pLevelUp->n16Vitality);

	// 根据玩家属性的投放值来增加一级属性
	ModBaseAttValue(ERA_Physique, m_nAttPointAdd[ERA_Physique]);
	ModBaseAttValue(ERA_Strength, m_nAttPointAdd[ERA_Strength]);
	ModBaseAttValue(ERA_Pneuma, m_nAttPointAdd[ERA_Pneuma]);
	ModBaseAttValue(ERA_InnerForce, m_nAttPointAdd[ERA_InnerForce]);
	ModBaseAttValue(ERA_Technique, m_nAttPointAdd[ERA_Technique]);
	ModBaseAttValue(ERA_Agility, m_nAttPointAdd[ERA_Agility]);
    

	// Jason 2010-5-24 v2.2.0
	//SetAttValue(ERA_Soar,pDataSave->nSoaringValue);
	//m_nAtt[ERA_Soar] = pDataSave->nSoaringValue;
	//m_nAtt[ERA_Toughness] = pDataSave->nToughness;
	int v = pDataSave->nSoaringValue;
	switch( v )
	{
	case ESV_NotSoaring:
	case ESV_SoaringUp:
	case ESV_NotCompletedSoaring:
		break;
	default:
		v = 0;
		break;
	}
	SetBaseAttValue(ERA_Soar,v);
	SetBaseAttValue(ERA_Toughness,pDataSave->nToughness);
}

//---------------------------------------------------------------------------------
// 初始化技能
//---------------------------------------------------------------------------------
VOID Role::InitSkill(const BYTE* &pData, INT32 nNum)
{
#if 0 //普通攻击为天生技能，不需要临时添加了
	// 首先加载普通攻击技能
	for(INT n = 0; n < X_NORMAL_ATTACK_SKILL_NUM; n++)
	{
		DWORD dwSkillID = NORMAL_ATTACK_SKILL_ID[n];
		if( !P_VALID( g_attRes.GetSkillProto(Skill::CreateTypeID(dwSkillID, 1)) ) )
		{
			continue;
		}
		Skill* pSkill = new Skill(dwSkillID, 1, 0, 0, 0);
		AddSkill(pSkill, FALSE);
	}
#endif
	// 加载保存的技能
	if( nNum <= 0 ) return;

	const tagSkillSave* pSkillSave = (const tagSkillSave*)pData;

	for(INT n = 0; n < nNum; n++)
	{
		if( !P_VALID( g_attRes.GetSkillProto(Skill::CreateTypeID(pSkillSave->dwID, pSkillSave->nSelfLevel + pSkillSave->nLearnLevel)) ) )
		{
			pSkillSave++;
			continue;
		}

		const tagForceSkillProto* pForceSkillProto = g_attRes.GetForceSkillProtoEx(Skill::CreateTypeID(pSkillSave->dwID, pSkillSave->nSelfLevel + pSkillSave->nLearnLevel));
		if( P_VALID(pForceSkillProto))
		{
			if( !pForceSkillProto->bForerver )
			{
				//技能已经过了激活期
				if( CalcTimeDiff(GetCurrentDWORDTime(), pSkillSave->dwActiveTime) > 0 )
				{
					g_dbSession.SendRemoveSkill( GetID(), pSkillSave->dwID );
					pSkillSave++;
					continue;
				}
			}
		}

		Skill* pSkill = new Skill(pSkillSave->dwID, pSkillSave->nSelfLevel, pSkillSave->nLearnLevel, pSkillSave->nCoolDown, pSkillSave->nProficiency, pSkillSave->dwActiveTime);

		// 将技能加入到列表中
		AddSkill(pSkill, FALSE);

		if( P_VALID(pForceSkillProto) && (!pForceSkillProto->bForerver) )
		{
			m_listSkillActiveTime.PushBack(pSkillSave->dwID);
		}

		pSkillSave++;
	}

	// 重设指针
	pData = (const BYTE*)((tagSkillSave*)pData + nNum);
	//pData = (const BYTE*)pSkillSave;
}

//-------------------------------------------------------------------------------
// 初始化状态
//-------------------------------------------------------------------------------
VOID Role::InitBuff(const BYTE* &pData, INT32 nNum)
{
	if( nNum <= 0 ) return;

	INT nBeBuffIndex = 0;
	INT nDeBuffIndex = MAX_BENEFIT_BUFF_NUM;

/*	const tagBuffSave* pEnd = (tagBuffSave*)pData + nNum;*/
	DWORD dwVipNetbarBuff = GT_INVALID,dwVipNetbarInstBuffID = GT_INVALID;

	for(INT n = 0; n < nNum; n++)
	{
		tagBuffSave* pBuffSave = (tagBuffSave*)pData;

		const tagBuffProto* pProto = g_attRes.GetBuffProto(Buff::GetTypeIDFromIDAndLevel(pBuffSave->dwBuffID, pBuffSave->n8Level));
		if( !P_VALID(pProto) ) goto next_buff;

		// 计算离线计时Buff
		if (pProto->bOfflineConsume)
		{
			// 获取当前时间和该玩家上次下线的时间差(单位：秒)
			DWORD dwOfflineTick = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime) * TICK_PER_SECOND;
			
			// 计算Buff剩余持续时间(即使DWORD=>INT32也没问题)
			if (dwOfflineTick > MAX_BUFF_PERSIST_TICK)
			{
				goto next_buff;
			}
			else if ( pProto->nPersistTick > (INT32)dwOfflineTick + pBuffSave->nPersistTick )
			{
				pBuffSave->nPersistTick += (INT32)dwOfflineTick;
			}
			else
			{
				goto next_buff;
			}
		}

		// 之前有网吧buff，但是没有在网吧登录，不加效果的
		dwVipNetbarBuff = GT_INVALID;
		dwVipNetbarInstBuffID = GT_INVALID;
		// 没有在金牌网吧，网吧加成的buff，不添加
		if( !g_VipNetBarMgr.IsInVipNetbar(this,&dwVipNetbarBuff,&dwVipNetbarInstBuffID) )
		{
			if( g_VipNetBarMgr.IsVipNetbarBuff(pProto->dwID) )
				goto next_buff;
		}
		// 网吧功能关闭，也不增加网吧增益buff
		if( g_VipNetBarMgr.IsOpen() == FALSE )
		{
			if( g_VipNetBarMgr.IsVipNetbarBuff(pProto->dwID) )
				goto next_buff;
		}
		// 不添加网吧的副本增益buff
		if( g_VipNetBarMgr.IsVipNetbarInstBuff( pProto->dwID ) )
			goto next_buff;

		// 如果是有益Buff
		INT nIndex = GT_INVALID;
		if( pProto->bBenifit )
		{
			nIndex = nBeBuffIndex;
			// 有益Buff的数量已满
			if( nIndex >= MAX_BENEFIT_BUFF_NUM ) continue;
			++nBeBuffIndex;
		}
		// 否则是有害Buff
		else
		{
			nIndex = nDeBuffIndex;
			// 有害Buff的数量已满
			if( nIndex >= MAX_BUFF_NUM ) continue;
			++nDeBuffIndex;
		}

		m_Buff[nIndex].Init(this, pBuffSave, nIndex);
		m_mapBuff.Add(m_Buff[nIndex].GetID(), &m_Buff[nIndex]);

next_buff:
		// 累加指针
		pData += sizeof(tagBuffSave) - 1 + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

/*	pData = (const BYTE*)pEnd;*/
}

//-------------------------------------------------------------------------------
// 初始化玩家物品(装备)
//-------------------------------------------------------------------------------
VOID Role::InitItem(const BYTE* &pData, INT32 nNum)
{
	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);
	
	// 清除外观信息
	ZeroMemory(&m_AvatarEquipEquip, SIZE_AVATAR_EQUIP);
	ZeroMemory(&m_AvatarEquipFashion, SIZE_AVATAR_EQUIP);
	ZeroMemory(&m_AvatarEquipWardrobe, SIZE_AVATAR_EQUIP);
	
	DWORD dwErrorCode;
	TList<tagItem *> listItem;
	const tagItem	*pTmpItem	= NULL;
	tagItem			*pNewItem	= NULL;
	BYTE * p = (BYTE*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		nItemSize		= sizeof(tagItem);
		nEquipSize	= sizeof(tagEquip);
		INT32 * pLen = (INT32*)p;
		p += sizeof(INT32);

		pTmpItem = (const tagItem *)p;
		// 检查物品是否在解除锁定倒计时中
		if(pTmpItem->dwUnlockTime != 0)
		{
			GetItemMgr().Add2UnlockCDTimeMap(pTmpItem->n64Serial, pTmpItem->dwUnlockTime);
		}

		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			nItemSize = *pLen;
			pNewItem = new tagItem;
			if(*pLen >= sizeof(tagItem))
			{
				IFASTCODE->MemCpy(pNewItem, pTmpItem, sizeof(tagItem));
				pNewItem->pProtoType = g_attRes.GetItemProto(pTmpItem->dwTypeID);
			}
			else
			{
				delete pNewItem;
				p += *pLen;
				continue;
			}
			if( !P_VALID(pNewItem->pProtoType) )
			{
				// 这边没有办法
				p += *pLen;
				delete pNewItem;
				continue;
			}

		}
		else
		{
			pNewItem = new tagEquip;
			((tagEquip*)pNewItem)->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
			if( *pLen >= sizeof(tagEquip) )
			{
				IFASTCODE->MemCpy(pNewItem, pTmpItem, nEquipSize);
				pNewItem->pProtoType = g_attRes.GetEquipProto(pTmpItem->dwTypeID);
			}
			else // 错误，划拨时，应该会有数据，而不会少。
			{
				p += *pLen;
				delete (tagEquip*)pNewItem;
				continue;
			}
			if( !P_VALID(pNewItem->pProtoType) )
			{
				nEquipSize = *pLen;
				// 这边没有办法判断法宝了
				p += *pLen;
				delete (tagEquip*)pNewItem;
				continue;
			}
			if (MIsSoulCrystalType(pNewItem->pProtoType->eType))
			{
				delete (tagEquip*)pNewItem ;
				tagSoulCrystal * pSoulCrystal = new tagSoulCrystal;
				pSoulCrystal->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
				pNewItem = pSoulCrystal;
				if( *pLen >= sizeof(tagSoulCrystal) )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, sizeof(tagSoulCrystal));
					pNewItem->pProtoType = g_attRes.GetEquipProto(pNewItem->dwTypeID);
				}
				else
				{
					p += *pLen;
					delete pSoulCrystal;
					continue;
				}
			}
			if( MIsFaBaoType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				if( !g_world.IsFabaoOpen() )
				{
					p += *pLen;
					continue;
				}
				tagFabao * pFabao = new tagFabao;
				pFabao->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
				pNewItem = pFabao;
				if( *pLen >= SIZE_FABAO )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_FABAO);
				}
				else // 可能是划拨过来的，但是也有可能是错误，目前按这种办法做吧
				{
					if( *pLen >= SIZE_EQUIP )
					{
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pFabao->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
						tagNDBC_NewFabao msg;
						memcpy(&msg .fabao,pFabao,SIZE_FABAO);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						delete pFabao;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetFabaoProto(pTmpItem->dwTypeID);
				if( pFabao->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				{
					ItemCreator::IdentifyEquip(pFabao);
					tagNDBC_UpdateEquipAtt send;
					send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
					IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

					g_dbSession.Send(&send, send.dwSize);
					GetItemMgr().SendFabaoSpec2DB(*pFabao);
				}
				// 临时
				const tagConsolidateItem* p = NULL;
				INT size_n = min(pFabao->nSlotNum,MaxWuxingSlotNum);
				pFabao->bySlotLingzhuMatch = 0;
				for(int i = 0; i < size_n; ++i)
				{
					pFabao->bySlotMacthNdx[i] = 0;
					if( P_VALID(pFabao->dwWuxingBeadID[i]) )
					{
						p = g_attRes.GetConsolidateProto(pFabao->dwWuxingBeadID[i]);
						if( P_VALID(p) )
						{
							if( p->eWuXing == pFabao->eWuxingHole[i] )
								pFabao->bySlotMacthNdx[i] = 1;
						}
					}
					if( pFabao->bySlotMacthNdx[i] )
						pFabao->bySlotLingzhuMatch++;
				}

				// 防止由于意外原因导致，技能栏被-1覆盖，奇怪。。。
				if(pFabao->n16Stage >= 60)
				{
					if( pFabao->n16NativeIntelligence < 80 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence < 90 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence < 95 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
					else if( pFabao->n16NativeIntelligence >= 95 )
					{
						for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
							if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
								pFabao->dwFairySkill[nSkillNdx] = 0;
					}
				}

				switch( pFabao->eFabaoStatus )
				{
				case EEFS_Waiting:
					pFabao->LogFairyBirth(EEFS_Waiting, EEFS_Normal);
					pFabao->eFabaoStatus = EEFS_Normal;
					pFabao->dwStatusChangeTime = 0;
					break;
				case EEFS_Pregnancy:
				case EEFS_Rest:
					if( pFabao->dwStatusChangeTime > 0 )
						GetItemMgr().InsertMatingFabao(pFabao);
					else
					{
						if(pFabao->eFabaoStatus == EEFS_Pregnancy)
						{
							pFabao->LogFairyBirth(EEFS_Pregnancy, EEFS_GiveBirth);
							pFabao->eFabaoStatus = EEFS_GiveBirth;
						}
						else
						{
							BOOL bNormalChg = pFabao->LogFairyBirth(pFabao->eFabaoStatus, EEFS_Normal);
							if (!bNormalChg)
								ILOG->Write(_T("FairyBirth status change invalid, file: %s line: %d"), __FILE__, __LINE__);
							pFabao->eFabaoStatus = EEFS_Normal;
						}
						GetItemMgr().SendFabaoSpec2DB(*pFabao);
					}
					break;
				default :
					pFabao->dwStatusChangeTime = 0;
				}

				if( g_world.IsFairyMatingOpen() )
				{
					BOOL bCanBirth = TRUE;
					if( P_VALID(pFabao->pProtoType) )
					{
						tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
						if( pFabaoProto->bCanBirth == GT_INVALID )
							bCanBirth = FALSE;
					}
					if( bCanBirth && pFabao->byIsGivenBirthTimes == 0 && pFabao->n16Stage >= 40 && pFabao->nBirthTimes == 0 )
					{
						tagFabaoProto * pFabaoProto = (tagFabaoProto*)pFabao->pProtoType;
						INT nStart = 0;
						if(P_VALID(pFabaoProto) && pFabaoProto->bCanBirth != GT_INVALID)
							nStart = pFabaoProto->bCanBirth;
						pFabao->nBirthTimes = (pFabao->n16Stage - 40) / 20 + 1 + nStart;
						pFabao->byIsGivenBirthTimes = 1;
						tagNDBC_UpdateFabaoGivenBirthTimes __msg;
						__msg._n64ID = pFabao->n64Serial;
						__msg.nTimes = pFabao->nBirthTimes;
						g_dbSession.Send(&__msg,__msg.dwSize);
					}
					if( !bCanBirth && pFabao->nBirthTimes > 0 )
					{
						pFabao->nBirthTimes = 0;
						tagNDBC_UpdateFabaoGivenBirthTimes __msg;
						__msg._n64ID = pFabao->n64Serial;
						__msg.nTimes = pFabao->nBirthTimes;
						g_dbSession.Send(&__msg,__msg.dwSize);
					}
				}
			}

			if( MIsHolyType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				//if( !g_world.IsFabaoOpen() )
				//{
				//	p += *pLen;
				//	continue;
				//}
				tagHolyMan * pHoly = new tagHolyMan;
				pHoly->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
				pNewItem = pHoly;
				if( *pLen >= SIZE_HOLY )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_HOLY);

					//// 玩家每离线一分钟，圣灵的默契值减1，其最小值为0
					//if ( pHoly->nCoValue >= nOffMinute * 1)
					//{
					//	pHoly->nCoValue -=  nOffMinute;
					//}
					//else
					//{
					//	pHoly->nCoValue = 0;					
					//}	

					// 如果跨天登录，重置玩家 圣灵的每天吞噬妖精次数
					if( m_LogoutTime.year != m_LoginTime.year || m_LogoutTime.month != m_LoginTime.month || m_LogoutTime.day != m_LoginTime.day )
					{
						pHoly->nToDayEatNum = 0;
					}

					// 圣灵属性修改通知DB
					GetItemMgr().SendHolySpec2DB(*pHoly);
				}
				else // 可能是划拨过来的，但是也有可能是错误，目前按这种办法做吧
				{
					if( *pLen >= SIZE_EQUIP )
					{
						ILOG->Write(_T("ShengLingErrSize1 -%d\n"), *pLen);
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pHoly->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
						tagNDBC_NewHoly msg;
						memcpy(&msg.holy,pHoly,SIZE_HOLY);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						ILOG->Write(_T("ShengLingErrSize2 -%d\n"), *pLen);
						delete pHoly;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetHolyProto(pTmpItem->dwTypeID);
				//if( pHoly->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				//{
				//	ItemCreator::IdentifyEquip(pFabao);
				//	tagNDBC_UpdateEquipAtt send;
				//	send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
				//	IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

				//	g_dbSession.Send(&send, send.dwSize);
				//	GetItemMgr().SendFabaoSpec2DB(*pFabao);
				//}
				// 临时				
			}

			if( MIsHolyEquipType(pNewItem->pProtoType->eType) )
			{
				delete (tagEquip*)pNewItem ;
				//if( !g_world.IsFabaoOpen() )
				//{
				//	p += *pLen;
				//	continue;
				//}
				tagHolyEquip * pHolyEquip = new tagHolyEquip;
				pHolyEquip->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
				pNewItem = pHolyEquip;
				if( *pLen >= SIZE_HOLYEQUIP )
				{
					IFASTCODE->MemCpy(pNewItem, pTmpItem, SIZE_HOLYEQUIP);					
				}
				else // 可能是划拨过来的，但是也有可能是错误，目前按这种办法做吧
				{
					if( *pLen >= SIZE_EQUIP )
					{
						ILOG->Write(_T("ShengLingEquipErrSize1 -%d"), *pLen);
						IFASTCODE->MemCpy((tagEquip*)pNewItem, pTmpItem, nEquipSize);
						pHolyEquip->equipSpec.byQuality = GT_INVALID; // 给byte赋值，会变成255
						tagNDBC_NewHolyEquip msg;
						memcpy(&msg.holyEquip,pHolyEquip,SIZE_HOLYEQUIP);
						g_dbSession.Send(&msg, msg.dwSize);
					}
					else
					{
						ILOG->Write(_T("ShengLingEquipErrSize2 -%d"), *pLen);
						delete pHolyEquip;
						p += *pLen;
						continue;
					}
				}
				pNewItem->pProtoType = g_attRes.GetHolyEquipProto(pTmpItem->dwTypeID);
				//if( pHoly->equipSpec.byQuality >= EIQ_End /*|| pFabao->n16NativeIntelligence > 100 */|| pFabao->n16Stage > 150 || pFabao->nSlotNum > 5 )
				//{
				//	ItemCreator::IdentifyEquip(pFabao);
				//	tagNDBC_UpdateEquipAtt send;
				//	send.equipSpecUpdate.n64Serial = pFabao->n64Serial;
				//	IFASTCODE->MemCpy(&send.equipSpecUpdate.equipSpec, &pFabao->equipSpec, sizeof(tagEquipSpec));

				//	g_dbSession.Send(&send, send.dwSize);
				//	GetItemMgr().SendFabaoSpec2DB(*pFabao);
				//}
				// 临时				
			}


			//if( MIsFaBaoType(pNewItem->pProtoType->eType) )
			//	pTmpItem = (const tagItem*)((BYTE*)pTmpItem + SIZE_FABAO);
			//else
			//	pTmpItem = (const tagItem*)((BYTE*)pTmpItem + nEquipSize);
		}
		p += *pLen;

		if(!P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pNewItem->dwTypeID);
			IMSG(_T("InitItem failed The item(SerialNum: %lld) hasn't found proto type!\n"), pNewItem->n64Serial);
			Destroy(pNewItem);
			continue;
		}

		pNewItem->eStatus = EUDBS_Null;
		pNewItem->pScript = g_ScriptMgr.GetItemScript( pNewItem->dwTypeID);
		
		dwErrorCode = Put2Container(pNewItem);
		if(dwErrorCode != E_Success)
		{
			if(dwErrorCode != E_Item_Place_NotFree && dwErrorCode != E_Offline_Role_Sprite && dwErrorCode != E_Item_EquipBar_Error )
			{
				ASSERT(0);
				Destroy(pNewItem);
				continue;
			}

			listItem.PushBack(pNewItem);
		}
	}


	// 检查是否有因位置重复导致的不能添加到容器中
	while(listItem.Size() != 0)
	{
		pNewItem = listItem.PopFront();

		// 如果是背包或仓库中的物品，放入空位
		switch(pNewItem->eConType)
		{
		case EICT_Bag:
			IMSG(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			ILOG->Write(_T("Ignor bag overlap for role<roleid:%u> item<serial: %lld> Container<id:%u> Position<index:%u>\n"), m_dwID, pNewItem->n64Serial, pNewItem->eConType, pNewItem->n16Index);
			//Destroy(pNewItem);
			if (FALSE == GetItemMgr().GetBag().OverLappedItemHandler(pNewItem))
				GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
			break;
		case EICT_Equip:
// 			dwErrorCode = GetItemMgr().Add2Bag(pNewItem, (DWORD)ELCID_Bag_PosOverlap, FALSE, FALSE);
// 			if(E_Con_NotEnoughSpace == dwErrorCode)
// 			{
// 				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld\n> load failed!\n"), 
// 					EICT_Bag, pNewItem->n64Serial);
// 				IMSG(_T("Make some places and login again can solve this problem!\n"));				
// 			}
// 			if(dwErrorCode != E_Success)
// 				Destroy(pNewItem);
			if (FALSE == GetItemMgr().GetBag().OverLappedItemHandler(pNewItem))
				GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
			break;
		case EICT_RoleWare:
			dwErrorCode = GetItemMgr().Add2RoleWare(pNewItem, (DWORD)ELCID_RoleWare_PosOverlap, FALSE, FALSE);
			if(E_Con_NotEnoughSpace == dwErrorCode)
			{
				IMSG(_T("Because container<eType: %d> is full, item<serial: %lld> load failed!\n"), 
					EICT_RoleWare, pNewItem->n64Serial);
				IMSG(_T("Make some places and login again can solve this problem!\n"));
			}
			if(dwErrorCode != E_Success)
				if (FALSE == GetItemMgr().GetRoleWare().OverLappedItemHandler(pNewItem))
					GetItemMgr().GetBaiBaoBag().OverLappedItemHandler(pNewItem);
				//Destroy(pNewItem);
			break;
		case EICT_Family:
			{
				//if( GetFamilyID() > 0 )
				//{
				//	SetFamilyFabao(pNewItem);
				//}
				//else
				{//没有家族，返还家族妖精
					AddRoleSpriteFromFamily((tagFabao*)pNewItem);
					tagNDBC_DeleteRoleSpriteData send;
					send.dwRoleID = GetID();
					g_dbSession.Send( &send, send.dwSize );
				}
			}
			break;
		default:
			ASSERT(0);
			Destroy(pNewItem);
			continue;
		}
		
		ASSERT(E_Success == dwErrorCode);
	}

	// 重设指针
	//pData = (const BYTE*)pTmpItem;
	pData = p;
}

//-------------------------------------------------------------------------------
// 将从数据库中读入的物品(装备),放入到相应的容器中
//-------------------------------------------------------------------------------
DWORD Role::Put2Container(tagItem *pItem)
{
	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
	if (P_VALID(pItemScript))
	{
		if(pItemScript->CanDeleteItem(pItem->dwTypeID, pItem->n64Serial))
		{
			tagNDBC_DelItem send;
			send.n64Serial = pItem->n64Serial;
			g_dbSession.Send(&send, send.dwSize);
			return GT_INVALID;
		}
	}
	DWORD dwErrorCode = GetItemMgr().Put2Container(pItem);

	if(EICT_Equip == pItem->eConType && E_Success == dwErrorCode)
	{
		MTRANS_POINTER(p, pItem, tagEquip);
		ProcEquipEffectAtt(p, TRUE, p->n16Index);
		ResetOneEquipDisplay(p, p->n16Index);
		m_Suit.Add(p, p->n16Index, FALSE);
		m_LongHun.Add(p, p->n16Index, FALSE);
	}
	/*else if ( EICT_HolyEquip == pItem->eConType && E_Success == dwErrorCode)
	{
		MTRANS_POINTER(p, pItem, tagEquip);
		m_HolySoul.ProcEquipEffectAtt(p, TRUE, p->n16Index);
	}*/

	return dwErrorCode;
}

//-------------------------------------------------------------------------------
// 将从数据库中读入玩家物品冷却时间
//-------------------------------------------------------------------------------
VOID Role::InitItemCDTime(const BYTE* &pData, INT32 nNum)
{
	if(nNum <= 0)
	{
		return;
	}

	// 获取当前时间和该玩家上次下线的时间差(单位：秒)
	DWORD dwInterval = CalcTimeDiff(g_world.GetWorldTime(), m_LogoutTime);
	if(dwInterval > (DWORD)MAX_ITEM_DCTIME)
	{
		// 重设指针
		pData = pData + nNum * sizeof(tagCDTime);
		return;
	}

	// 转换成毫秒
	dwInterval *= 1000;
	
	ItemMgr &itemMgr = GetItemMgr();
	const tagCDTime *p = (const tagCDTime*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		if(p->dwTime > dwInterval && p->dwTime <= (DWORD)MAX_ITEM_DCTIME * 1000 )
		{
			itemMgr.Add2CDTimeMap(p->dwTypeID, p->dwTime - dwInterval);
		}

		++p;
	}

	// 重设指针
	pData = (const BYTE*)((tagCDTime *)pData + nNum);
	//pData = (const BYTE*)p;
}

//-------------------------------------------------------------------------------
// 初始化好友列表
//-------------------------------------------------------------------------------
VOID Role::InitFriend(const BYTE* &pData, INT32 nNum)
{
	for(INT i = 0; i < MAX_FRIENDNUM; ++i)
	{
		m_Friend[i].dwFriendID = GT_INVALID;
		m_Friend[i].dwFriVal = 0;
		m_Friend[i].byGroup = 1;
	}

	// 黑名单初时化
	memset(m_dwBlackList, 0XFF, sizeof(m_dwBlackList));
	if(nNum <= 0)
		return;

	const tagFriendSave	*pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendSave);
	pFriend = (const tagFriendSave*)pData;

	for(INT m = 0; m < nNum; ++m)
	{
		SetFriend(m, pFriend->dwFriendID, 0, pFriend->nGroupID);
		pFriend = (const tagFriendSave*)((BYTE*)pFriend + dwSizeFriend);
	}

	// 重设指针
	pData = (const BYTE*)((tagFriendSave*)pData + nNum);
	//pData = (const BYTE*)pFriend;
}

//-------------------------------------------------------------------------------
// 仇敌列表相关消息
//-------------------------------------------------------------------------------
VOID Role::InitEnemy(const BYTE* &pData, INT32 nNum)
{
	const DWORD *pEnemyList = NULL;
	DWORD dwSize = sizeof(DWORD);
	pEnemyList = (const DWORD*)pData;

	// 仇敌列表初时化
	memset(m_dwEnemyList, 0XFF, sizeof(m_dwEnemyList));

	for(INT i = 0; i < nNum; ++i)
	{
		SetEnemyList(i, *pEnemyList);
		pEnemyList = (const DWORD*)((BYTE*)pEnemyList + dwSize);
	}

	// 重设指针
	pData = (const BYTE*)((DWORD*)pData + nNum);
	//pData = (const BYTE*)pEnemyList;
}

//-------------------------------------------------------------------------------
// 初始化好友度
//-------------------------------------------------------------------------------
VOID Role::InitFriendValue(const BYTE* &pData, INT32 nNum)
{
	const tagFriendshipSave *pFriendSave = NULL;
	tagFriend *pFriend = NULL;
	DWORD dwSizeFriend = sizeof(tagFriendshipSave);
	pFriendSave = (const tagFriendshipSave*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		pFriend = m_mapFriend.Peek(pFriendSave->dwFriendID);

		if(P_VALID(pFriend))
			pFriend->dwFriVal = pFriendSave->nFriVal;

		pFriendSave = (const tagFriendshipSave*)((BYTE*)pFriendSave + dwSizeFriend);
	}

	// 重设指针
	pData = (const BYTE*)((tagFriendshipSave*)pData + nNum);
	//pData = (const BYTE*)pFriendSave;
}

//-------------------------------------------------------------------------------
// 初始化黑名单
//-------------------------------------------------------------------------------
VOID Role::InitBlackList(const BYTE* &pData, INT32 nNum)
{
	const DWORD *pBlackList = NULL;
	DWORD dwSize = sizeof(DWORD);
	pBlackList = (const DWORD*)pData;

	for(INT i = 0; i < nNum; ++i)
	{
		SetBlackList(i, *pBlackList);
		pBlackList = (const DWORD*)((BYTE*)pBlackList + dwSize);
	}

	// 重设指针
	pData = (const BYTE*)((DWORD*)pData + nNum);
	//pData = (const BYTE*)pBlackList;
}

//-------------------------------------------------------------------------------
// 初始化师徒
//-------------------------------------------------------------------------------
VOID Role::InitMaster(const BYTE* &pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	BOOL bIsMaster = IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID);
	if (bIsMaster)
	{
		m_nJingWuPoint	= p->nJingWuPoint;
	}

	m_nQinWuPoint	= p->nQinWuPoint;
	m_nLevelUpBonus = p->nLevelUpBonus;
	m_nRewardNum	= p->nRewardNum;
	m_ApprenticeData = p->sApprentice;

	for (int i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		if (m_ApprenticeData.dwData[i] != 0)
		{
			m_mapApprentice.Add(m_ApprenticeData.dwData[i], m_ApprenticeData.dwData[i]);
		}
	}

	// 重设指针
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagMasterApprentice2DB));

	// 读取师傅ID
	MTRANS_POINTER(pTmp, pData, tagMasterApprentice2DB);
	m_dwMasterID = pTmp->dwID;
	if (!bIsMaster && m_dwMasterID == GT_INVALID)
	{
		// 如果没有师傅且自身不是师傅则清空精武点数
		m_nJingWuPoint = 0;
	}

	// 重设指针
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagMasterApprentice2DB));
}

//-------------------------------------------------------------------------------
// 初始化同门
//-------------------------------------------------------------------------------
VOID Role::InitClassMates(const BYTE* &pData)
{
	MTRANS_POINTER(p, pData, tagClassMateData);
	m_ClassMateData = *p;

	// 重设指针
	pData = (const BYTE*)((BYTE*)pData + sizeof(tagClassMateData));
}

//-------------------------------------------------------------------------------
// 初始化实力值----不仅在角色初始化的时候用到，在每次保存角色信息的时候也用到
//-------------------------------------------------------------------------------
VOID Role::InitStrength()
{
	UpdateEquipValue();
	UpdateYaoJingValue();				// 计算妖精的实力值
	UpdateBaseStrength();
	UpdateAdvanceStrength();
	//UpdateConsumptiveStrength();
	UpdateStrength();
}

//-------------------------------------------------------------------------------
// 将从数据库中读入玩家当前的任务集合
//-------------------------------------------------------------------------------
VOID Role::InitCurrentQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestSave* pQuestSave = (const tagQuestSave*)pData;
	const tagQuestSave* pEndQuestSave = pQuestSave + nNum;
	const tagQuestProto* pProto = NULL;
	INT nIndex = 0;

	for ( ; pQuestSave < pEndQuestSave; ++pQuestSave)
	{
		pProto = g_questMgr.GetQuestProto(pQuestSave->u16QuestID);
		if( !P_VALID(pProto) ) 
		{
			MAttResCaution(_T("QuestFile"), _T("questID"), pQuestSave->u16QuestID);
			continue;
		}

		m_Quests[nIndex].Init(pQuestSave, this, nIndex);
		m_mapCurrentQuests.Add(m_Quests[nIndex].GetID(), &m_Quests[nIndex]);
		++nIndex;
	}
	
	pData = (const BYTE*)((tagQuestSave*)pData + nNum);
	//pData = (const BYTE *)pEndQuestSave;
}

//-------------------------------------------------------------------------------
// 将从数据库中读入玩家已经完成的任务集合
//-------------------------------------------------------------------------------
VOID Role::InitCompleteQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestDoneSave* pQuestDoneSave = (const tagQuestDoneSave*)pData;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagQuestDoneSave* pDoneQuest = new tagQuestDoneSave;
		IFASTCODE->MemCpy(pDoneQuest, pQuestDoneSave, sizeof(tagQuestDoneSave));
		m_mapCompleteQuests.Add(pDoneQuest->u16QuestID, pDoneQuest);
		
		++pQuestDoneSave;
	}

	// 重设指针
	pData = (const BYTE*)((tagQuestDoneSave*)pData + nNum);
	//pData = (const BYTE*)pQuestDoneSave;
}

//-------------------------------------------------------------------------------
// 将从数据库中读入玩家每日已经完成的任务次数集合
//-------------------------------------------------------------------------------
VOID Role::InitDailyCompleteQuest(const BYTE* &pData, INT32 nNum)
{
	const tagQuestDoneSave* pQuestDoneSave = (const tagQuestDoneSave*)pData;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagQuestDoneSave* pDoneQuest = new tagQuestDoneSave;
		IFASTCODE->MemCpy(pDoneQuest, pQuestDoneSave, sizeof(tagQuestDoneSave));
		m_mapDailyCompleteQuests.Add(pDoneQuest->u16QuestID, pDoneQuest);

		++pQuestDoneSave;
	}

	
	pData = (const BYTE*)((tagQuestDoneSave*)pData + nNum);
}



//-------------------------------------------------------------------------------
// 计算人物初始当前属性
//-------------------------------------------------------------------------------
VOID Role::CalInitAtt()
{
	// 先保存一些不随基础值改变的属性
	INT nHP				=	m_nAtt[ERA_HP];
	INT nMP				=	m_nAtt[ERA_MP];
	INT nVitality		=	m_nAtt[ERA_Vitality];
	INT nEndurance		=	m_nAtt[ERA_Endurance];
	INT nKnowledge		=	m_nAtt[ERA_Knowledge];
	INT nInjury			=	m_nAtt[ERA_Injury];
	INT nMorale			=	m_nAtt[ERA_Morale];
	INT nMorality		=	m_nAtt[ERA_Morality];
	INT nCulture		=	m_nAtt[ERA_Culture];
	INT nAttPoint		=	m_nAtt[ERA_AttPoint];
	INT nTalentPoint	=	m_nAtt[ERA_TalentPoint];
	INT nHostility		=	m_nAtt[ERA_Hostility];
	INT nRage			=	m_nAtt[ERA_Rage];
	INT nSpirit			=	m_nAtt[ERA_Spirit];
	INT nWuxun			=	m_nAtt[ERA_WuXun];
	INT nWuJi			=	m_nAtt[ERA_WuJi];
	INT nWeary			=	m_nAtt[ERA_Weary];
	INT nVipLevel		= m_nAtt[ERA_VipLevel];
	
	INT	nGodhead		= m_nAtt[ERA_God_Godhead];
	INT	nFaith			= m_nAtt[ERA_God_Faith];
	INT nCondenced		= m_nAtt[ERA_God_Condenced];

	INT nFlowerNum		= m_nAtt[ERA_FlowerNum];// 鲜花
	INT nEggNum			= m_nAtt[ERA_EggsNum];// 鸡蛋
	INT nHoly			= m_nAtt[ERA_Holy];	// 圣灵值
	// 妖精元素伤害，以及抗性
	//INT nEEI_Fire		=	m_nAtt[ERA_EEI_Fire];
	//INT nEEI_Water		=	m_nAtt[ERA_EEI_Water];
	//INT nEEI_Earth		=	m_nAtt[ERA_EEI_Earth];
	//INT nEEI_Wind		=	m_nAtt[ERA_EEI_Wind];
	//INT nEEI_Bright		=	m_nAtt[ERA_EEI_Bright];
	//INT nEEI_Dark		=	m_nAtt[ERA_EEI_Dark];
	//INT nEER_Fire		=	m_nAtt[ERA_EER_Fire];
	//INT nEER_Water		=	m_nAtt[ERA_EER_Water];
	//INT nEER_Earth		=	m_nAtt[ERA_EER_Earth];
	//INT nEER_Wind		=	m_nAtt[ERA_EER_Wind];
	//INT nEER_Bright		=	m_nAtt[ERA_EER_Bright];
	//INT nEER_Dark		=	m_nAtt[ERA_EER_Dark];

	for(INT n = 0; n < ERA_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nBaseAtt[n], n);
	}

	// 通过一级属性的当前值计算二级属性
	m_nAtt[ERA_MaxHP]		=	m_nAtt[ERA_Physique] * 6 + m_nBaseAtt[ERA_MaxHP];
	m_nAtt[ERA_MaxMP]		=	m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
	m_nAtt[ERA_ExAttack]	=	m_nAtt[ERA_Strength] * 10;
	m_nAtt[ERA_ExDefense]	=	m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
	m_nAtt[ERA_InAttack]	=	m_nAtt[ERA_InnerForce] * 10;
	m_nAtt[ERA_InDefense]	=	m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
	m_nAtt[ERA_AttackTec]	=	0;//m_nAtt[ERA_Technique] * 6;
	m_nAtt[ERA_DefenseTec]	=	0;//m_nAtt[ERA_Agility] * 4;
	m_nAtt[ERA_HitRate]		=	0;//m_nAtt[ERA_Technique] * 10;
	m_nAtt[ERA_Dodge]		=	0;//m_nAtt[ERA_Agility] * 10;
	const tagLevelUpEffect* pLevelupEff = g_attRes.GetLevelUpEffect(GetLevel());
	if( P_VALID(pLevelupEff) )
		m_nAtt[ERA_MaxEndurance]=	100 + pLevelupEff->n16Durance;
	else
		m_nAtt[ERA_MaxEndurance]=	0;//(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
	m_nAtt[ERA_WuXun]		=	nWuxun;
	m_nAtt[ERA_WuJi]		=	nWuJi;
	m_nAtt[ERA_Weary]		=	nWeary;
	m_nAtt[ERA_VipLevel] = nVipLevel;

	m_nAtt[ERA_God_Godhead] = nGodhead;
	m_nAtt[ERA_God_Faith] = nFaith;
	m_nAtt[ERA_God_Condenced] = nCondenced;

	m_nAtt[ERA_FlowerNum] = nFlowerNum;
	m_nAtt[ERA_EggsNum] = nEggNum;
	m_nAtt[ERA_Holy] = nHoly;

	// 计算二级属性加成
	for(INT n = ERA_AttB_Start; n < ERA_AttB_End; n++)
	{
		m_nAtt[n] = CalAttMod(m_nAtt[n], n);
	}

	//加上家族妖精的影响
	for(int i = 0; i < EFSA_Num; ++i)
	{
		if( m_nAbility[i] >= 0 )
		{
			ERoleAttribute eType = GetRoleIndexFromFamilySprite((EFamilySpriteAtt)i);
			if(eType > ERA_Null && eType < ERA_End)
			{
				m_nAtt[eType] += m_nAbility[i];
				SetRecalAbility((EFamilySpriteAtt)i,FALSE);
			}
		}
	}

	// 计算某些“当前属性”与最大值之间的最小值
	if( nHP < 0 )
	{
		m_nAtt[ERA_HP] = m_nAtt[ERA_MaxHP];
	}
	else
	{
		m_nAtt[ERA_HP] = min(nHP, m_nAtt[ERA_MaxHP]);
	}

	if( nMP < 0 )
	{
		m_nAtt[ERA_MP] = m_nAtt[ERA_MaxMP];
	}
	else
	{
		m_nAtt[ERA_MP] = min(nMP, m_nAtt[ERA_MaxMP]);
	}

	if( nVitality < 0 )
	{
		m_nAtt[ERA_Vitality] = m_nAtt[ERA_MaxVitality];
	}
	else
	{
		m_nAtt[ERA_Vitality] = min(nVitality, m_nAtt[ERA_MaxVitality]);
	}

	if( nEndurance < 0 )
	{
		m_nAtt[ERA_Endurance] = m_nAtt[ERA_MaxEndurance];
	}
	else
	{
		m_nAtt[ERA_Endurance] = min(nEndurance, m_nAtt[ERA_MaxEndurance]);
	}

	if(nSpirit < 0)
	{
		m_nAtt[ERA_Spirit] = g_attRes.GetAttMin(ERA_Spirit);
	}
	else
	{
		m_nAtt[ERA_Spirit] = min(nSpirit, g_attRes.GetAttMax(ERA_Spirit));
	}
	// 设置上不随基础属性改变的当前属性
	m_nAtt[ERA_Knowledge]	=	nKnowledge;
	m_nAtt[ERA_Injury]		=	nInjury;
	m_nAtt[ERA_Morale]		=	nMorale;
	m_nAtt[ERA_Morality]	=	nMorality;
	m_nAtt[ERA_Culture]		=	nCulture;
	m_nAtt[ERA_AttPoint]	=	nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	nTalentPoint;
	m_nAtt[ERA_Hostility]	=	nHostility;
	m_nAtt[ERA_Rage]		=	nRage;
	// 妖精元素伤害以及抗性
	//m_nAtt[ERA_EEI_Fire]		=	nEEI_Fire		;
	//m_nAtt[ERA_EEI_Water]		=	nEEI_Water	;
	//m_nAtt[ERA_EEI_Earth]		=	nEEI_Earth	;
	//m_nAtt[ERA_EEI_Wind]		=	nEEI_Wind		;
	//m_nAtt[ERA_EEI_Bright]	=	nEEI_Bright	;
	//m_nAtt[ERA_EEI_Dark]		=	nEEI_Dark		;
	//m_nAtt[ERA_EER_Fire]		=	nEER_Fire		;
	//m_nAtt[ERA_EER_Water]		=	nEER_Water	;
	//m_nAtt[ERA_EER_Earth]		=	nEER_Earth	;
	//m_nAtt[ERA_EER_Wind]		=	nEER_Wind		;
	//m_nAtt[ERA_EER_Bright]	=	nEER_Bright	;
	//m_nAtt[ERA_EER_Dark]		=	nEER_Dark		;

	// 设置某些随基本属性变化的属性
	m_fXZSpeed			= X_DEF_XZ_SPEED;
	m_fYSpeed			= X_DEF_Y_SPEED; 
	m_fSwimXZSpeed		= X_DEF_XZ_SPEED; 
	m_fDropXZSpeed		= X_DEF_DROP_XZ_SPEED; 
	m_fSlideSpeed		= X_DEF_SLIDE_SPEED;

	m_fMountXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Mount]) / 10000.0f;
	m_fXZSpeed *= FLOAT(m_nAtt[ERA_Speed_XZ]) / 10000.0f;
	m_fYSpeed *= FLOAT(m_nAtt[ERA_Speed_Y]) / 10000.0f;
	m_fSwimXZSpeed *= FLOAT(m_nAtt[ERA_Speed_Swim]) / 10000.0f;
	m_Size *= FLOAT(m_nAtt[ERA_Shape]) / 10000.0f;
	// todo: pk状态

	// 实力系统
	InitStrength();
	m_nAtt[ERA_Shili] = m_Strength.nStrength;

	//精力系统
	if(g_attRes.IsSpiritSysOn() && GT_INVALID != GetOnlineTime() && GetLevel() >= g_attRes.SpiritStartLevel()) //不是新玩家 级别大于40级
	{
		if(m_nAtt[ERA_Spirit] < g_attRes.GetAttDefRole(ERA_Spirit))	// 下线回复不超过默认点数
		{
			DWORD offlineTime = CalcTimeDiff(m_LoginTime, m_LogoutTime) / 60; // 计算上次离线时间
			if(offlineTime > 0)
			{
				m_nAtt[ERA_Spirit] += offlineTime * g_attRes.GetSpiritRSpeed(); // 按照速度 恢复到指定点数
				m_nAtt[ERA_Spirit] = min(m_nAtt[ERA_Spirit], g_attRes.GetAttDefRole(ERA_Spirit)); // 系统判断不超过默认点数
			}			
		}
		m_fSpiRate = CalSpiritRate(m_nAtt[ERA_Spirit]);	// 都要计算
	}
	else
	{
		m_nAtt[ERA_Spirit] = g_attRes.GetAttDefRole(ERA_Spirit);
	}

	//计算元神属性加成
	/*if ( m_HolySoul.IsActivated() )
	{
		m_HolySoul.HolyChangeRoleAtt();
	}*/


	// 清空重算字段
	ClearAttRecalFlag();
}

//-----------------------------------------------------------------------
// 计算初始状态
//-----------------------------------------------------------------------
VOID Role::CalInitState()
{
	// 计算PK状态
	CalPKState(FALSE);

	// 计算死亡状态
	if( GetAttValue(ERA_HP) <= 0 )
	{
		SetState(ES_Dead, FALSE);
	}
}

//------------------------------------------------------------------------
// 上线
//------------------------------------------------------------------------
VOID Role::Online(BOOL bFirst/* =FALSE */)
{
	if( bFirst )
		VirginOnline();

	// 调用上线的脚本函数
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleOnline(this);
	}
}

//------------------------------------------------------------------------
// 第一次上线
//------------------------------------------------------------------------
VOID Role::VirginOnline()
{
	// 设置在线时间
	m_nCurOnlineTime = 0;
	m_nOnlineTime = 0;

	// 设置出生地图和复活地图
	m_dwRebornMapID = m_dwMapID = g_mapCreator.GetBornMapID();

	// 得到一个随机的出生点
	Vector3 vBornPos = g_mapCreator.RandGetOneBornPos();

	// 随机一个朝向
	FLOAT fYaw = 180;//FLOAT(IUTIL->Rand() % 360);

	// 出生点
	m_MoveData.Reset(vBornPos.x, vBornPos.y, vBornPos.z, cosf(fYaw * 3.1415927f / 180.0f), 0.0f, sinf(fYaw * 3.1415927f / 180.0f));

	// 调用初次上限的脚本函数
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnRoleFirstOnline(this);
	}
}

//------------------------------------------------------------------------
// 初始化宝箱相关数据
//------------------------------------------------------------------------
VOID Role::InitChestState()
{
	m_TreasureState.nChestSerial = 0;
	m_TreasureState.nKeySerial = 0;
	m_TreasureState.dwChestTypeID = 0;
	m_TreasureState.dwKeyTypeID = 0;
	m_TreasureState.ChestItem.dwTypeID = 0;
	m_TreasureState.ChestItem.nNum = 0;
	m_TreasureState.ChestItem.nTableID = 0;
	m_TreasureState.ChestItem.fBeginRate = 0.0;
	m_TreasureState.ChestItem.fNormalRate = 0.0;
	m_TreasureState.ChestItem.fRoleRate = 0.0;
	m_TreasureState.ChestItem.fServerRate = 0.0;
	
}

VOID Role::InitTrainState(const BYTE* &pData, INT32 nNum)
{
	const tagTrainState* pTrainStateSave = (const tagTrainState*)pData;

	// 首先从试炼原型表中读取所有的试炼
	tagTrainAttProto* pTrainAtt = NULL;
	TMap<DWORD, tagTrainAttProto*>& TrainAttProto = g_attRes.GetTrainAttProto();
	TrainAttProto.ResetIterator();
	while (TrainAttProto.PeekNext(pTrainAtt))
	{
		if (!P_VALID(pTrainAtt)) return;

		tagTrainState* pTrainState = new tagTrainState;
		if (!P_VALID(pTrainState))	return;
		
		pTrainState->dwTrainID = pTrainAtt->dwTrainID;
		pTrainState->eTrainState = ETS_Close;
		pTrainState->nCompleteness = 0;

		m_mapTrainStates.Add(pTrainState->dwTrainID, pTrainState);
	}

	// 从数据库中读取的数据合并到m_mapTrainStates里面去
	for(INT32 i = 0; i < nNum; ++i)
	{
		if (P_VALID(pTrainStateSave))
		{
			tagTrainState * pTrainState = m_mapTrainStates.Peek(pTrainStateSave->dwTrainID);
			if (P_VALID(pTrainState))
			{
				pTrainState->eTrainState = pTrainStateSave->eTrainState;
				pTrainState->nCompleteness = pTrainStateSave->nCompleteness;
			}
			++pTrainStateSave;
		}
	}

	// 重设指针
	pData = (const BYTE*)((tagTrainState*)pData + nNum);

	// 给客户端发送试炼状态表
	SendTrainStateList2Client();
}


VOID Role::InitCurMonthOnlineTime(const BYTE* &pData, INT nNum)
{
	tagMonthOnlineTime* pMonthOnlineTime = (tagMonthOnlineTime*)pData;
	if (P_VALID(pMonthOnlineTime) && nNum == 1)
	{
		for (int nMonthIdx = EM_January; nMonthIdx < EM_End; nMonthIdx++)
		{	
			m_CurMonthOnlineTime.n16OnlineTime[nMonthIdx] = (nNum > 0)?pMonthOnlineTime->n16OnlineTime[nMonthIdx]:0;
		}
		m_CurMonthOnlineTime.dwCurTime = GetCurrentDWORDTime();
	}
	// 重设指针
	pData = (const BYTE*)((tagMonthOnlineTime*)pData + nNum);
}
//---------------------------------------------------------------------------------
// 初始化领域
VOID Role::InitArea(const BYTE* &pData, INT32 nNum)
{
	//设置除神职领域意外的领域
	m_GodAreaMgr.InitFromDB(pData,nNum);
	//添加稀有领域特效buff
	m_GodAreaMgr.AddAreaBuff();//add by xp
}
//---------------------------------------------------------------------------------
VOID Role::SetFamilyFabao(tagItem *pItem)
{
	if (!P_VALID(pItem)) 
		return;

	if(!MIsFaBao(pItem))
		return;

	//目前有没有登记的妖精
	// 检查家族
	//Family *pFamily = g_FamilyMgr.GetFamily(GetFamilyID());
	//if( !P_VALID(pFamily) )
	//	return;

	//// 检查修炼内容
	//FamilySprite* pFamilySprite = pFamily->GetFamilySprite();
	//if( !P_VALID(pFamilySprite) )
	//	return;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	pFamilySprite->SetRoleSprite( GetID(), (tagFabao*)pItem );

	//pFamilySprite->UpdateRoleAtt( GetID() );
}
//---------------------------------------------------------------------------------
//初始化玩家的家族妖精增加的能力属性
VOID Role::InitAbilityFromFamilySprite()
{
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(GetFamilyID());
	if( !P_VALID(pFamilySprite) )
		return;

	pFamilySprite->CaculateRoleAtt(GetID(),this);
}
//---------------------------------------------------------------------------------
VOID Role::InitBuyMallItemCount(const BYTE* &pData, INT32 nNum)
{
	if (nNum <= 0)
	{
		return;
	}

	tagBuyMallItemCountSaveData* p = (tagBuyMallItemCountSaveData*)pData;
	for (int i=0; i<nNum; ++i)
	{
		tagBuyMallItemCountSaveData* pTmp = p+i;
		if (P_VALID(pTmp->dwTypeID) && P_VALID(pTmp->nNum))
		{
			m_mapBuyMallItemCount.Add(pTmp->dwTypeID, pTmp->nNum);
		}
	}

	pData = (const BYTE*)((tagBuyMallItemCountSaveData*)pData + nNum);
}

VOID Role::ClearBuyMallItemCount()
{
	m_mapBuyMallItemCount.Clear();
}


//-------------------------------------------------------------------------------
// 初始化签元神信息
//-------------------------------------------------------------------------------
VOID Role::InitHolySoulInfo(const BYTE* &pData,INT nOffHour/*离线小时*/)
{
	const tagHolySoulInfoEx* pHolySoulInfo = (const tagHolySoulInfoEx*)pData;

	DWORD dwRet = m_HolySoul.Init(pHolySoulInfo,nOffHour);
	if ( E_Success != dwRet )
	{
		IMSG(_T(" HolySoul Init Failed!"));   
		ILOG->Write(_T(" HolySoul Init Failed!")); 
	}

	//重设指针
	pData = (const BYTE*)((tagHolySoulInfoEx*)pData + nOffHour);
}

//-------------------------------------------------------------------------------
// 激活元神
//-------------------------------------------------------------------------------
VOID	Role::ActiveHolySoul()
{
	bool bIsSoulActivated = m_HolySoul.IsActivated();
	if ( !bIsSoulActivated )
	{
		//激活元神
		DWORD dwRet = m_HolySoul.ActivateSoul();
		if ( E_Success == dwRet )
		{
			tagNS_ActivateHolySoul msg_HolySoul;
			SendMessage(&msg_HolySoul,msg_HolySoul.dwSize);
		}
	}

}