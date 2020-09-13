//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONG.NET All rights reserved.
// filename: exit_game_handle.cpp
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: 龙之试炼系统-消息处理
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/msg_train.h"
#include "../WorldDefine/train_define.h"

#include "player_session.h"
#include "role.h"

//-----------------------------------------------------------------------------
// 取试炼列表
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetTrainList(tagNetCmd* pCmd)
{
//	MGET_MSG(pRecv, pCmd, NC_GetTrainList);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))	return GT_INVALID;

	return pRole->SendTrainStateList2Client();
}

//-----------------------------------------------------------------------------
// 完成试炼
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleFinishTrain(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_FinishTrain);
	
	DWORD dwErrCode = E_Train_Not_Pass;
	tagNS_FinishTrain FinishTrain;
	Role* pRole		= GetRole();
	DWORD dwTrainID = pRecv->dwTrainID;

	if (!P_VALID(pRole))
	{
		return E_Train_Unknown;
	}

	// 已完成或者未开启的试炼直接返回
	if (pRole->GetTrainState(dwTrainID) != ETS_Finish)
	{
		return E_Train_Unknown;
	}

	const tagTrainAttProto* pTrainAttProto = g_attRes.GetOneTrainAttProto(dwTrainID);
	if (!P_VALID(pTrainAttProto))
	{
		return E_Train_Unknown;
	}
	INT nLapCnt = 0;
	nLapCnt += (pTrainAttProto->TrainReward.pItemReward[0].nItemCount>0)?1:0;
	nLapCnt += (pTrainAttProto->TrainReward.pItemReward[1].nItemCount>0)?1:0;

	// 检测背包剩余容量
	ItemMgr& itemMgr = pRole->GetItemMgr();	
	INT nFreeSize = itemMgr.GetBagFreeSize();
	if (nFreeSize < nLapCnt)
	{
		//回消息
		FinishTrain.dwTrainID		= dwTrainID;
		FinishTrain.eTrainErrCode	= E_Train_Bag_Full;
		FinishTrain.eTrainState		= pRole->GetTrainState(dwTrainID);
	}
	else
	{
		// 返回值为1通过试炼
		if (g_ScriptMgr.GetRoleScript()->IsTrainFinished(pRole, dwTrainID) == 1)
		{
			// 给奖励
			dwErrCode = pRole->GiveTrainReward(dwTrainID);
			switch (dwErrCode)
			{
			case E_Train_Pass:				// 通过试炼
				FinishTrain.eTrainErrCode = E_Train_Pass;
				FinishTrain.eTrainState = ETS_Pass;
				break;
			case E_Train_List_Null:			// 试炼列表为空
			case E_Train_Bag_Full:			// 行囊空间不足，请为行囊清理出足够空间再重新点击完成试炼！
			case E_Train_Not_Pass:			// 不能通过试炼
				FinishTrain.eTrainErrCode = static_cast<ETrainErrCode>(dwErrCode);
				FinishTrain.eTrainState = pRole->GetTrainState(dwTrainID);
				break;
			case E_Train_Unknown:			// 未知错误
			default:
				FinishTrain.eTrainErrCode = E_Train_Unknown;
				FinishTrain.eTrainState = pRole->GetTrainState(dwTrainID);
				break;
			}
			FinishTrain.dwTrainID = dwTrainID;
		}
		else
		{
			//回消息
			FinishTrain.dwTrainID		= dwTrainID;
			FinishTrain.eTrainErrCode	= E_Train_Not_Pass;
			FinishTrain.eTrainState		= pRole->GetTrainState(dwTrainID);
		}
	}


	// 保存试炼状态
	if (FinishTrain.eTrainState == ETS_Pass)
	{
		pRole->SaveTrainState(FinishTrain.dwTrainID, FinishTrain.eTrainState);
	}

	pRole->SendMessage((LPVOID)(&(FinishTrain)), FinishTrain.dwSize);

	return dwErrCode;
}
