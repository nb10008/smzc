#include "stdafx.h"

#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "PvPFrame.h"
#include "PvPEvent.h"
#include "IconStatic.h"
#include "PvPRoomListItemCtrl.h"
#include "PvPRoomList.h"
#include "PvPMgr.h"
#include "PvPRoom.h"
#include "CombatSysUtil.h"
#include "StaticEx.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "PvPRoomPosCtrl.h"
#include "PvPRoomCtrl.h"
#include "QuestMgr.h"
#include "ButtonEx2.h"
#include "UIEvent.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "PlayerNameTab.h"
#include "RandomPvPFrame.h"

const tstring SELECT_ICON_PATH  =   _T("data\\ui\\Common\\L_icon-l2.bmp");
const tstring SELECT_DIBAN      =   _T("data\\ui\\sys\\l_diban-a.bmp");
#define DEF_LOWER_LEVEL 30
#define DEF_UPPER_LEVEL 100
#define WUXUN_LIMIT		600
#define NPC_ID			4006512
#define PREPARE_TIMEOUT	1000*10
#define ENTERPVP_TIMEOUT	1000*5

PvPFrame::PvPFrame()
:m_Trunk(this), m_curHall(E_PvP_RoomTypeNull), m_curLevel(E_PvP_RoomLevelNull)
{
	m_dwTime = 0;
	m_dwTimeDue = 1000; // 1 sec
	m_pWnd3v3Room = NULL;
	m_pWndHall = NULL;
	m_pWndCreate = NULL;
	m_iCurPage = 0;
	memset(m_pRoomCtrls, 0, sizeof(m_pRoomCtrls));

	m_bIsEnterring = FALSE;
	m_enterHallType = E_PvP_RoomTypeNull;
	m_iCurSelected = -1;

	m_bIsCreatingRoom = FALSE;

	m_bIsJoinRoom = FALSE;
	m_nJoinRoomID = -1;

	m_nRoomID = -1;

	m_uc3v3CurSelect = 0;
	m_bPreparing = FALSE;
	m_bUnPreparing = FALSE;

	m_b3v3IsSendingMsg = FALSE;

	m_dwNPCID = 0;

	m_bPositionFixed = FALSE;

	m_dwEnterPvPTimeOut = 0;

	m_dwLastInvite = 0;
}

PvPFrame::~PvPFrame()
{

}

BOOL PvPFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;	

	m_pCmdMgr->Register("NS_PvPEnterHall",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvEnterHall),	_T("NS_PvPEnterHall"));	
	m_pCmdMgr->Register("NS_PvPLeaveHall",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvLeaveHall),	_T("NS_PvPLeaveHall"));	
	m_pCmdMgr->Register("NS_PvPGetRoomList",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvRoomList),	_T("NS_PvPGetRoomList"));	
	m_pCmdMgr->Register("NS_PvPGetRoomInfo",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvRoomInfo),	_T("NS_PvPGetRoomInfo"));	
	m_pCmdMgr->Register("NS_PvPGetSimRoomInfo",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvSimRoomInfo),_T("NS_PvPGetSimRoomInfo"));
	m_pCmdMgr->Register("NS_PvPCreateRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvCreatRoom),	_T("NS_PvPCreateRoom"));	
	m_pCmdMgr->Register("NS_PvPJoinRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvJoinRoom),	_T("NS_PvPJoinRoom"));	
	m_pCmdMgr->Register("NS_PvPKickPlayer",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvKickPlayer),	_T("NS_PvPKickPlayer"));	
	m_pCmdMgr->Register("NS_PvPKicked",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvKicked),		_T("NS_PvPKicked"));
	m_pCmdMgr->Register("NS_PvPChangePosition",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvChangePos),	_T("NS_PvPChangePosition"));
	m_pCmdMgr->Register("NS_PvPPrepare",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvPrepare),	_T("NS_PvPPrepare"));	
	m_pCmdMgr->Register("NS_PvPUnPrepare",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvUnPrepare),	_T("NS_PvPUnPrepare"));	
	m_pCmdMgr->Register("NS_PvPLeaveRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvLeaveRoom),	_T("NS_PvPLeaveRoom"));	
	m_pCmdMgr->Register("NS_PvPStart",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvStart),		_T("NS_PvPStart"));	
	m_pCmdMgr->Register("NS_PvPStarted",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvStarted),	_T("NS_PvPStarted"));	
	m_pCmdMgr->Register("NS_PvPSetIntoRoom",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvSetInfoRoom),_T("NS_PvPSetIntoRoom"));	
	m_pCmdMgr->Register("NS_PvPInvite",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvInvite),		_T("NS_PvPInvite"));	
	m_pCmdMgr->Register("NS_PvPInviteJoin",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvInviteJoin),	_T("NS_PvPInviteJoin"));	
	m_pCmdMgr->Register("NS_PvPEnterPvP",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvEnterPvP),	_T("NS_PvPEnterPvP"));

	return TRUE;
}

BOOL PvPFrame::Destroy()
{
	for(int i=0; i<MAX_PVP_ROOM; i++)
		SAFE_DEL(m_pRoomCtrls[i]);

	for(int i=0; i<NUM_3V3; i++)
		SAFE_DEL(m_p3v3PosCtrl[i]);

	SAFE_DEL(m_p3v3RoomCtrl);

	if(P_VALID(m_pWndCreate))
		m_pGUI->AddToDestroyList(m_pWndCreate);

	if( P_VALID(m_pWnd3V3Folder) )
		m_pGUI->AddToDestroyList(m_pWnd3V3Folder);

	if( P_VALID(m_pWnd3v3Room) )
		m_pGUI->AddToDestroyList(m_pWnd3v3Room);

	if( P_VALID(m_pWndHall) )
		m_pGUI->AddToDestroyList(m_pWndHall);

	if(P_VALID(m_pWndInviteConfirm))
		m_pGUI->AddToDestroyList(m_pWndInviteConfirm);

	if(P_VALID(m_pWndRuleDescription))
		m_pGUI->AddToDestroyList(m_pWndRuleDescription);

	if(P_VALID(m_pWndEnterPvPTimeOut))
		m_pGUI->AddToDestroyList(m_pWndEnterPvPTimeOut);

	m_pMgr->UnRegisterEventHandler(_T("tagRoomListChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnRoomListChange));
	m_pMgr->UnRegisterEventHandler(_T("tagRoomChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnRoomChange));
	m_pMgr->UnRegisterEventHandler(_T("tagInvalidRoomEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnInvalidRoom));
	m_pMgr->UnRegisterEventHandler(_T("tagEnterHallEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnEnterHall));


	m_pCmdMgr->UnRegister("NS_PvPEnterHall",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvEnterHall));	
	m_pCmdMgr->UnRegister("NS_PvPLeaveHall",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvLeaveHall));	
	m_pCmdMgr->UnRegister("NS_PvPGetRoomList",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvRoomList));	
	m_pCmdMgr->UnRegister("NS_PvPGetRoomInfo",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvRoomInfo));	
	m_pCmdMgr->UnRegister("NS_PvPGetSimRoomInfo",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvSimRoomInfo));
	m_pCmdMgr->UnRegister("NS_PvPCreateRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvCreatRoom));	
	m_pCmdMgr->UnRegister("NS_PvPJoinRoom",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvJoinRoom));	
	m_pCmdMgr->UnRegister("NS_PvPKickPlayer",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvKickPlayer));	
	m_pCmdMgr->UnRegister("NS_PvPKicked",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvKicked));
	m_pCmdMgr->UnRegister("NS_PvPChangePosition",		(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvChangePos));	
	m_pCmdMgr->UnRegister("NS_PvPPrepare",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvPrepare));	
	m_pCmdMgr->UnRegister("NS_PvPUnPrepare",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvUnPrepare));	
	m_pCmdMgr->UnRegister("NS_PvPLeaveRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvLeaveRoom));	
	m_pCmdMgr->UnRegister("NS_PvPStart",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvStart));	
	m_pCmdMgr->UnRegister("NS_PvPStarted",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvStarted));	
	m_pCmdMgr->UnRegister("NS_PvPSetIntoRoom",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvSetInfoRoom));
	m_pCmdMgr->UnRegister("NS_PvPInvite",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvInvite));	
	m_pCmdMgr->UnRegister("NS_PvPInviteJoin",			(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvInviteJoin));	
	m_pCmdMgr->UnRegister("NS_PvPEnterPvP",				(NETMSGPROC)m_Trunk.sfp2(&PvPFrame::NetRecvEnterPvP));

	return GameFrame::Destroy();
}

BOOL PvPFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\compet_room.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_room_win", "AlignType", "NoAlign");
	for(int i=1; i<= MAX_PVP_ROOM; i++)
	{
		stringstream ts;
		ts << "compet_room_win\\compet_room_pic\\compet_room_pic\\compet_room" << i;
		string roomPath = ts.str();
		string statePath = roomPath + "\\pic";
		m_pGUI->ChangeXml( &ele, statePath.c_str(), "ClassName", "IconStatic");
		string imgPath = roomPath + "\\img";
		m_pGUI->ChangeXml( &ele, imgPath.c_str(), "ClassName", "IconStatic");
		string listItemPath = roomPath + "\\butt";
		m_pGUI->ChangeXml( &ele, listItemPath.c_str(), "ClassName", "ButtonEx2");
	}
	m_pWndHall = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	
	for(int i=1; i<= MAX_PVP_ROOM; i++)
	{
		tstringstream ts;
		ts << "compet_room_pic\\compet_room_pic\\compet_room" << i;
		tstring roomPath = ts.str();
		GUIPatch* pButton = (GUIPatch*)m_pWndHall->GetChild(roomPath.c_str());
		tstring posPath = roomPath + _T("\\num");
		GUIStatic* pPos = (GUIStatic*) m_pWndHall->GetChild(posPath.c_str());
		tstring levelPath = roomPath + _T("\\state");
		GUIStatic* pLevel = (GUIStatic*) m_pWndHall->GetChild(levelPath.c_str());
		tstring namePath = roomPath + _T("\\name");
		GUIStatic* pName = (GUIStatic*) m_pWndHall->GetChild(namePath.c_str());
		tstring numPath = roomPath + _T("\\family");
		GUIStatic* pNum = (GUIStatic*) m_pWndHall->GetChild(numPath.c_str());
		tstring statePath = roomPath + _T("\\pic");
		IconStatic* pState = (IconStatic*) m_pWndHall->GetChild(statePath.c_str());
		tstring imgPath = roomPath + _T("\\img");
		IconStatic* pImg = (IconStatic*) m_pWndHall->GetChild(imgPath.c_str());
		tstring limitPath = roomPath + _T("\\lev");
		GUIStatic* pLimit = (GUIStatic*) m_pWndHall->GetChild(limitPath.c_str());
		tstring backImgPath = roomPath + _T("\\lightpic");
		StaticEx* pBackImg = (StaticEx*) m_pWndHall->GetChild(backImgPath.c_str());
		m_pImgListItem[i-1] = pBackImg;
		tstring listItemPath = roomPath + _T("\\butt");
		m_pBtnListItem[i-1]  = (GUIButton*)m_pWndHall->GetChild(listItemPath.c_str());
		m_pRoomCtrls[i-1] = new RoomListItemCtrl(pButton, pPos, pLevel, pName, pNum, pState, pImg, pLimit, pBackImg);			
	}

	m_pStaticPageNum = (GUIStatic*)m_pWndHall->GetChild(_T("compet_room_pic\\num"));
	m_pBtnNextPage = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\nextbutt"));
	m_pBtnPrePage = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\backbutt"));
	m_pBtnRefresh = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\listbutt"));	
	m_pBtnClose = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\closebutt"));
	m_pBtnLeave = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\leave"));	
	m_pBtnCreate = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\creat"));	
	m_pBtnEnter = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\enter"));	
	m_pRadioFiler = (GUIPushButton*)m_pWndHall->GetChild(_T("compet_room_pic\\openbutt"));	
	m_pEdtSeach = (GUIEditBox*)m_pWndHall->GetChild(_T("compet_room_pic\\inputpic\\input"));	
	m_pBtnSearch = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\search"));	
	m_pTabJunior = (GUIPushButton*)m_pWndHall->GetChild(_T("compet_room_pic\\fra\\op1"));	
	m_pTabMid = (GUIPushButton*)m_pWndHall->GetChild(_T("compet_room_pic\\fra\\op2"));	
	m_pBtn3v3Rule = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\rules"));
	m_pBtn3v3AutoEnter = (GUIButton*)m_pWndHall->GetChild(_T("compet_room_pic\\autoenter"));

	m_pStaticPageNum->SetText(_T("1"));
	// tbd. disable search
	m_pWndHall->GetChild(_T("compet_room_pic\\inputpic"))->SetInvisible(TRUE);
	m_pWndHall->GetChild(_T("compet_room_pic\\word1"))->SetInvisible(TRUE);
	m_pBtnSearch->SetInvisible(TRUE);
	m_pWndHall->SetMovable(TRUE);
	tagPoint offSet = (tagPoint(m_pGUI->GetDesktop()->GetSize()) -tagPoint(m_pWndHall->GetSize()))/2 - m_pWndHall->GetView();
	m_pWndHall->Move(offSet);
	//m_pWndHall->SetMovable(FALSE);

	m_pWndHall->SetInvisible(TRUE);	
	m_pWndHall->FlipToTop();

	strPath = g_strLocalPath + _T("\\ui\\compet_creatroom.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_creatroom_win", "AlignType", "NoAlign");
	m_pWndCreate = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);	
	m_pWndCreate->SetInvisible(TRUE);
	m_pWndCreate->FlipToTop();
	m_pWndCreate->SetMovable(TRUE);
	offSet = (tagPoint(m_pGUI->GetDesktop()->GetSize()) -tagPoint(m_pWndCreate->GetSize()))/2 - m_pWndCreate->GetView();
	m_pWndCreate->Move(offSet);
	m_pWndCreate->SetMovable(FALSE);

	m_pEdtWuXun = (GUIEditBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word0\\comboback\\num"));
	m_pEdtLevelLower =(GUIEditBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word1\\comboback\\num"));
	m_pEdtLevelUpper = (GUIEditBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word2\\comboback\\num"));
	m_pLstWuXun = (GUIListBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word0\\comboback\\numlist"));	
	m_pLstLevelLower = (GUIListBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word1\\comboback\\numlist"));
	m_pLstLevelUpper = (GUIListBox*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\word2\\comboback\\numlist"));
	m_pBtnCreateRoom = (GUIButton*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\creatbutt"));
	m_pBtnCloseCreateWnd = (GUIButton*)m_pWndCreate->GetChild(_T("compet_creatroom_backpic\\closebutt"));

	for(int i=30, rowIndex=0; i<=100; i+=10, rowIndex++)
	{
		tstringstream ts;
		ts << i;
		m_pLstLevelLower->SetText(rowIndex, 0, ts.str().c_str());
		m_pLstLevelUpper->SetText(rowIndex, 0, ts.str().c_str());
	}
	tagPoint ptSizeListBox = m_pLstLevelLower->GetSize();
	ptSizeListBox.y = m_pLstLevelLower->GetRowHeight() * m_pLstLevelLower->GetTotalRowNum();
	m_pLstLevelLower->SetResizable(TRUE);
	m_pLstLevelLower->Resize(ptSizeListBox);
	m_pLstLevelLower->SetResizable(FALSE);
	ptSizeListBox = m_pLstLevelUpper->GetSize();
	ptSizeListBox.y = m_pLstLevelUpper->GetRowHeight() * m_pLstLevelUpper->GetTotalRowNum();
	m_pLstLevelUpper->SetResizable(TRUE);
	m_pLstLevelUpper->Resize(ptSizeListBox);
	m_pLstLevelUpper->SetResizable(FALSE);
	ele = XmlElement();
	ele.SetAttribute("ReadOnly","yes");
	m_pEdtLevelLower->GetVar()->Load(&ele);
	m_pEdtLevelUpper->GetVar()->Load(&ele);
	m_pEdtWuXun->GetVar()->Load(&ele);

	strPath = g_strLocalPath + _T("\\ui\\compet_junior_fold.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pWnd3V3Folder = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pBtn3v3UnFolder = (GUIButton*)m_pWnd3V3Folder->GetChild(_T("pvp_pic\\butt2"));
	GUIStatic* pFolderName = (GUIStatic*)m_pWnd3V3Folder->GetChild(_T("pvp_pic\\name"));
	m_pWnd3V3Folder->SetInvisible(TRUE);

	strPath = g_strLocalPath + _T("\\ui\\compet_junior.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "compet_junior_win", "AlignType", "NoAlign");
	for(int i=1; i<= NUM_3V3; i++)
	{
		stringstream ts;
		if(i<=NUM_3V3/2)
			ts << "compet_junior_win\\compet_junior_pic\\compet_junior_pic\\compet_junior" << i;
		else 
			ts << "compet_junior_win\\compet_junior_pic\\compet_junior_pic0\\compet_junior" << i-NUM_3V3/2;
		string roomPath = ts.str();
		string statePath = roomPath + "\\pic";
		m_pGUI->ChangeXml( &ele, statePath.c_str(), "ClassName", "IconStatic");
		string imgPath = roomPath + "\\word1";
		m_pGUI->ChangeXml( &ele, imgPath.c_str(), "ClassName", "IconStatic");
		string tabBtnPath =roomPath + "\\butt";
		m_pGUI->ChangeXml( &ele, tabBtnPath.c_str(), "ClassName", "ButtonEx2");
	}
	m_pWnd3v3Room = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWnd3v3Room->SetInvisible(TRUE);
	m_pWnd3v3Room->SetMovable(TRUE);
	offSet = (tagPoint(m_pGUI->GetDesktop()->GetSize()) -tagPoint(m_pWnd3v3Room->GetSize()))/2 - m_pWnd3v3Room->GetView();
	m_pWnd3v3Room->Move(offSet);
	m_pWnd3v3Room->FlipToTop();

	GUIPatch* p3v3Title = (GUIPatch*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic")); // 标题
	GUIStatic* pStatic3v3BtnRoomName = (GUIStatic*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\compet_room_pic00\\word0")); //name
	GUIStatic* pStatic3v3LevelLimit = (GUIStatic*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\compet_room_pic00\\num")); //等级 a级-b级
	GUIStatic* pStatic3v3WuxunLimit = (GUIStatic*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\compet_room_pic00\\word3")); //武勋
	m_pEdt3v3Friend = (GUIEditBox*) m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\inputpic\\input"));
	m_pCmb3v3FriendList = (GUIComboBox*) m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\comboback"));
	m_pBtn3v3Invide = (GUIButton*) m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\startbutt0"));
	m_pBtn3v3Kick = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\outbutt")); // 踢人
	m_pBtn3v3Prapare = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\preparebutt")); // 准备
	m_pBtn3v3Leave = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\leavebutt")); // 离开
	m_pBtn3v3Start = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\startbutt")); // 开始
	//m_pBtn3v3Close = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\closebutt")); // 关闭
	m_pBtn3v3Folder = (GUIButton*)m_pWnd3v3Room->GetChild(_T("compet_junior_pic\\butt2"));
	m_p3v3RoomCtrl = new PvPRoomCtrl(p3v3Title, pStatic3v3BtnRoomName,pStatic3v3LevelLimit, pStatic3v3WuxunLimit, m_pBtn3v3Kick, m_pBtn3v3Prapare, m_pBtn3v3Start, m_pBtn3v3Leave, pFolderName, m_pCmb3v3FriendList);
	m_pCmb3v3FriendList->GetListBox()->SetResizable(TRUE);
	m_pCmb3v3FriendList->GetListBox()->SetColWidth(0, m_pCmb3v3FriendList->GetListBox()->GetSize().x);
	m_pCmb3v3FriendList->GetListBox()->SetResizable(FALSE);
	m_pCmb3v3FriendList->GetEditBox()->SetInvisible(TRUE);

	for(int i=1; i<=NUM_3V3;i++)
	{
		tstringstream ts;
		if(i<=NUM_3V3/2)
			ts << "compet_junior_pic\\compet_junior_pic\\compet_junior" << i;
		else
			ts << "compet_junior_pic\\compet_junior_pic0\\compet_junior" << i-NUM_3V3/2;
		tstring posTabPath = ts.str();
		tstring posPath =posTabPath + _T("\\pic");
		IconStatic* pPos = (IconStatic*)m_pWnd3v3Room->GetChild(posPath.c_str()); // 位置图标 图 (IconStatic*)
		tstring statePath =posTabPath + _T("\\word1");
		IconStatic* pState = (IconStatic*)m_pWnd3v3Room->GetChild(statePath.c_str()); // 是否准备图标 图 (IconStatic*)
		tstring levelPath =posTabPath + _T("\\num0");
		GUIStatic* pLevel = (GUIStatic*)m_pWnd3v3Room->GetChild(levelPath.c_str()); //玩家等级
		tstring namePath =posTabPath + _T("\\word0");
		GUIStatic* pName = (GUIStatic*)m_pWnd3v3Room->GetChild(namePath.c_str()); //玩家姓名
		BYTE pos = i<=NUM_3V3/2?i*2-1:i*2-6;
		m_p3v3PosCtrl[i-1] = new PvPRoomPosCtrl(pPos, pState, pLevel, pName, pos);
		tstring tabBtnPath =posTabPath + _T("\\butt");
		m_pBtn3v3PosTab [i-1] = (GUIButton*)m_pWnd3v3Room->GetChild(tabBtnPath.c_str());
		tstring backImgPath =posTabPath + _T("\\lightpic");
		m_p3v3ImgListItem[i-1] = (StaticEx*)m_pWnd3v3Room->GetChild(backImgPath.c_str());
	}

	tstring ruleUIPath = g_strLocalPath + _T("\\ui\\msgbox3.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", ruleUIPath.c_str());
	m_pWndRuleDescription = m_pGUI->CreateWnd(_T("\\desktop"), &ele);	

	m_pBtnRuleDescOK = (GUIButton*)m_pWndRuleDescription->GetChild(_T("back\\ok"));
	m_pBtnRuleDescCancel = (GUIButton*)m_pWndRuleDescription->GetChild(_T("back\\cancel"));

	tagPoint p = m_pWndRuleDescription->GetSize();
	GUIStaticEx* descriptiontext = (GUIStaticEx*)m_pWndRuleDescription->GetChild(_T("back\\textback\\win\\text"));
	tstring desc = g_StrTable[_T("PvP_UI_Rule0")];
	desc += g_StrTable[_T("PvP_UI_Rule1")];
	desc += g_StrTable[_T("PvP_UI_Rule2")];
	descriptiontext->SetText(desc.c_str());
	tagPoint size = descriptiontext->GetSize();
	p.y -= size.y;
	descriptiontext->SetResizable(TRUE);
	tagPoint offset(0, descriptiontext->GetTextTotalHeight()-size.y);
	size.y = descriptiontext->GetTextTotalHeight();
	p.y += size.y;
	descriptiontext->Resize(size);
	descriptiontext->SetResizable(FALSE);
	/*m_pBtnRuleDescOK->SetMovable(TRUE);
	m_pBtnRuleDescOK->Move(offSet);
	m_pBtnRuleDescOK->SetMovable(FALSE);
	m_pBtnRuleDescCancel->SetMovable(TRUE);
	m_pBtnRuleDescCancel->Move(offSet);
	m_pBtnRuleDescCancel->SetMovable(FALSE);
	m_pWndRuleDescription->SetResizable(TRUE);
	m_pWndRuleDescription->Resize(p);
	m_pWndRuleDescription->SetResizable(FALSE);*/
	m_pWndRuleDescription->SetInvisible(TRUE);

	tstring invideConfirmUI = g_strLocalPath + _T("\\ui\\msgbox.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", invideConfirmUI.c_str());
	m_pWndInviteConfirm = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pBtnInviteOK = (GUIButton*)m_pWndInviteConfirm->GetChild(_T("back\\ok"));
	m_pBtnInviteCancel = (GUIButton*)m_pWndInviteConfirm->GetChild(_T("back\\cancel"));
	m_pBtnInviteOK->SetText(g_StrTable[_T("PvP_UI_InviteAccpet")]);
	m_pBtnInviteCancel->SetText(g_StrTable[_T("PvP_UI_InviteRefuse")]);
	m_pWndInviteConfirm->SetInvisible(TRUE);

	GUIWnd* pDesttop = m_pGUI->GetDesktop();

	tagPoint clientSize = pDesttop->GetSize();
	strPath = g_strLocalPath + _T("\\ui\\number.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "number_win\\num_pic", "ClassName", "IconStatic");
	m_pWndEnterPvPTimeOut = (GUIWindow*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pStaticEnterTimeOut = (IconStatic*)m_pWndEnterPvPTimeOut->GetChild(_T("num_pic"));
	m_pWndEnterPvPTimeOut->SetInvisible(TRUE);
	m_pWndEnterPvPTimeOut->SetUnClickable(TRUE);
	m_pWndEnterPvPTimeOut->FlipToTop();
	/*m_pWndEnterPvPTimeOut->SetResizable(TRUE);
	m_pWndEnterPvPTimeOut->SetMovable(TRUE);
	tagPoint s = (clientSize - m_pWndEnterPvPTimeOut->GetSize())/2 - m_pWndEnterPvPTimeOut->GetClient();
	m_pWndEnterPvPTimeOut->Move(s);
	m_pWndEnterPvPTimeOut->SetMovable(FALSE);
	m_pWndEnterPvPTimeOut->SetResizable(FALSE);*/

	//注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndHall->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::RoomListEventHandler));
	m_pGUI->RegisterEventHandler(m_pWndCreate->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::CreateRoomEventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd3v3Room->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::Room3v3EventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd3V3Folder->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::Room3v3EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndRuleDescription->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::RoomListEventHandler));
	m_pGUI->RegisterEventHandler(m_pWndInviteConfirm->GetFullName().c_str(), m_Trunk.sfp1(&PvPFrame::RoomListEventHandler));

	m_pMgr->RegisterEventHandle(_T("tagRoomListChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnRoomListChange));
	m_pMgr->RegisterEventHandle(_T("tagRoomChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnRoomChange));
	m_pMgr->RegisterEventHandle(_T("tagInvalidRoomEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnInvalidRoom));
	m_pMgr->RegisterEventHandle(_T("tagEnterHallEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PvPFrame::OnEnterHall));

	FixPostion();
	return TRUE;
}

// 调整窗口位置，避免遮住聊天窗口
void PvPFrame::FixPostion()
{
	if(m_bPositionFixed) return;
	GUIWnd* pWndChat = m_pGUI->GetWnd(_T("\\desktop\\new"));
	if(!pWndChat) return;
	m_bPositionFixed = TRUE;
	INT dxX0(0), dxX1(0);
	dxX0 = min(m_pWnd3v3Room->GetView().x - pWndChat->GetView().x- pWndChat->GetSize().x, dxX0);
	dxX0 = min(m_pWndCreate->GetView().x - pWndChat->GetView().x- pWndChat->GetSize().x, dxX0);
	dxX0 = min(m_pWndHall->GetView().x - pWndChat->GetView().x- pWndChat->GetSize().x, dxX0);
	if(dxX0 >= 0) return;
	dxX1 = m_pGUI->GetDesktop()->GetSize().x -  m_pWnd3v3Room->GetView().x- m_pWnd3v3Room->GetSize().x;
	dxX1 = min(m_pGUI->GetDesktop()->GetSize().x -  m_pWndCreate->GetView().x- m_pWndCreate->GetSize().x, dxX1);
	dxX1 = min(m_pGUI->GetDesktop()->GetSize().x -  m_pWndHall->GetView().x- m_pWndHall->GetSize().x, dxX1);
	if(dxX1 <=0 ) return;
	tagPoint offset(min(-dxX0, dxX1), 0);
	m_pWndHall->SetMovable(TRUE);
	m_pWndHall->Move(offset);
	//m_pWndHall->SetMovable(TRUE);
	m_pWndCreate->SetMovable(TRUE);
	m_pWndCreate->Move(offset);
	m_pWndCreate->SetMovable(FALSE);
	m_pWnd3v3Room->SetMovable(TRUE);
	m_pWnd3v3Room->Move(offset);
	//m_pWnd3v3Room->SetMovable(TRUE);
}

// 不断更新
VOID PvPFrame::Update()
{
	DWORD dwTime = Kernel::Inst()->GetDeltaTimeDW();
	if((P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible()) || (P_VALID(m_pWnd3V3Folder) && !m_pWnd3V3Folder->IsInvisible()))
	{
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_p3v3RoomCtrl->GetHallType(), m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
		if(P_VALID(pRoom) && pRoom->GetManagerID() != RoleMgr::Inst()->GetLocalPlayerID())
		{
			PvPRoom::PRoleInfo pRoleInfo = pRoom->GetRoleInfo(RoleMgr::Inst()->GetLocalPlayerID());
			if(P_VALID(pRoleInfo))
			{
				if(pRoleInfo->eState != E_PvP_Ready)				
				{					
					if(m_dwPrepareTimeOut < dwTime)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Msg_PrepareTimeOut")]);
						m_dwPrepareTimeOut = PREPARE_TIMEOUT;
					}
					else
					{
						m_dwPrepareTimeOut -= dwTime;
					}
				}
			}
		}
	}

	if(m_dwEnterPvPTimeOut > 0)
	{
		if(m_dwEnterPvPTimeOut > dwTime)
		{
			m_dwEnterPvPTimeOut -= dwTime;
			ShowEnterPvPPic(INT(m_dwEnterPvPTimeOut/1000));
		}
		else
		{
			m_dwEnterPvPTimeOut = 0;
			ShowEnterPvPPic(-1);
		}
	}
}

// 大厅界面控件事件处理
DWORD PvPFrame::RoomListEventHandler(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		if(P_VALID(m_pBtnRefresh) && pEvent->dwWndID == m_pBtnRefresh->GetID()) // 刷新
		{
			PvPRoomList* pList = PvPMgr::Inst()->GetRoomList(m_curHall, m_curLevel);
			if(P_VALID(pList)) pList->Update();
		}
		else if((P_VALID(m_pBtnClose) &&pEvent->dwWndID == m_pBtnClose->GetID()) || (P_VALID(m_pBtnLeave) && pEvent->dwWndID == m_pBtnLeave->GetID())) // 关闭
		{
			if( (!P_VALID(m_pWndCreate) || m_pWndCreate->IsInvisible()) && (!P_VALID(m_pWnd3v3Room) || m_pWnd3v3Room->IsInvisible()))
				LeaveHall();
		}
		else if(P_VALID(m_pBtnCreate) && pEvent->dwWndID == m_pBtnCreate->GetID()) // 创建房间
		{
			if(!m_pWndCreate->IsInvisible() || (P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())) break;
			ShowCreateRoomWnd();
		}
		else if(P_VALID(m_pBtnPrePage) && pEvent->dwWndID == m_pBtnPrePage->GetID()) // 向前翻页
		{
			if(m_iCurPage > 0)
			{
				SetCurrentPage(m_iCurPage-1);
			}
		}
		else if(P_VALID(m_pBtnNextPage) && pEvent->dwWndID == m_pBtnNextPage->GetID()) // 向后翻页
		{
			PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, m_curLevel);
			if(P_VALID(pRoomList))
			{
				std::list<INT16> idList = pRoomList->GetOrderedRoomList();
				if(m_pRadioFiler->GetState() == EGUIBS_PushDown)
					idList = FilterRooms(idList);
				INT total = idList.size();
				INT totalPage = INT((total -1) /MAX_PVP_ROOM) + 1;
				if(m_iCurPage + 1< totalPage)
				{
					SetCurrentPage(m_iCurPage+1);
				}
			}
		}
		else if(P_VALID(m_pBtnEnter) && pEvent->dwWndID == m_pBtnEnter->GetID()) // 加入房间
		{
			if(!m_pWndCreate->IsInvisible()) break;
			if(m_iCurSelected>=0)
			{
				if(P_VALID(m_pRoomCtrls[m_iCurSelected]) && m_pRoomCtrls[m_iCurSelected]->GetRoomID()>=0)
				{
					JoinRoom(m_curLevel, m_pRoomCtrls[m_iCurSelected]->GetRoomID());
				}
				else
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L2")]);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L1")]);
			}
		}
		else if(P_VALID(m_pBtnSearch) && pEvent->dwWndID == m_pBtnSearch->GetID()) // 查询
		{
			// tbd
		}
		else if(P_VALID(m_pBtn3v3AutoEnter) && pEvent->dwWndID == m_pBtn3v3AutoEnter->GetID()) // 自动加入
		{
			AutoJoin(m_curLevel);
		}
		else if(P_VALID(m_pBtn3v3Rule) && pEvent->dwWndID == m_pBtn3v3Rule->GetID()) // 竞技规则
		{
			if(P_VALID(m_pWndRuleDescription))
			{
				m_pWndRuleDescription->SetInvisible(FALSE);
				m_pWndRuleDescription->FlipToTop();
			}
		}
		else if((P_VALID(m_pBtnRuleDescOK) && pEvent->dwWndID == m_pBtnRuleDescOK->GetID()) || (P_VALID(m_pBtnRuleDescCancel) && pEvent->dwWndID == m_pBtnRuleDescCancel->GetID())) // 自动加入
		{
			if(P_VALID(m_pWndRuleDescription))
			{
				m_pWndRuleDescription->SetInvisible(TRUE);
			}
		}
		else if(P_VALID(m_pBtnInviteOK) && pEvent->dwWndID == m_pBtnInviteOK->GetID()) // 加入
		{
			RefuseJoin(m_eInvitedRoomType, m_eInvitedLevel, m_nInvitedRoomID, m_dwInvitorID, FALSE);
			m_pWndInviteConfirm->SetInvisible(TRUE);
			//JoinRoom(m_eInvitedLevel, m_iInvitedRoomID);
		}
		else if(P_VALID(m_pBtnInviteCancel) && pEvent->dwWndID == m_pBtnInviteCancel->GetID()) // 决绝
		{
			RefuseJoin(m_eInvitedRoomType, m_eInvitedLevel, m_nInvitedRoomID, m_dwInvitorID);
			m_pWndInviteConfirm->SetInvisible(TRUE);
		}
		else
		{
			for(int i = 0; i<MAX_PVP_ROOM; i++)
			{
				if(P_VALID(m_pBtnListItem[i]) && m_pBtnListItem[i]->GetID() == pEvent->dwWndID) // 房间控件
				{
					if(P_VALID(m_pRoomCtrls[i]) && m_pRoomCtrls[i]->GetRoomID()>=0)
					{
						SetCurSelect(i);
					}
					break;
				}

			}
		}
		break;
	case EGUIE_PushButtonDown:
		if(P_VALID(m_pTabJunior) && pEvent->dwWndID == m_pTabJunior->GetID()) // 选择级别
		{
			if(m_bIsJoinRoom || m_bIsCreatingRoom) 
			{
				switch(m_curLevel)
				{
				case E_PvP_Junior:
					if(m_pTabJunior->GetState() != EGUIBS_PushDown)
					{
						m_pTabJunior->SetState(EGUIBS_PushDown, FALSE);
						m_pTabMid->SetState(EGUIBS_PopUp, FALSE);
					}
					break;
				case E_PvP_Middle:
					if(m_pTabMid->GetState() != EGUIBS_PushDown)
					{
						m_pTabJunior->SetState(EGUIBS_PopUp, FALSE);
						m_pTabMid->SetState(EGUIBS_PushDown, FALSE);
					}
					break;
				}
				break;
			}
			if(m_curLevel != E_PvP_Junior)
			{
				if(! PvPMgr::Inst()->CanEnterHall(E_PvP_Junior)) break;
				PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, E_PvP_Junior);
				if(P_VALID(pRoomList))
				{
					m_curLevel = E_PvP_Junior;
					if(!pRoomList->IsInitialized()) pRoomList->Update();
					SetCurrentPage(0);
				}
			}
		}
		else if(P_VALID(m_pTabMid) && pEvent->dwWndID == m_pTabMid->GetID())
		{
			if(m_bIsJoinRoom || m_bIsCreatingRoom)
			{				
				switch(m_curLevel)
				{
				case E_PvP_Junior:
					if(m_pTabJunior->GetState() != EGUIBS_PushDown)
					{
						m_pTabJunior->SetState(EGUIBS_PushDown, FALSE);
						m_pTabMid->SetState(EGUIBS_PopUp, FALSE);
					}
					break;
				case E_PvP_Middle:
					if(m_pTabMid->GetState() != EGUIBS_PushDown)
					{
						m_pTabJunior->SetState(EGUIBS_PopUp, FALSE);
						m_pTabMid->SetState(EGUIBS_PushDown, FALSE);
					}
					break;
				}
				break;
			}
			if(m_curLevel != E_PvP_Middle)
			{
				if(! PvPMgr::Inst()->CanEnterHall(E_PvP_Middle)) break;
				PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, E_PvP_Middle);
				if(P_VALID(pRoomList))
				{
					m_curLevel = E_PvP_Middle;
					pRoomList->Update();
					SetCurrentPage(0);
				}
			}
		}
		else if(P_VALID(m_pRadioFiler) && pEvent->dwWndID == m_pRadioFiler->GetID()) // 过滤房间
		{
			SetCurrentPage(0);
		}
		break;
	case EGUIE_DoubleClick:
		for(int i = 0; i<MAX_PVP_ROOM; i++)
		{
			if(P_VALID(m_pBtnListItem[i]) && m_pBtnListItem[i]->GetID() == pEvent->dwWndID) // 房间控件
			{
				if(P_VALID(m_pRoomCtrls[i]) && m_pRoomCtrls[i]->GetRoomID()>=0)
				{
					SetCurSelect(i);
					JoinRoom(m_curLevel, m_pRoomCtrls[i]->GetRoomID());
				}
				break;
			}

		}
		break;
	}
	return 0;
}


// 创建房间界面事件处理
DWORD PvPFrame::CreateRoomEventHandler(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_ItemClick:
		if(P_VALID(m_pLstWuXun) && pEvent->dwWndID == m_pLstWuXun->GetID()) // 选择武勋级别
		{
			m_pEdtWuXun->SetText(m_pLstWuXun->GetText(pEvent->dwParam1, 0));
		}
		else if(P_VALID(m_pLstLevelLower) && pEvent->dwWndID == m_pLstLevelLower->GetID()) // 选择等级最低限制
		{
			m_pEdtLevelLower->SetText(m_pLstLevelLower->GetText(pEvent->dwParam1, 0));
		}
		else if(P_VALID(m_pLstLevelUpper) && pEvent->dwWndID == m_pLstLevelUpper->GetID()) // 选择等级最高限制
		{
			m_pEdtLevelUpper->SetText(m_pLstLevelUpper->GetText(pEvent->dwParam1, 0));
		}
		break;
	case EGUIE_Click:
		if(P_VALID(m_pBtnCreateRoom) && pEvent->dwWndID == m_pBtnCreateRoom->GetID()) // 创建房间
		{			
			if(m_bIsCreatingRoom) break;
			INT lowerLevel = DEF_LOWER_LEVEL; // 默认值
			INT upperLevel = DEF_UPPER_LEVEL; // 默认值
			DWORD wuXin = (DWORD)m_pLstWuXun->GetCurSelectedRow()+1; // 武勋等级

			if(wcslen(m_pEdtLevelLower->GetText()) != 0)
				lowerLevel = _wtoi(m_pEdtLevelLower->GetText());
			if(wcslen(m_pEdtLevelUpper->GetText()) != 0)
				upperLevel = _wtoi(m_pEdtLevelUpper->GetText());

			if(lowerLevel > upperLevel) // 非法
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L3")]);
				break;
			}
			LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
			ASSERT(P_VALID(pPlayer));
			INT level = pPlayer->GetRoleLevel();
			DWORD wuXin2 = PvPMgr::Inst()->Wuxun2Level(pPlayer->GetAttribute(ERA_WuXun));
			if(lowerLevel <= level && upperLevel>=level && wuXin<=wuXin2) // 判断自身是否符合
			{
				CreateRoom(m_curLevel, upperLevel, lowerLevel, wuXin);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L4")]);
				break;
			}
		}
		else if(P_VALID(m_pBtnCloseCreateWnd) && pEvent->dwWndID == m_pBtnCloseCreateWnd->GetID())
		{
			if(P_VALID(m_pWndCreate)) m_pWndCreate->SetInvisible(TRUE);
			m_pWndHall->SetUnClickable(FALSE);
			m_bIsCreatingRoom = FALSE;
			//m_pGUI->SetModal(m_pWndHall);
		}
		break;
	case EGUIE_KeyDown:
		break;
	}
	return 0;
}

// 房间界面控件事件处理
DWORD PvPFrame::Room3v3EventHandler(tagGUIEvent* pEvent)
{
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		if(P_VALID(m_pBtn3v3Kick) && pEvent->dwWndID == m_pBtn3v3Kick->GetID())	// 踢人
		{
			INT curIndex = (m_uc3v3CurSelect%2)==0? (m_uc3v3CurSelect+6)/2-1:(m_uc3v3CurSelect+1)/2-1;
			if(m_uc3v3CurSelect>0 && m_p3v3PosCtrl[curIndex]->GetRoleID()>0)
			{
				KickPlayer(m_p3v3PosCtrl[curIndex]->GetRoleID());
			}
		}
		else if(P_VALID(m_pBtn3v3Prapare) && pEvent->dwWndID == m_pBtn3v3Prapare->GetID()) //准备
		{
			switch(m_p3v3RoomCtrl->GetPrepareState())
			{
			case PvPRoomCtrl::E_Prepared:
				m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Unprepring);
				UnPrepare();
				break;
			case PvPRoomCtrl::E_Unprepared:
				m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Preparing);
				Prepare();					
				break;
			}
		}
		else if((P_VALID(m_pBtn3v3Leave) && pEvent->dwWndID == m_pBtn3v3Leave->GetID())) // 离开
		{
			LeaveRoom();
		}
		else if((P_VALID(m_pBtn3v3Folder) && pEvent->dwWndID == m_pBtn3v3Folder->GetID())) // 离开
		{
			FolderRoom();
		}
		else if(P_VALID(m_pBtn3v3Start) && pEvent->dwWndID == m_pBtn3v3Start->GetID()) // 开始
		{
			if(m_p3v3RoomCtrl->IsMyRoom())
			{
				Start();
			}
		}
		else if(P_VALID(m_pBtn3v3UnFolder) && pEvent->dwWndID == m_pBtn3v3UnFolder->GetID()) // 展开
		{
			FolderRoom(FALSE);
		}
		else if(P_VALID(m_pBtn3v3Invide) && pEvent->dwWndID == m_pBtn3v3Invide->GetID()) // 邀请玩家
		{
			LPCTSTR pFriend = m_pEdt3v3Friend->GetText();
			if(P_VALID(pFriend) && _tcslen(pFriend) > 0)
			{
				InviteFriend(pFriend);
			}
		}else if(P_VALID(m_pCmb3v3FriendList) && P_VALID(m_pCmb3v3FriendList->GetButton()) && m_pCmb3v3FriendList->GetButton()->GetID() == pEvent->dwWndID)
		{
			m_p3v3RoomCtrl->OnFriendChange();
		}
		else
		{
			for(BYTE i=0; i<NUM_3V3; i++)
			{
				if(P_VALID(m_pBtn3v3PosTab[i]) && pEvent->dwWndID == m_pBtn3v3PosTab[i]->GetID()) // 在位置上面单击
				{
					if(m_p3v3PosCtrl[i]->GetRoleID()>0) // 有人
					{
						if(m_p3v3RoomCtrl->IsMyRoom())
							SetCurSelectPos(m_p3v3PosCtrl[i]->GetPos());
					}
					else
					{
						ChangePos(m_p3v3PosCtrl[i]->GetPos());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_RightClick:
		if(m_p3v3RoomCtrl->GetLevel() == E_PvP_Middle) // 屏蔽随机竞技场的右键菜单
		{
			for(BYTE i=0; i<NUM_3V3; i++)
			{
				if(P_VALID(m_pBtn3v3PosTab[i]) && pEvent->dwWndID == m_pBtn3v3PosTab[i]->GetID()) // 在位置上面单击
				{
					if(m_p3v3PosCtrl[i]->GetRoleID()>0) // 有人
					{
						if(m_p3v3PosCtrl[i]->GetRoleID() != RoleMgr::Inst()->GetLocalPlayerID() )
						{						
							PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_p3v3PosCtrl[i]->GetHallType(), m_p3v3PosCtrl[i]->GetLevel(), m_p3v3PosCtrl[i]->GetRoomID());
							if(P_VALID(pRoom))
							{
								PvPRoom::PRoleInfo pRoleInfo = pRoom->GetRoleInfoAtPos(m_p3v3PosCtrl[i]->GetPos());
								if(P_VALID(pRoleInfo) && pRoleInfo->dwRoleID == m_p3v3PosCtrl[i]->GetRoleID())
								{
									tagPvPRoomMenuEvent evt;
									evt.pt = pEvent->msg.pt;
									evt.dwRoleID = pRoleInfo->dwRoleID;
									memcpy(evt.szName, pRoleInfo->szName, X_SHORT_NAME);
									m_pMgr->SendEvent(&evt);
								}
							}						
						}
					}
					break;
				}
			}
		}
		break;
	case EGUIE_ItemClick:
		if(P_VALID(m_pCmb3v3FriendList) && pEvent->dwWndID == m_pCmb3v3FriendList->GetListBox()->GetID()) // 选择武勋级别
		{
			m_pEdt3v3Friend->SetText( m_pCmb3v3FriendList->GetListBox()->GetText(pEvent->dwParam1, 0));
			m_pCmb3v3FriendList->GetEditBox()->SetText(_T(""));
		}
		break;
	}

	return 0;
}

// 设置当前选中的座位 （房间界面）
void PvPFrame::SetCurSelectPos(BYTE pos)
{
	INT index = (pos%2)==0? (pos+6)/2-1:(pos+1)/2-1; // 位置转为控件索引
	INT curIndex = (m_uc3v3CurSelect%2)==0? (m_uc3v3CurSelect+6)/2-1:(m_uc3v3CurSelect+1)/2-1;  // 位置转为控件索引
	if(index>=0 && P_VALID(m_p3v3PosCtrl[index]) && m_p3v3PosCtrl[index]->GetRoleID()>0)
	{
		if(m_uc3v3CurSelect!= pos)
		{
			if(m_uc3v3CurSelect > 0) // 需取消原来选中
				m_p3v3ImgListItem[curIndex]->SetPicFile(SELECT_DIBAN);	
			m_uc3v3CurSelect = pos;
			m_p3v3ImgListItem[index]->SetPicFile(SELECT_ICON_PATH);
		}		
	}
	else 
	{
		if(m_uc3v3CurSelect > 0) // 需取消原来选中
			m_p3v3ImgListItem[curIndex]->SetPicFile(SELECT_DIBAN);
		m_uc3v3CurSelect = 0;
	}
}

// 打开房间界面
void PvPFrame::ShowCreateRoomWnd()
{
	if(m_curHall == E_PvP_RoomTypeNull || m_curLevel == E_PvP_RoomLevelNull) return;
	if(!P_VALID(m_pWndCreate)) return;

	// 更新可选的武勋级别
	if(P_VALID(m_pLstWuXun))
	{
		tagRect rcRect(0, 0, 22, 22);
		m_pLstWuXun->Clear();		
		std::list<INT> imgIndex;
		switch(m_curLevel)
		{
		case E_PvP_Middle:
		case E_PvP_Junior:
			imgIndex.push_back(4);						
			imgIndex.push_back(5);
			imgIndex.push_back(6);			
			imgIndex.push_back(1);
			imgIndex.push_back(2);
			imgIndex.push_back(3);			
			// fall though
		case E_PvP_Senior:
			imgIndex.push_back(7);
			imgIndex.push_back(8);
			imgIndex.push_back(9);
			imgIndex.push_back(10);
			break;
		}		
		tagPoint ptSizeListBox = m_pLstWuXun->GetSize();
		ptSizeListBox.y = m_pLstWuXun->GetRowHeight() * imgIndex.size();
		m_pLstWuXun->SetResizable(TRUE);
		m_pLstWuXun->Resize(ptSizeListBox);
		m_pLstWuXun->SetResizable(FALSE);

		imgIndex.sort();
		std::list<INT>::iterator itor = imgIndex.begin();
		for(int nRow=0; itor!= imgIndex.end(); itor++, nRow++)
		{
			tstringstream ts;
			ts << _T("PvP_UI_WuXin") << *itor;
			m_pLstWuXun->SetText(nRow, 0, g_StrTable[ts.str().c_str()]);
		}
		m_pLstWuXun->SetColWidth(0, m_pLstWuXun->GetSize().x);
		m_pLstWuXun->SetCurSelectedRow(0);
		m_pEdtWuXun->SetText(m_pLstWuXun->GetText(0, 0));
		m_pEdtWuXun->SetRefresh();
	}
	if(m_curLevel ==  E_PvP_Junior) // 随机竞技场限制级别
	{
		int nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
		INT lowLevel = max(INT((nLevel-1) /10)*10+1, 30);
		if(lowLevel == 31) lowLevel = 30;
		tstringstream ts;
		ts << lowLevel;
		m_pEdtLevelLower->SetText(ts.str().c_str());
		ts.str(_T(""));
		ts << min(max(INT((nLevel-1) /10 +1)*10, 40), 100);
		m_pEdtLevelUpper->SetText(ts.str().c_str());
		m_pLstLevelLower->GetFather()->SetUnClickable(TRUE);
		m_pEdtLevelUpper->GetFather()->SetUnClickable(TRUE);
	}
	else
	{
		m_pEdtLevelLower->SetText(_T(""));		
		m_pEdtLevelUpper->SetText(_T(""));
		m_pLstLevelLower->GetFather()->SetUnClickable(FALSE);
		m_pEdtLevelUpper->GetFather()->SetUnClickable(FALSE);
	}
	m_pWndCreate->SetInvisible(FALSE);
	m_pWndCreate->FlipToTop();
	m_pWndHall->SetUnClickable(TRUE);
	//m_pGUI->SetModal(m_pWndCreate);
}

// 房间信息（大厅和房间）
DWORD PvPFrame::OnRoomChange(tagRoomChange* pGameEvent)
{
	// 大厅
	if(P_VALID(m_pWndHall) && !m_pWndHall->IsInvisible())
	{
		for(int i=0; i< MAX_PVP_ROOM; i++)
		{
			if(P_VALID(m_pRoomCtrls[i]))
				m_pRoomCtrls[i]->OnUpdate(pGameEvent);
		}
		if(m_pRadioFiler->GetState() == EGUIBS_PushDown) // 如果开启过滤，房间可能会因为刷新而改变过滤结果
			SetCurrentPage(m_iCurPage);
	}

	// 房间
	if(P_VALID(m_pWnd3v3Room))// && !m_pWnd3v3Room->IsInvisible())
	{
		m_p3v3RoomCtrl->OnUpdate(pGameEvent); // 更新房主
		for(int i=0; i< NUM_3V3; i++)
		{
			m_p3v3PosCtrl[i]->OnUpdate(pGameEvent); // 更新每个位置
		}
		SetCurSelectPos(m_uc3v3CurSelect); // 可能位置变化，导致没有当前选中
		
		if(m_dwEnterPvPTimeOut>0) // 取消房间倒计时
		{
			if(pGameEvent->eHallType == m_p3v3RoomCtrl->GetHallType() && pGameEvent->eLevel == m_p3v3RoomCtrl->GetLevel() && pGameEvent->nRoomID == m_p3v3RoomCtrl->GetRoomID())
			{				
				m_dwEnterPvPTimeOut = 0;
				ShowEnterPvPPic(-1);
			}
		}
	}

	return 0;
}

// 处理房间列表变化消息， 并重新刷新超过3秒未更新过的房间（大厅）
DWORD PvPFrame::OnRoomListChange(tagRoomListChange* pGameEvent)
{
	if(m_pWndHall->IsInvisible() || m_curHall == E_PvP_RoomTypeNull || m_curLevel == E_PvP_RoomLevelNull) return 0; // not visable;
	if(pGameEvent->eHallType== m_curHall &&pGameEvent->eLevel == m_curLevel)
	{
		SetCurrentPage(m_iCurPage);	// 刷新当前页
		std::vector<INT16> roomIDs;
		for(int i=0; i< MAX_PVP_ROOM; i++)
		{
			if(!P_VALID(m_pRoomCtrls[i]) || m_pRoomCtrls[i]->GetRoomID()<0) continue;
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_pRoomCtrls[i]->GetHallType(), m_pRoomCtrls[i]->GetLevel(), m_pRoomCtrls[i]->GetRoomID());
			if(P_VALID(pRoom))
			{
				if(pRoom->NeedUpdate()) // 超过3秒未更新
				{
					roomIDs.push_back(pRoom->GetRoomID());
					pRoom->RefreshUpdateTime();
				}
			}
		}
		if(roomIDs.size()>0)
		{
			tagNC_PvPGetSimRoomInfo msg;
			tagNC_PvPGetSimRoomInfo* pSend = (tagNC_PvPGetSimRoomInfo*)new BYTE[sizeof(tagNC_PvPGetSimRoomInfo) + sizeof(INT16)*(roomIDs.size()-1)];
			if(P_VALID(pSend))
			{
				memcpy(pSend, &msg, sizeof(tagNC_PvPGetSimRoomInfo));
				pSend->dwSize = sizeof(tagNC_PvPGetSimRoomInfo) + sizeof(INT16)*(roomIDs.size()-1);
				pSend->eRoomLevel = m_curLevel;
				pSend->dwCount = roomIDs.size();
				for(DWORD i=0; i<pSend->dwCount; i++)
				{
					pSend->pData[i] = roomIDs[i];
				}
				m_pSession->Send(pSend);
			}
			SAFE_DEL_ARRAY(pSend);
		}
	}
	return 0;
}

// 处理一组房间被关闭的信息,更新大厅和房间界面
DWORD PvPFrame::OnInvalidRoom(tagInvalidRoomEvent* pGameEvent)
{
	SetCurrentPage(m_iCurPage);	// 更新当前页（可能有房间就在当前页）
	if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible()) 
	{
		if(m_p3v3RoomCtrl->GetHallType() == pGameEvent->eHallType && m_p3v3RoomCtrl->GetLevel() == pGameEvent->eLevel)
		{
			for(std::list<INT16>::const_iterator itor=pGameEvent->nRoomIDs.begin();itor!=pGameEvent->nRoomIDs.end();itor++)
			{
				if(*itor == m_p3v3RoomCtrl->GetRoomID())
				{
					LeaveRoom(); // 发送离开房间消息
					// 立即关闭房间界面
					m_pWnd3v3Room->SetInvisible(TRUE);
					m_pWnd3V3Folder->SetInvisible(TRUE);
					m_pWndHall->SetUnClickable(FALSE);
					m_pWndHall->SetInvisible(FALSE);
					//m_pGUI->SetModal(m_pWndHall);
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L5")]);
					break;
				}
			}
		}
	}
	return 0;
}

// 翻页（大厅界面）
void PvPFrame::SetCurrentPage(INT page)
{
	PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, m_curLevel);
	if(P_VALID(pRoomList))
	{
		std::list<INT16> idList = pRoomList->GetOrderedRoomList();
		if(m_pRadioFiler->GetState() == EGUIBS_PushDown)
			idList = FilterRooms(idList);	// 过滤
		INT total = idList.size();
		INT totalPage = INT((total -1) /MAX_PVP_ROOM) + 1;
		m_iCurPage = min(totalPage-1, page); // 计算当前页，索引从0开始
		INT startIndex = m_iCurPage * MAX_PVP_ROOM; // 当前页第一个房间
		std::list<INT16>::const_iterator itor = idList.begin();
		for(int i=0; i<startIndex;i++) itor++;	// 前移到当前页的第一个房间
		for(int i=0; i< MAX_PVP_ROOM; i++)
		{
			if(!P_VALID(m_pRoomCtrls[i])) continue;
			if(startIndex + i >= total) //判断该位置是否为空
			{
				m_pRoomCtrls[i]->ResetRoom();
				continue;
			}
			m_pRoomCtrls[i]->SetRoom(m_curHall, m_curLevel, *itor);
			itor++; // 未到最后的时候才能前移
		}
		if(P_VALID(m_pStaticPageNum)) // 更新页码到界面
		{
			tstringstream ts;
			ts << m_iCurPage+1;
			m_pStaticPageNum->SetText(ts.str().c_str());
			m_pStaticPageNum->SetRefresh();
		}
		SetCurSelect(m_iCurSelected); // 设置当前选择的房间

		// 批量更新当前页的房间
		std::vector<INT16> roomIDs;
		for(int i=0; i< MAX_PVP_ROOM; i++)
		{
			if(!P_VALID(m_pRoomCtrls[i]) || m_pRoomCtrls[i]->GetRoomID()<0) continue;
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_pRoomCtrls[i]->GetHallType(), m_pRoomCtrls[i]->GetLevel(), m_pRoomCtrls[i]->GetRoomID());
			if(P_VALID(pRoom))
			{
				if(!pRoom->IsInitialized()) // 未初始化的才更新
				{
					roomIDs.push_back(pRoom->GetRoomID());
					pRoom->RefreshUpdateTime();
				}
			}
		}
		if(roomIDs.size()>0)
		{
			tagNC_PvPGetSimRoomInfo msg;
			tagNC_PvPGetSimRoomInfo* pSend = (tagNC_PvPGetSimRoomInfo*)new BYTE[sizeof(tagNC_PvPGetSimRoomInfo) + sizeof(INT16)*(roomIDs.size()-1)];
			if(P_VALID(pSend))
			{
				memcpy(pSend, &msg, sizeof(tagNC_PvPGetSimRoomInfo));
				pSend->dwSize = sizeof(tagNC_PvPGetSimRoomInfo) + sizeof(INT16)*(roomIDs.size()-1);
				pSend->eRoomLevel = m_curLevel;
				pSend->dwCount = roomIDs.size();
				for(DWORD i=0; i<pSend->dwCount; i++)
				{
					pSend->pData[i] = roomIDs[i];
				}
				m_pSession->Send(pSend);
			}
			SAFE_DEL_ARRAY(pSend);
		}
	}
}

// 设置当前选择的位置（房间界面）
void PvPFrame::SetCurSelect(INT nIndex)
{
	if(nIndex>=0 && P_VALID(m_pRoomCtrls[nIndex]) && m_pRoomCtrls[nIndex]->GetRoomID()>=0)
	{
		if(m_iCurSelected!= nIndex)
		{
			if(m_iCurSelected >=0)
				m_pImgListItem[m_iCurSelected]->SetPicFile(SELECT_DIBAN);	
			m_iCurSelected = nIndex;
			m_pImgListItem[m_iCurSelected]->SetPicFile(SELECT_ICON_PATH);
		}		
	}
	else 
	{
		if(m_iCurSelected >=0)
			m_pImgListItem[m_iCurSelected]->SetPicFile(SELECT_DIBAN);
		m_iCurSelected = -1;
	}
}

// 显示某一级别的房间列表（大厅界面）
void PvPFrame::SetLevel(E_PvP_RoomLevel level)
{
	PvPRoomList* pList = PvPMgr::Inst()->GetRoomList(m_curHall, level);
	if(P_VALID(pList)) pList->Update();

	if(m_curLevel == level) return;

	if(m_bIsJoinRoom || m_bIsCreatingRoom) return;

	if(! PvPMgr::Inst()->CanEnterHall(E_PvP_Middle))
	{
		return;
	}

	m_curLevel = level;	
	m_iCurPage = 0;

	// 取消加入房间
	m_bIsJoinRoom = FALSE;
	m_nJoinRoomID = -1;

	tagRoomListChange evt;
	evt.eHallType = m_curHall;
	evt.eLevel = m_curLevel;
	m_pFrameMgr->SendEvent(&evt);	
}

// 处理进入大厅消息，发送给服务器
DWORD PvPFrame::OnEnterHall(tagEnterHallEvent* pGameEvent)
{
	if(!P_VALID(pGameEvent)) return 0;
	E_PvP_RoomType hallType = pGameEvent->eHallType;
	if(m_bIsEnterring) return 0;
	if(m_curHall != E_PvP_RoomTypeNull)
	{
		if((P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible()) || (P_VALID(m_pWnd3V3Folder) && !m_pWnd3V3Folder->IsInvisible())) // 在房间内
			ToggleFolderRoom();
		else if(P_VALID(m_pWndHall) && !m_pWndHall->IsInvisible())
		{
			if(P_VALID(m_pWndCreate) && !m_pWndCreate->IsInvisible()) m_pWndCreate->SetInvisible(TRUE);
			LeaveHall();
		}
		return 0;
	}

	if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() < 30) // 小于30级 服务器端提示错误，客户端直接提示
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L14")]);
		return 0;
	}

	m_dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();

	m_enterHallType = hallType;
	m_bIsEnterring = TRUE;

	for(int i=0; i< MAX_PVP_ROOM; i++)
		m_pRoomCtrls[i]->ResetRoom();

	tagNC_PvPEnterHall msg;
	msg.eRoomType = hallType;
	m_pSession->Send(&msg);
	return 0;
}

// 处理esc键
BOOL PvPFrame::EscCancel()
{
	if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible()) 
	{
		//LeaveRoom();
		FolderRoom();
		return TRUE;
	}
	else if(P_VALID(m_pWndCreate) && !m_pWndCreate->IsInvisible())
	{
		m_bIsCreatingRoom = FALSE;
		m_pWndCreate->SetInvisible(TRUE);
		m_pWndHall->SetUnClickable(FALSE);
		//m_pGUI->SetModal(m_pWndHall);
		return TRUE;
	}
	else if(P_VALID(m_pWndHall) && !m_pWndHall->IsInvisible() && m_curHall!= E_PvP_RoomTypeNull)
	{
		LeaveHall();
		return TRUE;
	}
	return FALSE;
}

// 过滤房间
std::list<INT16> PvPFrame::FilterRooms(const std::list<INT16>& original)const
{
	std::list<INT16> res;

	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer)) return res;
	INT level = pPlayer->GetRoleLevel();
	DWORD wuXun = PvPMgr::Inst()->Wuxun2Level(pPlayer->GetAttribute(ERA_WuXun));

	for(std::list<INT16>::const_iterator itor=original.begin(); itor!=original.end(); itor++)
	{
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, m_curLevel, *itor);
		if(P_VALID(pRoom))
		{
			if(!pRoom->IsInitialized())
			{
				res.push_back(*itor);
			}
			else
			{
				if(pRoom->GetLevelLowerBound() <= level && pRoom->GetLevelUpperBound()>=level && pRoom->GetWuXunLimit()<=wuXun)
				{
					res.push_back(*itor);
				}
			}
		}
	}
	return res;
}

// 设置当前房间并打开房间界面
void PvPFrame::ShowRoom3v3(E_PvP_RoomLevel eLevel, INT16 roomID)
{
	if(P_VALID(m_pWndCreate) && !m_pWndCreate->IsInvisible()) 
	{
		m_pWndCreate->SetInvisible(TRUE);
		m_pWndHall->SetUnClickable(FALSE);
	}
	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, eLevel, roomID);
	if(!P_VALID(pRoom))
	{
		PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, eLevel);
		if(!P_VALID(pRoomList))
			return;
		pRoom = pRoomList->AddRoom(roomID);
	}
	//SetLevel(eLevel);
	m_nRoomID = roomID;
	m_p3v3RoomCtrl->SetRoom(m_curHall, eLevel, m_nRoomID);
	for(int i=0; i<NUM_3V3; i++)
		m_p3v3PosCtrl[i]->SetRoom(m_curHall, eLevel, roomID);	
	SetCurSelectPos(0);
	if(P_VALID(m_pWnd3v3Room)) 
	{
		m_pWnd3v3Room->SetInvisible(FALSE);
		m_pWnd3v3Room->FlipToTop();
		m_pWndHall->SetInvisible(TRUE);
		m_pWnd3V3Folder->SetInvisible(TRUE);
		//m_pWndHall->SetUnClickable(TRUE);
		//m_pGUI->SetModal(m_pWnd3v3Room);
	}
	pRoom->ForceUpdate();
	m_b3v3IsSendingMsg = FALSE;	
}

// 发送离开大厅
void PvPFrame::LeaveHall()
{
	if(m_curHall == E_PvP_RoomTypeNull)
	{
		if(P_VALID(m_pWndHall)) 
		{
			m_pWndHall->SetInvisible(TRUE);	
			m_pWndHall->SetUnClickable(FALSE);
		}
		if(P_VALID(m_pWnd3v3Room)) m_pWnd3v3Room->SetInvisible(TRUE);		
		if(P_VALID(m_pWndCreate)) m_pWndCreate->SetInvisible(TRUE);	
		if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
		//m_pGUI->SetModal(NULL);
	}
	else
	{
		tagNC_PvPLeaveHall msg;
		m_pSession->Send(&msg);
	}
}

// 发送加入房间
void PvPFrame::JoinRoom(E_PvP_RoomLevel level, INT16 roomID)
{
	if(m_curHall == E_PvP_RoomTypeNull || m_curLevel == E_PvP_RoomLevelNull || level != m_curLevel) return;

	if(m_bIsJoinRoom || m_bIsCreatingRoom) return;

	m_bIsJoinRoom = TRUE;
	m_nJoinRoomID = roomID;

	tagNC_PvPJoinRoom msg;
	msg.eRoomLevel = level;
	msg.nRoomID = roomID;
	m_pSession->Send(&msg);
}

// 发送创建房间
void PvPFrame::CreateRoom(E_PvP_RoomLevel level, INT iLevelUpperLimit, INT iLevelLowerLimit,DWORD dwWuXunLimit)
{
	if(m_bIsJoinRoom || m_bIsCreatingRoom) return;
	m_bIsCreatingRoom = TRUE;
	tagNC_PvPCreateRoom msg;
	msg.eRoomLevel = level;
	msg.iLevelLowerLimit = iLevelLowerLimit;
	msg.iLevelUpperLimit = iLevelUpperLimit;
	msg.dwWuXunLimit = dwWuXunLimit;
	m_pSession->Send(&msg);
}

// 发送离开房间
void PvPFrame::LeaveRoom()
{
	tagNC_PvPLeaveRoom msg;
	m_pSession->Send(&msg);
}

// 发送更换位置
void PvPFrame::ChangePos(BYTE newPos)
{
	if(m_b3v3IsSendingMsg) 
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L6")]);
		return;
	}
	m_b3v3IsSendingMsg = TRUE;
	tagNC_PvPChangePosition msg;
	msg.ucNewPos = newPos;
	m_pSession->Send(&msg);
}

// 发送踢人
void PvPFrame::KickPlayer(DWORD dwRoleID)
{
	if(m_b3v3IsSendingMsg)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L6")]);
		return;
	}
	else if(dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L12")]);
		return ;
	}
	m_b3v3IsSendingMsg = TRUE;
	tagNC_PvPKickPlayer msg;
	msg.dwPlayerID = dwRoleID;
	m_pSession->Send(&msg);
}

// 发送准备
void PvPFrame::Prepare()
{
	if(m_b3v3IsSendingMsg)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L6")]);
		return;
	}
	m_b3v3IsSendingMsg = TRUE;
	tagNC_PvPPrepare msg;
	m_pSession->Send(&msg);
}

// 发送取消准备
void PvPFrame::UnPrepare()
{
	if(m_b3v3IsSendingMsg)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L6")]);
		return;
	}
	m_b3v3IsSendingMsg = TRUE;
	tagNC_PvPUnPrepare msg;
	m_pSession->Send(&msg);
}

// 发送开战
void PvPFrame::Start()
{
	if(m_b3v3IsSendingMsg)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L6")]);
		return;
	}
	m_b3v3IsSendingMsg = TRUE;
	tagNC_PvPStart msg;
	m_pSession->Send(&msg);
}

// 处理进入大厅消息
DWORD PvPFrame::NetRecvEnterHall(tagNS_PvPEnterHall* pMsg, DWORD)
{
	m_bIsEnterring = FALSE;
	m_bIsCreatingRoom = FALSE;
	m_bIsJoinRoom = FALSE;
	m_b3v3IsSendingMsg = FALSE;

	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		m_curHall = m_enterHallType;
		if(PvPMgr::Inst()->CanEnterHall(E_PvP_Middle))
			m_pTabMid->SetState(EGUIBS_PushDown);
		else if(PvPMgr::Inst()->CanEnterHall(E_PvP_Junior))
			m_pTabJunior->SetState(EGUIBS_PushDown);
		else if(PvPMgr::Inst()->CanEnterHall(E_PvP_Senior))
			SetLevel(E_PvP_Senior);
		else
		{
			m_curHall = E_PvP_RoomTypeNull;
			m_enterHallType = E_PvP_RoomTypeNull;
			ASSERT(0);
			return 0;
		}
				
		if(PvPMgr::Inst()->CanEnterHall(E_PvP_Middle))
		{
			m_pTabMid->SetUnClickable(FALSE);
			m_pTabMid->SetEnable(TRUE);
		}
		else
		{
			m_pTabMid->SetUnClickable(TRUE);
			m_pTabMid->SetEnable(FALSE);
		}

		if(PvPMgr::Inst()->CanEnterHall(E_PvP_Junior))
		{
			m_pTabJunior->SetUnClickable(FALSE);
			m_pTabJunior->SetEnable(TRUE);
		}
		else
		{
			m_pTabJunior->SetUnClickable(TRUE);
			m_pTabJunior->SetEnable(FALSE);
		}

		if(P_VALID(m_pWndHall))
		{
			m_pWndHall->SetInvisible(FALSE);
			m_pWndHall->SetUnClickable(FALSE);
			m_pWndHall->FlipToTop();
			//m_pGUI->SetModal(m_pWndHall);
		}
		m_bIsCreatingRoom = FALSE;
		m_nJoinRoomID = m_nRoomID = -1;
		m_dwTimeDue = 1000;
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	m_enterHallType = E_PvP_RoomTypeNull;
	return 0;
}

// 处理离开大厅消息
DWORD PvPFrame::NetRecvLeaveHall(tagNS_PvPLeaveHall* pMsg, DWORD)
{
	m_curHall = E_PvP_RoomTypeNull;
	m_curLevel = E_PvP_RoomLevelNull;
	if(P_VALID(m_pWndHall))
	{
		m_pWndHall->SetInvisible(TRUE);		
		m_pWndHall->SetUnClickable(FALSE);
	}
	if(P_VALID(m_pWnd3v3Room)) m_pWnd3v3Room->SetInvisible(TRUE);		
	if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
	if(P_VALID(m_pWndCreate)) m_pWndCreate->SetInvisible(TRUE);	
	if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
	//m_pGUI->SetModal(NULL);
	m_dwNPCID = 0;
	m_dwTimeDue = 1000;
	return 0;
}

// 处理房间列表消息
DWORD PvPFrame::NetRecvRoomList(tagNS_PvPGetRoomList* pMsg, DWORD)
{
	PvPRoomList* pList = PvPMgr::Inst()->GetRoomList(m_curHall, pMsg->eRoomLevel);
	if(P_VALID(pList)) pList->UpdateRoomList(pMsg->dwCount, pMsg->pData);
	return 0;
}

// 处理房间简易消息
DWORD PvPFrame::NetRecvSimRoomInfo(tagNS_PvPGetSimRoomInfo* pMsg, DWORD)
{
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		std::list<INT16> invalidList;
		E_PvP_RoomLevel eLevel = E_PvP_RoomLevelNull;
		for(DWORD i=0; i<pMsg->dwCount; i++)
		{
			tagPvPSimRoom* pRoomInfo = &pMsg->pData[i];
			eLevel = pRoomInfo->eLevel;
			if(pRoomInfo->ucCurrentPersonNum > 0)
			{
				PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, eLevel, pRoomInfo->nRoomID);			
				if(P_VALID(pRoom)) pRoom->Update(*pRoomInfo);
			}
			else
			{
				invalidList.push_back(pRoomInfo->nRoomID);				
			}
		}
		if(invalidList.size() > 0)
		{
			PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, eLevel); //所有的level相同
			if(P_VALID(pRoomList)) pRoomList->RemoveInvalid(invalidList);
		}
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 处理房间详细消息
DWORD PvPFrame::NetRecvRoomInfo(tagNS_PvPGetRoomInfo* pMsg, DWORD)
{
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		PBYTE pData = (PBYTE)pMsg->pData;
		std::list<INT16> invalidList;
		E_PvP_RoomLevel eLevel = E_PvP_RoomLevelNull;
		for(DWORD i=0; i<pMsg->dwCount; i++)
		{
			tagPvPRoom* pRoomInfo = (tagPvPRoom*)pData;
			eLevel = pRoomInfo->eLevel;
			if(pRoomInfo->ucCurrentPersonNum > 0)
			{
				PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, eLevel, pRoomInfo->nRoomID);			
				if(P_VALID(pRoom)) pRoom->Update(*pRoomInfo);
			}
			else
			{
				invalidList.push_back(pRoomInfo->nRoomID);				
			}
			pData += sizeof(tagPvPRoom) + (pRoomInfo->ucCurrentPersonNum-1)*sizeof(tagPvPRoleInfo);
		}
		if(invalidList.size() > 0)
		{
			PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(m_curHall, eLevel); //所有的level相同
			if(P_VALID(pRoomList)) pRoomList->RemoveInvalid(invalidList);
		}
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 处理进入房间消息
DWORD PvPFrame::NetRecvJoinRoom(tagNS_PvPJoinRoom* pMsg, DWORD)
{
	//if(!m_bIsJoinRoom) return 0;

	m_bIsJoinRoom = FALSE;

	if( pMsg->eRoomLevel == m_curLevel && (pMsg->nRoomID == m_nJoinRoomID || m_nJoinRoomID==-2)) // 指定加入或自动加入
	{
		if(pMsg->dwErrorCode == E_PvP_Success)
		{
			m_dwPrepareTimeOut = PREPARE_TIMEOUT;
			ShowRoom3v3(pMsg->eRoomLevel, pMsg->nRoomID);			
		}
		else
		{
			ShowErrorMsg(pMsg->dwErrorCode);
			
			if(E_PvP_RoomIsFull == pMsg->dwErrorCode || E_PvP_NoRoom == pMsg->dwErrorCode)
			{
				// update room info
				tagNC_PvPGetSimRoomInfo msg;
				msg.eRoomLevel = m_curLevel;
				msg.dwCount = 1;
				msg.pData[0] = pMsg->nRoomID;
				m_pSession->Send(&msg);
			}
		}
	}

	m_nJoinRoomID = -1;
	return 0;
}

// 处理创建房间详细消息
DWORD PvPFrame::NetRecvCreatRoom(tagNS_PvPCreateRoom* pMsg, DWORD)
{
	//if(!m_bIsCreatingRoom) return 0; // 自动加入时，创建房间

	m_bIsCreatingRoom = FALSE;
	m_bIsJoinRoom = FALSE;

	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		ShowRoom3v3(pMsg->eRoomLevel, pMsg->nRoomID);
		if(pMsg->eRoomLevel != m_curLevel)
		{
			switch(pMsg->eRoomLevel)
			{
			case E_PvP_Junior:
				m_pTabJunior->SetState(EGUIBS_PushDown);
				break;
			case E_PvP_Middle:
				m_pTabMid->SetState(EGUIBS_PushDown);
				break;
			}
		}
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}

	return 0;
}

// 处理退出房间消息
DWORD PvPFrame::NetRecvLeaveRoom(tagNS_PvPLeaveRoom* pMsg, DWORD)
{
	return 0;

	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		if(P_VALID(m_pWnd3v3Room)) 
		{
			m_pWnd3v3Room->SetInvisible(TRUE);
			//m_pWndHall->SetUnClickable(FALSE);
			//m_pGUI->SetModal(m_pWndHall);
		}
		if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
		m_nRoomID = -1;
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_p3v3RoomCtrl->GetHallType(), m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
		if(P_VALID(pRoom))
		{
			pRoom->ForceUpdate();
		}
		m_pWndHall->SetInvisible(FALSE);
		m_pEdt3v3Friend->SetText(_T(""));
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 处理置换位置消息
DWORD PvPFrame::NetRecvChangePos(tagNS_PvPChangePosition* pMsg, DWORD)
{
	if(!m_b3v3IsSendingMsg) return 0;
	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		// 服务器会主动通知
		/*if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())
		{
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
			if(P_VALID(pRoom)) pRoom->ForceUpdate();
		}*/
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 处理准备详细消息
DWORD PvPFrame::NetRecvPrepare(tagNS_PvPPrepare* pMsg, DWORD)
{
	if(!m_b3v3IsSendingMsg) return 0;
	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		// 服务器会主动通知
		/*if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())
		{
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
			if(P_VALID(pRoom)) pRoom->ForceUpdate();
		}*/
		m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Prepared);
	}
	else
	{
		m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Unprepared);
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 处理取消准备消息
DWORD PvPFrame::NetRecvUnPrepare(tagNS_PvPUnPrepare* pMsg, DWORD)
{
	if(!m_b3v3IsSendingMsg) return 0;
	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		// 服务器会主动通知
		/*if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())
		{
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
			if(P_VALID(pRoom)) pRoom->ForceUpdate();
		}*/
		m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Unprepared);
		m_dwPrepareTimeOut = PREPARE_TIMEOUT;
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
		m_p3v3RoomCtrl->SetPrepare(PvPRoomCtrl::E_Prepared);
	}
	return 0;
}

// 处理踢人消息
DWORD PvPFrame::NetRecvKickPlayer(tagNS_PvPKickPlayer* pMsg, DWORD)
{
	if(!m_b3v3IsSendingMsg) return 0;
	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		// 服务器会主动通知
		/*if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())
		{
			PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_curHall, m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
			if(P_VALID(pRoom)) pRoom->ForceUpdate();
		}*/
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

DWORD PvPFrame::NetRecvKicked(tagNS_PvPKicked* pMsg, DWORD)
{
	m_b3v3IsSendingMsg = FALSE;
	if(P_VALID(m_pWnd3v3Room)) 
	{
		m_pWnd3v3Room->SetInvisible(TRUE);
		m_pWndHall->SetUnClickable(FALSE);
		//m_pGUI->SetModal(m_pWndHall);
	}
	if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
	m_pWndHall->SetInvisible(FALSE);
	m_nRoomID = -1;
	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_p3v3RoomCtrl->GetHallType(), m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
	if(P_VALID(pRoom))
	{
		pRoom->ForceUpdate();
	}
	if(GT_VALID(pMsg->dwManagerID) && pMsg->dwManagerID != 0) // 房主踢人
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L7")]);
	else	// 系统踢人
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L13")]);
	return 0;
}

// 开始战斗操作
DWORD PvPFrame::NetRecvStart(tagNS_PvPStart* pMsg, DWORD)
{
	m_b3v3IsSendingMsg = FALSE;
	if(pMsg->dwErrorCode == E_PvP_Success)
	{
		// wait for NS_PvPStarted
	}
	else
	{
		ShowErrorMsg(pMsg->dwErrorCode);
	}
	return 0;
}

// 开始战斗
DWORD PvPFrame::NetRecvStarted(tagNS_PvPStarted* pMsg, DWORD)
{	
	m_dwEnterPvPTimeOut = ENTERPVP_TIMEOUT;
	
	return 0;
}

// 返回房间
DWORD PvPFrame::NetRecvSetInfoRoom(tagNS_PvPSetIntoRoom* pMsg, DWORD)
{
	if(P_VALID(pMsg))
	{
		SetIntoRoom(pMsg->hallType, pMsg->eLevel, pMsg->roomID);
	}
	return 0;
}

void PvPFrame::SetIntoRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID)
{
	m_curHall = hallType;	
	if(PvPMgr::Inst()->CanEnterHall(eLevel))
	{
		switch(eLevel)
		{
		case E_PvP_Junior:
			m_pTabJunior->SetState(EGUIBS_PushDown);
			break;
		case E_PvP_Middle:
			m_pTabMid->SetState(EGUIBS_PushDown);
			break;
		default:
			SetLevel(eLevel);
			break;
		}
	}
	else if(PvPMgr::Inst()->CanEnterHall(E_PvP_Middle))
		m_pTabMid->SetState(EGUIBS_PushDown);
	else if(PvPMgr::Inst()->CanEnterHall(E_PvP_Junior))
		m_pTabJunior->SetState(EGUIBS_PushDown);
	else if(PvPMgr::Inst()->CanEnterHall(E_PvP_Senior))
		SetLevel(E_PvP_Senior);
	else
		ASSERT(0);
	
	if(P_VALID(m_pWndHall)) 
	{
		m_pWndHall->SetInvisible(FALSE);	
		m_pWndHall->SetUnClickable(FALSE);
		m_pWndHall->FlipToTop();

		if(PvPMgr::Inst()->CanEnterHall(E_PvP_Middle))
		{
			m_pTabMid->SetUnClickable(FALSE);
			m_pTabMid->SetEnable(TRUE);
		}
		else
		{
			m_pTabMid->SetUnClickable(TRUE);
			m_pTabMid->SetEnable(FALSE);
		}

		if(PvPMgr::Inst()->CanEnterHall(E_PvP_Junior))
		{
			m_pTabJunior->SetUnClickable(FALSE);
			m_pTabJunior->SetEnable(TRUE);
		}
		else
		{
			m_pTabJunior->SetUnClickable(TRUE);
			m_pTabJunior->SetEnable(FALSE);
		}
	}
	m_bIsCreatingRoom = FALSE;
	m_nJoinRoomID = 0;
	m_dwPrepareTimeOut = PREPARE_TIMEOUT;
	m_dwEnterPvPTimeOut = 0;
	ShowEnterPvPPic(-1);
	ShowRoom3v3(eLevel, roomID);		
}

// 邀请结果
DWORD PvPFrame::NetRecvInvite(tagNS_PvPInvite* pMsg, DWORD)
{
	if(pMsg->dwErrorCode != E_PvP_Success)
	{
		ShowErrorMsg(pMsg->dwErrorCode, pMsg->dwFriendID);
	}
	return 0;
}

// 被邀请
DWORD PvPFrame::NetRecvInviteJoin(tagNS_PvPInviteJoin* pMsg, DWORD)
{
	BOOL bValid = FALSE;
	while(true)
	{
		if(m_bIsCreatingRoom || !m_pWndCreate->IsInvisible() || !m_pWnd3v3Room->IsInvisible() || !m_pWnd3V3Folder->IsInvisible() || m_bIsJoinRoom || !m_pWndInviteConfirm->IsInvisible()) break;
		SocialMgr* pSocialMgr = (SocialMgr*)TObjRef<GameFrameMgr>()->GetFrame(_T("SocialMgr"));
		if( !P_VALID(pSocialMgr) ) break;
		SocialData* pData = pSocialMgr->GetData();
		if(!P_VALID(pData->GetFriend(pMsg->dwInvitorID))) break;

		tstring invitorName;
		Role* pRole = (Role*)RoleMgr::Inst()->FindRole(pMsg->dwInvitorID);
		if(P_VALID(pRole) && pRole->IsAttributeInited())
		{
			invitorName = pRole->GetRoleName();
		}
		else
		{
			LPCTSTR szFriendName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwInvitorID);
			if(P_VALID(szFriendName) && _tcslen(szFriendName) > 0)
			{
				invitorName = szFriendName;
			}	
			else
			{
				break;
			}
		}
		bValid = TRUE;
		m_eInvitedRoomType = pMsg->eRoomType;
		m_eInvitedLevel = pMsg->eRoomLevel;
		m_nInvitedRoomID = pMsg->nRoomID;
		m_dwInvitorID = pMsg->dwInvitorID;
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(pMsg->eRoomType, pMsg->eRoomLevel, pMsg->nRoomID);
		if(!P_VALID(pRoom))
		{
			PvPRoomList* pRoomList = PvPMgr::Inst()->GetRoomList(pMsg->eRoomType, pMsg->eRoomLevel);
			if(P_VALID(pRoomList))
			{
				pRoom = pRoomList->AddRoom(pMsg->nRoomID);
				pRoom->ForceUpdate();
			}
		}
		tagPoint p = m_pWndInviteConfirm->GetSize();
		tagPoint offSet;
		GUIStaticEx* descriptiontext = (GUIStaticEx*)m_pWndInviteConfirm->GetChild(_T("back\\textback\\text"));
		TCHAR temp[128];
		wsprintf(temp, g_StrTable[_T("PvP_UI_InviteDisplay")], invitorName.c_str());
		descriptiontext->SetText(temp);
		tagPoint size = descriptiontext->GetSize();
		p.y -= size.y;
		offSet.y = descriptiontext->GetTextTotalHeight()- size.y;
		descriptiontext->SetResizable(TRUE);
		size.y = descriptiontext->GetTextTotalHeight();
		p.y += size.y;
		descriptiontext->Resize(size);
		descriptiontext->SetResizable(FALSE);
		/*m_pBtnInviteCancel->SetMovable(TRUE);
		m_pBtnInviteCancel->Move(offSet);
		m_pBtnInviteCancel->SetMovable(FALSE);
		m_pBtnInviteOK->SetMovable(TRUE);
		m_pBtnInviteOK->Move(offSet);
		m_pBtnInviteOK->SetMovable(FALSE);
		m_pWndInviteConfirm->SetResizable(TRUE);
		m_pWndInviteConfirm->Resize(p);
		m_pWndInviteConfirm->SetResizable(FALSE);*/
		m_pWndInviteConfirm->SetInvisible(FALSE);
		m_pWndInviteConfirm->FlipToTop();
		break;
	}
	if(!bValid)
	{
		RefuseJoin(pMsg->eRoomType, pMsg->eRoomLevel, pMsg->nRoomID, pMsg->dwInvitorID);
	}
	
	return 0;
}

// 开始传送
DWORD PvPFrame::NetRecvEnterPvP(tagNS_PvPEnterPvP* pMsg, DWORD)
{
	RandomPvPFrame* pPvPFrame = (RandomPvPFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("RandomPvP"));
	if( P_VALID( pPvPFrame ) )
	{
		pPvPFrame->HideWnd();
	}

	return 0;

	PvPMgr::Inst()->SetRoomToRoleMap(m_p3v3RoomCtrl->GetHallType(), m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
	if(P_VALID(m_pWndHall))
	{
		m_pWndHall->SetInvisible(TRUE);		
		m_pWndHall->SetUnClickable(FALSE);
	}
	if(P_VALID(m_pWnd3v3Room)) m_pWnd3v3Room->SetInvisible(TRUE);		
	if(P_VALID(m_pWndCreate)) m_pWndCreate->SetInvisible(TRUE);	
	if(P_VALID(m_pWnd3V3Folder)) m_pWnd3V3Folder->SetInvisible(TRUE);
	//m_pGUI->SetModal(NULL);
	m_curHall = E_PvP_RoomTypeNull;
	m_curLevel = E_PvP_RoomLevelNull;
	m_nRoomID = 0;
	
	m_dwEnterPvPTimeOut = 0;
	ShowEnterPvPPic(-1);
	// 创建竞技场内状态frame
	/*if(m_pFrameMgr->GetFrame(_T("PvPStateFrame")) == NULL)
		m_pFrameMgr->CreateFrame(_T("Root"), _T("PvPStateFrame"), _T("PvPStateFrame"), 0);*/
	return 0;
}

void PvPFrame::ShowErrorMsg( DWORD dwErrCode )
{
	if (dwErrCode > E_PvP_Success&& dwErrCode < E_PvP_End)
	{
		tstringstream tss;
		tss << _T("PvPErrorMsg_") << dwErrCode;
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);		
	}
}

void PvPFrame::ShowErrorMsg( DWORD dwErrCode, DWORD roleID)
{
	if (dwErrCode > E_PvP_Success&& dwErrCode < E_PvP_End)
	{
		tstringstream tss;
		tss << _T("PvPErrorMsg_") << dwErrCode;
		tstring szFriendName = g_StrTable[_T("PvP_Msg_Player")];
		Role* pRole = (Role*)RoleMgr::Inst()->FindRole(roleID);
		if(P_VALID(pRole) && pRole->IsAttributeInited())
		{
			szFriendName = pRole->GetRoleName();
		}
		else
		{
			szFriendName = PlayerNameTab::Inst()->FindNameByID(roleID);
		}
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()], szFriendName.c_str());		
	}
}

// 折叠窗口
void PvPFrame::FolderRoom(BOOL bFolder/* = TRUE*/)
{
	if(bFolder)
	{
		if(P_VALID(m_pWnd3v3Room) && !m_pWnd3v3Room->IsInvisible())
		{
			m_pWnd3v3Room->SetInvisible(TRUE);
		}
		if(P_VALID(m_pWnd3V3Folder) && m_pWnd3V3Folder->IsInvisible())
		{
			m_pWnd3V3Folder->SetInvisible(FALSE);
			m_pWnd3V3Folder->FlipToTop();
		}
	}
	else
	{
		if(P_VALID(m_pWnd3v3Room) && m_pWnd3v3Room->IsInvisible())
		{
			m_pWnd3v3Room->SetInvisible(FALSE);
			m_pWnd3v3Room->FlipToTop();
		}
		if(P_VALID(m_pWnd3V3Folder) && !m_pWnd3V3Folder->IsInvisible())
		{
			m_pWnd3V3Folder->SetInvisible(TRUE);
		}
	}
}

// 折叠/展开窗口
void PvPFrame::ToggleFolderRoom()
{
	if(P_VALID(m_pWnd3v3Room) && m_pWnd3v3Room->IsInvisible())
	{
		FolderRoom(FALSE);
	}
	else
	{
		FolderRoom();
	}
}

void PvPFrame::AutoJoin(E_PvP_RoomLevel level)
{
	m_bIsJoinRoom = TRUE;
	m_nJoinRoomID = -2;
	tagNC_PvPAutoJoinRoom send;
	send.eRoomLevel = level;
	m_pSession->Send(&send);
}

void PvPFrame::InviteFriend(LPCTSTR szRoleName)
{
	if(Kernel::Inst()->GetAccumTimeDW() - m_dwLastInvite < 500) //操作太频繁
	{
		return;
	}
	SocialMgr* pSocialMgr = (SocialMgr*)TObjRef<GameFrameMgr>()->GetFrame(_T("SocialMgr"));
	if( P_VALID(pSocialMgr) )
	{
		SocialData* pData = pSocialMgr->GetData();
		const tagFriendInfo* pInfo = NULL;
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_p3v3RoomCtrl->GetHallType(), m_p3v3RoomCtrl->GetLevel(), m_p3v3RoomCtrl->GetRoomID());
		if(!P_VALID(pRoom) || !pRoom->IsInitialized()) return;
		for(int t=SocialData::ELT_Friend1; t<= SocialData::ELT_Friend4; t++)
		{
			pInfo = pData->GetFriendPtrByIndex( SocialData::EListType(t), 0 );
			for( int i = 0; P_VALID(pInfo); pInfo = pData->GetFriendPtrByIndex(SocialData::EListType(t), ++i))
			{
				Role* pRole = (Role*)RoleMgr::Inst()->FindRole(pInfo->dwFriendID);
				if(P_VALID(pRole) && pRole->IsAttributeInited())
				{
					if(pRole->GetRoleName() == szRoleName)
						break;
				}
				LPCTSTR szFriendName = PlayerNameTab::Inst()->FindNameByID(pInfo->dwFriendID);
				if(P_VALID(szFriendName) && _tcslen(szFriendName) > 0)
				{
					if(_tcscmp(szRoleName,szFriendName) == 0)
						break;
				}
			}
			if(P_VALID(pInfo)) break;
		}
		
		if(P_VALID(pInfo)) // 找到好友
		{			
			if(pRoom->GetCount() < pRoom->GetNumLimit()) // 房间人数未满
			{
				if(pInfo->bOnline && pInfo->nLevel<=pRoom->GetLevelUpperBound() && pInfo->nLevel>=pRoom->GetLevelLowerBound()) // 符合房间限制
				{
					if(!P_VALID(pRoom->GetRoleInfo(pInfo->dwFriendID))) // 不在房间内
					{
						tagNC_PvPInvite send;
						send.nRoomID = pRoom->GetRoomID();
						send.dwFriendID = pInfo->dwFriendID;
						send.eRoomLevel = pRoom->GetRoomLevel();
						send.eRoomType = pRoom->GetHallType();
						m_pSession->Send(&send);

						m_dwLastInvite = Kernel::Inst()->GetAccumTimeDW();
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Msg_InvitePromote")], szRoleName);
					}
					else
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L11")]);
				}
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L10")]);
				}
			}
			else
			{
				ShowErrorMsg(E_PvP_InviteFull);
			}
		}
		else
		{
			ShowErrorMsg(E_PvP_InviteNotFriend);
		}
	}
}

void PvPFrame::RefuseJoin(E_PvP_RoomType hall, E_PvP_RoomLevel level, INT16 roomID, DWORD invitorID, BOOL bRefused/* = TRUE*/)
{
	if(!this->m_pWnd3v3Room->IsInvisible()) bRefused = TRUE;
	if(!bRefused)
	{
		if(PvPMgr::Inst()->CanEnterHall(level))
		{
			m_bIsEnterring = FALSE;
			m_bIsCreatingRoom = FALSE;
			m_bIsJoinRoom = FALSE;
			m_b3v3IsSendingMsg = FALSE;

			m_curHall = hall;			

			if(PvPMgr::Inst()->CanEnterHall(E_PvP_Junior))
			{
				m_pTabJunior->SetUnClickable(FALSE);
				m_pTabJunior->SetEnable(TRUE);
			}
			else
			{
				m_pTabJunior->SetUnClickable(TRUE);
				m_pTabJunior->SetEnable(FALSE);
			}

			if(P_VALID(m_pWndHall))
			{
				m_pWndHall->SetInvisible(FALSE);
				m_pWndHall->SetUnClickable(FALSE);
				m_pWndHall->FlipToTop();
				//m_pGUI->SetModal(m_pWndHall);
			}
			switch(level)
			{
			case E_PvP_Junior:
				m_pTabJunior->SetState(EGUIBS_PushDown);
				break;
			case E_PvP_Middle:
				m_pTabMid->SetState(EGUIBS_PushDown);
				break;
			}
			SetLevel(level);
			m_bIsJoinRoom = TRUE;
			m_nJoinRoomID = roomID;			
			//m_curLevel = level;
		}
		else
		{
			bRefused = TRUE;
		}
	}
	tagNC_PvPInviteJoin send;
	send.dwInvitorID = invitorID;
	send.eRoomLevel = level;
	send.nRoomID = roomID;
	send.bIsAccpeted = !bRefused;
	send.eRoomType = hall;
	m_pSession->Send(&send);
}

void PvPFrame::ShowEnterPvPPic(INT nIndex)
{
	if(P_VALID(m_pWndEnterPvPTimeOut) && P_VALID(m_pStaticEnterTimeOut))
	{
		tstring strPath = _T("data\\ui\\compet\\");
		//tagRect rect;
		switch(nIndex)
		{
		case 0:
			strPath += _T("one.bmp");
			//rect = tagRect(523,370,719,608);	//(560, 360, 660, 600);
			break;
		case 1:
			strPath += _T("two.bmp");
			//rect = tagRect(523,370,719,608);	//(560, 360, 660, 600);
			break;
		case 2:
			strPath += _T("three.bmp");
			//rect = tagRect(722,362,917,605); //(740, 400, 920, 560);
			break;
		case 3:
			strPath += _T("four.bmp");
			//rect = tagRect(557,608,754,848); //(600, 665, 740, 800);
			break;
		case 4:
			strPath += _T("five.bmp");
			//rect = tagRect(557,608,754,848); //(600, 665, 740, 800);
			break;
		default:
			m_pWndEnterPvPTimeOut->SetInvisible(TRUE);
			return;
		}
		m_pStaticEnterTimeOut->SetPic(strPath);//, rect);
		m_pStaticEnterTimeOut->SetRefresh();
		m_pWndEnterPvPTimeOut->SetInvisible(FALSE);
		m_pWndEnterPvPTimeOut->FlipToTop();
	}
}