#include "StdAfx.h"
#include "FamilyFrame.h"
#include "FamilySpriteFrame.h"
#include "FamilyQuestFrame.h"
#include "..\WorldDefine\msg_family.h"
#include "FamilyMemberFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "FamilyEvent.h"
#include "FamilyMgr.h"

FamilyFrame::FamilyFrame(void):
m_trunk(this)
,m_pSpriteFrame(NULL)
,m_pWndMember(NULL)
,m_pFamilyQuestFrame(NULL)
{
}

FamilyFrame::~FamilyFrame(void)
{
}

BOOL FamilyFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pFrameMgr->RegisterEventHandle(_T("Open_FamilyFrame"),	 (FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventFamilyOpen));
	m_pFrameMgr->RegisterEventHandle(_T("Close_FamilyFrame"),	 (FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventFamilyClose));
	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_GetName"),	 (FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventGetName));


	return TRUE;
}

BOOL FamilyFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	m_pFrameMgr->UnRegisterEventHandler(_T("Open_FamilyFrame"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventFamilyOpen));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_FamilyFrame"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventFamilyClose));
	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_GetName"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyFrame::OnEventGetName));

	m_pWndMember = NULL;
	m_pFamilyQuestFrame = NULL;
	m_pSpriteFrame = NULL;

	return bRet;
}

BOOL FamilyFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\family.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	// chang your xml here
	//m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\role", "ClassName", "PlayerViewWindow");
	
	//家族成员
	for (int i=0; i<MAX_FAMILY_MEMBER; ++i)
	{
		stringstream ssClass;//职业
		if ( i != 0 )
			ssClass<<"family\\backpic\\memberback\\player"<<i-1<<"\\jobback\\jobpic";
		else//家长
			ssClass<<"family\\backpic\\memberback\\leader\\jobback\\jobpic";
		m_pGUI->ChangeXml(&ele, ssClass.str().c_str(), "ClassName", "IconStatic");

		stringstream ssSex;//性别
		if ( i != 0 )
			ssSex<<"family\\backpic\\memberback\\player"<<i-1<<"\\sex";
		else//家长
			ssSex<<"family\\backpic\\memberback\\leader\\sex";
		m_pGUI->ChangeXml(&ele, ssSex.str().c_str(), "ClassName", "IconStatic");
	}


	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	
	m_pWndCaption	= m_pWnd->GetChild(_T("backpic"));
	m_pPbnState		= (GUIPushButton*)m_pWnd->GetChild(_T("backpic\\choose\\familymember"));		
	m_pPbnSprite	= (GUIPushButton*)m_pWnd->GetChild(_T("backpic\\choose\\familyfairy"));		
	m_pPbnQuest		= (GUIPushButton*)m_pWnd->GetChild(_T("backpic\\choose\\familytask"));		

	m_pClose		= (GUIButton*)m_pWnd->GetChild(_T("backpic\\closebutt"));	
	m_pStcFamilyName= (GUIStatic*)m_pWnd->GetChild(_T("backpic\\family\\familyname"));
	m_pStcFamilyName->SetText(_T(""));
	m_pStcLeaderName= (GUIStatic*)m_pWnd->GetChild(_T("backpic\\family\\familymaster\\name"));
	m_pStcLeaderName->SetText(_T(""));
	m_pStcMenbersNum= (GUIStatic*)m_pWnd->GetChild(_T("backpic\\family\\familynum\\num"));
	m_pStcMenbersNum->SetText(_T(""));
	m_pStcTodayActiv= (GUIStatic*)m_pWnd->GetChild(_T("backpic\\family\\familyvitality\\num"));
	m_pStcTodayActiv->SetText(_T(""));

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&FamilyFrame::EventHandler));

	m_pSpriteFrame = (FamilySpriteFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("FamilySprite"), _T("FamilySpriteFrame"), 0);
	m_pFamilyQuestFrame = (FamilyQuestFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("FamilyQuest"), _T("FamilyQuestFrame"), 0);
	
	
	m_pWndMember		= (FamilyMemberFrame*) m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("FamilyMemberFrame"), _T("FamilyMemberFrame"),0);
	if (P_VALID(m_pWndMember))
		m_pWndMember->ShowWnd(TRUE);

	if (P_VALID(m_pSpriteFrame))
		m_pSpriteFrame->ShowFrame(FALSE);

	m_pPbnState->SetState( EGUIBS_PushDown, FALSE );


	return TRUE;
}

BOOL FamilyFrame::EscCancel()
{
	if (!m_pWnd->IsInvisible())
	{
		ShowWnd(FALSE);
		return TRUE;
	}

	return FALSE;
}

VOID FamilyFrame::Update()
{

}

DWORD FamilyFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pClose)
			{
				ShowWnd(FALSE);
			}

		}
		break;
	case EGUIE_PushButtonDown:
		{
			if ( pEvent->dwParam1 != 1 )	/* 不是按下不处理 */
				return 0;
			if(pWnd == m_pPbnQuest)
			{
				tagNC_OpenFamilyQuestPage cmd;
				m_pSession->Send(&cmd);
				m_pFamilyQuestFrame->ShowFrame(TRUE);
				m_pWndMember->ShowWnd(FALSE);
				m_pSpriteFrame->ShowFrame(FALSE);
			}
			else if (pWnd == m_pPbnState)
			{
				m_pWndMember->ShowWnd(TRUE);
				m_pFamilyQuestFrame->ShowFrame(FALSE);
				m_pSpriteFrame->ShowFrame(FALSE);
			}
			else if (pWnd == m_pPbnSprite)
			{
				tagNC_GetFamilySpriteInfo cmd3;
				m_pSession->Send(&cmd3);

				m_pSpriteFrame->ShowFrame(TRUE);
				m_pWndMember->ShowWnd(FALSE);
				m_pFamilyQuestFrame->ShowFrame(FALSE);
			}
		}
		break;
	case EGUIE_Drag:
		{

		}
		break;
	}
	return 0;
}

VOID FamilyFrame::ShowWnd(BOOL bShow/*=TRUE*/)
{
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( !P_VALID(m_pWnd) || !P_VALID(pLp)  )	
		return;

	if (bShow && pLp->IsInFamily())
	{
		m_pPbnState->SetState(EGUIBS_PushDown);  //打开就显示

		//家族名
		DWORD dwFamilyID = pLp->GetRoleFamilyID();
		tstring strFamilyName	= FamilyMgr::Inst()->GetFamilyName(dwFamilyID);
		if (!strFamilyName.empty())
			m_pStcFamilyName->SetText(strFamilyName.c_str());
		//族长名
		DWORD dwLeaderID = FamilyMgr::Inst()->GetFamilyLeaderID();
		if (dwLeaderID!=GT_INVALID)
		{
			tstring strLeaderName = PlayerNameTab::Inst()->FindNameByID(dwLeaderID);
			if (!strLeaderName.empty())
				m_pStcLeaderName->SetText(strLeaderName.c_str());
		}
		//成员数
		INT nMemberNum = FamilyMgr::Inst()->GetFamilyMemberNum();
		tstringstream tss;
		tss<<nMemberNum<<_T("/")<<MAX_FAMILY_MEMBER;
		m_pStcMenbersNum->SetText(tss.str().c_str());
		//活跃度
		INT nActive = pLp->GetLPActiveDegree();
		tstringstream tssActive;
		tssActive<<nActive;
		m_pStcTodayActiv->SetText(tssActive.str().c_str());
	}

	if (m_pWndMember->IsShowing())
		m_pWndMember->ShowWnd(bShow);

	tagFamilyFrameEvent evt( _T("FamilyEvent_Frame"), this );
	evt.bShowFrame		= bShow;
	m_pMgr->SendEvent( &evt );

	m_pWnd->SetInvisible(!bShow);
	m_pWnd->SetRefresh(bShow);

}

VOID FamilyFrame::RefreshUI()
{
	if (!P_VALID(m_pWnd)||m_pWnd->IsInvisible())
		return ;

	if (P_VALID(m_pWndMember))
	{
		m_pWndMember->RefreshUI();
	}

}

BOOL FamilyFrame::IsShowing()
{
	BOOL bHide = TRUE;
	if (P_VALID(m_pWnd))
		bHide = m_pWnd->IsInvisible();

	return (!bHide);
}

DWORD FamilyFrame::OnEventFamilyOpen( tagGameEvent* pGameEvent )
{
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;

	if (pLp->IsInFamily())
	{
		ShowWnd(TRUE);
	}

	return 0;
}

DWORD FamilyFrame::OnEventFamilyClose( tagGameEvent* pGameEvent )
{
	ShowWnd(FALSE);	
	return 0;
}

DWORD FamilyFrame::OnEventGetName( tagFamilyNameEvent* pGameEvent )
{
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;
	if ( pLp->IsInFamily() && P_VALID(m_pWnd) && (!m_pWnd->IsInvisible()) )
	{
		//家族名
		DWORD dwFamilyID = pLp->GetRoleFamilyID();
		tstring strFamilyName	= FamilyMgr::Inst()->GetFamilyName(dwFamilyID);
		if (!strFamilyName.empty())
			m_pStcFamilyName->SetText(strFamilyName.c_str());
		//族长名
		DWORD dwLeaderID = FamilyMgr::Inst()->GetFamilyLeaderID();
		if (dwLeaderID!=GT_INVALID)
		{
			tstring strLeaderName = PlayerNameTab::Inst()->FindNameByID(dwLeaderID);
			if (!strLeaderName.empty())
				m_pStcLeaderName->SetText(strLeaderName.c_str());
		}
		//成员数
		INT nMemberNum = FamilyMgr::Inst()->GetFamilyMemberNum();
		tstringstream tss;
		tss<<nMemberNum<<_T("/")<<MAX_FAMILY_MEMBER;
		m_pStcMenbersNum->SetText(tss.str().c_str());
		//活跃度
		INT nActive = pLp->GetLPActiveDegree();
		tstringstream tssActive;
		tssActive<<nActive;
		m_pStcTodayActiv->SetText(tssActive.str().c_str());
		m_pWnd->SetRefresh(TRUE);
	}

	return 0;
}
