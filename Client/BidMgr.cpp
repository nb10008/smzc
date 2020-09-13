#include "StdAfx.h"
#include "BidMgr.h"
#include "GodBidFrame.h"
#include "BlackMarketFrame.h"
#include "..\WorldDefine\creature_define.h"
#include "CreatureData.h"
#include "QuestEvent.h"
#include "..\WorldDefine\msg_GodbidDefine.h"

BidMgr BidMgr::s_guiBidMgr;
BidMgr* BidMgr::Inst()
{
	return &s_guiBidMgr;
}


BidMgr::BidMgr(void):m_Trunk(this)
{
}

BidMgr::~BidMgr(void)
{
}

VOID BidMgr::Init()
{
	m_pCmdMgr	= TObjRef<NetCmdMgr>();
	m_pSession	= TObjRef<NetSession>();
	m_pFrameMgr	= TObjRef<GameFrameMgr>();

	m_pFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&BidMgr::OnStartNPCTalkEvent));
	m_bidData.LoadFormFile();
}
VOID BidMgr::Destroy()
{
	m_pFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&BidMgr::OnStartNPCTalkEvent));
}
VOID BidMgr::Update()
{

}
DWORD BidMgr::OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
	if( pCreatureProto->eFunctionType == EFNPCT_GodBidNPC )
	{
		GodBidFrame *pFrame = (GodBidFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("GodBid") );
		if( !P_VALID(pFrame) )
			pFrame = (GodBidFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("GodBid"), _T("GodBidFrame"), 0 );
		if( P_VALID(pFrame) )
		{
			pFrame->SetNpcID(pEvt->globalID);
			tagNC_GodBiderShopOpen cmd;
			cmd.dwNPCID = pEvt->globalID;
			m_pSession->Send(&cmd);
		}
	} 
	else if ( pCreatureProto->eFunctionType == EFNPCT_BlackMarketNPC)
	{
		BlackMarketFrame *pFrame = (BlackMarketFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("BlackMarket") );
		if( !P_VALID(pFrame) )
			pFrame = (BlackMarketFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("BlackMarket"), _T("BlackMarketFrame"), 0 );
		if( P_VALID(pFrame) )
		{
			pFrame->SetNpcID(pEvt->globalID);
			tagNC_BlackMarketShopOpen cmd;
			cmd.dwNPCID = pEvt->globalID;
			m_pSession->Send(&cmd);
		}
	}
	return 0;
}

const tagBidDataLocal* BidMgr::FindData(const DWORD& id) const
{
	return m_bidData.GetBidData(id);
}