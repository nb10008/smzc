//-----------------------------------------------------------------------
// 神之竞标系统
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "../WorldDefine/godbid_define.h"
#include "../WorldDefine/msg_GodbidDefine.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_godbid.h"
#include "../ServerDefine/msg_log.h"
#include "MsgInfoMgr.h"
#include "att_res.h"
#include "item_creator.h"
#include "role.h"
#include "godbid_mgr.h"

 GodBidMgr g_GodBidMgr;

 GodBidMgr::GodBidMgr()
 :m_bBidStartUpdated(FALSE)
 ,m_bBidTime(FALSE)
 ,m_bSwitchOn(TRUE)
 ,m_bBidEnding(FALSE)
 {

 }
GodBidMgr::~GodBidMgr()
{
	ClearGodBidInfoFromMemory();
}

VOID GodBidMgr::Update()
{
	if (!IsSwitchOn())
		return;

	CHECK_TIME()

	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (dwCurTime.min == 0)
	{
		if (!CheckCurIsBidTime())
			return;

		if (m_bBidEndUpdated == TRUE)
			m_bBidEndUpdated = FALSE;

		if (m_bBidStartUpdated == FALSE)
		{
			m_bBidStartUpdated = TRUE;
			// 开始竞标
			BidStart();
		}
	}
	else if (dwCurTime.min == 1 && m_bBidStartUpdated == TRUE)
	{
		m_bBidStartUpdated = FALSE;
	}
	else if (dwCurTime.min == 59 && m_bBidEndUpdated == FALSE)
	{
		if (!IsBidTime())
			return;

		// 通知数据库，清空竞标记录, 等待数据库返回时结束竞标
		ClearGodBidInfoFromDB(TRUE);

		SetBidEnding(TRUE);
		m_bBidEndUpdated = TRUE;
	}
}

 VOID GodBidMgr::BidStart()
 {
	 SetIsBidTime(TRUE);
	 SetBidEnding(FALSE);
	 InitBidItems();
	 SendBroadcast(GODBID_START_BROADCAST);
 }

 VOID GodBidMgr::BidEnd()
 {
	SetIsBidTime(FALSE);
	SetBidEnding(FALSE);
	SendBroadcast(GODBID_END_BROADCAST);
	NormalBidEnd();
	EachBidEnd(EGBT_BlackMarket_Gold);
	EachBidEnd(EGBT_BlackMarket_ShengBi);
	
	// 清除内存数据
	ClearGodBidInfoFromMemory();

	SendBroadcast(GODBID_ALL_END_BROADCAST);
 }

 // 初始化竞标开始时间list
 VOID GodBidMgr::InitFixedBidStartTimeList(TObjRef<VarContainer>& pVar, BOOL bFixed)
 {
	INT nBidStartTimeCnt = 0;
	if (bFixed)
		nBidStartTimeCnt = pVar->GetInt(_T("FixedBidStartTimeCnt"),_T("god_bid"),1);
	else
		nBidStartTimeCnt = pVar->GetInt(_T("WeekendAddedBidStartTimeCnt"),_T("god_bid"),1);
	
	if (nBidStartTimeCnt <= 0)
		return;

	TCHAR szTemp[X_SHORT_NAME] = {'\0'};
	for(INT n = 0; n < nBidStartTimeCnt; ++n)
	{
		if (bFixed)
			_stprintf(szTemp, _T("FixedBidStartTime_%d"), n+1);
		else
			_stprintf(szTemp, _T("WeekendAddedBidStartTime_%d"), n+1);
		INT16 n16StartTime = static_cast<INT16>(pVar->GetInt(szTemp, _T("god_bid"), GT_INVALID));
		if (n16StartTime == GT_INVALID)
			continue;
		else
		{
			if (bFixed)
				m_FixedBidStartTimeList.PushBack(n16StartTime);
			else
				m_WeekendAddedBidStartTimeList.PushBack(n16StartTime);
		}
	}
 }

 VOID GodBidMgr::InitGodBidSwitch(TObjRef<VarContainer>& pVar)
 {
	 m_bSwitchOn = (pVar->GetDword(_T("on"),_T("god_bid"),1) == 1)?TRUE:FALSE;
 }

 BOOL GodBidMgr::CheckIsBidTime(BidStartTimeList& listBidStartTime, tagDWORDTime dwTime)
 {
	 BidStartTimeList::TListIterator iterator = listBidStartTime.Begin();
	 INT16 n16BidStartTime = 0;
	 while (listBidStartTime.PeekNext(iterator, n16BidStartTime))
	 {
		 if (n16BidStartTime == static_cast<INT16>(dwTime.hour))
			 return TRUE;
	 }
	 return FALSE;
 }

BOOL GodBidMgr::CheckCurIsBidTime()
{
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();

	// 先判断是否是每周固定竞标时间
	if (CheckIsBidTime(m_FixedBidStartTimeList, dwCurTime))
		return TRUE;

	// 如果是周末
	DWORD dwWeekday = WhichWeekday(dwCurTime);
	if (dwWeekday == 0 || dwWeekday == 6)
	{
		return CheckIsBidTime(m_WeekendAddedBidStartTimeList, dwCurTime);
	}
	return FALSE;
}
 
VOID GodBidMgr::NormalBidEnd()
{
	for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	{
		EachBidEnd(EGBT_Normal, nCnt);
	}
}
tagBidInfoNode* GodBidMgr::GetBidInfoNode(EGodBidType eGodBidType, DWORD dwIdx/* = GT_INVALID*/)
{
	tagBidInfoNode* pBidInfoNode = NULL;
	if (eGodBidType == EGBT_BlackMarket_ShengBi)
		pBidInfoNode = &m_ShengBiBlackMarketBidInfo;	
	else if (eGodBidType == EGBT_BlackMarket_Gold)
		pBidInfoNode = &m_GoldBlackMarketBidInfo;
	else if (eGodBidType == EGBT_Normal)
	{
		if (dwIdx > MAX_GODBID_ITEM_NUM)
			return NULL;
		pBidInfoNode = &m_NormalBidInfoList[dwIdx];
	}
	else
		return NULL;
	return pBidInfoNode;
}

// 注意，调用此方法要在上层检查eGodBidType和dwIdx的合法性
VOID GodBidMgr::EachBidEnd(EGodBidType eGodBidType, DWORD dwIdx/* = GT_INVALID*/)
{
	tagBidInfoNode* pBidInfoNode = GetBidInfoNode(eGodBidType, dwIdx);
	if (!P_VALID(pBidInfoNode))
		return;

	tagGodBidProto* pGodBidProto = g_attRes.GetGodBidProto(pBidInfoNode->dwGodBidProtoID);
	if (!P_VALID(pGodBidProto))
		return;
	
	// 金钱类型，0：金币； 1：圣币
	BYTE byMoneyType = (eGodBidType == EGBT_BlackMarket_ShengBi)?1:0;
	DWORD dwLogMoney = 0;
	DWORD dwAccountID = 0;
	DWORD dwRoleID = 0;

	// 没有参与竞标者
	BidInfoMap* pBidInfoMap = pBidInfoNode->pBidInfoMap;
	if (!P_VALID(pBidInfoMap))
	{
		LogGodBid(dwRoleID, dwAccountID, pGodBidProto->dwItemID, byMoneyType, dwLogMoney);
		return;
	}

	// 生成物品
	tagItem* pItem = ItemCreator::Create(EICM_GodBid, NULL, pGodBidProto->dwItemID, pGodBidProto->dwItemCnt);
	if (!P_VALID(pItem))
		return;

	// 将物品放到竞标者百宝袋中
	dwRoleID = pBidInfoNode->CurHighestBidInfo.dwRoleID;
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (P_VALID(pRoleInfo) && dwRoleID != 0)
	{
		Role* pWinner = g_roleMgr.GetRolePtrByID(dwRoleID);
		if (!P_VALID(pWinner))
		{
			ItemMgr::InsertBaiBao2DB(pItem, dwRoleID, ELCID_Godbid_NormalBid_Added);

			// 删除物品
			::Destroy(pItem);
		}
		else
		{
			pWinner->GetItemMgr().Add2BaiBao(pItem, ELCID_Godbid_NormalBid_Added);
		}

		dwLogMoney = static_cast<DWORD>(pBidInfoNode->CurHighestBidInfo.n64Bidden);
		if (eGodBidType == EGBT_Normal || eGodBidType == EGBT_BlackMarket_Gold)
			dwLogMoney = dwLogMoney/GOLD2SILVER;
		dwAccountID = pRoleInfo->dwAccountID;
	}
	else
	{
		dwRoleID = 0;
		::Destroy(pItem);
	}

	LogGodBid(dwRoleID, dwAccountID, pGodBidProto->dwItemID, byMoneyType, dwLogMoney);

	// 将中标者从竞标信息表中删除
	pBidInfoMap->Erase(pBidInfoNode->CurHighestBidInfo.dwRoleID);

	// 返还其他玩家金钱
	GiveMoneyBack2Player(pBidInfoMap, eGodBidType);

	DWORD dwMoney = (eGodBidType == EGBT_BlackMarket_ShengBi)?
		static_cast<DWORD>(pBidInfoNode->CurHighestBidInfo.n64Bidden):(static_cast<DWORD>(pBidInfoNode->CurHighestBidInfo.n64Bidden/GOLD2SILVER));

	DWORD dwBroadCastID = 0;
	if (eGodBidType == EGBT_BlackMarket_ShengBi)
		dwBroadCastID = GODBID_BLACK_SHENGBI_BROADCAST;	
	else if (eGodBidType == EGBT_BlackMarket_Gold)
		dwBroadCastID = GODBID_BLACK_GOLD_BROADCAST;
	else if (eGodBidType == EGBT_Normal)
		dwBroadCastID = GODBID_NORMAL_END_BROADCAST;
	// 发广播
	SendBroadcast(dwBroadCastID,
		pBidInfoNode->CurHighestBidInfo.dwRoleID,
		dwMoney,
		pGodBidProto->dwItemID);
}

 VOID GodBidMgr::InitBidItems()
 {
	TMap<DWORD,tagGodBidProto *>& mapGodBidProto = g_attRes.GetGodBidProtoMap();
	if (mapGodBidProto.Size() <= 0)
		return;

	TList<DWORD> listNormalBidItem;
	TList<DWORD> listBlackGoldGodBidItem;
	TList<DWORD> listBlackShengBiBidItem;

	TMap<DWORD,tagGodBidProto *>::TMapIterator iterator = mapGodBidProto.Begin();
	tagGodBidProto *pGodBidProto = NULL;
	while(mapGodBidProto.PeekNext(iterator, pGodBidProto))
	{
		if (!P_VALID(pGodBidProto))
			continue;
		if (pGodBidProto->eGodBidType == EGBT_Normal)
			listNormalBidItem.PushBack(pGodBidProto->dwProtoID);
		else if (pGodBidProto->eGodBidType == EGBT_BlackMarket_Gold)
			listBlackGoldGodBidItem.PushBack(pGodBidProto->dwProtoID);
		else if (pGodBidProto->eGodBidType == EGBT_BlackMarket_ShengBi)
			listBlackShengBiBidItem.PushBack(pGodBidProto->dwProtoID);
	}
	// 抽取普通竞标物品
	for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	{
		DWORD dwProtoID = GT_INVALID;
		listNormalBidItem.RandPeek(dwProtoID, TRUE);
		if (dwProtoID == GT_INVALID)
			break;
		m_NormalBidInfoList[nCnt].dwGodBidProtoID = dwProtoID;
	}

	// 抽取黑市金币竞标物品
	DWORD dwBlackProtoID = GT_INVALID;
	listBlackGoldGodBidItem.RandPeek(dwBlackProtoID, TRUE);
	if (dwBlackProtoID != GT_INVALID)
		m_GoldBlackMarketBidInfo.dwGodBidProtoID = dwBlackProtoID;

	// 抽取黑市圣币竞标物品
	dwBlackProtoID = GT_INVALID;
	listBlackShengBiBidItem.RandPeek(dwBlackProtoID, TRUE);
	if (dwBlackProtoID != GT_INVALID)
		m_ShengBiBlackMarketBidInfo.dwGodBidProtoID = dwBlackProtoID;
 }
 
 VOID GodBidMgr::SendBroadcast(DWORD dwBroadcastID)
 {
	 if (dwBroadcastID == 0 || dwBroadcastID == GT_INVALID)
		 return;
	 DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	 g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwBroadcastID);
	 g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
 }

 VOID GodBidMgr::SendBroadcast(DWORD dwBroadcastID, DWORD dwRoleID, DWORD dwMoney, DWORD dwItemID)
 {
	 if (dwBroadcastID == 0 || dwBroadcastID == GT_INVALID)
		 return;
	 DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	 g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwBroadcastID);
	 g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
	 g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, &dwMoney);
	 g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &dwItemID);
	 g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
 }

 // 处理客户端请求
 VOID GodBidMgr::SendNormalBidInfo2Client(Role* pRole)
 {
	 if (!P_VALID(pRole) || !IsSwitchOn())
		 return;

	 tagNS_GodBiderShopOpen send;
	 if (!IsBidTime())
	 {
		 send.dwErrorCode = EGBEC_Black_OutOfTime;
		 pRole->SendMessage(&send, send.dwSize);
		 return;
	 }

	 DWORD dwRoleID = pRole->GetID();

	 for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	 {
		 tagGodBidProto* pGodBidProto = g_attRes.GetGodBidProto(m_NormalBidInfoList[nCnt].dwGodBidProtoID);
		 if (P_VALID(pGodBidProto))
		 {
			 send.bidItemInfo[nCnt].sBidData.dwID = pGodBidProto->dwProtoID;
			 send.bidItemInfo[nCnt].sBidData.dwItemID = pGodBidProto->dwItemID;
			 send.bidItemInfo[nCnt].sBidData.dwMinBidPrice = pGodBidProto->dwBidLowerLimit;
			 send.bidItemInfo[nCnt].sBidData.dwStartPrice = pGodBidProto->dwBasePrice;
			 send.bidItemInfo[nCnt].sBidData.eType = pGodBidProto->eGodBidType;
			 send.bidItemInfo[nCnt].sBidData.nNum = pGodBidProto->dwItemCnt;
		 }
		 send.bidItemInfo[nCnt].n64CurPrice = m_NormalBidInfoList[nCnt].CurHighestBidInfo.n64Bidden;
		 send.bidItemInfo[nCnt].bHighestPrice = (dwRoleID == m_NormalBidInfoList[nCnt].CurHighestBidInfo.dwRoleID)? TRUE:FALSE;
		 BidInfoMap* pBidinfoMap = m_NormalBidInfoList[nCnt].pBidInfoMap;
		 if (!P_VALID(pBidinfoMap))
		 {
			 send.bidItemInfo[nCnt].n64PlayerOfferPrice = 0;
		 }
		 else
		 {
			 INT64 n64Bidden = pBidinfoMap->Peek(dwRoleID);
			 if (n64Bidden != GT_INVALID && n64Bidden != 0)
				 send.bidItemInfo[nCnt].n64PlayerOfferPrice = n64Bidden;
			 else
				 send.bidItemInfo[nCnt].n64PlayerOfferPrice = 0;
		 }
	 }
	 send.dwErrorCode = E_Success;
	 send.dwLeftTime = GetBidEndLeftTime();
	 pRole->SendMessage(&send, send.dwSize);
 }

 VOID GodBidMgr::SendBlackMarketBidInfo2Client(Role* pRole)
 {
	 if (!P_VALID(pRole) || !IsSwitchOn())
		 return;

	 tagNS_BlackMarketShopOpen send;
	 if (!IsBidTime())
	 {
		 send.dwErrorCode = EGBEC_Black_OutOfTime;
		 pRole->SendMessage(&send, send.dwSize);
		 return;
	 }

	 DWORD dwRoleID = pRole->GetID();
	 send.dwErrorCode = E_Success;
	 send.dwLeftTime = GetBidEndLeftTime();
	 tagGodBidProto* pGodBidProto = g_attRes.GetGodBidProto(m_GoldBlackMarketBidInfo.dwGodBidProtoID);
	 if (P_VALID(pGodBidProto))
	 {
		 send.GoldItemInfo.sBidData.dwID = pGodBidProto->dwProtoID;
		 send.GoldItemInfo.sBidData.dwItemID = pGodBidProto->dwItemID;
		 send.GoldItemInfo.sBidData.dwMinBidPrice = pGodBidProto->dwBidLowerLimit;
		 send.GoldItemInfo.sBidData.dwStartPrice = pGodBidProto->dwBasePrice;
		 send.GoldItemInfo.sBidData.eType = pGodBidProto->eGodBidType;
		 send.GoldItemInfo.sBidData.nNum = pGodBidProto->dwItemCnt;
	 }
	 pGodBidProto = g_attRes.GetGodBidProto(m_ShengBiBlackMarketBidInfo.dwGodBidProtoID);
	 if (P_VALID(pGodBidProto))
	 {
		 send.ShengBiItemInfo.sBidData.dwID = pGodBidProto->dwProtoID;
		 send.ShengBiItemInfo.sBidData.dwItemID = pGodBidProto->dwItemID;
		 send.ShengBiItemInfo.sBidData.dwMinBidPrice = pGodBidProto->dwBidLowerLimit;
		 send.ShengBiItemInfo.sBidData.dwStartPrice = pGodBidProto->dwBasePrice;
		 send.ShengBiItemInfo.sBidData.eType = pGodBidProto->eGodBidType;
		 send.ShengBiItemInfo.sBidData.nNum = pGodBidProto->dwItemCnt;
	 }
	 if (P_VALID(m_GoldBlackMarketBidInfo.pBidInfoMap))
		 send.GoldItemInfo.bIsBid = (m_GoldBlackMarketBidInfo.pBidInfoMap->IsExist(dwRoleID) == TRUE)?true:false;
	 else
		 send.GoldItemInfo.bIsBid = FALSE;
	 if (P_VALID(m_ShengBiBlackMarketBidInfo.pBidInfoMap))
		 send.ShengBiItemInfo.bIsBid = (m_ShengBiBlackMarketBidInfo.pBidInfoMap->IsExist(dwRoleID) == TRUE)?true:false;
	 else
		 send.ShengBiItemInfo.bIsBid = FALSE;

	 pRole->SendMessage(&send, send.dwSize);
 }

VOID GodBidMgr::ProcessNormalBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price)
{
	if (!P_VALID(pRole) || n64Price <= 0 || !IsSwitchOn() || IsBidEnding())
		return;

	if (!IsBidTime())
	{
		tagNS_ConformPrice send;
		send.dwErrorCode = EGBEC_Black_OutOfTime;
		pRole->SendMessage(&send, send.dwSize);
		return;
	}

	tagGodBidProto* pGodBidProto = g_attRes.GetGodBidProto(dwProtoID);
	if (!P_VALID(pGodBidProto))
		return;

	INT64 n64NewPrice = n64Price;

	for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	{
		if (m_NormalBidInfoList[nCnt].dwGodBidProtoID == dwProtoID)
		{
			if (!P_VALID(m_NormalBidInfoList[nCnt].pBidInfoMap))
				m_NormalBidInfoList[nCnt].pBidInfoMap = new BidInfoMap;
			BidInfoMap* pBidInfoMap = m_NormalBidInfoList[nCnt].pBidInfoMap;
			if (!P_VALID(pBidInfoMap))
				return;

			// 检查是否是首次出价
			if (!pBidInfoMap->IsExist(pRole->GetID()))
			{
				n64NewPrice += pGodBidProto->dwBasePrice;
			}

			// 检查钱是否足够
			if (pRole->GetCurMgr().GetBagSilver() < n64NewPrice)
				return;

			// 扣钱
			if (!pRole->GetCurMgr().DecBagSilver(n64NewPrice, ELCID_Godbid_NormalBid_Cost))
				return;

			pBidInfoMap->ModifyValue(pRole->GetID(), n64NewPrice);

			BOOL bIsCurHighestBid = FALSE;
			
			INT64 n64Bidden = pBidInfoMap->Peek(pRole->GetID());

			if (m_NormalBidInfoList[nCnt].CurHighestBidInfo.dwRoleID == 0 || m_NormalBidInfoList[nCnt].CurHighestBidInfo.dwRoleID == GT_INVALID)
				bIsCurHighestBid = TRUE;
			else if (n64Bidden > m_NormalBidInfoList[nCnt].CurHighestBidInfo.n64Bidden)
				bIsCurHighestBid = TRUE;

			if (bIsCurHighestBid)
			{
				m_NormalBidInfoList[nCnt].CurHighestBidInfo.dwRoleID = pRole->GetID();
				m_NormalBidInfoList[nCnt].CurHighestBidInfo.n64Bidden = n64Bidden;
			}

			SaveOneGodBidInfo2DB(pRole->GetID(), n64NewPrice, EGBT_Normal, dwProtoID);

			tagNS_ConformPrice send;
			send.dwErrorCode = E_Success;
			send.n64Price = n64NewPrice;
			pRole->SendMessage(&send, send.dwSize);
			break;
		}
	}
}

VOID GodBidMgr::ProcessBlackBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price)
{
	if (!P_VALID(pRole) || n64Price <= 0 || !IsSwitchOn() || IsBidEnding())
		return;

	if (!IsBidTime())
	{
		tagNS_BlackConPrice send;
		send.dwErrorCode = EGBEC_Black_OutOfTime;
		pRole->SendMessage(&send, send.dwSize);
		return;
	}

	tagGodBidProto* pGodBidProto = g_attRes.GetGodBidProto(dwProtoID);
	if (!P_VALID(pGodBidProto))
		return;

	INT64 n64NewPrice = n64Price;

	tagBidInfoNode* pBidInfo = NULL;
	BOOL bGold = TRUE;
	if (dwProtoID == m_GoldBlackMarketBidInfo.dwGodBidProtoID)
		pBidInfo = &m_GoldBlackMarketBidInfo;
	else if (dwProtoID == m_ShengBiBlackMarketBidInfo.dwGodBidProtoID)
	{
		bGold = FALSE;
		pBidInfo = &m_ShengBiBlackMarketBidInfo;
	}
	else
		return;

	if (!P_VALID(pBidInfo->pBidInfoMap))
		pBidInfo->pBidInfoMap = new BidInfoMap;
	if (!P_VALID(pBidInfo->pBidInfoMap))
		return;

	DWORD dwErrorCode = EGBEC_Success;
	if (!pBidInfo->pBidInfoMap->IsExist(pRole->GetID()))
	{
		n64NewPrice += pGodBidProto->dwBasePrice;

		// 检查钱是否足够
		//INT64 n64Bidden = bGold? pRole->GetCurMgr().GetBagSilver() : pRole->GetCurMgr().GetBagYuanBao();
		//if (n64Bidden < n64NewPrice)
		//	return;

		INT64 n64Bidden = 0;
		if(bGold)
		{
			n64Bidden = pRole->GetCurMgr().GetBagSilver();
		}
		else
		{
			if( g_world.IsUseIPGSystem() )
			{
				n64Bidden = pRole->GetIPGCash();
			}
			else
			{
				n64Bidden = pRole->GetCurMgr().GetBagYuanBao();
			}
		}

		if (n64Bidden < n64NewPrice)
			return;

		// 扣钱
		if (bGold)
		{
			if (!pRole->GetCurMgr().DecBagSilver(n64NewPrice, ELCID_Godbid_BlackBid_Cost))
				return;
		}
		else
		{
			if( g_world.IsUseIPGSystem())
			{
				if(pRole->GetGetCash())
				{
					TCHAR tszTemp[64] = {0};
					wsprintf(tszTemp,_T("DecIPGCash5. Dec cash:%d. \r\n"),n64NewPrice);
					g_world.WriteKroeaLog(tszTemp);

					if( !pRole->DecIPGCash(n64NewPrice))
						return;
				}
			}
			else
			{
				if (!pRole->GetCurMgr().DecBagYuanBao(static_cast<INT32>(n64NewPrice), ELCID_Godbid_BlackBid_Cost))
					return;
			}
		}

		pBidInfo->pBidInfoMap->Add(pRole->GetID(), n64NewPrice);
	}
	else
	{
		dwErrorCode = EGBEC_Black_Only_One_Chance;
		goto end;
	}

	BOOL bIsCurHighestBid = FALSE;
	if (pBidInfo->CurHighestBidInfo.dwRoleID == 0 || pBidInfo->CurHighestBidInfo.dwRoleID == GT_INVALID)
		bIsCurHighestBid = TRUE;
	else if (n64NewPrice > pBidInfo->CurHighestBidInfo.n64Bidden)
		bIsCurHighestBid = TRUE;

	if (bIsCurHighestBid)
	{
		pBidInfo->CurHighestBidInfo.dwRoleID = pRole->GetID();
		pBidInfo->CurHighestBidInfo.n64Bidden = n64NewPrice;
	}

	BYTE BidType = bGold?EGBT_BlackMarket_Gold:EGBT_BlackMarket_ShengBi;
 	SaveOneGodBidInfo2DB(pRole->GetID(), n64NewPrice, BidType, dwProtoID);

end:
	tagNS_BlackConPrice send;
	send.dwErrorCode = dwErrorCode;
	if (dwErrorCode == E_Success)
	{
		send.bIsGod = bGold?true:false;
		send.n64Price = n64NewPrice;
	}
	pRole->SendMessage(&send, send.dwSize);
}

DWORD GodBidMgr::GetBidEndLeftTime()
{
	if (!IsBidTime()  || !IsSwitchOn())
		return 0;
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	DWORD dwLeftSec = (60-dwCurTime.min)*60+60-dwCurTime.sec;
	if (dwLeftSec > 3600)
		dwLeftSec = 3600;
	return dwLeftSec;
}

// 返还其他玩家金钱
void GodBidMgr::GiveMoneyBack2Player( BidInfoMap* pBidInfoMap, EGodBidType eGodBidType ) 
{
	BidInfoMap::TMapIterator iterator = pBidInfoMap->Begin();
	INT64 n64Bidden = 0;
	DWORD dwFailerID =0;
	while (pBidInfoMap->PeekNext(iterator, dwFailerID, n64Bidden))
	{
		if (n64Bidden > 0)
		{
			Role* pFailer = g_roleMgr.GetRolePtrByID(dwFailerID);
			if (!P_VALID(pFailer))
			{
				// 直接向数据库中返还金钱
				if (eGodBidType == EGBT_BlackMarket_ShengBi)
					CurrencyMgr::IncBagYuanBao(dwFailerID, static_cast<INT32>(n64Bidden), ELCID_Godbid_NormalBid_Added);
				else
					CurrencyMgr::IncBagGold(dwFailerID, n64Bidden, ELCID_Godbid_NormalBid_Added);
			}
			else
			{
				if (eGodBidType == EGBT_BlackMarket_ShengBi)
					pFailer->GetCurMgr().IncBagYuanBao(static_cast<INT32>(n64Bidden), ELCID_Godbid_NormalBid_Added);
				else
				{
					if( g_world.IsUseIPGSystem() && (!pFailer->GetGetCash() ))
						continue;
					pFailer->GetCurMgr().IncBagSilver(n64Bidden, ELCID_Godbid_NormalBid_Added);
				}
			}
		}
	}
}

// 处理数据库操作
VOID GodBidMgr::InitGodBidInfoFromDB(DWORD dwBidCount, tagGodBidInfo2DB* pGodBidInfo2DB)
{
	if (dwBidCount == 0 || dwBidCount == GT_INVALID || !P_VALID(pGodBidInfo2DB))
		return;

	// 将数据转存到map里面
	BidInfoMap mapNormalBidInfo[MAX_GODBID_ITEM_NUM];
	BidInfoMap mapBlackGoldBidInfo;
	BidInfoMap mapShengBiBidInfo;
	INT nNormalBidId[MAX_GODBID_ITEM_NUM] = {0};
	for (DWORD dwCnt = 0; dwCnt < dwBidCount; dwCnt++)
	{
		if (pGodBidInfo2DB[dwCnt].byBidType == EGBT_Normal)
		{
			INT nIdx = GT_INVALID;
			for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
			{
				if (nNormalBidId[nCnt] == 0)
					nNormalBidId[nCnt] = pGodBidInfo2DB[dwCnt].dwBidID;
				if (nNormalBidId[nCnt] == pGodBidInfo2DB[dwCnt].dwBidID)
				{
					nIdx = nCnt;
					break;
				}
			}
			if (nIdx != GT_INVALID)
				mapNormalBidInfo[nIdx].Add(pGodBidInfo2DB[dwCnt].dwRoleID, pGodBidInfo2DB[dwCnt].n64Bidden);
		}
		else if (pGodBidInfo2DB[dwCnt].byBidType == EGBT_BlackMarket_Gold)
			mapBlackGoldBidInfo.Add(pGodBidInfo2DB[dwCnt].dwRoleID, pGodBidInfo2DB[dwCnt].n64Bidden);
		else if (pGodBidInfo2DB[dwCnt].byBidType == EGBT_BlackMarket_ShengBi)
			mapShengBiBidInfo.Add(pGodBidInfo2DB[dwCnt].dwRoleID, pGodBidInfo2DB[dwCnt].n64Bidden);
	}

	// 先将钱还给玩家
	for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	{
		GiveMoneyBack2Player(&mapNormalBidInfo[nCnt], EGBT_Normal);	
	}
	GiveMoneyBack2Player(&mapBlackGoldBidInfo, EGBT_BlackMarket_Gold);	
	GiveMoneyBack2Player(&mapShengBiBidInfo, EGBT_BlackMarket_ShengBi);	

	// 通知数据库，清空竞标记录
	ClearGodBidInfoFromDB(FALSE);
}

VOID GodBidMgr::SaveOneGodBidInfo2DB(DWORD dwRoleID, INT64 n64Bidden, BYTE byBidType, DWORD dwBidID)
{
	tagNDBC_SaveOneGodBidInfo send;

	send.GodBidInfo2DB.byBidType = byBidType;
	send.GodBidInfo2DB.dwRoleID = dwRoleID;
	send.GodBidInfo2DB.n64Bidden = n64Bidden;
	send.GodBidInfo2DB.dwBidID = dwBidID;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GodBidMgr::ClearGodBidInfoFromDB(BOOL bBidEnd)
{
	tagNDBC_ClearGodBidInfo send;
	send.bBidEnd = bBidEnd;
	g_dbSession.Send(&send, send.dwSize);
}

VOID GodBidMgr::LoadGodBidInfoFromDB()
{
	tagNDBC_LoadGodBidInfo send;
	g_dbSession.Send(&send, send.dwSize);
}

VOID GodBidMgr::ClearGodBidInfoFromMemory()
{
	for (INT nCnt = 0; nCnt < MAX_GODBID_ITEM_NUM; nCnt++)
	{
		m_NormalBidInfoList[nCnt].CurHighestBidInfo.dwRoleID = 0;
		m_NormalBidInfoList[nCnt].CurHighestBidInfo.n64Bidden = 0;
		m_NormalBidInfoList[nCnt].dwGodBidProtoID = 0;
		if (P_VALID(m_NormalBidInfoList[nCnt].pBidInfoMap))
		{
			m_NormalBidInfoList[nCnt].pBidInfoMap->Clear();
			SAFE_DEL(m_NormalBidInfoList[nCnt].pBidInfoMap);
		}
	}

	m_GoldBlackMarketBidInfo.CurHighestBidInfo.dwRoleID = 0;
	m_GoldBlackMarketBidInfo.CurHighestBidInfo.n64Bidden = 0;
	m_GoldBlackMarketBidInfo.dwGodBidProtoID = 0;
	if (P_VALID(m_GoldBlackMarketBidInfo.pBidInfoMap))
	{
		m_GoldBlackMarketBidInfo.pBidInfoMap->Clear();
		SAFE_DEL(m_GoldBlackMarketBidInfo.pBidInfoMap);
	}

	m_ShengBiBlackMarketBidInfo.CurHighestBidInfo.dwRoleID = 0;
	m_ShengBiBlackMarketBidInfo.CurHighestBidInfo.n64Bidden = 0;
	m_ShengBiBlackMarketBidInfo.dwGodBidProtoID = 0;
	if (P_VALID(m_ShengBiBlackMarketBidInfo.pBidInfoMap))
	{
		m_ShengBiBlackMarketBidInfo.pBidInfoMap->Clear();
		SAFE_DEL(m_ShengBiBlackMarketBidInfo.pBidInfoMap);
	}
}

VOID GodBidMgr::LogGodBid(DWORD dwRoleID, DWORD dwAccountID, DWORD dwItemID, BYTE byMoneyType, DWORD dwMoney)
{
	tagNDBC_LogGodBid send;

	send.sLogGodBid.dwAccountID	= dwAccountID;
	send.sLogGodBid.dwRoleID = dwRoleID;
	send.sLogGodBid.dwItemID = dwItemID;
	send.sLogGodBid.byMoneyType = byMoneyType;
	send.sLogGodBid.dwMoney = dwMoney;

	g_dbSession.Send(&send, send.dwSize);
}

BOOL GodBidMgr::ReLoadGodBidProto()
{
	// 重新读取资源文件
	g_attRes.ReloadGodBidProto();
	return TRUE;
}
