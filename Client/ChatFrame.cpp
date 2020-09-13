#include "stdafx.h"
#include "ChatFrame.h"
#include "RichEditBoxForChatInput.h"
#include "RichEditBoxForChatShow.h"
#include "ScrollRichEditBox.h"
#include "StaticForCast.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ChatShowPop.h"
#include "WorldFrame.h"
#include "NPC.h"
#include "Role.h"
#include "CreatureData.h"
#include "SkillMgr.h"
#include "Item.h"
#include "ItemMgr.h"
#include "Container.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ChatShowItemEquip.h"
#include "ItemProtoData.h"
#include "PersonalityActData.h"
#include "CombatEvent.h"
#include "CurrencyMgr.h"
#include "TargetStateFrame.h"
#include "ChatEvent.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "TeamEvent.h"
#include "SocialEvent.h"
#include "FilterData.h"
#include "WorldPickerFrame.h"
#include "GameSet.h"
#include "GuildEvent.h"
#include "HttpPicEx.h"
#include "GuildMgr.h"
#include "ChatChannelSetFrame.h"
#include "StyleActionFrame.h"
#include "MilleSoundFrame.h"
#include "AudioSys.h"
#include "GlintPushButton.h"
#include "GlintButton.h"
#include "MasterEvent.h"
#include "..\WorldDefine\msg_prison.h"
#include "MultChatData.h"
#include "..\WorldDefine\role_att.h"
#include "FamilyEvent.h"
#include "ServerTime.h"
#include "SaveAccount.h"

const TCHAR* KeyStart = _T("<keystart>");
const TCHAR* KeyEnd = _T("<keyend>");

#define CHANNEL_SET_FILE _T("config\\%s\\ChannelSet.xml") 

static BOOL bOpenFlag = TRUE;
static float fWorldChatTime = 0.0f;

const static DWORD	CSDW_World_Tool_TypeID1 = 3070001;
const static DWORD	CSDW_World_Tool_TypeID2 = 3070002;

ChatFrame::ChatFrame(void):m_Trunk(this)
{
	m_pWnd					= NULL;
	m_pWndCaption			= NULL;
	m_pSclFatherWnd			= NULL;
	m_pRunHorseWnd			= NULL;
	m_pBtnHide				= NULL;
	m_pBtnChannelSet		= NULL;
	m_pBtnCurChannel		= NULL;
	m_pBtnEmotion			= NULL;
	m_pBtnMiniChat          = NULL;   //初始化密语聊天按钮
	m_pStaticLangue[0]		= NULL;
	m_pStaticLangue[1]		= NULL;
	m_pRebInput				= NULL;
	m_pScrollRichText		= NULL;
	m_pRunHorseScrollText	= NULL;
	m_dwCurrentRunHorsePriority	= 0;
	m_pChannelSel			= NULL;
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		m_pChatContext[i]	= NULL;
		m_pScl[i]			= NULL;
		m_pBtnUp[i]			= NULL;
		m_pBtnDp[i]			= NULL;
		m_dwRecvChalShow[i] = 0;
	}
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		m_pPage[i]	= NULL;
	}
	m_curRecvChannel		= 0;
	m_curSendChannel		= ESCC_NULL;
	m_lnSlcFatherWndX		= 0;
	m_lnSlcFatherWndY		= 0;
	m_pEmotionFrame			= NULL;
	m_dwLocalID				= GT_INVALID;
	m_bChatContext			= FALSE;

	m_lnCastWndX			= 0;
	m_lnCastWndY			= 0;
	for(int i=0; i<NUM_CHAT_CAST_WND; i++)
	{
		m_pCastRichText[i] = NULL;
		m_pStcName[i] = NULL;
	}
	//m_pWndCastBack			= NULL;
	m_pBtnZoomUp			= NULL;
	m_nZoomMax				= 2;
	m_bHide					= false;
	m_pStcCast				= NULL;
	m_bWorldOK				= FALSE;
	m_pWndWorld				= NULL;
	m_pBtnWorldOK			= NULL;
	m_pBtnWorldCancel		= NULL;
	m_pPBtnWorld			= NULL;
	m_pStcExWold			= NULL;
	m_eSendMsgState			= SendMsgState_NULL;
	m_pWndMenuEx			= NULL;
	m_pListMenuEx			= NULL;
	m_pWndImName			= NULL;
	m_HKL					= NULL;
	m_HKLenglish			= NULL;
	m_pBtnHideOthers		= NULL;
	//-----------------密语聊天控件初始化---------------------
	m_pWnd            = NULL;
	m_pBtn_Exit       = NULL;      //退出
	m_pBtn_Send       = NULL;      //发送
	m_pBtn_SetObj     = NULL;      //设置对象名字的按钮
	m_pLB_ChatObj     = NULL;      //聊天对象ListBox
	m_pWndText        = NULL;      //聊天信息
	m_pEB_SendMsg     = NULL;      //发送信息框
	m_pEB_SetObj      = NULL;      //设置聊天对象名字
	m_pPB_CloseTag    = NULL;      //控制发送信息后是否关闭聊天窗口的按钮
	m_pSE_ShowObjName = NULL;      //显示正在跟谁聊天
	m_pSB_MiniChat    = NULL;
	m_pBtn_up         = NULL;
	m_pBtn_down       = NULL;
	m_pMiniChatWnd    = NULL;
	m_bisfromChat     = FALSE; 
	m_destID          = GT_INVALID;
	m_NowRoleID       = GT_INVALID;
	m_NowChatWith     = _T("");
	//-------------------群聊-----------------------------------
	m_pMultChatFrame    = NULL;
	m_pBn_StyleAction   = NULL;
	m_pWndHide         = NULL;
	m_pBtnGroupChat = NULL;

	m_dwViewEquipTime = 0;
	m_nOnlineHour = 1;
}	

ChatFrame::~ChatFrame(void)
{
	
}


ChatFrame g_chatFrame;
ChatFrame* ChatFrame::Inst()
{
	return &g_chatFrame;
}

// init
BOOL ChatFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	tstring szFile = g_strLocalPath + _T("\\chat\\emotion.xml");

	//创建表情框
	m_pEmotionFrame = (EmotionFrame*)m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("Emotion"), _T("EmotionFrame"), 0);
	if( P_VALID(m_pEmotionFrame) )
		m_pEmotionFrame->LoadEmotionFile(szFile);

	TCHAR szEmnName[32] = {0};
	for(int i=0; i<MAX_EMOTION; i++)
	{
		_tcscpy(szEmnName, m_pEmotionFrame->GetEmtionTran(i));
		ASSERT(_tcslen(szEmnName)>1);
		this->RegisterPicForRichEdit(szEmnName, i);
	}
	m_pShowAffiche->RegisterPic(_T("//gg"), _T("data\\ui\\chat\\channel_01.bmp"));
	m_pShowAffiche->RegisterPic(_T("//xt"), _T("data\\ui\\chat\\channel_02.bmp"));
	for(int i = 0; i <NUM_CHAT_CHANNEL ; i++)
	{
		m_pChatContext[i]->RegisterPic(_T("//gg"), _T("data\\ui\\chat\\channel_01.bmp"));
		m_pChatContext[i]->RegisterPic(_T("//xt"), _T("data\\ui\\chat\\channel_02.bmp"));
	}


	m_pMgr->RegisterEventHandle( _T("tagGameEventForChat"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnGameEventForChat));
	m_pMgr->RegisterEventHandle( _T("tagRoleGetIDEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvNetRoleGetID));
	m_pMgr->RegisterEventHandle( _T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvNetRoleGetName));
	m_pMgr->RegisterEventHandle( _T("tagPrivateChatEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnPrivateChatEvent));
	m_pMgr->RegisterEventHandle( _T("tagSocialInfoEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnSocialInfoEvent));
	m_pMgr->RegisterEventHandle( _T("tagDungeonEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OntagDungeonEvent));
	m_pMgr->RegisterEventHandle( _T("tagNPCEaseSpeechEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnNPCEaseSpeechEvent));
	m_pMgr->RegisterEventHandle( _T("tagGetHeadpicByID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvHeadpicByID));
	m_pMgr->RegisterEventHandle( _T("tagChannelSetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnChannelSetEvent));
	m_pMgr->RegisterEventHandle( _T("tagLeaveMsg"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnLeaveMsg));
	m_pMgr->RegisterEventHandle( _T("tagRoleRenameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRoleGetNameEvent));
	m_pMgr->RegisterEventHandle( _T("tagOpenMiniChatEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnOpenMiniChat));         //注册打开密语窗口消息
	m_pMgr->RegisterEventHandle(_T("GroupChatButtonGlin"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnGroupChatButtonGlin));


	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_RoleChat",				(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleChat),		_T("NS_RoleChat"));	
	m_pCmdMgr->Register("NS_RoleShowEquip",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleShowEquip),	_T("NS_RoleShowEquip"));
	m_pCmdMgr->Register("NS_RoleShowItem",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleShowItem),	_T("NS_RoleShowItem"));
	m_pCmdMgr->Register("NS_MonsterEnterCombat",	(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_MonsterEnterCombat),		_T("NS_MonsterEnterCombat"));
	m_pCmdMgr->Register("NS_RoleDead",				(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_RoleDead),				_T("NS_RoleDead"));
	m_pCmdMgr->Register("NS_Skill",					(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_Skill),					_T("NS_Skill"));
	m_pCmdMgr->Register("NS_AutoNotice",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_AutoNotice),				_T("NS_AutoNotice"));
	m_pCmdMgr->Register("NS_RoleShowDroptItemInTeam", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_TemmatePickUpBroadCast), _T("NS_RoleShowDroptItemInTeam") );
	m_pCmdMgr->Register("NS_SomeoneViewYourEquip", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_SomeoneViewYourEquip), _T("NS_SomeoneViewYourEquip") );

	m_pKeyMap->Register( 0, g_StrTable[_T("HotKey_Emotion")],		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ChatFrame::OnKey), DIK_X, TRUE, FALSE, 0, (INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ChatFrame::OnKeyChange) );
	m_pKeyMap->Register(0, g_StrTable[_T("Hotkey_HidePlayer")], (INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ChatFrame::OnHideOtherPlayers), DIK_F9, TRUE, TRUE, 1);
	m_pKeyMap->Register( 0, g_StrTable[_T("Hotkey_MiniChat")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ChatFrame::OpenMiniChat), (DIK_Z<<16)+DIK_LCONTROL,TRUE,TRUE, 0 ); //密语热键Ctrl+Z
	
	m_pCmdMgr->Register("NS_SendToPrisonEx", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMessage_SendToPrisonEx), _T("NS_SendToPrisonEx") );


	m_dwLocalID	= RoleMgr::Inst()->GetLocalPlayer()->GetID();
	m_strLocalName = PlayerNameTab::Inst()->FindNameByID(m_dwLocalID);
	m_HKLenglish = ::GetKeyboardLayout( 0 );


	tstring strTemp = g_StrTable[_T("IsShowSysInfo")];
	LPCSTR szTemp1 = TObjRef<Util>()->UnicodeToUnicode8(strTemp.c_str());
	LPCSTR szTemp2 = "1"; 
	if( strcmp(szTemp1, szTemp2) == 0 )
	{
		PushInfo(g_StrTable[_T("SysShowInfo")],ESCC_System);
	}

	m_stStartTime = ServerTime::Inst()->CalCurrentServerDwordTime();

	return TRUE;
}

// destroy
BOOL ChatFrame::Destroy()
{
	m_vecRecv.clear();
	m_vecSpeak.clear();
	m_vecPvtSpeak.clear();
	m_vecSclText.clear();
	m_vecBigSclText.clear();
	m_vecCast.clear();
	m_vecCastNoName.clear();
	m_vecRecvEquip.clear();
	m_vecRecvItem.clear();
	MiniChatMsg.clear();    //清空密语消息

	BOOL ret = GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	if( P_VALID(m_pSclFatherWnd) )
		m_pGUI->DestroyWnd(m_pSclFatherWnd);

	if( P_VALID(m_pRunHorseWnd) )
		m_pGUI->DestroyWnd(m_pRunHorseWnd);

	if( P_VALID(m_pStcCast) )
		m_pGUI->DestroyWnd(m_pStcCast);

	if( P_VALID(m_pWndImName) )
		m_pGUI->DestroyWnd(m_pWndImName);

	if( P_VALID(m_pWndHeadPic) )
		m_pGUI->DestroyWnd(m_pWndHeadPic);

	if( P_VALID(m_pWndChannel) )
		m_pGUI->DestroyWnd(m_pWndChannel);

	if( P_VALID(m_pWndWorld) )
		m_pGUI->DestroyWnd(m_pWndWorld);

	if( P_VALID(m_pWndMenuEx) )
		m_pGUI->DestroyWnd(m_pWndMenuEx);

 	if( P_VALID(m_pMiniChatWnd))
 		m_pGUI->DestroyWnd(m_pMiniChatWnd);

	if( P_VALID(m_pWndHide))
		m_pGUI->DestroyWnd(m_pWndHide);

	m_pEmotionFrame = NULL;
	m_pMultChatFrame = NULL;
	m_pWndHide = NULL;

	m_pMgr->UnRegisterEventHandler( _T("tagGameEventForChat"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnGameEventForChat));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleGetIDEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvNetRoleGetID));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvNetRoleGetName));
	m_pMgr->UnRegisterEventHandler( _T("tagPrivateChatEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnPrivateChatEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagSocialInfoEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnSocialInfoEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagDungeonEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OntagDungeonEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagNPCEaseSpeechEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnNPCEaseSpeechEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagGetHeadpicByID"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRecvHeadpicByID));
	m_pMgr->UnRegisterEventHandler( _T("tagChannelSetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnChannelSetEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagLeaveMsg"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnLeaveMsg));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleRenameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnRoleGetNameEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagOpenMiniChatEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnOpenMiniChat));
	m_pMgr->UnRegisterEventHandler( _T("GroupChatButtonGlin"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ChatFrame::OnGroupChatButtonGlin));




	m_pCmdMgr->UnRegister("NS_RoleChat",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleChat));	
	m_pCmdMgr->UnRegister("NS_RoleShowEquip",		(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleShowEquip));
	m_pCmdMgr->UnRegister("NS_RoleShowItem",		(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMsgNS_RoleShowItem));
	m_pCmdMgr->UnRegister("NS_MonsterEnterCombat",	(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_MonsterEnterCombat));
	m_pCmdMgr->UnRegister("NS_RoleDead",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_RoleDead));
	m_pCmdMgr->UnRegister("NS_Skill",				(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_Skill));
	m_pCmdMgr->UnRegister("NS_AutoNotice",			(NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_AutoNotice));
	m_pCmdMgr->UnRegister("NS_RoleShowDroptItemInTeam", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_TemmatePickUpBroadCast) );
	m_pCmdMgr->UnRegister("NS_SomeoneViewYourEquip", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::OnNS_SomeoneViewYourEquip) );
	m_pCmdMgr->UnRegister("NS_SendToPrisonEx", (NETMSGPROC)m_Trunk.sfp2(&ChatFrame::NetRecvMessage_SendToPrisonEx) );

	m_pKeyMap->SetEnable( g_StrTable[_T("HotKey_Emotion")],	    FALSE );


	ChatShowPop::Inst()->Destroy();

	return ret;
}

//加载控件
BOOL ChatFrame::ReloadUI()
{
	// 创建
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\chat.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	//调整位置
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	std::stringstream streamX, streamY;
	streamX << 0;
	streamY << (pDesktop->GetSize().y - 247);
	m_pGUI->ChangeXml(&element, "new", "LogicPos_x", streamX.str().c_str());
	m_pGUI->ChangeXml(&element, "new", "LogicPos_y", streamY.str().c_str());

	m_pGUI->ChangeXml(&element, "new\\back\\new0\\pic\\new", "ClassName", "RichEditBoxForChatInput");	//修改输入框的类
	m_pGUI->ChangeXml(&element, "new\\back\\chatset", "ClassName", "CGUIButton");
	//m_pGUI->ChangeXml(&element, "new\\back\\message", "ClassName", "GlintButton");

	m_pGUI->ChangeXml(&element, "new\\back\\minichat","ClassName", "GlintButton");         //更改为支持闪烁的按钮
	m_pGUI->ChangeXml(&element, "new\\back\\groupchat","ClassName", "GlintButton");         //更改为支持闪烁的按钮
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		char temp[256] = {0};
		sprintf(temp, "new\\back\\showchat%d", i);
		m_pGUI->ChangeXml(&element, temp, "ClassName", "RichEditBoxForChatShow");			//修改显示框的类
		m_pGUI->ChangeXml(&element, temp, "ShadowColor", "0xFF000000");
	}
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		//修改频道按钮的类型，以便支持闪烁效果
		char temp[256] = {0};
		sprintf_s(temp, "new\\back\\showchoose\\chatshowbutton%d", i );
		m_pGUI->ChangeXml( &element, temp, "ClassName", "GlintPushButton" );
	}
	//修改公告牌类型
	m_pGUI->ChangeXml( &element, "new\\proclaimback\\words", "ClassName", "RichEditBoxForChatShow" );
	m_pGUI->ChangeXml( &element, "new\\proclaimback\\words", "ShadowColor", "0xFF000000" );

	//---------------------创建------------------------------
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWnd->Align();

	//加载跑马灯	
	tstring strPath1 = g_strLocalPath + _T("\\ui\\runhorse.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath1.c_str());
	m_pGUI->ChangeXml(&element, "sclfatherwnd\\sclricheditbox", "ClassName", "ScrollRichEditBox"); 
	m_pGUI->ChangeXml(&element, "sclfatherwnd", "LogicPos_x", "0"); //
	m_pGUI->ChangeXml(&element, "sclfatherwnd", "LogicPos_y", "0"); //
	std::stringstream ss;
	ss << pDesktop->GetSize().x;
	m_pGUI->ChangeXml(&element, "sclfatherwnd", "Size_x", ss.str().c_str()); //
	m_pSclFatherWnd	= m_pGUI->CreateWnd(_T("\\desktop"), &element);

	//加载跑马灯2	
	tstring strPath9 = g_strLocalPath + _T("\\ui\\runhorse2.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath9.c_str());
	m_pGUI->ChangeXml(&element, "runhorse2_win\\sclfatherwnd\\sclricheditbox", "ClassName", "ScrollRichEditBox"); 
	m_pGUI->ChangeXml(&element, "runhorse2_win\\sclfatherwnd", "LogicPos_x", "0"); //
	m_pGUI->ChangeXml(&element, "runhorse2_win\\sclfatherwnd", "LogicPos_y", "0"); //

	m_pGUI->ChangeXml(&element, "runhorse2_win\\sclfatherwnd", "Size_x", ss.str().c_str()); //
	m_pRunHorseWnd	= m_pGUI->CreateWnd(_T("\\desktop"), &element);

	//加载传音
	tstring strPath6 = g_strLocalPath + _T("\\ui\\imname.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath6.c_str());
	m_pGUI->ChangeXml(&element, "imname_win\\name1", "ClassName", "StaticForCast");
	m_pGUI->ChangeXml(&element, "imname_win\\name2", "ClassName", "StaticForCast");
	m_pGUI->ChangeXml(&element, "imname_win\\name3", "ClassName", "StaticForCast");
	m_pWndImName = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndImName->Align();
	m_pWndImName->SetUnClickable(TRUE);
	m_pWndImName->SetInvisible(TRUE);
	tstring strPath2 = g_strLocalPath + _T("\\ui\\imtalk.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath2.c_str());
	m_pGUI->ChangeXml(&element, "imtalkstatic", "ClassName", "CGUIWindow");
	m_pGUI->ChangeXml(&element, "imtalkstatic\\talk1", "ClassName", "ScrollRichEditBox"); 
	m_pGUI->ChangeXml(&element, "imtalkstatic\\talk2", "ClassName", "ScrollRichEditBox");
	m_pGUI->ChangeXml(&element, "imtalkstatic\\talk3", "ClassName", "ScrollRichEditBox");
	m_pStcCast = (GUIWnd*)m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pStcCast->Align();
	m_pStcCast->SetUnClickable(TRUE);
	m_pStcCast->SetInvisible(TRUE);
	tstring strPath7 = g_strLocalPath + _T("\\ui\\imhead.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath7.c_str());
	m_pGUI->ChangeXml(&element, "imhead_win\\head1", "ClassName", "HttpPicEx");
	m_pGUI->ChangeXml(&element, "imhead_win\\head2", "ClassName", "HttpPicEx");
	m_pGUI->ChangeXml(&element, "imhead_win\\head3", "ClassName", "HttpPicEx");
	m_pWndHeadPic = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndHeadPic->Align();
	m_pWndHeadPic->SetUnClickable(TRUE);
	m_pWndHeadPic->SetInvisible(TRUE);
	//加载频道选择
	tstring strPath3 = g_strLocalPath + _T("\\ui\\chat_optinput.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath3.c_str());
	m_pWndChannel = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndChannel->Align();

	//加载世界发言扣钱
	tstring strPath4 = g_strLocalPath + _T("\\ui\\common10.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath4.c_str());
	m_pWndWorld =  m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndWorld->SetInvisible(TRUE);

	//加载扩展菜单
	tstring strPath5 = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath5.c_str());
	m_pGUI->ChangeXml(&element, "expand", "Name", "expandforchat");
	m_pWndMenuEx = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndMenuEx->SetInvisible(TRUE);

	//加载密语聊天窗口
	MiniChatReloadUI();

	//----------------------------隐藏按钮----------------------------
	if (P_VALID(m_pWndHide))
		m_pGUI->AddToDestroyList(m_pWndHide);
	tstring strPath10 = g_strLocalPath + _T("\\ui\\chat_fold.xml");
	XmlElement eleMenu10;
	if( !m_pGUI->LoadXml( &eleMenu10, "VFS_System", strPath10.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml(&element, "chat_fold_win\\show", "ClassName", "GlintButton");
	m_pWndHide = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu10 );
	m_pBn_Hide = (GlintButton*)m_pWndHide->GetChild( _T("show") );
	m_pWndHide->SetInvisible(TRUE);
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndChannel->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndWorld->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndMenuEx->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::GUIEventHandlerMenu));
	m_pGUI->RegisterEventHandler(m_pWndImName->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::GUIEventHandlerMenu));
	m_pGUI->RegisterEventHandler(m_pMiniChatWnd->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndHide->GetFullName().c_str(), m_Trunk.sfp1(&ChatFrame::EventHandler));

	//公告
	m_pStcAffiche   	= (GUIStatic*)m_pWnd->GetChild(_T("proclaimback"));	 
	m_pShowAffiche		= (RichEditBoxForChatShow*)m_pStcAffiche->GetChild(_T("words"));
	m_pShowAffiche->ClsText();
	m_pShowAffiche->SetKeyWordDownLine(TRUE);
	m_pShowAffiche->BeFlex();
	m_pShowAffiche->SetInvisible(FALSE);
	m_pWndCaption		= (GUIStatic*)m_pWnd->GetChild(_T("back"));	
	m_pBtnHide			= (GUIButton*)m_pWndCaption->GetChild(_T("hide"));
	m_pBtnChannelSet	= (GUIButton*)m_pWndCaption->GetChild(_T("chatset"));
	//m_pBtnLeave			= (GlintButton*)m_pWndCaption->GetChild(_T("message"));
	m_pBtnMiniChat      = (GlintButton*)m_pWndCaption->GetChild(_T("minichat"));     //初始化密语按钮
	m_pBtnGroupChat    = (GlintButton*)m_pWndCaption->GetChild(_T("groupchat"));   //群聊
	
	//m_pBtnLeave->SetEnable(false);
	//m_pBtnHide->SetText(g_StrTable[_T("Chat_Hide")]);
	m_pBtnZoomUp		= (GUIButton*)m_pWndCaption->GetChild(_T("zoom0"));
	m_pBtnZoomDn		= (GUIButton*)m_pWndCaption->GetChild(_T("zoom"));
	m_pBtnHideOthers	= (GUIPushButton*)m_pWndCaption->GetChild(_T("hide"));
	m_pMidBack			= (GUIPatch*)m_pWndCaption->GetChild(_T("new0"));
	m_pBn_StyleAction   = (GUIButton*)m_pWndCaption->GetChild( _T("new0\\actionbutton") );
	m_pBtnCurChannel	= (GUIButton*)m_pWndCaption->GetChild(_T("new0\\pic\\inputchoosebutton"));
	m_pBtnEmotion		= (GUIButton*)m_pWndCaption->GetChild(_T("new0\\emobutton"));
	tstring strEmotion = g_StrTable[_T("HotKey_Emotion")];
	strEmotion += _T("(X)");
	m_pBtnEmotion->SetTips(strEmotion.c_str());
	m_pStaticLangue[0]	= (GUIStatic*)m_pWndCaption->GetChild(_T("new0\\pic\\showenglish"));
	m_pStaticLangue[1]	= (GUIStatic*)m_pWndCaption->GetChild(_T("new0\\pic\\showsino"));
	m_pRebInput			= (RichEditBoxForChatInput*)m_pWndCaption->GetChild(_T("new0\\pic\\new"));
	//m_pInpuBack			= (GUIStatic*)m_pWndCaption->GetChild(_T("new0\\inpupict"));
	m_pRebInput->SetMaxTextCount(MAX_CHAT_LEN/2);								//最多输入100个字
	m_pRebInput->ClsText();
	m_pRebInput->SetShowItemOrEquip(true);
	m_pMutex			= (GUIFrameMutex*)m_pWndCaption->GetChild(_T("showchoose"));
	m_pChannelSel		= (GUIListBox*)m_pWndChannel->GetChild(_T("inputchoose"));		//发言频道选择
	m_pChannelSel->SetColNum(1, 39);
	m_pChannelSel->SetText(0, 0, g_StrTable[_T("Chat_World")]);
	m_pChannelSel->SetText(1, 0, g_StrTable[_T("Chat_Pvt")]);
	m_pChannelSel->SetText(2, 0, g_StrTable[_T("Chat_Guild")]);
	m_pChannelSel->SetText(3, 0, g_StrTable[_T("Chat_Troop")]);
	m_pChannelSel->SetText(4, 0, g_StrTable[_T("Chat_Current")]);
	m_pChannelSel->SetText(5, 0, g_StrTable[_T("Chat_Scene")]);
	m_pChannelSel->SetText(6, 0, g_StrTable[_T("Chat_Family")]);
	m_pChannelSel->SetInvisible(TRUE);
	m_pWndChannel->SetInvisible(TRUE);

	TCHAR temp[256] = {0};
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		_stprintf(temp, _T("showchoose\\chatshowbutton%d"), i);
		//m_pPage[i]		= (GUIPushButton*)m_pWndCaption->GetChild(temp);					//对应显示频道的pushbutton
		m_pPage[i]		= (GlintPushButton*)m_pWndCaption->GetChild(temp);
		if( P_VALID(m_pPage[i]) )
		{
			m_pPage[i]->SetGlintTextColor( 0xff00ff00 );
		}
		
		_stprintf(temp, _T("showchat%d"), i);
		m_pChatContext[i]	= (RichEditBoxForChatShow*)m_pWndCaption->GetChild(temp);		//显示频道
		m_pChatContext[i]->BeFlex();														
		m_pChatContext[i]->ClsText();
		m_pChatContext[i]->SetKeyWordDownLine(TRUE);
		if(i>0)
			m_pChatContext[i]->SetInvisible(TRUE);											//初始化时只显示一个显示频道

		_stprintf(temp, _T("showchat%d\\scrollobar%d"), i, i);
		m_pScl[i]			= (GUIScrollBar*)m_pWndCaption->GetChild(temp);					//滚动条
		_stprintf(temp, _T("showchat%d\\scrollobar%d\\up%d"), i, i, i);
		m_pBtnUp[i]			= (GUIButton*)m_pWndCaption->GetChild(temp);					//滚动条向上按钮
		_stprintf(temp, _T("showchat%d\\scrollobar%d\\down%d"), i, i, i);
		m_pBtnDp[i]			= (GUIButton*)m_pWndCaption->GetChild(temp);					//滚动条向下按钮
		m_pBtnDp[i]->Align();
		m_pScl[i]->SetValue(5,5,0);
	}

	m_pMutex->SetPush(m_pPage[0]);
	m_pChatContext[0]->SetInvisible(FALSE);
	//m_pPage[7]->SetInvisible(TRUE);															//同城按钮先隐藏，以后扩展用

	m_curSendChannel = ESCC_Current;														//设置发言频道为当前
	SetCurChannel(m_curSendChannel);

	//------------------------------------------------------------------------------------------
	//创建跑马灯
	m_pScrollRichText	= (ScrollRichEditBox*)m_pSclFatherWnd->GetChild(_T("sclricheditbox"));
	
	m_pScrollRichText->SetCanSelect(FALSE);
	m_pScrollRichText->SetShowCursor(FALSE);
	m_pScrollRichText->ClsText();
	m_pSclFatherWnd->SetInvisible(FALSE);
	m_pScrollRichText->SetInvisible(TRUE);
	LONG lnSizeX = m_pSclFatherWnd->GetSize().x;
	LONG lnViewX = m_pSclFatherWnd->GetView().x;
	m_lnSlcFatherWndX = lnSizeX + lnViewX;					//计算跑马灯开始的位置，即聊天窗口最右端
	m_pScrollRichText->SetStartPos(m_lnSlcFatherWndX, 0);
	

	//创建跑马灯2
	m_pRunHorseScrollText	= (ScrollRichEditBox*)m_pRunHorseWnd->GetChild(_T("sclfatherwnd\\sclricheditbox"));
	m_pRunHorseScrollText->SetCanSelect(FALSE);
	m_pRunHorseScrollText->SetShowCursor(FALSE);
	m_pRunHorseScrollText->ClsText();
	m_pRunHorseWnd->SetInvisible(FALSE);
    m_pRunHorseWnd->SetUnClickable(TRUE);
	m_pRunHorseScrollText->SetInvisible(TRUE);
	LONG lnSizeX2 = m_pRunHorseWnd->GetSize().x;
	LONG lnViewX2 = m_pRunHorseWnd->GetView().x;
	m_lnSlcFatherWndX = lnSizeX2 + lnViewX2;					//计算跑马灯开始的位置，即聊天窗口最右端
	//LONG lnSizeY = m_pRunHorseWnd->GetSize().y;
	//LONG lnViewY = m_pRunHorseWnd->GetView().y;
	//m_lnSlcFatherWndY = lnSizeY + lnViewY;	
	m_pRunHorseScrollText->SetStartPos(m_lnSlcFatherWndX, 0/*m_lnSlcFatherWndY*/);

	//传音
	m_lnCastWndX = m_pStcCast->GetSize().x + m_pStcCast->GetView().x;
	for(int i=0; i<NUM_CHAT_CAST_WND; i++)
	{
		TCHAR szBuff[32];
		_stprintf(szBuff, _T("talk%d"), i+1);
		m_pCastRichText[i] = (ScrollRichEditBox*)m_pStcCast->GetChild(szBuff);
		m_pCastRichText[i]->SetCanSelect(FALSE);
		m_pCastRichText[i]->SetShowCursor(FALSE);
		m_pCastRichText[i]->ClsText();
		m_pCastRichText[i]->SetInvisible(TRUE);
		m_pCastRichText[i]->SetStartPos(m_lnCastWndX, (NUM_CHAT_CAST_WND-1-i)*19);

		_stprintf( szBuff, _T("name%d"), i+1 );
		m_pStcName[i] = (StaticForCast*)m_pWndImName->GetChild(szBuff);

		_stprintf( szBuff, _T("head%d"), i+1 );
		m_pStcHeadPic[i] = (HttpPicEx*)m_pWndHeadPic->GetChild(szBuff);
		m_pStcHeadPic[i]->SetUnClickable(FALSE);
		m_pStcHeadPic[i]->SetInvisible(TRUE);
	}

	m_pBtnWorldOK = (GUIButton*)m_pWndWorld->GetChild(_T("comm10_pic\\surebutt"));
	m_pBtnWorldOK->SetText(g_StrTable[_T("Common_OK")]);
	m_pStcExWold = (GUIStaticEx*)m_pWndWorld->GetChild(_T("comm10_pic\\comm10_words1"));
	m_pBtnWorldCancel = (GUIButton*)m_pWndWorld->GetChild(_T("comm10_pic\\canclebutt"));
	m_pBtnWorldCancel->SetText(g_StrTable[_T("Common_Cancel")]);
	m_pPBtnWorld = (GUIPushButton*)m_pWndWorld->GetChild(_T("comm10_pic\\new"));

	m_pListMenuEx = (GUIListBox*)m_pWndMenuEx->GetChild(_T("listbutton1"));
	m_pListMenuEx->SetColWidth(0, m_pListMenuEx->GetSize().y);
	ForceResizeMenu( m_pListMenuEx, 7 );
	m_pListMenuEx->SetText( 0, 0, g_StrTable[_T("Chat_Menu_0")] );
	m_pListMenuEx->SetText( 1, 0, g_StrTable[_T("Chat_Menu_1")] );
	m_pListMenuEx->SetText( 2, 0, g_StrTable[_T("Chat_Menu_2")] );
	m_pListMenuEx->SetText( 3, 0, g_StrTable[_T("Chat_Menu_3")] );
	m_pListMenuEx->SetText( 4, 0, g_StrTable[_T("Chat_Menu_4")] );
	m_pListMenuEx->SetText( 5, 0, g_StrTable[_T("Chat_Menu_5")] );
	m_pListMenuEx->SetText( 6, 0, g_StrTable[_T("Chat_Menu_6")] );
    //m_pListMenuEx->SetText( 5, 0, g_StrTable[_T("TargetMenu_13")] );
    //m_pListMenuEx->SetText( 6, 0, g_StrTable[_T("TargetMenu_14")] );

 	ResetRecvChannel();										//加载显示频道
	//设置一些初始的界面状态
	if( ::ImmGetDescription(::GetKeyboardLayout(0),NULL,0)==0 )
	{
		//输入法关闭状态
		SwitchLangueInfo(true);
	}
	else
	{
		SwitchLangueInfo(false);
	}

	return TRUE;
}

VOID ChatFrame::Render3D()
{
	ChatShowPop::Inst()->DrawInfo();
}

// !Update
VOID ChatFrame::Update()
{
	float countTime = Kernel::Inst()->GetAccumTime();
	////处理缓存的传音
	//for(int i=0; i<NUM_CHAT_CAST_WND; ++i)
	//{
	//	if( m_pCastRichText[i]->GetContinueScrText() )
	//	{
	//		if( P_VALID(m_pStcName[i]) && !m_pStcName[i]->GetText().empty() )
	//		{
	//			m_pStcName[i]->SetText(_T(""));
	//			m_pWndImName->SetRefresh(TRUE);
	//			m_pStcHeadPic[i]->SetInvisible(TRUE);
	//		}
	//		vector<tagScroText>::iterator it=m_vecCast.begin(); 
	//		if(it!=m_vecCast.end())
	//		{
	//			m_pCastRichText[i]->SetContinueScrText();
	//			m_pCastRichText[i]->PushScrollText(it->szInfo.c_str(), it->fTime);						
	//			m_pStcName[i]->SetText(it->szName.c_str());	
	//			m_pWndImName->SetUnClickable(FALSE);
	//			if( !it->url.empty() )
	//			{
	//				m_pStcHeadPic[i]->SetPicURL(it->url.c_str());
	//				m_pStcHeadPic[i]->SetInvisible(FALSE);
	//			}
	//			m_vecCast.erase(it);
	//		}
	//	}
	//}

	////所有传音都为空时才不可点
	//int nCastEmpty = 0;
	//for(int i=0; i<NUM_CHAT_CAST_WND; ++i)
	//{
	//	if( P_VALID(m_pStcName[i]) && m_pStcName[i]->GetText().empty() )
	//		++nCastEmpty;
	//}
	//if( nCastEmpty == NUM_CHAT_CAST_WND )
	//	m_pWndImName->SetUnClickable(TRUE);
	
	//处理缓存的跑马灯消息，处理先进入的跑马灯
	if( m_pScrollRichText->GetContinueScrText() )
	{	
		vector<tagScroText>::iterator itSclTex = m_vecSclText.begin();
		if(itSclTex!=m_vecSclText.end())
		{
			m_pScrollRichText->SetContinueScrText();
			m_pScrollRichText->PushScrollText(itSclTex->szInfo.c_str(), (*itSclTex).fTime);		

			m_vecSclText.erase(itSclTex);
		}
	}


	//处理缓存的BIG跑马灯消息，处理先进入的跑马灯
	if( m_pRunHorseScrollText->GetContinueScrText() )
	{	
		vector<tagScroText>::iterator itSclTex = m_vecBigSclText.begin();
		if(itSclTex!=m_vecBigSclText.end())
		{
			m_pRunHorseScrollText->SetContinueScrText();
			m_pRunHorseScrollText->PushScrollText(itSclTex->szInfo.c_str(), (*itSclTex).fTime,(*itSclTex).dwColour);

			m_dwCurrentRunHorsePriority = itSclTex->dwRoleID;		//这里利用RoleID来传递优先级
			m_vecBigSclText.erase(itSclTex);
		}
		else{
			m_dwCurrentRunHorsePriority = 0;
		}
	}

	//保存半分钟
	for(vector<tagSendSpeak>::iterator it=m_vecSpeak.begin();
		it!=m_vecSpeak.end(); )
	{
		if( it->eChannel==ESCC_Current && it->fSendTime<=countTime-(float)MIN_REPEAT_SPEAK_TIME )
		{
			it = m_vecSpeak.erase(it);
			continue;
		}
		else if( it->eChannel==ESCC_World && it->fSendTime<=countTime-(float)MIN_WORLD_RESPEAK_TIME )
		{
			it = m_vecSpeak.erase(it);
			continue;
		}
		/*else if( it->eChannel==ESCC_CastTalk && it->fSendTime<=countTime-(float)MIN_RUNHORSE_RESPEAK_TIME )
		{
			it = m_vecSpeak.erase(it);
			continue;
		}*/
		else if( it->eChannel==ESCC_Troop && it->fSendTime<=countTime-(float)MIN_RUNHORSE_RESPEAK_TIME )
		{
			it = m_vecSpeak.erase(it);
			continue;
		}
		else if( it->eChannel==ESCC_Guild && it->fSendTime<=countTime-(float)MIN_RUNHORSE_RESPEAK_TIME )
		{
			it = m_vecSpeak.erase(it);
			continue;
		}

		++it;
	}


	// 防止滚动菜单
	if( 0 != m_pListMenuEx->GetCurrentTextRow() )
		m_pListMenuEx->SetCurrentTextRow( 0 );

	if(SaveAccount::Inst()->GetLoginMode() == 11)
		UpDateFangChenMi();
	
	GameFrame::Update();
}

BOOL ChatFrame::EscCancel()
{
	if(!m_pMiniChatWnd->IsInvisible())
	{
		m_pMiniChatWnd->SetInvisible(TRUE);
		m_pGUI->SetActive(m_pWnd);
		return TRUE;
	}
	return FALSE;
}

// !切换中英文提示图标,isEnglish:是否是英文
VOID ChatFrame::SwitchLangueInfo(bool isEnglish)
{
	if(m_pStaticLangue[0]==NULL || m_pStaticLangue[1]==NULL)
		return;
	//切换显示
	m_pStaticLangue[0]->SetInvisible(!isEnglish);
	m_pStaticLangue[1]->SetInvisible(isEnglish);
}

DWORD ChatFrame::OnGameEventForChat( tagGameEventForChat* pEvent )
{
	if( !P_VALID(m_pRebInput) )
		return 0;
	tstring strEmnTran = pEvent->strEmotion;
	AddInputEmotion(strEmnTran);
	return 0;
}

BOOL ChatFrame::GUIEventHandlerMenu(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch( pEvent->eEvent )
	{
	case EGUIE_Deactive:                                        // 鼠标点击其他区域，关闭扩展菜单
		if( !m_pWndMenuEx->IsInvisible()
			&& pEvent->dwParam1 != (DWORD)m_pWndMenuEx 
			&& pEvent->dwParam1 != (DWORD)m_pListMenuEx  )
		{
			m_pWndMenuEx->SetInvisible( TRUE );
		}
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pListMenuEx )                             // 聊天扩展菜单条目被点击
		{
			switch(pEvent->dwParam1)
			{
			case 0:
				{
					tagInviteJoinTeamEvent evt;
					evt.dwRoleID = PlayerNameTab::Inst()->FindIDByName(m_strMenuName.c_str());
					m_pFrameMgr->SendEvent(&evt);
				}
				break;
			case 1:
				{
					tagAddFriendEvent evt(_T("SocialEvent_AddFriend"), NULL);
					evt.roleID = PlayerNameTab::Inst()->FindIDByName(m_strMenuName.c_str());
					m_pFrameMgr->SendEvent(&evt);
				}
				break;
			case 2:
				{
					DWORD dwTargetRoleID = PlayerNameTab::Inst()->FindIDByName(m_strMenuName.c_str());
					DWORD dwReaseon = GT_INVALID;
					if( IS_PLAYER(dwTargetRoleID) && !GuildMgr::Inst()->IsCanJionReq(dwTargetRoleID, &dwReaseon) )
					{
						TCHAR szBuff[X_LONG_NAME] = {0};
						switch(dwReaseon)
						{
						case GuildMgr::ERQ_NoSelfRight:
							_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("Chat_Menu_GuildJoinError0")]);
							break;
						case GuildMgr::ERQ_AlreadyInSelfGuild:
							_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("Chat_Menu_GuildJoinError1")], m_strMenuName.c_str());
							break;
						case GuildMgr::ERQ_AlreadyInOtherGuild:
							_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("Chat_Menu_GuildJoinError2")], m_strMenuName.c_str());
							break;
						case GuildMgr::ERQ_OtherReason:
							_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("Chat_Menu_GuildJoinError0")]);
							break;
						}
						PushInfo(szBuff, ESCC_System);
						m_pWndMenuEx->SetInvisible( TRUE );
						return FALSE;
					}
					tagInviteJoinGuildEvent evt;
					evt.dwRoleID = PlayerNameTab::Inst()->FindIDByName(m_strMenuName.c_str());
					m_pFrameMgr->SendEvent(&evt);
				}
				break;
			case 3:
				{
					tagAddBlackEvent evt(_T("SocialEvent_AddBlack"), NULL);
					evt.roleID = PlayerNameTab::Inst()->FindIDByName(m_strMenuName.c_str());
					m_pFrameMgr->SendEvent(&evt);
				}
				break;
			case 4:
				{					
					tagApplyJoinTeamEvent evt;
					evt.dwRoleID = PlayerNameTab::Inst()->FindIDByName( m_strMenuName.c_str() );
					m_pMgr->SendEvent( &evt );
				}
                break;
            case 5:
                {
                   /* tagGetAquireMasterEvent e(_T("tagGetAquireMasterEvent"), this);
                    e.roleID = PlayerNameTab::Inst()->FindIDByName( m_strMenuName.c_str() );
                    m_pMgr->SendEvent(&e);*/
						tagFamilyInviteEvent evt(_T("FamilyEvent_Invite"), this);
						evt.dwRoleID		= PlayerNameTab::Inst()->FindIDByName( m_strMenuName.c_str() );
						evt.strRoleName	= m_strMenuName;
						m_pMgr->SendEvent(&evt);
                }
                break;
            case 6:
                {
                    /*tagAquireApprenticeEvent e(_T("tagAquireApprenticeEvent"), this);
                    e.roleID = PlayerNameTab::Inst()->FindIDByName( m_strMenuName.c_str() );
                    m_pMgr->SendEvent(&e);*/
						tagFamilyApplyEvent evt(_T("FamilyEvent_Apply"), this);
						evt.dwRoleID = PlayerNameTab::Inst()->FindIDByName( m_strMenuName.c_str() );
						evt.strRoleName	= m_strMenuName;
						m_pMgr->SendEvent(&evt);
                }
                break;
			default:
				break;
			}
			m_pWndMenuEx->SetInvisible( TRUE );
		}
	case EXUIE_SFCMouseClick:
		{
			for(int i=0; i<NUM_CHAT_CAST_WND; i++)
			{
				if( pWnd == m_pStcName[i] )
				{
					TCHAR szKey[X_SHORT_NAME]={0};
					_tcsncpy( szKey,  m_pStcName[i]->GetText().c_str(), X_SHORT_NAME );
					if(szKey==NULL || _tcslen(szKey)==0)
						return FALSE;
					if( _tcscmp(szKey, m_strLocalName.c_str())==0 )
						return FALSE;
					this->ForceMoveMenu(pEvent->msg.pt);
					m_strMenuName = szKey;
				}
			}
		}
		break;
	case EXUIE_SFCMouseRClick:
		{
			for(int i=0; i<NUM_CHAT_CAST_WND; i++)
			{
				if( pWnd == m_pStcName[i] )
				{
					const tstring& strName = m_pStcName[i]->GetText();
					if(strName.empty())
						return FALSE;
					TCHAR szKey[X_SHORT_NAME]={0};
					_tcsncpy( szKey, strName.c_str(), X_SHORT_NAME );
					_tcscat( szKey, _T(" ") );
					m_strLastName = szKey;
					m_curSendChannel = ESCC_Pvt;
					SetCurChannel(m_curSendChannel);
					m_pRebInput->ClsText();
					m_pRebInput->AddText(szKey);	
					m_pGUI->SetActive(m_pRebInput);
					if(m_bChatContext)
					{
						HideWnd();
					}
				}
			}
		}
		break;
	}
	return TRUE;
}

//事件处理
DWORD ChatFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{	
	case EXUIE_ShowItemOrEquip:
		{
			m_ShowEvent.dwParam1 = pEvent->dwParam1;
			m_ShowEvent.dwParam2 = pEvent->dwParam2;
			m_ShowEvent.dwParam3 = pEvent->dwParam3;
			m_ShowEvent.dwParam4 = pEvent->dwParam4;
			if( m_bWorldOK==FALSE && m_curSendChannel==ESCC_World)
			{
				//m_pStcExWold->SetText(g_StrTable[_T("Chat_WorldMoney")]);
				m_pStcExWold->SetText(g_StrTable[_T("E_RoleChat_World_NoIMItem")]);

              /* if ( HaveWorldSpeakTool() == FALSE )
               {
				   m_pWndWorld->SetInvisible(FALSE);
				   m_pWndWorld->FlipToTop();
               }      */ 
				m_eSendMsgState = SendMsgState_Show;
				//return TRUE;
			}
			OnRoleSendItemOrEquip(&m_ShowEvent);
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUIE_RichLinkClick:					//显示展示的装备或物品Tips
		{
			if( pWnd==m_pChatContext[m_curRecvChannel] )
			{
				tstring strText;
				BOOL bGetLinkText = m_pChatContext[m_curRecvChannel]->GetLinkContent(pEvent->dwParam2, strText);
				if( TRUE == bGetLinkText && !strText.empty())
				{
					if(pEvent->dwParam1 == WM_LBUTTONDOWN)
					{
						if (_tcsnccmp(strText.c_str(), _T("tagItem"), 7) == 0)
						{
							DWORD dwTypeID = ChatShowItemEquip::Inst()->FindItemData(strText.c_str());
							if( dwTypeID==GT_INVALID || dwTypeID==0 )
								return FALSE;
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(dwTypeID).c_str());						
						}
						else if (_tcsnccmp(strText.c_str(), _T("tagEquip"), 8) == 0)
						{
							Equipment* pEquip = const_cast<Equipment*>(ChatShowItemEquip::Inst()->FindEquipData(strText.c_str()));
							if( !P_VALID(pEquip) )
								return FALSE;
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
							ToolTipCreator::Inst()->ShowEquipCompare(pEquip->GetItemTypeID());/*
							if(pEquip->pEquipProto->eType != EIT_FaBao)
							{
								Equipment equip(*pEquip);
								ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(&equip).c_str());
								ToolTipCreator::Inst()->ShowEquipCompare(equip.GetItemTypeID());
							}
							else
							{
								const tagFabao* pFabao = (const tagFabao*)pEquip;
								Equipment equip(*pFabao);
								ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(&equip).c_str());
								ToolTipCreator::Inst()->ShowEquipCompare(equip.GetItemTypeID());
							}*/
						}
					}
				}
			}
		}
		break;
	case EXUIE_MouseDoubleClick:				//取关键字,双击聊天框内的角色的名字弹出密语框
		{
			if( pWnd==m_pChatContext[m_curRecvChannel] )
			{
				TCHAR szKey[_MAX_PATH]={0};
				int nStart=0, nEnd=0;
				m_pChatContext[m_curRecvChannel]->GetCursorKey(szKey, nStart, nEnd);
				if(szKey==NULL || _tcslen(szKey)==0)
					return FALSE;
				if( _tcscmp(szKey, m_strLocalName.c_str())==0 )
					return FALSE;
				m_strLastName = szKey;
				//_tcscat(szKey, _T(" "));
				m_curSendChannel = ESCC_Pvt;
				SetCurChannel(m_curSendChannel);
				m_pRebInput->ClsText();
				//m_pRebInput->AddText(szKey);
				if(m_pMiniChatWnd->IsInvisible())
					m_pMiniChatWnd->SetInvisible(FALSE);
				tstring temp = szKey;
				setNowChatWith(temp,TRUE);
				SetPvtRoleName(temp.c_str());
				m_strLastName = temp;
				m_pGUI->SetActive(m_pEB_SendMsg);  //使密语框获得焦点
				if(m_bChatContext)
				{
					HideWnd();
				}
			}
		}
		break;
	case EXUIE_MouseRClick:
		{
			if( pWnd==m_pChatContext[m_curRecvChannel] )
			{
				TCHAR szKey[_MAX_PATH]={0};
				int nStart=0, nEnd=0;
				m_pChatContext[m_curRecvChannel]->GetCursorKey(szKey, nStart, nEnd);
				if(szKey==NULL || _tcslen(szKey)==0)
					return FALSE;
				if( _tcscmp(szKey, m_strLocalName.c_str())==0 )
					return FALSE;
				this->ForceMoveMenu(pEvent->msg.pt);
				m_strMenuName = szKey;

			}
		}
		break;
	case EXUIE_ContextChange:													//处理输入框文本变化
		{
			if( pWnd == m_pRebInput )
			{
				OnInputContextChange();
			}
		}
		break;
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnCurChannel )					//点击当前发言频道按钮
			{
				BOOL b = m_pWndChannel->IsInvisible();
				m_pChannelSel->SetInvisible(!b);
				m_pWndChannel->SetInvisible(!b);
				m_pGUI->SetActive(m_pChannelSel);
			}	
			else if( pWnd == m_pBtnHide )					//点击隐藏按钮
			{	
				//m_bHide = !m_bHide;
				HideWnd();
			}	
			else if ( pWnd == m_pBn_Hide )				//点击隐藏还原按钮
			{
				ShowWnd();
			}
			else if( pWnd == m_pBtnChannelSet )
			{
				ChannelSet();
			}
			else if( pWnd == m_pBtnMiniChat)               //点击密语按钮
			{
				ShowMiniChat();

				if(P_VALID(m_pBtnMiniChat))
					m_pBtnMiniChat->EndGlint();       //按钮停止闪烁
			}
			else if( pWnd == m_pBtnGroupChat )				//点击群聊
			{
				if( m_pBtnGroupChat->IsGlintNow())
				{
					m_pBtnGroupChat->EndGlint();
				}
				if (P_VALID(m_pFrameMgr->GetFrame(_T("MultChat"))))
				{
					m_pMultChatFrame = (MultChatFrame*) m_pFrameMgr->GetFrame(_T("MultChat"));
					bOpenFlag = m_pMultChatFrame->FrameIsInvisible();
				}
				if( bOpenFlag == TRUE )
				{
					m_pMgr->SendEvent( &tagGameEvent( _T("Open_MultChatFrame"), this ) );
					bOpenFlag = FALSE;
				}
				else
				{
					m_pMgr->SendEvent( &tagGameEvent( _T("Close_MultChatFrame"), this ) );
					bOpenFlag = TRUE;
				}
				
			}
			/*else if( pWnd == m_pBtnLeave )
			{
				if( m_pBtnLeave->IsGlintNow() )
				{
					m_pBtnLeave->EndGlint();
				}
				tagNC_LoadLeftMsg msg;
				m_pSession->Send(&msg);
				m_pBtnLeave->SetEnable(false);
			}*/
			else if( pWnd == m_pBtnEmotion )				//点击表情按钮
			{
				BOOL bShow = m_pEmotionFrame->IsInvisibleFrame();
				m_pEmotionFrame->SetInVisibleFrame(!bShow);
			}
			else if( pWnd == m_pBn_StyleAction )		   //点击动作按钮
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("OpenClose_StyleAction"), this ) );
			}
			else if( pWnd == m_pBtnZoomUp )
			{
				m_nZoomMax++;
				if( m_nZoomMax <= NUM_CHAT_ZOOM_MAX )
				{
					m_pChatContext[0]->ZoomWnd(65);
					MilleSoundFrame* pMilleFrame = (MilleSoundFrame*)m_pFrameMgr->GetFrame( _T("MilleSound") );
					if( P_VALID(pMilleFrame) )
					{
						pMilleFrame->Move( tagPoint( 0,-65 ) );
					}
				}
				else 
					m_nZoomMax = NUM_CHAT_ZOOM_MAX;

                m_pWnd->Align();
                m_pWnd->SetRefresh(TRUE);
			}
			else if( pWnd == m_pBtnZoomDn )
			{
				m_nZoomMax--;
				if( m_nZoomMax >= NUM_CHAT_ZOOM_MIN)
				{
					m_pChatContext[0]->ZoomWnd(-65);
					MilleSoundFrame* pMilleFrame = (MilleSoundFrame*)m_pFrameMgr->GetFrame( _T("MilleSound") );
					if( P_VALID(pMilleFrame) )
					{
						pMilleFrame->Move( tagPoint( 0,65 ) );
					}
				}
				else 
					m_nZoomMax = NUM_CHAT_ZOOM_MIN;

                m_pWnd->Align();
                m_pWnd->SetRefresh(TRUE);
			}
			else if( pWnd == m_pBtnWorldCancel )
			{
				m_bWorldOK = FALSE;
				m_pWndWorld->SetInvisible(TRUE);
			}	
			else if( pWnd == m_pBtnWorldOK )
			{
				m_bWorldOK = (BOOL)m_pPBtnWorld->GetState();
				switch(m_eSendMsgState)
				{
				case SendMsgState_Chat:
					OnSendWorld();
					break;
				case SendMsgState_Show:
					OnRoleSendItemOrEquip(&m_ShowEvent);
					break;
				}
				m_pWndWorld->SetInvisible(TRUE);
				this->SetActive();
			}
			//----------------------------------------------------密语事件处理----------------------------------------------------------------------------
			else if( pWnd == m_pBtn_Exit)
			{
				m_pMiniChatWnd->SetInvisible(TRUE);
				m_pGUI->SetActive(m_pWnd);
			}
			else if( pWnd == m_pBtn_Send )
			{
				INT len = 0;
				tstring sendMsg = m_pEB_SendMsg->GetText(len);
				if( len > 0&& !sendMsg.empty())
				{
					m_NowRoleID = PlayerNameTab::Inst()->FindIDByName(m_NowChatWith.c_str());
					if(m_NowRoleID != GT_INVALID && len != -1)
					{
						tstring strtemp;   //用于清理自动添加的回车符用的
						OnSendChatMsg(sendMsg,ESCC_Pvt,m_NowRoleID,0);
						m_pEB_SendMsg->ClsText();
						TCHAR msg[150] = {0};
						tstring tempcolor;
						tempcolor.append(_T("<color=(0,255,0)>"));
						tempcolor.append(m_NowChatWith);
						tempcolor.append(_T("<color=(255,255,255)>"));
						m_pEB_SendMsg->FilterString(sendMsg);          //过滤颜色等设置
						FilterData::Inst()->WordsFilteror(sendMsg);    //屏蔽字
						_stprintf( msg, g_StrTable[_T("Chat_Youto")],tempcolor.c_str(),sendMsg.c_str() );
						strtemp += msg;
						DeleteRETURNS(strtemp);
						strtemp += _T("\n");
						//--------添加数据应该先判断一下list中是否有此人信息------------
						BOOL isfind = FALSE;
						for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
						{
							if(m_NowRoleID == (*MiniChatMsgit).RoleID)
							{
								isfind = TRUE;
							}
							else 
							{
								MiniChatMsgit++;
							}
						}
						if(!isfind)
						{
							tagMiniMsg temp;
							temp.RoleID  = m_NowRoleID;
							temp.Minimsg += strtemp;
							temp.isRead = true;
							MiniChatMsg.push_front(temp);
							m_pWndText->AddText(strtemp.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);                                 //如果没找到就把当前信息输出显示并存入list
						}
						else 
						{
							m_pWndText->ClsText();
							(*MiniChatMsgit).Minimsg += strtemp;
							(*MiniChatMsgit).isRead = true;
							m_pWndText->AddText((*MiniChatMsgit).Minimsg.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);        //如果找到就把信息加上当前输入的信息然后显示出来
						}
						m_pWndText->SetRefresh(TRUE); 
						if(m_pPB_CloseTag->GetState() == EGUIBS_PushDown)
						{
							m_pMiniChatWnd->SetInvisible(TRUE);
							m_pGUI->SetActive(m_pWnd);
						}
					}
					m_pGUI->SetActive(m_pEB_SendMsg);
				}
				else 
				{
					m_pWndText->AddText(g_StrTable[_T("Chat_MsgisNull")], FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE); 
					m_pWndText->AddText(_T("\n"));
				}
			}
			else if( pWnd == m_pBtn_SetObj )     //基本完成
			{
				m_NowChatWith = m_pEB_SetObj->GetText();
				m_pEB_SetObj->SetText(_T(""));
				m_pSE_ShowObjName->SetText(_T(""));
				m_pWndText->ClsText();
				m_NowRoleID =  PlayerNameTab::Inst()->FindIDByName(m_NowChatWith.c_str());
				if( m_NowRoleID != GT_INVALID)
					setNowChatWith(m_NowChatWith.c_str(),FALSE);
				else 
				{
					float fSendTime = Kernel::Inst()->GetAccumTime();
					tagSendSpeak newSpeak;
					newSpeak.fSendTime = fSendTime;
					newSpeak.eChannel  = m_curSendChannel;
					newSpeak.strDestName = m_NowChatWith;
					newSpeak.strMsg.assign(_T(""));	
					newSpeak.byAutoReply = 0;

					//保存发言
					m_vecPvtSpeak.push_back(newSpeak);
				}
				m_pGUI->SetActive(m_pEB_SendMsg);
			}
			//--------------------------------------------------------------------------------------------------------------------------------------------
		}
		
		break;
	case EGUIE_EditBoxEnter:
		{

			if( pWnd == m_pEB_SendMsg)
			{
				INT len = 0;
				tstring sendMsg = m_pEB_SendMsg->GetText(len);
				if( len > 0&& !sendMsg.empty())
				{
					m_NowRoleID = PlayerNameTab::Inst()->FindIDByName(m_NowChatWith.c_str());
					if(m_NowRoleID != GT_INVALID && len != -1&&m_NowRoleID != RoleMgr::Inst()->GetLocalPlayerID())
					{
						tstring strtemp;   //用于清理自动添加的回车符用的
						OnSendChatMsg(sendMsg,ESCC_Pvt,m_NowRoleID,0);
						m_pEB_SendMsg->ClsText();
						TCHAR msg[150] = {0};
						tstring tempcolor;
						tempcolor.append(_T("<color=(0,255,0)>"));
						tempcolor.append(m_NowChatWith);
						tempcolor.append(_T("<color=(224,247,255)>"));
						m_pEB_SendMsg->FilterString(sendMsg);          //过滤颜色等设置
						FilterData::Inst()->WordsFilteror(sendMsg); 
						_stprintf( msg, g_StrTable[_T("Chat_Youto")],tempcolor.c_str(),sendMsg.c_str() );
						strtemp += msg;
						DeleteRETURNS(strtemp);
						strtemp += _T("\n");
						//--------添加数据应该先判断一下list中是否有此人信息------------
						BOOL isfind = FALSE;
						for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
						{
							if(m_NowRoleID == (*MiniChatMsgit).RoleID)
							{
								isfind = TRUE;
							}
							else 
							{
								MiniChatMsgit++;
							}
						}
						if(!isfind)
						{
							tagMiniMsg temp;
							temp.RoleID  = m_NowRoleID;
							temp.Minimsg += strtemp;
							temp.isRead = true;
							MiniChatMsg.push_front(temp);
							m_pWndText->AddText(strtemp.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);                                 //如果没找到就把当前信息输出显示并存入list
						}
						else 
						{
							m_pWndText->ClsText();
							(*MiniChatMsgit).Minimsg += strtemp;
							(*MiniChatMsgit).isRead = true;
							m_pWndText->AddText((*MiniChatMsgit).Minimsg.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);        //如果找到就把信息加上当前输入的信息然后显示出来
						}
						m_pWndText->SetRefresh(TRUE); 
						if(m_pPB_CloseTag->GetState() == EGUIBS_PushDown)
						{
							m_pMiniChatWnd->SetInvisible(TRUE);
							m_pGUI->SetActive(m_pWnd);
						}
						m_pGUI->SetActive(m_pEB_SendMsg);
					}
				}
				else 
				{
					m_pWndText->AddText(g_StrTable[_T("Chat_MsgisNull")], FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE); 
					m_pWndText->AddText(_T("\n"));
				}
			}
			else if( pWnd == m_pRebInput )						//在输入框中按下回车
			{			
				//没有输入任何发言
				INT len = 0;
				tstring context = m_pRebInput->GetText(len);
				if(len <= 0 || context.empty())
				{
					if(m_bHide)
					{
						HideWnd();
					}
					this->SetActive();
					if( P_VALID(m_HKLenglish) )
					{		
						ActivateKeyboardLayout(( HKL )m_HKLenglish, KLF_SETFORPROCESS );
					}
					return FALSE;
				}

				//先判断是否为播放个性动作
				if(JudePersonalityAct())
					return FALSE;
				
				if( m_bWorldOK==FALSE && m_curSendChannel==ESCC_World )
				{
					//m_pStcExWold->SetText(g_StrTable[_T("Chat_WorldMoney")]);
					m_pStcExWold->SetText(g_StrTable[_T("E_RoleChat_World_NoIMItem")]);

					/*if ( HaveWorldSpeakTool() == FALSE )
					{
						m_pWndWorld->SetInvisible(FALSE);
						m_pWndWorld->FlipToTop();
					}   */

					m_eSendMsgState = SendMsgState_Chat;
				}
				switch(m_curSendChannel)
				{
				//case ESCC_CastTalk:
				//	OnSendCastTalk();
				//	break;
				case ESCC_World:
					OnSendWorld();
					break;
				case ESCC_Pvt:
					OnSendPvt();
					break;
				case ESCC_Guild:
					OnSendGuild();
					break;
				case ESCC_Troop:
					OnSendTroop();
					break;
				case ESCC_Current:
					OnSendCurrent();
					break;
				case ESCC_Scene:
					OnSendScene();
					break;
				case ESCC_Family:
					OnSendFamily();
					break;
				case ESCC_Affiche:
					OnSendAffiche();
					break;
				}
				m_pRebInput->SetRefresh(TRUE);
				if(m_bHide)
				{
					HideWnd();
				}
				this->SetActive();	
				if( P_VALID(m_HKLenglish) )
				{		
					ActivateKeyboardLayout(( HKL )m_HKLenglish, KLF_SETFORPROCESS );
				}
			}

		}
		break;
	case EGUIE_ItemClick:									//listbox 选择发言频道
		{
			if( pWnd == m_pChannelSel )
			{
				m_curSendChannel = (ESendChatChannel)pEvent->dwParam1;
				SetCurChannel(m_curSendChannel);
				m_pChannelSel->SetInvisible(TRUE);			//选择发言频道后，关闭listbox
				m_pWndChannel->SetInvisible(TRUE);
				m_pGUI->SetActive(m_pRebInput);				//使输入框获得焦点
				if( m_curSendChannel==ESCC_Pvt && m_strLastName!=_T(""))
				{
					this->SetPvtRoleName(m_strLastName.c_str());
				}
			}
			else if(pWnd == m_pLB_ChatObj)
			{

				if(P_VALID(m_pLB_ChatObj->GetText(pEvent->dwParam1,0)))
				{
					m_NowChatWith = m_pLB_ChatObj->GetText(pEvent->dwParam1,0);
					m_NowRoleID = PlayerNameTab::Inst()->FindIDByName(m_NowChatWith.c_str());
					if( m_NowRoleID != GT_INVALID)
						setNowChatWith(m_NowChatWith.c_str(),TRUE);
					else
					{
						float fSendTime = Kernel::Inst()->GetAccumTime();
						tagSendSpeak newSpeak;
						newSpeak.fSendTime = fSendTime;
						newSpeak.eChannel  = m_curSendChannel;
						newSpeak.strDestName = m_NowChatWith;
						newSpeak.strMsg.assign(_T(""));	
						newSpeak.byAutoReply = 0;

						//保存发言
						m_vecPvtSpeak.push_back(newSpeak);
					}
					m_pGUI->SetActive(m_pEB_SendMsg);
				}
			}
		}
		break;
	case EGUIE_Active:
		{
			if( pWnd==m_pRebInput && !m_pChannelSel->IsInvisible())	//当输入框获胜者得焦点，并且发言频道选择正在选择时
			{
				m_pChannelSel->SetInvisible(TRUE);
				m_pWndChannel->SetInvisible(TRUE);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pBtnHideOthers )
			{
				if( 1 == pEvent->dwParam1 )
					m_pMgr->SendEvent( &tagGameEvent( _T("Hide_Players"), this ) );
				else
					m_pMgr->SendEvent( &tagGameEvent( _T("Show_Players"), this ) );
				m_pBtnHideOthers->SetRefresh( TRUE );
			}

			if(pEvent->dwParam1 != 1)    //1 为down事件，0 为up事件
				break;
			if( pWnd == m_pPBtnWorld )
			{
				if( pEvent->dwParam1==1 )
					m_bWorldOK = TRUE;
				return TRUE;
			}
			for(int i=0; i<NUM_CHAT_CHANNEL; i++)
			{
				if( pWnd == m_pPage[i] )
				{
					m_curRecvChannel = i;
					m_pPage[m_curRecvChannel]->SetPicColor(0xFFFFFFFF);
					OnSwitchPageShow(m_curRecvChannel);		//切换显示频道的同时改变发言频道
					m_curSendChannel = TranRecv2SendChannel(m_curRecvChannel);
					SetCurChannel(m_curSendChannel);
					if(!m_pChannelSel->IsInvisible())
					{
						m_pChannelSel->SetInvisible(TRUE);
						m_pWndChannel->SetInvisible(TRUE);
					}
					
					//点击哪个频道，哪个频道不再闪烁
					if( P_VALID(m_pPage[i]) )
						m_pPage[i]->SetGlintStart( false );
				}
			}	
			
		}
		break;
	case EGUIE_Drag:
		{
			if(pEvent->dwParam2!=1)							//不是拖动窗口事件
				break;
			int i;
			bool bDrag = false;								//当前动作是否为拖动聊天窗口
			for(i=0; i<NUM_CHAT_CHANNEL; i++)
			{
				if( pWnd == m_pChatContext[i] )				//拖拽的是哪个页
				{
					bDrag = true;							//当前拖动某一个窗口
					break;
				}
			}
			//拖动聊天窗口
			if( bDrag == false )break;
			
			tagPoint ptSize = (tagPoint&)m_pChatContext[i]->GetSize();
			int nOffset = (int)pEvent->dwParam1;
			ptSize.y += nOffset;
			int curHight = 0;

			m_pMidBack->Align();
			m_pBtnHide->Align();
			m_pMutex->Align();
			m_pBtnZoomUp->Align();
			m_pBtnZoomDn->Align();
			m_pWndImName->Align();
			m_pStcCast->Align();
			m_pWndHeadPic->Align();
			

			for(int j=0; j<NUM_CHAT_CHANNEL; j++)					//使全部显示窗口大小一致，包含滚动条
			{				
				m_pChatContext[j]->Resize(ptSize);
				m_pChatContext[j]->Align();
				m_pChatContext[j]->SetScrollLast(TRUE);
		
				tagPoint pt = m_pScl[j]->GetClient();
				pt.y = -22;
				m_pScl[j]->SetMovable(TRUE);
				m_pScl[j]->Move(pt);
				m_pScl[j]->SetMovable(FALSE);
				m_pScl[j]->SetResizable(TRUE);
				tagPoint ptFclSize = (tagPoint&)m_pScl[j]->GetSize();			
				ptFclSize.y = ptSize.y-22;							//滚动条的大小要与显示框一致		
				m_pScl[j]->Resize(ptFclSize);
				if( ptFclSize.y<20 )
					m_pScl[j]->SetInvisible(TRUE);
				else 
					m_pScl[j]->SetInvisible(FALSE);
				m_pScl[j]->Align();
				m_pBtnDp[j]->Align();								//对齐向下按钮
				curHight = m_pScl[j]->GetValue();
				m_pScl[j]->SetValue(curHight);						//重新设置滚动条位置
				m_pChatContext[j]->ScrollForLastLine(TRUE);
				m_pScl[j]->SetResizable(FALSE);
			}

			m_pWnd->SetInvisible(TRUE);
			m_pWnd->SetInvisible(FALSE);
		}
		break;
	}
	return FALSE;
}
//显示窗口
void ChatFrame::ShowWnd()
{
	/*m_pMidBack->SetInvisible(FALSE);
	m_pMutex->SetInvisible(FALSE);
	for(int i=0; i<NUM_CHAT_CHANNEL-1; i++)
	{
		m_pPage[i]->SetInvisible(FALSE);
	}
	tagPoint ptSize = m_pScl[m_curRecvChannel]->GetSize();
	if(ptSize.y > 20)
		m_pScl[m_curRecvChannel]->SetInvisible(FALSE);
	for (int i=0; i<NUM_CHAT_CHANNEL-1; i++)
	{
		m_pChatContext[i]->SetInvisible(FALSE);
	}
	m_pBtnZoomUp->SetInvisible(FALSE);
	m_pBtnZoomDn->SetInvisible(FALSE);
	m_pWnd->SetRefresh(FALSE);
	m_pEmotionFrame->SetInVisibleFrame(FALSE);*/
	m_pWnd->SetInvisible(FALSE);
	m_pWnd->SetRefresh(FALSE);
	m_pWndHide->SetInvisible(TRUE);
	if (P_VALID(m_pBn_Hide))
	{
		if (m_pBn_Hide->IsGlintNow())
		{
			m_pBn_Hide->EndGlint();
		}
	}
}
//隐藏窗口
void ChatFrame::HideWnd()
{
	/*m_pMidBack->SetInvisible(!m_pMidBack->IsInvisible());
	m_pMutex->SetInvisible(!m_pMutex->IsInvisible());
	for(int i=0; i<NUM_CHAT_CHANNEL-1; i++)
	{
		m_pPage[i]->SetInvisible(!m_pPage[i]->IsInvisible());
	}
	tagPoint ptSize = m_pScl[m_curRecvChannel]->GetSize();
	if(ptSize.y > 20)
		m_pScl[m_curRecvChannel]->SetInvisible(!m_bChatContext);
	
	m_pBtnZoomUp->SetInvisible(!m_pBtnZoomUp->IsInvisible());
	m_pBtnZoomDn->SetInvisible(!m_pBtnZoomDn->IsInvisible());
	m_bChatContext = !m_bChatContext;
	m_pWnd->SetRefresh(TRUE);
	m_pEmotionFrame->SetInVisibleFrame(true);*/
	m_pWnd->SetInvisible(TRUE);
	m_pWnd->SetRefresh(TRUE);
	m_pWndHide->SetInvisible(FALSE);
	m_pEmotionFrame->SetInVisibleFrame(TRUE);
}

//!设置当前发言频道按钮显示
VOID ChatFrame::SetCurChannel(ESendChatChannel eChannel)
{
	switch(eChannel)
	{
	//case ESCC_CastTalk:
	//	m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_CastTalk")]);
	//	break;
	case ESCC_World:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_World")]);
		break;
	case ESCC_Pvt:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Pvt")]);
		break;
	case ESCC_Guild:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Guild")]);
		break;
	case ESCC_Troop:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Troop")]);
		break;
	case ESCC_Current:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Current")]);
		break;
	//case ESCC_Affiche:
	//	m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Affiche")]);
	//	break;
	case ESCC_Scene:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Scene")]);
		break;
	case ESCC_Family:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Family")]);
		break;
	default:
		m_pBtnCurChannel->SetText(g_StrTable[_T("Chat_Current")]);
	}
}

//切换聊天显示频道，玩家通过发言频道listbox选择
VOID ChatFrame::OnSwitchPageShow(int index)
{
	for(int i=0; i<NUM_CHAT_CHANNEL; i++)
	{
		if( i == index )
			m_pChatContext[i]->SetInvisible(FALSE);
		else
			m_pChatContext[i]->SetInvisible(TRUE);
	}
}

//!取得输入框中输入的的文字,在发送聊天内容时使用
VOID ChatFrame::GetInputText(tstring& str)
{
	if(m_pRebInput==NULL)return;
	int nLen=0;
	const TCHAR* szInput=m_pRebInput->GetText(nLen);
	if(nLen<=0 || szInput==NULL || !GT_VALID((INT)szInput) )
	{
		return;
	}
	str=szInput;
}

//!点击显示频道时，自动切换发言频道的转换函数
ESendChatChannel ChatFrame::TranRecv2SendChannel(int nRecvChannel)
{
	ESendChatChannel eRes = ESCC_NULL;
	switch(nRecvChannel)
	{
	case 0:		eRes = ESCC_Current;			break;			//点击全部按钮页时，发言频道为普通
	case 1:		eRes = ESCC_Troop;				break;			//点击队伍按钮页时，发言频道为队伍
	case 2:		eRes = ESCC_Guild;				break;			//点击帮派按钮页时，发言频道为帮派
	case 3:		eRes = ESCC_Pvt;					break;			//点击私聊按钮页时，发言频道为私聊
	case 4:		eRes = ESCC_World;				break;			//点击世界按钮页时，发言频道为世界
	case 5:		eRes = ESCC_Scene;				break;			//点击场景按钮页时，发言频道为场景
	case 6:     eRes = ESCC_Combat;			break;			//点击战斗按钮页时，发言频道为战斗
	case 7:		eRes = ESCC_Family;				break;			//点击家族按钮页时，发言频道为家族

	default:	eRes = m_curSendChannel;		break;			//点击公告按钮页时，发言频道为不变换
	}
	return eRes;
}

//输入框输入内容发生变化，自动切换到相应发言频道
VOID ChatFrame::OnInputContextChange()
{
	//处理命令行切换聊天频道
	int nLen = 0;
	const TCHAR* szInput = m_pRebInput->GetText(nLen);
	if( nLen!=3 || szInput==NULL || !GT_VALID(szInput) )
		return;
	const TCHAR* szCmd[9]=
	{	_T("/gg"), _T("/sj"), _T("/sl"), _T("/bp"), _T("/dw"), _T("/dq"), _T("/gm"), _T("/cj"), _T("/jz")	};   
	for(int i=0; i<8; i++)
	{
		if( _tcsnccmp(szCmd[i], szInput, 3)==0 )
		{
			m_curSendChannel = TranCMD2SendChannel(i);
			SetCurChannel(m_curSendChannel);
			m_pRebInput->ClsText();
		}
	}
}

//处理命令行切换聊天发言频道
ESendChatChannel ChatFrame::TranCMD2SendChannel(int nChannel)
{
	ESendChatChannel eRes = ESCC_NULL;
	switch(nChannel)
	{
	case 0:		eRes = ESCC_Affiche;			break;
	case 1:		eRes = ESCC_World;				break;
	case 2:		eRes = ESCC_Pvt;				break;
	case 3:		eRes = ESCC_Guild;				break;
	case 4:		eRes = ESCC_Troop;				break;
	case 5:		eRes = ESCC_Current;			break;
	case 6:		eRes = ESCC_GM;					break;
	case 7:		eRes = ESCC_Scene;				break;
	case 8:		eRes = ESCC_Family;				break;
	default:	eRes = ESCC_Current;			break;
	}
	return eRes;
}

//将发言频道转换为客户端显示频道
DWORD ChatFrame::TranIntEChat2EClient(ESendChatChannel eChannel)
{
	WORD dwRes = 0;
	switch(eChannel)
	{
	case ESCC_Scene:			dwRes = ERCC_Scene;		break;
	case ESCC_Family:			dwRes = ERCC_Family;	break;
	case ESCC_World:			dwRes = ERCC_World;		break;
	case ESCC_Pvt:				dwRes = ERCC_Pvt;		break;
	case ESCC_Guild:			dwRes = ERCC_Guild;		break;
	case ESCC_Troop:			dwRes = ERCC_Troop;		break;
	case ESCC_Current:			dwRes = ERCC_Current;	break;
	case ESCC_Leave:			dwRes = ERCC_Pvt;		break;
	case ESCC_Combat:			dwRes = ERCC_Combat;	break;
	case ESCC_Affiche:			dwRes = ERCC_Affiche;	break;
	case ESCC_GM:				dwRes = ERCC_GM;		break;
	case ESCC_System:			dwRes = ERCC_System;	break;
	default:					dwRes = ERCC_Current;	break;
	}
	return dwRes;
}

//初始化时设置聊天显示频道
VOID ChatFrame::ResetRecvChannel()
{
	TCHAR szBuff[X_LONG_NAME] = {0};
	_sntprintf( szBuff, X_LONG_NAME, CHANNEL_SET_FILE, m_strLocalName.c_str() );
	TObjRef<VarContainer> pSet = CreateObj("ChannelSet", "VarContainer");

	std::list<tstring> listName;
	if( pSet->Load(NULL, szBuff, "name", &listName) )
	{
		std::list<tstring>::iterator it;
		INT i=0;
		for(it=listName.begin();it!=listName.end();it++)
		{
			if( i < NUM_CHAT_CHANNEL)
			{
				m_dwRecvChalShow[i] = pSet->GetDword((*it).c_str());
				++i;
			}
		}
	}
	else
	{
		for(int i=0; i<NUM_CHAT_CHANNEL; i++)
		{
			m_dwRecvChalShow[i] = g_dwDefaultChannel[i];
		}
	}

	KillObj("ChannelSet");
}

//过滤格式自定义格式字符串
VOID ChatFrame::FilterMarkerString(tstring&  strText)
{
	if( P_VALID(m_pRebInput) )
		m_pRebInput->FilterString(strText);
}	


//删除richEditBox为了进行排版在文字（strText）里自动加上的回车键
VOID ChatFrame::DeleteRETURNS(tstring& strText)
{
	tstring::iterator new_end;
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0d);
	strText.erase(new_end, strText.end());
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0a);
	strText.erase(new_end, strText.end());
}

//缓存跑马灯消息
VOID ChatFrame::PushScrollText(LPCTSTR szText, float fTime)
{
	tagScroText tagSText;
	//tagSText.szInfo.assign(_T("<=(0, 251, 255)>"));
	tagSText.szInfo = szText;
	tagSText.fTime = fTime;
	m_vecSclText.push_back(tagSText);
}

VOID ChatFrame::PushBigScrollText(LPCTSTR szText, DWORD dwPriority, float fTime,DWORD colour)
{
	tagScroText tagSText;
	//tagSText.szInfo.assign(_T("<=(0, 251, 255)>"));
	tagSText.szInfo = szText;
	tagSText.fTime = fTime;
	tagSText.dwRoleID = dwPriority;
	tagSText.dwColour = colour;
	m_vecBigSclText.push_back(tagSText);
}

VOID ChatFrame::PushBigScrollTextImmediately(LPCTSTR szText, DWORD dwPriority ,float fTime,DWORD colour)
{
	m_pRunHorseScrollText->PushScrollText(szText,40.0,colour);//这里后面的默认时间是40秒
	m_dwCurrentRunHorsePriority = dwPriority;
}

VOID ChatFrame::PushCastText(LPCTSTR szText, DWORD dwRoleID, float fTime)
{
	tstring strName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
	tstring strUrl = PlayerNameTab::Inst()->FindHeadPicURLbyID(dwRoleID);
	if(!strName.empty() && !strUrl.empty())
	{
		tagScroText text;
		text.szInfo = szText ;
		text.szName = strName;
		text.fTime = fTime;
		text.url = strUrl;
		m_vecCast.push_back(text);
	}
	else
	{
		tagScroText text;
		text.szInfo = szText;
		text.fTime = fTime;
		text.dwRoleID = dwRoleID;
		text.szName = strName;
		text.url = strUrl;
		m_vecCastNoName.push_back(text);
	}
}

//添加从服务器接受的聊天数据到相应的显示频道
VOID ChatFrame::PushInfo(const TCHAR* strText, ESendChatChannel escc)
{
	if(strText==NULL || strText[0]==_T('\0'))
		return;

	tstring strTemp = GetChatChannelColor(escc);						//前面加上频道名称提示
	DWORD dwRecvChannel = TranIntEChat2EClient(escc);					//将发言频道转换成显示频道

	switch(escc)
	{
	//case ESCC_CastTalk:
	//	strTemp.append(g_StrTable[_T("ChatShow_CastTalk")]);
	//	break;
	case ESCC_World:
		strTemp.append(g_StrTable[_T("ChatShow_World")]);
		break;
	case ESCC_Pvt:
		strTemp.append(g_StrTable[_T("ChatShow_Pvt")]);
		break;
	case ESCC_Guild:
		strTemp.append(g_StrTable[_T("ChatShow_Guild")]);
		break;
	case ESCC_Troop:
		strTemp.append(g_StrTable[_T("ChatShow_Troop")]);
		break;
	case ESCC_Current:
		strTemp.append(g_StrTable[_T("ChatShow_Current")]);
		break;
	case ESCC_Combat:
		strTemp.append(g_StrTable[_T("ChatShow_Combat")]);
		break;
	case ESCC_Affiche:
		strTemp.append(g_StrTable[_T("ChatShow_Affiche")]);
		break;
	case ESCC_Scene:
		strTemp.append(g_StrTable[_T("ChatShow_Scene")]);
		break;
	case ESCC_Family:
		strTemp.append(g_StrTable[_T("ChatShow_Family")]);
		break;
	case ESCC_GM:
		strTemp.append(g_StrTable[_T("ChatShow_GM")]);
		break;
	case ESCC_System:
		strTemp.append(g_StrTable[_T("ChatShow_System")]);
		break;

	}

	strTemp.append(strText);

	if ( dwRecvChannel == ERCC_Affiche )
	{
		AddStringToAffiche(strTemp, escc);
		return;
	}
	else if ( ESCC_GM == escc || ESCC_System == escc ) //系统消息需放在 当前 和 场景
	{
		AddStringToWnd(strTemp, 0, escc);	//ESCC_Current;
		AddStringToWnd(strTemp, 5, escc);	//ESCC_Scene;
	}
	else 
	{
		for(int i=0; i<NUM_CHAT_CHANNEL; i++)
		{
			if( (dwRecvChannel&m_dwRecvChalShow[i]) != ERCC_NULL )
				AddStringToWnd(strTemp, i, escc);
		}
	}

}

//将聊天信息添加到公告显示频道
VOID ChatFrame::AddStringToAffiche(const tstring& strText,ESendChatChannel escc)
{
	//是否到达做大的长度，是则删除最前面的10行
	int nLen = 0;
	m_pShowAffiche->GetText(nLen);
	if(nLen >= RICHEDITBOX_MAX_TEXT-MAX_ROW-1)
		m_pShowAffiche->DelFrontText(10);

	//是否显示到最后一行
	if(m_pShowAffiche->IsShowLastLine())
		m_pShowAffiche->SetScrollLast(TRUE);
	else
		m_pShowAffiche->SetScrollLast(FALSE);

	// <!F计划公告单独显示
	BOOL bRes = TRUE;
	if( nLen <= 0 )
		bRes = m_pShowAffiche->AddText(strText.c_str(),FALSE,GT_INVALID,GT_INVALID,TRUE,FALSE);
	else
		bRes = m_pShowAffiche->AddText(strText.c_str(),TRUE,GT_INVALID,GT_INVALID,TRUE,FALSE);
	//加入文字失败则重新添加
	while( bRes == FALSE )
	{
		m_pShowAffiche->DelFrontText(10);
		if( nLen <= 0 )
			bRes = m_pShowAffiche->AddText(strText.c_str(),FALSE,GT_INVALID,GT_INVALID,TRUE,FALSE);
		else
			bRes = m_pShowAffiche->AddText(strText.c_str(),TRUE,GT_INVALID,GT_INVALID,TRUE,FALSE);
	}
	m_pShowAffiche->SetRefresh();
}

//将聊天信息添加到显示频道
VOID ChatFrame::AddStringToWnd(const tstring& strText, int nPate, ESendChatChannel escc)
{
	ASSERT(nPate>=0&&nPate<NUM_CHAT_CHANNEL);

	//是否到达做大的长度，是则删除最前面的10行
	int nLen = 0;
	//int nVisiableNum = 0;
	//nLen = m_pChatContext[nPate]->GetRowNum(nVisiableNum);
	m_pChatContext[nPate]->GetText(nLen);
	if(nLen >= RICHEDITBOX_MAX_TEXT-MAX_ROW-1)
		m_pChatContext[nPate]->DelFrontText(10);

	//是否显示到最后一行
	if(m_pChatContext[nPate]->IsShowLastLine())
		m_pChatContext[nPate]->SetScrollLast(TRUE);
	else
		m_pChatContext[nPate]->SetScrollLast(FALSE);

	BOOL bMatchPic = escc==ESCC_NULL
		||escc==ESCC_End||escc==ESCC_Combat||escc==ESCC_Affiche;
	// 添加信息到窗口 
	BOOL bRes=TRUE;
	if(nLen<=0)
		bRes=m_pChatContext[nPate]->AddText(strText.c_str(),FALSE,GT_INVALID,GT_INVALID,!bMatchPic,FALSE);
	else
		bRes=m_pChatContext[nPate]->AddText(strText.c_str(),TRUE,GT_INVALID,GT_INVALID,!bMatchPic,FALSE);

	//加入文字失败则重新添加
	while(bRes==FALSE)
	{
		m_pChatContext[nPate]->DelFrontText(10);
		if(nLen<=0)
			bRes=m_pChatContext[nPate]->AddText(strText.c_str(),FALSE,GT_INVALID,GT_INVALID,!bMatchPic,FALSE);
		else
			bRes=m_pChatContext[nPate]->AddText(strText.c_str(),TRUE,GT_INVALID,GT_INVALID,!bMatchPic,FALSE);
	}
	m_pChatContext[nPate]->SetRefresh();
	
}

//!为RichEditBox注册图片（转义字符）
//!这里由于使用的是固定路径，而且按序号排好图片所以只需要index
void ChatFrame::RegisterPicForRichEdit(const TCHAR *szName,int index)
{
	if(index<0 || index>=MAX_EMOTION)return;

	TCHAR szPicPath[_MAX_PATH] = {0};
	_stprintf(szPicPath, _T("data\\ui\\chat\\emotion\\emo%02d.bmp"), index+1);

	TCHAR szPicPath1[_MAX_PATH] = {0};
	_stprintf(szPicPath1, _T("data\\ui\\chat\\emotion\\semo%02d.bmp"), index+1);

	//开始注册转义字符
	TCHAR szTranName[32] = {0};
	_tcscpy(szTranName, szName);									//注RegisterPic函数不能使用const char[]传递

	m_pRebInput->RegisterPic(szTranName,szPicPath1);
	m_pScrollRichText->RegisterPic(szTranName,szPicPath);
	m_pRunHorseScrollText->RegisterPic(szTranName,szPicPath);		//跑马灯2号的表情图片注册
	m_pWndText->RegisterPic(szTranName,szPicPath);					//注册密语显示框表情

	m_pShowAffiche->RegisterPic(szTranName,szPicPath);
	
	for (int i=0;i<NUM_CHAT_CHANNEL;i++)
	{
		m_pChatContext[i]->RegisterPic(szTranName,szPicPath);
	}
	/*for(int i=0; i<NUM_CHAT_CAST_WND; i++)
	{
		m_pCastRichText[i]->RegisterPic(szTranName, szPicPath);
	}*/ //关闭传音相关功能
}

//获得输入框当前输入文本的大小
int ChatFrame::GetInputValidTextSize()
{
	if( m_pRebInput == NULL )
		return -1;
	tstring str;
	int InputLen = m_pRebInput->GetText(str);
	return InputLen;
}

//向输入框添加表情
void ChatFrame::AddInputEmotion(const tstring &str)
{
	if( !P_VALID(m_pRebInput) )
		return;
	m_pRebInput->AddText(str.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);	
	m_pGUI->SetActive(m_pRebInput);	
}

//处理私聊
tstring ChatFrame::HandleStringForPvt(tstring& strText)
{
	TCHAR seps[] = _T(" ");
	TCHAR *token = NULL;
	
	token = _tcstok((TCHAR *)strText.c_str(), seps);
	tstring strName(token);
	if( strName.empty() )
		return _T("");

	strText = strText.erase(0, _tcslen(strName.c_str())+1);
	
	return strName;
}

const TCHAR* ChatFrame::GetChatChannelColor(ESendChatChannel escc)
{
	switch(escc)
	{
	case ESCC_Scene:
		return _T("<color=(119,224,17)>");
		break;
	case ESCC_Family:
		return _T("<color=(254,165,160)>");
		break;
	case ESCC_World:
		return _T("<color=(81,237,255)>");
		break;
	case ESCC_Pvt:
		return _T("<color=(232,124,255)>");
		break;
	case ESCC_Guild:
		return _T("<color=(254,165,160)>");
		break;
	case ESCC_Troop:
		return _T("<color=(255,187,71)>");
		break;
	case ESCC_Current:
		return _T("<color=(224,247,255)>");
		break;
	case ESCC_Combat:
		return _T("<color=(83,144,213)>");
		break;
	case ESCC_Affiche:
	case ESCC_System:
		return _T("<color=(30,136,255)>");
		break;
	case ESCC_GM:
		return _T("<color=(45,64,255)>");
		break;
	}
	return _T("<color=(224,247,255)>");
}

//判断是否重复发言
bool ChatFrame::IsViladeSpeak(const tstring& str, ESendChatChannel escc)
{
	bool bRes=true;
	for (vector<tagSendSpeak>::iterator it=m_vecSpeak.begin();
		it !=m_vecSpeak.end(); it++)
	{
		if((*it).strMsg==str && escc==(*it).eChannel )	//发言的内容相同并且频道也相同
		{
			bRes=false;
			break;
		}
	}
	return bRes;
}

bool ChatFrame::IsIntervalSpeak(ESendChatChannel escc)
{
	bool bRes=true;
	for (vector<tagSendSpeak>::iterator it=m_vecSpeak.begin();
		it !=m_vecSpeak.end(); it++)
	{
		if( escc==(*it).eChannel )	//世界频道发言必须小于10秒
		{
			bRes=false;
			break;
		}
	}
	return bRes;
}

//接收服务器发送的聊天信息
DWORD ChatFrame::NetRecvMsgNS_RoleChat( tagNS_RoleChat* pMsg, DWORD dwParam )
{
	if (P_VALID(m_pBn_Hide) && P_VALID(m_pWndHide))
	{
		if (!m_pBn_Hide->IsGlintNow()&&!m_pWndHide->IsInvisible()) //m_pWndHide可见等价与pWnd不可见
		{
			m_pBn_Hide->BeginGlint();
		}
	}
	if ( ESCC_Group == (ESendChatChannel)pMsg->byChannel )
	{
		if (P_VALID(m_pMultChatFrame) && P_VALID(m_pBtnGroupChat))
		{
			if (m_pMultChatFrame->FrameIsInvisible())
			{
				m_pBtnGroupChat->BeginGlint();
				m_pBtnGroupChat->SetGlintTextColor(0xff00ffff); 
			}
			else
			{
				m_pBtnGroupChat->EndGlint();
			}
		}
		return 0;
	}
	switch(pMsg->dwErrorCode)
	{
	case E_RoleChat_ForbidChat:					//禁止发言
		PushInfo(g_StrTable[_T("RoleChat_ForbidChat")], ESCC_Current);
		return 0;
		break;
	case E_RoleChat_ServerNotReady:				//服务器没有准备好
		PushInfo(g_StrTable[_T("RoleChat_ServerNotReady")], ESCC_System);
		return 0;
		break;
	case E_RoleChat_Pvt_RemoteRoleLeave:		//私聊,对方不在线，同时给密语聊天框内添加不在线提示
		{
			PushInfo(g_StrTable[_T("Pvt_RemoteRoleLeave")], ESCC_Pvt);
			tstring temp;
			temp += g_StrTable[_T("Pvt_RemoteRoleLeave")];
			temp += _T("\n");
			m_bisfromChat = FALSE;
			//m_pWndText->AddText(temp.c_str());  //在聊天框中设置信息提示对方不在线
		}
		return 0;
		break;
	case E_RoleChat_TooLength:					//消息过长,发言失败
		PushInfo(g_StrTable[_T("RoleChat_TooLength")], (ESendChatChannel)pMsg->byChannel);
		return 0;
		break;
	case E_RoleChat_Quest_Bag_Full:				//任务物品背包已满
		PushInfo(g_StrTable[_T("RoleChat_Quest_Bag_Full")], ESCC_System);
		return 0;
		break;
	//case E_RoleChat_World_NoMoney:				//世界发言,钱不够	
	//	PushInfo(g_StrTable[_T("RoleChat_World_NoMoney")], ESCC_World);
	//	return 0;
	//	break;
	//case E_RoleChat_World_RoleNo10:
	//	PushInfo(g_StrTable[_T("RoleChat_World_RoleNo10")], ESCC_World);
	//	return 0;
	//	break;
	case E_RoleChat_Guild_NoJoin:
		PushInfo(g_StrTable[_T("RoleChat_Guild_NoJoin")], ESCC_Guild);
		return 0;
		break;
	case E_RoleChat_NoFamily:
		PushInfo(g_StrTable[_T("RoleChat_NoFamily")], ESCC_Family);
		return 0;
		break;
	case E_RoleChat_Troop_NoJoin:
		PushInfo(g_StrTable[_T("RoleChat_Troop_NoJoin")], ESCC_Troop);
		return 0;
		break;
	case E_RoleChat_CastTalk_NoIMItem:
		{
			const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pMsg->dwSrcRoleID);
			if( P_VALID(pInfo) )
			{
				TCHAR szBuff[X_LONG_NAME] = {0};
				_sntprintf( szBuff, X_LONG_NAME, g_StrTable[_T("RoleChat_CastTalk_NoIMItem0")], pInfo->szName );
				PushInfo(szBuff, ESCC_System);
			}
			else
			{
				PushInfo(g_StrTable[_T("RoleChat_CastTalk_NoIMItem1")], ESCC_System);
			}
		}
		break;
	case E_RoleChat_Scene_Frequently:
		{
			PushInfo(g_StrTable[_T("E_RoleChat_Scene_Frequently")], ESCC_Scene);
			return 0;
		}
	case E_RoleChat_NotOpened:
		{
			if( P_VALID(GetObj("E_RoleChat_NotOpened")) )
				KillObj("E_RoleChat_NotOpened");
			CreateObj( "E_RoleChat_NotOpened", "MsgBoxEx" );
			TObjRef<MsgBoxEx>("E_RoleChat_NotOpened")->Init( _T(""), g_StrTable[_T("E_RoleChat_NotOpened")], _T("E_RoleChat_NotOpened") );
			return 0;
		}
	case E_RoleChat_World_NoIMItem:
		{
			PushInfo(g_StrTable[_T("E_RoleChat_World_NoIMItem")], ESCC_World);
			return 0;
		}
	case E_RoleChat_World_Frequently:
		{
			PushInfo(g_StrTable[_T("World_Chat_frequency")], ESCC_World);
			return 0;
		}
	}

	//黑名单判断
	SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
	if( P_VALID(pSocialMgr) )
	{
		SocialData* pData = pSocialMgr->GetData();
		if( P_VALID(pData) && pData->HasBlack( pMsg->dwSrcRoleID ) )
		{
			return 0;
		}

	}

	tstring strTemp;
	tstring strMsg = pMsg->szMsg;
	//过滤
	FilterMarkerString(strMsg);	
	if((ESendChatChannel)pMsg->byChannel != ESCC_Affiche && (ESendChatChannel)pMsg->byChannel != ESCC_System)
		FilterData::Inst()->WordsFilteror(strMsg);
	ESendChatChannel eChannel = (ESendChatChannel)pMsg->byChannel;

	if(m_dwLocalID==GT_INVALID || m_strLocalName.empty())
	{
		m_dwLocalID		= RoleMgr::Inst()->GetLocalPlayer()->GetID();
		m_strLocalName	= PlayerNameTab::Inst()->FindNameByID(m_dwLocalID);
	}

	if((ESendChatChannel)pMsg->byChannel != ESCC_Affiche && (ESendChatChannel)pMsg->byChannel != ESCC_System)
		FilterData::Inst()->SysPicFilteror(strMsg);
	tagRecvSpeak n;
	n.dwSendRoleID = pMsg->dwSrcRoleID;					//谁说
	n.dwRecvRoleID = pMsg->dwDestRoleID;				//对谁说的
	n.strText.assign(strMsg);							//说什么
	n.eChannel = (ESendChatChannel)pMsg->byChannel;		//哪个频道
	n.byAutoReply = pMsg->byAutoReply;


	switch( eChannel )
	{
	case ESCC_Pvt:
		{
			TCHAR szTemp[128] = {0};
 			if(pMsg->dwSrcRoleID == m_dwLocalID)
			{
				//我对别人说的
				const TCHAR* name = PlayerNameTab::Inst()->FindNameByID(pMsg->dwDestRoleID);
				if(name==_T(""))
				{
					m_vecRecv.push_back(n);
					return 0;
				}

				n.strSendRoleName.assign(name);
				_stprintf(szTemp, g_StrTable[_T("Chat_ToSpeek")], KeyStart, name, KeyEnd);
				strTemp.append(szTemp);
				strTemp.append(n.strText);
				DeleteRETURNS(strTemp);
				PushInfo(strTemp.c_str(), n.eChannel);
				//-------------------------向密语框中添加发送的信息---------------------------
				m_NowRoleID = pMsg->dwDestRoleID;
				m_NowChatWith = name;
				if(m_NowRoleID != GT_INVALID && m_bisfromChat)  //如果是从聊天框传进来的
				{
					tstring strtemp;   //用于清理自动添加的回车符用的
					m_pEB_SendMsg->ClsText();
					TCHAR msg[150] = {0};
					tstring tempcolor;
					tempcolor.append(_T("<color=(0,255,0)>"));
					tempcolor.append(name);
					tempcolor.append(_T("<color=(224,247,255)>"));
					_stprintf( msg, g_StrTable[_T("Chat_Youto")],tempcolor.c_str(),n.strText.c_str() );
					strtemp += msg;
					DeleteRETURNS(strtemp);
					strtemp += _T("\n");
					//--------添加数据应该先判断一下list中是否有此人信息------------
					BOOL isfind = FALSE;
					for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
					{
						if(m_NowRoleID == (*MiniChatMsgit).RoleID)
						{
							isfind = TRUE;
						}
						else 
						{
							MiniChatMsgit++;
						}
					}
					if(!isfind)
					{
						tagMiniMsg temp;
						temp.RoleID  = m_NowRoleID;
						temp.Minimsg += strtemp;
						temp.isRead = true;
						MiniChatMsg.push_front(temp);
						m_pWndText->AddText(strtemp.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);                                 //如果没找到就把当前信息输出显示并存入list
					}
					else 
					{
						m_pWndText->ClsText();
						(*MiniChatMsgit).Minimsg += strtemp;
						(*MiniChatMsgit).isRead = true;
						m_pWndText->AddText((*MiniChatMsgit).Minimsg.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);        //如果找到就把信息加上当前输入的信息然后显示出来
					}
					m_pBtn_Send->SetEnable(TRUE);
 					m_pWndText->SetRefresh(TRUE); 
// 					if(m_pMiniChatWnd->IsInvisible())
// 						m_pMiniChatWnd->SetInvisible(FALSE);
// 					m_pGUI->SetActive(m_pEB_SendMsg);
 					UpdataMiniChat(m_NowRoleID);
					m_bisfromChat = FALSE;
				}
				//----------------------------------------------------------------------------------------------
			}
			else if(pMsg->dwDestRoleID == m_dwLocalID)
			{
				//别人对我说的
				const TCHAR* name = PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID);
				tstring temp = name;
				if(temp==_T(""))
				{
					m_vecRecv.push_back(n);
					return 0;
				}

				n.strSendRoleName.assign(name);
				strTemp.append(KeyStart);
				strTemp.append(n.strSendRoleName);
				strTemp.append(KeyEnd);
				strTemp.append(g_StrTable[_T("Chat_FromSpeek")]);
				strTemp.append(n.strText);
				DeleteRETURNS(strTemp);
				PushInfo(strTemp.c_str(),n.eChannel);
				SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
				if( P_VALID(pSocialMgr) && pSocialMgr->IsAutoReply() && n.byAutoReply==0 )
				{
					AutoReplyPvt(pMsg->dwSrcRoleID, pSocialMgr->GetReply());
				}

				//------------------------------------------------------------
				tstring tstname;
				tstname += name;
				m_destID = pMsg->dwSrcRoleID;
				if(m_destID == GT_INVALID)
				{
					float fSendTime = Kernel::Inst()->GetAccumTime();
					tagSendSpeak newSpeak;
					newSpeak.fSendTime = fSendTime;
					newSpeak.eChannel  = m_curSendChannel;
					newSpeak.strDestName = m_NowChatWith;
					newSpeak.strMsg.assign(_T(""));	
					newSpeak.byAutoReply = 0;

					//保存发言
					m_vecPvtSpeak.push_back(newSpeak);
				}
				OnReceiveMsg(tstname,n.strText);
				UpdataMiniChat(m_destID);
				if( m_pMiniChatWnd->IsInvisible())
					setNowChatWith(tstname,TRUE);
				m_destID = GT_INVALID;
				//------------------------------------------------------------
				//聊天界面中 私聊按钮的索引是 3
				//如果当前正在显示的不是私聊频道，私聊频道有消息到来，闪烁
				/*if( m_curRecvChannel != 3 )
				{
					if( P_VALID(m_pPage[3]) )
						m_pPage[3]->SetGlintStart( true );

				}*/
				if( P_VALID(m_pBtnMiniChat)&&m_pMiniChatWnd->IsInvisible())                     //窗口未打开时且收到消息时密语按钮闪烁
				{
					m_pBtnMiniChat->BeginGlint();
					m_pBtnMiniChat->SetGlintTextColor(0xff00ffff);                              //设置闪烁时文字的颜色
				}
				//收到私聊信息，播放音效
				TObjRef<AudioSys>()->Play2DSound( _T("pvt_chat") );
			}
			return 0;
		}
		break;
	//case ESCC_CastTalk:
	//	{
	//		MilleSoundFrame* milleSoundFrame = (MilleSoundFrame*)(m_pFrameMgr->GetFrame( _T("MilleSound") ) );
	//		if( P_VALID(milleSoundFrame) )
	//		{
	//			milleSoundFrame->PushCastText( strMsg.c_str(), pMsg->dwSrcRoleID,(ESex)(pMsg->n16Sex) );
	//		}

	//		//PushCastText(strMsg.c_str(), pMsg->dwSrcRoleID);
	//		return 0;
	//	}
	//	break;
	case ESCC_Scene:
	case ESCC_Family:
	case ESCC_Guild:
	case ESCC_Troop:
	case ESCC_World:
	case ESCC_Current:
		{
			//对应频道索引
			DWORD dwRecvChannel = TranIntEChat2EClient(eChannel);		
			if( eChannel == ESCC_Current )
			{
				if( GameSet::Inst()->Get(EGS_ShowChatPop) )
					ChatShowPop::Inst()->NetRecvRoleChat(pMsg);
			}
			//队伍频道有消息闪烁
			else if( eChannel == ESCC_Troop && pMsg->dwSrcRoleID != m_dwLocalID )
			{
				//当前频道中未设置接受当前消息
				if( (dwRecvChannel&m_dwRecvChalShow[m_curRecvChannel]) == ERCC_NULL )
				{
					//当前显示的不是队伍频道
					if( m_curRecvChannel != 1 && P_VALID(m_pPage[1]) )
						m_pPage[1]->SetGlintStart( true );
				}
			}
			//帮派频道有消息闪烁
			else if( eChannel == ESCC_Guild && pMsg->dwSrcRoleID != m_dwLocalID )
			{
				//当前频道中未设置接受当前消息
				if( (dwRecvChannel&m_dwRecvChalShow[m_curRecvChannel]) == ERCC_NULL )
				{
					//当前显示的频道不是帮派
					if( m_curRecvChannel != 2 && P_VALID(m_pPage[2]) )
						m_pPage[2]->SetGlintStart( true );
				}
			}
			//场景频道有消息闪烁
			else if( eChannel == ESCC_Scene && pMsg->dwSrcRoleID != m_dwLocalID )
			{
				//当前频道中未设置接受当前消息
				if( (dwRecvChannel&m_dwRecvChalShow[m_curRecvChannel]) == ERCC_NULL )
				{
					//当前显示的频道不是场景
					if( m_curRecvChannel != 5 && P_VALID(m_pPage[5]) )
						m_pPage[5]->SetGlintStart( true );
				}
			}
			//家族频道有消息闪烁
			else if( eChannel == ESCC_Family && pMsg->dwSrcRoleID != m_dwLocalID )
			{
				//当前频道中未设置接受当前消息
				if( (dwRecvChannel&m_dwRecvChalShow[m_curRecvChannel]) == ERCC_NULL )
				{
					//当前显示的频道不是家族
					if( m_curRecvChannel != 7 && P_VALID(m_pPage[7]) )
						m_pPage[7]->SetGlintStart( true );
				}
			}

			if(pMsg->dwSrcRoleID==m_dwLocalID && !m_strLocalName.empty())
			{
				n.strSendRoleName.assign(m_strLocalName);
				strTemp.append(KeyStart);
				strTemp.append(n.strSendRoleName);
				strTemp.append(KeyEnd);
				strTemp.append(_T("："));
				strTemp.append(n.strText);
				DeleteRETURNS(strTemp);
				PushInfo(strTemp.c_str(), n.eChannel);

				//世界频道，VIP剩余次数提示
				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				if (eChannel == ESCC_World && P_VALID(plp) && plp->GetAttribute(ERA_VipLevel)>0 && pMsg->nLeftTimes != 0)
				{
					TCHAR szText[X_LONG_NAME];
					_stprintf(szText,g_StrTable[_T("VIP_brocastTimesLeft")],pMsg->nLeftTimes);
					tagPushSpecInfoEvent event(_T("tagPushSpecInfoEvent"),NULL);
					event.strDesc = szText;
					event.eChannelType = ESCC_World;
					TObjRef<GameFrameMgr>()->SendEvent(&event);
				}
				return 0;
			}
			if( IS_CREATURE(pMsg->dwSrcRoleID) )
			{
				const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt( pMsg->dwSrcRoleID );
				if( !P_VALID(pProto) )
					return 0;
				n.strSendRoleName.assign(pProto->szName);
				strTemp.append(n.strSendRoleName);
				strTemp.append(_T("："));
				strTemp.append(n.strText);
				DeleteRETURNS(strTemp);
				PushInfo(strTemp.c_str(), n.eChannel);
				return 0;
			}
		}
		break;
	case ESCC_System:
		PushScrollText(strMsg.c_str());
	case ESCC_Combat:	
		{
			strTemp.append(strMsg);
			DeleteRETURNS(strTemp);
			PushInfo(strTemp.c_str(), (ESendChatChannel)pMsg->byChannel);
			return 0;
		}
		break;
	case ESCC_GM:
	case ESCC_Affiche:  //F计划传音区域用来显示公告
		{
			/*MilleSoundFrame* milleSoundFrame = (MilleSoundFrame*)(m_pFrameMgr->GetFrame( _T("MilleSound") ) );
			if( P_VALID(milleSoundFrame) )
			{
				milleSoundFrame->PushCastText( strMsg.c_str(), GT_INVALID ,ES_Null );
			}*/
			strTemp.append(strMsg);
			DeleteRETURNS(strTemp);
			PushInfo(strTemp.c_str(), (ESendChatChannel)pMsg->byChannel);
			return 0;
		}
		break;
	}

	//处理当前、队伍、门派、世界别人的发言
	m_vecRecv.push_back(n);
	const TCHAR* szName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID);
 	if( _tcslen( szName ) > 0 )
	{
		//如果已经保存了对应ID的Name
		tagRoleGetNameEvent getNameEvent(_T("tagRoleGetNameEvent"), NULL);
		getNameEvent.bResult	= TRUE;
		getNameEvent.dwRoleID	= pMsg->dwSrcRoleID;
		getNameEvent.strRoleName = szName;
		OnRecvNetRoleGetName(&getNameEvent);
	}
	return 0;
}

void ChatFrame::OnSendChatMsg(const tstring& strMsg, ESendChatChannel eChannel, DWORD dwDestID, BYTE byAutoReply)
{
	if(strMsg.empty())
		return;
	tstring strTemp = strMsg;

	TCHAR mess[1024] = {0};
	tagNC_RoleChat* pMsg = (tagNC_RoleChat*)mess;
	pMsg->dwID			= m_pUtil->Crc32("NC_RoleChat");
	pMsg->dwDestRoleID	= dwDestID;						//对谁说 
	pMsg->byChannel		= (BYTE)eChannel;				//那个频道
	pMsg->byAutoReply	= byAutoReply;
	_tcsncpy(pMsg->szMsg, strTemp.c_str(),MAX_CHAT_LEN);//说话内容
	pMsg->dwSize		= sizeof(tagNC_RoleChat) +(_tcslen(pMsg->szMsg))*sizeof(TCHAR);
	m_pSession->Send(pMsg);
}

//当前聊天
void ChatFrame::OnSendCurrent()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Current)
		return;
	DeleteRETURNS(strMsg);
	//重复发言
	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		tstring strText = g_StrTable[_T("Chat_Repeat")];
		PushInfo(strText.c_str(), ESCC_System);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);
	
	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}
//公告
void ChatFrame::OnSendAffiche()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Affiche)
		return;
	DeleteRETURNS(strMsg);

	OnSendChatMsg(strMsg, m_curSendChannel);
	m_pRebInput->ClsText();
}
//传音
void ChatFrame::OnSendCastTalk()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是传音
	if(m_curSendChannel!=ESCC_CastTalk)
		return;
	//(待定)传音的限定条件

	DeleteRETURNS(strMsg);

	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		PushInfo(g_StrTable[_T("Chat_CastRepeat")], ESCC_System);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}
//世界聊天
void ChatFrame::OnSendWorld()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;
	//不是世界
	if(m_curSendChannel!=ESCC_World)
		return;
	////世界聊天的限定条件 10级前不能使用世界频道
	//LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	//if( P_VALID(pPlayer) && pPlayer->GetRoleLevel() < 10)
	//{
	//	if( P_VALID(GetObj("RoleChatWorldNo10_MsgBox")) )
	//		KillObj("RoleChatWorldNo10_MsgBox");
	//	CreateObj("RoleChatWorldNo10_MsgBox", "MsgBox");

	//	TObjRef<MsgBox>("RoleChatWorldNo10_MsgBox")->Init(_T(""), g_StrTable[_T("RoleChat_World_RoleNo10")], 
	//		_T("RoleChatWorldNo10_Event"), MsgBoxFlag(MBF_OK), TRUE);
	//	return ;
	//}

	////钱不够
	//INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
	//if( n64Silver < WORLD_CHANNEL_DEC_SLIVER )
	//{
	//	PushInfo(g_StrTable[_T("RoleChat_World_NoMoney")], ESCC_World);
	//	return ;
	//}
	

	DeleteRETURNS(strMsg);
	//重复发言
	/*if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		PushInfo(g_StrTable[_T("Chat_WorldRepeat")], ESCC_World);
		return;
	}*/
	////世界发言间隔30秒
	//if(IsIntervalSpeak(m_curSendChannel) == false)
	//{
	//	PushInfo(g_StrTable[_T("Chat_WorldRepeat")], ESCC_World);
	//	m_pRebInput->ClsText();
	//	return;
	//}
	//字数不得大于200
	if ( _tcslen(strMsg.c_str()) > 200 )
	{
		if( P_VALID(GetObj("World_Chat_Word_Limit")) )
			KillObj("World_Chat_Word_Limit");
		CreateObj( "World_Chat_Word_Limit", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("World_Chat_Word_Limit")->Init( _T(""), g_StrTable[_T("World_Chat_Word_Limit")], _T("World_Chat_Word_Limit") );
		return;
	}	

	OnSendChatMsg(strMsg, m_curSendChannel);
	float fSendTime = Kernel::Inst()->GetAccumTime();
	
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}
//私聊
void ChatFrame::OnSendPvt()
{
	//没有输入任何发言
	m_bisfromChat = TRUE;
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;
	//不是私聊
	if(m_curSendChannel!=ESCC_Pvt)
		return;
	DeleteRETURNS(strMsg);

	if( strMsg[0]==(TCHAR)0x20 )
	{
		PushInfo(g_StrTable[_T("Chat_PvtError")], ESCC_Pvt);
		return;
	}

	tstring name = HandleStringForPvt(strMsg);
	m_strLastName = name;
	
	DWORD roleID = PlayerNameTab::Inst()->FindIDByName(name.c_str());

	if(roleID!=GT_INVALID && roleID!=m_dwLocalID)
	{
		OnSendChatMsg(strMsg, m_curSendChannel, roleID);
		m_pRebInput->ClsText();
		this->SetPvtRoleName(m_strLastName.c_str());
		return;
	}
	
	if(roleID==GT_INVALID)
	{
		float fSendTime = Kernel::Inst()->GetAccumTime();
		tagSendSpeak newSpeak;
		newSpeak.fSendTime = fSendTime;
		newSpeak.eChannel  = m_curSendChannel;
		newSpeak.strDestName = name;
		newSpeak.strMsg.assign(strMsg);	
		newSpeak.byAutoReply = 0;

		//保存发言
		m_vecPvtSpeak.push_back(newSpeak);
	}
	m_pRebInput->ClsText();
}

//门派聊天
void ChatFrame::OnSendGuild()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Guild)
		return;

	LocalPlayer* pLocal = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pLocal) && !pLocal->IsInGuild() )
	{
		tstring strText = g_StrTable[_T("RoleChat_Guild_NoJoin")];
		PushInfo(strText.c_str(), ESCC_Guild);
		return;
	}

	//重复发言
	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		tstring strText = g_StrTable[_T("Chat_Repeat")];
		PushInfo(strText.c_str(), ESCC_Guild);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();

}
//队伍聊天
void ChatFrame::OnSendTroop()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Troop)
		return;
	DeleteRETURNS(strMsg);
	//重复发言
	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		tstring strText = g_StrTable[_T("Chat_Repeat")];
		PushInfo(strText.c_str(), ESCC_Troop);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}

//场景聊天
void ChatFrame::OnSendScene()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Scene)
		return;
	DeleteRETURNS(strMsg);
	//重复发言
	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		tstring strText = g_StrTable[_T("Chat_Repeat")];
		PushInfo(strText.c_str(), ESCC_System);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}

//家族聊天
void ChatFrame::OnSendFamily()
{
	//没有输入任何发言
	INT len = 0;
	tstring strMsg = m_pRebInput->GetText(len);
	if(len <= 0 || strMsg.empty())
		return;

	//不是当前聊天
	if(m_curSendChannel!=ESCC_Family)
		return;
	DeleteRETURNS(strMsg);
	//重复发言
	if(IsViladeSpeak(strMsg, m_curSendChannel) == false)
	{
		tstring strText = g_StrTable[_T("Chat_Repeat")];
		PushInfo(strText.c_str(), ESCC_System);
		return;
	}

	OnSendChatMsg(strMsg, m_curSendChannel);

	float fSendTime = Kernel::Inst()->GetAccumTime();
	tagSendSpeak newSpeak;
	newSpeak.fSendTime = fSendTime;
	newSpeak.eChannel  = m_curSendChannel;
	newSpeak.strMsg.assign(strMsg);

	//保存发言
	m_vecSpeak.push_back(newSpeak);
	m_pRebInput->ClsText();
}
//得到角色名字
DWORD ChatFrame::OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg )
{
	//检测是否是自己的ID
	if(m_dwLocalID!=GT_INVALID && pMsg->dwRoleID==m_dwLocalID)
	{
		if(m_strLocalName.empty())
		{
			m_strLocalName.assign(pMsg->strRoleName);
			ResetRecvChannel();
		}
	}

	//检测从服务器接收的发言缓存
	for (vector<tagRecvSpeak>::iterator it=m_vecRecv.begin();
		it!=m_vecRecv.end(); )
	{
		if( pMsg->bResult==TRUE && pMsg->dwRoleID==it->dwSendRoleID )
		{
				tstring strTemp;
				strTemp.append(KeyStart);
				strTemp.append(pMsg->strRoleName);
				strTemp.append(KeyEnd);
				if(it->eChannel==ESCC_Pvt)
				{
					strTemp.append(g_StrTable[_T("Chat_FromSpeek")]);
					strTemp.append(_T(" "));
				}
				else
					strTemp.append(_T(": "));
				strTemp.append(it->strText);
				DeleteRETURNS(strTemp);
				PushInfo(strTemp.c_str(), it->eChannel);
				if( it->eChannel==ESCC_Pvt)
				{
					SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
					if( P_VALID(pSocialMgr) && pSocialMgr->IsAutoReply() && it->byAutoReply==0 )
					{
						AutoReplyPvt(it->dwSendRoleID, pSocialMgr->GetReply());
					}
				}
				//--------------------密语相关--------------------
				if( it->eChannel == ESCC_Pvt)
				{
					m_destID = it->dwSendRoleID;
					OnReceiveMsg(pMsg->strRoleName,it->strText);
					UpdataMiniChat(it->dwSendRoleID);
					if(m_pMiniChatWnd->IsInvisible())
						setNowChatWith(pMsg->strRoleName,TRUE);
					//------------------------------------------------
					//聊天界面中 私聊按钮的索引是 3
					//如果当前正在显示的不是私聊频道，私聊频道有消息到来，闪烁
					/*if( m_curRecvChannel != 3 )
					{
						if( P_VALID(m_pPage[3]) )
							m_pPage[3]->SetGlintStart( true );

					}*/
					if( P_VALID(m_pBtnMiniChat)&&m_pMiniChatWnd->IsInvisible())                     //窗口未打开时且收到消息时密语按钮闪烁
					{
						m_pBtnMiniChat->BeginGlint();
						m_pBtnMiniChat->SetGlintTextColor(0xff00ffff);                              //设置闪烁时文字的颜色
					}
					//收到私聊信息，播放音效
					TObjRef<AudioSys>()->Play2DSound( _T("pvt_chat") );
				}
				
				//-------------------密语模块结束-----------------
				it = m_vecRecv.erase(it); 
				continue;
		}
		++it;
	}

	//检测传音
	for(vector<tagScroText>::iterator it=m_vecCastNoName.begin();
		it!=m_vecCastNoName.end(); )
	{
		if(pMsg->bResult==TRUE && pMsg->dwRoleID==it->dwRoleID)
		{
			if( it->url.empty() )
			{
				it->szName =  pMsg->strRoleName;
				++it;
				continue;
			}
			else
			{
				tagScroText text;
				text.szName = pMsg->strRoleName;
				text.fTime = text.fTime;
				text.url = it->url;
				m_vecCast.push_back(text);
				it = m_vecCastNoName.erase(it);
				continue;
			}
		}
		++it;
	}

	//检测展示装备的缓存
	for(vector<tagChatShowEquip>::iterator it=m_vecRecvEquip.begin();
		it!=m_vecRecvEquip.end(); )
	{
		if(it->eChannel!=ESCC_Pvt)
		{
			if(pMsg->dwRoleID==it->dwSrcID && pMsg->bResult==TRUE)
			{
				OnRoleShowEquip(it->equip, pMsg->strRoleName.c_str(), _T(""), it->eChannel);	
				it = m_vecRecvEquip.erase(it); 
				continue;
			}	
		}
		else
		{
			if(pMsg->dwRoleID==it->dwSrcID && it->strDstName!=_T(""))
			{
				OnRoleShowEquip(it->equip, pMsg->strRoleName.c_str(), it->strDstName.c_str(), it->eChannel);	
				it = m_vecRecvEquip.erase(it); 
				continue;
			}
			else if(pMsg->dwRoleID==it->dwDstID && it->strSrcName!=_T(""))
			{	
				OnRoleShowEquip(it->equip, it->strSrcName.c_str(), pMsg->strRoleName.c_str(), it->eChannel);	
				it = m_vecRecvEquip.erase(it); 
				continue;
			}
			else if(pMsg->dwRoleID==it->dwSrcID)
			{
				it->strSrcName = pMsg->strRoleName;
			}
			else
			{
				it->strDstName = pMsg->strRoleName;
			}
		}
		it++;
	}

	for(vector<tagChatShowItem>::iterator it=m_vecRecvItem.begin();
		it!=m_vecRecvItem.end(); )
	{
		if(it->eChannel!=ESCC_Pvt)
		{
			if(pMsg->dwRoleID==it->dwSrcID && pMsg->bResult==TRUE)
			{
				OnRoleShowItem(it->item, pMsg->strRoleName.c_str(), _T(""), it->eChannel);	
				it = m_vecRecvItem.erase(it); 
				continue;
			}	
		}
		else
		{
			if(pMsg->dwRoleID==it->dwSrcID && it->strDstName!=_T(""))
			{
				OnRoleShowItem(it->item, pMsg->strRoleName.c_str(), it->strDstName.c_str(), it->eChannel);	
				it = m_vecRecvItem.erase(it); 
				continue;
			}
			else if(pMsg->dwRoleID==it->dwDstID && it->strSrcName!=_T(""))
			{	
				OnRoleShowItem(it->item, it->strSrcName.c_str(), pMsg->strRoleName.c_str(), it->eChannel);	
				it = m_vecRecvItem.erase(it); 
				continue;
			}
			else if(pMsg->dwRoleID==it->dwSrcID)
			{
				it->strSrcName = pMsg->strRoleName;
			}
			else
			{
				it->strDstName = pMsg->strRoleName;
			}
		}
		it++;
	}

	return 0;
}

//得到角色ID
DWORD ChatFrame::OnRecvNetRoleGetID( tagRoleGetIDEvent* pMsg )
{
	float countTime = Kernel::Inst()->GetAccumTime();

	//处理发送私聊
	for(vector<tagSendSpeak>::iterator it=m_vecPvtSpeak.begin();
		it!=m_vecPvtSpeak.end(); )
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_tcsncpy(szBuff, it->strDestName.c_str(), X_SHORT_NAME);
		_tcslwr(szBuff);
		if( it->strMsg == _T(""))    //如果信息为空表示是从设置密语传进来的角色ID请求
		{
			m_NowRoleID = pMsg->dwRoleID;
			setNowChatWith(m_NowChatWith,false);
		}
		else if( m_pUtil->Crc32(szBuff) == pMsg->dwRoleNameCrc)
		{
			OnSendChatMsg(it->strMsg, it->eChannel, pMsg->dwRoleID, it->byAutoReply);
			m_NowRoleID = pMsg->dwRoleID;   //把从服务器返回的ID赋给正在聊天的对象ID
			it = m_vecPvtSpeak.erase(it); 
			continue;
		}
		//如果2分钟没有找到该玩家的ID则从缓存中删除保存的私聊信息
		if(countTime-(*it).fSendTime>=(float)MAX_PVT_SPEAK_VALIDETIME)
		{
			//m_vecSpeak.erase(it);
			it = m_vecPvtSpeak.erase(it); 
			continue;
		}
		++it;
	}

	return 0;
}


//接收win32消息
void ChatFrame::OnWin32Msg( DWORD dwMsg, DWORD dwParam1, DWORD dwParam2 )
{
	switch(dwMsg)
	{
	case WM_INPUTLANGCHANGE:
		{
			if( ::ImmGetDescription(::GetKeyboardLayout(0),NULL,0)==0 )
			{
				//输入法关闭状态
				SwitchLangueInfo(true);
				m_HKLenglish = ::GetKeyboardLayout( 0 );
			}
			else
			{
				SwitchLangueInfo(false);
				m_HKL = ::GetKeyboardLayout( 0 );
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if( dwParam1==VK_RETURN && m_pGUI->GetActive()!=m_pRebInput )
			{
				if(m_pMidBack->IsInvisible()==TRUE)
				{
					//窗口隐藏时,先显示窗口
					HideWnd();
					m_pGUI->SetActive(m_pRebInput);
					m_HKL = ::GetKeyboardLayout( 0 );
					if( P_VALID(m_HKL) )
					{		
						ActivateKeyboardLayout(( HKL )m_HKL, KLF_SETFORPROCESS );
					}
					
				}
				else if(m_pMiniChatWnd->IsInvisible())
				{
					m_pGUI->SetActive(m_pRebInput);
					if( P_VALID(m_HKL) )
					{		
						ActivateKeyboardLayout(( HKL )m_HKL, KLF_SETFORPROCESS );
					}

				}
			}	
			else if( dwParam1==VK_RETURN && m_pGUI->GetActive()==m_pRebInput )
			{
				m_pGUI->SetActive(m_pGUI->GetDesktop());
			}
		}
		break;
	}
}



void ChatFrame::OnRoleSendItemOrEquip(ShowEvent* pEvent)
{
	switch(m_curSendChannel)
	{
	case ESCC_World:	
		{
			/*LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
			if( P_VALID(pPlayer) && pPlayer->GetRoleLevel() < 10 && m_curSendChannel==ESCC_World)
			{
				if( P_VALID(GetObj("RoleChatWorldNo10_MsgBox")) )
					KillObj("RoleChatWorldNo10_MsgBox");
				CreateObj("RoleChatWorldNo10_MsgBox", "MsgBox");

				TObjRef<MsgBox>("RoleChatWorldNo10_MsgBox")->Init(_T(""), g_StrTable[_T("RoleChat_World_RoleNo10")], 
					_T("RoleChatWorldNo10_Event"), MsgBoxFlag(MBF_OK), TRUE);
				return ;
			}*/
			//判断放到服务器端
			//INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
			//if( n64Silver < WORLD_CHANNEL_DEC_SLIVER )
			//{
			//	PushInfo(g_StrTable[_T("E_RoleChat_World_NoIMItem")], ESCC_World);
			//	return ;
			//}
		}
	case ESCC_Guild:	
		{
			LocalPlayer* pLocal = RoleMgr::Inst()->GetLocalPlayer();
			if( P_VALID(pLocal) && !pLocal->IsInGuild() && m_curSendChannel==ESCC_Guild )
			{
				tstring strText = g_StrTable[_T("RoleChat_Guild_NoJoin")];
				PushInfo(strText.c_str(), ESCC_Guild);
				return;
			}
		}
	case ESCC_Troop:
	case ESCC_Scene:
	case ESCC_Family:
	case ESCC_Current:
		{
			EItemConType eContainerType = ChatShowItemEquip::Inst()->DragType2ContainerType((EDragType)pEvent->dwParam1);
			
			switch(eContainerType)
			{
			case EICT_Bag:
				{
					DWORD dwTypeID = pEvent->dwParam3;
					if(MIsEquipment(dwTypeID))
					{
						INT16 nPos = INT16(pEvent->dwParam2);
						Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
						if(!P_VALID(pEquip))
							return;
						tagNC_RoleShowEquip equip;
						equip.byChannel = m_curSendChannel;
						equip.dwDestRoleID = GT_INVALID;
						equip.n64Serial = pEquip->GetItemId();
						equip.eConType = eContainerType;
						m_pSession->Send(&equip);
					}
					else if(!MIsEquipment(dwTypeID))
					{
						INT16 nPos = INT16(pEvent->dwParam2);
						Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
						if(!P_VALID(pItem))
							return;
						tagNC_RoleShowItem item;
						item.byChannel = m_curSendChannel;
						item.dwDestRoleID = GT_INVALID;
						item.n64Serial = pItem->GetItemId();
						item.eConType = eContainerType;
						m_pSession->Send(&item);
					}
				}
				break;
			case EICT_Equip:
				{
					tagNC_RoleShowEquip equip;
					equip.byChannel = m_curSendChannel;
					equip.dwDestRoleID = GT_INVALID;
					equip.n64Serial = (INT64)pEvent->dwParam2;
					equip.eConType = eContainerType;
					m_pSession->Send(&equip);
				}
				break;
			case EICT_RoleWare:
				break;
			}
		}
		break;
	case ESCC_Pvt:
		{
			INT len = 0;
			tstring strMsg = m_pRebInput->GetText(len);
			if(len <= 0 || strMsg.empty())
				return;
			tstring name = HandleStringForPvt(strMsg);
			DWORD roleID = PlayerNameTab::Inst()->FindIDByName(name.c_str());

			if(roleID!=GT_INVALID && roleID!=m_dwLocalID)
			{
				EItemConType eContainerType = ChatShowItemEquip::Inst()->DragType2ContainerType((EDragType)pEvent->dwParam1);
				switch(eContainerType)
				{
				case EICT_Bag:
					{
						DWORD dwTypeID = pEvent->dwParam3;
						if(MIsEquipment(dwTypeID))
						{
							INT16 nPos = INT16(pEvent->dwParam2);
							Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
							if(!P_VALID(pEquip))
								return;
							tagNC_RoleShowEquip equip;
							equip.byChannel = ESCC_Pvt;
							equip.dwDestRoleID = roleID;
							equip.n64Serial = pEquip->GetItemId();
							equip.eConType = eContainerType;
							m_pSession->Send(&equip);
						}
						else if(!MIsEquipment(dwTypeID)) 
						{
							INT16 nPos = INT16(pEvent->dwParam2 );
							Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
							if(!P_VALID(pItem))
								return;
							tagNC_RoleShowItem item;
							item.byChannel = ESCC_Pvt;
							item.dwDestRoleID = roleID;
							item.n64Serial = pItem->GetItemId();
							item.eConType = eContainerType;
							m_pSession->Send(&item);
						}
					}
					break;
				case EICT_Equip:
					{
						tagNC_RoleShowEquip equip;
						equip.byChannel = ESCC_Pvt;
						equip.dwDestRoleID = roleID;
						equip.n64Serial = (INT64)pEvent->dwParam2;
						equip.eConType = eContainerType;
						m_pSession->Send(&equip);
					}
					break;
				case EICT_RoleWare:
					break;
				}
				m_pRebInput->ClsText();
			}
		}
		break;	
	}
}

DWORD ChatFrame::NetRecvMsgNS_RoleShowEquip( tagNS_RoleShowEquip* pMsg, DWORD dwParam )
{
	ESendChatChannel eChannel = (ESendChatChannel)pMsg->byChannel;
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			//黑名单判断
			SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
			if( P_VALID(pSocialMgr) )
			{
				SocialData* pData = pSocialMgr->GetData();
				if( P_VALID(pData) && pData->HasBlack( pMsg->dwSrcRoleID ) )
				{
					return 0;
				}

			}
			
			Equipment equip;

			if(pMsg->byEquiType == EIT_FaBao)
			{
				tagFabao fabao;
				memcpy(&fabao, &pMsg->szEquip[0], sizeof(tagFabao));
				equip = fabao;
			}
			else if (pMsg->byEquiType == EIT_SoulCrystal)
			{
				tagSoulCrystal hunjing;
				memcpy(&hunjing, &pMsg->szEquip[0], sizeof(tagSoulCrystal));
				equip = hunjing;
			}
			else if(pMsg->byEquiType == EIT_Holy)
			{
				tagHolyMan holyman;
				memcpy(&holyman, &pMsg->szEquip[0], sizeof(tagHolyMan));
				equip = holyman;
			}
			else if(pMsg->byEquiType == EIT_HolyEquip)
			{
				tagHolyEquip holyEquip;
				memcpy(&holyEquip, &pMsg->szEquip[0], sizeof(tagHolyEquip));
				equip = holyEquip;
			}
			else
			{
				tagEquip equipData;
				memcpy(&equipData, &pMsg->szEquip[0], sizeof(tagEquip));
				equip = equipData;
			}

			tstring szSrcRoleName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID);
			tstring szDstRoleName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwDestRoleID);
			if(eChannel!=ESCC_Pvt && szSrcRoleName!=_T(""))
			{
				OnRoleShowEquip(equip, szSrcRoleName.c_str(), _T(""), eChannel);
			}
			else if(eChannel==ESCC_Pvt && szSrcRoleName!=_T("") && szDstRoleName!=_T(""))
			{
				OnRoleShowEquip(equip, szSrcRoleName.c_str(), szDstRoleName.c_str(), eChannel);
			}
			else
			{
				tagChatShowEquip showequip;
				showequip.dwSrcID = pMsg->dwSrcRoleID;
				showequip.strSrcName = szSrcRoleName;
				showequip.dwDstID = pMsg->dwDestRoleID;
				showequip.strDstName = szDstRoleName;
				showequip.eChannel = eChannel;
				showequip.equip = equip;
				m_vecRecvEquip.push_back(showequip);
			}

			//世界频道，VIP剩余次数提示
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();	
			if( eChannel == ESCC_World && pMsg->dwSrcRoleID == m_dwLocalID 
				&& P_VALID(plp) && plp->GetAttribute(ERA_VipLevel)>0 && pMsg->nLeftTimes != 0)
			{
				TCHAR szText[X_LONG_NAME];
				_stprintf(szText,g_StrTable[_T("VIP_brocastTimesLeft")],pMsg->nLeftTimes);
				tagPushSpecInfoEvent event(_T("tagPushSpecInfoEvent"),NULL);
				event.strDesc = szText;
				event.eChannelType = ESCC_World;
				TObjRef<GameFrameMgr>()->SendEvent(&event);
			}
		}
		break;
	case E_RoleShowItem_ForbidChat:
		PushInfo(g_StrTable[_T("RoleShowItem_ForbidChat")], eChannel);
		break;
	case E_RoleShowItem_ServerNotReady:
		PushInfo(g_StrTable[_T("RoleShowItem_ServerNotReady")], eChannel);
		break;
	case E_RoleShowItem_SendFailure:
		PushInfo(g_StrTable[_T("RoleShowItem_SendFailure")], eChannel);
		break;
	case E_RoleShowItem_World_NoItem:
		PushInfo(g_StrTable[_T("RoleShowItem_World_NoItem")], eChannel);
		break;
	case E_RoleShowItem_World_NoMoney:
		PushInfo(g_StrTable[_T("RoleShowItem_World_NoMoney")], eChannel);
		break;
	case E_RoleShowItem_Frequently:
		PushInfo(g_StrTable[_T("RoleShowItem_Frequently")], eChannel);
		break;
	case E_RoleShowItem_Pvt_RemoteRoleLeave:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_RemoteRoleLeave")], eChannel);
		break;
	case E_RoleShowItem_Pvt_NoRoleName:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_NoRoleName")], eChannel);
		break;
	case E_RoleShowItem_Pvt_NoSelf:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_NoSelf")], eChannel);
		break;
	case E_RoleShowItem_Guild_NoJoin:
		PushInfo(g_StrTable[_T("RoleShowItem_Guild_NoJoin")], eChannel);
		break;
	case E_RoleShowItem_Troop_NoJoin:
		PushInfo(g_StrTable[_T("RoleShowItem_Troop_NoJoin")], eChannel);
		break;
	}
	return 0;
}

DWORD ChatFrame::NetRecvMsgNS_RoleShowItem( tagNS_RoleShowItem* pMsg, DWORD dwParam )
{
	ESendChatChannel eChannel = (ESendChatChannel)pMsg->byChannel;
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			//黑名单判断
			SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
			if( P_VALID(pSocialMgr) )
			{
				SocialData* pData = pSocialMgr->GetData();
				if( P_VALID(pData) && pData->HasBlack( pMsg->dwSrcRoleID ) )
				{
					return 0; 
				}

			}

			tstring szSrcRoleName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwSrcRoleID);
			tstring szDstRoleName = PlayerNameTab::Inst()->FindNameByID(pMsg->dwDestRoleID);
			
			if(eChannel!=ESCC_Pvt && szSrcRoleName!=_T(""))
			{
				OnRoleShowItem(pMsg->dwTypeID, szSrcRoleName.c_str(), _T(""), eChannel);
			}
			else if(eChannel==ESCC_Pvt && szSrcRoleName!=_T("") && szDstRoleName!=_T(""))
			{
				OnRoleShowItem(pMsg->dwTypeID, szSrcRoleName.c_str(), szDstRoleName.c_str(), eChannel);
			}
			else
			{
				tagChatShowItem showItem;
				showItem.dwSrcID = pMsg->dwSrcRoleID;
				showItem.strSrcName = szSrcRoleName;
				showItem.dwDstID = pMsg->dwDestRoleID;
				showItem.strDstName = szDstRoleName;
				showItem.eChannel = eChannel;
				showItem.item = pMsg->dwTypeID;
				m_vecRecvItem.push_back(showItem);
			}

			//世界频道，VIP剩余次数提示
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();	
			if( eChannel == ESCC_World && pMsg->dwSrcRoleID == m_dwLocalID 
				&& P_VALID(plp) && plp->GetAttribute(ERA_VipLevel)>0 && pMsg->nLeftTimes != 0)
			{
				TCHAR szText[X_LONG_NAME];
				_stprintf(szText,g_StrTable[_T("VIP_brocastTimesLeft")],pMsg->nLeftTimes);
				tagPushSpecInfoEvent event(_T("tagPushSpecInfoEvent"),NULL);
				event.strDesc = szText;
				event.eChannelType = ESCC_World;
				TObjRef<GameFrameMgr>()->SendEvent(&event);
			}

		}
		break;
	case E_RoleShowItem_ForbidChat:
		PushInfo(g_StrTable[_T("RoleShowItem_ForbidChat")], eChannel);
		break;
	case E_RoleShowItem_ServerNotReady:
		PushInfo(g_StrTable[_T("RoleShowItem_ServerNotReady")], eChannel);
		break;
	case E_RoleShowItem_SendFailure:
		PushInfo(g_StrTable[_T("RoleShowItem_SendFailure")], eChannel);
		break;
	case E_RoleShowItem_World_NoItem:
		PushInfo(g_StrTable[_T("RoleShowItem_World_NoItem")], eChannel);
		break;
	case E_RoleShowItem_World_NoMoney:
		PushInfo(g_StrTable[_T("RoleShowItem_World_NoMoney")], eChannel);
		break;
	case E_RoleShowItem_Frequently:
		PushInfo(g_StrTable[_T("RoleShowItem_Frequently")], eChannel);
		break;
	case E_RoleShowItem_Pvt_RemoteRoleLeave:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_RemoteRoleLeave")], eChannel);
		break;
	case E_RoleShowItem_Pvt_NoRoleName:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_NoRoleName")], eChannel);
		break;
	case E_RoleShowItem_Pvt_NoSelf:
		PushInfo(g_StrTable[_T("RoleShowItem_Pvt_NoSelf")], eChannel);
		break;
	case E_RoleShowItem_Guild_NoJoin:
		PushInfo(g_StrTable[_T("RoleShowItem_Guild_NoJoin")], eChannel);
		break;
	case E_RoleShowItem_Troop_NoJoin:
		PushInfo(g_StrTable[_T("RoleShowItem_Troop_NoJoin")], eChannel);
		break;
	}

	return 0;
}

void ChatFrame::OnRoleShowEquip(const Equipment& data, const TCHAR* szSrcRole, const TCHAR* szDstRole, ESendChatChannel escc)
{
	const tagEquipProto* pEquipProto = (const tagEquipProto*)data.GetProto();
	if( !P_VALID(pEquipProto) )
		return;
	
	tstringstream tss;
	tstring strEquipName = pEquipProto->pDisplayInfo->szName;
	tss<<strEquipName;
	
	INT dwBrandLevel = 0, dwConsolidateLevel = 0;
	const tagEquipSpec *pEquipSpec = data.GetEquip();
	
	if( P_VALID(pEquipSpec) )
	{
		dwBrandLevel = (INT)pEquipSpec->byConsolidateLevel;
		dwConsolidateLevel = (INT)pEquipSpec->byBrandLevel;
	}
	
	if ( dwBrandLevel != 0)
	{
		tss<<g_StrTable[_T("Chat_Show_Equip_AddSignal")]<<dwBrandLevel;
	}
	if ( dwConsolidateLevel != 0 )
	{
		tss<<g_StrTable[_T("Chat_Show_Equip_Supplement")]<<dwConsolidateLevel;
	}

	TCHAR szItemDec[1024] = {0};
	TCHAR szKey[64] = {0};
	_stprintf(szKey, _T("tagEquip%ld"), ChatShowItemEquip::Inst()->GetCurrentShowEquipIndex());
	tstring strColor = ChatShowItemEquip::Inst()->GetColorByQuality(data.GetEquip()->byQuality);
	if(escc == ESCC_Pvt)
	{
		//我对别人展示装备
		if(szSrcRole == m_strLocalName)
		{
			_stprintf(szItemDec, _T("<=(0,255,0)>%s<keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
				g_StrTable[_T("Chat_Show_1")], szDstRole, g_StrTable[_T("Chat_Show_2")], szKey, strColor.c_str(),
				tss.str().c_str());
			PushInfo(szItemDec, escc);
			ChatShowItemEquip::Inst()->AddEquipData(szKey, data);
		}
		else if(szDstRole == m_strLocalName)
		{
			_stprintf(szItemDec, _T("<=(0,255,0)><keystart>%s<keyend>%s<=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
				szSrcRole, g_StrTable[_T("Chat_Show_3")], g_StrTable[_T("Chat_Show_2")], szKey, strColor.c_str(), 
				tss.str().c_str());
			PushInfo(szItemDec, escc);
			ChatShowItemEquip::Inst()->AddEquipData(szKey, data);
		}		
	}
	else 
	{		
		_stprintf(szItemDec,_T("<=(0,255,0)><keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
			szSrcRole, g_StrTable[_T("Chat_Show_2")], szKey, strColor.c_str(), tss.str().c_str());
		PushInfo(szItemDec, escc);
		ChatShowItemEquip::Inst()->AddEquipData(szKey, data);
	}
	
}

void ChatFrame::OnRoleShowItem(DWORD data, const TCHAR* szSrcRole, const TCHAR* szDstRole, ESendChatChannel escc)
{
	const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(data);
	if( !P_VALID(pItemProto) )
		return;
	tstring strItemName = pItemProto->pDisplayInfo->szName;
	TCHAR szItemDec[1024] = {0};
	TCHAR szKey[64] = {0};
	_stprintf(szKey, _T("tagItem%ld"), ChatShowItemEquip::Inst()->GetCurrentShowItemIndex());
	tstring strColor = ChatShowItemEquip::Inst()->GetColorByQuality(pItemProto->byQuality);
	if(escc == ESCC_Pvt)
	{
		//我对别人展示装备
		if(szSrcRole == m_strLocalName)
		{
			_stprintf(szItemDec, _T("<=(0,255,0)>%s<keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
				g_StrTable[_T("Chat_Show_1")], szDstRole, g_StrTable[_T("Chat_Show_4")], szKey, strColor.c_str(),
				strItemName.c_str());
			PushInfo(szItemDec, escc);
			ChatShowItemEquip::Inst()->AddItemData(szKey, data);
		}
		else if(szDstRole == m_strLocalName)
		{
			_stprintf(szItemDec, _T("<=(0,255,0)><keystart>%s<keyend>%s<=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
				szSrcRole, g_StrTable[_T("Chat_Show_3")], g_StrTable[_T("Chat_Show_4")], szKey, strColor.c_str(),
				strItemName.c_str());
			PushInfo(szItemDec, escc);
			ChatShowItemEquip::Inst()->AddItemData(szKey, data);
		}		
	}
	else 
	{		
		_stprintf(szItemDec,_T("<=(0,255,0)><keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
			szSrcRole, g_StrTable[_T("Chat_Show_4")], szKey, strColor.c_str(), strItemName.c_str());
		PushInfo(szItemDec, escc);
		ChatShowItemEquip::Inst()->AddItemData(szKey, data);
	}
}

BOOL ChatFrame::JudePersonalityAct()
{
	tstring strText;
	m_pRebInput->GetText(strText);
	if(strText.empty())
		return FALSE;

	if(strText[0]!='/')
		return FALSE;

	const map<DWORD, tagPersonAct>& mapAct = PersonalityActData::Inst()->GetPersonActMap();

	for(map<DWORD, tagPersonAct>::const_iterator it=mapAct.begin();
		it!=mapAct.end(); ++it)
	{
		if(it->second.strCommand1 == strText || it->second.strCommand2 == strText )
		{
			tagTryToPlayPersonalityActEvent act(_T("tagTryToPlayPersonalityActEvent"), this, &it->second);
			m_pFrameMgr->SendEvent(&act);
			m_pRebInput->ClsText();
			//设置焦点
			this->SetActive();
			//关闭输入法
			if( P_VALID(m_HKLenglish) )
			{		
				ActivateKeyboardLayout(( HKL )m_HKLenglish, KLF_SETFORPROCESS );
			}
			return TRUE;
		}
	}

	return FALSE;
}

void ChatFrame::SetPvtRoleName(const TCHAR* name)
{ 
	TCHAR szKey[_MAX_PATH]={0};
	_tcscpy( szKey, name );
	_tcscat( szKey, _T(" ") );
	m_pRebInput->ClsText();
	m_pRebInput->AddText(szKey);	
	m_pGUI->SetActive(m_pRebInput);
}

DWORD ChatFrame::OnPrivateChatEvent( tagPrivateChatEvent* pEvent )
{
	m_curSendChannel = ESCC_Pvt;
	SetCurChannel(m_curSendChannel);
	SetPvtRoleName(pEvent->strRoleName.c_str());
	m_strLastName = pEvent->strRoleName;
	if(m_bChatContext)
	{
		HideWnd();
	}
	return 0;
}

void ChatFrame::SetActive()
{
	//重新设置焦点
	GUIWnd* pWnd = m_pGUI->GetDesktop();
	if( P_VALID(pWnd) )
		m_pGUI->SetActive(pWnd);
}

DWORD ChatFrame::OnSocialInfoEvent( tagSocialInfoEvent* pEvent )
{
	if( pEvent->strInfo==_T("") )
		return 0;
	PushInfo(pEvent->strInfo.c_str(), ESCC_System);
	return 0;
}

void ChatFrame::AutoReplyPvt( DWORD dwRoleID, LPCTSTR strText )
{
	OnSendChatMsg(strText, ESCC_Pvt, dwRoleID, 1);	
}

void ChatFrame::ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow )
{
	tagPoint ptSizeOldListBox = pListBox->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = pListBox->GetRowHeight() * nNumRow;

	BOOL bResizable = pListBox->IsResizable();
	pListBox->SetResizable( TRUE );
	pListBox->Resize( ptSizeListBox );
	pListBox->SetResizable( bResizable );

	GUIWnd* pFather = pListBox->GetFather();
	if( !P_VALID( pFather ) )
		return;

	bResizable = pFather->IsResizable();
	pFather->SetResizable( TRUE );
	tagPoint ptSizeFather = pFather->GetSize();
	ptSizeFather.y += ptSizeListBox.y - ptSizeOldListBox.y;
	pFather->Resize( ptSizeFather );
	pFather->SetResizable( bResizable );

	tagPoint ptOffset( 0, ptSizeOldListBox.y - ptSizeListBox.y );
	BOOL bMoveable = pFather->IsMovable();
	pFather->SetMovable( TRUE );
	pFather->Move( ptOffset );
	pFather->SetMovable( bMoveable );
}

void ChatFrame::ForceMoveMenu(const tagPoint& pos)
{
	m_pListMenuEx->SetCurrentTextRow(0);
	tagPoint pointOff((tagPoint&)pos - (tagPoint&)(m_pWndMenuEx->GetView()));
	m_pWndMenuEx->SetMovable(TRUE);
	m_pWndMenuEx->Move(pointOff);
	m_pWndMenuEx->SetMovable(FALSE);
	m_pWndMenuEx->SetInvisible(FALSE);
	m_pWndMenuEx->FlipToTop();
	m_pGUI->SetActive( m_pWndMenuEx );
}

DWORD ChatFrame::OntagDungeonEvent( tagDungeonEvent* pEvent )
{
	if( pEvent->strInfo==_T("") )
		return 0;
	//PushScrollText(pEvent->strInfo.c_str());
	PushInfo(pEvent->strInfo.c_str(), ESCC_Affiche);

	return 0;
}

DWORD ChatFrame::OnNPCEaseSpeechEvent( tagNPCEaseSpeechEvent* pEvent )
{
	Role* pRole = RoleMgr::Inst()->FindRole(pEvent->dwNPCID);
	if( !P_VALID(pRole) )
		return 0;
	if( pRole->IsHide() )
		return 0;
	const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt(pEvent->dwTypeID);
	if( P_VALID(pProto) )
	{
		ChatShowPop::Inst()->PushChatInfo( pEvent->dwNPCID, pProto->szIdleSpeech );
	}

	return 0;
}

DWORD ChatFrame::OnNS_MonsterEnterCombat( tagNS_MonsterEnterCombat* pMsg, DWORD dwParam )
{
	WorldPickerFrame* pFrame = GetWorldPickerFrm();
	if( P_VALID(pFrame) )
	{
		//是否为当前目标
		if( pFrame->GetCurSelectedRoleID() != pMsg->dwRoleID )
			return 0;
	}
	Role* pRole = RoleMgr::Inst()->FindRole(pMsg->dwRoleID);
	if( !P_VALID(pRole) )
		return 0;
	if( pRole->IsHide() )
		return 0;
	NPC* pNPC = (NPC*)pRole;
	const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt(pNPC->GetTypeID());
	if( P_VALID(pProto) )
	{
		ChatShowPop::Inst()->PushChatInfo( pMsg->dwRoleID, pProto->szCombatSpeech );
	}
	return 0;
}

DWORD ChatFrame::OnNS_RoleDead( tagNS_RoleDead* pMsg, DWORD dwParam )
{
	WorldPickerFrame* pFrame = GetWorldPickerFrm();
	if( P_VALID(pFrame) )
	{
		//是否为当前目标
		if( pFrame->GetCurSelectedRoleID() != pMsg->dwRoleID )
			return 0;
	}
	Role* pRole = RoleMgr::Inst()->FindRole(pMsg->dwRoleID, false, true);
	if( !P_VALID(pRole) )
		return 0;
	if( pRole->IsHide() )
		return 0;
	NPC* pNPC = (NPC*)pRole;
	const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt(pNPC->GetTypeID());
	if( P_VALID(pProto) )
	{
		ChatShowPop::Inst()->PushChatInfo( pMsg->dwRoleID, pProto->szDeathSpeech );
	}
	return 0;
}

DWORD ChatFrame::OnNS_Skill( tagNS_Skill* pMsg, DWORD dwParam )
{
	WorldPickerFrame* pFrame = GetWorldPickerFrm();
	if( P_VALID(pFrame) )
	{
		//是否为当前目标
		if( pFrame->GetCurSelectedRoleID() != pMsg->dwSrcRoleID )
			return 0;
	}
	if( !IS_CREATURE(pMsg->dwSrcRoleID) )
		return 0;
	Role* pRole = RoleMgr::Inst()->FindRole(pMsg->dwSrcRoleID, false, true);
	if( !P_VALID(pRole) )
		return 0;
	if( pRole->IsHide() )
		return 0;
	NPC* pNPC = (NPC*)pRole;
	const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt(pNPC->GetTypeID());
	if( P_VALID(pProto) )
	{
		ChatShowPop::Inst()->PushChatInfo( pMsg->dwSrcRoleID, pProto->szSkillSpeech );
	}

	return 0;
}

void ChatFrame::OnNS_AutoNotice( tagNS_AutoNotice* pMsg, DWORD dwParam )
{
	tstring strText = AutoNoticeColor(pMsg->nType);
	TCHAR szBuff[1024] = {0};
	INT nLen = pMsg->dwSize - sizeof(tagNS_AutoNotice) + sizeof(TCHAR);
	nLen /= sizeof(TCHAR);
	_sntprintf( szBuff, nLen, pMsg->szContent );

	tstringstream stream;
	stream << strText.c_str() << szBuff ;
	PushScrollText(stream.str().c_str(), (float)(pMsg->nCirInterval));
	stream.clear();
}

tstring ChatFrame::AutoNoticeColor( INT nType )
{
	switch(nType)
	{
	case 0:
		return _T("<=(0, 0, 255)>");
		break;
	case 1:
		return _T("<=(0, 251, 255)>");
		break;
	case 2:
		return _T("<=(0, 251, 0)>");
		break;
	case 3:
		return _T("<=(255, 0, 0)>");
		break;
	case 4:
		return _T("<=(0, 162, 255)>");
		break;
	default:
		return _T("<=(0, 251, 255)>");
		break;

	}
	return _T("<color=(224,247,255)>");
}

DWORD ChatFrame::OnRecvHeadpicByID( tagGetHeadpicByID* pGameEvent )
{
	for(vector<tagScroText>::iterator it=m_vecCastNoName.begin();
		it!=m_vecCastNoName.end(); )
	{
		if( pGameEvent->dwRoleID==it->dwRoleID)
		{
			if( it->szName.empty() )
			{
				it->url =  pGameEvent->strImageURL;
				++it;
				continue;
			}
			else
			{
				tagScroText text;
				text.szName = it->szName;
				text.fTime = it->fTime;
				text.url = pGameEvent->strImageURL;
				text.szInfo = it->szInfo;
				m_vecCast.push_back(text);
				it = m_vecCastNoName.erase(it);
				continue;
			}
		}
		++it;
	}

	return 0;
}

void ChatFrame::ChannelSet()
{
	ChatChannelSetFrame* pFrame = (ChatChannelSetFrame*)m_pFrameMgr->GetFrame( _T("ChatChannelSet") );
	if( !P_VALID( pFrame ) )
	{
		pFrame = (ChatChannelSetFrame*)m_pFrameMgr->CreateFrame( m_strName.c_str(), _T("ChatChannelSet"), _T("ChatChannelSetFrame"), 0);
		if( P_VALID(pFrame) )
		{
			pFrame->CheckChatChannelSetParam(m_dwRecvChalShow);
		}
	}
	else if( P_VALID( pFrame ) )
	{
		m_pMgr->AddToDestroyList(pFrame);
	}
}

DWORD ChatFrame::OnChannelSetEvent( tagChannelSetEvent* pGameEvent )
{
	m_dwRecvChalShow[pGameEvent->nChannel] = pGameEvent->nParam;
	SaveChannelSet();
	return 0;
}

void ChatFrame::ShowMiniChat()    //显示密语窗口
{
	BOOL b = m_pMiniChatWnd->IsInvisible();
	if( b )
		UpdataNowchat();
	m_pMiniChatWnd->SetInvisible(!b);  //显示窗口

	if(m_NowChatWith == _T(""))
		m_pBtn_Send->SetEnable(FALSE);
	m_pMiniChatWnd->FlipToTop();
	m_pMiniChatWnd->SetRefresh(TRUE);
	//m_pBtnMiniChat->SetState(EGUIBS_PopUp, FALSE);    //点完以后自己就弹起来
}

void ChatFrame::OnGroupChatButtonGlin(tagGroupChatButtonGlin* e)
{
	BOOL isOpen = FALSE;
	if (P_VALID(m_pFrameMgr->GetFrame(_T("MultChat"))))
	{
		m_pMultChatFrame = (MultChatFrame*) m_pFrameMgr->GetFrame(_T("MultChat"));
		isOpen = m_pMultChatFrame->FrameIsInvisible();
	}
	if (isOpen)
	{
		m_pBtnGroupChat->EndGlint(); 
	} 
	else if ( !m_pBtnGroupChat->IsGlintNow() )
	{
		m_pBtnGroupChat->BeginGlint();
	} 
	 
}

void ChatFrame::OnOpenMiniChat(tagOpenMiniChatEvent* e)
{
	if(m_pMiniChatWnd->IsInvisible())
	{
		m_pMiniChatWnd->SetInvisible(FALSE);
		UpdataNowchat();
	}
	if(e->name.c_str() != NULL)   //如果是传过来名字则设置该目标为正在聊天对象
		setNowChatWith(e->name,TRUE);
	m_pGUI->SetActive(m_pEB_SendMsg);
}

DWORD ChatFrame::OpenMiniChat( DWORD dwID, BOOL bDown )   //热键处理函数
{
	if(bDown)
	{
		m_pMiniChatWnd->SetInvisible(!m_pMiniChatWnd->IsInvisible());
		if(!m_pMiniChatWnd->IsInvisible())
			UpdataNowchat();
		m_pBtnMiniChat->EndGlint();
		if( !m_pMiniChatWnd->IsInvisible() )
			m_pGUI->SetActive(m_pEB_SendMsg);  //设置焦点到密语界面
		else
			m_pGUI->SetActive(m_pWnd);
		if( m_NowRoleID == GT_INVALID)
			m_pBtn_Send->SetEnable(false);
	}
	return 0;
}
void CreateDirEx(const TCHAR* Path)
{
	TCHAR DirName[256];
	const TCHAR* p = Path;
	TCHAR* q = DirName;	

	while(*p)
	{
		if ((_T('\\') == *p) || (_T('/') == *p))
		{
			if (_T(':') != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = _T('\0');
	}
	CreateDirectory(DirName, NULL);
}

void ChatFrame::SaveChannelSet()
{
	tstring strPath = Kernel::Inst()->GetWorkPath();
	strPath += _T("config\\");
	strPath += m_strLocalName;
	strPath += _T("\\");
	CreateDirEx(strPath.c_str());

	TObjRef<VarContainer> pSave = CreateObj("ChannelSet", "VarContainer");
	pSave->SetSaveSupport(TRUE);

	TCHAR szBuff[X_SHORT_NAME] = {0};
	TCHAR szPath[X_LONG_NAME] = {0};
	for( int i=0; i<NUM_CHAT_CHANNEL; ++i )
	{
		_sntprintf( szBuff, X_SHORT_NAME, _T("Channel%d"), i );
		tstring strName = _T("value ");
		strName += szBuff;
		pSave->Add( m_dwRecvChalShow[i], strName.c_str() );
	}

	_sntprintf(szPath, X_LONG_NAME, CHANNEL_SET_FILE, m_strLocalName.c_str() );
	pSave->Save(szPath);
	KillObj("ChannelSet");
}

DWORD ChatFrame::OnLeaveMsg( tagGameEvent* pGameEvent )
{
	if( pGameEvent->strEventName == _T("tagLeaveMsg") )
	{
		/*m_pBtnLeave->SetEnable(true);
		m_pBtnLeave->BeginGlint();*/
		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD ChatFrame::OnKey( DWORD dwID, BOOL bDown )
{
	if( bDown )
	{
		BOOL bShow = m_pEmotionFrame->IsInvisibleFrame();
		m_pEmotionFrame->SetInVisibleFrame(!bShow);
	}

	return 0;
}

DWORD ChatFrame::OnKeyChange( DWORD dwID, DWORD key1, DWORD key2 )
{
	m_pBtnEmotion->SetTips( ToolTipCreator::Inst()->GetHotKeyTips(g_StrTable[_T("HotKey_Emotion")], key1, key2 ).c_str() );
	return 0;
}


DWORD ChatFrame::OnNS_TemmatePickUpBroadCast( tagNS_RoleShowDroptItemInTeam* pMsg, DWORD dwParam )
{
	//信息要广播的频道
	ESendChatChannel eChannel = (ESendChatChannel)pMsg->byChannel;
	
	const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto( pMsg->dwTypeID );
	if( !P_VALID(pItemProto) )
		return 0;
	tstring strItemName = pItemProto->pDisplayInfo->szName;
	TCHAR szItemDec[1024] = {0};
	TCHAR szKey[64] = {0};
	tstring strColor;
	Equipment equip;
	if(MIsEquipment(pMsg->dwTypeID))
	{
		if( 1 == pMsg->byFabao )
		{
			tagFabao fabao;
			memcpy(&fabao, &pMsg->byEquipData[0], sizeof(tagFabao));
			equip = fabao;
		}
		else if (2 == pMsg->byFabao)
		{
			tagSoulCrystal hunjing;
			memcpy(&hunjing, &pMsg->byEquipData[0], sizeof(tagSoulCrystal));
			equip = hunjing;
		}
		else if (3 == pMsg->byFabao)
		{
			tagHolyMan holyman;
			memcpy(&holyman, &pMsg->byEquipData[0], sizeof(tagHolyMan));
			equip = holyman;
		}
		else if (4 == pMsg->byFabao)
		{
			tagHolyEquip holyEquip;
			memcpy(&holyEquip, &pMsg->byEquipData[0], sizeof(tagHolyEquip));
			equip = holyEquip;
		}
		else
		{
			tagEquip equipData;
			memcpy(&equipData, &pMsg->byEquipData[0], sizeof(tagEquip));
			equip = equipData;
		}

		strColor = ChatShowItemEquip::Inst()->GetColorByQuality(pMsg->byQlty);
		_stprintf(szKey, _T("tagEquip%ld"), ChatShowItemEquip::Inst()->GetCurrentShowEquipIndex());
	}
	else
	{
		strColor = ChatShowItemEquip::Inst()->GetColorByQuality(pItemProto->byQuality);
		_stprintf(szKey, _T("tagItem%ld"), ChatShowItemEquip::Inst()->GetCurrentShowItemIndex());
	}

	tstring szSrcRoleName = pMsg->szPickerName;
	if( szSrcRoleName.size() > 0 )
	{
		_stprintf(szItemDec,_T("<=(0,255,0)><keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
			szSrcRoleName.c_str(), g_StrTable[_T("Chat_Pick_Broadcast_PlayerName")], szKey, strColor.c_str(), strItemName.c_str());
	}
	else
	{
		_stprintf(szItemDec,_T("<=(0,255,0)><keystart>%s<keyend><=(255,255,255)>%s<linkstart=%s>%s[%s]<linkend>"),
			szSrcRoleName.c_str(), g_StrTable[_T("Chat_Pick_Broadcast_DefaultName")], szKey, strColor.c_str(), strItemName.c_str());
	}

	PushInfo( szItemDec, eChannel );
	if(MIsEquipment(pMsg->dwTypeID))
		ChatShowItemEquip::Inst()->AddEquipData(szKey, equip);
	else
		ChatShowItemEquip::Inst()->AddItemData( szKey, pMsg->dwTypeID );

	//tstring szSrcRoleName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwSrcRoleID );
	//OnRoleShowItem( pMsg->dwTypeID, szSrcRoleName.c_str(), _T(""), eChannel );

	return 0;
}

DWORD ChatFrame::OnRoleGetNameEvent(tagRoleRenameEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_strLocalName = pEvent->strRoleName;
	}
	return 0;
}

DWORD ChatFrame::OnHideOtherPlayers( DWORD dwID, BOOL bDown )
{
	if( bDown )
	{
		m_pBtnHideOthers->SetState( m_pBtnHideOthers->GetState() == EGUIBS_PushDown ? EGUIBS_PopUp: EGUIBS_PushDown );
	}
	return 0;
}


//------------------------------------------------------------密语聊天的-----------------------------------------------------------------------
BOOL ChatFrame::MiniChatReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\chat_pri.xml");
	m_pGUI->LoadXml(&element, "VFS_System",strPath.c_str());
  	m_pGUI->ChangeXml(&element, "chat_pri_win\\chat_pri_pic\\textback0\\editback\\inpupict\\editbox","ClassName","RichEditBoxForChatInput");
	m_pGUI->ChangeXml(&element, "chat_pri_win\\chat_pri_pic\\textback\\text","ClassName","RichEditBoxForChatShow");           //更改聊天信息窗口控件的类型
	m_pMiniChatWnd = m_pGUI->CreateWnd(_T("\\desktop"),&element);


	m_pBtn_Exit       = (GUIButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\closebutt"));                                  //退出
	m_pBtn_Send       = (GUIButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback0\\editback\\send"));                  //发送
	m_pBtn_SetObj     = (GUIButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback0\\set"));                             //设置对象名字的按钮
	m_pLB_ChatObj     = (GUIListBox*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\listback\\list"));                            //聊天对象ListBox
	m_pWndText        = (RichEditBoxForChatShow*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback\\text"));           //聊天信息
	m_pEB_SendMsg     = (RichEditBoxForChatInput*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback0\\editback\\inpupict\\editbox")); //发送信息框
	m_pEB_SetObj      = (GUIEditBox*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback0\\putinpic\\putin"));   //设置聊天对象名字
	m_pPB_CloseTag    = (GUIPushButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\optbutt"));                                //控制发送信息后是否关闭聊天窗口的按钮
	m_pSE_ShowObjName = (GUIStaticEx*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback\\sl_title"));                       //显示正在跟谁聊天
	m_pSB_MiniChat    = (GUIScrollBar*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback\\text\\scr"));                //聊天信息滚动条
	m_pBtn_up         = (GUIButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback\\text\\scr\\scrup"));            //聊天现实的两个按钮
	m_pBtn_down       = (GUIButton*)m_pMiniChatWnd->GetChild(_T("chat_pri_pic\\textback\\text\\scr\\scrdown"));

	m_pWndText->BeFlex();
	m_pWndText->ClsText();
	m_pLB_ChatObj->SetColNum(1,8);
    m_pEB_SetObj->SetMaxTextCount(8);
	m_pEB_SendMsg->ClsText();
	m_pEB_SendMsg->SetMaxTextCount((MAX_CHAT_LEN/2)-8);
	m_pSB_MiniChat->FlipToTop();
	m_pSB_MiniChat->SetInvisible(FALSE);
	m_pPB_CloseTag->SetState(EGUIBS_PushDown, FALSE);



	//m_pEB_SendMsg->SetMaxTextCount(MAX_CHAT_LEN/2);																	  //控制字的数量不超过100

	m_pMiniChatWnd->SetInvisible(TRUE);     //隐藏
	m_pEB_SendMsg->SetShowItemOrEquip(false);//不展示拖放进空间的物品或者装备
	m_pMiniChatWnd->SetRefresh(TRUE);
	return TRUE;
	
}


void ChatFrame::OnReceiveMsg(tstring name,tstring strMsg)
{
	//---------------------密语聊天相关----------------------------------------
	tstring strtemp;
	TCHAR msg[150] = {0};
	tstring tempcolor;
	tempcolor.append(_T("<color=(0,255,0)>"));
	tempcolor.append(name);
	tempcolor.append(_T("<color=(224,247,255)>"));
	_stprintf( msg, g_StrTable[_T("Chat_Toyou")],tempcolor.c_str(),strMsg.c_str() );
	strtemp += msg;
	DeleteRETURNS(strtemp);
	strtemp += _T("\n");
	BOOL isfind = FALSE;
	for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
	{
		if(m_destID == (*MiniChatMsgit).RoleID)
		{
			isfind = TRUE;
		}
		else 
		{
			MiniChatMsgit++;
		}
	}
	if(!isfind)
	{
		tagMiniMsg temp;
		temp.RoleID  = m_destID;
		temp.Minimsg += strtemp;
		temp.isRead = false;
		MiniChatMsg.push_front(temp);
	}
	else   
	{
		(*MiniChatMsgit).Minimsg += strtemp;                                     //移除该元素然后插入到第一位
		tagMiniMsg temp;
		temp.RoleID = (*MiniChatMsgit).RoleID;
		temp.Minimsg= (*MiniChatMsgit).Minimsg;
		temp.isRead = false;                                                      //收到消息标记为未读
		MiniChatMsg.erase(MiniChatMsgit);
		MiniChatMsg.push_front(temp);
	}
	//--------------------------------------------------------------------------
}


void ChatFrame::UpdataMiniChat(DWORD roleID)
{
	if(roleID == m_NowRoleID)    //如果当前正在跟此人说话更新显示屏
	{
		BOOL isfind = FALSE;
		for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
		{
			if(roleID == (*MiniChatMsgit).RoleID)
			{
				isfind = TRUE;
			}
			else 
			{
				MiniChatMsgit++;
			}
		}
		if(isfind)
		{
			m_pWndText->ClsText();
			m_pWndText->AddText((*MiniChatMsgit).Minimsg.c_str(), FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);   //更新当前的显示框
			if(!m_pMiniChatWnd->IsInvisible())
				(*MiniChatMsgit).isRead = true;
		}
	}

	UpdataListBox();   //更新listbox控件
	m_pWndText->SetRefresh(TRUE);

}

void ChatFrame::UpdataListBox()   //更新listbox列表
{
	int i = 0;
	for(int i = 0;i<10;i++)
	{
		m_pLB_ChatObj->SetText(i,0,_T(""),0xffffffff);
 		if(i < m_pLB_ChatObj->GetTotalRowNum())
 			m_pLB_ChatObj->Clear();
	}
	for(MiniChatMsgit = MiniChatMsg.begin(),i = 0;MiniChatMsgit != MiniChatMsg.end()&&i<10;i++,MiniChatMsgit++)
	{
		//m_pLB_ChatObj->InsertRow(i);
		//m_pLB_ChatObj->SetItemEditable(i,0,TRUE);
		tstring name;
		name = PlayerNameTab::Inst()->FindNameByID((*MiniChatMsgit).RoleID);
		if(!(*MiniChatMsgit).isRead)   
			m_pLB_ChatObj->SetText(i,0,name.c_str(),0xff00ff00);    //有未读信息的
		else 
			m_pLB_ChatObj->SetText(i,0,name.c_str(),0xffffff00);
	}
// 	for(;MiniChatMsgit != MiniChatMsg.end();MiniChatMsgit++)   //删除超过10条的信息
// 	{
// 		MiniChatMsg.erase(MiniChatMsgit);
// 	}
}

void ChatFrame::setNowChatWith(tstring name,BOOL isresetname)  //isresetname表示是否重新设置名字
{ 
	if(isresetname == TRUE)
	{
		m_NowChatWith = name;
		m_NowRoleID = PlayerNameTab::Inst()->FindIDByName(name.c_str());
	}
	m_dwLocalID = RoleMgr::Inst()->GetLocalPlayer()->GetID();
	m_pBtnMiniChat->EndGlint(); 
	BOOL isfind = FALSE;
	if(P_VALID(m_NowRoleID)&&m_NowRoleID != m_dwLocalID)
	{
		TCHAR chatwith[50] = {0};
		_stprintf( chatwith, g_StrTable[_T("ChatWith")],m_NowChatWith.c_str() );
		m_pSE_ShowObjName->SetText(chatwith);        //和XXX聊天
		for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)  //查找是否已经有此人
		{
			if((*MiniChatMsgit).RoleID == m_NowRoleID)
				isfind = TRUE;
			else
				MiniChatMsgit++;
		}
		if(!isfind)  //如果没找到添加一个新节点
		{
			tagMiniMsg temp ;
			temp.RoleID = m_NowRoleID;
			temp.isRead = false;
			MiniChatMsg.push_front(temp);
			m_pWndText->ClsText();
		}
		else
		{
			if(!isresetname)  //如果不是点击密语框要求重新设置的话则不把当前对象放到第一位
			{
				tagMiniMsg temp ;
				temp.RoleID = m_NowRoleID;
				temp.isRead = false;
				temp.Minimsg= (*MiniChatMsgit).Minimsg;
				MiniChatMsg.erase(MiniChatMsgit);
				MiniChatMsg.push_front(temp);
			}
			
		}
		UpdataMiniChat(m_NowRoleID);
		m_pBtn_Send->SetEnable(TRUE);
		m_pBtn_Send->SetRefresh(TRUE);
	}
	else 
	{
		TCHAR msg[150] = {0};
		if(m_dwLocalID != m_NowRoleID)
		{
			_stprintf( msg, g_StrTable[_T("Pvt_Error")] );       //角色不存在
		}
		else _stprintf( msg, g_StrTable[_T("Pvt_LocalError")] ); //对象不能为自己
		m_pWndText->ClsText();
		m_pWndText->AddText(msg, FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);
		m_pSE_ShowObjName->SetText(_T(""));
		m_pBtn_Send->SetEnable(FALSE);
		m_pBtn_Send->SetRefresh(TRUE);

	}
}

void ChatFrame::UpdataNowchat()  //正在跟但前对象聊天时把信息设置为已读
{
	BOOL isfind = FALSE;
	for(MiniChatMsgit = MiniChatMsg.begin();MiniChatMsgit != MiniChatMsg.end()&&!isfind;)
	{
		if(m_NowRoleID == (*MiniChatMsgit).RoleID)
		{
			isfind = TRUE;
		}
		else 
		{
			MiniChatMsgit++;
		}
	}
	if(isfind)
	{
		(*MiniChatMsgit).isRead = true;  //设置为已读
		UpdataListBox();
	}
}

VOID ChatFrame::BigScrollTextCancelAfterThisTime()
{
	m_pRunHorseScrollText->SetScrollTime(0);
}
void		 ChatFrame::NetRecvMessage_SendToPrisonEx(tagNS_SendToPrisonEx* pMsg, DWORD pParam)
{
	if( P_VALID(pMsg) )
	{
		TCHAR szInfo[512] = {0};
		_stprintf( szInfo, g_StrTable[_T("Dungeon_SendToPrison")], pMsg->strCaptorRoleName, pMsg->strCatcherRoleName );

		tstringstream stream;
		stream << szInfo ;
		PushScrollText(stream.str().c_str(), 60);
		stream.clear();
	}
}

BOOL ChatFrame::HaveWorldSpeakTool()
{
	ItemContainer *pPocket = ItemMgr::Inst()->GetPocket();
	INT nNum = 0;
	if (P_VALID(pPocket))
	{
		nNum = pPocket->GetItemQuantity(CSDW_World_Tool_TypeID1);
		nNum = nNum + pPocket->GetItemQuantity(CSDW_World_Tool_TypeID2);
		if ( nNum > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

DWORD ChatFrame::OnNS_SomeoneViewYourEquip( tagNS_SomeoneViewYourEquip* pMsg, DWORD dwParam )
{
	if(SaveAccount::Inst()->GetLoginMode() != 7 && SaveAccount::Inst()->GetLoginMode() != 0 )
	{
		TCHAR buffer[X_LONG_NAME];
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), g_StrTable[_T("SomeOneViewingYourEquip")], pMsg->szRoleName);

		float countTime = Kernel::Inst()->GetAccumTime();

		if(countTime - m_dwViewEquipTime >= 10)
		{
			PushInfo(buffer, ESCC_Current);
			m_dwViewEquipTime = countTime;
		}
	}

	return 0;
}

void ChatFrame::UpDateFangChenMi()
{
	tagDWORDTime dwServerTime = ServerTime::Inst()->CalCurrentServerDwordTime();
	DWORD dwSecond = CalcTimeDiff(dwServerTime,m_stStartTime);
	if(dwSecond/3600 > m_nOnlineHour)
	{
		TCHAR buffer[X_LONG_NAME];
		TCHAR zs[X_SHORT_NAME];

		_sntprintf(zs, sizeof(zs)/sizeof(TCHAR),_T("FangChenMi_HanGuo_hour_%d"), m_nOnlineHour);

		if(m_nOnlineHour < 4)
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR),g_StrTable[_T("FangChenMi_HanGuo")], m_nOnlineHour,_T("。"));
		else if(m_nOnlineHour < 13)
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR),g_StrTable[_T("FangChenMi_HanGuo")], m_nOnlineHour,g_StrTable[zs]);
		else
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR),g_StrTable[_T("FangChenMi_HanGuo")], m_nOnlineHour,g_StrTable[_T("FangChenMi_HanGuo_hour_13")]);

		PushInfo(buffer, ESCC_Current);
		m_nOnlineHour++;
	}
}