//-----------------------------------------------------------------------------
// filename: force_breakout_apprentice_mgr.cpp
// author: 
// data: 
// last:
// brief: 强制脱离帮派的徒弟管理器
//-----------------------------------------------------------------------------
#include "Stdafx.h"
#include "force_breakout_apprentice_mgr.h"
#include "db_session.h"
#include "role_mgr.h"
#include "role.h"

// 返回值 TRUE：到时间了  FALSE：还没到时间呢
BOOL CForceBreakOutApprentice::OnMinute()
{
	if (CalcTimeDiff(GetCurrentDWORDTime(), m_BreakoutInsertTime) >= DAY_SECOND_COUNT)
		return TRUE;

	return FALSE;
}

// 执行角色退出师门操作
VOID CForceBreakOutApprentice::ApprenticeBreakOut()
{
	Role* pMaster = g_roleMgr.GetRolePtrByID(m_dwMasterID);
	Role* pApprentice = g_roleMgr.GetRolePtrByID(m_dwApprenticeID);
	BOOL bNotifyClassmate = FALSE;

	// 如果师傅在线
	if (P_VALID(pMaster))
	{
		pMaster->DeleteApprentice(m_dwApprenticeID);

		pMaster->ModifyMasterApprenticeCount(-1);
		// 给师傅发送脱离师门关系消息
		tagNS_BreakOutMasterConfirm send;
		send.dwRoleID = m_dwApprenticeID;
		pMaster->GetSession()->SendMessage(&send, send.dwSize);

		// 通知师傅在线师门人数可能发生变化
		pMaster->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMaster->GetOnlineClassmatesCnt());

		if (bNotifyClassmate == FALSE)
		{
			// 通知徒弟有人脱离师门
			TMap<DWORD, DWORD>::TMapIterator iterator = pMaster->GetApprenticeData().Begin();
			DWORD dwApprenticeID = GT_INVALID;
			while (pMaster->GetApprenticeData().PeekNext(iterator, dwApprenticeID))
			{
				Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
				if (P_VALID(pApprentice))
				{
					pApprentice->DeleteClassMate(m_dwApprenticeID);

					// 通知徒弟在线师门人数发生变化
					pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
				}
			}

			bNotifyClassmate = TRUE;
		}
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(m_dwMasterID, -1);
		g_roleMgr.SaveRoleInfoToDB(m_dwMasterID);
	}

	// 如果徒弟在线
	if (P_VALID(pApprentice))
	{
		if (bNotifyClassmate == FALSE)
		{
			// 通知同门有人脱离师门
			DWORD dwMateID = GT_INVALID;
			for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
			{
				dwMateID = pApprentice->GetClassMate().dwData[i];
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->DeleteClassMate(pApprentice->GetID());
		 			pMate->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMate->GetOnlineClassmatesCnt()-1);
				}
			}

			bNotifyClassmate = TRUE;
		}
		pApprentice->SetMasterID(GT_INVALID);
		pApprentice->SetJingWuPoint(0);			// 清空精武点数
		pApprentice->ModifyMasterApprenticeCount(-1);		

		// 删除所有同门
		pApprentice->DeleteAllClassMate();

		// 给徒弟发送脱离师门关系消息
		tagNS_BreakOutApprenticeConfirm send;
		send.dwRoleID = m_dwMasterID;
		pApprentice->GetSession()->SendMessage(&send, send.dwSize);

		// 通知被逐出师门的徒弟师门在线人数发生变化
		pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(m_dwApprenticeID, -1);
		g_roleMgr.SaveRoleInfoToDB(m_dwApprenticeID);

		// 清空精武点数
		tagNDBC_ModifyApprenticeJingWuPoint sendDB;
		sendDB.dwApprenticeID = m_dwApprenticeID;
		sendDB.dwJingWuPoint = 0;
		g_dbSession.Send(&sendDB, sendDB.dwSize);
	}

	// 师傅徒弟都不在线，从数据库中读取师门数据，通知师傅的其他徒弟
// 	if (bNotifyClassmate == FALSE)
// 	{
// 		tagNDBC_LoadMasterApprenticeData
// 	}

	// 更新数据库
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = m_dwMasterID;
	sendDB.dwApprenticeID = m_dwApprenticeID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);
}

// 将数据存入数据库
VOID CForceBreakOutApprentice::InsertApprenticeData2DB()
{
	tagNDBC_InsertForceBreakOutApprentice send;
	send.sForceBreakOutApprentice.BreakoutInsertTime = m_BreakoutInsertTime;
	send.sForceBreakOutApprentice.dwMasterID = m_dwMasterID;
	send.sForceBreakOutApprentice.dwApprenticeID = m_dwApprenticeID;

	g_dbSession.Send(&send, send.dwSize);
}

// 删除数据库中的数据
VOID CForceBreakOutApprentice::RemoveApprenticeFromDB()
{
	tagNDBC_RemoveForceBreakOutApprentice send;
	send.sForceBreakOutApprentice.BreakoutInsertTime = m_BreakoutInsertTime;
	send.sForceBreakOutApprentice.dwMasterID = m_dwMasterID;
	send.sForceBreakOutApprentice.dwApprenticeID = m_dwApprenticeID;

	g_dbSession.Send(&send, send.dwSize);
}

CForceBreakOutApprenticeMgr::~CForceBreakOutApprenticeMgr()
{
	// 释放强制脱离师门申请列表
	DestoryApprenticeList();
}

VOID CForceBreakOutApprenticeMgr::OnMinute()
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		if (P_VALID(pForceBreakOutApprentice))
		{
			// 更新列表中节点,检查是否到时间了
			if (TRUE == pForceBreakOutApprentice->OnMinute())
			{
				// 到时间脱离师门了，先执行退出师门操作
				pForceBreakOutApprentice->ApprenticeBreakOut();
				
				// 删除数据库中的数据
				pForceBreakOutApprentice->RemoveApprenticeFromDB();

				// 再删除节点
				m_listForceBreakOutApprentice.Erase(pForceBreakOutApprentice);
				SAFE_DEL(pForceBreakOutApprentice);
			}
		}
	}
}

// 添加强制脱离师门的徒弟的申请
VOID CForceBreakOutApprenticeMgr::InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID)
{
	InsertApprentice2List(dwApprenticeID, dwMasterID, GetCurrentDWORDTime());
}

VOID CForceBreakOutApprenticeMgr::InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime, BOOL bNeedSave2DB/* = TRUE*/)
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = new CForceBreakOutApprentice(dwApprenticeID, dwMasterID, dwordBreakoutTime);
	if (P_VALID(pForceBreakOutApprentice))
	{
		m_listForceBreakOutApprentice.PushBack(pForceBreakOutApprentice);

		// 将数据存入数据库
		if (bNeedSave2DB)
		{
			pForceBreakOutApprentice->InsertApprenticeData2DB();
		}
	}
}

VOID CForceBreakOutApprenticeMgr::Init()
{
	// 载入数据库中存储的强制脱离师门申请列表
	LoadApprenticeListFromDB();
}

VOID CForceBreakOutApprenticeMgr::InitApprenticeList(DWORD dwForceBreakOutApprenceCnt, tagForceBreakOutApprentice *pForceBreakOutApprentice)
{
	for (DWORD dwCnt = 0; dwCnt < dwForceBreakOutApprenceCnt; dwCnt++)
	{
		InsertApprentice2List(pForceBreakOutApprentice[dwCnt].dwApprenticeID, pForceBreakOutApprentice[dwCnt].dwMasterID, pForceBreakOutApprentice[dwCnt].BreakoutInsertTime, FALSE);		
	}
}

// 载入数据库中存储的强制脱离师门申请列表
VOID CForceBreakOutApprenticeMgr::LoadApprenticeListFromDB()
{
	// 向数据库发请求
	tagNDBC_LoadForceBreakOutApprenticeList send;
	g_dbSession.Send(&send, send.dwSize);
}

// 释放强制脱离师门申请列表
VOID CForceBreakOutApprenticeMgr::DestoryApprenticeList()
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		SAFE_DEL(pForceBreakOutApprentice);
	}
	m_listForceBreakOutApprentice.Clear();
}

BOOL CForceBreakOutApprenticeMgr::FindApprentice(DWORD dwApprenticeID, INT& dwRemainTime)
{
	CForceBreakOutApprentice* pForceBreakOutApprentice = NULL;
	TList<CForceBreakOutApprentice*>::TListIterator iterator = m_listForceBreakOutApprentice.Begin();
	while(m_listForceBreakOutApprentice.PeekNext(iterator, pForceBreakOutApprentice))
	{
		if (pForceBreakOutApprentice->GetApprenticeID() == dwApprenticeID)
		{
			dwRemainTime = pForceBreakOutApprentice->GetRemainTime();
			return TRUE;
		}
	}
	dwRemainTime = GT_INVALID;
	return FALSE;
}

CForceBreakOutApprenticeMgr g_forceBreakOutApprenticeMgr;