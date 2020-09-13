#include "StdAfx.h"
#include "GuildWarRewardFrame.h"
#include "IconStatic.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "GuildMgr.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "SkillProtoData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"

const TCHAR* szBackPic = _T("data\\ui\\Bag\\L-main-icon-lock.bmp");

GuildWarRewardFrame::GuildWarRewardFrame(void):m_Trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
{
}

GuildWarRewardFrame::~GuildWarRewardFrame(void)
{
}

BOOL GuildWarRewardFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	
	m_pCmdMgr->Register( "NS_GuildOccupyedNum",		(NETMSGPROC)m_Trunk.sfp2(&GuildWarRewardFrame::OnNS_GuildOccupyedNum),		_T("NS_GuildOccupyedNum"));
	m_pCmdMgr->Register( "NS_TakeGuildWarReword",		(NETMSGPROC)m_Trunk.sfp2(&GuildWarRewardFrame::OnNS_TakeGuildWarReword),		_T("NS_TakeGuildWarReword"));
	
	m_dwItemID = GT_INVALID;
	m_dwBuffID = GT_INVALID;
	m_byItemQuity = 0;
	m_pIsAlreadyTake->SetInvisible(true);
	return TRUE;
}
BOOL GuildWarRewardFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_GuildOccupyedNum",							(NETMSGPROC)m_Trunk.sfp2(&GuildWarRewardFrame::OnNS_GuildOccupyedNum) );
	m_pCmdMgr->UnRegister( "NS_TakeGuildWarReword",							(NETMSGPROC)m_Trunk.sfp2(&GuildWarRewardFrame::OnNS_TakeGuildWarReword) );

	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	return bRet;
}
BOOL GuildWarRewardFrame::ReloadUI()
{
 	XmlElement element;
 	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\battlefield_reward.xml");
 	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
 		return FALSE;
 
 	m_pGUI->ChangeXml( &element, "rewardwin\\rewardpic\\rewardback\\itemback\\pic_back\\pic", "ClassName", "IconStatic");
 	m_pGUI->ChangeXml( &element, "rewardwin\\rewardpic\\rewardback\\stateback\\pic_back\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "rewardwin\\rewardpic\\pic", "ClassName", "IconStatic");
	//m_pGUI->ChangeXml( &element, "rewardwin\\rewardpic\\surebutton", "ClassName", "GUIButton");

 	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
 	if( !P_VALID( m_pWnd ) )
 	{
 		m_pGUI->DestroyWnd(m_pWnd);
 		m_pWnd = NULL;		
 		return FALSE;
 	}

 	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&GuildWarRewardFrame::EventHandler));

	m_pIsItemReword		= (IconStatic*)m_pWnd->GetChild( _T("rewardpic\\rewardback\\itemback\\pic_back\\pic") );
	m_pIsBuffReword		= (IconStatic*)m_pWnd->GetChild( _T("rewardpic\\rewardback\\stateback\\pic_back\\pic") );
	m_pIsAlreadyTake		= (IconStatic*)m_pWnd->GetChild( _T("rewardpic\\pic") );

	m_pBnGetReword		= (GUIButton*)m_pWnd->GetChild( _T("rewardpic\\surebutton") );
	m_pBnClose		= (GUIButton*)m_pWnd->GetChild( _T("rewardpic\\closebutt") );

	m_pSeItemName		= (GUIStaticEx*)m_pWnd->GetChild( _T("rewardpic\\rewardback\\itemback\\itemname") );
	m_pSeBuffName		= (GUIStaticEx*)m_pWnd->GetChild( _T("rewardpic\\rewardback\\stateback\\statename") );

	m_pStGuildName		= (GUIStatic*)m_pWnd->GetChild( _T("rewardpic\\partyname") );
	m_pStGuildPos		= (GUIStatic*)m_pWnd->GetChild( _T("rewardpic\\playerpost") );
	m_pStOccupyNum		= (GUIStatic*)m_pWnd->GetChild( _T("rewardpic\\battlefieldstate\\num") );

	m_pIsItemReword->SetPic(szBackPic);
	m_pIsBuffReword->SetPic(szBackPic);

	m_pWnd->SetInvisible(false);

	return TRUE;
}

BOOL GuildWarRewardFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

VOID GuildWarRewardFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
		{
			m_pFrameMgr->AddToDestroyList(this);
		}
	}
}

BOOL GuildWarRewardFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBnClose )
		{
			m_pFrameMgr->AddToDestroyList(this);
		}
		else if (pWnd == m_pBnGetReword)
		{
			tagNC_TakeGuildWarReword cmd;
			cmd.dwNPCID = m_dwNPCID;
			m_pSession->Send(&cmd);
		}	
		break;
	case EGUIE_MoveIntoCtrl:
		if (pWnd == m_pIsItemReword)
		{
			if (P_VALID(m_dwItemID) && P_VALID(m_byItemQuity))
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwItemID ,(BYTE)m_byItemQuity).c_str() );
		}
		else if (pWnd == m_pIsBuffReword)
		{
			if (P_VALID(m_dwBuffID))
			{
				tagBuffProtoClient* pProto = const_cast<tagBuffProtoClient*>(SkillProtoData::Inst()->FindBuffProto(m_dwBuffID));
				if (P_VALID(pProto))
				{
					ShowTip( pWnd, pProto->szDesc );
				}
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

DWORD GuildWarRewardFrame::OnNS_GuildOccupyedNum(tagNS_GuildOccupyedNum *pNetCmd, DWORD)
{
	LocalPlayer * pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer))
		return 0;

	DWORD dwGuildID = pPlayer->GetRoleGuildID();
	// 如果ID无效则直接返回
	if(!GT_VALID(dwGuildID))
		m_pStGuildName->SetText(_T(""));

	// 公会名称
	tstring strName = GuildMgr::Inst()->GetGuildName(dwGuildID);
	if(strName.size() > 0)
	{
		m_pStGuildName->SetText(strName.c_str());
	}
	
	// 职位名称
	int pos = pPlayer->GetRoleGuildPos();
	tstring posName = GuildMgr::Inst()->GetMemberPos(pos);
	if (posName.size() > 0)
	{
		m_pStGuildPos->SetText(posName.c_str());
	}
	else
		m_pStGuildPos->SetText(_T(""));

	// 占领战场数量
	TCHAR szText[X_SHORT_NAME];
	_stprintf(szText,_T("%d"),pNetCmd->nNum);
	m_pStOccupyNum->SetText(szText);

	// 是否已经领取完了奖励
	if (pNetCmd->bAlreadyTake)
		m_pIsAlreadyTake->SetInvisible(false);

	// 刷新物品
	const GuildRewardData* pData = GuildMgr::Inst()->GetGuildRewardData(EGuildPos(pos),pNetCmd->nNum);
	if (P_VALID(pData))
	{
		m_dwItemID = pData->dwItem;
		m_dwBuffID = pData->dwBuff;

		// 刷新图片
		m_pIsItemReword->SetPic(ItemMgr::Inst()->GetItemIconPath(pData->dwItem).c_str());

		// 保存品级
		const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pData->dwItem);
		if (P_VALID(pItem))
		{
			m_byItemQuity = pItem->byQuality;
		}
		// 刷新名称
		const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(pData->dwItem);
		if( P_VALID(itemProto) )
		{
			m_pSeItemName->SetText(itemProto->szName);
		}

		// 刷新buff
		tagBuffProtoClient* pProto = const_cast<tagBuffProtoClient*>(SkillProtoData::Inst()->FindBuffProto(pData->dwBuff));
		if (P_VALID(pProto))
		{
			// 刷新图片
			tstring strIconFileName = _T("data\\ui\\Icon\\");
			strIconFileName += pProto->szIcon;
			strIconFileName += _T(".tga");
			m_pIsBuffReword->SetPic( strIconFileName );

			// 刷新文字
			m_pSeBuffName->SetText(pProto->szName);
		}
	}
	
	return 0;
}
DWORD GuildWarRewardFrame::OnNS_TakeGuildWarReword(tagNS_TakeGuildWarReword *pNetCmd, DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		m_pIsAlreadyTake->SetInvisible(false);
	}
	else
	{
		switch (pNetCmd->dwErrorCode)
		{
		case EGEC_NPC_NotExist:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_NPC_NotExist")]);
			break;
		case EGEC_NPC_DisFar:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_NPC_DisFar")]);
			break;
		case EGEC_NPC_TypeMismatch:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_NPC_TypeMismatch")]);
			break;
		case EGEC_DailyReward_LevelLimit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_DailyReward_LevelLimit")]);
			break;
		case EGEC_DailyReward_GuildPosLimit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_DailyReward_GuildPosLimit")]);
			break;
		case EGEC_DailyReward_AlreadyTaken:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_DailyReward_AlreadyTaken")]);
			break;
		case EGEC_DailyReward_CannotFindProto:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_DailyReward_CannotFindProto")]);
			break;
		case EGEC_DailyReward_BagFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EGEC_DailyReward_BagFull")]);
			break;
		}
	}
	return 0;
}