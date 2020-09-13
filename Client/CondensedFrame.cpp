#include "stdafx.h"
#include "CondensedFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\msg_combat.h"
#include "CondensedProtoData.h"
#include "SkillMgr.h"

CondensedFrame::CondensedFrame()
:m_Trunk(this)
,m_bIsConsdensed(FALSE)
{

}
CondensedFrame::~CondensedFrame()
{

}

BOOL CondensedFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	//m_pFrameMgr->RegisterEventHandle(_T("OnOpenCondensed"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&CondensedFrame::OnOpenCondensedFrame));

	m_pNetCmdMgr->Register("NS_CondensedChange",	        (NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_CondensedChange),     _T("NS_CondensedChange"));
	m_pNetCmdMgr->Register("NS_StopCondensed",              (NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_StopCondensed),       _T("NS_StopCondensed"));
	m_pNetCmdMgr->Register("NS_BeginCondensed",				(NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_BeginCondensed),		 _T("NS_BeginCondensed"));
	m_pNetCmdMgr->Register("NS_Skill",						(NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_Skill),				 _T("NS_Skill"));
	return TRUE;
}

BOOL CondensedFrame::EscCancel()
{
	if(!m_pWnd->IsInvisible())
	{
		tagNC_StopCondensed cmd;
		cmd.nType = 2;					//关闭界面
		m_pNetSession->Send(&cmd);
		return TRUE;
	}
	return FALSE;
}

BOOL CondensedFrame::Destroy()
{
	GameFrame::Destroy();
	//m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Diamond"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&DiamondFrame::OnItemEvent_Move2Diamond));
	//m_pFrameMgr->UnRegisterEventHandler(_T("OnOpenCondensed"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&CondensedFrame::OnOpenCondensedFrame));

	m_pNetCmdMgr->UnRegister("NS_CondensedChange",	        (NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_CondensedChange));
	m_pNetCmdMgr->UnRegister("NS_StopCondensed",            (NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_StopCondensed));
	m_pNetCmdMgr->UnRegister("NS_BeginCondensed",			(NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_BeginCondensed));
	m_pNetCmdMgr->UnRegister("NS_Skill",					(NETMSGPROC)m_Trunk.sfp2(&CondensedFrame::OnNS_Skill));

	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
	}
	

	return TRUE;
}
BOOL CondensedFrame::ReloadUI()
{
	TCHAR szText[256] = {0};
	char temp[256] = {0};
	XmlElement ele;
	tstring strParth = g_strLocalPath + _T("\\ui\\god_exp.xml");
	m_pGUI->LoadXml(&ele,"VFS_System",strParth.c_str());

	m_pGUI->ChangeXml(&ele,"godexp_win\\back\\progpic0\\proqress","ClassName","ActiveProgress"); //change
	m_pGUI->ChangeXml(&ele,"godexp_win\\back\\progpic1\\proqress","ClassName","ActiveProgress");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"),&ele);
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(),m_Trunk.sfp1(&CondensedFrame::EventHandler));
	m_pWndCaption	= m_pWnd->GetChild(_T("back"));
	m_pStcGodHead	= (GUIStatic*)m_pWndCaption->GetChild(_T("level"));
    m_pStcBelief	= (GUIStatic*)m_pWndCaption->GetChild(_T("exp\\num"));					//信仰
    m_pProCondensed = (ActiveProgress*)m_pWndCaption->GetChild(_T("progpic0\\proqress"));	//凝聚度
	m_pStcCondensed = (GUIStatic*)m_pWndCaption->GetChild(_T("progpic0\\num"));	//凝聚度数值
    m_pProBar		= (ActiveProgress*)m_pWndCaption->GetChild(_T("progpic1\\proqress"));				//进度条
    m_pBtnBegin		= (GUIButton*)m_pWndCaption->GetChild(_T("startbutt"));					//开始凝聚
    m_pBtnClose		= (GUIButton*)m_pWndCaption->GetChild(_T("canclebutt"));				//关闭
    m_pBtnCloseWnd	= (GUIButton*)m_pWndCaption->GetChild(_T("closebutton"));				//关闭*

	for(int i = 0; i < MAX_GodHead; i++)
	{
		_stprintf(szText, _T("levelpic\\%d\\get"), i);
		m_pStcStaricon[i] = (GUIStatic*)m_pWndCaption->GetChild(szText);
	}

	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}
VOID CondensedFrame::Update()
{
	if(m_bIsConsdensed)
	{
		m_pProBar->Update();
		if(m_pProBar->GetCurrentValue() >= m_pProBar->GetMaxValue())
		{
			m_pProBar->SetValue(0, TRUE);
			m_pProBar->SetValue(5000);
		}
	}
	GameFrame::Update();
}

BOOL CondensedFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if(!P_VALID(pWnd))
		return FALSE;
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if(pWnd == m_pBtnBegin)
		{
			if(!m_bIsConsdensed)
			{
				tagNC_BeginCondensed cmd;	//开始凝聚
				m_pNetSession->Send(&cmd);
			}
			else
			{
				tagNC_StopCondensed cmd;
				cmd.nType = 1;				//停止凝聚
				m_pNetSession->Send(&cmd);
			}

		}
		else if(pWnd == m_pBtnClose || pWnd == m_pBtnCloseWnd)
		{
			tagNC_StopCondensed cmd;
			cmd.nType = 2;					//关闭界面
			m_pNetSession->Send(&cmd);
		}
		break;
	default:break;
	}
	return FALSE;
}

DWORD CondensedFrame::OnNS_CondensedChange(tagNS_CondensedChange* pCmd, DWORD)
{
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	TCHAR szText[X_LONG_NAME] = {0};
	_itot(pCmd->dwBelief, szText, 10);
	m_pStcBelief->SetText(szText);
	int nGodHead = pPlayer->GetAttribute(ERA_God_Godhead);
	_itot(nGodHead, szText, 10);
	m_pStcGodHead->SetText(szText);
	for(int i = 0; i < nGodHead; i++)
	{
		m_pStcStaricon[i]->SetInvisible(FALSE);
	}
	const tagCondensedProto* pCondensedProto = CondensedProtoData::Inst()->FindCondensedProto(pPlayer->GetAttribute(ERA_God_Godhead)+1);
	if(P_VALID(pCondensedProto))
	{
		_stprintf(szText, _T("%0.2f%%"), (FLOAT)pCmd->dwCondensedVal/(FLOAT)pCondensedProto->dwAgglomerate_val*100);
		m_pProCondensed->SetMaxValue(pCondensedProto->dwAgglomerate_val);
		m_pProCondensed->SetValue(pCmd->dwCondensedVal, TRUE);
		m_pStcCondensed->SetText(szText);
	}
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD CondensedFrame::OnNS_StopCondensed(tagNS_StopCondensed* pCmd, DWORD)
{
	if( E_UseSkill_BeInterrupted == pCmd->dwErrorCode)
	{
		//终止凝聚操作关闭界面
		m_pWnd->SetInvisible(TRUE);

		return 0;
	}
	if(E_Success == pCmd->dwErrorCode)
	{
		if(pCmd->nType == 1)
		{
			m_pBtnBegin->SetText(g_StrTable[_T("CondensedBegin")]);
			m_pProBar->SetSpeed(0);
			m_pProBar->SetValue(0,TRUE);
			m_bIsConsdensed = FALSE;
		}
		else if(pCmd->nType == 2)
		{
			m_pWnd->SetInvisible(TRUE);
			m_pProBar->SetSpeed(0);
			m_pProBar->SetValue(0,TRUE);
			m_bIsConsdensed = FALSE;
		}
		return 0;
	}
	return 0;
}

DWORD CondensedFrame::OnNS_BeginCondensed(tagNS_BeginCondensed* pCmd, DWORD)
{
	if(pCmd->dwErrorCode == E_Success)
	{
		m_bIsConsdensed = TRUE;
		m_pBtnBegin->SetText(g_StrTable[_T("CondensedStop")]);
		m_pProBar->SetSpeed(1000);
		m_pProBar->SetValue(5000);
		m_pProBar->SetMaxValue(5000);
	}
	else
	{

	}
	return 0;
}

VOID CondensedFrame::Show(bool bShow)
{
	m_pWnd->SetInvisible(!bShow);
	if(bShow)
	{
		Clear();

		TCHAR szText[X_LONG_NAME] = {0};
		LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();

		int nGodHead = pPlayer->GetAttribute(ERA_God_Godhead);
		_itot(nGodHead, szText, 10);
		m_pStcGodHead->SetText(szText);
		DWORD dwFaith = pPlayer->GetAttribute(ERA_God_Faith);
		_itot(dwFaith, szText, 10);
		m_pStcBelief->SetText(szText);
		DWORD dwCondensed = pPlayer->GetAttribute(ERA_God_Condenced);
		const tagCondensedProto* pCondensedProto = CondensedProtoData::Inst()->FindCondensedProto(nGodHead+1);
		if(P_VALID(pCondensedProto))
		{
			_stprintf(szText, _T("%0.2f%%"), (FLOAT)dwCondensed/(FLOAT)pCondensedProto->dwAgglomerate_val*100);
			m_pProCondensed->SetMaxValue(pCondensedProto->dwAgglomerate_val);
			m_pProCondensed->SetValue(dwCondensed, TRUE);
			m_pStcCondensed->SetText(szText);
		}
		for(int i = 0; i < nGodHead; i++)
		{
			m_pStcStaricon[i]->SetInvisible(FALSE);
		}
		m_pWnd->SetRefresh(TRUE);
	}

}

VOID CondensedFrame::Clear()
{
	for(int i = 0; i < MAX_GodHead; i++)
	{
		m_pStcStaricon[i]->SetInvisible(TRUE);
		m_bIsConsdensed = FALSE;
		m_pBtnBegin->SetText(g_StrTable[_T("CondensedBegin")]);
	}
}

DWORD CondensedFrame::OnOpenCondensedFrame(tagGameEvent* pEvent)
{
	if(m_pWnd->IsInvisible())
		Show(TRUE);

	return 0;
}

DWORD CondensedFrame::OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam)
{
	if(pMsg->dwErrorCode!=E_Success)
		return 0; 

	const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pMsg->dwSkillID/100);
	if(P_VALID(pData))
	{
		//凝聚技能
		if(ESSTE2_GodCondense == pData->pProto->nType3)
		{
			if(m_pWnd->IsInvisible())
				Show(TRUE);
		}
	}

	return 0;
}
