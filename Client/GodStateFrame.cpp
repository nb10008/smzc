#include "stdafx.h"
#include "GodStateFrame.h"
#include "GuildEvent.h"
#include "GuildMgr.h"
#include "IconStatic.h"
#include "..\WorldDefine\currency_define.h"

GodStateFrame::GodStateFrame()
: m_Trunk(this)
, m_pWndExpand(NULL)
, m_pAttackNum(NULL)
, m_pAttackGuildName(NULL)
, m_pDefenceNum(NULL)
, m_pDefenceGuildName(NULL)
, m_pStcKillTime(NULL)
, m_pStcLiveTime(NULL)
, m_dwAttackGuildID(NULL)
, m_dwDefenceGuildID(NULL)
{
}

GodStateFrame::~GodStateFrame()
{

}

BOOL GodStateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWndExpand->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&GodStateFrame::EventHandlerExpand)));
	m_pGUI->RegisterEventHandler(m_pWndFold->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&GodStateFrame::EventHandlerExpand)));
	m_pMgr->RegisterEventHandle(_T("tagRefreshGodStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshGodState)));
	m_pMgr->RegisterEventHandle(_T("tagRefreshPersonalGodStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshPersonalGodState)));
	m_pMgr->RegisterEventHandle(_T("tagRefreshFlagStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshFlagState)));

	// 计算服务器时间
	tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
	// 构造
	m_ActivityTime.year = time.year;
	m_ActivityTime.month = time.month;
	m_ActivityTime.day = time.day;
	m_ActivityTime.hour = 20;
	m_ActivityTime.min = 0;
	m_ActivityTime.sec = 0;

	return TRUE;
}

BOOL GodStateFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	m_pGUI->UnRegisterEventHandler(m_pWndExpand->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&GodStateFrame::EventHandlerExpand)));
	m_pGUI->UnRegisterEventHandler(m_pWndFold->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&GodStateFrame::EventHandlerExpand)));
	m_pMgr->UnRegisterEventHandler(_T("tagRefreshGodStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshGodState)));
	m_pMgr->UnRegisterEventHandler(_T("tagRefreshPersonalGodStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshPersonalGodState)));
	m_pMgr->UnRegisterEventHandler(_T("tagRefreshFlagStateEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&GodStateFrame::OnEventRefreshFlagState)));
	if (P_VALID(GetObj("InfoMsgBox")))
		KillObj("InfoMsgBox");

	if (P_VALID(m_pWndExpand))
	{
		m_pGUI->AddToDestroyList(m_pWndExpand);
		m_pWndExpand = NULL;
	}

	if (P_VALID(m_pWndFold))
	{
		m_pGUI->AddToDestroyList(m_pWndFold);
		m_pWndFold = NULL;
	}
	return TRUE;
}

BOOL GodStateFrame::ReloadUI()
{
	XmlElement e;
	tstring strPahtExpand = vEngine::g_strLocalPath + _T("\\ui\\god_battle_state.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPahtExpand.c_str() ) )
		return FALSE;
	for( int i = 0; i < 3; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "god_win\\god_pic\\pic%d", i );
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pWndExpand = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	
	m_pDefenceNum = (GUIPatch*)m_pWndExpand->GetChild(_T("god_pic\\back2"));
	m_pDefenceGuildName = (GUIPatch*)m_pWndExpand->GetChild(_T("god_pic\\back1"));
	m_pAttackNum = (GUIPatch*)m_pWndExpand->GetChild(_T("god_pic\\back4"));
	m_pAttackGuildName = (GUIPatch*)m_pWndExpand->GetChild(_T("god_pic\\back3"));
	m_pStcKillTime = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\killback\\num"));
	m_pStcLiveTime = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\timeback\\num"));
	m_pStcTime = (GUIStatic*)m_pWndExpand->GetChild(_T("god_pic\\back7"));
	m_pBtnDetail = (GUIButton*)m_pWndExpand->GetChild(_T("personal_pic\\butt"));
	m_pBtnFold = (GUIButton*)m_pWndExpand->GetChild(_T("personal_pic\\foldbutt"));

	m_pIconFlagSky = (IconStatic*)m_pWndExpand->GetChild(_T("god_pic\\pic0"));
	m_pIconFlagDevil = (IconStatic*)m_pWndExpand->GetChild(_T("god_pic\\pic1"));
	m_pIconFlagHuman = (IconStatic*)m_pWndExpand->GetChild(_T("god_pic\\pic2"));

	m_pStcExp = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\expback\\num"));
	m_pStcGold = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\moneyback\\goldnum"));
	m_pStcSilver = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\moneyback\\silvernum"));
	m_pStcGuildExploit = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\back0\\num"));
	m_pStcGuildContribute = (GUIStatic*)m_pWndExpand->GetChild(_T("personal_pic\\back1\\num"));

	strPahtExpand = vEngine::g_strLocalPath + _T("\\ui\\god_battle_state_fold.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPahtExpand.c_str() ) )
		return FALSE;
	for( int i = 0; i < 3; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "god_battle_win\\god_pic\\pic%d", i );
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pWndFold = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	m_pDefenceNumFold = (GUIPatch*)m_pWndFold->GetChild(_T("god_pic\\back2"));
	m_pDefenceGuildNameFold = (GUIPatch*)m_pWndFold->GetChild(_T("god_pic\\back1"));
	m_pAttackNumFold = (GUIPatch*)m_pWndFold->GetChild(_T("god_pic\\back4"));
	m_pAttackGuildNameFold = (GUIPatch*)m_pWndFold->GetChild(_T("god_pic\\back3"));
	m_pStcTimeFold = (GUIStatic*)m_pWndFold->GetChild(_T("god_pic\\back7"));
	m_pIconFlagSkyFold = (IconStatic*)m_pWndFold->GetChild(_T("god_pic\\pic0"));
	m_pIconFlagDevilFold = (IconStatic*)m_pWndFold->GetChild(_T("god_pic\\pic1"));
	m_pIconFlagHumanFold = (IconStatic*)m_pWndFold->GetChild(_T("god_pic\\pic2"));
	m_pBtnExpend = (GUIButton*)m_pWndFold->GetChild(_T("god_pic\\unfoldbutt"));
	m_pWndFold->SetInvisible(TRUE);
	return TRUE;
}


DWORD GodStateFrame::EventHandlerExpand(tagGUIEvent *pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	switch (pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnDetail)
			{
				if (P_VALID(GetObj("InfoMsgBox")))
					KillObj("InfoMsgBox");
				CreateObj("InfoMsgBox", "MsgBox");
				TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("GodBattle_Detail")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
			}
			else if( m_pBtnFold == pWnd )
			{
				m_pWndFold->SetInvisible(FALSE);
				m_pWndExpand->SetInvisible(TRUE);
			}
			else if( m_pBtnExpend == pWnd )
			{
				m_pWndFold->SetInvisible(TRUE);
				m_pWndExpand->SetInvisible(FALSE);
			}
		} 
		break;
	default: 
		break;
	}
	return 0;
}

DWORD GodStateFrame::OnEventRefreshGodState(tagRefreshGodStateEvent *pEvent)
{
	m_dwAttackGuildID = pEvent->dwAttackGuildID;
	m_dwDefenceGuildID = pEvent->dwDefenceGuildID;
	
	m_pAttackGuildName->SetText(GuildMgr::Inst()->GetGuildName(m_dwAttackGuildID).c_str());
	m_pDefenceGuildName->SetText(GuildMgr::Inst()->GetGuildName(m_dwDefenceGuildID).c_str());

	m_pAttackGuildNameFold->SetText(GuildMgr::Inst()->GetGuildName(m_dwAttackGuildID).c_str());
	m_pDefenceGuildNameFold->SetText(GuildMgr::Inst()->GetGuildName(m_dwDefenceGuildID).c_str());

	TCHAR szBuffer[X_LONG_NAME] = {0};
	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pEvent->nDefenceGuildMember);
	m_pDefenceNum->SetText(szBuffer);
	m_pDefenceNumFold->SetText(szBuffer);
	
	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pEvent->nAttackGuildMember);
	m_pAttackNum->SetText(szBuffer);
	m_pAttackNumFold->SetText(szBuffer);
	m_pWndExpand->SetRefresh(TRUE);
	m_pWndFold->SetRefresh(TRUE);
	
	return 0;
}

DWORD GodStateFrame::OnEventRefreshPersonalGodState(tagRefreshPersonalGodStateEvent *pEvent)
{
	TCHAR szBuffer[X_LONG_NAME] = {0};
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Kill")], pEvent->nKillTime);
	m_pStcKillTime->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Time")], pEvent->dwLiveTime);
	m_pStcLiveTime->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Exp")], pEvent->nExp);
	m_pStcExp->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Exp")], MSilver2DBGold(pEvent->nMoney));
	m_pStcGold->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Exp")], MSilver2DBSilver(pEvent->nMoney));
	m_pStcSilver->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Exploit")], pEvent->nGuildExploit);
	m_pStcGuildExploit->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Contribute")], pEvent->nGuildContribute);
	m_pStcGuildContribute->SetText(szBuffer);

	m_pWndExpand->SetRefresh(TRUE);

	return 0;
}

DWORD GodStateFrame::OnEventRefreshFlagState(tagRefreshFlagStateEvent *pEvent)
{
	IconStatic *pFlag = NULL;
	IconStatic *pFlagFold = NULL;
	switch(pEvent->eFlagType)
	{
	case EFT_God:
		pFlag = m_pIconFlagSky;
		pFlagFold = m_pIconFlagSkyFold;
		break;
	case EFT_People:
		pFlag = m_pIconFlagHuman;
		pFlagFold = m_pIconFlagHumanFold;
		break;
	case EFT_Monster:
		pFlag = m_pIconFlagDevil;
		pFlagFold = m_pIconFlagDevilFold;
		break;
	default:
		pFlag = m_pIconFlagSky;
		pFlagFold = m_pIconFlagSkyFold;
		break;
	}

	tstring strPic = _T("");
	if(pEvent->eGuildCapType == EGCT_NULL )
		strPic = _T("data\\ui\\city\\white.bmp");
	else if( pEvent->eGuildCapType == EGCT_Capture )
		strPic = _T("data\\ui\\city\\red.bmp");
	else if( pEvent->eGuildCapType == EGCT_Defend )
		strPic = _T("data\\ui\\city\\blue.bmp");

	pFlag->SetPic(strPic);
	pFlagFold->SetPic(strPic);

	return 0;
}

VOID GodStateFrame::Show(bool bShow)
{
	if (bShow)
	{
		if(!m_pWndExpand->IsInvisible())
			return;
		else if(!m_pWndFold->IsInvisible())
			return;

		m_pWndExpand->SetInvisible(FALSE);
	}
	else
	{
		m_pWndExpand->SetInvisible(TRUE);
		m_pWndFold->SetInvisible(TRUE);
	}

}

VOID GodStateFrame::Update()
{
	tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
	// 计算流逝的时间
	DWORD dwEplase = CalcTimeDiff(time, m_ActivityTime);
	//计算剩下的时间
	int nRemain = 60 * 60 - static_cast<int>(dwEplase);
	// 换算成分秒
	int nMinute = nRemain / 60;
	int nSec = nRemain % 60;
	

	TCHAR szBuffer[X_SHORT_NAME] = {0};

	_sntprintf(szBuffer, X_SHORT_NAME, _T("%02d:%02d"), nMinute, nSec);
	m_pStcTime->SetText(szBuffer);
	m_pStcTimeFold->SetText(szBuffer);
	m_pWndExpand->SetRefresh(TRUE);
	GameFrame::Update();
}