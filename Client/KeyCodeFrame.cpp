#include "StdAfx.h"
#include "KeyCodeFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "QuestEvent.h"
#include "..\WorldDefine\msg_func_npc.h"

KeyCodeFrame::KeyCodeFrame(void)
:m_Trunk(this)
,m_escapeTime(GT_INVALID)
,m_bIsNew(FALSE)
,IsCheckDistance(TRUE)//add by xp
{
	m_pWnd			= NULL;
	m_pStETips		= NULL;
	m_pEBxKeyCode	= NULL;
	m_BtnConfirm	= NULL;
	m_BtnCancel		= NULL;
	m_bOldPlayerBack = false;
}

KeyCodeFrame::~KeyCodeFrame(void)
{
}

VOID KeyCodeFrame::Update()
{
	GameFrame::Update();

	//检测是否离NPC过远，过远则关闭frame
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000  && !m_bOldPlayerBack)//每隔1秒检测
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if(IsCheckDistance)//add by xp
		{
			if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) ) 
			{
				if (P_VALID(m_pWnd))
					m_pFrameMgr->AddToDestroyList(this);
			}			

		}//add end
		
	}

}

BOOL KeyCodeFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL KeyCodeFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL ret    =   GameFrame::Init(pMgr, pFather, dwParam);

	// 注册游戏事件
	m_pCmdMgr->Register("NS_KeyCode", (NETMSGPROC)m_Trunk.sfp2(&KeyCodeFrame::OnRecvServerMsg),  _T("NS_KeyCode"));
	m_pCmdMgr->Register("NS_NewKeyCode", (NETMSGPROC)m_Trunk.sfp2(&KeyCodeFrame::OnNS_NewKeyCode),  _T("NS_NewKeyCode"));

	if (1 == dwParam)
	{
		m_bOldPlayerBack = true;
		m_pStETips->SetText(g_StrTable[_T("OldPlayerBack_Tips")]);
	}


	return ret;
}

BOOL KeyCodeFrame::Destroy()
{
	BOOL ret    =   GameFrame::Destroy();

	// 注销游戏事件
	m_pCmdMgr->UnRegister("NS_KeyCode", (NETMSGPROC)m_Trunk.sfp2(&KeyCodeFrame::OnRecvServerMsg));
	m_pCmdMgr->UnRegister("NS_NewKeyCode", (NETMSGPROC)m_Trunk.sfp2(&KeyCodeFrame::OnNS_NewKeyCode));

	DestroyObjects();

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return ret;
}

BOOL KeyCodeFrame::ReloadUI()
{
	// 创建UI
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\common3.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	//m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml(&ele, "comm3_win\\comm_pic\\putinpic\\putin", "MaxTextCount", "36");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pStETips		= (GUIStaticEx*)m_pWnd->GetChild(_T("comm_pic\\comm_words"));
	m_pEBxKeyCode	= (GUIEditBox*)m_pWnd->GetChild(_T("comm_pic\\putinpic\\putin"));
	m_BtnConfirm	= (GUIButton*)m_pWnd->GetChild(_T("comm_pic\\surebutt"));
	m_BtnCancel		= (GUIButton*)m_pWnd->GetChild(_T("comm_pic\\canclebutt"));
	m_pStETips->SetText(g_StrTable[_T("KeyCode_Tips")]);
	m_pWnd->SetInvisible(TRUE);

	// 注册UI事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&KeyCodeFrame::EventHandler));
	
	return TRUE;
}

DWORD KeyCodeFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_BtnConfirm )
			{
				if (m_bOldPlayerBack)
				{
					tstring tstrKey = m_pEBxKeyCode->GetText();
					if (tstrKey.empty())
						return 0;

					tagNC_ReactiveKeyCode cmd;
					cmd.dwKeyCode = _tstoi(tstrKey.c_str());
					m_pSession->Send(&cmd);
					m_pFrameMgr->AddToDestroyList(this);

				}
				else if (CanSendKeyCode())
				{
					m_pEBxKeyCode->SetText(_T(""));
					m_pFrameMgr->AddToDestroyList(this);
				}
			}
			else if ( pWnd == m_BtnCancel )
			{
				m_pEBxKeyCode->SetText(_T(""));
				m_pFrameMgr->AddToDestroyList(this);
			}
			
		}
		break;
	default:
		break;
	}

	return 0;
}

DWORD KeyCodeFrame::OnRecvServerMsg( tagNS_KeyCode* msg, DWORD )
{
	int nError = (msg->nErrorCode);
	if ( nError<0 || nError>14 )
	{
		nError = 14;//未知错误
	} 
	RevMsgError(nError);
	return 0;
}

DWORD KeyCodeFrame::OnNS_NewKeyCode( tagNS_NewKeyCode* msg, DWORD )
{
	/*
	E_KeyCode_CodeInvalid		= -423,		// 无效的key码
	E_KeyCode_NPCInvalid		= -424,		// NPC无效
	E_KeyCode_OnlyOnce			= -425,		// 只能领取一次
	*/
	int nError = -(msg->dwErrorCode);
	if ( nError < 423 || nError > 425 )
	{
		nError = 14;//未知错误
	} 
	RevMsgError(nError);
	return 0;

}


VOID KeyCodeFrame::DestroyObjects()
{
	if(P_VALID(GetObj("KeyCode_Msg")))
		KillObj("KeyCode_Msg");
}

BOOL  KeyCodeFrame::RevMsgError(DWORD dwErrorID)
{
	if(P_VALID(GetObj("KeyCode_Msg")))
		KillObj("KeyCode_Msg");
	CreateObj("KeyCode_Msg", "MsgBoxEx");
	tstringstream tss;
	tss<<_T("KeyCode_Msg")<<dwErrorID;
	TObjRef<MsgBoxEx>("KeyCode_Msg")->Init( _T(""), g_StrTable[tss.str().c_str()], _T("KeyCode_Msg") );
	
	return FALSE;
}

BOOL KeyCodeFrame::CanSendKeyCode()
{
	tstring tstrKey = m_pEBxKeyCode->GetText();
	string strKey = m_pUtil->UnicodeToAnsi(tstrKey.c_str());
	if (strKey.empty())
		return FALSE;

	if (!m_bIsNew)
	{
		tagNC_KeyCode SendKeyCode;
		SendKeyCode.dwNPCID = m_dwNPCID;
		strncpy_s( SendKeyCode.szKeyCode, strKey.c_str(), X_LONG_NAME );
		m_pSession->Send(&SendKeyCode);
	}
	else
	{
		tagNC_NewKeyCode SendKeyCode;
		SendKeyCode.dwNPCID = m_dwNPCID;
		strncpy_s( SendKeyCode.szKeyCode, strKey.c_str(), X_LONG_NAME );
		m_pSession->Send(&SendKeyCode);
	}
	
	return TRUE;
}
