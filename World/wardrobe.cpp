//-------------------------------------------------------------------------------------
// 衣橱系统逻辑实现
//-------------------------------------------------------------------------------------
#include "stdafx.h"
#include "att_res.h"
#include "role.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/msg_fashion.h"
#include "wardrobe.h"

Wardrobe::Wardrobe(DWORD dwRoleID)
:m_dwCurWareLayer(GT_INVALID),
m_dwPlayActingFriend(GT_INVALID),
m_dwPlayActingLayerID(GT_INVALID),
m_dwPlayActRequestStartTime(0),
m_dwRoleID(dwRoleID),
m_dwBePlayActLayerID(GT_INVALID),
m_byBePlayActLevel(0)
{
	m_mapWardrobeInfo.Clear();
	m_mapPlayAct.Clear();
}
Wardrobe::~Wardrobe()
{
	tagWardrobeLayer* pWardrobeLayer = NULL;
	m_mapWardrobeInfo.ResetIterator();
	while (m_mapWardrobeInfo.PeekNext(pWardrobeLayer))
	{
		SAFE_DEL(pWardrobeLayer);
	}
	m_mapWardrobeInfo.Clear();
	m_mapPlayAct.Clear();
	m_dwPlayActingFriend = GT_INVALID;
	m_dwCurWareLayer = GT_INVALID;
	m_dwPlayActingLayerID = GT_INVALID;
	m_dwPlayActRequestStartTime = 0;
}

DWORD Wardrobe::PlayActMyself(DWORD dwLayerID)
{
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (!P_VALID(pWardrobeLayer))
		return GT_INVALID;

	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwLayerID);
	if (!P_VALID(pWardrobeProto))
		return GT_INVALID;

	if (pWardrobeLayer->eState == EFS_Incomplete || pWardrobeLayer->eState == EFS_Complete)
		;
	else if (pWardrobeLayer->eState == EFS_None)
		return EFEC_None;
	else if (pWardrobeLayer->eState == EFS_UseBySelf)
		return EFEC_UseBySelf;
	else if (pWardrobeLayer->eState == EFS_UseForFriends)
		return EFEC_UseForFriends;
	else
		return GT_INVALID;

	return E_Success;
}

// 改变衣橱栏位状态
VOID Wardrobe::ChangeLayerState(DWORD dwLayerID, EFashionState eFashionState)
{
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (!P_VALID(pWardrobeLayer))
		return;
	
	bool bComplete = MIsFashionSuitComplete(pWardrobeLayer->eState);
	if (IsFashionStateValid(eFashionState) && eFashionState != pWardrobeLayer->eState)
	{
		if (eFashionState == EFS_UseBySelf)
			m_dwCurWareLayer = dwLayerID;
		if (pWardrobeLayer->eState == EFS_UseBySelf)
			m_dwCurWareLayer = GT_INVALID;
		pWardrobeLayer->eState = eFashionState;

		SyncLayerInfo(dwLayerID, pWardrobeLayer->byLevel, pWardrobeLayer->eState);
		SaveWardrobeLayerInfo2DB(dwLayerID);
	}

	if (bComplete ^ (MIsFashionSuitComplete(pWardrobeLayer->eState)))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
		if (P_VALID(pRole))
			pRole->OnCompleteFashionSuitNumChg();
	}
}

VOID Wardrobe::SyncLayerInfo(DWORD dwLayerID, BYTE byLevel, EFashionState eFashionState)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
	if (P_VALID(pRole))
	{
		MCREATE_MSG(pSend, (sizeof(tagNS_SyncFashionInfo)+sizeof(tagFashionInfo)), NS_SyncFashionInfo);
		if (P_VALID(pSend))
		{
			pSend->nSuitNum = 1;
			tagFashionInfo FashionInfo = {dwLayerID, byLevel, eFashionState};
			memcpy(pSend->FashionInfo, &FashionInfo, sizeof(tagFashionInfo));
			pRole->SendMessage(pSend, pSend->dwSize);
		}
	}
}

// 升级衣橱
DWORD Wardrobe::LevelUpWardrobeLayer(Role* pRole, DWORD dwLayerID)
{
	DWORD dwErrorCode = GT_INVALID;
	if (!P_VALID(pRole))
		return GT_INVALID;

	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (!P_VALID(pWardrobeLayer))
		return GT_INVALID;

	if (pWardrobeLayer->byLevel == MAX_LEVEL_PERLAYER)
		return GT_INVALID;

	// 检查道具是否存在
	tagItem* pItem = pRole->GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_WardrobeLayerLvlUp);
	if (!P_VALID(pItem))
	{
		tagNS_WardrobeLevelUp send;
		send.dwErrorCode = EFEC_NoLevelUpItem;
		pRole->SendMessage(&send, send.dwSize);
		return EFEC_NoLevelUpItem;
	}

	if (E_Success != pRole->GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_Wardrobe_Layer_LevelUp_Cost,1))
	{
		IMSG(_T("LevelUpWardrobeLayer::DelFromBagFailed!!!\r\n"));
		return GT_INVALID;
	}
	pWardrobeLayer->byLevel++;
	tagNS_WardrobeLevelUp send;
	send.dwErrorCode = E_Success;
	pRole->SendMessage(&send, send.dwSize);

	SyncLayerInfo(dwLayerID, pWardrobeLayer->byLevel, pWardrobeLayer->eState);
	SaveWardrobeLayerInfo2DB(dwLayerID);

	pRole->OnWardLevelUp();

	return dwErrorCode;
}

// 开始装扮好友
DWORD Wardrobe::StartPlayActFriend(DWORD dwLayerID, DWORD dwFriendID, INT nPlayActFriendLimit)
{
	if (m_dwPlayActingFriend != GT_INVALID)
		return EFEC_IsWaitingPlayActFriendReq;

	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (!P_VALID(pWardrobeLayer))
		return GT_INVALID;

	if (pWardrobeLayer->eState == EFS_Complete)
		;
	else if (pWardrobeLayer->eState == EFS_UseBySelf)
		return EFEC_UseBySelf;
	else if (pWardrobeLayer->eState == EFS_UseForFriends)
		return EFEC_UseForFriends;
	else if (pWardrobeLayer->eState == EFS_None)
		return EFEC_None;
	else
		return GT_INVALID;

	if (m_mapPlayAct.Size() >= nPlayActFriendLimit)
		return EFEC_PlayActCountLimit;

	m_dwPlayActingFriend = dwFriendID;
	m_dwPlayActingLayerID = dwLayerID;
	m_dwPlayActRequestStartTime = GetCurrentDWORDTime();
	return E_Success;
}

// 结束装扮好友
DWORD Wardrobe::EndPlayActFriend(DWORD dwFriendID, BYTE& byLevel, DWORD& dwLayerID, BOOL bAgree)
{
	if (m_dwPlayActingFriend != dwFriendID)
		return GT_INVALID;
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwPlayActingLayerID);
	if (!P_VALID(pWardrobeLayer))
		return GT_INVALID;

	if (pWardrobeLayer->eState == EFS_Complete)
		;
	else if (pWardrobeLayer->eState == EFS_UseBySelf)
		return EFEC_UseBySelf;
	else if (pWardrobeLayer->eState == EFS_UseForFriends)
		return EFEC_UseForFriends;
	else if (pWardrobeLayer->eState == EFS_None)
		return EFEC_None;
	else
		return GT_INVALID;

	if (bAgree)
	{
		m_mapPlayAct.Add(m_dwPlayActingLayerID, m_dwPlayActingFriend);
		ChangeLayerState(m_dwPlayActingLayerID, EFS_UseForFriends);
		pWardrobeLayer->dwPlayActStartTime = GetCurrentDWORDTime();
		pWardrobeLayer->dwFriendID = m_dwPlayActingFriend;
		SaveWardrobeLayerInfo2DB(m_dwPlayActingLayerID);
	}
	dwLayerID = m_dwPlayActingLayerID;
	byLevel = pWardrobeLayer->byLevel;
	m_dwPlayActingFriend = GT_INVALID;
	m_dwPlayActingLayerID = GT_INVALID;
	m_dwPlayActRequestStartTime = 0;
	
	return E_Success;
}

// 更新衣橱状态
VOID Wardrobe::Update()
{
	// 更新装扮好友
	UpdatePlayActRequest();

	// 更新装扮好友请求
	UpdatePlayActCDTime();

	UpdateBePlayAct();
}

BOOL Wardrobe::CanAddFashion(INT16 n16TargetIndex, tagItem* pItem)
{
	if (!P_VALID(pItem))
		return FALSE;

	if (!P_VALID(pItem->pEquipProto))
		return FALSE;

	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
	if (!P_VALID(pRole))
		return FALSE;

	DWORD dwLayer = CalLayerID(n16TargetIndex);
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayer);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwLayer);
	if (!P_VALID(pWardrobeLayer) || !P_VALID(pWardrobeProto))
		return FALSE;
	if (pWardrobeLayer->eState == EFS_UseBySelf || !IsFashionStateValid(pWardrobeLayer->eState))
		return FALSE;

	BYTE bySex = (pItem->pEquipProto->eSexLimit == ESL_Woman)?0:1;
	if (bySex != pRole->GetSex() && pWardrobeProto->eLayerType == EWLT_Diy)
		return GT_INVALID;

	return TRUE;
}

VOID Wardrobe::FixWardrobeLayerState(tagWardrobeLayer* pNewWardrobeLayer, Role* pRole)
{
	if (pNewWardrobeLayer->eState == EFS_UseBySelf)
	{
		if (m_dwCurWareLayer == GT_INVALID)
		{
			m_dwCurWareLayer = pNewWardrobeLayer->dwLayerID;
		}
		else
		{
			if (P_VALID(pRole))
			{
				DWORD dwLayerID = pNewWardrobeLayer->dwLayerID;
				DWORD dwIdx = (dwLayerID>0)? (dwLayerID-1)*MAX_FASHION_PERLAYER : 0 ;
				pNewWardrobeLayer->eState = pRole->OnFashionChange(dwIdx, dwLayerID);
			}
		}
	}
	else if (pNewWardrobeLayer->eState == EFS_UseForFriends)
	{
		m_mapPlayAct.Add(pNewWardrobeLayer->dwLayerID, pNewWardrobeLayer->dwFriendID);
	}
}

VOID Wardrobe::InitWardrobe(const BYTE* &pData, DWORD dwLayerCnt, Role* pRole)
{
	tagWardrobeLayer2DB* pWardrobeLayer = (tagWardrobeLayer2DB*)pData;
	if (!P_VALID(pWardrobeLayer))
		return;

	InitWardrobe(pRole, dwLayerCnt, pWardrobeLayer);

	// 重设指针
	pData = (const BYTE*)((tagWardrobeLayer2DB*)pData + dwLayerCnt);
}

VOID Wardrobe::InitWardrobe(Role* pRole, DWORD dwLayerCnt, tagWardrobeLayer2DB* pWardrobeLayer)
{
	TMap<DWORD,tagWardrobeProto *>& mapWardrobeProto = g_attRes.GetWardrobeProtoMap();
	if (mapWardrobeProto.Size() <= 0)
		return;

	for (DWORD nCnt = 0; nCnt < dwLayerCnt; nCnt++)
	{
		if (!P_VALID(pWardrobeLayer))
			break;
		if (!P_VALID(g_attRes.GetWardrobeProto(pWardrobeLayer[nCnt].dwLayerID)))
			continue;
		tagWardrobeLayer* pNewWardrobeLayer = new tagWardrobeLayer(
			pWardrobeLayer[nCnt].byLevel,
			(EFashionState)pWardrobeLayer[nCnt].byState,
			pWardrobeLayer[nCnt].dwPlayActStartTime,
			pWardrobeLayer[nCnt].dwLayerID,
			0,// 气质值需要重新计算
			pWardrobeLayer[nCnt].dwFriendID);
		if (P_VALID(pNewWardrobeLayer))
		{
			// 进行衣橱栏位状态修正，主要是衣橱栏位可能是自己穿着或者装扮好友状态
			FixWardrobeLayerState(pNewWardrobeLayer, pRole);
			m_mapWardrobeInfo.Add(pNewWardrobeLayer->dwLayerID, pNewWardrobeLayer);
		}
	}
	TMap<DWORD,tagWardrobeProto *>::TMapIterator iterator = mapWardrobeProto.Begin();
	tagWardrobeProto* pWardrobeProto = NULL;
	while(mapWardrobeProto.PeekNext(iterator, pWardrobeProto))
	{
		if (!P_VALID(pWardrobeProto))
			continue;
		if (!m_mapWardrobeInfo.IsExist(pWardrobeProto->dwLayerID))
		{
			tagWardrobeLayer* pNewWardrobeLayer = new tagWardrobeLayer(pWardrobeProto->dwLayerID);
			m_mapWardrobeInfo.Add(pWardrobeProto->dwLayerID, pNewWardrobeLayer);
		}
	}

	if (P_VALID(pRole))
	{
		pRole->OnCompleteFashionSuitNumChg();
	}

}

VOID Wardrobe::UpdatePlayActCDTime()
{
	PlayActMap::TMapIterator iterator = m_mapPlayAct.Begin();
	DWORD dwRoleID = GT_INVALID;
	DWORD dwLayerID = GT_INVALID;
	while(m_mapPlayAct.PeekNext(iterator, dwLayerID, dwRoleID))
	{
		tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
		if (!P_VALID(pWardrobeLayer))
		{
			m_mapPlayAct.Erase(dwLayerID);
			continue;
		}
		if ((pWardrobeLayer->eState != EFS_UseForFriends )||
			(pWardrobeLayer->dwPlayActStartTime == 0) ||
			(PLAYACT_CDTIME < CalcTimeDiff(GetCurrentDWORDTime(), pWardrobeLayer->dwPlayActStartTime)))
		{
			ChangeLayerState(dwLayerID, EFS_Complete);
			pWardrobeLayer->dwPlayActStartTime = 0;
			pWardrobeLayer->dwFriendID = GT_INVALID;
			m_mapPlayAct.Erase(dwLayerID);
			SaveWardrobeLayerInfo2DB(m_dwPlayActingLayerID);
		}
	}
}

VOID Wardrobe::UpdatePlayActRequest()
{
	if (m_dwPlayActingFriend == GT_INVALID)
		return;
	if (m_dwPlayActRequestStartTime == 0)
	{
		m_dwPlayActingFriend  = GT_INVALID;
		m_dwPlayActingLayerID = GT_INVALID;
	}
	
	if (PLAYACT_REQUEST_TIMEOUT < CalcTimeDiff(GetCurrentDWORDTime(), m_dwPlayActRequestStartTime))
	{
		m_dwPlayActingFriend  = GT_INVALID;
		m_dwPlayActingLayerID = GT_INVALID;
		m_dwPlayActRequestStartTime = 0;
	}
}

VOID Wardrobe::UpdateBePlayAct()
{
	if (m_dwBePlayActLayerID == GT_INVALID)
		return;

	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwBePlayActLayerID);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(m_dwBePlayActLayerID);
	if (!P_VALID(pWardrobeLayer) || !P_VALID(pWardrobeProto))
		return;

	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
	if (!P_VALID(pRole))
		return;

	BYTE byLevel = (m_byBePlayActLevel >0)?(m_byBePlayActLevel-1):0;
	if (!pRole->IsHaveBuff(Buff::GetIDFromTypeID(pWardrobeProto->dwOtherBuffIDByLvl[byLevel])))
	{
		m_dwBePlayActLayerID = GT_INVALID;
		m_byBePlayActLevel = 0;
		pRole->ResetWardrobeDisplay();
		if (P_VALID(pRole->GetMap()) && pRole->GetDisplaySet().bFashionDisplay)
		{
			tagNS_AvatarEquip send;
			send.dwRoleID		= m_dwRoleID;
			send.bFashion		= TRUE;
			send.sAvatarEquip	= pRole->GetAvatarEquip();
			pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}
	}
}

VOID Wardrobe::SyncWardrobeInfo(Role* pRole)
{
	if (!P_VALID(pRole))
		return;

	DWORD dwMapSize = m_mapWardrobeInfo.Size();
	if (dwMapSize == 0)
		return;

	MCREATE_MSG(pSend, (sizeof(tagNS_SyncFashionInfo)+dwMapSize*sizeof(tagFashionInfo)), NS_SyncFashionInfo);
	if (!P_VALID(pSend))
		return;

	WardrobeInfoMap::TMapIterator iterator = m_mapWardrobeInfo.Begin();
	tagWardrobeLayer* pWardrobeLayer = NULL;
	tagFashionInfo* pFashionInfo = (tagFashionInfo*)pSend->FashionInfo;
	INT nIdx = 0;
	while(m_mapWardrobeInfo.PeekNext(iterator, pWardrobeLayer))
	{
		if (!P_VALID(pWardrobeLayer))
			continue;
		pFashionInfo[nIdx].byLevel = pWardrobeLayer->byLevel;
		pFashionInfo[nIdx].dwIndex = pWardrobeLayer->dwLayerID;
		pFashionInfo[nIdx].eState = pWardrobeLayer->eState;
		nIdx++;
	}
	pSend->nSuitNum = nIdx;
	if (pSend->nSuitNum <= 0)
		return;
	pRole->SendMessage(pSend, pSend->dwSize);
}

VOID Wardrobe::SaveWardrobeLayerInfo2DB(DWORD dwLayerID)
{
	if (m_dwRoleID == 0)
		return;
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (!P_VALID(pWardrobeLayer))
		return;
	tagNDBC_SaveWardrobeLayerInfo send;
	send.WardrobeLayerInfo.dwRoleID = m_dwRoleID;
	send.WardrobeLayerInfo.dwLayerID = dwLayerID;
	send.WardrobeLayerInfo.byLevel = pWardrobeLayer->byLevel;
	send.WardrobeLayerInfo.byState = (BYTE)pWardrobeLayer->eState;
	send.WardrobeLayerInfo.dwPlayActStartTime = pWardrobeLayer->dwPlayActStartTime;
	send.WardrobeLayerInfo.dwFriendID = pWardrobeLayer->dwFriendID;
	g_dbSession.Send(&send, send.dwSize);
}

DWORD Wardrobe::PutOffCurFashion()
{
	Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
	if (P_VALID(pRole) && m_dwCurWareLayer != GT_INVALID)
	{
		tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwCurWareLayer);
		tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(m_dwCurWareLayer);
		if (P_VALID(pWardrobeLayer) && P_VALID(pWardrobeProto))
		{
			if (pWardrobeLayer->eState == EFS_UseBySelf)
			{
				DWORD dwLayerID = GT_INVALID;
				DWORD dwIdx = (m_dwCurWareLayer>1)?((m_dwCurWareLayer-1)*MAX_FASHION_PERLAYER):0;
				BYTE byLevel = (pWardrobeLayer->byLevel >0)?(pWardrobeLayer->byLevel-1):0;
				EFashionState eFashionState = pRole->OnFashionChange(dwIdx, dwLayerID);
				pRole->GetWardrobe().ChangeLayerState(m_dwCurWareLayer, eFashionState);
				pRole->RemoveBuff(Buff::GetIDFromTypeID(pWardrobeProto->dwBuffIDByLvl[byLevel]), TRUE);
				pRole->UnsetRoleState(ERS_WardrobeWare);
				return E_Success;
			}
		}
	}
	return GT_INVALID;
}

VOID Wardrobe::CancelBePlayActing(Role* pRole)
{
	if (P_VALID(pRole))
	{
		if (m_dwBePlayActLayerID == GT_INVALID)
			return;

		tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(m_dwBePlayActLayerID);
		if ( !P_VALID(pWardrobeProto))
			return;
		pRole->ResetWardrobeDisplay();
		BYTE byLevel = (m_byBePlayActLevel >0)?(m_byBePlayActLevel-1):0;
		pRole->RemoveBuff(Buff::GetIDFromTypeID(pWardrobeProto->dwOtherBuffIDByLvl[byLevel]), TRUE);
		m_dwBePlayActLayerID = GT_INVALID;
		m_byBePlayActLevel = 0;
	}
}

DWORD Wardrobe::PutOnCurFashion(Role* pRole, BOOL bInitPutOnFashion/* = FALSE*/)
{
	DWORD dwErrorCode = GT_INVALID;
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwCurWareLayer);
	if (P_VALID(pWardrobeLayer))
		dwErrorCode = PutOnFashion(pRole, m_dwCurWareLayer, pWardrobeLayer->byLevel, TRUE, FALSE, bInitPutOnFashion);
	return dwErrorCode;
}

DWORD Wardrobe::PutOnBePlayActFashion(Role* pRole, BOOL bInitPutOnFashion/* = FALSE*/)
{
	DWORD dwErrorCode = GT_INVALID;
	if (IsPlayActing())// 不可能同时穿了时装并且有被装扮
	{
		tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(m_dwBePlayActLayerID);
		if (P_VALID(pWardrobeProto))
		{
			BYTE byLevel = (m_byBePlayActLevel >0)?(m_byBePlayActLevel-1):0;
			pRole->RemoveBuff(Buff::GetIDFromTypeID(pWardrobeProto->dwOtherBuffIDByLvl[byLevel]), TRUE);
			dwErrorCode = E_Success;
		}
		m_dwBePlayActLayerID = GT_INVALID;
		m_byBePlayActLevel = 0;
	}
	else
	{
		tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwBePlayActLayerID);
		if (P_VALID(pWardrobeLayer))
			dwErrorCode = PutOnFashion(pRole, m_dwBePlayActLayerID, m_byBePlayActLevel, FALSE, FALSE, bInitPutOnFashion);
	}
	return dwErrorCode;
}

// 换装操作
DWORD Wardrobe::PutOnFashion(Role* pRole, DWORD dwWardrobeLayerID, BYTE byLevel, BOOL bSelf, BOOL bSendMsg, BOOL bInitPutOnFashion)
{
	if (!P_VALID(pRole))
		return GT_INVALID;

	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwWardrobeLayerID);
	if (!P_VALID(pWardrobeProto))
		return GT_INVALID;

	if (!bSelf && pWardrobeProto->eLayerType == EWLT_Diy)
		return EFEC_DiyLayerPlayActLimit;

	// 检查性别
	if (pWardrobeProto->eLayerType == EWLT_Normal)
	{
		EWardrobeLayerSex eSex = (pRole->GetSex() == 0)?EWLS_Women:EWLS_Men;
		if (pWardrobeProto->eLayerSex != eSex)
			return EFEC_Sex_NotMatch;
	}

	// 原来好友的装扮状态删除
	CancelBePlayActing(pRole);

	if (!bSelf)
	{
		SetBePlayActLayerID(dwWardrobeLayerID);
		SetBePlayActLevel(byLevel);
	}

	byLevel = (byLevel >0)?(byLevel-1):0;
	DWORD dwStartIdx = (dwWardrobeLayerID>0)?((dwWardrobeLayerID-1)*MAX_FASHION_PERLAYER):0;
	DWORD dwBuffID = bSelf?pWardrobeProto->dwBuffIDByLvl[byLevel]:pWardrobeProto->dwOtherBuffIDByLvl[byLevel];

	pRole->ResetWardrobeDisplay();
	for (INT nWardrobeFashionPos = 0; nWardrobeFashionPos < EWFP_End; nWardrobeFashionPos++)
	{
		EAvatarElement eAvatarElement = pRole->GetItemMgr().GetWardrobeContainer().EWFP2EAE((EWardrobeFashionPos)nWardrobeFashionPos);
		if (eAvatarElement == EAE_Null)
			continue;
		if (bSelf)
		{
			tagItem* pItem = pRole->GetItemMgr().GetWardrobeContainer().GetItem((INT16)(dwStartIdx+nWardrobeFashionPos));
			if (P_VALID(pItem))
				pRole->SetWardrobeDisplay(eAvatarElement, pItem->dwTypeID, X_DEFAULT_FLARE_VAL, EC_Null);
			else
				pRole->SetWardrobeDisplay(eAvatarElement, GT_INVALID, X_DEFAULT_FLARE_VAL, EC_Null);
		}
		else
		{
			pRole->SetWardrobeDisplay(eAvatarElement, pWardrobeProto->dwFashionID[nWardrobeFashionPos], X_DEFAULT_FLARE_VAL, EC_Null);
		}
	}
	tagEquip* pREquip = pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_RightHand);
	tagEquip* pLEquip = pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_LeftHand);
	DWORD dwREquipID = (P_VALID(pREquip))?pREquip->dwTypeID:GT_INVALID;
	DWORD dwLEquipID = (P_VALID(pLEquip))?pLEquip->dwTypeID:GT_INVALID;
	pRole->SetWardrobeDisplay(EAE_RWeapon, dwREquipID, X_DEFAULT_FLARE_VAL, EC_Null);
	pRole->SetWardrobeDisplay(EAE_LWeapon, dwLEquipID, X_DEFAULT_FLARE_VAL, EC_Null);
	BOOL bAddBuff = FALSE;
	if (bSelf)
	{
		tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwWardrobeLayerID);
		if (P_VALID(pWardrobeLayer) && pWardrobeLayer->eState == EFS_Complete)
			bAddBuff = TRUE;
		ChangeLayerState(dwWardrobeLayerID, EFS_UseBySelf);
	}
	else
	{
		bAddBuff = TRUE;
	}
	if (bAddBuff && !bInitPutOnFashion && !pRole->IsHaveBuff(Buff::GetIDFromTypeID(dwBuffID)))
	{
		pRole->TryAddBuffToMyself(dwBuffID);
	}

	if (bSelf)
		pRole->SetRoleState(ERS_WardrobeWare);

	if (P_VALID(pRole->GetMap()) && pRole->GetDisplaySet().bFashionDisplay)
	{
		tagNS_AvatarEquip send;
		send.dwRoleID		= pRole->GetID();
		send.bFashion		= TRUE;
		send.sAvatarEquip	= pRole->GetAvatarEquip();
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}
	
	return E_Success;
}

VOID Wardrobe::SetFashionTemperament(DWORD dwLayerID, BYTE byFashionTemperament)
{
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(dwLayerID);
	if (P_VALID(pWardrobeLayer))
		pWardrobeLayer->byFashionTemperament = byFashionTemperament;
}

BYTE Wardrobe::GetCurFashionTemperament()
{
	if (m_dwCurWareLayer == GT_INVALID)
		return 0;
	tagWardrobeLayer* pWardrobeLayer = m_mapWardrobeInfo.Peek(m_dwCurWareLayer);
	if (!P_VALID(pWardrobeLayer))
		return 0;
	return pWardrobeLayer->byFashionTemperament;
}

VOID Wardrobe::MaggriageMakeUp(DWORD dwRoleID, DWORD dwLayerID)
{
	m_dwPlayActingFriend = dwRoleID; 
	m_dwPlayActingLayerID = dwLayerID; 
	m_dwPlayActRequestStartTime = GetCurrentDWORDTime();

	tagWardrobeLayer* pLayer = m_mapWardrobeInfo.Peek(m_dwPlayActingLayerID);
	if (!P_VALID(pLayer))
	{
		return;
	}

	pLayer->eState = EFS_Complete;
}

VOID Wardrobe::GetFashionTemperamentSum(INT& nSum, INT& nSuit1Sum, INT& nSuit2Sum, INT& nSuit3Sum, INT& nSuitSum)
{
	nSum = 0; // 气质值和
	nSuit1Sum = 0; // 集齐的时装套数
	nSuit2Sum = 0;
	nSuit3Sum = 0;
	nSuitSum = m_mapWardrobeInfo.Size(); // 时装总套数

	Wardrobe::WardrobeInfoMap::TMapIterator it = m_mapWardrobeInfo.Begin();
	tagWardrobeLayer* pLayer = NULL;
	DWORD dwTmp = 0;
	Role* pRole = NULL;
	while (m_mapWardrobeInfo.PeekNext(it, dwTmp, pLayer))
	{
		if (!P_VALID(dwTmp) && !P_VALID(pLayer))
		{
			continue;
		}

		if (!P_VALID(pRole) && 0 == pLayer->byFashionTemperament )
		{
			pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
			if (!P_VALID(pRole))
			{
				return;
			}

			DWORD dwMaxIdx = MAX_FASHION_PERLAYER*m_mapWardrobeInfo.Size();
			for (DWORD dwIdx=0; dwIdx< dwMaxIdx; )
			{
				pRole->OnFashionChange(dwIdx, pLayer->dwLayerID);
				dwIdx+= MAX_FASHION_PERLAYER;
			}
		}

		nSum += pLayer->byFashionTemperament;

		if(MIsFashionSuitComplete(pLayer->eState))
		{
			if (1==pLayer->byLevel)
				nSuit1Sum++;
			else if (2==pLayer->byLevel)
				nSuit2Sum++;
			else if(3==pLayer->byLevel)
				nSuit3Sum++;
		}
	}
}
