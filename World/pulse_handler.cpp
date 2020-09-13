#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "..\WorldDefine\msg_pulse.h"

DWORD PlayerSession::HandleActivePulse(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_ActivePulse);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	tagNS_ActivePulse send;
	send.dwid = pRecv->dwPulseID;
	send.dwErrorCode = pRole->ActivePulse(pRecv->dwPulseID);
	send.nLearnTime = pRole->GetPulseLearnTime();
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}
