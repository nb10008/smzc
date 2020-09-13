#include "StdAfx.h"
#include "PickupMgr.h"
#include "../WorldDefine/msg_loot.h"
#include "PickStrategyFrame.h"
#include "../WorldDefine/ItemDefine.h"
#include "ItemProtoData.h"

PickupMgr PickupMgr::s_guiPickMgr;
PickupMgr* PickupMgr::Inst()
{
	return &s_guiPickMgr;
}
PickupMgr::PickupMgr(void):m_Trunk(this)
{
}

PickupMgr::~PickupMgr(void)
{
}

VOID PickupMgr::Init()
{
	m_pCmdMgr	= TObjRef<NetCmdMgr>();
	m_pSession	= TObjRef<NetSession>();
	m_pFrameMgr	= TObjRef<GameFrameMgr>();

	m_pCmdMgr->Register( "NS_PickItemStrategy",		(NETMSGPROC)m_Trunk.sfp2(&PickupMgr::OnNS_PickItemStrategy),		_T("NS_PickItemStrategy"));

}
VOID PickupMgr::Destroy()
{
	m_pCmdMgr->UnRegister( "NS_PickItemStrategy",							(NETMSGPROC)m_Trunk.sfp2(&PickupMgr::OnNS_PickItemStrategy) );
}


DWORD PickupMgr::OnNS_PickItemStrategy(tagNS_PickItemStrategy* pNetCmd,DWORD)
{
	m_PickStrategy = (tagPickStrategy&)pNetCmd->dwStrategy;
	return 0;
}

VOID PickupMgr::SavePickStrategy()
{
	tagNC_PickItemStrategy cmd;
	memcpy( &cmd.dwStrategy, &m_PickStrategy, sizeof(DWORD) );
	m_pSession->Send(&cmd);
}	

VOID PickupMgr::OpenUI()
{
	PickStrategyFrame*pFrame = (PickStrategyFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("PickStrategy") );
	if( !P_VALID(pFrame) )
		pFrame = (PickStrategyFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("PickStrategy"), _T("PickStrategyFrame"), 0 );
}
VOID PickupMgr::CloseUI()
{
	PickStrategyFrame*pFrame = (PickStrategyFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("PickStrategy") );
	if( P_VALID(pFrame) )
		pFrame->close();
}

bool PickupMgr::CanPick(DWORD itemID,int bQuality)
{
	if (TYPE_ID_MONEY == itemID)
	{
		if (m_PickStrategy.bMoney)
			return true;
		else
			return false;
	}
	 const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(itemID);
	 if(!P_VALID(pItemProto))	return false;

	 bool canPick = false;
	switch (pItemProto->ePickType)
	{
	case EPT_Money:
		canPick = m_PickStrategy.bMoney;
		break;
	case EPT_Equip:
		canPick = m_PickStrategy.bEquip;
		break;
	case EPT_Medicine:
		canPick = m_PickStrategy.bMedicine;
		break;
	case EPT_StrengthenStuff:
		canPick = m_PickStrategy.bStrengthen;
		break;
	case EPT_Gem:
		canPick = m_PickStrategy.bGem;
		break;
	case EPT_Quest:
		canPick = m_PickStrategy.bQuest;
		break;
	case EPT_Sprite:
		canPick = m_PickStrategy.bSprite;
		break;
	case EPT_Others:
		canPick = m_PickStrategy.bOthers;
		break;
	}

	// 类型不匹配
	if(!canPick)	return false;

	// 如果是武器(武器的品级要特殊处理)
	 if(MIsEquipment(itemID))
	 {
		 canPick = false;
		switch ((EItemQuality)bQuality)
		{
		case EIQ_White:
			canPick = m_PickStrategy.bWrite;
			break;
		case EIQ_Yellow:
			canPick = m_PickStrategy.bYellow;
			break;
		case EIQ_Green:
			canPick = m_PickStrategy.bGreen;
			break;
		case EIQ_Blue:
			canPick = m_PickStrategy.bBlue;
			break;
		case EIQ_Orange:
			canPick = m_PickStrategy.bOrange;
			break;
		case EIQ_Purple:
			canPick = m_PickStrategy.bPurple;
			break;
		case EIQ_Pink:
			canPick = m_PickStrategy.bPink;
			break;
		}

		// 品级不匹配
		if(!canPick)	return false;
	 }

	return true;
}