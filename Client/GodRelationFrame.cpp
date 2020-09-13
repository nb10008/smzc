#include "stdafx.h"
#include "GodRelationFrame.h"
#include "..\WorldDefine\msg_god.h"
#include "..\WorldDefine\msg_common_errorcode.h"
#include "RoleMgr.h"
#include "QuestMgr.h"
#include "ToolTipFrame.h"
#include "ClergyProtoData.h"
#include "..\WorldDefine\clergy_define.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"

GodRelationFrame::GodRelationFrame()
:m_Trunk(this)
,m_escapeTime(0)
{

}
GodRelationFrame::~GodRelationFrame()
{

}

BOOL GodRelationFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pFrameMgr->RegisterEventHandle(_T("OpenGodRelation"),        (FRAMEEVENTPROC)m_Trunk.sfp1( &GodRelationFrame::OnOpenGodRelation));

	m_pNetCmdMgr->Register("NS_GetGodRelation",	       (NETMSGPROC)m_Trunk.sfp2(&GodRelationFrame::OnNS_GetGodRelation),         _T("NS_GetGodRelation"));
	return TRUE;
}

BOOL GodRelationFrame::EscCancel()
{
	if(!m_pWnd->IsInvisible())
	{
		Hide();
		return TRUE;
	}

	return FALSE;
}

BOOL GodRelationFrame::Destroy()
{
	GameFrame::Destroy();
	m_pFrameMgr->UnRegisterEventHandler(_T("OpenGodRelation"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&GodRelationFrame::OnOpenGodRelation));

	m_pNetCmdMgr->UnRegister("NS_GetGodRelation",	        (NETMSGPROC)m_Trunk.sfp2(&GodRelationFrame::OnNS_GetGodRelation));

	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
	}

	return TRUE;
}
BOOL GodRelationFrame::ReloadUI()
{
	TCHAR szText[256] = {0};
	char temp[256] = {0};
	XmlElement ele;
	tstring strParth = g_strLocalPath + _T("\\ui\\god_tree.xml");
	m_pGUI->LoadXml(&ele,"VFS_System",strParth.c_str());

	//m_pGUI->ChangeXml(&ele,"diamond_win\\diamond_pic\\back0\\imgback1\\img","ClassName","ComposeStatic"); //change

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"),&ele);
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(),m_Trunk.sfp1(&GodRelationFrame::EventHandler));
	m_pWndCaption = m_pWnd->GetChild(_T("tree_back"));
	m_pBtnClose   = (GUIButton*)m_pWndCaption->GetChild(_T("closebutton"));
	//加载那一堆名字控件
	for(int i = 0; i < ECGT_Num; i++)
	{
		for(int j = 0; j < MAX_GOD_NUM; j++)
		{
			_stprintf(szText, _T("name%d"), i*MAX_GOD_NUM+j);
			m_pStcMap[i][j] = (GUIStaticEx*)m_pWndCaption->GetChild(szText);
		}
	}

	//加载显示tips的那26个控件
	for(int i = 0; i < MAX_GOD_Relation_NUM; i++)
	{
		_stprintf(szText, _T("%d"), i+1);
		m_pStcGodTips[i] = (GUIStatic*)m_pWndCaption->GetChild(szText);
	}

	m_pWnd->SetInvisible(TRUE);
	
	return TRUE;
}
VOID GodRelationFrame::Update()
{
	if (m_pWnd->IsInvisible())
		return;

	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			Hide();
		}
	}

	GameFrame::Update();
}

BOOL GodRelationFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if(!P_VALID(pWnd))
		return FALSE;
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if(pWnd == m_pBtnClose)
		{
			Hide();
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for(int i = 0; i < MAX_GOD_Relation_NUM; i++)
			{
				if(pWnd == m_pStcGodTips[i])
				{
					ShowClergyTips(pWnd, i+1);
					break;
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, _T(""));
		}
		break;
	default:break;
	}
	return FALSE;
}

VOID GodRelationFrame::Show()
{
	m_pWnd->SetInvisible(FALSE);
	m_pWnd->FlipToTop();
}

VOID GodRelationFrame::Hide()
{
	m_pWnd->SetInvisible(TRUE);
}

DWORD GodRelationFrame::OnNS_GetGodRelation(tagNS_GetGodRelation* pCmd, DWORD)
{
	int nNum = 2;
	TCHAR szText[X_LONG_NAME] = {0};
	for(int i = 0; i < pCmd->nNum; i++)
	{
		if(pCmd->sResult.sPlayerInfo[i].eClergy == ECT_MenChamp)  //男第一
		{
			_stprintf(szText, _T("<font=%s,0,16,400>%s"),g_StrTable[_T("Font_MSYH")],pCmd->sResult.sPlayerInfo[i].szName);
			m_pStcMap[pCmd->eType][0]->SetTextColor(dwNameColor);
			m_pStcMap[pCmd->eType][0]->SetShadeColor(dwNameShade);
			m_pStcMap[pCmd->eType][0]->SetText(szText);
			m_pStcMap[pCmd->eType][0]->SetRefresh(TRUE);
			continue;
		}
		if(pCmd->sResult.sPlayerInfo[i].eClergy == ECT_WomenChamp) //女第一
		{
			_stprintf(szText, _T("<font=%s,0,16,400>%s"),g_StrTable[_T("Font_MSYH")],pCmd->sResult.sPlayerInfo[i].szName);
			m_pStcMap[pCmd->eType][1]->SetTextColor(dwNameColor);
			m_pStcMap[pCmd->eType][1]->SetShadeColor(dwNameShade);
			m_pStcMap[pCmd->eType][1]->SetText(szText);
			m_pStcMap[pCmd->eType][1]->SetRefresh(TRUE);
			continue;
		}
		//剩下的那些
		_stprintf(szText, _T("<font=%s,0,16,400>%s"),g_StrTable[_T("Font_MSYH")],pCmd->sResult.sPlayerInfo[i].szName);
		m_pStcMap[pCmd->eType][nNum]->SetTextColor(dwNameColor);
		m_pStcMap[pCmd->eType][nNum]->SetShadeColor(dwNameShade);
		m_pStcMap[pCmd->eType][nNum]->SetText(szText);
		m_pStcMap[pCmd->eType][nNum++]->SetRefresh(TRUE);
	}
	m_pWnd->SetRefresh(TRUE);

	return 0;
}

DWORD GodRelationFrame::OnOpenGodRelation(tagGameEvent* pEvent)
{
	ClearName();

	tagNC_GetGodRelation cmd;
	m_pNetSession->Send(&cmd);

	Show();

	return 0;
}

VOID GodRelationFrame::ClearName()
{
	for(int i = 0; i < ECGT_Num; i++)
	{
		for(int j = 0; j < MAX_GOD_NUM; j++)
		{
			m_pStcMap[i][j]->SetTextColor(dwDefaultColor);
			m_pStcMap[i][j]->SetShadeColor(dwDefaultShade);
			m_pStcMap[i][j]->SetText(g_StrTable[_T("Vacancy")]);
			m_pStcMap[i][j]->SetRefresh(TRUE);
		}
	}
}

VOID GodRelationFrame::ShowClergyTips(GUIWnd* pWnd, int nClergyID)
{
	const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(nClergyID);
	if(!P_VALID(pClergyName))
		return;
	TCHAR szText[X_HUGE_NAME] = {0};
	TCHAR szTemp[X_LONG_NAME] = {0};
	//名字
	_stprintf(szText, _T("<color=0xff00ff00>%s"), pClergyName->szName);
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//描述
	_stprintf(szText,_T("%s\\n%s"), szText, pClergyName->szClergyText);
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//神位数量
	if(pClergyName->nNum != 0 && pClergyName->nNum != GT_INVALID)
		_stprintf(szText,_T("%s\\n%s%d"), szText, g_StrTable[_T("GodRelationTips1")], pClergyName->nNum);
	else
		_stprintf(szText,_T("%s\\n%s%s"), szText, g_StrTable[_T("GodRelationTips1")], g_StrTable[_T("NullLimit")]);
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//对应职业
	DWORD dwClassVal;
	BOOL bIsRedColor = FALSE;
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	GetClassEnumBinary(pLocalPlayer->GetPlayerClass(), dwClassVal);
	if(!(dwClassVal | pClergyName->dwClassLimit))
	{
		bIsRedColor = TRUE;
	}
	switch(pClergyName->dwClassLimit)
	{
	case 8191:	//无
		dwClassVal = 0;
		break;
	case 98:	//神兵
		dwClassVal = 2;
		break;
	case 388:	//玄翎
		dwClassVal = 3;
		break;
	case 1544:	//乾坤
		dwClassVal = 4;
		break;
	case 6160:	//仙音
		dwClassVal = 5;
		break;
	default:dwClassVal = 0;
	}
	_stprintf(szTemp, _T("ClassName_%d"), dwClassVal);
	_stprintf(szText, _T("%s\\n%s"), szText, g_StrTable[_T("GodRelationTips2")]);
	if(bIsRedColor)
	{
		_stprintf(szText, _T("%s<color=0xffff0000>"), szText);
	}
	else
	{
		_stprintf(szText, _T("%s<color=0xff00ff00>"), szText);
	}
	if(dwClassVal == 0)
	{
		_stprintf(szText, _T("%s%s"), szText, g_StrTable[_T("NullLimit")]);
	}
	else
	{
		_stprintf(szText, _T("%s%s"), szText, g_StrTable[szTemp]);
	}
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//神格需求
	if(pLocalPlayer->GetAttribute(ERA_God_Godhead) >= pClergyName->nGodheadLimit)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%d"), szText, g_StrTable[_T("GodRelationTips3")], pClergyName->nGodheadLimit);
	}
	else
	{
		_stprintf(szText, _T("%s\\n%s<color=0xffff0000>%d"), szText, g_StrTable[_T("GodRelationTips3")], pClergyName->nGodheadLimit);
	}
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//等级要求
	if(pLocalPlayer->GetRoleLevel() >= pClergyName->nLevelLimit)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%d"), szText, g_StrTable[_T("GodRelationTips4")], pClergyName->nLevelLimit);
	}
	else
	{
		_stprintf(szText, _T("%s\\n%s<color=0xffff0000>%d"), szText, g_StrTable[_T("GodRelationTips4")], pClergyName->nLevelLimit);
	}
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//性别限制
	_stprintf(szTemp, _T("Sex%d"), pClergyName->eSexLimit);
	if(pClergyName->eSexLimit == ES_Null)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%s"), szText, g_StrTable[_T("GodRelationTips5")], g_StrTable[_T("NullLimit")]);
	}
	else if(pLocalPlayer->GetPlayerSex() == pClergyName->eSexLimit)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%s"), szText, g_StrTable[_T("GodRelationTips5")], g_StrTable[szTemp]);
	}
	else 
	{
		_stprintf(szText, _T("%s\\n%s<color=0xffff0000>%s"), szText, g_StrTable[_T("GodRelationTips5")], g_StrTable[szTemp]);
	}
	_stprintf(szText, _T("%s<color=0xfffff7e0>"),szText);
	//晋升条件
	_stprintf(szTemp, _T("ERoleAttribute_%d"), pClergyName->eAttType);
	if(pClergyName->eAttType == ERA_Null)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%s"), szText, g_StrTable[_T("GodRelationTips6")], g_StrTable[_T("NullLimit")]);
	}
	else if(pLocalPlayer->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue)
	{
		_stprintf(szText, _T("%s\\n%s<color=0xff00ff00>%s%s%d"), szText, g_StrTable[_T("GodRelationTips6")], g_StrTable[szTemp], g_StrTable[_T("BigeOrQual")], pClergyName->dwAttValue);
	}
	else
	{
		_stprintf(szText, _T("%s\\n%s<color=0xffff0000>%s%s%d"), szText, g_StrTable[_T("GodRelationTips6")], g_StrTable[szTemp], g_StrTable[_T("BigeOrQual")], pClergyName->dwAttValue);
	}
	ShowTip(pWnd, szText);
}