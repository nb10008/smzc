#include "stdafx.h"
#include "att_res.h"
#include "role.h"
#include "quest_board.h"

class Role;

BoardQuestMgr::BoardQuestMgr(Role* pRole)
{
	m_pRole = pRole;
	m_nCompleteRefreshTime = 0;
	m_nIMRefreshTime = 0;
	m_n16QuestPanelStartTaskID = 0;
	m_eQuestBoardType = EQBT_Null;
	//m_byLevel = 0;
	m_BoardQuest.Clear();
}

BoardQuestMgr::~BoardQuestMgr()
{
	tagRoleBoardQuest* p = NULL;
	RoleBoardQuestMap::TMapIterator it = m_BoardQuest.Begin();
	while (m_BoardQuest.PeekNext(it, p))
	{
		if (P_VALID(p)) SAFE_DEL(p);
	}

	m_BoardQuest.Clear();
}

BOOL BoardQuestMgr::Init(const BYTE* &pData, INT nNum, Role* pRole, BOOL bMovePointer/* = TRUE*/)
{
	const tagBoardQuestSaveData* pQuest = (const tagBoardQuestSaveData*)pData;
	tagBoardQuestSaveData _data;

	for(INT32 i = 0; i < nNum; ++i)
	{
		tagBoardQuestSaveData* pNewQuest = &_data;
		if (P_VALID(pNewQuest))
		{
			IFASTCODE->MemCpy(pNewQuest, pQuest, sizeof(tagBoardQuestSaveData));
			if (pNewQuest->byType == m_eQuestBoardType)
				Add(pNewQuest->u16QuestID, pNewQuest->u16BoardID, pNewQuest->eState);
		}

		++pQuest;
	}

	// 重设指针
	if (bMovePointer)
		pData = (const BYTE*)((tagBoardQuestSaveData*)pData + nNum);

	return TRUE;
}

BOOL BoardQuestMgr::IsHaveThisQuest(UINT16 u16BoardID)
{
	RoleBoardQuestMap::TMapIterator it = m_BoardQuest.Begin();
	tagRoleBoardQuest* pQuest = NULL;
	while (m_BoardQuest.PeekNext(it, pQuest))
	{
		if (pQuest->u16BoardID == u16BoardID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

tagBoardQuestRandomRate* BoardQuestMgr::GetRewardQuestRule(EBoardRefreshType eType, BOOL bSecond/* 是否要使用二次刷新规则*/)
{
	//	刷新规则如下：
	//	第一次品级生成几率			0
	//	交付任务默认品级刷新几率	1
	//	交付任务二次品级刷新几率	2
	//	道具刷新默认品级刷新几率	3
	//	道具刷新二次品级刷新几率	4
	//	总完成次数为5的整数倍		5
	//	总完成次数为10的整数倍		6
	//	总完成次数为50的整数倍		7
	//	立即刷新总次数为10的整数倍	8
	//	---- 第一条规则是不会被获得的，只是在首次打开任务板的时候使用该规则，已写死在程序中
	if (bSecond)
	{
		if (eType == EBRT_CompleteQuest)
		{
			return g_attRes.GetBoardQuestRule(2);
		}
		else if(eType == EBRT_IM)
		{
			return g_attRes.GetBoardQuestRule(4);
		}
	}

	if (eType == EBRT_CompleteQuest)
	{
		if (m_nCompleteRefreshTime%50 == 0)
		{
			return g_attRes.GetBoardQuestRule(7);
		}
		else if (m_nCompleteRefreshTime%10 == 0)
		{
			return g_attRes.GetBoardQuestRule(6);
		}
		else if (m_nCompleteRefreshTime%5 == 0)
		{
			return g_attRes.GetBoardQuestRule(5);
		}
		else
		{
			return g_attRes.GetBoardQuestRule(1);
		}
	}
	else if(eType == EBRT_IM)
	{
		if (m_nIMRefreshTime%10 == 0)
		{
			return g_attRes.GetBoardQuestRule(8);
		}
		else
		{
			return g_attRes.GetBoardQuestRule(3);
		}
	}
	else
	{
		return g_attRes.GetBoardQuestRule(0);
	}
}
tagBoardQuestRandomRate* BoardQuestMgr::GetYellowListRule(EBoardRefreshType eType, BOOL bSecond/* 是否要使用二次刷新规则*/)
{
	//	刷新规则如下：
	//	第一次品级生成几率			0
	//	交付任务默认品级刷新几率	9
	//	交付任务二次品级刷新几率	10
	//	道具刷新默认品级刷新几率	11
	//	道具刷新二次品级刷新几率	12
	//	总完成次数为5的整数倍		13
	//	总完成次数为10的整数倍		14
	//	总完成次数为50的整数倍		15
	//	立即刷新总次数为10的整数倍	16
	//	---- 第一条规则是不会被获得的，只是在首次打开任务板的时候使用该规则，已写死在程序中
	if (bSecond)
	{
		if (eType == EBRT_CompleteQuest)
		{
			return g_attRes.GetBoardQuestRule(10);
		}
		else if(eType == EBRT_IM)
		{
			return g_attRes.GetBoardQuestRule(12);
		}
	}

	if (eType == EBRT_CompleteQuest)
	{
		if (m_nCompleteRefreshTime%50 == 0)
		{
			return g_attRes.GetBoardQuestRule(15);
		}
		else if (m_nCompleteRefreshTime%10 == 0)
		{
			return g_attRes.GetBoardQuestRule(14);
		}
		else if (m_nCompleteRefreshTime%5 == 0)
		{
			return g_attRes.GetBoardQuestRule(13);
		}
		else
		{
			return g_attRes.GetBoardQuestRule(9);
		}
	}
	else if(eType == EBRT_IM)
	{
		if (m_nIMRefreshTime%10 == 0)
		{
			return g_attRes.GetBoardQuestRule(16);
		}
		else
		{
			return g_attRes.GetBoardQuestRule(11);
		}
	}
	else
	{
		return g_attRes.GetBoardQuestRule(9);
	}
}

tagBoardQuestRandomRate* BoardQuestMgr::GetNowtimeRule(EBoardRefreshType eType, BOOL bSecond)
{
	switch (m_eQuestBoardType)
	{
	case EQBT_YellowList:
		return GetYellowListRule(eType, bSecond);
	case EQBT_Null:
	case EQBT_Reward:
	default:
		return GetRewardQuestRule(eType, bSecond);
	}
}

BOOL BoardQuestMgr::RefreshBoardByQuestColor(Role* pRole, BYTE byLevel, EBoardQuestQuality eColor, INT nNum, UINT16 u16ID)
{
	TMap<UINT16, tagBoardQuestProto*>& mapAllQuest = g_attRes.GetBoardQuestMap();
	TMap<UINT16, tagBoardQuestProto*>::TMapIterator it = mapAllQuest.Begin();
	TList<tagBoardQuestProto*> tempList; // 可接任务列表
	UINT16 u16BoardID = 0;		
	tagBoardQuestProto* pTmp = NULL;
	INT nCount = 0;

	// 建立可接任务列表
	while(mapAllQuest.PeekNext(it, u16BoardID, pTmp))
	{
		if (pTmp->byLevel<byLevel-1 || pTmp->byLevel>byLevel+1)
		{
			continue;
		}

		// 第一次打开任务板，只能随出白色任务
		if (pTmp->eQuality != eColor)
		{
			continue;
		}

		if (pTmp->byType != m_eQuestBoardType)
		{
			continue;
		}

		// 已经随出过的任务不要再出
		if(pRole->GetBoardQuestMgr(m_eQuestBoardType)->IsHaveThisQuest(u16BoardID))
		{
			continue;
		}

		tempList.PushBack(pTmp);
	}

	// 添加任务
	if(nNum >= tempList.Size()) 
	{
		// 可接任务少于接受的任务时全盘接受
		while(!tempList.Empty())
		{
			pTmp = tempList.PopFront();
			pRole->GetBoardQuestMgr(m_eQuestBoardType)->Add(u16ID + nCount, pTmp->u16ID);
			nCount++;
		}

		return FALSE;
	}
	else
	{
		while (nCount < nNum)
		{
			if( tempList.RandPeek(pTmp, TRUE) )
			{
				pRole->GetBoardQuestMgr(m_eQuestBoardType)->Add(u16ID + nCount, pTmp->u16ID);
				++nCount; // 随到一个，计数加一
			}
			else
			{
				ASSERT(0);
			}

			if (nCount >= nNum)
			{
				break; // 已随满
			}
		}

		return TRUE;
	}		
}

VOID BoardQuestMgr::RefreshBoard(Role* pRole, EBoardRefreshType eType, INT64 n64Serial/* =0 */, DWORD dwNPCID/* =GT_INVALID */)
{
	// 获得刷新规则
	tagBoardQuestRandomRate* pRule = GetNowtimeRule(eType, FALSE);
	if (!P_VALID(pRule))
	{
		return;
	}
	
	RoleBoardQuestMap& quest = pRole->GetBoardQuestMgr(m_eQuestBoardType)->GetRoleBoardQuest();

	// 将未接任务清空
	bool bHaveAcceptQuest = false;	// 记录是否有已经接取了的任务
	for (UINT16 i=m_n16QuestPanelStartTaskID; i<m_n16QuestPanelStartTaskID+QUEST_BOARD_MAX_CAPACITY; ++i)
	{
		tagRoleBoardQuest* pQuest = quest.Peek(i);
		if ( P_VALID(pQuest) && pQuest->eState == EQS_NotAccept)
		{
			pRole->GetBoardQuestMgr(m_eQuestBoardType)->Del(i);
		}
		else if (P_VALID(pQuest))
		{
			bHaveAcceptQuest = true;
		}
	}

	INT nMaxCount = GetQuestMaxCnt(pRole);
	BOOL bSecond = FALSE;
	for (UINT16 i=m_n16QuestPanelStartTaskID; i<m_n16QuestPanelStartTaskID+nMaxCount; ++i)
	{
		tagRoleBoardQuest* pQuest = quest.Peek(i);
		if ( P_VALID(pQuest) && pQuest->eState == EQS_HaveAccept)
		{
			continue;
		}
		
		if (bSecond)
		{
			// 只要随到了非白色品级的任务，就需要变换规则，使用“二次规则”
			pRule = GetNowtimeRule(eType, TRUE);
		}

		// 开始随机生成任务，先从品级高的任务开始随
		if( IUTIL->Rand() % 10000 < pRule->n16PurpleRate )
		{
			// 随到了紫色品级的任务
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_Purple, 1, i))
			{
				bSecond = TRUE;
				continue;
			}
		}
		
		if ( IUTIL->Rand() % 10000 < pRule->n16OrangeRate)
		{
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_Orange, 1, i))
			{
				bSecond = TRUE;
				continue;
			}
		}

		if ( IUTIL->Rand() % 10000 < pRule->n16BlueRate)
		{
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_Blue, 1, i))
			{
				bSecond = TRUE;
				continue;
			}
		}

		if ( IUTIL->Rand() % 10000 < pRule->n16GreenRate)
		{
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_Green, 1, i))
			{
				bSecond = TRUE;
				continue;
			}
		}

		if ( IUTIL->Rand() % 10000 < pRule->n16YellowRate)
		{
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_Yellow, 1, i))
			{
				bSecond = TRUE;
				continue;
			}
		}

		if ( IUTIL->Rand() % 10000 < pRule->n16WhiteRate)
		{
			if (RefreshBoardByQuestColor(pRole, pRole->GetLevel(), EBQQ_White, 1, i))
			{
				continue;
			}
		}

		//pRole->GetBoardQuestMgr()->Add(i, u16BoardID, EQS_NotAccept);
	}

	// 如果全部任务板任务都进行了刷新，则更新任务板等级
	SetBoardQuestLevel(pRole->GetLevel());
	INT nScriptDataIdx = GetScriptDataIdx();
	if (nScriptDataIdx != GT_INVALID)
		m_pRole->SetScriptData(nScriptDataIdx, pRole->GetLevel());

	// 将随出的任务发给客户端
	tagNS_OpenQuestBoard send;
	send.eQuestBoardType = m_eQuestBoardType;
	send.dwErrorCode = E_Success;

	RoleBoardQuestMap RoleQuest = pRole->GetBoardQuestMgr(m_eQuestBoardType)->GetRoleBoardQuest();
	RoleBoardQuestMap::TMapIterator it = RoleQuest.Begin();
	tagRoleBoardQuest* pQuest = NULL;
	INT nIndex = 0;
	while( RoleQuest.PeekNext(it, pQuest) )
	{
		if (nIndex>=QUEST_BOARD_MAX_CAPACITY)
		{
			ASSERT(nIndex<QUEST_BOARD_MAX_CAPACITY);
			break;
		}
		send.u16Quest[nIndex] = pQuest->u16BoardID;
		++nIndex;
	}

	// 将刷新后的任务板数据保存数据库
	SaveBoardQuest2DB();

	pRole->SendMessage(&send, send.dwSize);
}

INT BoardQuestMgr::GetQuestMaxCnt(Role* pRole)
{
	BYTE byLevel = pRole->GetLevel();
	INT nMaxCount = 0;
	if ( byLevel<10 ) 
	{
		// 10级以下只能获得两个任务
		nMaxCount = QUEST_BOARD_MIN_CAPACITY;
	}
	else if(byLevel>= 10 && byLevel<15)
	{
		nMaxCount = QUEST_BOARD_MIN_CAPACITY + 1;
	}
	else if (byLevel>=15 && byLevel<20)
	{
		nMaxCount = QUEST_BOARD_MIN_CAPACITY + 2;
	}
	else if (byLevel>=20 && byLevel<30)
	{
		nMaxCount = QUEST_BOARD_MIN_CAPACITY + 3;
	}
	else 
	{
		// 30级以上，将要随出六个任务
		nMaxCount = QUEST_BOARD_MAX_CAPACITY;
	}
	return nMaxCount;
}

VOID BoardQuestMgr::SaveBoardQuest2DB()
{	
	if (!P_VALID(m_pRole))
		return;

	UINT16 n16QuestCnt = m_BoardQuest.Size();
	if (n16QuestCnt <= 0)
		return;

	DWORD dwMsgSize = sizeof(tagNDBC_QuestBoardSaveAllQuest) + (n16QuestCnt-1)*sizeof(tagBoardQuestSaveData);
	MCREATE_MSG(pSend, dwMsgSize, NDBC_QuestBoardSaveAllQuest);
	if (!P_VALID(pSend))
		return;

	INT nIdx = 0;
	pSend->n16QuestCnt = n16QuestCnt;
	for(INT i=0; i<pSend->n16QuestCnt; ++i)
	{
		UINT16 u16ID = m_n16QuestPanelStartTaskID+i;
		if (!P_VALID(m_BoardQuest.Peek(u16ID)))
			continue;

		pSend->pData[nIdx].dwRoleID = m_pRole->GetID();
		pSend->pData[nIdx].u16QuestID = u16ID;
		pSend->pData[nIdx].u16BoardID = m_BoardQuest.Peek(u16ID)->u16BoardID;
		pSend->pData[nIdx].eState = m_BoardQuest.Peek(u16ID)->eState;
		pSend->pData[nIdx].byType = m_eQuestBoardType;

		++nIdx;
	}
	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

VOID BoardQuestMgr::ChangeQuestState(UINT16 u16ID, EBoardQuestState eState)
{
	tagRoleBoardQuest* p = m_BoardQuest.Peek(u16ID);
	if ( P_VALID(p) && p->eState != eState && P_VALID(m_pRole))
	{
		p->eState = eState;
		tagNDBC_QuestBoardSaveOneQuest send;
		send.sQuestSaveData.dwRoleID = m_pRole->GetID();
		send.sQuestSaveData.eState = eState;
		send.sQuestSaveData.u16BoardID = p->u16BoardID;
		send.sQuestSaveData.u16QuestID = p->u16ID;
		send.sQuestSaveData.byType = m_eQuestBoardType;
		g_dbSession.Send(&send, send.dwSize);
	}
}