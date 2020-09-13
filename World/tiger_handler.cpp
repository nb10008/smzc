#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "..\WorldDefine\msg_tiger.h"
#include "TigerMgr.h"

DWORD PlayerSession::HandleTiger(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_GetTigerResult);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	tagNS_GetTigerResult send;
	
	send.dwErrorCode = pRole->GetTigerMgr().CheckRoleTigerCoins(pRole,pRecv->nType);
	if(!send.dwErrorCode)
		send.dwResult = pRole->GetTigerMgr().CreateResult(pRole,pRecv->nType);

	send.dwNum = 0;
	if (send.dwResult > 0)
	{
		tagTigerProto* pProto = g_attRes.GetTigerProto(pRecv->nType);
		if(P_VALID(pProto))
		{
			send.dwNum = pProto->dwNum[send.dwResult-1];
		}
		pRole->GetTigerMgr().SendBroadcast(send.dwResult,pRole->GetID(),pRecv->nType);
	}
	
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}
