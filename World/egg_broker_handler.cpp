// 砸金蛋消息接收处理

#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "egg_broker.h"
#include "role.h"

DWORD PlayerSession::HandleEggBegin(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_EggBegin);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	EggBroker& pEggBroker = pRole->GetEggBroker();
	DWORD dwErrorCode = pEggBroker.SendEggInfo2Client(pRole);

	return dwErrorCode;
}

DWORD PlayerSession::HandleBreakEgg(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_BreakEgg);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	EggBroker& pEggBroker = pRole->GetEggBroker();
	DWORD dwErrorCode = pEggBroker.BreakEgg(pRole, pRecv->nLayer, pRecv->nPos);

	return dwErrorCode;
}

DWORD PlayerSession::HandleTakeEggTreasure(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_TakeEggTreasure);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	EggBroker& pEggBroker = pRole->GetEggBroker();
	DWORD dwErrorCode = pEggBroker.TakeEggTreasure(pRole, pRecv->nlayer);

	return dwErrorCode;
}

DWORD PlayerSession::HandleFinishEggGame(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_FinishEggGame);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	EggBroker& pEggBroker = pRole->GetEggBroker();
	DWORD dwErrorCode = pEggBroker.FinishEggGame(pRole->GetID());

	return dwErrorCode;
}

