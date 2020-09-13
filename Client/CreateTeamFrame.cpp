#include "StdAfx.h"
#include "CreateTeamFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_easy_team.h"
#include "..\WorldDefine\msg_common_errorcode.h"
#include "TeamSys.h"

CreateTeamFrame::CreateTeamFrame(void) : 
m_Trunk(this),
m_pWnd(NULL),
m_pWndBack(NULL),
m_bTeam(FALSE),
m_byTeamMaxNum(6),
m_byTeamScene(GT_INVALID),
m_byTeamType(GT_INVALID),
m_byTeamTypeDetail(0)
{
}

CreateTeamFrame::~CreateTeamFrame(void)
{
}

BOOL CreateTeamFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	m_pCmdMgr->Register("NS_PutOutInfo", (NETMSGPROC)(m_Trunk.sfp2(&CreateTeamFrame::OnNetPutOutInfo)), _T("NS_PutOutInfo"));
	return bRet;
}

BOOL CreateTeamFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}
	m_pCmdMgr->UnRegister("NS_PutOutInfo", (NETMSGPROC)(m_Trunk.sfp2(&CreateTeamFrame::OnNetPutOutInfo)));
	return bRet;
}

BOOL CreateTeamFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL CreateTeamFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\create_team.xml");
	m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CreateTeamFrame::EventHandler));
	m_pWndBack = (GUIPatch*)m_pWnd->GetChild( _T("create_pic") );
	m_pComboBoxType = (GUIComboBox*)m_pWndBack->GetChild( _T("back1\\back1\\ground") );
	m_pComboBoxScene = (GUIComboBox*)m_pWndBack->GetChild( _T("back1\\back2\\ground") );
	m_pComboBoxDetail = (GUIComboBox*)m_pWndBack->GetChild( _T("back1\\back3\\ground") );
	m_pComboBoxTeamNum = (GUIComboBox*)m_pWndBack->GetChild( _T("back1\\back4\\ground") );
	m_pStcTeamNum = (GUIStatic*)m_pWndBack->GetChild( _T("back1\\word4") );

	m_pEditMinLevel = (GUIEditBox*)m_pWndBack->GetChild( _T("back1\\num1\\input") );
	m_pEditMaxLevel = (GUIEditBox*)m_pWndBack->GetChild( _T("back1\\num2\\input") );
	m_pEditTeamDetail = (GUIEditBox*)m_pWndBack->GetChild( _T("back1\\back5\\word") );

	m_pBtnCreate = (GUIButton*)m_pWndBack->GetChild( _T("back1\\createbutt") );
	m_pBtnExit = (GUIButton*)m_pWndBack->GetChild( _T("closebutt") );
	return TRUE;
}

DWORD CreateTeamFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Active:
		if( pWnd == m_pComboBoxType->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxType->GetListBox() );
		else if( pWnd == m_pComboBoxScene->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxScene->GetListBox() );
		else if( pWnd == m_pComboBoxDetail->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxDetail->GetListBox() );
		else if( pWnd == m_pComboBoxTeamNum->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxTeamNum->GetListBox() );
		break;
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit )
				m_pMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnCreate )
			{
				tagNC_PutOutInfo *pMsg;
				tagNC_PutOutInfo vTemp;
				DWORD dwLength = sizeof(tagNC_PutOutInfo);
				tstring strTemp = m_pEditTeamDetail->GetText();
				LPCTSTR szText = strTemp.c_str();
				BYTE* pbyTemp = NULL;
				if( _tcslen(szText) > 0)
				{
					dwLength += _tcslen(szText) * sizeof(TCHAR) - 1;
					pbyTemp = new BYTE[dwLength];
					ZeroMemory(pbyTemp, dwLength);
					pMsg = (tagNC_PutOutInfo*)pbyTemp;
					memcpy( pbyTemp, &vTemp, sizeof(tagNC_PutOutInfo) );
					memcpy( pbyTemp + sizeof(tagNC_PutOutInfo) - 1, szText, _tcslen(szText) * sizeof(TCHAR) );
				}
				else
					pMsg = &vTemp;
				
				pMsg->bTeam = m_bTeam;
				pMsg->byTeamType = (BYTE)m_byTeamType;
				pMsg->byTeamTypeDetail = (BYTE)m_byTeamTypeDetail;
				pMsg->byScene = (BYTE)m_byTeamScene;
				pMsg->byTeamNum = (BYTE)m_byTeamMaxNum;
				pMsg->byMinLevel = (BYTE)_tcstol(m_pEditMinLevel->GetText(),   NULL, 10);
				pMsg->byMaxLevel = (BYTE)_tcstol(m_pEditMaxLevel->GetText(),   NULL, 10);
				pMsg->btLen = _tcslen(szText);
				pMsg->dwSize = dwLength; 
				m_pSession->Send(pMsg);
				if( pbyTemp!= NULL )
					SAFE_DELETEA(pbyTemp);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pComboBoxType->GetListBox() )
		{
			m_byTeamType = BYTE(pEvent->dwParam1 + 1);
			ChangeTeamType();
		}
		else if( pWnd == m_pComboBoxScene->GetListBox() && !m_vecScene.empty() && pEvent->dwParam1 < m_vecScene.size() )
		{
			if(!m_vecScene.empty())
				m_byTeamScene = m_vecScene[BYTE(pEvent->dwParam1)];
			ChangeTeamTypeDetail();
		}
		else if( pWnd == m_pComboBoxDetail->GetListBox() && !m_vecTypeDetail.empty() && pEvent->dwParam1 < m_vecTypeDetail.size() )
		{
			if(!m_vecTypeDetail.empty())
				m_byTeamTypeDetail = m_vecTypeDetail[BYTE(pEvent->dwParam1)];
			ChangeScene();
		}
		else if( pWnd == m_pComboBoxTeamNum->GetListBox() )
			m_byTeamMaxNum = BYTE(pEvent->dwParam1+2);
		break;
	}
	return 0;
}

DWORD CreateTeamFrame::OnNetPutOutInfo(tagNS_PutOutInfo *pNetCmd, DWORD)
{
	if(pNetCmd->dwErrorCode == E_Success )
	{
		m_pFrameMgr->AddToDestroyList(this);
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutOutInfoSucc")]);
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TooManyTeamInfo")]);
		//CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutOutInfoFail")]);
	return 0;
}


VOID CreateTeamFrame::InitData()
{
	// 队伍类型
	m_pComboBoxType->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Instance")]);
	m_pComboBoxType->GetListBox()->SetText(1, 0, g_StrTable[_T("Team_Activity")]);
	m_pComboBoxType->GetListBox()->SetText(2, 0, g_StrTable[_T("Team_Other")]);
	tagPoint ptSize	= m_pComboBoxType->GetListBox()->GetSize();
	ptSize.x = m_pComboBoxType->GetEditBox()->GetSize().x;
	ptSize.y = m_pComboBoxType->GetListBox()->GetRowHeight() * 3;
	m_pComboBoxType->GetListBox()->SetResizable(TRUE);
	m_pComboBoxType->GetListBox()->SetColWidth(0, ptSize.x);
	m_pComboBoxType->GetListBox()->Resize(ptSize);
	m_pComboBoxType->GetListBox()->SetResizable(FALSE);
	tstring strTemp = m_pComboBoxType->GetListBox()->GetText(2, 0);
	m_pComboBoxType->GetEditBox()->SetText(strTemp.c_str());
	m_byTeamType = 3;

	//// 场景
	//m_pComboBoxScene->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
	//ptSize	= m_pComboBoxScene->GetListBox()->GetSize();
	//ptSize.x = m_pComboBoxScene->GetEditBox()->GetSize().x;
	//ptSize.y = m_pComboBoxScene->GetListBox()->GetRowHeight() * 1;
	//m_pComboBoxScene->GetListBox()->SetResizable(TRUE);
	//m_pComboBoxScene->GetListBox()->SetColWidth(0, ptSize.x);
	//m_pComboBoxScene->GetListBox()->Resize(ptSize);
	//m_pComboBoxScene->GetListBox()->SetResizable(FALSE);
	//m_pComboBoxScene->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);
	//m_byTeamScene = GT_INVALID;

	//// 队伍具体类型
	//m_pComboBoxDetail->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
	//ptSize	= m_pComboBoxDetail->GetListBox()->GetSize();
	//ptSize.x = m_pComboBoxDetail->GetEditBox()->GetSize().x;
	//ptSize.y = m_pComboBoxDetail->GetListBox()->GetRowHeight() * 1;
	//m_pComboBoxDetail->GetListBox()->SetResizable(TRUE);
	//m_pComboBoxDetail->GetListBox()->SetColWidth(0, ptSize.x);
	//m_pComboBoxDetail->GetListBox()->Resize(ptSize);
	//m_pComboBoxDetail->GetListBox()->SetResizable(FALSE);
	//m_pComboBoxDetail->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);
	//m_byTeamTypeDetail = GT_INVALID;

	ChangeTeamType();

	// 组队人数
	if(!m_bTeam)
	{
		m_pStcTeamNum->SetInvisible(TRUE);
		m_pComboBoxTeamNum->SetInvisible(TRUE);
		m_pBtnCreate->SetText(g_StrTable[_T("PerAppTeam")]);
		m_pWndBack->SetText(g_StrTable[_T("PerAppTeam")]);
	}
	else
	{
		m_pComboBoxTeamNum->GetListBox()->SetText(0, 0, _T("2"));
		m_pComboBoxTeamNum->GetListBox()->SetText(1, 0, _T("3"));
		m_pComboBoxTeamNum->GetListBox()->SetText(2, 0, _T("4"));
		m_pComboBoxTeamNum->GetListBox()->SetText(3, 0, _T("5"));
		m_pComboBoxTeamNum->GetListBox()->SetText(4, 0, _T("6"));
		ptSize	= m_pComboBoxTeamNum->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxTeamNum->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxTeamNum->GetListBox()->GetRowHeight() * 5;
		m_pComboBoxTeamNum->GetListBox()->SetResizable(TRUE);
		m_pComboBoxTeamNum->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxTeamNum->GetListBox()->Resize(ptSize);
		m_pComboBoxTeamNum->GetListBox()->SetResizable(FALSE);
		strTemp = m_pComboBoxTeamNum->GetListBox()->GetText(4, 0);
		m_pComboBoxTeamNum->GetEditBox()->SetText(strTemp.c_str());
		m_byTeamMaxNum = 6;
	}
	
	m_pEditMinLevel->SetText(_T("1"));
	m_pEditMaxLevel->SetText(_T("100"));
}

VOID CreateTeamFrame::SetTeamFlag(BOOL bFlag)
{
	m_bTeam = bFlag;
	InitData();
}

VOID CreateTeamFrame::ChangeTeamType()
{
	m_vecScene.clear();
	m_vecTypeDetail.clear();
	m_pComboBoxScene->GetListBox()->Clear();
	m_pComboBoxDetail->GetListBox()->Clear();
	INT nRow = 0;
	INT nRowEx = 0;
	m_byTeamTypeDetail = 0;
	switch(m_byTeamType)
	{
	case EETT_Instance:
	case EETT_Activity:
		{
			const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
			for( size_t i = 0; i < vecTeamData.size(); i++ )
			{
				if( vecTeamData[i].byType == m_byTeamType )
				{
					vector<BYTE>::iterator iter = find(m_vecScene.begin(), m_vecScene.end(), vecTeamData[i].bySceneIndex);
					if( iter == m_vecScene.end() )
					{
						m_vecScene.push_back(vecTeamData[i].bySceneIndex);	
						m_pComboBoxScene->GetListBox()->SetText(nRow, 0, vecTeamData[i].strSceneName.c_str());
						nRow++;
					}
					
					if( !m_vecScene.empty() && vecTeamData[i].bySceneIndex == m_vecScene[0] )
					{
						m_vecTypeDetail.push_back(BYTE(vecTeamData[i].dwID));
						m_pComboBoxDetail->GetListBox()->SetText(nRowEx, 0, vecTeamData[i].strTypeName.c_str());
						nRowEx++;
					}
				}
			}
		}
		break;
	case EETT_Other:
		{
			const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
			for( size_t i = 0; i < vecTeamData.size(); i++ )
			{
				vector<BYTE>::iterator iter = find(m_vecScene.begin(), m_vecScene.end(), vecTeamData[i].bySceneIndex);
				if( iter == m_vecScene.end() )
				{
					m_vecScene.push_back(vecTeamData[i].bySceneIndex);	
					m_pComboBoxScene->GetListBox()->SetText(nRow, 0, vecTeamData[i].strSceneName.c_str());
					nRow++;
				}
			}
			m_pComboBoxDetail->GetListBox()->SetText(nRowEx, 0, g_StrTable[_T("Team_Detail")]);
			nRowEx++;
		}
		break;
	default:
		m_pComboBoxScene->GetListBox()->SetText(nRow, 0, g_StrTable[_T("Team_Detail")]);
		m_pComboBoxDetail->GetListBox()->SetText(nRowEx, 0, g_StrTable[_T("Team_Detail")]);
		nRow++;
		nRowEx++;
		break;
	}

	tagPoint ptSize	= m_pComboBoxScene->GetListBox()->GetSize();
	ptSize.x = m_pComboBoxScene->GetEditBox()->GetSize().x;
	ptSize.y = m_pComboBoxScene->GetListBox()->GetRowHeight() * nRow;
	m_pComboBoxScene->GetListBox()->SetResizable(TRUE);
	m_pComboBoxScene->GetListBox()->SetColWidth(0, ptSize.x);
	m_pComboBoxScene->GetListBox()->Resize(ptSize);
	m_pComboBoxScene->GetListBox()->SetResizable(FALSE);
	tstring strTemp = m_pComboBoxScene->GetListBox()->GetText(0, 0);
	m_pComboBoxScene->GetEditBox()->SetText(strTemp.c_str());

	ptSize	= m_pComboBoxDetail->GetListBox()->GetSize();
	ptSize.x = m_pComboBoxDetail->GetEditBox()->GetSize().x;
	ptSize.y = m_pComboBoxDetail->GetListBox()->GetRowHeight() * nRowEx;
	m_pComboBoxDetail->GetListBox()->SetResizable(TRUE);
	m_pComboBoxDetail->GetListBox()->SetColWidth(0, ptSize.x);
	m_pComboBoxDetail->GetListBox()->Resize(ptSize);
	m_pComboBoxDetail->GetListBox()->SetResizable(FALSE);
	strTemp = m_pComboBoxDetail->GetListBox()->GetText(0, 0);
	m_pComboBoxDetail->GetEditBox()->SetText(strTemp.c_str());
	if(!m_vecScene.empty())
		m_byTeamScene = m_vecScene[0];
	if(!m_vecTypeDetail.empty())
		m_byTeamTypeDetail =m_vecTypeDetail[0];
}

VOID CreateTeamFrame::ChangeScene()
{
	m_vecScene.clear();
	m_pComboBoxScene->GetListBox()->Clear();
	INT nRow = 0;
	if( m_byTeamType == EETT_Other )
		return;
	else if( m_byTeamType == EETT_Instance || EETT_Activity == m_byTeamType )
	{
		const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
		for( size_t i = 0; i < vecTeamData.size(); i++ )
		{
			if( BYTE(vecTeamData[i].dwID) == m_byTeamTypeDetail )
			{
				m_vecScene.push_back(vecTeamData[i].bySceneIndex);
				m_pComboBoxScene->GetListBox()->SetText(nRow, 0, vecTeamData[i].strSceneName.c_str());
				nRow++;
				break;
			}
		}
	}
	else
	{
		m_vecScene.push_back(0);
		m_pComboBoxScene->GetListBox()->SetText(nRow, 0, g_StrTable[_T("Team_Detail")]);
		nRow++;
	}
	
	tagPoint ptSize	= m_pComboBoxScene->GetListBox()->GetSize();
	ptSize.x = m_pComboBoxScene->GetEditBox()->GetSize().x;
	ptSize.y = m_pComboBoxScene->GetListBox()->GetRowHeight() * nRow;
	m_pComboBoxScene->GetListBox()->SetResizable(TRUE);
	m_pComboBoxScene->GetListBox()->SetColWidth(0, ptSize.x);
	m_pComboBoxScene->GetListBox()->Resize(ptSize);
	m_pComboBoxScene->GetListBox()->SetResizable(FALSE);
	tstring strTemp = m_pComboBoxScene->GetListBox()->GetText(0, 0);
	m_pComboBoxScene->GetEditBox()->SetText(strTemp.c_str());

	if(!m_vecScene.empty())
		m_byTeamScene = m_vecScene[0];
}


VOID CreateTeamFrame::ChangeTeamTypeDetail()
{
	m_byTeamTypeDetail = 0;
	m_vecTypeDetail.clear();
	m_pComboBoxDetail->GetListBox()->Clear();
	INT nRow = 0;
	if( m_byTeamType == EETT_Instance || EETT_Activity == m_byTeamType)
	{
		const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
		for( size_t i = 0; i < vecTeamData.size(); i++ )
		{
			if( BYTE(vecTeamData[i].bySceneIndex) == m_byTeamScene && m_byTeamType == vecTeamData[i].byType )
			{
				m_vecTypeDetail.push_back(BYTE(vecTeamData[i].dwID));
				m_pComboBoxDetail->GetListBox()->SetText(nRow, 0, vecTeamData[i].strTypeName.c_str());
				nRow++;
			}
		}
	}
	else
	{
		m_vecScene.push_back(0);
		m_pComboBoxDetail->GetListBox()->SetText(nRow, 0, g_StrTable[_T("Team_Detail")]);
		nRow++;
	}

	tagPoint ptSize	= m_pComboBoxDetail->GetListBox()->GetSize();
	ptSize.x = m_pComboBoxDetail->GetEditBox()->GetSize().x;
	ptSize.y = m_pComboBoxDetail->GetListBox()->GetRowHeight() * nRow;
	m_pComboBoxDetail->GetListBox()->SetResizable(TRUE);
	m_pComboBoxDetail->GetListBox()->SetColWidth(0, ptSize.x);
	m_pComboBoxDetail->GetListBox()->Resize(ptSize);
	m_pComboBoxDetail->GetListBox()->SetResizable(FALSE);
	tstring strTemp = m_pComboBoxDetail->GetListBox()->GetText(0, 0);
	m_pComboBoxDetail->GetEditBox()->SetText(strTemp.c_str());

	if(!m_vecTypeDetail.empty())
		m_byTeamTypeDetail = m_vecTypeDetail[0];
}