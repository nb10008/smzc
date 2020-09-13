#include "StdAfx.h"
#include "SpriteMgr.h"
#include "SpriteBornFrame.h"
#include "CreatureData.h"
#include "QuestEvent.h"
#include "SpritePartnershipFrame.h"
#include "Role.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"

const DWORD MAX_WAIT_FEEDBACK_TIME = 30*1000;

SpriteMgr::SpriteMgr(void):m_Trunk(this)
,m_dwApplyPlayerID(0)
{
}

SpriteMgr::~SpriteMgr(void)
{
}
SpriteMgr SpriteMgr::s_guiSpriteMgr;
SpriteMgr* SpriteMgr::Inst()
{
	return &s_guiSpriteMgr;
}

VOID SpriteMgr::Init()
{
	m_pCmdMgr	= TObjRef<NetCmdMgr>();
	m_pSession	= TObjRef<NetSession>();
	m_pFrameMgr	= TObjRef<GameFrameMgr>();
	m_pFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteMgr::OnStartNPCTalkEvent));
	m_pFrameMgr->RegisterEventHandle(_T("Spriteevent_IsProductSpriteMsgBox"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteMgr::OnSpriteevent_IsProductSpriteMsgBox)); 

	m_pCmdMgr->Register( "NS_TransmitReplyFairyBirth",		(NETMSGPROC)m_Trunk.sfp2(&SpriteMgr::OnNS_TransmitReplyFairyBirth),		_T("NS_TransmitReplyFairyBirth"));
	m_pCmdMgr->Register( "NS_TransmitApplyFairyBirth",		(NETMSGPROC)m_Trunk.sfp2(&SpriteMgr::OnNS_TransmitApplyFairyBirth),		_T("NS_TransmitApplyFairyBirth"));

	m_bNeedCheckFeedBack = false;
	m_dwFeedBackTime = 0;
}
VOID SpriteMgr::Destroy()
{
	m_pFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteMgr::OnStartNPCTalkEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("Spriteevent_IsProductSpriteMsgBox"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteMgr::OnSpriteevent_IsProductSpriteMsgBox));

	m_pCmdMgr->UnRegister( "NS_TransmitReplyFairyBirth",							(NETMSGPROC)m_Trunk.sfp2(&SpriteMgr::OnNS_TransmitReplyFairyBirth) );
	m_pCmdMgr->UnRegister( "NS_TransmitApplyFairyBirth",							(NETMSGPROC)m_Trunk.sfp2(&SpriteMgr::OnNS_TransmitApplyFairyBirth) );

	if( P_VALID(GetObj("IsProductSpriteMsgBox")) )
		KillObj("IsProductSpriteMsgBox");
}

VOID SpriteMgr::Update()
{
	if (m_bNeedCheckFeedBack)
	{
		m_dwFeedBackTime += Kernel::Inst()->GetDeltaTimeDW();
		if (m_dwFeedBackTime > MAX_WAIT_FEEDBACK_TIME)
		{
			tagNC_ReplyFairyBirth cmd;
			cmd.dwApplicantID = m_dwApplyPlayerID;
			cmd.bAgreeOrNot = false;
			m_pSession->Send(&cmd);
			m_dwApplyPlayerID = 0;

			m_bNeedCheckFeedBack = false;
			m_dwFeedBackTime =0;

			if( P_VALID(GetObj("IsProductSpriteMsgBox")) )
				KillObj("IsProductSpriteMsgBox");
		}
	}
}
VOID SpriteMgr::OpenSpriteBornFrame(DWORD dwNPCID)
{
	SpriteBornFrame *pFrame = (SpriteBornFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("SpriteBorn") );
	if( !P_VALID(pFrame) )
		pFrame = (SpriteBornFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("SpriteBorn"), _T("SpriteBornFrame"), 0 );
	if( P_VALID(pFrame) )
	{
		pFrame->SetNpcID(dwNPCID);
	}
}

DWORD SpriteMgr::OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
	if( pCreatureProto->eFunctionType == EFNPCT_SpriteBornNPC )
	{
		OpenSpriteBornFrame(pEvt->globalID);
	} 
	return 0;
}

VOID SpriteMgr::OpenSpritePartnerShipFrame(DWORD dwNPCID)
{
	SpritePartnershipFrame *pFrame = (SpritePartnershipFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("SpritePartnership") );
	if( !P_VALID(pFrame) )
		pFrame = (SpritePartnershipFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("SpritePartnership"), _T("SpritePartnershipFrame"), 0 );

	if( P_VALID(pFrame) )
	{
		pFrame->SetNpcID(dwNPCID);
	}
}
DWORD SpriteMgr::OnNS_TransmitApplyFairyBirth(tagNS_TransmitApplyFairyBirth* pNetCmd,DWORD)
{
	Role* pRole = RoleMgr::Inst()->FindRole(pNetCmd->dwApplicantID);
	if (P_VALID(pRole))
	{
		TCHAR msg[256] = {0};
		_stprintf( msg, g_StrTable[_T("Sprite_IsProductSprite")], pRole->GetRoleName().c_str() );

		if( P_VALID(GetObj("IsProductSpriteMsgBox")) )
			KillObj("IsProductSpriteMsgBox");

		CreateObj("IsProductSpriteMsgBox", "MsgBox");
		TObjRef<MsgBox>("IsProductSpriteMsgBox")->Init( _T(""), msg,
			_T("Spriteevent_IsProductSpriteMsgBox"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);

		m_dwApplyPlayerID = pNetCmd->dwApplicantID;

		m_bNeedCheckFeedBack = true;
	}

	return 0;
}

DWORD SpriteMgr::OnNS_TransmitReplyFairyBirth(tagNS_TransmitReplyFairyBirth* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == TRUE/*E_Success*/)
	{
		OpenSpritePartnerShipFrame(pNetCmd->dwInvitee);
	}
	else if ( FALSE == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpritePartnership_refuse")]);
	}
	else if ( E_FabaoMating_DistanceTooFar == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_DistanceTooFar")]);
	}
	else if ( E_FabaoMating_NotOnline == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_NotOnline")]);
	}
	return 0;
}

DWORD SpriteMgr::OnSpriteevent_IsProductSpriteMsgBox( tagMsgBoxEvent* pEvent )
{
	if( MBF_OK == pEvent->eResult )
	{
		SpritePartnershipFrame *pFrame = (SpritePartnershipFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("SpritePartnership") );
		if( P_VALID(pFrame) )
		{
			tagNC_ReplyFairyBirth cmd;
			cmd.dwApplicantID = m_dwApplyPlayerID;
			cmd.bAgreeOrNot = false;
			m_pSession->Send(&cmd);
			m_dwApplyPlayerID = 0;

			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_OnceOnePlayer")]);

			return 0;
		}
		tagNC_ReplyFairyBirth cmd;
		cmd.dwApplicantID = m_dwApplyPlayerID;
		cmd.bAgreeOrNot = true;
		m_pSession->Send(&cmd);

		OpenSpritePartnerShipFrame(m_dwApplyPlayerID);

		m_bNeedCheckFeedBack = false;
		m_dwFeedBackTime = 0;
	}
	else if (MBF_Cancel == pEvent->eResult)
	{
		tagNC_ReplyFairyBirth cmd;
		cmd.dwApplicantID = m_dwApplyPlayerID;
		cmd.bAgreeOrNot = false;
		m_pSession->Send(&cmd);

		m_dwApplyPlayerID = 0;

		m_bNeedCheckFeedBack = false;
		m_dwFeedBackTime = 0;
	}
	return 0;
}