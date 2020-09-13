//-----------------------------------------------------------------------------
//!\file title_handler.cpp
//!\author xlguo
//!
//!\date 2009-01-20
//! last 2009-02-02
//!
//!\brief 称号消息处理过程
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/msg_role_title.h"


#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "role_mgr.h"
#include "title_mgr.h"
//-----------------------------------------------------------------------------
// 设置角色使用某称号
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleActiveTitle(tagNetCmd* pCmd)
{
	tagNC_ActiveRoleTitle* pRecv = (tagNC_ActiveRoleTitle*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;
	
	// 只能设置本角色
	if (GetRole()->GetID() != pRecv->dwRoleID)
		return GT_INVALID;

	// 激活称号
	TitleMgr* pTitleMgr = GetRole()->GetTitleMgr();
	DWORD dwRtv = pTitleMgr->ActiviteTitle(pRecv->u16TitleID);


	// 发送结果
	tagNS_ActiveRoleTitle send;
	send.dwRoleID = GetRole()->GetID();
	send.u16TitleID = pTitleMgr->GetActiviteTitle();
	send.dwErrorCode = dwRtv;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// 请求获得角色拥有的所有称号
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetTitles(tagNetCmd* pCmd)
{
	tagNC_GetRoleTitles* pRecv = (tagNC_GetRoleTitles*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	Role* pRole = pRecv->dwRoleID == GetRole()->GetID() ? GetRole() : GetOtherInMap(pRecv->dwRoleID);
	if ( !P_VALID(pRole) )
		return 0;

	TitleMgr* pTitleMgr = pRole->GetTitleMgr();

// 	// 查看权限
// 	if ( pRole->GetID() != m_pRole->GetID() && !pTitleMgr.Visibility() )
// 	{	// 不可发送
// 		tagNS_GetRoleTitles send;
// 		send.dwErrorCode = E_Title_NotVisible;
// 		send.dwRoleID = pRecv->dwRoleID;
// 		send.u16TitleNum = 0;
// 		SendMessage(&send, send.dwSize);
// 	}
// 	else
// 	{	// 可发送
		DWORD dwTitlesSize = pTitleMgr->GetObtainedTitlesNum() * sizeof(UINT16);
		DWORD dwMsgSize = sizeof(tagNS_GetRoleTitles) - 1 + (dwTitlesSize > 0 ? dwTitlesSize : 1);

		ASSERT(dwMsgSize >= sizeof(tagNS_GetRoleTitles));

		MCREATE_MSG(pSend, dwMsgSize, NS_GetRoleTitles);
		//tagNS_GetRoleTitles* pSend;
		UINT16* pu16 = reinterpret_cast<UINT16 *>(pSend->byData);
		DWORD dwRtv = pTitleMgr->GetObtainedTitleIDs(pu16, pSend->u16TitleNum);
		pSend->dwErrorCode = dwRtv;
		pSend->dwRoleID = pRecv->dwRoleID;
		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
//	}

	//发送已过期的称号------------------------
	TMap<UINT16, DOUBLE> mapTimeRemainTemp;
	pTitleMgr->GetRemainTimeMap( mapTimeRemainTemp );
	if ( mapTimeRemainTemp.Size() > 0 )
	{
		TMap<UINT16, DOUBLE>::TMapIterator it = mapTimeRemainTemp.Begin();
		DOUBLE dwRemainTime = NULL;

		INT		nCount = 0;
		UINT16	u16TitleIDTemp;
		DWORD   dwTempCount = 0 ; // 剩余时间的称号个数

		//为消息赋值
		while( mapTimeRemainTemp.PeekNext(it, u16TitleIDTemp, dwRemainTime ) )
		{
			if ( 0 >= dwRemainTime )
			{
				++nCount;
			}
			else if( dwRemainTime > 0 )
			{
				dwTempCount++;
			}
		}

	    //-----------------------------------------------------
		// 增加发送未过期的时限制称号的剩余时间 
		//------------------------------------------------------
		DWORD dwTempSize = sizeof(tagNS_TitleRemainTime) + (dwTempCount - 1 )*sizeof(tagTitleRemainTime);
        MCREATE_MSG(pSendMsg, dwTempSize, NS_TitleRemainTime);
        pSendMsg->dwRoleID    = pRecv->dwRoleID;
		pSendMsg->u16TitleNum = dwTempCount ;

		UINT16 u16TitleNum = nCount;
          
		DWORD dwSize = sizeof(tagNS_OutOfDataTitle) - 1 + u16TitleNum * sizeof(UINT16);
		MCREATE_MSG(pSend, dwSize, NS_OutOfDataTitle);

		UINT16* pu16TitleID = reinterpret_cast<UINT16*>(pSend->byData);

		INT u16CountTemp = 0;
		INT nI = 0 ;
		TMap<UINT16, DOUBLE>::TMapIterator it2 = mapTimeRemainTemp.Begin();
		while( mapTimeRemainTemp.PeekNext(it2, u16TitleIDTemp, dwRemainTime ) )
		{
			if ( 0 == dwRemainTime )
			{
				pu16TitleID[u16CountTemp] = u16TitleIDTemp;
				//++nCount;
				++u16CountTemp;
			}	
			else if ( dwRemainTime > 0)
			{
				if ( nI  < dwTempCount )
				{

					pSendMsg->byData[nI].dwTitleID    = u16TitleIDTemp;
					pSendMsg->byData[nI].dwRemainTime = dwRemainTime;
				}
				nI++ ;
			}
		}


		pSend->dwRoleID = pRecv->dwRoleID;
		pSend->u16TitleNum = u16TitleNum;

		//发送消息
		m_pRole->SendMessage(pSend, pSend->dwSize);

		m_pRole->SendMessage(pSendMsg,pSendMsg->dwSize);
        
		MDEL_MSG(pSend);
		MDEL_MSG(pSendMsg);

	}	
	//------------------------------------------
	return 0;
}
