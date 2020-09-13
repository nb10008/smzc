#include "stdafx.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_family.h"
#include "..\ServerDefine\family_define.h"
#include "..\ServerDefine\msg_family.h"
#include "player_session.h"
#include "Role.h"
#include "family.h"
#include "family_join.h"
#include "family_quest.h"
#include "family_mgr.h"

DWORD PlayerSession::HandleRoleOpenFamilyQuestPage(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_OpenFamilyQuestPage);

	if (!P_VALID(pRecv))
	{
		return E_SystemError;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return E_SystemError;
	}
	
	DWORD dwFamilyID = pRole->GetFamilyID();
	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
	{
		return E_SystemError;
	}
	
	FamilyQuest& tmpFamilyQuest = pFamily->GetFamilyQuest();
	tmpFamilyQuest.RenewQuest(); // 零点之后第一次家族成员打开界面的时候要进行任务更新 //

	BYTE byNum = tmpFamilyQuest.GetQuestNum();
	if (byNum<=0)
	{
		tagNS_OpenFamilyQuestPage send;
		send.byQuestNum = 0;
		SendMessage(&send, send.dwSize);
		return 0;
	}

	INT nSize = sizeof(tagNS_OpenFamilyQuestPage) + (byNum-1)*sizeof(tagFamilyQuest);
	//tagNS_OpenFamilyQuestPage* pSend = (tagNS_OpenFamilyQuestPage*)new BYTE(nSize);
	MCREATE_MSG(pSend, nSize, NS_OpenFamilyQuestPage);
	if (!P_VALID(pSend))
	{
		return GT_INVALID;
	}
	pSend->dwID = IUTIL->Crc32("NS_OpenFamilyQuestPage");
	pSend->dwSize = nSize;
	pSend->byQuestNum = byNum;
	pSend->bIsReceive = pFamily->GetFamilyMember().IsMemberRewarded(pRole->GetID());
	pSend->nProcess = tmpFamilyQuest.GetYesterdayFamilyQuestProcess();
	tmpFamilyQuest.FillFamilyQuestData((BYTE*)(pSend->Quests));
	SendMessage(pSend, pSend->dwSize);

	return 0;
}

DWORD PlayerSession::HandleRoleGetFamilyQuestYesterdayInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_YesterdayQuestInfo);

	return 0;
}

DWORD PlayerSession::HandleRoleGiveInFamilyQuestItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GiveInFamilyQuestItem);
	
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return E_SystemError;
	}

	DWORD dwFamilyID = pRole->GetFamilyID();
	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
	{
		return E_SystemError;
	}

	FamilyQuest& tmpFamilyQuest = pFamily->GetFamilyQuest();

	if (pRecv->nNum <= 0)
	{
		return GT_INVALID;
	}

	INT nSum = 0;
	DWORD dwErrorCode = tmpFamilyQuest.OnAnyMemberGiveInItem(pRole->GetID(), pRecv->dwQuestID, pRecv->dwTypeID, pRecv->n64EquipSerial, pRecv->nNum, nSum);

	if (GT_INVALID != dwErrorCode)
	{
		tagNS_GiveInFamilyQuestItem msg;
		msg.dwErrorCode = dwErrorCode;
		msg.dwQuestID = pRecv->dwQuestID;
		msg.nCount = nSum;
		SendMessage(&msg, msg.dwSize);
	}

	return E_Success;
}

DWORD PlayerSession::HandleRoleGetFamilyQuestReward(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RewardFamilyMember);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return E_SystemError;
	}

	DWORD dwFamilyID = pRole->GetFamilyID();
	Family* pFamily = g_FamilyMgr.GetFamily(dwFamilyID);
	if (!P_VALID(pFamily))
	{
		return E_SystemError;
	}
	
	FamilyQuest& tmpFamilyQuest = pFamily->GetFamilyQuest();
	
	DWORD dwErrorCode = tmpFamilyQuest.CanGetReward(pRole->GetID());
	if (E_Success != dwErrorCode)
	{
		tagNS_RewardFamilyMember send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return dwErrorCode;
	}

	const tagFamilyQuestRewardProto* pReward = g_attRes.GetOneFamilyRewardProto(pRole->GetLevel());
	if (!P_VALID(pReward))
	{
		return GT_INVALID;
	}

	// 计算奖励物品占得格子数
	INT nSpace = 0;
	if ( TRUE == tmpFamilyQuest.CanGetItemRewardI())
	{
		for(INT n = 0; n < MAX_FQ_RewardNum; ++n)
		{
			if( !P_VALID(pReward->dwItemID[n]) ) continue;

			if( pReward->dwItemNum[n] <= 0 ) continue;

			tagItemProto* pProto = NULL;
			if(!MIsEquipment(pReward->dwItemID[n]))
			{
				pProto = g_attRes.GetItemProto(pReward->dwItemID[n]);
			}
			else
			{
				pProto = g_attRes.GetEquipProto(pReward->dwItemID[n]);
			}
			if(!P_VALID(pProto))
			{
				continue;
			}

			// 计算占用的格子
			INT16 nUseSpace = 0;
			if(pReward->dwItemNum[n] <= pProto->n16MaxLapNum)
			{
				nUseSpace = 1;
			}
			else
			{
				nUseSpace = (1 == pProto->n16MaxLapNum) ? pReward->dwItemNum[n] : ((pReward->dwItemNum[n] - 1) / pProto->n16MaxLapNum + 1);
			}

			nSpace += nUseSpace;
		}
	}
	
	if ( TRUE == tmpFamilyQuest.CanGetItemRewardII())
	{
		for(INT n = 0; n < MAX_FQ_RewardNum; ++n)
		{
			if( !P_VALID(pReward->dwItemIDEx[n]) ) continue;

			if( pReward->dwItemNumEx[n] <= 0 ) continue;

			const tagItemProto *pProto = NULL;
			if(!MIsEquipment(pReward->dwItemIDEx[n]))
			{
				pProto = g_attRes.GetItemProto(pReward->dwItemIDEx[n]);
			}
			else
			{
				pProto = g_attRes.GetEquipProto(pReward->dwItemIDEx[n]);
			}

			if(!P_VALID(pProto))
			{
				continue;
			}

			// 计算占用的格子
			INT16 nUseSpace = 0;
			if(pReward->dwItemNumEx[n] <= pProto->n16MaxLapNum)
			{
				nUseSpace = 1;
			}
			else
			{
				nUseSpace = (1 == pProto->n16MaxLapNum) ? pReward->dwItemNumEx[n] : ((pReward->dwItemNumEx[n] - 1) / pProto->n16MaxLapNum + 1);
			}

			nSpace += nUseSpace;
		}
	}
	
	if (nSpace>pRole->GetItemMgr().GetBagFreeSize())
	{
		tagNS_RewardFamilyMember send;
		send.dwErrorCode = E_FamilyQuest_Rewarded_BagFull;
		SendMessage(&send, send.dwSize);
		return E_FamilyQuest_Rewarded_BagFull;
	}

	// 奖励经验金钱
	float fCompleteRate = (float)(tmpFamilyQuest.GetYesterdayFamilyQuestProcess())/10000;
	INT64 n64Exp = (INT64)(pReward->dwExp*fCompleteRate);
	pRole->ExpChange(n64Exp);
	INT64 n64Money = (INT64)(pReward->dwMoney*fCompleteRate);
	pRole->GetCurMgr().IncBagSilver(n64Money, ELCID_FamilyQuest_GetReward);

	// 奖励物品
	if ( TRUE == tmpFamilyQuest.CanGetItemRewardI())
	{
		for(INT n = 0; n < MAX_FQ_RewardNum; ++n)
		{
			if( !P_VALID(pReward->dwItemID[n]) ) break;

			if( pReward->dwItemNum[n] <= 0 ) continue;

			if( MIsEquipment(pReward->dwItemID[n]))
			{
				pRole->RewardEquip(EICM_FamilyQuest, pRole->GetID(), pReward->dwItemID[n], (INT16)pReward->dwItemNum[n], EIQ_Null, ELCID_FamilyQuest_rewards);
			}
			else
			{
				pRole->GetItemMgr().Add2Role(EICM_FamilyQuest, pRole->GetID(), pReward->dwItemID[n], (INT16)pReward->dwItemNum[n], EIQ_Null, ELCID_FamilyQuest_rewards);
			}
		}
	}
	
	if ( TRUE == tmpFamilyQuest.CanGetItemRewardII())
	{
		for(INT n = 0; n < MAX_FQ_RewardNum; ++n)
		{
			if( !P_VALID(pReward->dwItemIDEx[n]) ) break;

			if( pReward->dwItemNumEx[n] <= 0 ) continue;

			if( MIsEquipment(pReward->dwItemIDEx[n]))
			{
				pRole->RewardEquip(EICM_FamilyQuest, pRole->GetID(), pReward->dwItemIDEx[n], (INT16)pReward->dwItemNumEx[n], EIQ_Null, ELCID_FamilyQuest_rewards);
			}
			else
			{
				pRole->GetItemMgr().Add2Role(EICM_FamilyQuest, pRole->GetID(), pReward->dwItemIDEx[n], (INT16)pReward->dwItemNumEx[n], EIQ_Null, ELCID_FamilyQuest_rewards);
			}
		}
	}


	// 领取状态
	pFamily->GetFamilyMember().SetQuestRewardedFlag(pRole->GetID(), TRUE);

	BYTE byNum = tmpFamilyQuest.GetQuestNum();
	if (byNum<=0)
	{
		tagNS_OpenFamilyQuestPage send;
		send.byQuestNum = 0;
		SendMessage(&send, send.dwSize);
		return 0;
	}

	INT nSize = sizeof(tagNS_OpenFamilyQuestPage) + (byNum-1)*sizeof(tagFamilyQuest);
	MCREATE_MSG(pSend, nSize, NS_OpenFamilyQuestPage);
	if (!P_VALID(pSend))
	{
		return GT_INVALID;
	}
	pSend->dwID = IUTIL->Crc32("NS_OpenFamilyQuestPage");
	pSend->dwSize = nSize;
	pSend->byQuestNum = byNum;
	pSend->bIsReceive = pFamily->GetFamilyMember().IsMemberRewarded(pRole->GetID());
	pSend->nProcess = tmpFamilyQuest.GetYesterdayFamilyQuestProcess();
	tmpFamilyQuest.FillFamilyQuestData((BYTE*)(pSend->Quests));
	SendMessage(pSend, pSend->dwSize);


	tagNS_RewardFamilyMember send;
	send.dwErrorCode = E_Success;
	SendMessage(&send, send.dwSize);

	// 保存领取状态
	tagNDBC_SaveRoleFamilyQuestRewarded send1;
	send1.dwFamilyID = pFamily->GetID();
	send1.dwRoleID = pRole->GetID();
	g_dbSession.Send(&send1, send1.dwSize);

	return E_Success;
}