//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: sunnee
// actor:
// data: 2009-11-09
// last:
// brief: 客户端和服务器端间消息处理 -- 城市系统
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../WorldDefine/msg_city.h"

#include "player_session.h"
#include "city.h"
#include "city_mgr.h"
#include "role.h"
#include "role_mgr.h"
#include "activity_mgr.h"

//-----------------------------------------------------------------------------
// 获取城市属性信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityAttInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr)
					&& pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);
	if(E_Success != dwErrorCode)
	{
		tagNS_GetCityInfo send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// 创建消息
	INT nMsgSize = sizeof(tagNS_GetCityInfo) + (MAX_CITY_ITEM_OUTPUT - 1) * sizeof(tagCityItemOutput);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCityInfo);

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		pSend->dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		pSend->dwErrorCode = pCity->GetCityAtt(pRole, &(pSend->sCityAtt));
	}

	// 重新计算消息大小
	pSend->dwSize = sizeof(tagNS_GetCityInfo) + (pSend->sCityAtt.nItemOutputType - 1) * sizeof(tagCityItemOutput);

	// 发送消息
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	return E_Success;
}

//-----------------------------------------------------------------------------
// 根据城市id获取城市指定属性信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityAttInfoByCityID(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityInfoByCityID);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRecv->dwCityID);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// 创建消息
	INT nMsgSize = sizeof(tagNS_GetCityInfo) + (MAX_CITY_ITEM_OUTPUT - 1) * sizeof(tagCityItemOutput);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCityInfo);

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		pSend->dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		pSend->dwErrorCode = pCity->GetCityAtt(pRole, &(pSend->sCityAtt));
	}

	// 重新计算消息大小
	pSend->dwSize = sizeof(tagNS_GetCityInfo) + (pSend->sCityAtt.nItemOutputType - 1) * sizeof(tagCityItemOutput);

	// 发送消息
	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
	return E_Success;
}
//-----------------------------------------------------------------------------
// 获取城市指定属性信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetAppointedCityAtt(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetAppointedCityAtt);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	if((E_Success != dwErrorCode) || !P_VALID(pCity))
	{
		tagNS_GetAppointedCityAtt send;
		send.sCityAtt.eType = pRecv->eCityAttType;
		send.sCityAtt.nDataNum = 0;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// 消息作成
	INT nMsgSize = sizeof(tagNS_GetAppointedCityAtt) - 1 + pCity->GetCityAppointedAttSize(pRecv->eCityAttType);
	MCREATE_MSG(pSend, nMsgSize, NS_GetAppointedCityAtt);

	pSend->sCityAtt.eType = pRecv->eCityAttType;
	pCity->GetCityAppointedAtt(pRecv->eCityAttType, pSend->sCityAtt.nDataNum, pSend->sCityAtt.byData);

	// 发送消息
	SendMessage(pSend, pSend->dwSize);
	
	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 调整城市税率
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSetTaxRate(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTaxRate);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC检查
	tagNS_SetTaxRate send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (send.dwErrorCode != E_Success)
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// 设置税率
	INT nTemp = 0;
	pCity->GetCityAppointedAtt(ECAT_CityID, nTemp, &(send.dwCityID));
	send.dwErrorCode = pCity->ModifyTaxRate(pRole, pRecv->nTaxRate);
	if (send.dwErrorCode != E_Success )
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}
	pCity->GetCityAppointedAtt(ECAT_TaxRate, nTemp, &(send.nTaxRate));

	// 全世界广播
	g_roleMgr.SendWorldMsg(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 提取城市税金
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleTransformTaxation(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_TransformTaxation);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC检查
	tagNS_TransformTaxation send;
	send.dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (send.dwErrorCode != E_Success)
	{
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// 提取税金
	INT nTemp = 0;
	send.dwErrorCode = pCity->TransformTaxation(pRole, pRecv->eTransType, pRecv->nTaxation);
	pCity->GetCityAppointedAtt(ECAT_Taxation, nTemp, &(send.nTaxation));

	// 发送反馈
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 城战报名
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSignupForCitywar(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SignupForCitywar);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	tagNS_SignupForCitywar send;

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		send.dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		send.dwErrorCode = pCity->SignupForCitywar(pRole,pRecv->eForce,pRecv->nFare);
	}

	// 发送反馈
	SendMessage(&send, send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// 得到报名的基本信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSigupInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetSigupInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}
	
	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);

	if(dwErrorCode != E_Success)
	{
		tagNS_GetSigupInfo send;
		send.dwErrorCode = dwErrorCode;
		// 发送反馈
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}
	//进攻防守数
	INT32 nAckNum = 0;
	INT32 nDefNum = 0;
	INT32 nConfirmNum = 0;
	INT nTemp = 0;
	pCity->GetCityAppointedAtt(ECAT_AttackSignUpNum,nTemp,&nAckNum);
	pCity->GetCityAppointedAtt(ECAT_DefenceSignUpNum,nTemp,&nDefNum);
	pCity->GetCityAppointedAtt(ECAT_DefenceConfirmNum,nTemp,&nConfirmNum);

	INT32 nTotalNum = (nAckNum + nDefNum + nConfirmNum) == 0 ? 1 : (nAckNum + nDefNum + nConfirmNum);
	DWORD dwSize = sizeof(tagNS_GetSigupInfo) + (nTotalNum - 1) * sizeof(DWORD);
	MCREATE_MSG(pSend, dwSize, NS_GetSigupInfo);

	//得到报名信息
	pCity->GetCitySigupInfo(pRole,pSend);
	//攻守数量 已确认数量
	pSend->nAckNum		= nAckNum;
	pSend->nDefNum		= nDefNum;
	pSend->nConfirmNum	= nConfirmNum;

	// 发送反馈
	SendMessage(pSend, dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// 确认防守方
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleConfirmDefenceForce(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ConfirmDefenceForce);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_SignUp);

	if(dwErrorCode != E_Success)
	{
		tagNS_ConfirmDefenceForce send;
		send.dwErrorCode = dwErrorCode;
		// 发送反馈
		SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}
	
	tagNS_ConfirmDefenceForce send;

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		send.dwErrorCode = E_City_Struggle_Not_Open;
	}
	else
	{
		//防守方确认
		send.dwErrorCode = pCity->ConfirmDefenceForce(pRole,pRecv->dwGuildID);
	}

	// 发送反馈
	SendMessage(&send, send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
// 得到城战中的城市id
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityWarCity(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityWarCity);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}

	// 取得城战中城市数目
	DWORD dwNum = g_cityMgr.GetCityNum();

	DWORD dwSize = sizeof(tagNS_GetCityWarCity) + (dwNum - 1) * sizeof(DWORD);

	//创建变长消息
	MCREATE_MSG(pSend, dwSize, NS_GetCityWarCity);

	//得到城战中的城市id
	g_cityMgr.GetCityWarCity(pSend);

	// 发送反馈
	SendMessage(pSend, pSend->dwSize);

	return E_Success;

}
//-----------------------------------------------------------------------------
// 得到城战信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCityWarInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCityWarInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRecv->dwCityID);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	// 城战未开启
	if (g_activityMgr.GetCityStruggleState() == 0)
	{
		return GT_INVALID;
	}

	tagNS_GetCityWarInfo send;
	//得到报名信息
	pCity->GetCityWarInfo(pRole,&send);

	// 发送反馈
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 提升生产力
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpgradeProlificacy(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UpgradeProlificacy);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_CityMgr);
	if (dwErrorCode != E_Success)
	{	
		tagNS_UpgradeProlificacy send;
		send.dwErrorCode = dwErrorCode;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	// 取得城市
	City* pCity = g_cityMgr.GetCity(pRole);
	if (!P_VALID(pCity))
	{
		return GT_INVALID;
	}

	tagNS_UpgradeProlificacy send;
	send.dwErrorCode = pCity->UpgradeProlificacy(pRole,pRecv->eType);

	// 发送反馈
	SendMessage(&send, send.dwSize);

	return E_Success;

}