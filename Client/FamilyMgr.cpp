#include "StdAfx.h"
#include "FamilyMgr.h"
#include "..\WorldDefine\msg_family.h"
#include "FamilyProtoData.h"
#include "MsgBox.h"
#include "QuestMgr.h"
#include "RoleMgr.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "FilterData.h"
#include "CombatSysUtil.h"
#include "ChatFrame.h"
#include "..\WorldDefine\chat_define.h"
#include "PlayerNameTab.h"
#include "FamilyEvent.h"
#include "FamilyMemberFrame.h"
#include "FamilyEvent.h"
#include "CreatureData.h"
#include "MapMgr.h"
#include "FamilyFrame.h"

FamilyMgr g_familyMgr;
FamilyMgr* FamilyMgr::Inst()
{
	return &g_familyMgr;
}


FamilyMgr::FamilyMgr(void):m_trunk(this)
,m_bMsgBoxMutex(FALSE)
{
	
	for (int i =0; i< EFSA_Num; i++)
	{
		m_dwSpriteAtt[i]=0;
	}
}

FamilyMgr::~FamilyMgr(void)
{
}

void FamilyMgr::Init()
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();
	FamilyProtoData::Inst()->LoadFromFile();

	m_pFamilyFrame =  (FamilyFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Family"), _T("FamilyFrame"), 0);
	m_pFamilyFrame->ShowWnd(FALSE);
	// common----------------------------------------------------------------------------------------------------
	
	// 家族成员----------------------------------------------------------------------------------------------------
	m_setNameReq.clear();
	m_mapFamilyName.clear();
	m_mapInivteCache.clear();
	m_mapApplyCache.clear();
	ClearFamilyMemberMap();
	m_bMsgBoxMutex = FALSE;

	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_Create"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyCreate));
	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_Break"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyBreak));
	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_Invite"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyInvite));
	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_Apply"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyApply));
	m_pFrameMgr->RegisterEventHandle(_T("FamilyEvent_Event"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyEvent));

	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_CreateFamily"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgbox_CreateFamily));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_InputFamilyName"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_InputFamilyName));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_BreakFamily"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_BreakFamily));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_FamilyJoinReq"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyJoinReq));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_FamilyApplyReq"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyApplyReq));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_FamilyEvent"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyEvent));

	m_pCmdMgr->Register("NS_FamilyProcFailed",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyProcFailed),		_T("NS_FamilyProcFailed"));
	m_pCmdMgr->Register("NS_CreateFamilyBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_CreateFamilyBroad),		_T("NS_CreateFamilyBroad"));
	m_pCmdMgr->Register("NS_FamilyDismiss",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyDismiss),			_T("NS_FamilyDismiss"));
	m_pCmdMgr->Register("NS_FamilyJoinReq",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinReq),			_T("NS_FamilyJoinReq"));
	m_pCmdMgr->Register("NS_FamilyJoinReqRes",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinReqRes),		_T("NS_FamilyJoinReqRes"));
	m_pCmdMgr->Register("NS_FamilyApply",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyApply),				_T("NS_FamilyApply"));
	m_pCmdMgr->Register("NS_FamilyApplyReqRes",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyApplyReqRes),		_T("NS_FamilyApplyReqRes"));
	m_pCmdMgr->Register("NS_FamilyJoinBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinBroad),			_T("NS_FamilyJoinBroad"));
	m_pCmdMgr->Register("NS_FamilyHandOverBroad",	(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyHandOverBroad),		_T("NS_FamilyHandOverBroad"));
	m_pCmdMgr->Register("NS_FamilyKickBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyKickBroad),			_T("NS_FamilyKickBroad"));
	m_pCmdMgr->Register("NS_FamilyLeaveBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyLeaveBroad),		_T("NS_FamilyLeaveBroad"));
	m_pCmdMgr->Register("NS_GetFamilyName",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_GetFamilyName),			_T("NS_GetFamilyName"));
	m_pCmdMgr->Register("NS_GetFamilyInfo",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_GetFamilyInfo),			_T("NS_GetFamilyInfo"));
	m_pCmdMgr->Register("NS_CreateFamilyAllBroad",  (NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_CreateFamilyAllBroad),    _T("NS_CreateFamilyAllBroad"));

	// 家族任务----------------------------------------------------------------------------------------------------
	m_pCmdMgr->Register("NS_OpenFamilyQuestPage", (NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_OpenFamilyQuestPage), _T("NS_OpenFamilyQuestPage"));

	// 家族妖精----------------------------------------------------------------------------------------------------
	m_pCmdMgr->Register("NS_FamilySpritTrainBroad",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilySpritTrainBroad),			_T("NS_FamilySpritTrainBroad"));
	

}
void FamilyMgr::Destroy()
{

	// common----------------------------------------------------------------------------------------------------
	
	// 家族成员----------------------------------------------------------------------------------------------------
	KillObj_NPCTalk();

	m_setNameReq.clear();
	m_mapFamilyName.clear();
	m_mapInivteCache.clear();
	m_mapApplyCache.clear();
	ClearFamilyMemberMap();
	m_bMsgBoxMutex = FALSE;

	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_Create"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyCreate));
	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_Break"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyBreak));
	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_Invite"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyInvite));
	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_Apply"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyApply));
	m_pFrameMgr->UnRegisterEventHandler(_T("FamilyEvent_Event"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnEventFamilyEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_CreateFamily"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgbox_CreateFamily));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_InputFamilyName"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_InputFamilyName));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_BreakFamily"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_BreakFamily));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_FamilyJoinReq"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyJoinReq));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_FamilyApplyReq"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyApplyReq));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_FamilyEvent"),		(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMgr::OnMsgBox_FamilyEvent));
	
	m_pCmdMgr->UnRegister("NS_FamilyProcFailed",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyProcFailed));
	m_pCmdMgr->UnRegister("NS_CreateFamilyBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_CreateFamilyBroad));
	m_pCmdMgr->UnRegister("NS_FamilyDismiss",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyDismiss));
	m_pCmdMgr->UnRegister("NS_FamilyJoinReq",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinReq));
	m_pCmdMgr->UnRegister("NS_FamilyJoinReqRes",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinReqRes));
	m_pCmdMgr->UnRegister("NS_FamilyApply",				(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyApply));
	m_pCmdMgr->UnRegister("NS_FamilyApplyReqRes",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyApplyReqRes));
	m_pCmdMgr->UnRegister("NS_FamilyJoinBroad",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyJoinBroad));
	m_pCmdMgr->UnRegister("NS_FamilyHandOverBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyHandOverBroad));
	m_pCmdMgr->UnRegister("NS_FamilyKickBroad",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyKickBroad));
	m_pCmdMgr->UnRegister("NS_FamilyLeaveBroad",		(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilyLeaveBroad));
	m_pCmdMgr->UnRegister("NS_GetFamilyName",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_GetFamilyName));
	m_pCmdMgr->UnRegister("NS_GetFamilyInfo",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_GetFamilyInfo));
	m_pCmdMgr->UnRegister("NS_CreateFamilyAllBroad",    (NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_CreateFamilyAllBroad));

	
	// 家族任务----------------------------------------------------------------------------------------------------
	m_pCmdMgr->UnRegister("NS_OpenFamilyQuestPage", (NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_OpenFamilyQuestPage));

	// 家族妖精----------------------------------------------------------------------------------------------------
	m_pCmdMgr->UnRegister("NS_FamilySpritTrainBroad",			(NETMSGPROC)m_trunk.sfp2(&FamilyMgr::OnNS_FamilySpritTrainBroad));



}


DWORD FamilyMgr::OnEventFamilyCreate( tagGameEvent* pGameEvent )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	//// 如果有家族则返回
	//if(GT_VALID(pLp->GetRoleGuildID()))
	//{
	//	ShowGuildErrorMsg(E_Guild_Create_AlreadyIn);
	//	return 0;
	//}
	//// 等级不够则显示
	//if(pLp->GetRoleLevel() < CREATE_NEED_LEVEL)
	//{
	//	ShowGuildErrorMsg(E_Guild_Create_LevelLimit);
	//	return 0;
	//}
	//// 金钱不足则显示
	//if(CurrencyMgr::Inst()->GetBagSilver() < MGold2Silver(CREATE_NEED_GOLD))
	//{
	//	ShowGuildErrorMsg(E_BagSilver_NotEnough);
	//	return 0;
	//}

	KillObj_NPCTalk();

	CreateObj("CreateFamily", "MsgBox");
	TObjRef<MsgBox>("CreateFamily")->Init( 
		_T(""), g_StrTable[_T("Family_CreateTips")],       
		_T("MsgBox_CreateFamily"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );

	return 0;
}

DWORD FamilyMgr::OnEventFamilyBreak( tagGameEvent* pGameEvent )
{
	KillObj_NPCTalk();

	CreateObj("BreakFamily", "MsgBox");
	TObjRef<MsgBox>("BreakFamily")->Init( 
		_T(""), g_StrTable[_T("Family_BreakTips")],       
		_T("MsgBox_BreakFamily"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );

	return 0;
}

DWORD FamilyMgr::OnEventFamilyInvite( tagFamilyInviteEvent* pGameEvent )
{
	tstring strName = pGameEvent->strRoleName;
	INT nNameStrLen = _tcslen(strName.c_str());

	INT size = ( nNameStrLen - 1) * sizeof(TCHAR) + sizeof(tagNC_FamilyJoinReq);
	LPBYTE pMsg = new BYTE[size];

	tagNC_FamilyJoinReq* pCmd = (tagNC_FamilyJoinReq*)pMsg;
	pCmd->dwID = pCmd->Crc32("NC_FamilyJoinReq");
	pCmd->dwDstRoleID = pGameEvent->dwRoleID;
	memcpy(pCmd->szDstRoleName, strName.c_str(), nNameStrLen*sizeof(TCHAR));
	pCmd->dwSize = size;
	m_pSession->Send(pCmd);
	SAFE_DEL_ARRAY(pMsg);

	return 0;
}

DWORD FamilyMgr::OnEventFamilyApply( tagFamilyApplyEvent* pGameEvent )
{
	tstring strName = pGameEvent->strRoleName;
	INT nNameStrLen = _tcslen(strName.c_str());

	INT size = ( nNameStrLen - 1) * sizeof(TCHAR) + sizeof(tagNC_FamilyApply);
	LPBYTE pMsg = new BYTE[size];

	tagNC_FamilyApply* pCmd = (tagNC_FamilyApply*)pMsg;
	pCmd->dwID = pCmd->Crc32("NC_FamilyApply");
	pCmd->dwApplyRoleID = pGameEvent->dwRoleID;
	memcpy(pCmd->szDstRoleName, strName.c_str(), nNameStrLen*sizeof(TCHAR));
	pCmd->dwSize = size;
	m_pSession->Send(pCmd);
	SAFE_DEL_ARRAY(pMsg);

	//tagNC_FamilyApply cmd;
	//cmd.dwApplyRoleID = pGameEvent->dwRoleID;
	//cmd.szDstRoleName	= 
	//m_pSession->Send(&cmd);

	return 0;
}

DWORD FamilyMgr::OnEventFamilyEvent( tagFamilyEvent* pGameEvent )
{
	if ( TRUE == m_bMsgBoxMutex )
		return 0;

	/*	EFET_ChangeLeader;	//移交族长
		EFET_MemberDismis;	//开出成员
		EFET_LeaveFamily;	//离开家族  */
	EFamilyEventType eEventType	= pGameEvent->eEventType;
	if ( EFET_NoEvent >= eEventType || EFET_EventNum <= eEventType )
		return 0;

	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer))
		return 0;

	DWORD	dwRoleID = GT_INVALID;
	//开出、移交 
	if ( EFET_ChangeLeader==eEventType || EFET_MemberDismis==eEventType )
	{
		// 非族长
		if ( !pPlayer->IsRoleFamilyLeader() )
		{
			tstringstream tss;
			tss<<_T("Family_EventRespond")<<(INT)eEventType;
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);
			return 0;
		}
		//需判断选择的索引是否合法
		DWORD dwRoleIndex = pGameEvent->dwRoleIndex;
		const INT cnMemberNum = (INT)m_mapFamilyMember.size();
		if ( dwRoleIndex <= 0/*选的是族长*/	|| (INT)dwRoleIndex >= cnMemberNum/*选择越界*/ ) 
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FamilyEvent_IllegalIndex_22")]);
			return 0;
		}
		else
		{
			std::map<DWORD, DWORD>::const_iterator cit = m_mapFamilyMember.find(dwRoleIndex);
			if( cit!=m_mapFamilyMember.end() )
				dwRoleID = cit->second;
		}
	}
	// 离开
	else if ( EFET_LeaveFamily==eEventType )
	{
		// 族长需判断成员数目
		if ( pPlayer->IsRoleFamilyLeader() && 1 >= (INT)m_mapFamilyMember.size())
			dwRoleID = GT_INVALID;
		else 
			dwRoleID=0;
			//CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FamilyEvent_IllegalIndex_3")]);	
	}

	if ( dwRoleID==GT_INVALID ) //开出、移交 dwRoleID为正常的id， 离开的话id=0
	{
		tstringstream tssIndex;
		tssIndex<<_T("FamilyEvent_IllegalIndex_")<<(INT)eEventType;
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tssIndex.str().c_str()]);
		return 0;
	}

	//夫妻判断
	//do something

	TCHAR	szBuffer[X_LONG_NAME] = {0};
	tstringstream tssMsgbox;
	tssMsgbox<<_T("FamilyEvent_PopMsgBox_")<<(INT)eEventType;
	tstring strName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[tssMsgbox.str().c_str()],strName.c_str());

	if(P_VALID(GetObj("FamilyEvent_PopMsgBox")))
		KillObj("FamilyEvent_PopMsgBox");

	CreateObj("FamilyEvent_PopMsgBox", "MsgBox");
	TObjRef<MsgBox>("FamilyEvent_PopMsgBox")->Init( _T(""), szBuffer,       
		_T("MsgBox_FamilyEvent"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE );

	TObjRef<MsgBox>("FamilyEvent_PopMsgBox")->SetDwordUserData(dwRoleID);
	TObjRef<MsgBox>("FamilyEvent_PopMsgBox")->SetDwordUserData2((DWORD)eEventType);

		m_bMsgBoxMutex = TRUE;

	return 0;
}

DWORD FamilyMgr::OnMsgbox_CreateFamily( tagMsgBoxEvent* pGameEvent )
{
	if( MBF_OK == pGameEvent->eResult)
	{
		//创建判断
		//do something

		if(P_VALID(GetObj("CreateFamily_Input")))
			KillObj("CreateFamily_Input");

		CreateObj("CreateFamily_Input", "MsgInputBox");
		TObjRef<MsgInputBox>("CreateFamily_Input")->Init(
			_T(""),g_StrTable[_T("Family_InputName")],
			_T("MsgBox_InputFamilyName"),GT_INVALID, MBIT_Name, NULL, TRUE);
	}

	return 0;

}

DWORD FamilyMgr::OnMsgBox_InputFamilyName( tagMsgInputBoxEvent* pGameEvent )
{
	if( MBF_OK == pGameEvent->eResult && MBIT_Name == pGameEvent->eInputType )
	{
		tstring tstrInput = pGameEvent->strInputName;
		
		//判断名字是否非法
		DWORD dwErrorCode = FilterData::Inst()->IsValidName(tstrInput.c_str(), MAX_FAMILY_NAME_LEN);
		if(E_Success == dwErrorCode)
		{
			//发送网络消息
			INT nNameStrLen = _tcslen(tstrInput.c_str());
			
			INT size = ( nNameStrLen - 1) * sizeof(TCHAR) + sizeof(tagNC_CreateFamily);
			LPBYTE pMsg = new BYTE[size];

			tagNC_CreateFamily* pCmd = (tagNC_CreateFamily*)pMsg;
			pCmd->dwID = pCmd->Crc32("NC_CreateFamily");
			pCmd->dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
			memcpy(pCmd->szFamilyName, tstrInput.c_str(), nNameStrLen*sizeof(TCHAR));
			pCmd->dwSize = size;
			m_pSession->Send(pCmd);

			SAFE_DEL_ARRAY(pMsg);
		}
		else
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NameInput_Invalid")]);

	}

	return 0;
}

DWORD FamilyMgr::OnMsgBox_BreakFamily( tagMsgBoxEvent* pGameEvent )
{
	if( MBF_OK == pGameEvent->eResult)
	{
		//解散判断
		//do something
		
		tagNC_FamilyDismiss pCmd;
		pCmd.dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		m_pSession->Send(&pCmd);

	}

	return 0;
}

DWORD FamilyMgr::OnMsgBox_FamilyJoinReq( tagMsgBoxEvent* pGameEvent )
{
	
	DWORD dwFamilyID	= TObjRef<MsgBox>("FamilyJoinReq")->GetDwordUserData();
	DWORD dwInviterID	= TObjRef<MsgBox>("FamilyJoinReq")->GetDwordUserData2();
	
	if ( dwFamilyID == GT_INVALID || dwInviterID == GT_INVALID )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Family_JionReq_WrongID")]);
		return 0;
	}

	// 发送请求结果
	tagNC_FamilyJoinReqRes cmd;
	cmd.dwDstRoleID		= dwInviterID;
	cmd.dwDstFamilyID	= dwFamilyID;
	if(MBF_OK == pGameEvent->eResult)
		cmd.dwErrorCode = E_Success;
	else 
		cmd.dwErrorCode = E_FamilyJoin_Refuse;
	m_pSession->Send(&cmd);

	return 0;
}

DWORD FamilyMgr::OnMsgBox_FamilyApplyReq( tagMsgBoxEvent* pGameEvent )
{
	DWORD dwApplyID	= TObjRef<MsgBox>("FamilyApplyReq")->GetDwordUserData();

	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLp) && GT_INVALID != dwApplyID &&pLp->IsInFamily() )
	{
		// 发送请求结果
		tagNC_FamilyApplyRes cmd;
		cmd.dwDstRoleID		= dwApplyID;
		cmd.dwDstFamilyID	= pLp->GetRoleFamilyID();
		if(MBF_OK == pGameEvent->eResult)
			cmd.dwErrorCode = E_Success;
		else 
			cmd.dwErrorCode = E_FamilyApply_Refuse;
		m_pSession->Send(&cmd);
	}	

	return 0;
}

DWORD FamilyMgr::OnMsgBox_FamilyEvent( tagMsgBoxEvent* pGameEvent )
{
	m_bMsgBoxMutex = FALSE;
	DWORD			 dwRoleID	= TObjRef<MsgBox>("FamilyEvent_PopMsgBox")->GetDwordUserData();
	EFamilyEventType eEventType = (EFamilyEventType)(TObjRef<MsgBox>("FamilyEvent_PopMsgBox")->GetDwordUserData2());
	if( MBF_OK==pGameEvent->eResult )
	{
		switch (eEventType)
		{
		case EFET_ChangeLeader:
			{
				if (dwRoleID!=GT_INVALID)
				{
					tagNC_FamilyHandOver cmd;
					cmd.dwRoleID = dwRoleID;
					m_pSession->Send(&cmd);
				}
			}
			break;
		case EFET_MemberDismis:
			{
				if (dwRoleID!=GT_INVALID)
				{
					tagNC_FamilyKick cmd;
					cmd.dwRoleID = dwRoleID;
					m_pSession->Send(&cmd);
				}
			}
			break;
		case EFET_LeaveFamily:
			{
				if (dwRoleID==0)
				{
					tagNC_FamilyLeave cmd;
					m_pSession->Send(&cmd);
				}
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

DWORD FamilyMgr::OnNS_FamilyProcFailed(tagNS_FamilyProcFailed* pNetCmd, DWORD)
{
	DWORD dwErr = pNetCmd->dwErrorCode;

	ShowErrorMsg(dwErr);

	return 0;
}

DWORD FamilyMgr::OnNS_CreateFamilyBroad(tagNS_CreateFamilyBroad* pNetCmd, DWORD)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;
	
	ClearFamilyInfo();
	// 设置本地玩家家族ID
	pLp->SetRoleFamilyID(pNetCmd->dwFamilyID);
	// 是否族长
	if( pNetCmd->dwRoleID == pLp->GetID() )
		pLp->SetRoleFamilyLeader(true);

	// pNetCmd->szName = "角色名\0家族名\0"
	TCHAR szTmp[X_SHORT_NAME] =  { _T('\0') } ;
	// 计算家族名的偏移量
	INT nPos = _tcslen(pNetCmd->szName) + 1;
	// 计算家族名的长度
	INT nSize = (pNetCmd->dwSize - sizeof(tagNS_CreateFamilyBroad))/sizeof(TCHAR) + 1 - nPos;
	_tcsncpy(szTmp, pNetCmd->szName+nPos, nSize);

	// 显示聊天信息Family_CreateBroad
	ShowFamilyMsg( g_StrTable[_T("Family_CreateBroad")], pNetCmd->szName, szTmp );

	//refresh ui
	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	if( !P_VALID(pFamilyFrame) )
		pFamilyFrame =  (FamilyFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Family"), _T("FamilyFrame"), 0);
	if (P_VALID(pFamilyFrame))
		pFamilyFrame->ShowWnd(TRUE);

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyDismiss(tagNS_FamilyDismiss* pNetCmd, DWORD)
{
	// 如果发布者是当前玩家，删除家族信息
	ClearFamilyInfo();

	// 显示聊天信息
	ShowFamilyMsg( g_StrTable[_T("Family_DismissBroad")] );
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Family_DismissBroad")]);
	
	//refresh ui
	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	if (P_VALID(pFamilyFrame))
		pFamilyFrame->ShowWnd(FALSE);

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyJoinReq(tagNS_FamilyJoinReq* pNetCmd, DWORD)
{
	DWORD dwInviterID		= pNetCmd->dwSrcRoleID;
	DWORD dwInviterFamilyID	= pNetCmd->dwFamilyID;
	// 获取邀请者的姓名和家族名
	tstring strInviter = PlayerNameTab::Inst()->FindNameByID(dwInviterID);
	if (strInviter.empty())
		strInviter	= pNetCmd->szSrcRoleName;
	
	tstring strFamily	= GetFamilyName(dwInviterFamilyID,ENUT_Invite,dwInviterID, strInviter);
	// 名字和家族为空则返回
	if( strInviter.empty() || strFamily.empty() )
		return 0;

	ShowInvite(dwInviterFamilyID, dwInviterID, strInviter);

	return 0;

}



DWORD FamilyMgr::OnNS_FamilyJoinBroad(tagNS_FamilyJoinBroad* pNetCmd, DWORD)
{
	// 显示聊天信息
	ShowFamilyMsg(g_StrTable[_T("Family_JionFamily")], pNetCmd->szRoleName);
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Family_JionFamily")], pNetCmd->szRoleName );
	
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;

	// 如果是本地玩家被邀请
	if(pNetCmd->dwRoleID == pLp->GetID())
	{
		ClearFamilyInfo();
		// 设置本地玩家家族ID
		pLp->SetRoleFamilyID(pNetCmd->dwFamilyID);
	}
	else
	{
		//// 家族信息未获取则不处理该信息
		//if(!P_VALID(m_pGuild))
		//	return 0;
		//// 家族成员信息未获取则只改变成员数量
		//if(!P_VALID(m_pMembers))
		//{
		//	// 玩家加入家族成员数量自加
		//	++m_pGuild->n16MemberNum;
		//	return 0;
		//}

		//// 发送网络消息获取玩家信息
		//tagNC_GuildRefMember cmd;
		//cmd.dwRoleID = pNetCmd->dwRoleID;
		//m_pSession->Send(&cmd);
	}

	//refresh ui
	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	if (P_VALID(pFamilyFrame))
	{
		if (pFamilyFrame->IsShowing() )
			pFamilyFrame->ShowWnd(TRUE);	
		else
			pFamilyFrame->ShowWnd(FALSE);	
	}
	
	return 0;
}

DWORD FamilyMgr::OnNS_FamilyHandOverBroad(tagNS_FamilyHandOverBroad* pNetCmd, DWORD)
{
	//屏显、家族频道广播
	DWORD dwOriLeader = pNetCmd->dwSrcRoleID;
	DWORD dwNewLeader = pNetCmd->dwDstRoleID;
	
	tstring strOriLeader = PlayerNameTab::Inst()->FindNameByID(dwOriLeader);
	tstring strNewLeader = PlayerNameTab::Inst()->FindNameByID(dwNewLeader);

	if (!strOriLeader.empty()&&!strNewLeader.empty())
	{
		ShowFamilyMsg(g_StrTable[_T("FamilyEvent_ChangeLeader")], strNewLeader.c_str(), strOriLeader.c_str() );
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("FamilyEvent_ChangeLeader")], strNewLeader.c_str(), strOriLeader.c_str() );
	}

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;

	// 如果是本地玩家被移交
	if( dwNewLeader == pLp->GetID() )
	{
		pLp->SetRoleFamilyLeader(TRUE);
	}
	else if ( dwOriLeader == pLp->GetID() )
	{
		pLp->SetRoleFamilyLeader(FALSE);
	}

	//refresh ui
	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	if (P_VALID(pFamilyFrame)&&pFamilyFrame->IsShowing())
		pFamilyFrame->ShowWnd(TRUE);	

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyKickBroad(tagNS_FamilyKickBroad* pNetCmd, DWORD)
{
	//屏显、家族频道广播
	DWORD dwRoleID = pNetCmd->dwDstRoleID;
	tstring strRole = PlayerNameTab::Inst()->FindNameByID(dwRoleID);

	ShowFamilyMsg( g_StrTable[_T("FamilyEvent_KickMember")], strRole.c_str()  );
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("FamilyEvent_KickMember")], strRole.c_str() );

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;

	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	// 如果是本地玩家离开
	if(pNetCmd->dwDstRoleID == pLp->GetID())
	{
		ClearFamilyInfo();
		if (P_VALID(pFamilyFrame)&&pFamilyFrame->IsShowing())
			pFamilyFrame->ShowWnd(FALSE);
	}
	else
	{
		if (P_VALID(pFamilyFrame)&&pFamilyFrame->IsShowing())
			pFamilyFrame->ShowWnd(TRUE);
	}

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyLeaveBroad(tagNS_FamilyLeaveBroad* pNetCmd, DWORD)
{
	//屏显、家族频道广播
	DWORD dwRoleID = pNetCmd->dwSrcRoleID;
	tstring strRole = PlayerNameTab::Inst()->FindNameByID(dwRoleID);

	ShowFamilyMsg( g_StrTable[_T("FamilyEvent_MemberLeft")], strRole.c_str()  );
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("FamilyEvent_MemberLeft")], strRole.c_str() );

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return 0;

	FamilyFrame* pFamilyFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	// 如果是本地玩家离开
	if(pNetCmd->dwSrcRoleID == pLp->GetID())
	{
		ClearFamilyInfo();
		if (P_VALID(pFamilyFrame)&&pFamilyFrame->IsShowing())
			pFamilyFrame->ShowWnd(FALSE);

		m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Family_AllUi"), NULL ) );
	}
	else
	{
		if (P_VALID(pFamilyFrame)&&pFamilyFrame->IsShowing())
			pFamilyFrame->ShowWnd(TRUE);
	}
	

	return 0;
}

DWORD FamilyMgr::OnNS_GetFamilyName(tagNS_GetFamilyName* pNetCmd, DWORD)
{
	// 插入记录
	DWORD	dwFamilyID = pNetCmd->dwFamilyID;
	tstring strFamily;
	if( pNetCmd->szFamilyName == NULL || _tcslen(pNetCmd->szFamilyName) <= 0 )
		return 0;
	else
		strFamily = pNetCmd->szFamilyName;

	//刷新名称或加入记录
	std::map<DWORD, tstring>::iterator it = m_mapFamilyName.find(dwFamilyID);
	if( it != m_mapFamilyName.end())
		it->second = strFamily;
	else
		m_mapFamilyName.insert(make_pair(dwFamilyID,strFamily));
	//已插入，移出请求队列
	if(m_setNameReq.find(dwFamilyID) != m_setNameReq.end())
		m_setNameReq.erase(dwFamilyID);

	//邀请信息 处理
	std::map<DWORD, tagID2Name>::iterator itInivte = m_mapInivteCache.find(dwFamilyID);
	if ( itInivte!=m_mapInivteCache.end() && itInivte->second.dwRoleID != GT_INVALID )
	{
		ShowInvite(dwFamilyID , itInivte->second.dwRoleID,itInivte->second.strName );
		m_mapInivteCache.erase(itInivte);
	}
	 
	//申请信息 处理（废弃）
	//std::map<DWORD, DWORD>::iterator itApp = m_mapApplyCache.begin(); 
	//for ( ; itApp!=m_mapApplyCache.end(); ++itApp )
	//{
	//	if ( itApp->first == dwFamilyID )
	//	{
	//		if ( itApp->second != GT_INVALID )
	//			//ShowApply(itApp->second, );
	//		itApp = m_mapApplyCache.erase(itApp);
	//	}
	//}
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLp) && (dwFamilyID == pLp->GetRoleFamilyID()) )	
	{
		tagFamilyNameEvent evt( _T("FamilyEvent_GetName"), NULL );
		evt.strFamilyName = strFamily;
		m_pFrameMgr->SendEvent( &evt );
	}


	return 0;
}

DWORD FamilyMgr::OnNS_GetFamilyInfo(tagNS_GetFamilyInfo* pNetCmd, DWORD)
{
	INT nNum = (INT)pNetCmd->n8MemberNum;
	if (nNum<=0||nNum>MAX_FAMILY_MEMBER)
		return 0;

	//成员属性
	ClearFamilyMemberMap();
	for ( int i=0; i<nNum; ++i )
	{
		tagRoleFamilyInfo *pInfo = new tagRoleFamilyInfo;
		memcpy( pInfo, &(pNetCmd->sFamilyInfo[i]), sizeof(tagRoleFamilyInfo) );
		//若有记录先删除
		FamilyMemberMap::iterator it = m_mapFamilyInfo.find(pInfo->dwRoleID);
		if ( it!=m_mapFamilyInfo.end() )
		{
			SAFE_DELETE(it->second);
			it = m_mapFamilyInfo.erase(it);
		}	

		m_mapFamilyInfo.insert(make_pair(pInfo->dwRoleID,pInfo));
	}

	//成员索引
	m_mapFamilyMember.clear();

	//恶心的排序
	DWORD dwLeaderID = GT_INVALID;
	map<int, DWORD> mapMarriageMember;
	map<int, DWORD> mapNotmarriageMember;
	map<int, DWORD>::iterator tempIt;
	
	//把族长揪出来
	FamilyMemberMap::iterator iter = m_mapFamilyInfo.begin();
	for ( ; iter!=m_mapFamilyInfo.end(); ++iter )
	{
		tagRoleFamilyInfo *pFamilyInfo = iter->second;
		if ( P_VALID(pFamilyInfo) )
		{
			if ( pFamilyInfo->bLeader )
			{
				dwLeaderID = pFamilyInfo->dwRoleID;
				break;
			}
		}
	}

	//循环查找结婚并且在两个人都在本家族的
	int nMarriageIndex = 0;			//结婚的map索引
	int nNorMarriageIndex = 0;		//没结婚的map索引
	iter = m_mapFamilyInfo.begin();
	for ( ; iter!=m_mapFamilyInfo.end(); ++iter )
	{
		tagRoleFamilyInfo *pFamilyInfo = iter->second;
		if ( P_VALID(pFamilyInfo) )
		{
			if ( !pFamilyInfo->bLeader )	//过滤掉族长
			{
				if(pFamilyInfo->dwLoverID != GT_INVALID && pFamilyInfo->dwLoverID != dwLeaderID)	//不是族长且不是族长配偶
				{
					if(JudgeIsSameFamily(pFamilyInfo->dwLoverID))	//如果夫妻双方在同一家族，加入结婚表中
					{
						map<int, DWORD>::iterator it;
						bool bIsInsert = false;
						for(it = mapMarriageMember.begin(); it != mapMarriageMember.end(); ++it)
						{
							if((*it).second == pFamilyInfo->dwLoverID)
							{
								bIsInsert = true;
							}
						}
						if(!bIsInsert)
							mapMarriageMember.insert(make_pair(nMarriageIndex++, pFamilyInfo->dwRoleID));
					}
					else	//否则视为未婚处理..
					{
						mapNotmarriageMember.insert(make_pair(nNorMarriageIndex++, pFamilyInfo->dwRoleID));
					}
				}
			}
		}
	}

	//添加光棍
	iter = m_mapFamilyInfo.begin();
	for ( ; iter!=m_mapFamilyInfo.end(); ++iter )
	{
		tagRoleFamilyInfo *pFamilyInfo = iter->second;
		if ( P_VALID(pFamilyInfo) )
		{
			if ( !pFamilyInfo->bLeader )	//过滤掉族长
			{
				if(pFamilyInfo->dwLoverID == GT_INVALID)
				{
					mapNotmarriageMember.insert(make_pair(nNorMarriageIndex++, pFamilyInfo->dwRoleID));
				}
			}
		}
	}

	bool bLeaderHasMarriage = false;
	m_mapFamilyMember.insert(make_pair(0,dwLeaderID));
	iter = m_mapFamilyInfo.find(dwLeaderID);
	tagRoleFamilyInfo *pTempFamilyInfo = (*iter).second;

	if(pTempFamilyInfo->dwLoverID != GT_INVALID)	//有配偶了
	{
		if(JudgeIsSameFamily(pTempFamilyInfo->dwLoverID))	//并且在同一个家族
		{
			m_mapFamilyMember.insert(make_pair(1, pTempFamilyInfo->dwLoverID));
			bLeaderHasMarriage = true;
		}
	}

	if(!bLeaderHasMarriage)	//添加第二个位置的角色信息
	{
		tempIt = mapNotmarriageMember.begin();
		if(tempIt != mapNotmarriageMember.end())
		{
			m_mapFamilyMember.insert(make_pair(1, (*tempIt).second));
			mapNotmarriageMember.erase((*tempIt).first);
		}
	}
	
	int nUIIndex = 2;	//从第三个位置开始添加结婚的和没结婚的成员
	//先添加结婚的人
	for(tempIt = mapMarriageMember.begin(); tempIt != mapMarriageMember.end(); ++tempIt)
	{
		iter = m_mapFamilyInfo.find((*tempIt).second);
		tagRoleFamilyInfo *pTempFamilyInfo = (*iter).second;

		if(pTempFamilyInfo->dwLoverID != GT_INVALID)
		{
			m_mapFamilyMember.insert(make_pair(nUIIndex++, pTempFamilyInfo->dwLoverID));
		}
		m_mapFamilyMember.insert(make_pair(nUIIndex++, pTempFamilyInfo->dwRoleID));
	}

	//最后添加光棍
	for(tempIt = mapNotmarriageMember.begin(); tempIt != mapNotmarriageMember.end(); ++tempIt)
	{
		m_mapFamilyMember.insert(make_pair(nUIIndex++, (*tempIt).second));
	}
	// 恶心的排序结束，有种想骂人的感觉...

// 	INT j = 1;//0留给家长
// /*	INT nMemberNum = (INT)m_mapFamilyInfo.size();*/
// 	FamilyMemberMap::iterator iter = m_mapFamilyInfo.begin();
// 	for ( ; iter!=m_mapFamilyInfo.end(); ++iter )
// 	{
// 		tagRoleFamilyInfo *pFamilyInfo = iter->second;
// 		if ( P_VALID(pFamilyInfo) )
// 		{
// 			if ( pFamilyInfo->bLeader )
// 			{
// 				dwLeaderID = pFamilyInfo->dwRoleID;
// 				m_mapFamilyMember.insert(make_pair(0,pFamilyInfo->dwRoleID));
// 			}
// 			else
// 			{
// 				if (j>=MAX_FAMILY_MEMBER)
// 					break;
// 				m_mapFamilyMember.insert(make_pair(j,pFamilyInfo->dwRoleID));
// 				++j;
// 			}
// 		}
// 	}
	
	//这里可以做重排
	//do range();

	//refresh ui
	FamilyFrame* pFrame = (FamilyFrame*)m_pFrameMgr->GetFrame(_T("Family"));
	if( !P_VALID(pFrame) )
		pFrame =  (FamilyFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Family"), _T("FamilyFrame"), 0);
	if (P_VALID(pFrame))
		pFrame->RefreshUI();


	return 0;
}

DWORD FamilyMgr::OnNS_CreateFamilyAllBroad(tagNS_CreateFamilyAllBroad* pCmd, DWORD)
{
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Family_CreateBroad")], pCmd->szRoleName, pCmd->szFamilyName);

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyJoinReqRes(tagNS_FamilyJoinReqRes* pNetCmd, DWORD)
{

	if(E_Success == pNetCmd->dwErrorCode)
	{
		if(pNetCmd->dwInviterID == RoleMgr::Inst()->GetLocalPlayerID())// 邀请者
		{
			
		}
		else if(pNetCmd->dwInviteeID == RoleMgr::Inst()->GetLocalPlayerID())// 被邀请者
		{

		}
	}
	else
	{
		// 显示错误信息
		if ( E_FamilyJoin_TimeOut == pNetCmd->dwErrorCode )
		{
			if(P_VALID(GetObj("FamilyJoinReq")))
				KillObj("FamilyJoinReq");

		} 
		else
		{
			ShowErrorMsg(pNetCmd->dwErrorCode);
		}
		
	}

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyApply(tagNS_FamilyApply* pNetCmd, DWORD)
{
	DWORD dwApplyID		= pNetCmd->dwApplyRoleID;

	tstring strApp	= PlayerNameTab::Inst()->FindNameByID(dwApplyID);

	//tstring strApp = pNetCmd->szApplyRoleName;
	if( !strApp.empty() && dwApplyID != GT_INVALID )
		ShowApply(dwApplyID,strApp);

	return 0;
}

DWORD FamilyMgr::OnNS_FamilyApplyReqRes(tagNS_FamilyApplyReqRes* pNetCmd, DWORD)
{

	if(E_Success == pNetCmd->dwErrorCode)
	{
		
	}
	else
	{
		// 显示错误信息
		if ( E_FamilyApply_TimeOut == pNetCmd->dwErrorCode )
		{
			if(P_VALID(GetObj("FamilyApplyReq")))
				KillObj("FamilyApplyReq");
		}
		else
		{
			ShowErrorMsg(pNetCmd->dwErrorCode);
		}
		
	}

	return 0;
}

VOID FamilyMgr::ShowErrorMsg(DWORD dwErrorCode)
{
	DWORD dwErr = abs((INT)dwErrorCode) ;
	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("Family_ErrorCode_%d"), dwErr);
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
}

VOID  FamilyMgr::KillObj_NPCTalk()
{
	if(P_VALID(GetObj("CreateFamily_Input")))
		KillObj("CreateFamily_Input");

	if(P_VALID(GetObj("CreateFamily")))
		KillObj("CreateFamily");

	if(P_VALID(GetObj("FamilyJoinReq")))
		KillObj("FamilyJoinReq");

	if(P_VALID(GetObj("BreakFamily")))
		KillObj("BreakFamily");

	if(P_VALID(GetObj("FamilyApplyReq")))
		KillObj("FamilyApplyReq");

	if(P_VALID(GetObj("FamilyEvent_PopMsgBox")))
		KillObj("FamilyEvent_PopMsgBox");
}

tstring FamilyMgr::GetFamilyName(DWORD dwFamilyID,ENameUsedType eType/*=ENUT_Nothing*/ ,DWORD dwID /*= GT_INVALID*/, tstring roleName /*= _T("")*/ ,bool bSendCmd /*= true*/)
{
	if ( GT_INVALID == dwFamilyID )
		return _T("");
	
	std::map<DWORD, tstring>::iterator it = m_mapFamilyName.find(dwFamilyID);
	if( it != m_mapFamilyName.end() )
		return it->second;

	if ( ENUT_Nothing != eType && GT_INVALID != dwID )
	{
		if ( ENUT_Invite == eType )
		{
			std::map<DWORD, tagID2Name>::iterator itInivte = m_mapInivteCache.find(dwFamilyID);
			if ( itInivte!=m_mapInivteCache.end() ) //有则替换
				m_mapInivteCache.erase(dwFamilyID);
			tagID2Name sID2Name;
			sID2Name.dwRoleID= dwID;
			if (!roleName.empty())
				sID2Name.strName = roleName;
			else
				sID2Name.strName = PlayerNameTab::Inst()->FindNameByID(dwID);
			m_mapInivteCache.insert(make_pair(dwFamilyID,sID2Name));
		}
		else if ( ENUT_Apply == eType )
		{
			std::map<DWORD, DWORD>::iterator itApp = m_mapApplyCache.find(dwFamilyID);
			if ( itApp!=m_mapApplyCache.end()  ) //有则替换
				itApp->second = dwID;
			else
				m_mapApplyCache.insert(make_pair(dwFamilyID,dwID));
		}
	}

	//发送名字请求消息
	if(bSendCmd)
		SendFamilyNameReq(dwFamilyID);
	return _T("");

}

bool FamilyMgr::JudgeIsSameFamily(DWORD dwRoleID)
{
	FamilyMemberMap::iterator iter = m_mapFamilyInfo.begin();
	for ( ; iter!=m_mapFamilyInfo.end(); ++iter )
	{
		tagRoleFamilyInfo* pMemberInfo = (*iter).second;
		if(pMemberInfo->dwRoleID == dwRoleID)
			return true;
	}
	return false;
}

BOOL  FamilyMgr::ShowInvite(DWORD dwFamilyID,DWORD dwInviterID,tstring strSrcName /*= _T("")*/ )
{
	tstring strInviter		= PlayerNameTab::Inst()->FindNameByID(dwInviterID);
	if (strInviter.empty())
	{
		if (strSrcName.empty())
		{
			std::map<DWORD, tagID2Name>::iterator it = m_mapInivteCache.find(dwFamilyID);
			if ( it != m_mapInivteCache.end() )
				strInviter = it->second.strName;
		}
		else
			strInviter = strSrcName;
	}
	tstring strFamilyName	= GetFamilyName(dwFamilyID,ENUT_Invite,dwInviterID,strInviter);

	if ( strFamilyName.empty() || strInviter.empty() )
		return FALSE;

	TCHAR szTmp[X_LONG_NAME];
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Family_JionReq")], strInviter.c_str(), strFamilyName.c_str());

	if(P_VALID(GetObj("FamilyJoinReq")))
		KillObj("FamilyJoinReq");

	CreateObj("FamilyJoinReq", "MsgBox");
	TObjRef<MsgBox>("FamilyJoinReq")->Init(
		_T(""), szTmp, _T("MsgBox_FamilyJoinReq"), 
		(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE);

	TObjRef<MsgBox>("FamilyJoinReq")->SetDwordUserData(dwFamilyID);
	TObjRef<MsgBox>("FamilyJoinReq")->SetDwordUserData2(dwInviterID);

	return TRUE;
}

BOOL FamilyMgr::ShowApply(DWORD dwApplyID,tstring strAppRoleName)
{
	if ( strAppRoleName.empty() )
		return FALSE;

	TCHAR szTmp[X_LONG_NAME];
	_sntprintf( szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Family_ApplyReq")], strAppRoleName.c_str() );

	if(P_VALID(GetObj("FamilyApplyReq")))
		KillObj("FamilyApplyReq");

	CreateObj("FamilyApplyReq", "MsgBox");
	TObjRef<MsgBox>("FamilyApplyReq")->Init(
		_T(""), szTmp, _T("MsgBox_FamilyApplyReq"), 
		(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE);

	TObjRef<MsgBox>("FamilyApplyReq")->SetDwordUserData(dwApplyID);
	
	return TRUE;
}

void FamilyMgr::ShowFamilyMsg(const TCHAR* szFormat,...)
{
	va_list args;
	va_start(args, szFormat);

	TCHAR szBuffer[X_HUGE_NAME];
	_vstprintf_s(szBuffer, X_HUGE_NAME, szFormat, args);

	va_end(args);

	ChatFrame *pChatFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if (P_VALID(pChatFrame))
	{
		pChatFrame->PushInfo(szBuffer,ESCC_Family);
	}
		
}


VOID FamilyMgr::ClearFamilyInfo()
{
	// 删除数据
	//SAFE_DEL(m_pGuild);
	//SAFE_DEL(m_pLpInfo);
	//SAFE_DEL(m_pMembers);
	
	// 删除界面
	/*m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_GuildAffair"), NULL));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_GuildWare"), NULL));*/

	/*GuildFrame *pFrame = (GuildFrame*)m_pFrameMgr->GetFrame(_T("Guild"));
	if (P_VALID(pFrame))
		m_pFrameMgr->AddToDestroyList(pFrame);*/

	// 设置本地玩家家族状态属性
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		pLp->SetRoleFamilyID(GT_INVALID);
		pLp->SetRoleFamilyLeader(FALSE);
	}

	// 删除仓库相关
	//m_bifNeedInitWarehouse  = true;
	//m_dwWarehouseLastUpTime = 0;
	//m_syncFrameNames.clear();

}
//发送名字请求消息
VOID FamilyMgr::SendFamilyNameReq(DWORD dwID)
{
	if ( m_setNameReq.find(dwID) == m_setNameReq.end() )
	{
		tagNC_GetFamilyName cmd;
		cmd.dwFamilyID = dwID;
		m_pSession->Send(&cmd);

		m_setNameReq.insert(dwID);
	}	

}

//家族信息请求
VOID FamilyMgr::SendFamilyInfoReq(DWORD dwID)
{
	tagNC_GetFamilyInfo cmd;
	cmd.dwFamilyID = dwID;
	m_pSession->Send(&cmd);
}

VOID FamilyMgr::ClearFamilyMemberMap()
{
	FamilyMemberMap::iterator it = m_mapFamilyInfo.begin();
	for ( ; it!=m_mapFamilyInfo.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapFamilyInfo.clear();
}

const tagRoleFamilyInfo* FamilyMgr::GetMemberByID(DWORD dwID)
{
	FamilyMemberMap::iterator it = m_mapFamilyInfo.find(dwID);
	if ( it != m_mapFamilyInfo.end() )
	{
		return (it->second);
	}
	return NULL;
}

//查找族长ID
DWORD FamilyMgr::GetFamilyLeaderID()
{
	if (m_mapFamilyMember.empty())
		return GT_INVALID;

	std::map<DWORD, DWORD>::iterator it = m_mapFamilyMember.find(0);
	if (it!=m_mapFamilyMember.end())
	{
		return it->second;
	}
	return GT_INVALID; 
}

//----------家族任务--------------------------------------------------------------
DWORD FamilyMgr::OnNS_OpenFamilyQuestPage(tagNS_OpenFamilyQuestPage* pCmd, DWORD)
{
	//清空原先属性
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].QuestID = GT_INVALID;
		m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].dwCurDegree = 0;
	}

	//保存昨天属性
	m_pFamilyQuestInfo.m_dwYesterdayDegree  = pCmd->nProcess;
	m_pFamilyQuestInfo.m_bIsGetReward		= pCmd->bIsReceive;

	//保存当前属性
	for(int i = 0; i < pCmd->byQuestNum; i++)
	{
		m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].dwCurDegree = pCmd->Quests[i].nTargetCount;
		m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].QuestID = pCmd->Quests[i].dwQuestID;
	}

	//刷新家族任务
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetFamilyQuestInfo"), NULL));

	return 0;
}

BOOL FamilyMgr::IsFamilyQuestMonster(DWORD dwID)
{
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		//首先判断id
		if(m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].QuestID == 0 || m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].QuestID == GT_INVALID)
			continue;

		const tagFamilyQuestName* pQuestProto = FamilyProtoData::Inst()->FindQuestName(m_pFamilyQuestInfo.m_sFamilyQuestInfo[i].QuestID);

		if(!P_VALID(pQuestProto))
			continue;

		const tagFamilyQuestAttProto* pQuestInfo = FamilyProtoData::Inst()->FindQuestAttProto(pQuestProto->dwConditionID);
		if(!P_VALID(pQuestInfo))
			continue;

		if(pQuestInfo->eFinishType != EQFT_Monster)
			continue;

		for(int i = 0; i < MAX_ItemOrMonsterID; i++)
		{
			if(pQuestInfo->dwItemOrMonsterID[i] == GT_INVALID || pQuestInfo->dwItemOrMonsterID[i] == 0)
				break;

			if(dwID == pQuestInfo->dwItemOrMonsterID[i])
			{
				return TRUE;
			}
		}

		//判断条件
		if(MapMgr::Inst()->GetCurMapID() == pQuestInfo->dwMapcrc || pQuestInfo->dwMapcrc == 0)
		{
			const tagCreatureProto* pProto = CreatureData::Inst()->FindNpcAtt(dwID);
			if(P_VALID(pProto))
			{
				if(pProto->nLevel >= pQuestInfo->nLowLevel && pProto->nLevel <= pQuestInfo->nHighLevel)
				{
					if(pProto->eLite == pQuestInfo->nMonsterRank)
					{
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

DWORD FamilyMgr::OnNS_FamilySpritTrainBroad(tagNS_FamilySpritTrainBroad* pCmd, DWORD)
{
	/*TCHAR buffer[X_LONG_NAME];
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("FamilySpriteAbility_%d"), pCmd->dwType);*/
	tstringstream tss;
	tss<<_T("FamilySpriteAbility_")<<(INT)pCmd->dwType;
	// 显示聊天信息
	ShowFamilyMsg(g_StrTable[_T("Family_SpritTrainBroad")], pCmd->tszRoleName,g_StrTable[tss.str().c_str()],pCmd->nRet);
	return 0;
}
