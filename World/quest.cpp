//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: quest.h
// actor:
// data: 2008-9-1
// last:
// brief: 任务类
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/quest.h"

#include "role.h"
#include "quest.h"
#include "quest_mgr.h"
#include "script_mgr.h"

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Quest::Quest()
: ScriptData<ESD_Quest>(),m_pProto(NULL), m_pOwner(NULL),m_dwStartTime(GT_INVALID), m_nIndex(GT_INVALID), m_pScript(NULL), m_pDynamicTarget(NULL), m_bSpecTarget(FALSE)
{
	memset(m_n16CreatureCount, 0, sizeof(m_n16CreatureCount));
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Quest::~Quest()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------------
VOID Quest::Init(const tagQuestProto* pQuestProto, Role* pRole, INT nIndex)
{
	ASSERT( P_VALID(pQuestProto) && P_VALID(pRole) );
	ASSERT( nIndex >= 0 && nIndex < QUEST_MAX_COUNT );

	m_pProto = pQuestProto;
	m_pOwner = pRole;
	m_nIndex = nIndex;
	m_dwStartTime = g_world.GetWorldTime();
	m_pDynamicTarget = NULL;
	m_bComplete = FALSE;

	// 初始化各种任务状态
	memset(m_n16CreatureCount, 0, sizeof(m_n16CreatureCount));
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));
	m_n16FriendsNum = 0;
	m_byTeammatesNum = 0;
	m_nChuanDaoCount = 0;

	// 初始化脚本
	m_pScript = g_ScriptMgr.GetQuestScript(m_pProto->id);

	// 初始化任务动态目标
	if(EQTM_DYNAMIC == pQuestProto->target_mode)
	{
		m_pDynamicTarget = new tagQuestDynamicTarget;

		// 初始化脚本动态目标
		if(P_VALID(m_pDynamicTarget) && P_VALID(m_pScript))
		{
			m_pScript->OnInit(pQuestProto->id, m_pOwner);
		}
	}
	
	// 任务板任务
	if ( IsBoardQuest(pQuestProto->id) || IsYellowListQuest(pQuestProto->id))
	{
		SAFE_DEL(m_pDynamicTarget);
		EQuestBoardType eQuestBoardType = m_pOwner->GetQuestBoardType(pQuestProto->id);
		m_pDynamicTarget = new tagQuestDynamicTarget;
		if (P_VALID(m_pDynamicTarget))
		{
			InitBoardQuest(pQuestProto->id, m_pDynamicTarget, eQuestBoardType);
		}

		// 改变任务板任务的状态
		m_pOwner->GetBoardQuestMgr(eQuestBoardType)->ChangeQuestState(pQuestProto->id, EQS_HaveAccept);
	}

	// 组装玩家身上的物品个数
	InitQuestItem();
	InitFriendsNum();
	InitTeammatesNum();
}

VOID Quest::Init(const tagQuestSave* pQuestSave, Role* pRole, INT nIndex)
{
	ASSERT( P_VALID(pQuestSave) && P_VALID(pRole) );
	ASSERT( nIndex >= 0 && nIndex < QUEST_MAX_COUNT );

	m_pProto = g_questMgr.GetQuestProto(pQuestSave->u16QuestID);
	if( !P_VALID(m_pProto) ) return;

	m_pOwner = pRole;
	m_nIndex = nIndex;
	m_dwStartTime = pQuestSave->dwAcceptTime;
	m_pDynamicTarget = NULL;
	m_bComplete = FALSE;
	m_bSpecTarget = pQuestSave->bQuestFlag ? true : false;

	// 初始化各种任务状态
	IFASTCODE->MemCpy(m_n16CreatureCount, pQuestSave->n16MonsterNum, sizeof(m_n16CreatureCount));
	m_nChuanDaoCount = pQuestSave->nChuandaoCount;
	IFASTCODE->MemCpy(m_ScriptData.dwData, pQuestSave->Data.dwData, sizeof(DWORD)*ESD_Quest);
	memset(m_bSpeakNPC, false, sizeof(m_bSpeakNPC));
	memset(m_bTrigger, false, sizeof(m_bTrigger));
	memset(m_n16ItemCount, 0, sizeof(m_n16ItemCount));
	memset(m_bInvest, false, sizeof(m_bInvest));

	// 初始化脚本
	m_pScript = g_ScriptMgr.GetQuestScript(m_pProto->id);

	// 初始化任务动态目标
	if(EQTM_DYNAMIC == m_pProto->target_mode)
	{
		m_pDynamicTarget = new tagQuestDynamicTarget;

		if(!P_VALID(m_pDynamicTarget))
			return;

		m_pDynamicTarget->eTargetType = pQuestSave->DynamicTarget.eTargetType;
		m_pDynamicTarget->dwQuestTipsID = pQuestSave->DynamicTarget.dwQuestTipsID;
		IFASTCODE->MemCpy(m_pDynamicTarget->dwTargetID, pQuestSave->DynamicTarget.dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(m_pDynamicTarget->nTargetNum, pQuestSave->DynamicTarget.nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		m_pDynamicTarget->nRewMoney = pQuestSave->DynamicTarget.nRewMoney;
		m_pDynamicTarget->nRewExp = pQuestSave->DynamicTarget.nRewExp;
		IFASTCODE->MemCpy(m_pDynamicTarget->dwRewItemID, pQuestSave->DynamicTarget.dwRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(m_pDynamicTarget->dwRewItemNum, pQuestSave->DynamicTarget.dwRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(m_pDynamicTarget->dwOptionRewItemID, pQuestSave->DynamicTarget.dwOptionRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(m_pDynamicTarget->dwOptionRewItemNum, pQuestSave->DynamicTarget.dwOptionRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}

	// 组装玩家身上的物品个数
	InitQuestItem();
	InitFriendsNum();
	InitTeammatesNum();
}

//--------------------------------------------------------------------------------
// 销毁
//--------------------------------------------------------------------------------
VOID Quest::Destroy()
{
	m_pOwner = NULL;
	m_pProto = NULL;
	m_dwStartTime = GT_INVALID;
	m_nIndex = GT_INVALID;

	m_pScript = NULL;

	SAFE_DEL(m_pDynamicTarget);
}

//--------------------------------------------------------------------------------
// 保存
//--------------------------------------------------------------------------------
VOID Quest::InitQuestSave(OUT tagQuestSave* pQuestSave)
{
	pQuestSave->u16QuestID = GetID();
	pQuestSave->dwAcceptTime = GetAcceptTime();
	pQuestSave->bQuestFlag = m_bSpecTarget;
	pQuestSave->nChuandaoCount = m_nChuanDaoCount;
	IFASTCODE->MemCpy(pQuestSave->n16MonsterNum, m_n16CreatureCount, sizeof(pQuestSave->n16MonsterNum));
	IFASTCODE->MemCpy(pQuestSave->Data.dwData, m_ScriptData.dwData, sizeof(DWORD)*ESD_Quest);

	if(P_VALID(m_pDynamicTarget))
	{
		pQuestSave->DynamicTarget.eTargetType = m_pDynamicTarget->eTargetType;
		pQuestSave->DynamicTarget.dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		pQuestSave->DynamicTarget.nRewMoney = m_pDynamicTarget->nRewMoney;
		pQuestSave->DynamicTarget.nRewExp = m_pDynamicTarget->nRewExp;
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwRewItemID, m_pDynamicTarget->dwRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwRewItemNum, m_pDynamicTarget->dwRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwOptionRewItemID, m_pDynamicTarget->dwOptionRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pQuestSave->DynamicTarget.dwOptionRewItemNum, m_pDynamicTarget->dwOptionRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}
}

//--------------------------------------------------------------------------------
// 生成消息结构
//--------------------------------------------------------------------------------
VOID Quest::GenerateMsgInfo(tagIncompleteQuestMsgInfo* pInfo, DWORD &dwMsgSize, DWORD &dwQuestSize, Role* pRole)
{
	ASSERT( IsValid() );
	dwQuestSize = sizeof(tagIncompleteQuestMsgInfo);

	pInfo->u16QuestID = m_pProto->id;
	pInfo->dwAcceptTime = m_dwStartTime;
	IFASTCODE->MemCpy(pInfo->n16CreatureNum, m_n16CreatureCount, sizeof(pInfo->n16CreatureNum));
	IFASTCODE->MemCpy(pInfo->n16ItemNum, m_n16ItemCount, sizeof(pInfo->n16ItemNum));

	// 是否为传道任务
	if (P_VALID(this->GetProto()->chuandao_count))
	{
		pInfo->n16KillTimes = (INT16)m_nChuanDaoCount;
	}

	// 检查看该任务是否是特殊完成条件任务
	if (m_pProto->spec_condition)
	{
		//CHAR szFuncName[X_SHORT_NAME] = {0};
		//sprintf( szFuncName, "q%d_CheckComplete", m_pProto->id );
		//_sntprintf(szFuncName, X_SHORT_NAME, _T("q%d_CheckComplete"), m_pProto->id);
		const CHAR * strFunctionName = NULL;
		const QuestScript*	pQuestScript	= g_ScriptMgr.GetQuestScript(m_pProto->id);
		if( P_VALID(pQuestScript) )
		{
			if(P_VALID((*pQuestScript)[ESQE_On_CheckComplete]))
				strFunctionName = (*pQuestScript)[ESQE_On_CheckComplete];
		}
		INT nRet = 0;

		Map* pMap = pRole->GetMap();
		if(P_VALID(pMap) && P_VALID(strFunctionName))
		{
 			g_ScriptMgr.CallScriptFunction(const_cast<CHAR*>(strFunctionName), "uuuuu>i", pMap->GetMapID(), pMap->GetInstanceID(), m_pProto->id, pRole->GetID(), NULL, &nRet);
			pInfo->bCompleteState = nRet ? 1 : 0;
		}
	}

	if(EQTM_DYNAMIC == m_pProto->target_mode)
	{
		pInfo->DynamicTarget.eTargetType = m_pDynamicTarget->eTargetType;
		pInfo->DynamicTarget.dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->DynamicTarget.nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		pInfo->DynamicTarget.nRewMoney = m_pDynamicTarget->nRewMoney;
		pInfo->DynamicTarget.nRewExp = m_pDynamicTarget->nRewExp;
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwRewItemID, m_pDynamicTarget->dwRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwRewItemNum, m_pDynamicTarget->dwRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwOptionRewItemID, m_pDynamicTarget->dwOptionRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->DynamicTarget.dwOptionRewItemNum, m_pDynamicTarget->dwOptionRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}
	else
	{
		// 修正消息大小
		dwQuestSize-= sizeof(tagQuestDynamicTarget); 
		dwMsgSize -= sizeof(tagQuestDynamicTarget);
	}
}

//---------------------------------------------------------------------------------
// 得到初始物品列表
//---------------------------------------------------------------------------------
VOID Quest::GetInitQuestItem(INT16* const n16ItemCount)
{
	ASSERT( IsValid() );

	IFASTCODE->MemCpy(n16ItemCount, m_n16ItemCount, sizeof(m_n16ItemCount));
}

//---------------------------------------------------------------------------------
// 得到动态目标
//---------------------------------------------------------------------------------
VOID Quest::GetInitDynamicTarget(tagQuestDynamicTarget* pInfo, DWORD &dwSize)
{
	if (P_VALID(m_pProto))
	{
		if(EQTM_DYNAMIC == m_pProto->target_mode)
		{
			pInfo->eTargetType = m_pDynamicTarget->eTargetType;
			pInfo->dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
			IFASTCODE->MemCpy(pInfo->dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
			IFASTCODE->MemCpy(pInfo->nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
			pInfo->nRewMoney = m_pDynamicTarget->nRewMoney;
			pInfo->nRewExp = m_pDynamicTarget->nRewExp;
			IFASTCODE->MemCpy(pInfo->dwRewItemID, m_pDynamicTarget->dwRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
			IFASTCODE->MemCpy(pInfo->dwRewItemNum, m_pDynamicTarget->dwRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
			IFASTCODE->MemCpy(pInfo->dwOptionRewItemID, m_pDynamicTarget->dwOptionRewItemID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
			IFASTCODE->MemCpy(pInfo->dwOptionRewItemNum, m_pDynamicTarget->dwOptionRewItemNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
		}
		else
		{
			dwSize -= sizeof(tagQuestDynamicTarget);
		}
	}
	/*else if (P_VALID(m_pBoradProto))
	{
		pInfo->eTargetType = m_pDynamicTarget->eTargetType;
		pInfo->dwQuestTipsID = m_pDynamicTarget->dwQuestTipsID;
		IFASTCODE->MemCpy(pInfo->dwTargetID, m_pDynamicTarget->dwTargetID, sizeof(DWORD)*DYNAMIC_TARGET_COUNT);
		IFASTCODE->MemCpy(pInfo->nTargetNum, m_pDynamicTarget->nTargetNum, sizeof(INT)*DYNAMIC_TARGET_COUNT);
	}*/
}

//---------------------------------------------------------------------------------
// 所需要的任务物品是否已经足够
//---------------------------------------------------------------------------------
BOOL Quest::IsQuestItemFull(DWORD dwItemTypeID)
{
	if( FALSE == IsValid() ) return FALSE;

	// 如果该任务没有获得物品，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) ) return FALSE;

	// 检测任务物品是否已满
	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
			//break;

		if( m_pProto->complete_req_item[n] != dwItemTypeID )
			continue;

		return m_n16ItemCount[n] >= m_pProto->complete_req_item_num[n];
	}

	return FALSE;
}

//---------------------------------------------------------------------------------
// 任务现在是否可以完成
//---------------------------------------------------------------------------------
INT Quest::IsCanComplete(INT nChoiceItemIndex, Creature* pNPC)
{
	BOOL bComplete = FALSE;

	// 特殊完成条件的任务
	if (m_pProto->spec_condition)
	{
		if( P_VALID(m_pScript) )
		{
			bComplete = m_pScript->CheckComplete(m_pProto->id, m_pOwner, pNPC);
		}
		else if (IsBoardQuest(this->GetID()))
		{
			EQuestBoardType eType = m_pOwner->GetQuestBoardType(this->GetID());
			if (eType!=EQBT_Null)
			{
				UINT16 u16BoardID = m_pOwner->GetBoardQuestMgr(eType)->GetBoardIDFromID(this->GetID());

				const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
				if(P_VALID(pScript)) 
					bComplete = pScript->CheckComplete(u16BoardID, m_pOwner, pNPC);
			}
		}

		return bComplete ? E_Success : E_SystemError;
	}
	
	if( m_pProto->only_one_condition )
	{
		bComplete = IsCanCompleteCreatureKill(TRUE)	||
					IsCanCompleteItem(TRUE)			||
					IsCanCompleteNPCTalk(TRUE)		||
					IsCanCompleteTrigger(TRUE)		||
					IsCanCompleteMoney(TRUE)		||
					IsCanCompleteLevel(TRUE)		||
//					IsCanCompleteInvest(TRUE)		||
					IsCanCompleteMap(TRUE)			||
					IsCanCompleteFriends(TRUE)		||
					IsCanCompleteTeammates(TRUE);
	}
	else
	{
		bComplete = IsCanCompleteCreatureKill()	&&
					IsCanCompleteItem()			&&
					IsCanCompleteNPCTalk()		&&
					IsCanCompleteTrigger()		&&
					IsCanCompleteMoney()		&&
					IsCanCompleteLevel()		&&
//					IsCanCompleteInvest()		&&
					IsCanCompleteMap()			&&
					IsCanCompleteFriends()		&&
					IsCanCompleteTeammates();
	}

	if( P_VALID(m_pScript) )
	{
		bComplete = bComplete && m_pScript->CheckComplete(m_pProto->id, m_pOwner, pNPC);
	}
	else if (IsBoardQuest(this->GetID()))
	{
		EQuestBoardType eType = m_pOwner->GetQuestBoardType(this->GetID());
		if (eType!=EQBT_Null)
		{
			UINT16 u16BoardID = m_pOwner->GetBoardQuestMgr(eType)->GetBoardIDFromID(this->GetID());

			const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
			if(P_VALID(pScript)) 
				bComplete = bComplete && pScript->CheckComplete(u16BoardID, m_pOwner, pNPC);
		}
	}

	if (!bComplete)
	{
		return E_SystemError;
	}

	if (IsBoardQuest(this->GetID()))
	{
		EQuestBoardType eType = m_pOwner->GetQuestBoardType(this->GetID());
		if(eType == EQBT_Null) return E_SystemError;

		if(!IsCanCompleteBoardReward(eType) )
			return E_CanCompleteQuest_BAG_FULL;
		else
			return E_Success;
	}
	else
	{
		if(!IsCanCompleteReward(nChoiceItemIndex))
			return E_CanCompleteQuest_BAG_FULL;
		else
			return E_Success;
	}
	
}

//----------------------------------------------------------------------------------
// 事件触发
//----------------------------------------------------------------------------------
BOOL Quest::OnEvent(EQuestEvent eQuestType, DWORD dwEventMisc1/*=0*/, DWORD dwEventMisc2/*=0*/, DWORD dwEventMisc3/*=0*/)
{
	switch(eQuestType)
	{
	case EQE_Kill:
		return OnEventCreatureKill(dwEventMisc1, dwEventMisc2);
		break;

	case EQE_Item:
		OnEventItem(dwEventMisc1, dwEventMisc2, dwEventMisc3);
		break;

	case EQE_Talk:
		OnEventNPCTalk(dwEventMisc1, dwEventMisc2);
		break;

	case EQE_Trigger:
		OnEventTrigger(dwEventMisc1);
		break;

	case EQE_Invest:
		OnEventInvest(dwEventMisc1,dwEventMisc2);
		break;

	case EQE_DlgDefault:
		OnEventDlgDefault(dwEventMisc1);
		break;

	case EQE_Friends:
		OnEventFriends();
		break;

	case EQE_Teammates:
		OnEventTeammates();
		break;
	
	case EQE_Level:
		OnEventLevel();
		break;

	default:
		break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------------
// 杀怪事件
//------------------------------------------------------------------------------------
BOOL Quest::OnEventCreatureKill(DWORD dwCreatureTypeID, INT nCreatureLevel)
{
	if( !P_VALID(m_pProto) ) return FALSE;

	// 调用脚本
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnCreatureKill(m_pProto->id, m_pOwner, dwCreatureTypeID);
	}

	// 动态击杀目标计数
	if(P_VALID(m_pDynamicTarget) && EQTT_Kill == m_pDynamicTarget->eTargetType)
	{
		if(!IsBoardQuest(GetID()) && !IsYellowListQuest(GetID()))
		{
			for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
			{
				BOOL bFind = FALSE;
				BOOL bFull = FALSE;
				if(m_pDynamicTarget->dwTargetID[i] == dwCreatureTypeID)
				{
					m_n16CreatureCount[i] += 1;

					if(m_n16CreatureCount[i] > m_pDynamicTarget->nTargetNum[i])
						bFull = TRUE;

					bFind = TRUE;
				}

				if( bFind )
				{
					if (!bFull)
						m_pOwner->OnChuanDaoQuest(ECDT_KillTime);

					// 发送更新
					tagNS_QuestUpdateKillCreature send;
					send.u16QuestID = GetID();
					send.nCreatureIndex = i;
					send.n16NewKillNum = m_n16CreatureCount[i];
					m_pOwner->SendMessage(&send, send.dwSize);

					// 找到了，就不需要往下找了
					//break;
					return ( bFind && !bFull);
				}
			}

			return FALSE;
		}
		else // 任务板任务
		{
			for(int i = 0; i < BOARD_QUEST_MONSTER_NUM; ++i)
			{
				BOOL bFind = FALSE;
				BOOL bFull = FALSE;

				if(m_pDynamicTarget->dwTargetID[i] == dwCreatureTypeID)
				{
					m_n16CreatureCount[i] += 1;

					if(m_n16CreatureCount[i] > m_pDynamicTarget->nTargetNum[i])
						bFull = TRUE;

					bFind = TRUE;
				}

				if( bFind )
				{
					if (!bFull)
						m_pOwner->OnChuanDaoQuest(ECDT_KillTime);

					// 发送更新
					tagNS_QuestUpdateKillCreature send;
					send.u16QuestID = GetID();
					send.nCreatureIndex = i;
					send.n16NewKillNum = m_n16CreatureCount[i];
					m_pOwner->SendMessage(&send, send.dwSize);

					// 找到了，就不需要往下找了
					//break;
					return ( bFind && !bFull);
				}
			}
			
			return FALSE;
		}
	}


	// 如果该任务没有杀怪，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_KILL) ) return FALSE;
	
	for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
	{
		if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
			break;

		BOOL bFind = FALSE;
		BOOL bFull = FALSE;
		if( m_pProto->creature_level )
		{
			if( m_pProto->complete_req_creature[n*3]	== nCreatureLevel ||
				m_pProto->complete_req_creature[n*3+1]	== nCreatureLevel ||
				m_pProto->complete_req_creature[n*3+2]	== nCreatureLevel
			)
			{
				if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
				{
					m_n16CreatureCount[n] += 1;
				}
				else
				{
					bFull = TRUE;
				}

				bFind = TRUE;
			}
		}
		else
		{
			if( m_pProto->complete_req_creature[n*3]	== dwCreatureTypeID ||
				m_pProto->complete_req_creature[n*3+1]	== dwCreatureTypeID ||
				m_pProto->complete_req_creature[n*3+2]	== dwCreatureTypeID
				)
			{
				if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
				{
					m_n16CreatureCount[n] += 1;					
				}
				else
				{
					bFull = TRUE;
				}

				bFind = TRUE;
			}
		}

		if( bFind )
		{
			if(!bFull) 
				m_pOwner->OnChuanDaoQuest(ECDT_KillTime);

			// 发送更新
			tagNS_QuestUpdateKillCreature send;
			send.u16QuestID = GetID();
			send.nCreatureIndex = n;
			send.n16NewKillNum = m_n16CreatureCount[n];
			m_pOwner->SendMessage(&send, send.dwSize);

			// 找到了，就不需要往下找了
			//break;
			return ( bFind && !bFull);
		}
	}
	return FALSE;
}

//-------------------------------------------------------------------------------
// 物品事件
//-------------------------------------------------------------------------------
VOID Quest::OnEventItem(DWORD dwItemTypeID, INT nNum, BOOL bAdd)
{
	if( !P_VALID(m_pProto) ) return;
	if( nNum <= 0 ) return;
	if( m_bComplete ) return;

	// 动态收集物品计数
	if(P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;
			if(m_pDynamicTarget->dwTargetID[i] == dwItemTypeID)
			{
				/*if( bAdd) m_n16ItemCount[i] += nNum;
				else	  m_n16ItemCount[i] -= nNum;*/
				m_n16ItemCount[i] = m_pOwner->GetItemMgr().GetBagSameItemCount(dwItemTypeID)
					+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(dwItemTypeID);
				bFind = TRUE;
			}

			if( bFind )
			{
				// 发送更新
				tagNS_QuestUpdateItem send;
				send.u16QuestID = GetID();
				send.nItemIndex = i;
				send.n16NewItemNum = m_n16ItemCount[i];
				m_pOwner->SendMessage(&send, send.dwSize);

				// 找到了，就不需要往下找了
				break;
			}
		}

		return;
	}

	// 如果该任务没有获得物品，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) ) return;

	// 依次找对应的物品
	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
			//break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_item[n] == dwItemTypeID )
		{
			/*if( bAdd )	m_n16ItemCount[n] += nNum;
			else		m_n16ItemCount[n] -= nNum;*/
			m_n16ItemCount[n] = m_pOwner->GetItemMgr().GetBagSameItemCount(dwItemTypeID)
				+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(dwItemTypeID);

			bFind = TRUE;
		}

		if( bFind )
		{
			// 发送更新
			tagNS_QuestUpdateItem send;
			send.u16QuestID = GetID();
			send.nItemIndex = n;
			send.n16NewItemNum = m_n16ItemCount[n];
			m_pOwner->SendMessage(&send, send.dwSize);

			// 找到了，就不需要往下找了
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// NPC对话事件
//-----------------------------------------------------------------------------
VOID Quest::OnEventNPCTalk(DWORD dwNPCID, DWORD dwNPCTypeID)
{
	if( !P_VALID(m_pProto) ) return;

	// 动态NPC对话统计
	if(P_VALID(m_pDynamicTarget) && EQTT_NPCTalk == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;

			if(m_pDynamicTarget->dwTargetID[i] == dwNPCTypeID)
			{
				if( !m_bSpeakNPC[i] )
				{
					m_bSpeakNPC[i] = true;
					bFind = TRUE;
				}
			}
			
			if( bFind )
			{
				// 发送更新
				tagNS_QuestUpdateNPCTalk send;
				send.u16QuestID = GetID();
				send.nIndex = i;
				send.dwErrorCode = E_Success;
				m_pOwner->SendMessage(&send, send.dwSize);

				// 调用NPC对话脚本
				if( P_VALID(m_pScript) )
				{
					m_pScript->OnNPCTalk(m_pProto->id, m_pOwner, dwNPCID, dwNPCTypeID);
				}

				// 找到了，就不需要往下找了
				break;
			}
		}

		return;
	}

	// 如果该任务没有获得物品，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_NPC_TALK) ) return;

	// 依次找对应的物品
	for(INT n = 0; n < QUEST_NPC_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_npc[n]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_npc[n] == dwNPCTypeID )
		{
			if( !m_bSpeakNPC[n] )
			{
				m_bSpeakNPC[n] = true;
				bFind = TRUE;
			}
		}
		else
		{
			// 跟NPC的ID不一样，如果该NPC之前没有对过话，则不再向后检查了
			if( m_pProto->only_in_order )
			{
				if( false == m_bSpeakNPC[n] )
				{
					tagNS_QuestUpdateNPCTalk send;
					send.u16QuestID = GetID();
					send.nIndex = n;
					send.dwErrorCode = E_CanUpdateQuest_Fail;
					m_pOwner->SendMessage(&send, send.dwSize);

					break;
				}
			}
		}

		if( bFind )
		{
			// 发送更新
			tagNS_QuestUpdateNPCTalk send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			send.dwErrorCode = E_Success;
			m_pOwner->SendMessage(&send, send.dwSize);

			// 调用NPC对话脚本
			if( P_VALID(m_pScript) )
			{
				m_pScript->OnNPCTalk(m_pProto->id, m_pOwner, dwNPCID, dwNPCTypeID);
			}


			// 找到了，就不需要往下找了
			break;
		}
	}

	// 没有找到，也返回客户端
	tagNS_QuestUpdateNPCTalk send;
	send.u16QuestID = GetID();
	send.nIndex = GT_INVALID;
	send.dwErrorCode = E_CanUpdateQuest_Fail;
	m_pOwner->SendMessage(&send, send.dwSize);
}

//--------------------------------------------------------------------------------
// 触发器事件
//--------------------------------------------------------------------------------
VOID Quest::OnEventTrigger(DWORD dwTriggerSerial)
{
	if( !P_VALID(m_pProto) ) return;

	// 如果该任务没有获得物品，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_Trigger) ) return;

	// 依次找对应的物品
	for(INT n = 0; n < QUEST_TRIGGERS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_trigger[n]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->complete_req_trigger[n] == dwTriggerSerial )
		{
			m_bTrigger[n] = true;
			bFind = TRUE;
		}

		if( bFind )
		{
			// 发送更新
			tagNS_QuestUpdateTrigger send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			m_pOwner->SendMessage(&send, send.dwSize);

			// 找到了，就不需要往下找了
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// 调查地物事件
//--------------------------------------------------------------------------------
VOID Quest::OnEventInvest(DWORD dwNPCID, DWORD dwNPCTypeID)
{
	if( !P_VALID(m_pProto) ) return;

	// 动态NPC对话统计
	if(P_VALID(m_pDynamicTarget) && EQTT_Invest == m_pDynamicTarget->eTargetType)
	{
		for(INT i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			BOOL bFind = FALSE;

			if( m_pDynamicTarget->dwTargetID[i] == dwNPCTypeID )
			{
				if( !m_bInvest[i] )
				{
					m_bInvest[i] = true;
					bFind = TRUE;
				}
			}

			if( bFind )
			{
				// 发送更新
				tagNS_QuestUpdateInveset send;
				send.u16QuestID = GetID();
				send.nIndex = i;
				m_pOwner->SendMessage(&send, send.dwSize);

				if(P_VALID(m_pScript))
				{
					m_pScript->OnInvest(m_pProto->id, m_pOwner, dwNPCTypeID);
				}
				else if (IsBoardQuest(this->GetID()))
				{
					EQuestBoardType eType = m_pOwner->GetQuestBoardType(this->GetID());
					if (eType!=EQBT_Null)
					{
						UINT16 u16BoardID = m_pOwner->GetBoardQuestMgr(eType)->GetBoardIDFromID(this->GetID());

						const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
						if(P_VALID(pScript)) 
							pScript->OnInvest(u16BoardID, m_pOwner, dwNPCTypeID);
					}
				}

				// 找到了，就不需要往下找了
				break;
			}
		}

		return;
	}

	// 如果该任务没有获得物品，则直接返回
	if( !(m_pProto->quest_flags & Quest_Flag_Invest) ) return;

	// 依次找对应的物品
	for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->investigate_objects[n]) && !P_VALID(m_pProto->investigate_objects[n+4]) )
			break;

		BOOL bFind = FALSE;

		if( m_pProto->investigate_objects[n] == dwNPCTypeID || m_pProto->investigate_objects[n+4] == dwNPCTypeID)
		{
			m_bInvest[n] = true;
			bFind = TRUE;
		}

		if( bFind )
		{
			// 发送更新
			tagNS_QuestUpdateInveset send;
			send.u16QuestID = GetID();
			send.nIndex = n;
			m_pOwner->SendMessage(&send, send.dwSize);

			if(P_VALID(m_pScript))
			{
				m_pScript->OnInvest(m_pProto->id, m_pOwner, dwNPCTypeID);
			}
			else if (IsBoardQuest(this->GetID()))
			{
				EQuestBoardType eType = m_pOwner->GetQuestBoardType(this->GetID());
				if (eType!=EQBT_Null)
				{
					UINT16 u16BoardID = m_pOwner->GetBoardQuestMgr(eType)->GetBoardIDFromID(this->GetID());

					const QuestScript* pScript = g_ScriptMgr.GetQuestScript(u16BoardID);
					if(P_VALID(pScript)) 
						pScript->OnInvest(u16BoardID, m_pOwner, dwNPCTypeID);
				}
			}

			// 找到了，就不需要往下找了
			break;
		}
	}
}

//--------------------------------------------------------------------------------
// 服务器可控对话框缺省事件
//--------------------------------------------------------------------------------
VOID Quest::OnEventDlgDefault(DWORD dwDlgOption)
{
	if(!P_VALID(m_pScript))		return;

	m_pScript->OnDlgDefault(m_pProto->id, m_pOwner, dwDlgOption);
}

VOID Quest::OnEventFriends()
{
	if (m_pProto->complete_req_map == 0)
	{
		return;
	}

	tagNS_QuestUpdateFriends send;
	send.u16QuestID = GetID();
	send.nFriendsNum = m_pOwner->GetFriendCount();
	m_pOwner->SendMessage(&send, send.dwSize);
}

VOID Quest::OnEventTeammates()
{
	if (m_pProto->complete_req_map_x == 0)
	{
		return;
	}

	DWORD dwTeamID = m_pOwner->GetTeamID();
	BYTE byTeammates = 0;
	if (dwTeamID != GT_INVALID)
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if (!P_VALID(pTeam)) return;

		byTeammates = pTeam->GetMemNum();

		if (byTeammates>0)
		{
			byTeammates -= 1;
		}
	}
	
	tagNS_QuestUpdateTeammates send;
	send.u16QuestID = GetID();
	send.byTeammatesNum = byTeammates;
	
	m_pOwner->SendMessage(&send, send.dwSize);
}

VOID Quest::OnEventLevel()
{
	if (m_pProto->complete_req_level == 0)
	{
		return;
	}

	tagNS_QuestUpdateLevel send;
	send.u16QuestID = GetID();
	send.nLevel = m_pOwner->GetLevel();
	m_pOwner->SendMessage(&send, send.dwSize);
}

BOOL Quest::IsCanCompleteRewardEx(INT nChoiceItemIndex)
{
	return IsCanCompleteReward(nChoiceItemIndex);
}
//--------------------------------------------------------------------------------
// 初始化任务所需要的物品
//--------------------------------------------------------------------------------
VOID Quest::InitQuestItem()
{
	if( !P_VALID(GetProto()) ) return;

	// 判断动态收集的物品
	if (P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				break;

			m_n16ItemCount[i]  = m_pOwner->GetItemMgr().GetBagSameItemCount(m_pDynamicTarget->dwTargetID[i])
				+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(m_pDynamicTarget->dwTargetID[i]);
		}

		return;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) )
		return;

	for(INT n = 0; n < QUEST_ITEMS_COUNT; ++n)
	{
		if( !P_VALID(m_pProto->complete_req_item[n]) )
			break;

		m_n16ItemCount[n] = m_pOwner->GetItemMgr().GetBagSameItemCount(m_pProto->complete_req_item[n])
						+ m_pOwner->GetItemMgr().GetQuestBagSameItemCount(m_pProto->complete_req_item[n]);
	}
}

VOID Quest::InitFriendsNum()
{
	if (!P_VALID(m_pProto) || !P_VALID(m_pOwner))
	{
		return;
	}
	
	m_n16FriendsNum = m_pOwner->GetFriendCount();
}

VOID Quest::InitTeammatesNum()
{
	if (!P_VALID(m_pProto) || !P_VALID(m_pOwner))
	{
		return;
	}

	DWORD dwTeamID = m_pOwner->GetTeamID();
	if (dwTeamID != GT_INVALID)
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if (!P_VALID(pTeam)) return;

		m_byTeammatesNum = pTeam->GetMemNum()-1;

	}
}

//----------------------------------------------------------------------------------
// 杀怪是否满足
//----------------------------------------------------------------------------------
BOOL Quest::IsCanCompleteCreatureKill(BOOL bOnlyOne)
{
	// 判断动态击杀目标
	if (P_VALID(m_pDynamicTarget) && EQTT_Kill == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				continue;

			if(m_n16CreatureCount[i] < m_pDynamicTarget->nTargetNum[i])
				return FALSE;
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_KILL) )
		return (bOnlyOne ? FALSE : TRUE);

	// 满足一个就可以
	if( m_pProto->only_one_creature )
	{
		for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
				break;

			if( m_n16CreatureCount[n] >= m_pProto->complete_req_creature_num[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// 全部
	else
	{
		for(INT n = 0; n < QUEST_CREATURES_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_creature[n*3]) )
				break;

			if( m_n16CreatureCount[n] < m_pProto->complete_req_creature_num[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// 物品是否满足
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteItem(BOOL bOnlyOne)
{
	// 判断动态收集的物品
	if (P_VALID(m_pDynamicTarget) && EQTT_Collect == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->nTargetNum[i] == 0)
				continue;

			if(m_n16ItemCount[i] < m_pDynamicTarget->nTargetNum[i])
				return FALSE;
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_ITEM) )
		return (bOnlyOne ? FALSE : TRUE);

	// 满足一个就可以
	if( m_pProto->only_one_item )
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
				//break;

			if( m_n16ItemCount[n] >= m_pProto->complete_req_item_num[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// 全部
	else
	{
		for(INT n = 0; n < QUEST_ITEMS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_item[n]) ) continue;
				//break;

			if( m_n16ItemCount[n] < m_pProto->complete_req_item_num[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// NPC是否满足
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteNPCTalk(BOOL bOnlyOne)
{
	if (P_VALID(m_pDynamicTarget) && EQTT_NPCTalk == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->dwTargetID[i] == 0)
				continue;

			if( !m_bSpeakNPC[i])
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_NPC_TALK) )
		return (bOnlyOne ? FALSE : TRUE);

	// 满足一个就可以
	if( m_pProto->only_one_npc )
	{
		for(INT n = 0; n < QUEST_NPC_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_npc[n]) )
				break;

			if( m_bSpeakNPC[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// 全部
	else
	{
		for(INT n = 0; n < QUEST_NPC_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_npc[n]) )
				break;

			if( !m_bSpeakNPC[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// 触发器是否满足
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteTrigger(BOOL bOnlyOne)
{
	if( !(m_pProto->quest_flags & Quest_Flag_Trigger) )
		return (bOnlyOne ? FALSE : TRUE);

	// 满足一个就可以
	if( m_pProto->only_one_trigger )
	{
		for(INT n = 0; n < QUEST_TRIGGERS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_trigger[n]) )
				break;

			if( m_bTrigger[n] )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	// 全部
	else
	{
		for(INT n = 0; n < QUEST_TRIGGERS_COUNT; n++)
		{
			if( !P_VALID(m_pProto->complete_req_trigger[n]) )
				break;

			if( !m_bTrigger[n] )
			{
				return FALSE;
			}	
		}
		return TRUE;
	}
}

//----------------------------------------------------------------------------
// 调查地物是否满足
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteInvest(BOOL bOnlyOne)
{
	if (P_VALID(m_pDynamicTarget) && EQTT_Invest == m_pDynamicTarget->eTargetType)
	{
		for(int i = 0; i < DYNAMIC_TARGET_COUNT; ++i)
		{
			if(m_pDynamicTarget->dwTargetID[i] == GT_INVALID)
				continue;

			if( !m_bInvest[i])
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	if( !(m_pProto->quest_flags & Quest_Flag_Invest) )
		return (bOnlyOne ? FALSE : TRUE);

	for(INT n = 0; n < DYNAMIC_TARGET_COUNT; n++)
	{
		if( !P_VALID(m_pProto->investigate_objects[n]) && !P_VALID(m_pProto->investigate_objects[n+4]) )
			break;

		if( !m_bInvest[n] )
		{
			return FALSE;
		}	
	}
	return TRUE;
}

//----------------------------------------------------------------------------
// 金钱
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteMoney(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_money )
		return (bOnlyOne ? FALSE : TRUE);

	if( m_pOwner->GetCurMgr().GetBagSilver() < m_pProto->complete_req_money )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------
// 等级
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteLevel(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_level )
		return (bOnlyOne ? FALSE : TRUE);

	if( m_pOwner->GetLevel() < m_pProto->complete_req_level )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------
// 地图
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteMap(BOOL bOnlyOne)
{
	// todo:
	return TRUE;
}

//----------------------------------------------------------------------------
// 好友
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteFriends(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_map )
		return (bOnlyOne ? FALSE : TRUE);

	if( m_pOwner->GetFriendCount() < m_pProto->complete_req_map )
		return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------------
// 队友
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteTeammates(BOOL bOnlyOne)
{
	if( 0 == m_pProto->complete_req_map_x )
		return (bOnlyOne ? FALSE : TRUE);
	
	DWORD dwTeamID = m_pOwner->GetTeamID();
	if (dwTeamID != GT_INVALID)
	{
		const Team* pTeam = g_groupMgr.GetTeamPtr(dwTeamID);
		if (!P_VALID(pTeam)) return FALSE;

		if(pTeam->GetMemNum() < m_pProto->complete_req_map_x)
			return FALSE;
		else
			return TRUE;
		
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
// 奖励
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteReward(INT nChoiceItemIndex)
{
	INT16 n16BagSpace = 0;
	INT16 n16QuestSpace = 0;
	INT16 n16WarBagSpace = 0;
	INT16 n16TmpBagSpace = 0;
	INT16 n16TmpQuestSpace = 0;
	INT16 n16TmpWarBagSpace = 0;
	INT16 n16TmpMaxDummy;
	
	// 查看奖励的物品背包能否放得下
	for(INT n = 0; n < QUEST_REW_ITEM; ++n)
	{
		if( !P_VALID(m_pProto->rew_item[n]) )
			break;

		// 得到这个物品会在背包里占用的个数
		if(!ItemMgr::CalSpaceUsed(m_pProto->rew_item[n], 
			m_pProto->rew_item_num[n], n16TmpBagSpace, n16TmpQuestSpace, n16TmpWarBagSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
		n16WarBagSpace	+= n16TmpWarBagSpace;
	}

	// 如果有职业奖励，则查看奖励物品中的最大个数
	for(INT m = 0; m < QUEST_CLASS_REW_COUNT; ++m)
	{
		if( !P_VALID(m_pProto->rew_class_item[m].Item->dwItemID) )
			break;

		// 得到这个物品会在背包里占用的个数
		tagClassReward Reward = m_pProto->rew_class_item[m];
		if(!ItemMgr::CalSpaceUsed(Reward.Item->dwItemID, Reward.Item->byNum, n16TmpBagSpace, n16TmpQuestSpace, n16TmpWarBagSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
		n16WarBagSpace	+= n16TmpWarBagSpace;

		break;	// 找到了一种职业奖励，累加其奖励物品的个数之后即可退出不找了，因为我们认为每种职业奖励的物品个数是相同的
	}

	// 如果有可选奖励，则查看奖励物品中的最大个数
	INT nTmp = nChoiceItemIndex;
	if(nTmp >= 0 && nTmp < QUEST_REW_ITEM
		&& P_VALID(m_pProto->rew_choice_item[nTmp]))
	{
		// 得到这个物品会在背包里占用的个数
		if(!ItemMgr::CalSpaceUsed(m_pProto->rew_choice_item[nTmp], 
			m_pProto->rew_choice_item_num[nTmp], n16TmpBagSpace, n16TmpQuestSpace, n16TmpWarBagSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
		n16WarBagSpace	+= n16TmpWarBagSpace;
	}

	// 得到背包里空闲的栏位能否加上这些物品
	if( m_pOwner->GetItemMgr().GetBagFreeSize() < n16BagSpace
		|| m_pOwner->GetItemMgr().GetQuestItemBagFreeSize() < n16QuestSpace)
	{
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 任务板任务奖励
//----------------------------------------------------------------------------
BOOL Quest::IsCanCompleteBoardReward(EQuestBoardType eType)
{
	// 根据任务ID获得对应的任务板ID
	if (!P_VALID(m_pOwner->GetBoardQuestMgr(eType)))
	{
		return FALSE;
	}

	UINT16 uBoardID = m_pOwner->GetBoardQuestMgr(eType)->GetBoardIDFromID(this->GetID());

	tagBoardQuestProto* pProto = g_attRes.GetBoardQuestProto(uBoardID);
	if (!P_VALID(pProto))
	{
		return FALSE;
	}

	INT16 n16BagSpace = 0;
	INT16 n16QuestSpace = 0;
	INT16 n16WarBagSpace = 0;
	INT16 n16TmpBagSpace = 0;
	INT16 n16TmpQuestSpace = 0;
	INT16 n16TmpWarBagSpace = 0;
	INT16 n16TmpMaxDummy;

	// 查看奖励的物品背包能否放得下
	for(INT n = 0; n < BOARD_QUEST_REWARD_NUM; ++n)
	{
		if( !P_VALID(pProto->RewardItem[n].dwID) )
			break;

		// 得到这个物品会在背包里占用的个数
		if(!ItemMgr::CalSpaceUsed(pProto->RewardItem[n].dwID, 
			pProto->RewardItem[n].byNum, n16TmpBagSpace, n16TmpQuestSpace, n16TmpWarBagSpace, n16TmpMaxDummy))
		{
			return FALSE;
		}

		n16BagSpace		+= n16TmpBagSpace;
		n16QuestSpace	+= n16TmpQuestSpace;
		n16WarBagSpace	+= n16TmpWarBagSpace;
	}

	// 得到背包里空闲的栏位能否加上这些物品
	if( m_pOwner->GetItemMgr().GetBagFreeSize() < n16BagSpace
		|| m_pOwner->GetItemMgr().GetQuestItemBagFreeSize() < n16QuestSpace)
	{
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------------
// 任务接取时
//---------------------------------------------------------------------------------
VOID Quest::OnAccept(Creature* pNPC)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnAccept(m_pProto->id, m_pOwner, pNPC);
	}
}

//----------------------------------------------------------------------------------
// 任务完成时
//----------------------------------------------------------------------------------
VOID Quest::OnComplete(Creature* pNPC)
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnComplete(m_pProto->id, m_pOwner, pNPC);
	}
}

//-----------------------------------------------------------------------------------
// 任务取消时
//-----------------------------------------------------------------------------------
VOID Quest::OnCancel()
{
	if( P_VALID(m_pScript) )
	{
		m_pScript->OnCancel(m_pProto->id, m_pOwner);
	}
}

VOID Quest::InitBoardQuest(UINT16 u16ID, tagQuestDynamicTarget* pTarget, EQuestBoardType eQuestBoardType)
{
	// 根据任务ID获得其任务板上的ID
	UINT16 u16BoardID = m_pOwner->GetBoardQuestMgr(eQuestBoardType)->GetBoardIDFromID(u16ID);

	tagBoardQuestProto* pProto = g_attRes.GetBoardQuestProto(u16BoardID);
	if (!P_VALID(pProto)) return;

	pTarget->eTargetType = pProto->eTarget;	// 任务动态目标类型
	//pTarget->dwQuestTi

	if(EQTT_Invest == pProto->eTarget)
	{
		for(INT i = 0; i < BOARD_QUEST_INVEST_NUM; ++i)
		{
			pTarget->dwTargetID[i] = pProto->dwInvestItem[i];
		}
	}
	else if(EQTT_Kill == pProto->eTarget)
	{
		for(INT n = 0; n < BOARD_QUEST_MONSTER_NUM; ++n)
		{
			// todo 应对应两种怪
			pTarget->dwTargetID[n] = pProto->TargetMonster[n].dwTargetMonsterID_A;
			pTarget->nTargetNum[n] = pProto->TargetMonster[n].byTargetMonsterNum;
		}
	}
	else if(EQTT_NPCTalk == pProto->eTarget)
	{
		for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
		{
			if( n < BOARD_QUEST_NPC_NUM )
				pTarget->dwTargetID[n] = pProto->dwTalkToNPC[n];
			else
				pTarget->dwTargetID[n] = 0;
		}
	}
	else if(EQTT_Collect == pProto->eTarget)
	{
		for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
		{
			if( n < BOARD_QUEST_GATHER_NUM )
			{
				pTarget->dwTargetID[n] = pProto->GatherItem[n].dwID;
				pTarget->nTargetNum[n] = pProto->GatherItem[n].byNum;
			}
			else
			{
				pTarget->dwTargetID[n] = 0;
				pTarget->nTargetNum[n] = 0;
			}
		}
	}
}

BOOL Quest::DynamicRewardQuest(Role* pRole, INT32 nChoiceItemIndex)
{
	if (!P_VALID(m_pDynamicTarget))
	{
		return FALSE;
	}

	// 奖励物品
	for(INT n = 0; n < DYNAMIC_TARGET_COUNT; ++n)
	{
		if( !P_VALID(m_pDynamicTarget->dwRewItemID[n]) ) break;

		if( m_pDynamicTarget->dwRewItemNum[n] <= 0 ) continue;

		if( MIsEquipment(m_pDynamicTarget->dwRewItemID[n]))
		{
			pRole->RewardEquip(EICM_Quest, this->GetID(), m_pDynamicTarget->dwRewItemID[n], (INT16)m_pDynamicTarget->dwRewItemNum[n], EIQ_White, ELCID_Quest_Complete);
		}
		else
		{
			pRole->GetItemMgr().Add2Role(EICM_Quest, this->GetID(), m_pDynamicTarget->dwRewItemID[n], (INT16)m_pDynamicTarget->dwRewItemNum[n], EIQ_White, ELCID_Quest_Complete);
		}

	}

	// 可选奖励物品
	if( nChoiceItemIndex >= 0 && nChoiceItemIndex < QUEST_REW_ITEM )
	{
		if( P_VALID(m_pDynamicTarget->dwOptionRewItemID[nChoiceItemIndex]) &&
			m_pDynamicTarget->dwOptionRewItemNum[nChoiceItemIndex] > 0 )
		{
			if( MIsEquipment(m_pDynamicTarget->dwOptionRewItemID[nChoiceItemIndex]))
			{
				pRole->RewardEquip(EICM_Quest, this->GetID(), m_pDynamicTarget->dwOptionRewItemID[nChoiceItemIndex], 
					(INT16)m_pDynamicTarget->dwOptionRewItemNum[nChoiceItemIndex], 
					EIQ_White, ELCID_Quest_Complete);
			}
			else
			{
				pRole->GetItemMgr().Add2Role(EICM_Quest, this->GetID(), m_pDynamicTarget->dwOptionRewItemID[nChoiceItemIndex], 
					(INT16)m_pDynamicTarget->dwOptionRewItemNum[nChoiceItemIndex], 
					EIQ_White, ELCID_Quest_Complete);
			}
		}
	}

	// 经验
	if( m_pDynamicTarget->nRewExp != 0 )
	{
		pRole->ExpChange((INT64)m_pDynamicTarget->nRewExp);
	}

	// 金钱
	if( m_pDynamicTarget->nRewMoney > 0 )
	{
		pRole->GetCurMgr().IncBagSilver(m_pDynamicTarget->nRewMoney, (DWORD)ELCID_Quest_Complete);
		
		if(P_VALID(g_ScriptMgr.GetRoleScript()))
			g_ScriptMgr.GetRoleScript()->IncSilverByQuest( pRole, GetID(), m_pDynamicTarget->nRewMoney );
	}

	return TRUE;
}

VOID Quest::IncChuanDaoCount()
{
	++m_nChuanDaoCount;	

	tagNS_ChuanDaoQuestUpdateCount send;
	send.dwRoleID = m_pOwner->GetID();
	send.u16QuestID = GetID();
	send.nNewNum = m_nChuanDaoCount;
	m_pOwner->SendMessage(&send, send.dwSize);
}