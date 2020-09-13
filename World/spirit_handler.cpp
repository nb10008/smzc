#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/msg_spirit.h"
#include "Role.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/ItemDefine.h"
#include "role_mgr.h"
#include "../ServerDefine/log_cmdid_define.h"

DWORD PlayerSession::HandleSpiritReward(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpiritReward);
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SpiritReward send;
	send.dwError = E_Success;

	// 判断当时用户值是否是指定的领奖值
	INT attr = pRole->GetAttValue(ERA_Spirit);
	const std::map<DWORD, std::pair<int, EItemQuality> >& reward = g_attRes.GetSpiritReward();
	if (attr != g_attRes.GetSpiritRewardValue() && attr != g_attRes.GetSpiritRewardValue()-1) //允许相差1
	{
		send.dwError = E_InalidSprirtValue;
	}
	else if(pRole->GetItemMgr().GetBag().GetFreeSpaceSize() < (INT16)reward.size()) //包裹大小足够
	{
		send.dwError = E_BagIsFull;
	}
	

	if(send.dwError == E_Success)
	{
		// 扣点
		pRole->SetAttValue(ERA_Spirit,  g_attRes.GetAttDefRole(ERA_Spirit));
		// 给道具
		for(std::map<DWORD, std::pair<int, EItemQuality> >::const_iterator itor= reward.begin(); reward.end() != itor; itor++)
		{
			pRole->GetItemMgr().Add2Role(EICM_SpiritReward, -1, itor->first, itor->second.first, itor->second.second, ELCLD_Spirit_Reward);
		}
	}
	SendMessage(&send, send.dwSize);
	return send.dwError;
}

DWORD PlayerSession::HandleStrength2Spirit(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_StrengthReward);
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_StrengthReward send;
	send.dwError = E_Success;

	// 判断合法性
	int cost = 0;
	INT strength = pRole->GetStrength().nConsumptiveStrength;;
	if(pRecv->nNum >0)
	{
		const std::map<DWORD, int>& rewardMap = g_attRes.GetStrengthReward();
		std::map<DWORD, int>::const_iterator itor = rewardMap.find(pRecv->dwTypeID);
		if(itor != rewardMap.end())
		{
			cost = itor->second * pRecv->nNum;
			if(cost > strength || cost < 0)
			{
				send.dwError = E_StrengthNotEnough;
			}
			else
			{
				if(pRole->GetItemMgr().GetBag().GetFreeSpaceSize()<1)
				{
					send.dwError = E_BagIsFull;
				}
			}
		}
		else{
			send.dwError = E_NoRequestItem;
		}
	}
	else
	{
		send.dwError = E_WrongRequestNum;
	}

	if(send.dwError == E_Success)
	{
		// 扣除实力值
		pRole->SetConsumptiveStrength(strength - cost);
		// 给道具
		pRole->GetItemMgr().Add2Role(EICM_StrengthReward, -1, pRecv->dwTypeID, pRecv->nNum, EIQ_Null, ELCLD_Strength_Reward);		
	}
	SendMessage(&send, send.dwSize);

	return 0;
}