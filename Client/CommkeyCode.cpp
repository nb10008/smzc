#include "StdAfx.h"
#include "CommKeyCode.h"
#include "..\WorldDefine\msg_item.h"
#include "QuestEvent.h"
#include "..\WorldDefine\creature_define.h"
#include "CreatureData.h"
#include "CombatSysUtil.h"
#include "QuestMgr.h"
#include "RoleMgr.h"

CommKeyCode::CommKeyCode(void):m_Trunk(this)
{
	m_escapeTime = 0;
}

CommKeyCode::~CommKeyCode(void)
{

}

BOOL CommKeyCode::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
 	if( !GameFrame::Init(pMgr, pFather, dwParam) )
 		return FALSE;
 
	//--注册游戏事件
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagStartNPCTalkEvent"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&CommKeyCode::OnStartNPCTalkEvent));

	m_pCmdMgr->Register("NS_GetKeyCodeGift", (NETMSGPROC)m_Trunk.sfp2(&CommKeyCode::OnNS_GetKeyCodeGift), 		_T("NS_GetKeyCodeGift"));

	return TRUE;
}

BOOL CommKeyCode::Destroy()
{
 	GameFrame::Destroy();
 	if( P_VALID(m_pWndMain) )
 		m_pGUI->AddToDestroyList(m_pWndMain);
 
	//--注销游戏事件
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagStartNPCTalkEvent"), 	(FRAMEEVENTPROC)m_Trunk.sfp1(&CommKeyCode::OnStartNPCTalkEvent));

	m_pCmdMgr->UnRegister("NS_GetKeyCodeGift", (NETMSGPROC)m_Trunk.sfp2(&CommKeyCode::OnNS_GetKeyCodeGift));
	return TRUE;
}

BOOL CommKeyCode::EscCancel()
{
	if(!m_pWndMain->IsInvisible())
		m_pWndMain->SetInvisible(TRUE);

 	return FALSE;
}

BOOL CommKeyCode::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\common3.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pWndMain		= m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pEboxInput	= (GUIEditBox*)m_pWndMain->GetChild(_T("comm_pic\\putinpic\\putin"));
	m_pBtnOK		= (GUIButton*)m_pWndMain->GetChild(_T("comm_pic\\surebutt"));
	m_pBtnCancel	= (GUIButton*)m_pWndMain->GetChild(_T("comm_pic\\canclebutt"));
	m_pStcTip		= (GUIStatic*)m_pWndMain->GetChild(_T("comm_pic\\comm_words"));
	m_pStcTip->SetText(g_StrTable[_T("KeyCode_Tips")]);
	m_pWndMain->SetInvisible(TRUE);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&CommKeyCode::EventHandler));
	
	return TRUE;
}

VOID CommKeyCode::OnEvent(tagGameEvent* pEvent)
{

}

VOID CommKeyCode::Update()
{
	GameFrame::Update();

	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			m_pWndMain->SetInvisible(TRUE);
		}
	}
}


DWORD CommKeyCode::OnStartNPCTalkEvent(tagStartNPCTalkEvent* pGameEvent)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pGameEvent->typeID);
	if(!P_VALID(pCreatureProto))
		return 0;

	if(pCreatureProto->eFunctionType == EFNPCT_CommNewKeyCodeNPC)
	{
		m_pWndMain->SetInvisible(FALSE);
		m_pGUI->SetActive(m_pEboxInput);
		m_pEboxInput->SetText(_T(""));
	}

	return 0;
}

BOOL CommKeyCode::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnOK )				//确定
		{
			tagNC_GetKeyCodeGift cmd;
			cmd.dwKeyCodeCrc = TObjRef<Util>()->Crc32(m_pEboxInput->GetText());
			TObjRef<NetSession>()->Send(&cmd);
			m_pWndMain->SetInvisible(TRUE);
		}
		else if( pWnd == m_pBtnCancel )
		{
			m_pWndMain->SetInvisible(TRUE);
		}
	}

	return TRUE;
}

DWORD CommKeyCode::OnNS_GetKeyCodeGift(tagNS_GetKeyCodeGift* pMsg, DWORD pPrama)
{
	if(pMsg->dwErrorCode == 2)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NewKeyCodeHaveGetGift")]);
	}
	else if(pMsg->dwErrorCode == 1)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NewKeyCodeError")]);
	}
	return 0;
}
