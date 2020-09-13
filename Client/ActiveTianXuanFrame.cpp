#include "stdafx.h"
#include "ActiveTianXuanFrame.h"
#include "MapMgr.h"


ActiveTianXuanFrame::ActiveTianXuanFrame( void ):m_Trunk(this)
{
	m_pWndMain		= NULL;
	m_pWndMain		= NULL;
	m_pStcBluePoint = NULL;		//蓝方分数
	m_pStcRedPoint	= NULL;		//红方分数
	m_pStcBlueNum	= NULL;		//蓝方人数
	m_pStcRedNum	= NULL;		//红方人数
	m_pStcIcon		= NULL;		//中间那个东西
	m_pProPersent	= NULL;

}

ActiveTianXuanFrame::~ActiveTianXuanFrame( void )
{

}

BOOL ActiveTianXuanFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	//m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QiLingAttResetFrame::OnEventProduceAddItem));


	TObjRef<NetCmdMgr>()->Register("NS_DotaMsg",	(NETMSGPROC)m_Trunk.sfp2(&ActiveTianXuanFrame::OnNS_DotaMsg),		_T("NS_DotaMsg"));
	TObjRef<NetCmdMgr>()->Register("NS_GotoNewMap",	(NETMSGPROC)m_Trunk.sfp2(&ActiveTianXuanFrame::OnNS_GotoNewMap),	_T("NS_GotoNewMap"));

	return TRUE;
}

BOOL ActiveTianXuanFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	SetStrategy(EIST_ItemPocket);

	//--注册游戏事件
	//m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QiLingAttResetFrame::OnEventProduceAddItem));

	TObjRef<NetCmdMgr>()->UnRegister("NS_DotaMsg",		(NETMSGPROC)m_Trunk.sfp2(&ActiveTianXuanFrame::OnNS_DotaMsg));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GotoNewMap",	(NETMSGPROC)m_Trunk.sfp2(&ActiveTianXuanFrame::OnNS_GotoNewMap));


	return TRUE;
}

BOOL ActiveTianXuanFrame::ReloadUI()
{
	XmlElement element;
	TCHAR szText[128] = {0};
	tstring strPath = g_strLocalPath + _T("\\ui\\battle.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "battlewin\\battle\\pro", "ClassName", "ActiveProgress");

	m_pWndMain = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pStcBluePoint		= (GUIStatic*)m_pWndMain->GetChild(_T("battle\\points1"));			//蓝方分数
	m_pStcRedPoint		= (GUIStatic*)m_pWndMain->GetChild(_T("battle\\points0"));			//红方分数
	m_pStcBlueNum		= (GUIStatic*)m_pWndMain->GetChild(_T("battle\\num1"));				//蓝方人数
	m_pStcRedNum		= (GUIStatic*)m_pWndMain->GetChild(_T("battle\\num0"));				//红方人数
	m_pStcIcon			= (GUIStatic*)m_pWndMain->GetChild(_T("battle\\pro\\pic"));			//中间那个东西
	m_pProPersent		= (ActiveProgress*)m_pWndMain->GetChild(_T("battle\\pro"));			//进度条

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&ActiveTianXuanFrame::EventHandler));


	m_pWndMain->SetInvisible(TRUE);

	return TRUE;
}

VOID ActiveTianXuanFrame::Update()
{
	GameFrame::Update();
}

VOID ActiveTianXuanFrame::OnEvent( tagGameEvent* pEvent )
{

}

DWORD ActiveTianXuanFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{

		}
		break;
	case EGUIE_MoveIntoCtrl:
		{

		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{

		}
		break;
	}

	return TRUE;
}

DWORD ActiveTianXuanFrame::OnNS_DotaMsg(tagNS_DotaMsg* pCmd, DWORD)
{
	if(pCmd->dwMsgType == 1)	//天选者
	{
		m_pWndMain->SetInvisible(FALSE);
		//设置信息....
		LPCTSTR strMsg = m_pUtil->AnsiToUnicode(pCmd->msg);
		tstring strRedPoint		= GetNextValue(strMsg, ',');
		tstring strRedNum		= GetNextValue(strMsg, ',');
		tstring strBluePoint	= GetNextValue(strMsg, ',');
		tstring strBlueNum		= GetNextValue(strMsg, ',');

		m_dwRedPoint	= _ttoi(strRedPoint.c_str());	
		m_dwBluePoint	= _ttoi(strBluePoint.c_str());	
		m_nRedNum		= _ttoi(strRedNum.c_str());		
		m_nBlueNum		= _ttoi(strBlueNum.c_str());		

		//刷新信息
		RefreshUI();
	}

	return 0;
}

DWORD ActiveTianXuanFrame::OnNS_GotoNewMap(tagNS_GotoNewMap* pCmd, DWORD)
{
	if(MapMgr::Inst()->GetCurMapID() != pCmd->dwMapID)
	{
		if(!m_pWndMain->IsInvisible())
		{
			m_pWndMain->SetInvisible(TRUE);
		}
	}
	return 0;
}

VOID ActiveTianXuanFrame::RefreshUI()
{
	TCHAR szTemp[128] = {0};
	_stprintf(szTemp, _T("%d"), m_dwBluePoint);
	m_pStcBluePoint->SetText(szTemp);
	_stprintf(szTemp, _T("%d"), m_dwRedPoint);
	m_pStcRedPoint->SetText(szTemp);
	_stprintf(szTemp, _T("%d"), m_nBlueNum);
	m_pStcBlueNum->SetText(szTemp);	
	_stprintf(szTemp, _T("%d"), m_nRedNum);
	m_pStcRedNum->SetText(szTemp);	
	//设置中间游动的图标 X 0-428   Y -9
	m_pStcIcon->SetMovable(TRUE);
	const tagPoint& OldPos = m_pStcIcon->GetClient();
	DWORD	nXPos;
	if(m_dwRedPoint+m_dwBluePoint != 0)
		nXPos = 428*m_dwRedPoint/(m_dwRedPoint+m_dwBluePoint);
	else
		nXPos = 428/2;
	tagPoint tNewposVar;
	tNewposVar.x = nXPos - OldPos.x;
	tNewposVar.y = 0;
	m_pStcIcon->Move(tNewposVar);
	m_pStcIcon->SetMovable(FALSE);
	m_pProPersent->SetMaxValue(100);
	INT nPersent;
	if(m_dwRedPoint+m_dwBluePoint != 0)
		nPersent = m_dwRedPoint*100/(m_dwRedPoint+m_dwBluePoint);
	else
		nPersent = 50;
	m_pProPersent->SetValue(nPersent, TRUE);

	m_pWndMain->SetRefresh(TRUE);

}

tstring ActiveTianXuanFrame::GetNextValue(LPCTSTR &pMsg, TCHAR token)
{
	TCHAR temp[512] = {0};
	TCHAR* pTemp = temp;
	while(*pMsg != 0)
	{
		if(*pMsg == token)
		{
			pMsg++;
			break;
		}
		*pTemp++ = *pMsg++;
	}
	return tstring(temp);
}