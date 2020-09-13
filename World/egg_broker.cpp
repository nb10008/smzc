// 砸金蛋功能

#include "stdafx.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/att_res_define.h"
#include "MsgInfoMgr.h"
#include "item_creator.h"
#include "egg_broker.h"
#include "role.h"
#include "world.h"

VOID EggBroker::InitEggInfoFromDB(const BYTE* &pData, INT nNum)
{
	tagEggInfoInDB* pEggInfoInDB = (tagEggInfoInDB*)pData;
	if (!P_VALID(pEggInfoInDB) || nNum <= 0)
		return;

	InitEggInfoFromDB(pEggInfoInDB->nCurLayer, pEggInfoInDB->n16ColourEggPos1, pEggInfoInDB->n16ColourEggPos2, pEggInfoInDB->sBrokenEggInfo);

	// 重设指针
	pData = (const BYTE*)((tagEggInfoInDB*)pData + nNum);
}

VOID EggBroker::InitEggInfoFromDB(INT nCurLayer, INT16 n16ColourEggPos1, INT16 n16ColourEggPos2, tagEggInfo* pEggInfo)
{
	if (!g_world.IsEggBrokerOpen())
		return;
	if (!IsEggLayerValid(nCurLayer) || (!IsColourEggPosValid(n16ColourEggPos1) && !IsColourEggPosValid(n16ColourEggPos2)) || !P_VALID(pEggInfo))
		return;

	m_nCurLayer = nCurLayer;
	m_n16ColourEggPos1 = n16ColourEggPos1;
	m_n16ColourEggPos2 = n16ColourEggPos2;
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		m_BrokenEggInfo[nCnt].dwItemID = pEggInfo[nCnt].dwItemID;
		m_BrokenEggInfo[nCnt].nBrokePos = pEggInfo[nCnt].nBrokePos;
	}
}

BOOL EggBroker::IsColourEggPosValid(INT16 n16ColourEggPos)
{
	INT nColourEggPosSize = sizeof(COLOUR_EGG_POS)/sizeof(INT16);
	if (nColourEggPosSize <= 0)
		return FALSE;
	for (INT nCnt = 0; nCnt < nColourEggPosSize; nCnt++)
	{
		if (COLOUR_EGG_POS[nCnt] == n16ColourEggPos)
			return TRUE;
	}
	return FALSE;
}

BOOL EggBroker::IsColourEgg(INT16 nLayer, INT16 nPos)
{
	if ((m_n16ColourEggPos1 == GT_INVALID && m_n16ColourEggPos2 == GT_INVALID) || nPos == GT_INVALID || nLayer == GT_INVALID)
		return FALSE;

	if (m_n16ColourEggPos1%10 == nPos && m_n16ColourEggPos1/10 == nLayer)
		return TRUE;

	if (m_n16ColourEggPos2%10 == nPos && m_n16ColourEggPos2/10 == nLayer)
		return TRUE;
	
	return FALSE;
}

VOID EggBroker::GetColourEggPosByLayer(INT nLayer, INT& nColourEggPos1, INT& nColourEggPos2)
{
	if ((m_n16ColourEggPos1 == GT_INVALID && m_n16ColourEggPos2 == GT_INVALID) || nLayer == GT_INVALID)
		return;

	if (m_n16ColourEggPos1/10 == nLayer)
		nColourEggPos1 = m_n16ColourEggPos1%10;

	if (m_n16ColourEggPos2/10 == nLayer)
		nColourEggPos2 = m_n16ColourEggPos2%10;
}

DWORD EggBroker::SendEggInfo2Client(Role* pRole)
{
	if (!g_world.IsEggBrokerOpen())
		return GT_INVALID;

	if (!P_VALID(pRole))
		return GT_INVALID;

	// 第一次打开，要重新初始化彩蛋的位置
	if (m_nCurLayer == 0)
		InitColourEggPos();

	tagNS_EggBegin send;
	send.nCurrLayer = m_nCurLayer;
	send.nColourEggPos_1 = GT_INVALID;
	send.nColourEggPos_2 = GT_INVALID;
	GetColourEggPosByLayer(m_nCurLayer, send.nColourEggPos_1, send.nColourEggPos_2);
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		send.EggStartInfo[nCnt].nPos = m_BrokenEggInfo[nCnt].nBrokePos;
		send.EggStartInfo[nCnt].bIsColourEgg = IsColourEgg(nCnt, m_BrokenEggInfo[nCnt].nBrokePos)?true:false;
		send.EggStartInfo[nCnt].dwItemID = m_BrokenEggInfo[nCnt].dwItemID;
		send.EggStartInfo[nCnt].nLayer = nCnt;
	}

	pRole->SendMessage(&send, send.dwSize);
	return E_Success;
}

DWORD EggBroker::BreakEgg(Role* pRole, int nLayer, int nPos)
{
	if (!g_world.IsEggBrokerOpen())
		return GT_INVALID;

	if (!P_VALID(pRole))
		return GT_INVALID;

	tagGoldEggProto* pGoldEggProto = g_attRes.GetGoldEggProto(nLayer+1);
	if (!P_VALID(pGoldEggProto))
		return GT_INVALID;

	// 不是彩蛋，计算空蛋几率， 是彩蛋，跳过空蛋计算
	tagGoldEggPctProto* pGoldEggPctProto = g_attRes.GetGoldEggPctProto(m_nCurLayer+1);
	if (!P_VALID(pGoldEggPctProto))
		return GT_INVALID;

	DWORD dwErrorCode = E_Success;
	tagNS_BreakEgg send;
	TList<tagItem*> ItemList;

	// 判断是否可砸
	if (!IsEggLayerValid(nLayer) || !IsEggPosValid(nLayer, nPos) || (nLayer != m_nCurLayer))
	{
		dwErrorCode = EEBEC_CannotBreak;
		goto end;
	}

	// 判断锤子够不够
	BOOL bIsColourEgg = IsColourEgg(nLayer, nPos);
	INT nHammerCnt = bIsColourEgg?pGoldEggProto->nSpecConsum:pGoldEggProto->nGenConsum;
	CHECK_SPEC_BEGIN("BreakEgg::SearchPack",200);
	if (SearchPack(pRole->GetItemMgr().GetBag(), ItemList, EISF_EggHammer, tagFindItemSpecFunc()) < nHammerCnt)
	{
		dwErrorCode = EEBEC_HammerNotEnough;
		goto end;
	}
	CHECK_SPEC_END;

	CHECK_SPEC_BEGIN("BreakEgg::RemoveFromContainer",200);
	// 锤子够，扣除对应数量的金锤
	RemoveFromContainer(pRole->GetItemMgr(), pRole->GetItemMgr().GetBag(), ItemList, nHammerCnt, ELCID_Egg_Broker_Cost);
	CHECK_SPEC_END;
	// 计算空蛋
	if ((bIsColourEgg == FALSE) && (IUTIL->Rand() % 10000 <= pGoldEggPctProto->n32EmptyPct))
	{
		dwErrorCode = EEBEC_EmptyEgg;
		send.nLastEggLayer = nLayer;
		send.nLastEggPos = nPos;
		send.bLastEggIsColour = false;
		// 重置砸蛋游戏
		FinishEggGame(pRole->GetID());
		SaveEggInfo2DB(pRole->GetID());
		goto end;
	}

	// 不是空蛋，根据几率生成物品，并给客户端返回下一层蛋的信息；如果是顶层了，返回响应的错误码;
	DWORD dwNewItemID = GT_INVALID;
	INT32 n32RandNum = IUTIL->Rand() % 10000;
	INT32 n32Pct = 0;
	for (INT nCnt = 0; nCnt < GOLD_EGG_MAX_CNT; nCnt++)
	{
		n32Pct += pGoldEggPctProto->n32ItemPct[nCnt];
		if (n32RandNum <= n32Pct)
		{
			dwNewItemID = pGoldEggProto->dwItemID[nCnt];
			if (dwNewItemID == 0 || dwNewItemID == GT_INVALID)
				return GT_INVALID;
			break;
		}
	}

	// 记下砸出的物品和砸中的金蛋的位置
	m_BrokenEggInfo[m_nCurLayer].dwItemID = dwNewItemID;
	m_BrokenEggInfo[m_nCurLayer].nBrokePos = nPos;
	send.dwItemID = dwNewItemID;
	send.bLastEggIsColour = bIsColourEgg?true:false;
	send.nLastEggLayer = nLayer;
	send.nLastEggPos = nPos;
	if ((m_nCurLayer+1) == MAX_EGG_LAYER)
	{
		dwErrorCode = EEBEC_TopLayer;
	}
	else
	{
		dwErrorCode = EEBEC_Success;
		m_nCurLayer++;
		send.nNextLayer = m_nCurLayer;
		send.nColourEggPos_1 = GT_INVALID;
		send.nColourEggPos_2 = GT_INVALID;
		GetColourEggPosByLayer(m_nCurLayer, send.nColourEggPos_1, send.nColourEggPos_2);
	}

	SaveEggInfo2DB(pRole->GetID());
end:
	ItemList.Clear();
	send.dwErrorCode = dwErrorCode;
	pRole->SendMessage(&send, send.dwSize);
	return dwErrorCode;
}

DWORD EggBroker::TakeEggTreasure(Role* pRole, int nLayer)
{
	if (!g_world.IsEggBrokerOpen())
		return GT_INVALID;

	if (!P_VALID(pRole))
		return GT_INVALID;

	tagNS_TakeEggTreasure send;
	DWORD dwErrorCode = E_Success;

	// 判断取宝层有没有砸出宝物
	if (!IsEggLayerValid(nLayer) || nLayer > m_nCurLayer ||
		m_BrokenEggInfo[nLayer].dwItemID == GT_INVALID ||
		m_BrokenEggInfo[nLayer].dwItemID == 0 ||
		m_BrokenEggInfo[nLayer].nBrokePos == GT_INVALID)
	{
		dwErrorCode = EEBEC_ChooseNoItem;
		goto end;
	}

	// 生成物品
	tagItem* pNewItem = ItemCreator::Create(EICM_EggBroker, NULL, m_BrokenEggInfo[nLayer].dwItemID);
	if (!P_VALID(pNewItem))
	{
		IMSG(_T("EggBroker Create Item Failed,itemid=%d\r\n"), m_BrokenEggInfo[nLayer].dwItemID);
		return GT_INVALID;
	}

	BYTE byQuality = EIQ_Start;
	if (MIsEquipment(pNewItem->dwTypeID))
	{
		tagEquip* pEquip = (tagEquip*)pNewItem;
		if (P_VALID(pEquip))
			byQuality = pEquip->equipSpec.byQuality;
	}
	else
	{
		if (P_VALID(pNewItem->pProtoType))
			byQuality = pNewItem->pProtoType->byQuality;
	}
	if (byQuality >= EIQ_Purple && byQuality < EIQ_End)
	{
		SendBroadcast(nLayer, pRole->GetID(), pNewItem->dwTypeID);
	}

	// 判断背包是否已满，如果已满，放百宝袋，否则放背包
	if (pRole->GetItemMgr().GetBagFreeSize() < 1)
	{
		dwErrorCode = EEBEC_BaiBao;
		pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_Egg_Broker_Added);
	}
	else
	{
		dwErrorCode = EEBEC_Success;
		pRole->GetItemMgr().Add2Bag(pNewItem, ELCID_Egg_Broker_Added, TRUE);
	}

	// 重置砸蛋游戏
	FinishEggGame(pRole->GetID());

end:
	send.dwErrorCode = dwErrorCode;
	pRole->SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

DWORD EggBroker::GetBroadcastIDByLayer(INT nLayer)
{
	// nLayer = 4，5，6分别对应5、6、7层的广播
	DWORD dwBroadcastID = GT_INVALID;
	switch(nLayer)
	{
	case 4:
 		dwBroadcastID = 310001;
		break;
	case 5:
		dwBroadcastID = 310002;
		break;
	case 6:
		dwBroadcastID = 310003;
		break;
	default:
		break;
	}
	return dwBroadcastID;
}

VOID EggBroker::SendBroadcast(INT nLayer, DWORD dwRoleID, DWORD dwItemID)
{
	DWORD dwBroadcastID = GetBroadcastIDByLayer(nLayer);
	if (dwBroadcastID == 0 || dwBroadcastID == GT_INVALID)
		return;
	DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwBroadcastID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &dwItemID);
	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
}

DWORD EggBroker::FinishEggGame(DWORD dwRoleID)
{
	if (!g_world.IsEggBrokerOpen())
		return GT_INVALID;

	m_nCurLayer = 0;
	m_n16ColourEggPos1 = GT_INVALID;
	m_n16ColourEggPos2 = GT_INVALID;

	// 重新生成彩蛋位置
	InitColourEggPos();

	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		m_BrokenEggInfo[nCnt].nBrokePos = GT_INVALID;
		m_BrokenEggInfo[nCnt].dwItemID = 0;
	}
	ClearEggInfoFromDB(dwRoleID);
	return E_Success;
}

VOID EggBroker::InitColourEggPos()
{
	// 没有位置，直接返回
	INT nColourEggPosSize = sizeof(COLOUR_EGG_POS)/sizeof(INT16);
	if (nColourEggPosSize <= 0)
		return;

	// 先确定彩蛋的个数是1个还是2个
	INT nRandNum = IUTIL->Rand() % 10000;
	BOOL bOneColourEgg = (nRandNum > 5000)?TRUE:FALSE;

	// 从所有的可能出现彩蛋的位置随机抽取两个
	TList<INT16> ListColourEggPos;
	for (INT nCnt = 0; nCnt < nColourEggPosSize; nCnt++)
	{
		ListColourEggPos.PushBack(COLOUR_EGG_POS[nCnt]);
	}
	ListColourEggPos.RandPeek(m_n16ColourEggPos1, TRUE);
	if (!bOneColourEgg)
		ListColourEggPos.RandPeek(m_n16ColourEggPos2, TRUE);

	ListColourEggPos.Clear();
}

VOID EggBroker::SaveEggInfo2DB(DWORD dwRoleID)
{
	tagNDBC_SaveEggInfo send;
	send.dwRoleID = dwRoleID;
	send.n16ColourEggPos1 = m_n16ColourEggPos1;
	send.n16ColourEggPos2 = m_n16ColourEggPos2;
	send.nCurLayer = m_nCurLayer;
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		send.sBrokenEggInfo[nCnt].dwItemID = m_BrokenEggInfo[nCnt].dwItemID;
		send.sBrokenEggInfo[nCnt].nBrokePos = m_BrokenEggInfo[nCnt].nBrokePos;
	}
	g_dbSession.Send(&send, send.dwSize);
}

VOID EggBroker::ClearEggInfoFromDB(DWORD dwRoleID)
{
	tagNDBC_ClearEggInfo send;
	send.dwRoleID = dwRoleID;
	g_dbSession.Send(&send, send.dwSize);
}
