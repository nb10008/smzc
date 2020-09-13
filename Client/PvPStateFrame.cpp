#include "stdafx.h"

#include "PvPStateFrame.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "PvPMgr.h"
#include "IconStatic.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CountDownFrame.h"
#include "PlayerNameTab.h"
#include "RandomPvPFrame.h"
#include "Role.h"
#include "ActiveProgress.h"
#include "CombatSysUtil.h"

#define QUIT_PVP_TIME (60 * 1000)
#define SHOW_KILL_TIME (2 * 1000)
#define TEX_FILE_PATH	"data\\ui\\compose\\l-compose.bmp"
#define TEX_FILE_PATH_2	"data\\ui\\compose\\L-compose-02.bmp"

const TCHAR* szLeave=		_T("data\\ui\\compet\\compet_leave.bmp");
const TCHAR* szSuccess=		_T("data\\ui\\compet\\compet_win.bmp");
const TCHAR* szFail=		_T("data\\ui\\compet\\compet_lose.bmp");
const TCHAR* szPerfect=		_T("data\\ui\\compet\\compet_win1.bmp");
const TCHAR* szTie=			_T("data\\ui\\compet\\compet_draw.bmp");

const TCHAR* szDead=		_T("data\\ui\\compet\\compet_dead.bmp");
const TCHAR* szEmpty=		_T("data\\ui\\compet\\compet_empty.bmp");

const TCHAR* szShenBing=	_T("data\\ui\\compet\\compet_shenbing.bmp");
const TCHAR* szXuanYu=		_T("data\\ui\\compet\\compet_xuanling.bmp");
const TCHAR* szQianKun=		_T("data\\ui\\compet\\compet_qiankun.bmp");
const TCHAR* szXianYin=		_T("data\\ui\\compet\\compet_xianyin.bmp");


const DWORD GrayColour = 0xFF808080;
const DWORD AliveColour = 0xFFFCF157;

const DWORD KILL_SHOW_TIME = 3000;


PvPStateFrame::PvPStateFrame(void)
:m_Trunk(this)
{
	m_pUtil = "Util";
	m_dwTime = 0;
	m_dwStartToShow = 0;
}

PvPStateFrame::~PvPStateFrame(void)
{

}

BOOL PvPStateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;	

	m_pCmdMgr->Register("NS_PvPResultInfo",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvResultInfo),	_T("NS_PvPResultInfo"));
	m_pCmdMgr->Register("NS_PvPGetPvPState",		(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvPvPState),		_T("NS_PvPGetPvPState"));
	m_pCmdMgr->Register("NS_PvPNotifyKill",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvNotifyKill),	_T("NS_PvPNotifyKill"));
	m_pCmdMgr->Register("NS_PvPRanRoomResultInfo",	(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPRanRoomResultInfo),	_T("NS_PvPRanRoomResultInfo"));
	m_pCmdMgr->Register("NS_PvPSinglePlayerInfo",	(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPSinglePlayerInfo),	_T("NS_PvPSinglePlayerInfo"));
	m_pCmdMgr->Register("NS_PvPEnterPvP",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPEnterPvP),		_T("NS_PvPStart"));
	m_pCmdMgr->Register("NS_RemoteAttChangeSingle",	(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRemoteAttChangeSingle),		_T("NS_RemoteAttChangeSingle"));

	for (int i = 0;i < ENEMY_NUM; i++)
	{
		m_dwEnemy[i] = 0;
	}

	m_bStartDeadCheck = false;
	
	return TRUE;
}

BOOL PvPStateFrame::Destroy()
{
	if(P_VALID(m_pWndResult))
		m_pGUI->AddToDestroyList(m_pWndResult);
	if(P_VALID(m_pWndKill))
		m_pGUI->AddToDestroyList(m_pWndKill);
	if(P_VALID(m_pWndFriend))
		m_pGUI->AddToDestroyList(m_pWndFriend);
	if(P_VALID(m_pWndEnemy))
		m_pGUI->AddToDestroyList(m_pWndEnemy);
	if(P_VALID(m_pWndEnemy))
		m_pGUI->AddToDestroyList(m_pWndEnemyState);
	if(P_VALID(m_pWndEnemy))
		m_pGUI->AddToDestroyList(m_pWndEnemyStateMin);
	if(P_VALID(m_pWndEnemy))
		m_pGUI->AddToDestroyList(m_pFinalPointsWin);
	if(P_VALID(m_pWndKillNum))
		m_pGUI->AddToDestroyList(m_pWndKillNum);


	m_pMgr->UnRegisterEventHandler(_T("CloseMap"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPStateFrame::OnCloseMap));
	m_pMgr->UnRegisterEventHandler(_T("OpenMap"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPStateFrame::OnOpenMap));

	m_pCmdMgr->UnRegister("NS_PvPResultInfo",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvResultInfo));
	m_pCmdMgr->UnRegister("NS_PvPGetPvPState",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvPvPState));
	m_pCmdMgr->UnRegister("NS_PvPNotifyKill",			(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRecvNotifyKill));
	m_pCmdMgr->UnRegister("NS_PvPRanRoomResultInfo",	(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPRanRoomResultInfo));
	m_pCmdMgr->UnRegister("NS_PvPSinglePlayerInfo",		(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPSinglePlayerInfo));
	m_pCmdMgr->UnRegister("NS_PvPEnterPvP",				(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetNS_PvPEnterPvP));
	m_pCmdMgr->UnRegister("NS_RemoteAttChangeSingle",	(NETMSGPROC)m_Trunk.sfp2(&PvPStateFrame::NetRemoteAttChangeSingle));
	
	return GameFrame::Destroy();
}

BOOL PvPStateFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\compet_account.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pWndResult = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pBtnLeave = (GUIButton*)m_pWndResult->GetChild(_T("account_pic\\shutbutt")); // 离开
	m_pBtnClose = (GUIButton*)m_pWndResult->GetChild(_T("account_pic\\closebutt")); 
	m_pStaticTime = (GUIStatic*)m_pWndResult->GetChild(_T("account_pic\\word0")); 
	m_pLstTeamRst = (GUIListBox*)m_pWndResult->GetChild(_T("account_pic\\listback\\listback0\\member_list"));
	m_pLstEnemyRst = (GUIListBox*)m_pWndResult->GetChild(_T("account_pic\\listback0\\listback0\\member_list"));
	m_pStaticGain = (GUIStatic*)m_pWndResult->GetChild(_T("account_pic\\num0"));
	m_pStaticTotal = (GUIStatic*)m_pWndResult->GetChild(_T("account_pic\\num1"));
	m_pLstTeamRst->SetColNum(6, 70);
	m_pLstTeamRst->SetColWidth(0, 122);
	m_pLstTeamRst->SetColWidth(1, 115);
	m_pLstTeamRst->SetColWidth(2, 57);
	m_pLstTeamRst->SetColWidth(3, 73);
	m_pLstTeamRst->SetColWidth(4, 57);
	m_pLstTeamRst->SetColWidth(5, 73);

	m_pLstEnemyRst->SetColNum(6, 70);
	m_pLstEnemyRst->SetColWidth(0, 122);
	m_pLstEnemyRst->SetColWidth(1, 115);
	m_pLstEnemyRst->SetColWidth(2, 57);
	m_pLstEnemyRst->SetColWidth(3, 73);
	m_pLstEnemyRst->SetColWidth(4, 57);
	m_pLstEnemyRst->SetColWidth(5, 73);


	m_pGUI->RegisterEventHandler(m_pWndResult->GetFullName().c_str(), m_Trunk.sfp1(&PvPStateFrame::RstWndEventHandler));
	m_pMgr->RegisterEventHandle(_T("CloseMap"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPStateFrame::OnCloseMap));
	m_pMgr->RegisterEventHandle(_T("OpenMap"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPStateFrame::OnOpenMap));

	m_pWndResult->SetInvisible(TRUE);
	m_pWndResult->FlipToTop();

	GUIWnd* pDesttop = m_pGUI->GetDesktop();

	tagPoint clientSize = pDesttop->GetSize();
	strPath = g_strLocalPath + _T("\\ui\\compet_behead.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "try_behead_win\\try_behead_pic\\num", "ClassName", "IconStatic");
	m_pWndKill = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pStaticKillNum = (IconStatic*)m_pWndKill->GetChild(_T("try_behead_pic\\num"));
	m_pWndKill->SetInvisible(FALSE);
	m_pWndKill->FlipToTop();
	m_pWndKill->SetResizable(TRUE);
	m_pWndKill->SetMovable(TRUE);
	tagPoint s = (clientSize - m_pWndKill->GetSize())/2 - m_pWndKill->GetClient();
	m_pWndKill->Move(s);
	m_pWndKill->SetMovable(FALSE);
	m_pWndKill->SetUnClickable(TRUE);
	m_pWndKill->SetResizable(FALSE);
	

	strPath = g_strLocalPath + _T("\\ui\\compet_friendnum.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pWndFriend = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pStaticFriend = (GUIStatic*)m_pWndFriend->GetChild(_T("compet_friend_pic\\num"));
	m_pStaticFriend->SetText(_T("3"));
	m_pStaticFriend->SetRefresh(TRUE);
	m_pWndFriend->SetInvisible(TRUE);
	m_pWndFriend->FlipToTop();
	m_pWndFriend->SetMovable(TRUE);
	s.x = clientSize.x/2 - clientSize.x/14 - m_pWndFriend->GetSize().x - m_pWndFriend->GetClient().x;
	s.y = clientSize.y/7 - m_pWndFriend->GetClient().y;
	m_pWndFriend->Move(s);
	m_pWndFriend->SetMovable(FALSE);
	m_pWndFriend->SetUnClickable(TRUE);
	m_pWndFriend->SetResizable(FALSE);

	strPath = g_strLocalPath + _T("\\ui\\compet_enemynum.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pWndEnemy = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pStaticEnemy = (GUIStatic*)m_pWndEnemy->GetChild(_T("compet_enemy_pic\\num"));
	m_pStaticEnemy->SetText(_T("3"));
	m_pStaticEnemy->SetRefresh(TRUE);
	m_pWndEnemy->SetInvisible(TRUE);
	m_pWndEnemy->FlipToTop();
	m_pWndEnemy->SetMovable(TRUE);
	s.x = clientSize.x/2 +  clientSize.x/14  - m_pWndEnemy->GetClient().x;
	s.y = clientSize.y/7 - m_pWndEnemy->GetClient().y;
	m_pWndEnemy->Move(s);
	m_pWndEnemy->SetMovable(FALSE);
	m_pWndEnemy->SetUnClickable(TRUE);
	m_pWndEnemy->SetResizable(FALSE);

	strPath = g_strLocalPath + _T("\\ui\\compet_points.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_points_win\\compet_points_pic\\back\\personal_result\\resultpic", "ClassName", "IconStatic");
	m_pFinalPointsWin = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pGUI->RegisterEventHandler(m_pFinalPointsWin->GetFullName().c_str(), m_Trunk.sfp1(&PvPStateFrame::EventHandler));

	m_PvPResultWin[0].pKills = (GUIStatic*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\killnum"));
	m_PvPResultWin[0].pName = (GUIStatic*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\name"));
	m_PvPResultWin[0].pRongYu = (GUIStaticEx*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\honornum"));
	m_PvPResultWin[0].pRongYuChange = (GUIStaticEx*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\honoradd"));
	m_PvPResultWin[0].pSuccessRate = (GUIStaticEx*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\percentnum"));

	TCHAR tbuff[128];
	for (int i = 1; i < MAX_RANDOM_PVP_MUN; i++)
	{
		int j = i;
		if (i == 5 )
			j=6;

		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("compet_points_pic\\back\\player%d"), j-1);
		GUIStatic* pWnd = (GUIStatic*)m_pFinalPointsWin->GetChild(tbuff);

		m_PvPResultWin[i].pKills = (GUIStatic*)pWnd->GetChild(_T("killnum"));
		m_PvPResultWin[i].pName = (GUIStatic*)pWnd->GetChild(_T("name"));
		m_PvPResultWin[i].pRongYu = (GUIStaticEx*)pWnd->GetChild(_T("honornum"));
		m_PvPResultWin[i].pRongYuChange = (GUIStaticEx*)pWnd->GetChild(_T("honoradd"));
		m_PvPResultWin[i].pSuccessRate = (GUIStaticEx*)pWnd->GetChild(_T("percentnum"));

	}

	m_pLeftNum = (GUIStatic*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\word0\\num0"));
	m_pLeftTime = (GUIStatic*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\word1\\num0"));

	m_pBnHide = (GUIButton*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\hide")); 

	m_pResultPic = (IconStatic*)m_pFinalPointsWin->GetChild(_T("compet_points_pic\\back\\personal_result\\resultpic"));


	
	// 最大化界面
	strPath = g_strLocalPath + _T("\\ui\\compet_state.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\numpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy0\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy1\\jobpicback\\jobpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy2\\jobpicback\\jobpic", "ClassName", "IconStatic");

	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy0\\hpback\\hpbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy1\\hpback\\hpbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "compet_state_win\\compet_state_pic\\back\\enemy2\\hpback\\hpbar", "ClassName", "ActiveProgress");
	m_pWndEnemyState = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pGUI->RegisterEventHandler(m_pWndEnemyState->GetFullName().c_str(), m_Trunk.sfp1(&PvPStateFrame::EventHandler));

	m_pNumLeftPic = (IconStatic*)m_pWndEnemyState->GetChild(_T("compet_state_pic\\numpic"));
	m_pBnMin = (GUIButton*)m_pWndEnemyState->GetChild(_T("compet_state_pic\\foldbutton"));
	for (int i = 0; i < ENEMY_NUM; i++)
	{

		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("compet_state_pic\\back\\enemy%d"),i );
		GUIStatic* pWnd = (GUIStatic*)m_pWndEnemyState->GetChild(tbuff);

		m_PVPEnemy[i].pPic = (IconStatic*)pWnd->GetChild(_T("jobpicback\\jobpic"));
		m_PVPEnemy[i].pName = (GUIStatic*)pWnd->GetChild(_T("name"));
		m_PVPEnemy[i].pHP = (ActiveProgress*)pWnd->GetChild(_T("hpback\\hpbar"));
		m_PVPEnemy[i].pAllBack = pWnd;
		m_PVPEnemy[i].pJobBack = (GUIStatic*)pWnd->GetChild(_T("jobpicback"));
		m_PVPEnemy[i].dwPlayerID = 0;

	}

	// 最小化界面
	strPath = g_strLocalPath + _T("\\ui\\compet_state_fold.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "competstate_fold_win\\competstate_fold_pic\\numpic", "ClassName", "IconStatic");
	m_pWndEnemyStateMin = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pGUI->RegisterEventHandler(m_pWndEnemyStateMin->GetFullName().c_str(), m_Trunk.sfp1(&PvPStateFrame::EventHandler));

	m_pBnMax= (GUIButton*)m_pWndEnemyStateMin->GetChild(_T("competstate_fold_pic\\butt2"));
	m_pNumPic = (IconStatic*)m_pWndEnemyStateMin->GetChild(_T("competstate_fold_pic\\numpic"));


	strPath = g_strLocalPath + _T("\\ui\\compet_kill.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_kill_win\\kills_pic\\numback_pic\\killnum0", "ClassName", "IconStatic");
	m_pWndKillNum = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pKillNumPic = (IconStatic*)m_pWndKillNum->GetChild(_T("kills_pic\\numback_pic\\killnum0"));


	m_pFinalPointsWin->SetInvisible(true);
	m_pWndEnemyState->SetInvisible(true);
	m_pWndEnemyStateMin->SetInvisible(true);
	m_pWndKillNum->SetInvisible(FALSE);
	m_pKillNumPic->SetInvisible(FALSE);
	m_pWndKill->SetInvisible(true);

	m_dwKillShowTime = 0;
	m_bShow = false;

	return TRUE;
}

VOID PvPStateFrame::Update()
{
	DWORD runTime = timeGetTime() - m_dwTime;
	if(runTime > QUIT_PVP_TIME)
	{
		/*m_pFinalPointsWin->SetInvisible(true);*/
		//m_pWndResult->SetInvisible(TRUE);
	}

	if (!m_pFinalPointsWin->IsInvisible())
	{
		tstringstream ts;
		ts << ((QUIT_PVP_TIME - runTime) / 1000 + 1);

		if (QUIT_PVP_TIME < runTime)
			m_pLeftTime->SetText(_T("1"));
		else
			m_pLeftTime->SetText(ts.str().c_str());
		
		m_pLeftTime->SetRefresh(TRUE);
	}
// 	if(!m_pWndResult->IsInvisible())
// 	{
// 		tstringstream ts;
// 		ts << ((QUIT_PVP_TIME - runTime) / 1000 + 1) << g_StrTable[_T("PvP_UI_RstTime")];
// 		m_pStaticTime->SetText(ts.str().c_str());
// 		m_pStaticTime->SetRefresh(TRUE);
// 	}
	
	if(P_VALID(m_pWndKillNum) && !m_pWndKillNum->IsInvisible())
	{
		if(timeGetTime() - m_dwStartToShow > SHOW_KILL_TIME)
		{
			m_pWndKillNum->SetInvisible(TRUE);
		}
	}

	if (m_bStartDeadCheck)
		DeadCheck();

}

DWORD PvPStateFrame::RstWndEventHandler(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		if((P_VALID(m_pBtnLeave) && pEvent->dwWndID == m_pBtnLeave->GetID()) || (P_VALID(m_pBtnClose) && pEvent->dwWndID == m_pBtnClose->GetID()))
		{
			m_pWndResult->SetInvisible(TRUE);
		}
		break;
	}
	return 0;
}

DWORD PvPStateFrame::OnCloseMap(tagGameEvent* pGameEvent)
{
	CountDownFrame* pCDFrame = (CountDownFrame*)m_pFrameMgr->GetFrame(_T("CountDown"));
	if( P_VALID(pCDFrame) )	pCDFrame->Hide();
	m_pWndResult->SetInvisible(TRUE);
	m_pFinalPointsWin->SetInvisible(true);
	m_pWndEnemyState->SetInvisible(true);
	m_pWndEnemyStateMin->SetInvisible(true);
	HidePvPState();
	HideKillUser();
	m_bStartDeadCheck = false;
	return 0;
}

DWORD PvPStateFrame::OnOpenMap(tagGameEvent* pGameEvent)
{
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();

	CountDownFrame* pCDFrame = (CountDownFrame*)m_pFrameMgr->GetFrame(_T("CountDown"));
	if( !P_VALID(pCDFrame) )
	{
		pCDFrame = (CountDownFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("CountDown"), _T("CountDownFrame"), 0);
		pCDFrame->ReloadUI(NULL);
	}
	if(P_VALID(pPlayer))
	{
		if(pPlayer->IsInState(ES_PvPAttack) || pPlayer->IsInState(ES_PvPDefence))
		{
			//if(P_VALID(m_pWndEnemy)) m_pWndEnemy->SetInvisible(FALSE);
			//if(P_VALID(m_pWndFriend)) m_pWndFriend->SetInvisible(FALSE);

			if(P_VALID(m_pWndEnemyState)) m_pWndEnemyState->SetInvisible(FALSE);
			if(P_VALID(m_pWndEnemyStateMin)) m_pWndEnemyStateMin->SetInvisible(true);

			if( P_VALID(pCDFrame) )
			{
				pCDFrame->Show();
				pCDFrame->SetCDTime(11 * 60 *1000);
			}

			return 0;
		}
	}
	if( P_VALID(pCDFrame) )	
	{
		pCDFrame->Hide();
	}
	m_pWndResult->SetInvisible(TRUE);
	m_pFinalPointsWin->SetInvisible(true);
	HidePvPState();
	HideKillUser();
	return 0;
}

// 处理显示战斗结果
DWORD PvPStateFrame::NetRecvResultInfo(tagNS_PvPResultInfo* pMsg, DWORD)
{
	//UpdateRstInfo(pMsg->pData);
	UpdateRanPvPRstInfo(pMsg->pData);
	m_dwTime = timeGetTime();
	m_pFinalPointsWin->SetInvisible(false);
	m_pFinalPointsWin->FlipToTop();
// 	m_pWndResult->SetInvisible(FALSE);
// 	m_pWndResult->FlipToTop();
	return 0;
}

// 处理战斗状态
DWORD PvPStateFrame::NetRecvPvPState(tagNS_PvPGetPvPState* pMsg, DWORD)
{
	if(!P_VALID(pMsg) || pMsg->dwGreenCount<0 || pMsg->dwRedCount<0) return GT_INVALID;
	ShowPvPState(pMsg->dwRedCount, pMsg->dwGreenCount);
	return 0;
} 

// 处理杀人提醒
DWORD PvPStateFrame::NetRecvNotifyKill(tagNS_PvPNotifyKill* pMsg, DWORD)
{
	if(!P_VALID(pMsg) || pMsg->dwKillCount < 0) return GT_INVALID;
	ShowKillUser(pMsg->dwKillCount);
	return 0;
} 

// 随机竞技场结果
DWORD PvPStateFrame::NetNS_PvPRanRoomResultInfo(tagNS_PvPRanRoomResultInfo* pMsg, DWORD)
{

	return 0;
}

void PvPStateFrame::UpdateRanPvPRstInfo(TCHAR* pMsg)
{
	tstring ts = pMsg;
	std::vector<int> tokenPos;
	tokenPos.push_back(-1);
	for(int i=0; i<(int)ts.size(); i++)
	{
		if(pMsg[i] == ',' || pMsg[i] == '，')
		{
			tokenPos.push_back(i);
		}
	}
	if(tokenPos.size() != 32) return;
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer)) return;
	BOOL bRed = pPlayer->IsInState(ES_PvPAttack);
	tokenPos.push_back(ts.size());
	GUIListBox* pList = NULL;
	INT roleID;
	INT index0 = 0, index1 = 0, index = 0;
	INT SucRate;
	for(size_t i=0; i<tokenPos.size()-3; i++)
	{
		INT pos = i%5;
		INT num = i/5;
		tstring v = ts.substr(tokenPos[i]+1, tokenPos[i+1]-tokenPos[i] - 1);
		switch (pos) 
		{
		case 0:
			{
				roleID = _ttoi(v.c_str());
				const TCHAR* pName = PlayerNameTab::Inst()->FindNameByID(roleID);
				if (P_VALID(pName))
				{
					m_PvPResultWin[num].pName->SetText(pName);
				}
				else{
					m_PvPResultWin[num].pName->SetText(PvPMgr::Inst()->GetRoleName(roleID).c_str());
				}			
			}
			break;
		case 1:
			{
				m_PvPResultWin[num].pRongYu->SetText(v.c_str());
			}
			break;
		case 2:
			{
				int nums = _ttoi(v.c_str());
				if (nums > 0)
				{
					tstring pluss = _T("+");
					v = pluss + v;
					m_PvPResultWin[num].pRongYuChange->SetText(v.c_str());
				}
				else if (nums == 0)
				{
					m_PvPResultWin[num].pRongYuChange->SetText(_T("-0"));
				}
				else
				{
					m_PvPResultWin[num].pRongYuChange->SetText(v.c_str());
				}
			}
			break;
		case 3:
			{
				m_PvPResultWin[num].pKills->SetText(v.c_str());
			}
			break;
		case 4:
			{
				TCHAR szText[64];
				SucRate = _ttoi(v.c_str());
				SucRate /= 100;
				_stprintf( szText, _T("%d%%"),  SucRate);
				m_PvPResultWin[num].pSuccessRate->SetText(szText);
			}
			break;
		}
	}

	tstring v1 = ts.substr(tokenPos[30]+1, tokenPos[31]-tokenPos[30] - 1); 
	switch (_ttoi(v1.c_str()))
	{
	case 0:
		m_pResultPic->SetPic(szLeave);
		break;
	case 1:
		m_pResultPic->SetPic(szSuccess);
		break;
	case 2:
		m_pResultPic->SetPic(szFail);
		break;
	case 3:
		m_pResultPic->SetPic(szPerfect);
		break;
	case 4:
		m_pResultPic->SetPic(szTie);
		break;
	}

	tstring v2 = ts.substr(tokenPos[31]+1, tokenPos[32]-tokenPos[31] - 1);
	m_pLeftNum->SetText(v2.c_str());


}

void PvPStateFrame::UpdateRstInfo(TCHAR* pMsg)
{
	tstring ts = pMsg;
	std::vector<int> tokenPos;
	tokenPos.push_back(-1);
	for(int i=0; i<(int)ts.size(); i++)
	{
		if(pMsg[i] == ',' || pMsg[i] == '，')
		{
			tokenPos.push_back(i);
		}
	}
	if(tokenPos.size() != 54) return;
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer)) return;
	BOOL bRed = pPlayer->IsInState(ES_PvPAttack);
	tokenPos.push_back(ts.size());
	GUIListBox* pList = NULL;
	INT roleID;
	INT index0 = 0, index1 = 0, index = 0;
	for(size_t i=0; i<tokenPos.size()-1; i++)
	{
		INT pos = i% 9;
		tstring v = ts.substr(tokenPos[i]+1, tokenPos[i+1]-tokenPos[i] - 1);
		switch (pos) 
		{
		case 0:
			roleID = _ttoi(v.c_str());
			pList = NULL;
			break;
		case 1:
			{
				switch(_ttoi(v.c_str()))
				{
				case 0:
					pList = bRed?m_pLstTeamRst:m_pLstEnemyRst;
					index = index0;
					index0++;
					break;
				case 1:
					pList = bRed?m_pLstEnemyRst:m_pLstTeamRst;
					index = index1;
					index1++;
					break;
				default:
					ASSERT(0);
					break;
				}
				if(!P_VALID(pList)) break;
				const TCHAR* pName = PlayerNameTab::Inst()->FindNameByID(roleID);
				pList->SetText(index, 0, P_VALID(pName)?pName:PvPMgr::Inst()->GetRoleName(roleID).c_str());
			}
			break;
		case 2:
			{
				if(!P_VALID(pList)) break;
				tstringstream ts;
				ts << _T("PvP_UI_WuXin") << _ttoi(v.c_str());
				pList->SetText(index, 1, g_StrTable[ts.str().c_str()]);
			}
			break;
		case 6:
			{
				if(!P_VALID(pList)) break;
				tstringstream ts;
				ts << _T("PvP_UI_FinalState") << _ttoi(v.c_str());
				pList->SetText(index, 5, g_StrTable[ts.str().c_str()]);
			}
			break;
		case 3:
		case 4:
		case 5:
			if(!P_VALID(pList)) break;
			pList->SetText(index, pos-1, v.c_str());
			break;
		case 7:
			if(pPlayer->GetID() == roleID)
			{
				if(P_VALID(m_pStaticGain)) 
				{
					m_pStaticGain->SetText(v.c_str());
					m_pStaticGain->SetRefresh(TRUE);
				}
			}
			break;
		case 8:
			if(pPlayer->GetID() == roleID)
			{
				if(P_VALID(m_pStaticTotal)) 
				{
					tstring temp = _T("/") + v;
					m_pStaticTotal->SetText(temp.c_str());
					m_pStaticTotal->SetRefresh(TRUE);
				}
			}
			break;
		}
	}
}

void PvPStateFrame::ShowPvPState(DWORD redCount, DWORD greenCount)
{
	tstringstream ts;
	ts << redCount;
	if(P_VALID(m_pStaticEnemy))
	{
		m_pStaticEnemy->SetText(ts.str().c_str());
		m_pStaticEnemy->SetRefresh(TRUE);		
		//m_pWndEnemy->SetInvisible(FALSE);	// 只在打开地图的时候显示
	}
	tstringstream ts2;
	ts2 << greenCount;
	if(P_VALID(m_pStaticFriend))
	{
		m_pStaticFriend->SetText(ts2.str().c_str());
		m_pStaticFriend->SetRefresh(TRUE);
		//m_pWndFriend->SetInvisible(FALSE);	// 只在打开地图的时候显示
	}
}

void PvPStateFrame::HidePvPState()
{
	if(P_VALID(m_pWndEnemy))
	{
		m_pWndEnemy->SetInvisible(TRUE);
	}

	if(P_VALID(m_pWndFriend))
	{
		m_pWndFriend->SetInvisible(TRUE);
	}
}

void PvPStateFrame::ShowKillUser(DWORD nNum)
{
	if(P_VALID(m_pWndKillNum) && P_VALID(m_pKillNumPic))
	{
		tstring strPath = _T(TEX_FILE_PATH_2);
		tagRect rect;
		switch(nNum)
		{
		case 1:
			rect = tagRect(76,4,147,96);	//(560, 360, 660, 600);
			break;
		case 2:
			rect = tagRect(149,4,220,96); //(740, 400, 920, 560);
			break;
		case 3:
			rect = tagRect(222,4,294,96); //(600, 665, 740, 800);
			break;
		default:
			return;
		}
		m_pKillNumPic->SetPic(strPath, rect);
		m_pKillNumPic->SetRefresh(TRUE);
		m_dwStartToShow = timeGetTime();
		m_pWndKillNum->SetInvisible(FALSE);
	}
}

void PvPStateFrame::HideKillUser()
{
	if(P_VALID(m_pWndKillNum)) 
		m_pWndKillNum->SetInvisible(TRUE);

	m_dwStartToShow = 0;
}

DWORD PvPStateFrame::EventHandler(tagGUIEvent* pEvent)
{

	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBnHide )
			{
				m_pFinalPointsWin->SetInvisible(true);
			}
			else if ( pWnd == m_pBnMin)	// 最小化
			{
				m_pWndEnemyState->SetInvisible(true);
				tagPoint pos1 = m_pWndEnemyState->GetClient();
				tagPoint pos2 = m_pWndEnemyStateMin->GetClient();
				tagPoint posChip = pos1 - pos2;
				m_pWndEnemyStateMin->Move(posChip);
				
				m_pWndEnemyStateMin->SetInvisible(false);
			}
			else if (pWnd == m_pBnMax)	// 最大化
			{
				m_pWndEnemyStateMin->SetInvisible(true); 
 				tagPoint pos1 = m_pWndEnemyState->GetClient();
 				tagPoint pos2 = m_pWndEnemyStateMin->GetClient();
 				tagPoint posChip = pos2 - pos1;
 				m_pWndEnemyState->Move(posChip);

				m_pWndEnemyState->SetInvisible(false);
				
			}

			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (	pWnd == m_PVPEnemy[i].pJobBack
					||	pWnd == m_PVPEnemy[i].pName
					||	pWnd == m_PVPEnemy[i].pHP
					||	pWnd == m_PVPEnemy[i].pAllBack
					||	pWnd == m_PVPEnemy[i].pPic	)
				{
					if (m_PVPEnemy[i].eState != EPPS_Alive)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_State_SwitchTarget_StateIllegal")]);
						break;
					}
					SelectPlayer(m_PVPEnemy[i].dwPlayerID);
					break;
				}
			}
		}
		break;
		
	}

	return 0;
}

DWORD PvPStateFrame::NetNS_PvPSinglePlayerInfo(tagNS_PvPSinglePlayerInfo* pMsg, DWORD)
{
	bool full = true;
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (P_VALID(m_PVPEnemy[i].dwPlayerID))	// 说明已经有人了
			continue;

		full = false;
		if(P_VALID(m_pWndEnemyState)) m_pWndEnemyState->SetInvisible(false);
		if(P_VALID(m_pWndEnemyStateMin)) m_pWndEnemyStateMin->SetInvisible(true);
		m_bStartDeadCheck = true;

		EPvPPlayerState eState = pMsg->eState;
		m_PVPEnemy[i].eState = eState;
		m_PVPEnemy[i].dwPlayerID = pMsg->dwPlayerID;

		tstring strPath = _T("data\\ui\\compet\\");
		//tagRect rect;

		int counter =0;
		for (int j =0; j < ENEMY_NUM ; j++)
		{
			if (m_PVPEnemy[j].eState == EPPS_Alive)
			counter++;
		}
		switch(counter)
		{
		case 0:
			strPath += _T("compet_empty.bmp");
			break;
		case 1:
			strPath += _T("one.bmp");
			break;
		case 2:
			strPath += _T("two.bmp");
			break;
		case 3:
			strPath += _T("three.bmp");
			break;
		default:
			break;
		}
		m_pNumLeftPic->SetPic(strPath);
		m_pNumPic->SetPic(strPath);

		// 说明找到了有空的地方可以插入
		RandomPvPFrame* pPvPFrame = (RandomPvPFrame*)m_pFrameMgr->GetFrame(_T("RandomPvP"));
		if( !P_VALID( pPvPFrame ) )
			break;

		EClassType eClass = pPvPFrame->GetPvPmemberClass(pMsg->dwPlayerID);
		tstring name = pPvPFrame->GetPvPmemberName(pMsg->dwPlayerID);


		if (EPPS_Alive == eState)	// 如果活着显示职业
		{
			switch (eClass)
			{
				case EV_Blade:
				case EV_Base:
				case EV_Null:
				case EV_Fuhu:
				case EV_XuanJia:
					m_PVPEnemy[i].pPic->SetPic(szShenBing);
					break;
				case EV_Bow:
				case EV_HuanShi:
				case EV_ZhanXin:
					m_PVPEnemy[i].pPic->SetPic(szXuanYu);
					break;
				case EV_Strength:
				case EV_LeiMang:
				case EV_GuaXiang:
					m_PVPEnemy[i].pPic->SetPic(szQianKun);
					break;
				case EV_Musician:
				case EV_ShenShi:
				case EV_TianYi:
					m_PVPEnemy[i].pPic->SetPic(szXianYin);
					break;
			}
			m_PVPEnemy[i].pName->SetTextColor(AliveColour);
		}
		else	// 其余的显示状态
		{
			switch (eState)
			{
			case EPPS_Leave:
				m_PVPEnemy[i].pPic->SetPic(szLeave);
				break;
			case EPPS_Dead:
				m_PVPEnemy[i].pPic->SetPic(szDead);
				m_PVPEnemy[i].pHP->SetRefresh(true);
				break;
			}
			m_PVPEnemy[i].pName->SetTextColor(GrayColour);
		}

		m_PVPEnemy[i].pName->SetText(name.c_str());


		break;
	}

	if (full)	// 人员已满（和策划没有沟通好，导致代码畸形。。。）
	{
		RandomPvPFrame* pPvPFrame = (RandomPvPFrame*)m_pFrameMgr->GetFrame(_T("RandomPvP"));
		if( !P_VALID( pPvPFrame ) )
			return 0;

		EClassType eClass = pPvPFrame->GetPvPmemberClass(pMsg->dwPlayerID);
		EPvPPlayerState eState = pMsg->eState;
		
		int num = 0;
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (m_PVPEnemy[i].dwPlayerID == pMsg->dwPlayerID)
			{
				m_PVPEnemy[i].eState = eState;
				num = i;
			}
		}

		if (EPPS_Alive == eState)	// 如果活着显示职业
		{
			//m_PVPEnemy[i].bDead = false;	// 针对在竞技场被复活的情况

			switch (eClass)
			{
			case EV_Blade:
			case EV_Base:
			case EV_Null:
			case EV_Fuhu:
			case EV_XuanJia:
				m_PVPEnemy[num].pPic->SetPic(szShenBing);
				break;
			case EV_Bow:
			case EV_HuanShi:
			case EV_ZhanXin:
				m_PVPEnemy[num].pPic->SetPic(szXuanYu);
				break;
			case EV_Strength:
			case EV_LeiMang:
			case EV_GuaXiang:
				m_PVPEnemy[num].pPic->SetPic(szQianKun);
				break;
			case EV_Musician:
			case EV_ShenShi:
			case EV_TianYi:
				m_PVPEnemy[num].pPic->SetPic(szXianYin);
				break;
			}
			m_PVPEnemy[num].pName->SetTextColor(AliveColour);
		}
		else	// 其余的显示状态
		{
			switch (eState)
			{
			case EPPS_Leave:
				{
					m_PVPEnemy[num].pPic->SetPic(szLeave);

					m_PVPEnemy[num].pHP->SetValue(0);
					m_PVPEnemy[num].pHP->SetMaxValue(m_PVPEnemy[num].nMaxHP);
					m_PVPEnemy[num].pHP->SetSpeed(10000000.0f);
					m_PVPEnemy[num].pHP->SetPicColor(0x00000000); 
					m_PVPEnemy[num].pHP->SetForePicColor(0xFFFF402D);
					TCHAR szTmp[X_SHORT_NAME] = {0};
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),0,m_PVPEnemy[num].nMaxHP);
					m_PVPEnemy[num].pHP->SetText(szTmp);
					m_PVPEnemy[num].pHP->SetRefresh(true);
				}
				break;
			case EPPS_Dead:
				m_PVPEnemy[num].pPic->SetPic(szDead);
				break;
			}

			m_PVPEnemy[num].pName->SetTextColor(GrayColour);
		}

		tstring strPath = _T("data\\ui\\compet\\");
		int counter =0;
		for (int j =0; j < ENEMY_NUM ; j++)
		{
			if (m_PVPEnemy[j].eState == EPPS_Alive)
				counter++;
		}
		switch(counter)
		{
		case 0:
			strPath += _T("compet_empty.bmp");
			break;
		case 1:
			strPath += _T("one.bmp");
			break;
		case 2:
			strPath += _T("two.bmp");
			break;
		case 3:
			strPath += _T("three.bmp");
			break;
		default:
			break;
		}
		m_pNumLeftPic->SetPic(strPath);
		m_pNumPic->SetPic(strPath);
	}
	

	return 0;
}

DWORD PvPStateFrame::NetRemoteAttChangeSingle(tagNS_RemoteAttChangeSingle* pNetCmd, DWORD)
{
	LocalPlayer* pSelf=RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pSelf))
		return 0;

	if (pNetCmd->eType == ERRA_HP || pNetCmd->eType == ERRA_MaxHP)	//如果是血量变化的信息
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (m_PVPEnemy[i].dwPlayerID == pNetCmd->dwRoleID)	// 如果是相关敌人的ID
			{
				if (pNetCmd->eType == ERRA_HP)
				{
					if (pNetCmd->nValue == 0)
						m_PVPEnemy[i].pHP->SetValue(1);
					else
						m_PVPEnemy[i].pHP->SetValue(pNetCmd->nValue);

					m_PVPEnemy[i].pHP->SetMaxValue(m_PVPEnemy[i].nMaxHP);
					m_PVPEnemy[i].pHP->SetSpeed(10000000.0f);
					m_PVPEnemy[i].pHP->SetPicColor(0x00000000); 
					m_PVPEnemy[i].pHP->SetForePicColor(0xFFFF402D);

					m_PVPEnemy[i].nCurHP = pNetCmd->nValue;

					TCHAR szTmp[X_SHORT_NAME] = {0};
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),pNetCmd->nValue,m_PVPEnemy[i].nMaxHP);
					m_PVPEnemy[i].pHP->SetText(szTmp);
					m_PVPEnemy[i].pHP->SetRefresh(true);
				}

				if (pNetCmd->eType == ERRA_MaxHP)
				{
					if (m_PVPEnemy[i].nCurHP == 0)
						m_PVPEnemy[i].pHP->SetValue(1);
					else
						m_PVPEnemy[i].pHP->SetValue(m_PVPEnemy[i].nCurHP);

					m_PVPEnemy[i].pHP->SetMaxValue(pNetCmd->nValue);
					m_PVPEnemy[i].pHP->SetSpeed(10000000.0f);
					m_PVPEnemy[i].pHP->SetPicColor(0x00000000); 
					m_PVPEnemy[i].pHP->SetForePicColor(0xFFFF402D);
					m_PVPEnemy[i].nMaxHP = pNetCmd->nValue;

					TCHAR szTmp[X_SHORT_NAME] = {0};
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),m_PVPEnemy[i].nCurHP,pNetCmd->nValue);
					m_PVPEnemy[i].pHP->SetText(szTmp);
					m_PVPEnemy[i].pHP->SetRefresh(true);

				}
			}
		}
	}

	return 0;
}

DWORD PvPStateFrame::NetNS_PvPEnterPvP(tagNS_PvPEnterPvP* pMsg, DWORD)
{

	// 清空临时数据
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if(P_VALID(m_PVPEnemy[i].pPic))
			m_PVPEnemy[i].pPic->SetPic(szLeave);

		m_PVPEnemy[i].dwPlayerID = 0;
		m_PVPEnemy[i].nCurHP = 0;
		m_PVPEnemy[i].nMaxHP = 0;
		m_PVPEnemy[i].eState = EPPS_Leave;
	}	

	if(P_VALID(m_pNumLeftPic))  m_pNumLeftPic->SetPic(szEmpty);
	if(P_VALID(m_pNumPic))  m_pNumPic->SetPic(szEmpty);

	return 0;
}

void PvPStateFrame::DeadCheck()
{	
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if(P_VALID(m_PVPEnemy[i].dwPlayerID) && m_PVPEnemy[i].eState == EPPS_Dead)	// 玩家存在
		{
			m_PVPEnemy[i].pHP->SetValue(0);
			m_PVPEnemy[i].pHP->SetMaxValue(m_PVPEnemy[i].nMaxHP);
			m_PVPEnemy[i].pHP->SetSpeed(10000000.0f);
			m_PVPEnemy[i].pHP->SetPicColor(0x00000000); 
			m_PVPEnemy[i].pHP->SetForePicColor(0xFFFF402D);
			TCHAR szTmp[X_SHORT_NAME] = {0};
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"),0,m_PVPEnemy[i].nMaxHP);
			m_PVPEnemy[i].pHP->SetText(szTmp);
			m_PVPEnemy[i].pHP->SetRefresh(true);
		}
	}	

}

VOID PvPStateFrame::SelectPlayer(DWORD dwPlayerID)
{
	if(!RoleMgr::Inst()->AutoPickRole(dwPlayerID))
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PVP_State_SwitchTarget_StateIllegal")]);
}