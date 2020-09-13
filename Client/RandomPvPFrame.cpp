#include "StdAfx.h"
#include "RandomPvPFrame.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "IconStatic.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"

const TCHAR* szShenBing2=	_T("data\\ui\\compet\\compet_shenbing.bmp");
const TCHAR* szXuanYu2=		_T("data\\ui\\compet\\compet_xuanling.bmp");
const TCHAR* szQianKun2=		_T("data\\ui\\compet\\compet_qiankun.bmp");
const TCHAR* szXianYin2=		_T("data\\ui\\compet\\compet_xianyin.bmp");

const TCHAR* szEmpty2=		_T("data\\ui\\compet\\compet_empty.bmp");
const TCHAR* szDead2=		_T("data\\ui\\compet\\compet_dead.bmp");

RandomPvPFrame::RandomPvPFrame(void):m_Trunk(this)
,m_pBnChangeRoom(NULL)
{
}

RandomPvPFrame::~RandomPvPFrame(void)
{
}

BOOL RandomPvPFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pCmdMgr->Register("NS_PvPRanRoomInfo",		(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPRanRoomInfo),	_T("NS_PvPRanRoomInfo"));
	m_pCmdMgr->Register("NS_PvPRanRoomInfoSingleChange",		(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPRanRoomInfoSingleChange),		_T("NS_PvPRanRoomInfoSingleChange"));
	m_pCmdMgr->Register("NS_PvPJoinRoom",		(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPJoinRoom),	_T("NS_PvPJoinRoom"));
	m_pCmdMgr->Register("NS_PvPLeaveRoom",		(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPLeaveRoom),	_T("NS_PvPLeaveRoom"));
	
	m_strEmpty = _T("");
	return bRet;
}

BOOL RandomPvPFrame::Destroy()
{

	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister("NS_PvPRanRoomInfo",		(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPRanRoomInfo));
	m_pCmdMgr->UnRegister("NS_PvPRanRoomInfoSingleChange",			(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPRanRoomInfoSingleChange));
	m_pCmdMgr->UnRegister("NS_PvPJoinRoom",			(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPJoinRoom));
	m_pCmdMgr->UnRegister("NS_PvPLeaveRoom",			(NETMSGPROC)m_Trunk.sfp2(&RandomPvPFrame::OnNS_PvPLeaveRoom));
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd( m_pWnd );
		m_pWnd = NULL;
	}

	if ( P_VALID(m_pWndMin) )
	{
		m_pGUI->DestroyWnd(m_pWndMin);
		m_pWndMin = NULL;
	}

	return bRet;
}

BOOL RandomPvPFrame::ReloadUI()
{

	// load ui resources
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\compet_main.xml");
	if(!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;

	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback0\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback1\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback2\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback3\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback4\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &ele, "compet_room_win\\compet_room_pic\\compet_room_pic\\playerback5\\jobpicback\\jobpic", "ClassName", "IconStatic");
	// create the window
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&RandomPvPFrame::EventHandler));

	m_pBnHide = (GUIButton*)m_pWnd->GetChild( _T("compet_room_pic\\hide") );			// 隐藏
	m_pBnExit = (GUIButton*)m_pWnd->GetChild( _T("compet_room_pic\\leave") );			// 退出
	//m_pBnChangeRoom = (GUIButton*)m_pWnd->GetChild( _T("") );			

	m_pStcName = (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\playername") );		// 姓名
	m_pStcLevel= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\gradenum") );		// 等级
	m_pStcProfession= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\jobname") );	// 职业
	m_pStcGlory= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\glorynum") );		// 荣誉
	m_pStcRankLevel= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\ranknum") );	// 排名
	m_pStcSucessRate= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\percentnum") );// 胜率
	m_pStcPvPPoint= (GUIStatic*)m_pWnd->GetChild( _T("compet_room_pic\\playerback\\ranknum0") );	// 斗技点数

	TCHAR szTmp[X_LONG_NAME] = {0};
	for(int i =0; i< MAX_RANPVP_NUM; i++)
	{
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("compet_room_pic\\compet_room_pic\\playerback%d"), i);
		m_strPvPPlayerInfo[i].pStcBack = (GUIStatic*)m_pWnd->GetChild(szTmp);	

		m_strPvPPlayerInfo[i].pStcLevel = (GUIStatic*)m_strPvPPlayerInfo[i].pStcBack->GetChild(_T("num"));							//等级
		m_strPvPPlayerInfo[i].pStcName = (GUIStatic*)m_strPvPPlayerInfo[i].pStcBack->GetChild(_T("name"));							//姓名
		m_strPvPPlayerInfo[i].pStcProfession = (GUIStatic*)m_strPvPPlayerInfo[i].pStcBack->GetChild(_T("job"));						//职业	
		m_strPvPPlayerInfo[i].pStcProfessionPic = (IconStatic*)m_strPvPPlayerInfo[i].pStcBack->GetChild(_T("jobpicback\\jobpic"));	//职业图片
		m_strPvPPlayerInfo[i].pStcJi = (GUIStatic*)m_strPvPPlayerInfo[i].pStcBack->GetChild(_T("word"));							//“级”字
	}


	strPath = g_strLocalPath + _T("\\ui\\compet_fold.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pWndMin = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pGUI->RegisterEventHandler( m_pWndMin->GetFullName().c_str(),   m_Trunk.sfp1(&RandomPvPFrame::EventHandler));

	m_pBnMin = (GUIButton*)m_pWndMin->GetChild( _T("cm_fold_pic") );

	m_pWnd->SetInvisible(true);
	m_pBnMin->SetInvisible(true);
	m_pWndMin->SetInvisible(false);

	//m_pBnChangeRoom->SetInvisible(true);

	return TRUE;
}

VOID RandomPvPFrame::Update()
{

}

	

BOOL RandomPvPFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBnHide )
		{
			m_pWnd->SetInvisible(TRUE);
			m_pWndMin->SetInvisible(false);
			m_pBnMin->SetInvisible(false);
		}
		else if( pWnd == m_pBnExit )
		{
			tagNC_PvPLeaveRoom cmd;
			TObjRef<NetSession>()->Send(&cmd);
		}
		else if (pWnd == m_pBnMin)
		{
			m_pWnd->SetInvisible(false);
			m_pWndMin->SetInvisible(true);
			m_pBnMin->SetInvisible(true);
		}
		else if (pWnd == m_pBnChangeRoom)
		{
			tagNS_PvPChangeRandomRoom cmd;
			TObjRef<NetSession>()->Send(&cmd);
		}

		break;
	default:
		break;

	}

	return TRUE;
}

VOID RandomPvPFrame::RefreshPlayer(INT pos,INT level,EClassType eType)
{
	if (pos < 0 || pos > MAX_RANPVP_NUM -1)
		return;

	TCHAR szText[X_SHORT_NAME];
	m_strPvPPlayerInfo[pos].pStcName->SetText(_T("???"));
	_stprintf(szText,_T("%d"),level);
	m_strPvPPlayerInfo[pos].pStcLevel->SetText(szText);
	_stprintf(szText,g_StrTable[_T("ClassName_%d")],(INT)eType);
	m_strPvPPlayerInfo[pos].pStcProfession->SetText(szText);

}

VOID RandomPvPFrame::ClearInfo(tagPvPPlayerInfo &strPvPinfo)
{
	strPvPinfo.pStcLevel->SetText(_T(""));
	strPvPinfo.pStcName->SetText(_T(""));
	strPvPinfo.pStcProfession->SetText(_T(""));
	// todo PIC
	strPvPinfo.pStcProfessionPic->SetPic(szEmpty2);
	strPvPinfo.pStcJi->SetInvisible(true);

}

VOID RandomPvPFrame::ClearSaveInfo(tagPvPRanSaveInfo &strPvPinfo)
{
	strPvPinfo.dwPlayerID = 0;
	strPvPinfo.ePlayerProfession = EV_Null;
	strPvPinfo.nPlayerLevel = 0;
	strPvPinfo.strName = _T("");
}

DWORD RandomPvPFrame::OnNS_PvPRanRoomInfo(tagNS_PvPRanRoomInfo* pNetCmd,DWORD dwParam)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return 0;


	TCHAR szText[X_LONG_NAME];
	m_pStcName->SetText(plp->GetRoleName().c_str());
	_stprintf(szText,_T("%d"),plp->GetRoleLevel());
	m_pStcLevel->SetText(szText);

	_stprintf(szText,_T("ClassName_%d"),(INT)plp->GetPlayerClass());
	m_pStcProfession->SetText(g_StrTable[szText]);

	_stprintf(szText,_T("%d"),plp->GetAttribute(ERA_WuXun));
	m_pStcGlory->SetText(szText);
	
	_stprintf(szText,_T("%d"),plp->GetAttribute(ERA_WuJi));
	m_pStcPvPPoint->SetText(szText);


	bool a[MAX_RANPVP_NUM]={false,false,false,false,false,false};

	if (pNetCmd->bCount > MAX_RANPVP_NUM)
		return 0;

	for (int i = 0; i < pNetCmd->bCount ;i++)
	{
		int pos = pNetCmd->pInfo[i].nPos-1;
		if(pos > MAX_RANPVP_NUM -1 ) return 0;	// 数组保护
		a[pos] = true;

		m_strPvPPlayerInfo[pos].pStcName->SetText(_T("???"));

		_stprintf(szText,_T("%d"),pNetCmd->pInfo[i].nPlayerLevel);
		m_strPvPPlayerInfo[pos].pStcLevel->SetText(szText);

		m_strPvPPlayerInfo[pos].pStcJi->SetInvisible(false);

		if (pNetCmd->pInfo[i].ePlayerProfession == EV_Base ||pNetCmd->pInfo[i].ePlayerProfession == EV_Null )
		{
			_stprintf(szText,_T("ClassName_%d"),1);
			m_strPvPPlayerInfo[pos].pStcProfession->SetText(g_StrTable[szText]);
		}
		else
		{
			_stprintf(szText,_T("ClassName_%d"),(INT)pNetCmd->pInfo[i].ePlayerProfession);
			m_strPvPPlayerInfo[pos].pStcProfession->SetText(g_StrTable[szText]);
		}
		
		//todo PIC
		if (pNetCmd->pInfo[i].ePlayerProfession > EV_End)
			return 0;

		switch (pNetCmd->pInfo[i].ePlayerProfession)
		{
		case EV_Blade:
		case EV_Base:
		case EV_Null:
		case EV_Fuhu:
		case EV_XuanJia:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szShenBing2);
			break;
		case EV_Bow:
		case EV_HuanShi:
		case EV_ZhanXin:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szXuanYu2);
			break;
		case EV_Strength:
		case EV_LeiMang:
		case EV_GuaXiang:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szQianKun2);
			break;
		case EV_Musician:
		case EV_ShenShi:
		case EV_TianYi:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szXianYin2);
			break;
		default:
			break;
		}

		m_strPvPSaveInfo[i].dwPlayerID = pNetCmd->pInfo[i].dwPlayerID;
		m_strPvPSaveInfo[i].ePlayerProfession = pNetCmd->pInfo[i].ePlayerProfession;
		m_strPvPSaveInfo[i].nPlayerLevel = pNetCmd->pInfo[i].nPlayerLevel;
		m_strPvPSaveInfo[i].strName = pNetCmd->pInfo[i].szName;
	}
	
	for (int i =0; i < MAX_RANPVP_NUM; i++)
	{
		if (!a[i])
		{
			ClearInfo(m_strPvPPlayerInfo[i]);
			ClearSaveInfo(m_strPvPSaveInfo[i]);
		}
	}


	return 0;
}
DWORD RandomPvPFrame::OnNS_PvPRanRoomInfoSingleChange(tagNS_PvPRanRoomInfoSingleChange* pNetCmd, DWORD dwParam)
{
	if (!pNetCmd->bIsJoin)	// 离开房间
	{
		ClearInfo(m_strPvPPlayerInfo[pNetCmd->pInfo.nPos]);
	}
	else{
		int pos = pNetCmd->pInfo.nPos;

		TCHAR szText[X_SHORT_NAME];
		_stprintf(szText,_T("%d"),pNetCmd->pInfo.nPlayerLevel);
		m_strPvPPlayerInfo[pos].pStcLevel->SetText(szText);

		_stprintf(szText,g_StrTable[_T("ClassName_%d")],(INT)pNetCmd->pInfo.ePlayerProfession);
		m_strPvPPlayerInfo[pos].pStcProfession->SetText(szText);

		if (pNetCmd->pInfo.ePlayerProfession > EV_End)
			return 0;

		switch (pNetCmd->pInfo.ePlayerProfession)
		{
		case EV_Blade:
		case EV_Base:
		case EV_Null:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szShenBing2);
			break;
		case EV_Bow:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szXuanYu2);
			break;
		case EV_Strength:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szQianKun2);
			break;
		case EV_Musician:
			m_strPvPPlayerInfo[pos].pStcProfessionPic->SetPic(szXianYin2);
			break;
		}
	}
	return 0;
}

VOID RandomPvPFrame::ClearAll()
{
	m_pStcName->SetText(_T(""));
	m_pStcLevel->SetText(_T(""));
	m_pStcProfession->SetText(_T(""));
	m_pStcGlory->SetText(_T(""));
	m_pStcRankLevel->SetText(_T(""));
	m_pStcSucessRate->SetText(_T(""));
	m_pStcPvPPoint->SetText(_T(""));

	for (int i =0;i< MAX_RANPVP_NUM; i++)
	{
		ClearInfo(m_strPvPPlayerInfo[i]);
		ClearSaveInfo(m_strPvPSaveInfo[i]);
	}
}

DWORD RandomPvPFrame::OnNS_PvPJoinRoom(tagNS_PvPJoinRoom* pNetCmd,DWORD dwParam)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
			TCHAR szText[X_LONG_NAME];
		_stprintf(szText,_T("%d"),pNetCmd->dwLocalPlayerRankLevel);
		m_pStcRankLevel->SetText(szText);

		int rate = pNetCmd->fLocalPlayerSuccessRate * 100;
		_stprintf(szText,_T("%d%%"),rate);
		m_pStcSucessRate->SetText(szText);

		m_pWnd->SetInvisible(false);
	}
	else
	{
		switch(pNetCmd->dwErrorCode)
		{
		case E_PvP_InvalidLevel:
		case E_PvP_LevelLimitError:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed_level")]);
			break;
		case E_PvP_AlreadyInRoom:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed_alreadyIn")]);
			break;
		case E_PvP_RoomIsFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed_full")]);
			break;
		case E_PvP_CreateRoomFail:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed_Create")]);
			break;
		case E_PvP_UnOpened:
		case E_PvP_InviteNotFriend:
		case E_PvP_InviteFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed_OnOpen")]);
			break;
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_enter_failed")]);
		}
	}
	return 0;
}

DWORD RandomPvPFrame::OnNS_PvPLeaveRoom(tagNS_PvPLeaveRoom* pNetCmd,DWORD dwParam)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		m_pWnd->SetInvisible(true);
		m_pWndMin->SetInvisible(true);
		m_pBnMin->SetInvisible(true);
		ClearAll();
	}
	else
	{
		switch(pNetCmd->dwErrorCode)
		{
		case E_PvP_RoleInReadyState:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_Leave_failed_ready")]);
			break;
		case E_PvP_RoomInFighting:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_Leave_failed_fight")]);
			break;
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_Leave_failed")]);
		}
	}

	return 0;
}

tstring& RandomPvPFrame::GetPvPmemberName(DWORD dwPlayerID)
{
	for(int i =0; i< MAX_RANPVP_NUM; i++)
	{
		if (m_strPvPSaveInfo[i].dwPlayerID == dwPlayerID)
		{
			return m_strPvPSaveInfo[i].strName;
		}
	}

	return m_strEmpty;
}

EClassType RandomPvPFrame::GetPvPmemberClass(DWORD dwPlayerID)
{
	for(int i =0; i< MAX_RANPVP_NUM; i++)
	{
		if (m_strPvPSaveInfo[i].dwPlayerID == dwPlayerID)
		{
			return m_strPvPSaveInfo[i].ePlayerProfession;
		}
	}

	return EV_Null;
}