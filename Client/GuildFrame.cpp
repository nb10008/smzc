#include "StdAfx.h"
#include "GuildFrame.h"
#include "GuildMgr.h"
#include "GuildMembers.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\currency_define.h"
#include "PlayerNameTab.h"
#include "GuildEvent.h"
#include "FilterData.h"
#include "CombatSysUtil.h"
#include "GuildMemberPosFrame.h"
#include "RoleMgr.h"
#include "ServerTime.h"
#include <new>
#include "LocalPlayer.h"
#include "IconStatic.h"
#include "SkillMgr.h"
#include "GuildDefine.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CurrencyMgr.h"
#include "ProfessionMgr.h"
#include "SkillProtoData.h"
#include "Item.h"
#include "ItemMgr.h"
#include "CreatureData.h"
#include "..\WorldDefine\guild_define.h"
#include "ChatEvent.h"
#include "GuildApplyManageFrame.h"
#include "GlintButton.h"
#include "..\WorldDefine\guild_war_declare_define.h"
#include "..\WorldDefine\msg_guild_war_declare.h"
#include "CombatSys.h"
#include "MsgBox.h"
#include "ItemButton.h"
#include "..\WorldDefine\time.h"
#include "ServerTime.h"

const DWORD RedColour = 0xFFFF402D;
const DWORD DefaultColour = 0xFFFFFFFF;
const DWORD BlueColour = 0xFF00A2FF;
const DWORD GreenColour = 0xFF00FF00;
const DWORD YellowColour = 0xFFFFAE00;
const DWORD GrayColour = 0xFF808080;
const int GuildFacilityMaxLevel = 5;

// 工会战每一页显示数量
const int GUILD_APPLY_MAX_ITEMS_PER_PAGE_2 = 19;

GuildFrame::GuildFrame(void):m_Trunk(this)
{
	m_eSortType		 = GuildMembers::ELCT_Level;
	m_pWnd			 = NULL;
	m_pWndMembers	 = NULL;
	m_pBtnClose		 = NULL;
	m_pLstMembers	 = NULL;
	m_pStcName		 = NULL;	
	m_pStcLevel		 = NULL;
	m_pStcSymbol	 = NULL;
	m_pStcFounder	 = NULL;
	m_pStcLeader	 = NULL;
	m_pStcMemberNum	 = NULL;
	m_pStcReputation = NULL;

	m_pStcFund		 = NULL;		
	m_pStcMaterial	 = NULL;	
	m_pStcPeace		 = NULL;	
	m_pStcDailyCost	 = NULL;
	m_pEdtTenet		 = NULL;	
	m_pPbnMembers	 = NULL;	

	m_pBtnLookupInfo	= NULL;
	m_pBtnLeaveGuild	= NULL;
	m_pBtnTurnLeader	= NULL;
	m_pBtnAppointMember = NULL;
	m_pBtnManageGuild	= NULL;	
	m_pBtnKickMember	= NULL;	
	m_pBtnDemissPostion = NULL;
	m_pPbnOffline		= NULL;	

	m_pGuild			= NULL;
	m_dwDstRoleID		= GT_INVALID;

	m_pBtnListIndex.clear();
	m_pWndGuild = NULL;
	m_pBtnCurSkillIcon = NULL;
	m_pStcCurSkillName = NULL;
	m_pStcCurSkillLevel = NULL;
	m_pStcFundCost = NULL;
	m_pStcMaterialCost = NULL;
	m_pBtnBookIcon = NULL;
	m_pProgressIncrement = NULL;
	m_pLearnBook = NULL;
	m_pBtnBack = NULL;
	m_pBtnNext = NULL;
	m_pStcLearnGoldCost = NULL;
	m_pStcLearnSilverCost = NULL;
	m_pStcLearnContributionNeed = NULL;
	m_pStcLearnGuildFundCost = NULL;
	m_pStcLearnGuildMaterialCost = NULL;
	m_pBtnLearnSkill = NULL;
	m_pBtnCancelResearch = NULL;
	m_pBtnStartResearch = NULL;
	m_pSkillPatch = NULL;

	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		m_pSkillItemBack[i] = NULL;
		m_pSkillItem[i] = NULL;
		m_pSkillName[i] = NULL;
		m_pSkillLevel[i] = NULL;
	}
	
	m_pRemnantStatePatch = NULL;
	m_pLstClanState = NULL;
	m_pBtnApply = NULL;
	m_nCurChooseRenmant = 0;

	m_nCurPageNum = 0;
	m_dwCurChooseSkill = GT_INVALID;
	m_eGuildFrameState = EGFS_MemberList;
	m_bShow = false;
	m_pSkillBook = NULL;
	

	m_dwTempRoleID = GT_INVALID;
	m_nTempPOS = 0;
	m_pStcAttackGuildName = NULL;
	m_pStcAttackGuildLv = NULL;
	m_pStcAttackGuildMember = NULL;

	m_pStcDefenceGuildName = NULL;
	m_pStcDefenceGuildLv = NULL;
	m_pStcDefenceGuildMember = NULL;
	
	m_pEdtMoney = NULL;

	m_bIsHaveGuildApply = FALSE;

 	m_nMaxPage = 0;
 	m_nCurPage = 0;

	m_nCurAlly = 0;
	m_nCurAllyID = GT_INVALID;
	m_strSearchName.assign( _T("") );

	m_bNeedRefreshAlliance = false;
	needRefresh = false;

}

GuildFrame::~GuildFrame(void)
{
}

BOOL GuildFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	// 注册游戏事件处理函数
	m_pMgr->RegisterEventHandle(_T("GuildEvent_NameGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildName));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_TenetGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildTenet));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetRoleName));
	m_pMgr->RegisterEventHandle(_T("GetNameByNameID"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetNameByNameID));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_ReflashReq"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReq));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_PosAppoint"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildPosAppoint));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_ListReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildListReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_InfoReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_PosChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_RemoveMember"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->RegisterEventHandle(_T("MsgBox_ModifyTenet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxInputTenet));
	m_pMgr->RegisterEventHandle(_T("MsgBox_LeaveCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxLeaveCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_TurnoverCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxTurnoverCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_KickCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxKickCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_DemissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxDemissCheck));
	m_pMgr->RegisterEventHandle(_T("Refresh_GuildSkills"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuildSkills));
	m_pMgr->RegisterEventHandle(_T("ShowGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowGuildFrame));
	m_pMgr->RegisterEventHandle(_T("HideGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventHideGuildFrame));
	m_pMgr->RegisterEventHandle(_T("Refresh_Guild"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuild));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_UseGuildSkillBook"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventUsGuildSkillBook));
	m_pMgr->RegisterEventHandle(_T("Refresh_Book"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshBook));
	m_pMgr->RegisterEventHandle(_T("OnEventCommerceWarningMsgBox"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventCommerceWarningMsgBox));
	m_pMgr->RegisterEventHandle(_T("Refresh_Clan_Remnant"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshClanRemnantState));
	m_pMgr->RegisterEventHandle(_T("GodStateApplyEvent"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApply));
	m_pMgr->RegisterEventHandle(_T("GodStateAddMoney"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventAddMoney));
	m_pMgr->RegisterEventHandle(_T("ShowRemnant"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowRemnant));
	m_pMgr->RegisterEventHandle(_T("tagKickGuildMemberF"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventKickGuildMember));
	m_pMgr->RegisterEventHandle(_T("Open_GuildAllianceList"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventOpenGuildAllianceList));
	m_pMgr->RegisterEventHandle(_T("MsgBox_WarCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxWarCheck));
	m_pMgr->RegisterEventHandle(_T("BattleApplyAttackAlly"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApplyAttackAlly));
	m_pMgr->RegisterEventHandle(_T("BattleApplyDefenceAlly"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApplyDefenceAlly));
	m_pMgr->RegisterEventHandle(_T("MoneyConfirm_Apply"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMoneyConfirmApply));
	m_pMgr->RegisterEventHandle(_T("GuildEvent_PosAppoint_HuiZhang"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventTurnOverHuiZhang));
	
	SetSkillDefaultUI();
	m_pPbnMembers->SetState(EGUIBS_PushDown, TRUE);

	GuildMgr::Inst()->SendGuildAllMembersGet();
	GuildMgr::Inst()->SendGetGuildSkillInfo();
	m_pSkillPatch->SetInvisible(TRUE);

	m_pWndAlliance->SetTopMost(TRUE);
	m_pWndActiveSkill->SetTopMost(TRUE);

	m_pCmdMgr->Register( "NS_GuildApplierBill", (NETMSGPROC)m_Trunk.sfp2( &GuildFrame::OnNS_GuildApplierBill), _T("NS_GuildApplierBill") );
 	m_pCmdMgr->Register("NS_GetAllGuildSmallInfo", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GetAllGuildSmallInfo), _T("NS_GetAllGuildSmallInfo") );
 	m_pCmdMgr->Register("NS_SearchGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_SearchGuild), _T("NS_SearchGuild") );

	m_pCmdMgr->Register("NS_DeclareGuildWar", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_DeclareGuildWar), _T("NS_DeclareGuildWar") );
	m_pCmdMgr->Register("NS_GuildWarEnd", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GuildWarEnd), _T("NS_GuildWarEnd") );
	m_pCmdMgr->Register("NS_SyncWarStateGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_SyncWarStateGuild), _T("NS_SyncWarStateGuild") );

	m_pCmdMgr->Register("NS_GetGuildUpgradeInfo",	(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GetGuildUpgradeInfo),	_T("NS_GuildUpgradeFrame"));
	m_pCmdMgr->Register("NS_UpdateFacilitiesInfo",	(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_UpdateFacilitiesInfo),_T("NS_UpdateFacilitiesInfo"));
	m_pCmdMgr->Register("NS_ActiveGuildSkill",	(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_ActiveGuildSkill),_T("NS_ActiveGuildSkill"));
	const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
	if( P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tagNC_GetGuildApplierBill cmd;
		m_pSession->Send( &cmd );
	}

	//界面初始化完成先，显示出来，然后发送信息获取帮派信息
	//tagNC_GetAllGuildSmallInfo cmd;
	//m_pSession->Send( &cmd );

	tagNC_GetGuildUpgradeInfo cmd2;
	cmd2.dwNPCID = GT_INVALID;
	m_pSession->Send(&cmd2);

	if (!m_pSession->IsEnableGuildBattle())
		m_pPbnWar->SetEnable(FALSE);

	return bRet;
}

BOOL GuildFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	// 注销游戏事件处理函数
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_NameGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildName));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_TenetGet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetGuildTenet));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetRoleName));
	m_pMgr->UnRegisterEventHandler(_T("GetNameByNameID"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGetNameByNameID));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_ReflashReq"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReq));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_PosAppoint"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildPosAppoint));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_ListReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildListReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_InfoReflash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_PosChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_RemoveMember"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventGuildInfoReflash));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_ModifyTenet"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxInputTenet));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_LeaveCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxLeaveCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_TurnoverCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxTurnoverCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_KickCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxKickCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_DemissCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxDemissCheck));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_GuildSkills"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuildSkills));
	m_pMgr->UnRegisterEventHandler(_T("ShowGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowGuildFrame));
	m_pMgr->UnRegisterEventHandler(_T("HideGuildFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventHideGuildFrame));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Guild"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshGuild));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_UseGuildSkillBook"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventUsGuildSkillBook));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Book"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshBook));
	m_pMgr->UnRegisterEventHandler(_T("OnEventCommerceWarningMsgBox"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventCommerceWarningMsgBox));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Clan_Remnant"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventRefreshClanRemnantState));
	m_pMgr->UnRegisterEventHandler(_T("GodStateApplyEvent"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApply));
	m_pMgr->UnRegisterEventHandler(_T("GodStateAddMoney"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventAddMoney));
	m_pMgr->UnRegisterEventHandler(_T("ShowRemnant"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventShowRemnant));
	m_pMgr->UnRegisterEventHandler(_T("tagKickGuildMemberF"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventKickGuildMember));
	m_pMgr->UnRegisterEventHandler(_T("Open_GuildAllianceList"),              (FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventOpenGuildAllianceList));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_WarCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMsgBoxWarCheck));
	m_pMgr->UnRegisterEventHandler(_T("MoneyConfirm_Apply"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventMoneyConfirmApply));

	m_pMgr->UnRegisterEventHandler(_T("BattleApplyAttackAlly"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApplyAttackAlly));
	m_pMgr->UnRegisterEventHandler(_T("BattleApplyDefenceAlly"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventApplyDefenceAlly));
	m_pMgr->UnRegisterEventHandler(_T("GuildEvent_PosAppoint_HuiZhang"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GuildFrame::OnEventTurnOverHuiZhang));

	m_pCmdMgr->UnRegister("NS_GuildApplierBill", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GuildApplierBill) );
 	m_pCmdMgr->UnRegister("NS_GetAllGuildSmallInfo", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GetAllGuildSmallInfo) );
 	m_pCmdMgr->UnRegister("NS_SearchGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_SearchGuild) );

	m_pCmdMgr->UnRegister("NS_DeclareGuildWar", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_DeclareGuildWar) );
	m_pCmdMgr->UnRegister("NS_GuildWarEnd", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GuildWarEnd) );
	m_pCmdMgr->UnRegister("NS_SyncWarStateGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_SyncWarStateGuild) );
	
	m_pCmdMgr->UnRegister("NS_GetGuildUpgradeInfo",		(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_GetGuildUpgradeInfo));
	m_pCmdMgr->UnRegister("NS_UpdateFacilitiesInfo",	(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_UpdateFacilitiesInfo));
	m_pCmdMgr->UnRegister("NS_ActiveGuildSkill",	(NETMSGPROC)m_Trunk.sfp2(&GuildFrame::OnNS_ActiveGuildSkill));

	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	if(P_VALID(m_pWndSearch))
	{
		m_pGUI->AddToDestroyList(m_pWndSearch);
		m_pWndSearch = NULL;
	}

	if(P_VALID(m_pWndtenet))
	{
		m_pGUI->AddToDestroyList(m_pWndtenet);
		m_pWndtenet = NULL;
	}

	if(P_VALID(m_pWndAlliance))
	{
		m_pGUI->AddToDestroyList(m_pWndAlliance);
		m_pWndAlliance = NULL;
	}

	if(P_VALID(m_pWndActiveSkill))
	{
		m_pGUI->AddToDestroyList(m_pWndActiveSkill);
		m_pWndActiveSkill = NULL;
	}
	
	DestroyMsgBox();
	
	
	return bRet;
}

BOOL GuildFrame::ReloadUI()
{
	BOOL bRet = GameFrame::ReloadUI();
	
	// 创建
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\party.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	
	for (int i = 1; i <= 5; ++i)
	{
		char buffer[512];
		_snprintf(buffer, 512, "party_win\\party_back\\message_back\\basicmessage_back2\\static%d", i);
		m_pGUI->ChangeXml(&ele, buffer, "ClassName", "IconStatic");
	}

	for (int i = 0; i < 12; ++i)
	{
		std::string strItemPath = "party_win\\party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d";
		char buffer[512];
		_snprintf(buffer, 512, strItemPath.c_str(), i, i, i);
		m_pGUI->ChangeXml(&ele, buffer, "ClassName", "IconStatic");
	}

	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\guildskill_back\\doingskillback_pic\\skillback\\skill", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\guildskill_back\\doingskillback_pic1\\skillback\\skill", "ClassName", "IconStatic");
	//m_pGUI->ChangeXml(&ele, "party_win\\party_back\\message_back\\basicmessage_back\\partylogo_back\\party_logo", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\message_back\\basicmessage_back4\\verifybutt","ClassName","GlintButton");   //修改为支持闪烁的按钮
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\partymember_back\\partybutt3","ClassName","GlintButton");   //修改为支持闪烁的按钮
	m_pGUI->ChangeXml(&ele, "party_win\\party_back\\guildskill_back\\doingskillback_pic1\\skillback\\skill","ClassName","ItemButton");  
	
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	//m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GuildFrame::EventHandler));
	
	m_pBtnClose  = (GUIButton*)m_pWnd->GetChild(_T("party_back\\closebutt"));
	m_pStcName	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\party_name"));
	m_pStcLevel	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\party_grade"));
	//m_pStcSymbol = (IconStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back\\partylogo_back\\party_logo"));
	
	m_pStcFounder	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\name"));
	m_pStcLeader	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\leader"));
	m_pStcMemberNum	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\num1"));
	m_pStcReputation = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back2\\num2"));

	m_pStcFund		 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num1"));		
	m_pStcMaterial	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num2"));	
	//m_pStcPeace		 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num3"));	
	m_pStcDailyCost	 = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back3\\num3"));

	m_pBtnZhongzhi   = (GUIButton*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back4\\verifybutt"));

	m_pEdtTenet		 = (GUIEditBoxEx*)m_pWnd->GetChild(_T("party_back\\message_back\\basicmessage_back4\\purpose"));
	
	m_pPbnMembers = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\memberlist"));
	m_pPbnSkills = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\guildskill"));
	m_pPbnInfomation = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\battlefield"));
	m_pPbnWar = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\guildwar"));
	m_pPbnConstruction = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\fra\\guildconstruction"));

	m_pWndMembers = m_pWnd->GetChild(_T("party_back\\partymember_back"));
	m_pLstMembers = (GUIListBox*)m_pWndMembers->GetChild(_T("memberlist_back\\member_list"));
	for(INT i=0; i<GuildMembers::ELCT_End; ++i)
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("memberlist_back\\butt%d"), i+1);
		GUIButton* pBtn = (GUIButton*)m_pWndMembers->GetChild(szTmp);
		m_pBtnListIndex.push_back(pBtn);
	}
	m_pBtnLookupInfo	= (GUIButton*)m_pWnd->GetChild(_T("party_back\\partymember_back\\partybutt4"));
	m_pBtnLeaveGuild	= (GUIButton*)m_pWnd->GetChild(_T("party_back\\partymember_back\\partybutt2"));
	//m_pBtnTurnLeader	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt3"));
	//m_pBtnAppointMember = (GUIButton*)m_pWndMembers->GetChild(_T("partybutt4"));
	m_pBtnManageGuild	= (GlintButton*)m_pWnd->GetChild(_T("party_back\\partymember_back\\partybutt3"));	
	//m_pBtnKickMember	= (GUIButton*)m_pWndMembers->GetChild(_T("partybutt50"));	
	m_pBtnDemissPostion = (GUIButton*)m_pWnd->GetChild(_T("party_back\\partymember_back\\partybutt1"));


	m_pPbnOffline		= (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\partymember_back\\word1\\optbutt"));

	m_pLstMembers->SetColNum(7, 84);
	m_pLstMembers->SetColWidth(0, 130);
	m_pLstMembers->SetColWidth(1, 74);
	m_pLstMembers->SetColWidth(2, 58);
	m_pLstMembers->SetColWidth(3, 58);
	m_pLstMembers->SetColWidth(6, 112);

	m_pGuildMemberPatch = (GUIPatch*)m_pWnd->GetChild(_T("party_back\\partymember_back"));
	m_pGuildMemberListBox = (GUIListBox*)m_pWnd->GetChild(_T("party_back\\partymember_back\\memberlist_back\\member_list"));
	
	m_pWndGuild = m_pWnd;
	m_pSkillPatch = (GUIPatch*)m_pWnd->GetChild(_T("party_back\\guildskill_back"));
	m_pBtnCurSkillIcon = (IconStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillback\\skill"));
	m_pStcCurSkillName = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillname"));
	m_pStcCurSkillLevel = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\skillgrade"));
	m_pStcFundCost = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\num1"));
	m_pStcMaterialCost = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\num2"));
	m_pProResearchProgress = (GUIProgress*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic\\progressbarback\\progressbar"));
	m_pBtnBookIcon = (ItemButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\skillback\\skill"));
	m_pProgressIncrement = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\num5"));
	m_pLearnBook = (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic1\\researchbutt"));
	m_pBtnBack = (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic2\\backbutt"));
	m_pBtnNext = (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\doingskillback_pic2\\nextbutt"));
	m_pStcLearnGoldCost = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\goldnum"));
	m_pStcLearnSilverCost = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\silvernum"));
	m_pStcLearnContributionNeed = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num4"));
	//m_pStcLearnGuildFundCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num3"));
	//m_pStcLearnGuildMaterialCost = (GUIStatic*)m_pWndGuild->GetChild(_T("party_back\\guildskill_back\\needingback_pic\\num5"));

	m_pBtnActiveResearch	= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\gskillbutt4"));
	m_pBtnLearnSkill		= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\gskillbutt7"));
	m_pBtnCancelResearch	= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\gskillbutt6"));
	m_pBtnStartResearch		= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildskill_back\\gskillbutt5"));
	
	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		tstring strItemBackPath = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d");
		TCHAR buffer[512];
		_sntprintf(buffer, 512, strItemBackPath.c_str(), i, i, i);
		m_pSkillItemBack[i] = (IconStatic*)m_pWnd->GetChild(buffer);

		tstring strItemPath = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d");
		_sntprintf(buffer, 512, strItemPath.c_str(), i, i, i);
		m_pSkillItem[i] = (IconStatic*)m_pWnd->GetChild(buffer);

		tstring strItemNamePaht = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillname%d");
		_sntprintf(buffer, 512, strItemNamePaht.c_str(), i, i);
		m_pSkillName[i] = (GUIStatic*)m_pWnd->GetChild(buffer);

		tstring strItemLvPaht = _T("party_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillgrade%d");
		_sntprintf(buffer, 512, strItemLvPaht.c_str(), i, i);
		m_pSkillLevel[i] = (GUIStatic*)m_pWnd->GetChild(buffer);
	}
	
	for (int i = 1; i <= 5; ++i)
	{
		TCHAR buffer[512];
		_sntprintf(buffer, 512, _T("party_back\\message_back\\basicmessage_back2\\static%d"), i);
		m_pIstState[i - 1] = (IconStatic*)m_pWnd->GetChild(buffer);
	}
	
	//  [7/12/2010 zhangsu]工会设施
	m_pGuildConstruction = (GUIPatch*)m_pWnd->GetChild(_T("party_back\\guildconstruction"));

	
	m_pStcConstrLevel[EFT_Guild]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\num"));
	m_pStcConstrGold[EFT_Guild]		= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\goldnum"));
	m_pStcConstrSilver[EFT_Guild]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\silvernum"));
	m_pStcConstrResource[EFT_Guild] = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\resourcemun"));
	m_pProConstResearch[EFT_Guild]	= (GUIProgress*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\progressbarback\\progressbar"));
	m_pBtnLevelUp[EFT_Guild]			= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\party_upgrade\\upgrade"));

	m_pStcConstrLevel[EFT_Fund]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\num"));
	m_pStcConstrGold[EFT_Fund]		= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\goldnum"));
	m_pStcConstrSilver[EFT_Fund]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\silvernum"));
	m_pStcConstrResource[EFT_Fund] = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\resourcemun"));
	m_pProConstResearch[EFT_Fund]	= (GUIProgress*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\progressbarback\\progressbar"));
	m_pBtnLevelUp[EFT_Fund]			= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\gold_upgrade\\upgrade"));

	m_pStcConstrLevel[EFT_Material]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\num"));
	m_pStcConstrGold[EFT_Material]		= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\goldnum"));
	m_pStcConstrSilver[EFT_Material]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\silvernum"));
	m_pStcConstrResource[EFT_Material] = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\resourcemun"));
	m_pProConstResearch[EFT_Material]	= (GUIProgress*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\progressbarback\\progressbar"));
	m_pBtnLevelUp[EFT_Material]			= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\resource_upgrade\\upgrade"));

	m_pStcConstrLevel[EFT_Academy]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\num"));
	m_pStcConstrGold[EFT_Academy]		= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\goldnum"));
	m_pStcConstrSilver[EFT_Academy]	= (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\silvernum"));
	m_pStcConstrResource[EFT_Academy] = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\resourcemun"));
	m_pProConstResearch[EFT_Academy]	= (GUIProgress*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\progressbarback\\progressbar"));
	m_pBtnLevelUp[EFT_Academy]			= (GUIButton*)m_pWnd->GetChild(_T("party_back\\guildconstruction\\magic_upgrade\\upgrade"));

	//  [7/12/2010 zhangsu] 工会宣战

	m_pGuildWar= (GUIPatch*)m_pWnd->GetChild(_T("party_back\\guildwar"));

	m_pBtnSearch = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\guildwar\\searchbutt"));
	m_pBtnPartyWarStart = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\guildwar\\warbutt"));
	m_pBtnPartyWarCancel = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\guildwar\\canclebutt"));

	m_pBtnPageBack = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\guildwar\\backbutt"));
	m_pBtnPageFront = (GUIPushButton*)m_pWnd->GetChild(_T("party_back\\guildwar\\nextbutt"));

	m_pStcPageNum = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildwar\\num"));

	m_pLstGuilds = (GUIListBox*)m_pWnd->GetChild(_T("party_back\\guildwar\\listback0\\member_list"));

	m_pStcWarTimes = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\guildwar\\word\\num"));
	// 神迹状态分页
	m_pRemnantStatePatch = (GUIPatch*)m_pWnd->GetChild(_T("party_back\\battlefieldstate"));
														   
 	m_pLstClanState = (GUIListBox*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic\\list1"));
 
 	m_pLstClanState->SetColNum(4, 80);
 	m_pLstClanState->SetColWidth(0, 150);
 	m_pLstClanState->SetColWidth(1, 140);
 	m_pLstClanState->SetColWidth(2, 100);
	m_pLstClanState->SetColWidth(3, 166);
	
	m_pEdtMoney = (GUIEditBox*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic3\\putinpic\\putin"));
	m_pEdtMoney->SetMaxValue(9999999);
	m_pBtnApply = (GUIButton*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic3\\assignbutt"));
	m_pBtnBackMoney = (GUIButton*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic3\\addbutt"));
	m_pStcNowBegMoney = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic3\\price\\num"));	
	
	m_pStcBattleDesc = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic\\word3"));	
	m_pStcBattleTime = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic\\word3\\time"));

	m_pBtnApplyAttackAlly = (GUIButton*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic2\\assignbutt"));
	m_pBtnApplyDefenceAlly = (GUIButton*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic1\\assignbutt"));
	confirmAllys = (GUIButton*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\surebutt"));
	
	m_pStcAttackGuildName = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic2\\word4"));	
	m_pStcAttackGuildLv = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic2\\num"));
	m_pStcAttackGuildMember = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic2\\num1"));;	
	//m_pStcAttackMoney = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\godstate_pic\\listpic1\\num2"));		

	m_pStcDefenceGuildName = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic1\\word4"));	
	m_pStcDefenceGuildLv = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic1\\num"));	
	m_pStcDefenceGuildMember = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\battlefieldstate\\listpic1\\num1"));
	//m_pStcDefenceDay = (GUIStatic*)m_pWnd->GetChild(_T("party_back\\godstate_pic\\listpic2\\num2"));		
	
	for ( int i = 0; i < GUILD_WAR_MAXALLY_NUM; i++)
	{
		TCHAR buffer[512];
		tstring strNamePaht = _T("party_back\\battlefieldstate\\listpic1\\num%d");
		_sntprintf(buffer, 512, strNamePaht.c_str(), i+2);
		m_pStcDefenceAllies[i] = (GUIStatic*)m_pWnd->GetChild(buffer);

		strNamePaht.clear();
		strNamePaht = _T("party_back\\battlefieldstate\\listpic2\\num%d");
		_sntprintf(buffer, 512, strNamePaht.c_str(), i+2);
		m_pStcAttackAllies[i]=	(GUIStatic*)m_pWnd->GetChild(buffer);

	}
	

	//  [7/13/2010 zhangsu]
	strPath = vEngine::g_strLocalPath + _T("\\ui\\guild_search.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndSearch = m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	m_pGUI->RegisterEventHandler( m_pWndSearch->GetFullName().c_str(), m_Trunk.sfp1(&GuildFrame::EventHandler) );

	m_pWndInput			= (GUIEditBox*)m_pWndSearch->GetChild( _T("searchgroup_pic\\putinpic\\putin") );
	m_pBtnSearchOk		= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\surebutt") );
	m_pBtnSearchCancel	= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\canclebutt") );
	m_pBtnSearchClose	= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\closebutt") );
	m_pPbtnGuild		= (GUIPushButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\groupback\\optbutt") );
	m_pPbtnLeader		= (GUIPushButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\groupback\\optbutt1") );
	m_pWndSearch->SetInvisible( TRUE );


	strPath = vEngine::g_strLocalPath + _T("\\ui\\party_tenet.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml(&ele, "partytenet_win\\partytenet_pic\\back\\win\\word","ClassName","GUIEditBoxEx"); 
	m_pWndtenet = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	m_pEditBoxTenet = (GUIEditBoxEx*)m_pWndtenet->GetChild( _T("partytenet_pic\\pic\\editbox") );
	m_pBtnModifyTenet = (GUIButton*)m_pWndtenet->GetChild( _T("partytenet_pic\\surebutt") );
	m_pBtnCancelTenet = (GUIButton*)m_pWndtenet->GetChild( _T("partytenet_pic\\canclebutt") );
	m_pBtnCloseTenet = (GUIButton*)m_pWndtenet->GetChild( _T("partytenet_pic\\closebutton") );

	m_pWndtenet->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWndtenet->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GuildFrame::EventHandler));

	//  [7/20/2010 zhangsu]
	strPath = vEngine::g_strLocalPath + _T("\\ui\\battlefield_yes.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndAlliance = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	m_pLstAlliance			=	(GUIListBox*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\back\\listpic\\list") );
	m_pBtnAllianceOk		=	(GUIButton*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\back\\butt1") );
	m_pBtnAllianceCancel	=	(GUIButton*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\back\\butt2") );
	m_pStcNowAllies			=	(GUIStatic*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\back\\word0\\num") );
	m_pStcRemainAllies		=	(GUIStatic*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\back\\word1\\num") );
	m_pBtnAllianceXclose    =	(GUIButton*)m_pWndAlliance->GetChild( _T("battlefield_yes_pic\\close") );

	m_pLstAlliance->SetColNum(3, 80);
	m_pLstAlliance->SetColWidth(0, 150);
	m_pLstAlliance->SetColWidth(1, 80);
	m_pLstAlliance->SetColWidth(2, 100);

	m_pGUI->RegisterEventHandler( m_pWndAlliance->GetFullName().c_str(), m_Trunk.sfp1(&GuildFrame::EventHandler) );

	//  [7/28/2010 zhangsu]
	strPath = vEngine::g_strLocalPath + _T("\\ui\\team.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml(&ele, "team_win\\team_pic\\fairy_back\\fairy_pic", "ClassName", "ItemButton");
	m_pWndActiveSkill = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	m_pBnActive	=	(GUIButton*)m_pWndActiveSkill->GetChild( _T("team_pic\\surebutt") );
	m_pIbnActiveItem	=	(ItemButton*)m_pWndActiveSkill->GetChild( _T("team_pic\\fairy_back\\fairy_pic") );
	m_pBnActiveClose = (GUIButton*)m_pWndActiveSkill->GetChild( _T("team_pic\\closebutt") );

	m_pGUI->RegisterEventHandler( m_pWndActiveSkill->GetFullName().c_str(), m_Trunk.sfp1(&GuildFrame::EventHandler) );
	tstring strTenets = GuildMgr::Inst()->GetGuildTenet();
	m_pEditBoxTenet->SetText( strTenets.c_str() );
	m_pEditBoxTenet->SetRefresh( TRUE );

	m_pSkillPatch->SetInvisible(FALSE);
	m_pLearnBook->SetEnable(false);
	m_pPbnInfomation->SetEnable(true);//1.3.2暂时屏蔽

	m_pLstGuilds->SetColNum( 1, 73 );
	m_pLstGuilds->SetColNum( 5, 73 );
	m_pLstGuilds->SetColWidth( 0, 200 );
	m_pLstGuilds->SetColWidth( 1, 100 );
	m_pLstGuilds->SetColWidth( 2, 100 );
	m_pLstGuilds->SetColWidth( 3, 120 );
	m_pLstGuilds->SetColWidth( 4, 80 );

	// 获取帮派信息
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	if(P_VALID(m_pGuild))
	{	
		GuildMgr::Inst()->SendGuildAllMembersGet();
		// 刷新界面
		ReflashGuild();
		// 刷新按钮
		ReflashButton();
		// 获取成员信息
		
		// 刷新列表
		//ReflashList();
	}
	
	//m_pStcSymbol->SetPic(_T("data\\ui\\guild\\guild_icon.bmp"));

	m_pSkillPatch->SetInvisible(TRUE);
	m_pGuildMemberPatch->SetInvisible(FALSE);
	m_pRemnantStatePatch->SetInvisible(TRUE);
	m_pGuildWar->SetInvisible(TRUE);
	m_pGuildConstruction->SetInvisible(TRUE);

	m_pWndAlliance->SetInvisible(TRUE);
	m_pWndActiveSkill->SetInvisible(TRUE);

	// 帮派宗旨
	m_pEdtTenet->Clear();
	//-----------------------清一下格式------------------------
	tstring strTenet = GuildMgr::Inst()->GetGuildTenet().c_str();
	tstring::iterator new_end;
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
	strTenet.erase(new_end, strTenet.end());
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
	strTenet.erase(new_end, strTenet.end());
	//---------------------------------------------------------
	m_pEdtTenet->AddText(strTenet.c_str());
	m_pEdtTenet->SetRefresh( TRUE );

	m_bShow = true;
	return bRet;
}

VOID GuildFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Drag:
		{
			for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
			{
				if (pWnd == m_pSkillItem[i])
				{
					OnSkillButtonClick(i);
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
			{
				if (pWnd == m_pSkillItem[i])
				{
					int nChooseItem = m_nCurPageNum * (GUILD_SKILL_ITEM_NUM_PERPAGE - 1) + i;
					const std::vector<tagGuildSkills>& vecSkill = GuildMgr::Inst()->GetSkill();
					if ((size_t)nChooseItem < vecSkill.size())
					{
						tagGuildFacilitiesInfo* pInfo = NULL;
						for( int n = 0; n < MAX_GUILD_FACILITIES_TYPE; n++ )
						{
							if( EFT_Academy == m_sFacilitiesInfo[n].eType )
								pInfo = &m_sFacilitiesInfo[n];
						}
						if( NULL == pInfo )
							break;

						DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(vecSkill[nChooseItem].dwSkillID), 1);
						const tagGuildSkillInfo *pSkillInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
						if (!P_VALID(pSkillInfo))
							break;

						DWORD dwSkillID = vecSkill[nChooseItem].dwSkillID;

						// 不是一级技能则TIPS降一级
						DWORD dwLevel = MTransSkillLevel(dwSkillID);
						if ( 1 != dwLevel)
						{
							dwLevel = dwLevel -1;
						}
						dwSkillID = MTransSkillTypeID(MTransSkillID(vecSkill[nChooseItem].dwSkillID), dwLevel);

						// 如果玩家没有学习过则显示一级的TIPS
						const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(MTransSkillID(dwSkillID));
						if (!P_VALID(pSkillData))
						{
							dwSkillID = MTransSkillTypeID(MTransSkillID(vecSkill[nChooseItem].dwSkillID), 1);
						}

						if (!vecSkill[nChooseItem].bActive)	// 技能没有被激活
						{
							tstring strings = ToolTipCreator::Inst()->GetGuildSkillTips(dwSkillID);
							tstringstream ss;
							ss << g_StrTable[_T("GuildSkill_NotActive")] << _T("\\n") << strings;
							ShowTip(m_pSkillItem[i], ss.str().c_str());
						} 
						else if (pInfo->byLevel < pSkillInfo->SchoolRestrictlevel)
						{
							tstring strings = ToolTipCreator::Inst()->GetGuildSkillTips(dwSkillID);
							TCHAR szVer[128];
							_sntprintf(szVer,128,g_StrTable[_T("GuildSkill_MagicSchoolRestrict")],pSkillInfo->SchoolRestrictlevel);
							tstringstream ss;
							ss << szVer << _T("\\n") << strings;
							ShowTip(m_pSkillItem[i], ss.str().c_str());
						}
						else
						{
							ShowTip(m_pSkillItem[i], ToolTipCreator::Inst()->GetGuildSkillTips(dwSkillID).c_str());
						}
						
					}
				}
			}
			if (pWnd == m_pBtnCurSkillIcon)
			{
				DWORD dwSkillID = GuildMgr::Inst()->GetCurrentResearchSkillID();
				// 不是一级技能则TIPS降一级
				DWORD dwLevel = MTransSkillLevel(dwSkillID);
				if ( 1 != dwLevel)
				{
					dwLevel = dwLevel -1;
				}
				dwSkillID = MTransSkillTypeID(MTransSkillID(dwSkillID), dwLevel);
				ShowTip(m_pBtnCurSkillIcon, ToolTipCreator::Inst()->GetGuildSkillTips(dwSkillID).c_str());
			}
			if (pWnd == m_pIbnActiveItem)
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_strActiveItem.dwID).c_str());
			}
			if( pWnd == m_pLstMembers )
			{
				int row = pEvent->dwParam1;				//鼠标所在的行
				int col = pEvent->dwParam2;				//列
				if( col == 1 )							//第1列是帮众的职位信息
				{
					ShowTip( pWnd, NULL );				//先清空原来的tips
					
					const GuildMembers* pMembers = GuildMgr::Inst()->GetMembers();
					if( !P_VALID(pMembers) )
					{
						ShowTip(pWnd, NULL);
						break;
					}
					const GuildMembers::MembersArray *members;
					pMembers->GetMembersList( &members );
					if( !P_VALID(members) || row >= static_cast<int>(members->size()) )
					{
						ShowTip(pWnd, NULL);
						break;
					}

					int index = GetMember2GuildTips( row );
					if( !GT_VALID(index) )
					{
						ShowTip( pWnd, NULL );
						break;
					}

					const tagGuildMemberClient* pInfo = members->at( index );
					if( !P_VALID(pInfo) )
					{
						ShowTip(pWnd, NULL);
						break;
					}

					tstring strTips = ToolTipCreator::Inst()->GetGuildPosTips( (INT)pInfo->sInfo.n8GuildPos );

					if( !strTips.empty() )
						ShowTip( pEvent->msg.pt + tagPoint(16,16), strTips.c_str() );

				}
				else
				{
					ShowTip(pWnd, NULL);
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUIE_Click:
		{
			
			// 报名
			if (pWnd == m_pBtnApply)
			{
				// 如果不是帮主
				if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != EGMP_HuiZhang)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error1")]);
					return;
				}

				tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
				EWeek week = ServerTime::Inst()->GetCurrentWeekDay();
				switch (week)
				{
				case EWeek_FRI:
					{
						if (time.hour >= 22)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
							return;
						}
					}
					break;
				case EWeek_SAT:
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
						return;
					}
					break;
				case EWeek_SUN:
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
						return;
					}
					break;
				default:
					break;
				}


				const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
				if (P_VALID(pGodInfo))
				{
					if(pGodInfo->eGodState == EGMS_Closed)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
						return;
					}

					if ( pGodInfo->sCapGuild.dwGuildID != 0 && pGodInfo->sCapGuild.dwGuildID != GT_INVALID)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_CantApply")]);
						return;
					}

					if (pGodInfo->dwApplyMoney >= 9999999)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error2")]);
						return;
					}
					INT nMoney = _ttoi(m_pEdtMoney->GetText());
					if (nMoney <= (int)pGodInfo->dwApplyMoney)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error3")]);
						return;
					}
					if (nMoney > GuildMgr::Inst()->GetGuildInfo()->nFund)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error4")]);
						return;
					}
					
					
					if (P_VALID(GetObj("ApplyBox")))
						KillObj("ApplyBox");
					CreateObj("ApplyBox", "MsgBox");
					TCHAR szBuffer[X_LONG_NAME];
					tstring strName = GuildMgr::Inst()->GetGodName(pGodInfo->eGodType);
					_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodState_Apply_Text")], nMoney, strName.c_str());
					TObjRef<MsgBox>("ApplyBox")->Init(_T(""), szBuffer, _T("GodStateApplyEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);	
					return;
				}
			}

			if (pWnd == m_pBtnBackMoney)	//追加竞价
			{
				// 如果不是帮主
				if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != EGMP_HuiZhang)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error1")]);
					return;
				}

				tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
				EWeek week = ServerTime::Inst()->GetCurrentWeekDay();
				switch (week)
				{
				case EWeek_FRI:
					{
						if (time.hour >= 22)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
							return;
						}
					}
					break;
				case EWeek_SAT:
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
						return;
					}
					break;
				case EWeek_SUN:
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error8")]);
						return;
					}
					break;
				default:
					break;
				}

				const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
				if (P_VALID(pGodInfo))
				{
					if(pGodInfo->eGodState == EGMS_Closed)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
						return;
					}

					if ( pGodInfo->sCapGuild.dwGuildID != 0 && pGodInfo->sCapGuild.dwGuildID != GT_INVALID)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_CantApply")]);
						return;
					}

					if (pGodInfo->dwApplyMoney >= 9999999)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error2")]);
						return;
					}
					INT nMoney = _ttoi(m_pEdtMoney->GetText());
					if (nMoney <= (int)pGodInfo->dwApplyMoney)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error3")]);
						return;
					}
					if (nMoney > GuildMgr::Inst()->GetGuildInfo()->nFund)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error4")]);
						return;
					}
					DWORD tempMoney = nMoney + pGodInfo->dwBidFund;

					if (P_VALID(GetObj("ApplyAddBox")))
						KillObj("ApplyAddBox");
					CreateObj("ApplyAddBox", "MsgBox");
					TCHAR szBuffer[X_LONG_NAME];
					tstring strName = GuildMgr::Inst()->GetGodName(pGodInfo->eGodType);
					_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodState_ApplyAdd_Text")], nMoney, strName.c_str(),tempMoney);
					TObjRef<MsgBox>("ApplyAddBox")->Init(_T(""), szBuffer, _T("GodStateAddMoney"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);	
					return;
				}
			}
			// 申请进攻者同盟
			if (pWnd == m_pBtnApplyAttackAlly)
			{
				const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
				if (P_VALID(pGodInfo))
				{
					int nAttackNum = 0;
					for (int i =0; i< MAX_ATTACK_ALLIANCE_GUILD;i++)
					{
						if (pGodInfo->dwAttAlliance[i] == 0 || pGodInfo->dwAttAlliance[i] == GT_INVALID)
							continue;

						nAttackNum++;
					}

					if (nAttackNum == MAX_ATTACK_ALLIANCE_GUILD)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error10")]);
						break;
					}

					if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != EGMP_HuiZhang)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error9")]);
						break;
					}

					if(pGodInfo->eGodState == EGMS_Closed)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
						break;
					}

					if (pGodInfo->sCapGuild.dwGuildID == 0 || pGodInfo->sCapGuild.dwGuildID == GT_INVALID )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Battle_NotHaveAttacker")]);
						break;
					}
					
					for (int i = EGM_NH; i < EGM_MAX; ++i)
					{
						EGodMiracle eGod = static_cast<EGodMiracle>(i);
						const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eGod);
						if (P_VALID(pGodInfo))
						{
							if (	pGodInfo->sDefenceGuild.dwGuildID == RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID()
								||	pGodInfo->sCapGuild.dwGuildID == RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID())
							{
								CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Battle_CanNotApply_AlreadyHave")]);
								return;
							}
						}
					}

					tstring ss = GuildMgr::Inst()->GetGuildName(pGodInfo->sCapGuild.dwGuildID);
					TCHAR szBuffer[X_LONG_NAME];
					tstring strName = GuildMgr::Inst()->GetGodName(pGodInfo->eGodType);
					_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("Battle_ApplyAttackAlly")], strName.c_str(),ss.c_str());

					if(P_VALID(GetObj("ApplyDefenceAlly")))
						KillObj("ApplyDefenceAlly");

					if(P_VALID(GetObj("ApplyAttackAlly")))
						KillObj("ApplyAttackAlly");

					CreateObj("ApplyAttackAlly", "MsgBox");
					TObjRef<MsgBox>("ApplyAttackAlly")->Init(_T(""), szBuffer, _T("BattleApplyAttackAlly"));
				}
				

			}
			// 申请防御者同盟
			if (pWnd == m_pBtnApplyDefenceAlly)
			{
				const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
				if (P_VALID(pGodInfo))
				{

					int nDefenceNum = 0;
					for (int i =0; i< MAX_DEFANCE_ALLIANCE_GUILD;i++)
					{
						if (pGodInfo->dwDefAlliance[i] == 0 || pGodInfo->dwDefAlliance[i] == GT_INVALID)
							continue;

						nDefenceNum++;
					}

					if (nDefenceNum == MAX_DEFANCE_ALLIANCE_GUILD)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error11")]);
						break;
					}


					if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != EGMP_HuiZhang)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Apply_Error9")]);
						break;
					}

					if(pGodInfo->eGodState == EGMS_Closed)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
						break;
					}

					if (pGodInfo->sDefenceGuild.dwGuildID == 0 || pGodInfo->sDefenceGuild.dwGuildID == GT_INVALID )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Battle_NotHaveDefender")]);
						break;
					}

					for (int i = EGM_NH; i < EGM_MAX; ++i)
					{
						EGodMiracle eGod = static_cast<EGodMiracle>(i);
						const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eGod);
						if (P_VALID(pGodInfo))
						{
							if (	pGodInfo->sDefenceGuild.dwGuildID == RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID()
								||	pGodInfo->sCapGuild.dwGuildID == RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID())
							{
								CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Battle_CanNotApply_AlreadyHave")]);
								return;
							}
						}
					}

					tstring ss = GuildMgr::Inst()->GetGuildName(pGodInfo->sDefenceGuild.dwGuildID);
					TCHAR szBuffer[X_LONG_NAME];
					tstring strName = GuildMgr::Inst()->GetGodName(pGodInfo->eGodType);
					_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("Battle_ApplyDefenceAlly")], strName.c_str(),ss.c_str());

					if(P_VALID(GetObj("ApplyAttackAlly")))
						KillObj("ApplyAttackAlly");

					if(P_VALID(GetObj("ApplyDefenceAlly")))
						KillObj("ApplyDefenceAlly");

					CreateObj("ApplyDefenceAlly", "MsgBox");
					TObjRef<MsgBox>("ApplyDefenceAlly")->Init(_T(""), szBuffer, _T("BattleApplyDefenceAlly"));

				}
				
			}

			//确认同盟
			if (pWnd == confirmAllys)
			{
				const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
				if (P_VALID(pGodInfo))
				{
					if(pGodInfo->eGodState == EGMS_Closed)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
						break;
					}
				}
				m_pWndAlliance->SetInvisible(FALSE);

				tagNC_GetAllianceList cmd;
				cmd.eGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
				m_pSession->Send(&cmd);
			}
			

			if (pWnd == m_pBtnBack)
			{
				OnClickPreOrNextButton(true);
			}
			else if (pWnd == m_pBtnNext)
			{
				OnClickPreOrNextButton(false);

			}
			else if (pWnd == m_pBtnBookIcon)
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), this));
			}
			else if (pWnd == m_pLearnBook)
			{
				//判断状态是否满足
				if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage) == true ||
					GuildMgr::Inst()->IsInSpecState(EGDSS_Distress) == true ||
					GuildMgr::Inst()->IsInSpecState(EGDSS_Chaos) == true)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_CantAdvSkillState")]);
					return;
				}
				
				// 判断是否有权限
				EGuildMemberPos ePos = GuildMgr::Inst()->GetLpInfo()->eGuildPos;
				const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList(ePos);
				if (!pPower->bAdvSkill)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_CantAdvSkill")]);
					return;
				}

				DWORD skID = GuildMgr::Inst()->GetCurrentResearchSkillID();
				const std::vector<tagGuildSkills>& vecSkills = GuildMgr::Inst()->GetSkill();
				for (std::vector<tagGuildSkills>::const_iterator it = vecSkills.begin(); 
					it != vecSkills.end(); ++it)
				{
					if (it->dwSkillID == skID)
					{
						if (!it->bActive)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_NotActiveSkills")]);
							return;
						}
					}
				}
				GuildMgr::Inst()->SendGuildSkillUpgrade(m_pSkillBook->GetItemId());
			}
			else if (pWnd == m_pBtnStartResearch)
			{
				const tagGuildBase *pGuildBase = GuildMgr::Inst()->GetGuildInfo();
				if (!P_VALID(m_dwCurChooseSkill))
					return;
				if (MTransSkillLevel(m_dwCurChooseSkill) > MAX_GUILD_SKILL_LEVEL)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_FullLevel")]);
					return;
				}
				// 这里找不到技能原型被定义为已经学习到最大等级
				const tagGuildSkillInfo *pGuildSkillInfo = GuildMgr::Inst()->FindSkillInfo(m_dwCurChooseSkill);
				if (!P_VALID(pGuildSkillInfo))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_FullLevel")]);
					return;
				}

				if (pGuildBase->nFund < pGuildSkillInfo->nResearchFund || 
					pGuildBase->nMaterial < pGuildSkillInfo->nResearchMaterial)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_ResearchNotEnoughMoney")]);
					return;
				}
				if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage) == true || 
					GuildMgr::Inst()->IsInSpecState(EGDSS_Distress) == true || 
					GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare) == true)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_ResearchState")]);
					return;
				}

				const std::vector<tagGuildSkills>& vecSkill = GuildMgr::Inst()->GetSkill();
				for(std::vector<tagGuildSkills>::const_iterator iter = vecSkill.begin();
					iter != vecSkill.end(); ++iter)
				{
					if ( iter->dwSkillID == m_dwCurChooseSkill )
					{
						if (!iter->bActive)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillNotActive")]);
							return;
						}
					}

				}

				GuildMgr::Inst()->SendResearchSkill(MTransSkillID(m_dwCurChooseSkill));
			}
			else if (pWnd == m_pBtnCancelResearch)
			{
				if (!P_VALID(GuildMgr::Inst()->GetCurrentResearchSkillID()))
					return;
				GuildMgr::Inst()->SendResearchSkill(GT_INVALID);

			}
			else if (pWnd == m_pBtnLearnSkill)
			{
				if (!P_VALID(m_dwCurChooseSkill))
					return;
				// 金钱
				INT64 nSilver = CurrencyMgr::Inst()->GetBagSilver();
				
				const tagGuildMember *pMember = GuildMgr::Inst()->GetLpInfo();
				const tagGuildBase *pBase = GuildMgr::Inst()->GetGuildInfo();
				
				
				INT RoleLv = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
				const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillDataByTypeID(m_dwCurChooseSkill);
				if (P_VALID(pSkillData))
				{
					if (pSkillData->nLevel >= MTransSkillLevel(m_dwCurChooseSkill))
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillLearnAlready")]);
						return;
					}
				}

				DWORD dwCurPlayerSkillNextLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(m_dwCurChooseSkill));
				if (P_VALID(pData))
					dwCurPlayerSkillNextLevel = pData->nLevel + 1;
				else
					dwCurPlayerSkillNextLevel = 1;
				DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(m_dwCurChooseSkill), dwCurPlayerSkillNextLevel);
				if (MTransSkillLevel(dwNextSkillID) > MAX_GUILD_SKILL_LEVEL)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillLearnAlreadyFull")]);
					return;
				}
				const tagGuildSkillInfo *pSkillInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
				if (!P_VALID(pSkillInfo))
					return;

				const tagSkillProtoClient *pSkillClient = SkillProtoData::Inst()->FindSkillProto(dwNextSkillID);
				if (!P_VALID(pSkillClient))
					return;

				INT nNeedRoleLv = pSkillClient->nNeedRoleLevel;

				if (nSilver < pSkillInfo->nLearnSilver || 
					pMember->nContribution < pSkillInfo->nLearnContribution ||
					pBase->nFund < pSkillInfo->nLearnFund ||
					pBase->nMaterial < pSkillInfo->nLearnMaterial ||
					RoleLv < nNeedRoleLv ||
					(MTransSkillLevel(m_dwCurChooseSkill) - 1 == 0)
					)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_GuildSkillCannotLearn")]);
					return;
				}

				
				GuildMgr::Inst()->SendLearnGuildSkill(MTransSkillID(m_dwCurChooseSkill));
			}
			else if(pWnd == m_pBtnClose)// 关闭窗口
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), this));
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
				return;
			}
			else if(pWnd == m_pBtnLookupInfo)// 查看信息
			{
							
				OnClickedLookupInfo();
				return;
			}
			else if(pWnd == m_pBtnLeaveGuild)// 脱离门派
			{
				OnClickedLeaveGuild();
				return;
			}
			else if(pWnd == m_pBtnPartyWarStart)// 宣战
			{
				OnStartWar();
				return;
			}
			else if(pWnd == m_pBtnPartyWarCancel)// 取消宣战
			{
				
				return;
			}
			else if(pWnd == m_pBtnZhongzhi)// 修改帮派宗旨
			{
				m_pWndtenet->SetInvisible(FALSE);
				m_pWndtenet->FlipToTop();
				return;
			}
			else if(pWnd == m_pBtnModifyTenet)// 修改帮派宗旨
			{
				tstring strTenet = m_pEditBoxTenet->GetText( TRUE );
				//------------------------------------------------------
				tstring::iterator new_end;
				new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
				strTenet.erase(new_end, strTenet.end());
				new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
				strTenet.erase(new_end, strTenet.end());
				//------------------------------------------------------
				if( strTenet.size() > 200 )
				{
					TCHAR szWarning[100] = {0};
					_stprintf( szWarning, g_StrTable[_T("GuildApply_TenetTooLong")], 200 );
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szWarning );
					break;
				}

				INT nLen = strTenet.size();
				INT nMax = (nLen + 1) * sizeof(TCHAR) + sizeof(tagNC_GuildTenetChange) - 1;
				BYTE* pMsg = new BYTE[nMax];
				tagNC_GuildTenetChange* pCmd = (tagNC_GuildTenetChange*)pMsg;
				pCmd->dwID = pCmd->Crc32("NC_GuildTenetChange");
				memcpy( pCmd->szTenet, strTenet.c_str(), (nLen + 1) * sizeof(TCHAR) );
				pCmd->szTenet[nLen] = _T('\0');
				pCmd->dwSize = nMax;
				m_pSession->Send( pCmd );
				SAFE_DEL_ARRAY( pMsg );

				m_pWndtenet->SetInvisible(TRUE);
				return;
			}
			else if(pWnd == m_pBtnCancelTenet)// 关闭修改帮派宗旨
			{
				m_pWndtenet->SetInvisible(TRUE);
				return;
			}
			else if(pWnd == m_pBtnCloseTenet)// 关闭修改帮派宗旨
			{
				m_pWndtenet->SetInvisible(TRUE);
				return;
			}
			else if(pWnd == m_pBtnAllianceOk)	//确认同盟
			{
				SureAlliance();
				return;
			}
			else if(pWnd == m_pBtnAllianceCancel)// 关闭确认同盟
			{
				m_pWndAlliance->SetInvisible(TRUE);
				return;
			}
			else if(pWnd == m_pBtnAllianceXclose)// 关闭确认同盟
			{
				m_pWndAlliance->SetInvisible(TRUE);
				return;
			}	
// 			else if(pWnd == m_pBtnTurnLeader)// 移交帮主
// 			{				
// 				OnClickedTurnoverLeader();
// 				return;
// 			}
// 			else if(pWnd == m_pBtnAppointMember)// 任命职位《《《《《《《《《
// 			{
// 				OnClickedAppointMember();
// 				return;
// 			}
			else if(pWnd == m_pBtnManageGuild)// 帮派管理 《《《《《《《《《《
			{	
				//OnClickedModifyTenet();
				OnClickedAppointMember();
				//OnClickManageGuild();
				m_pBtnManageGuild->EndGlint();
				return;
			}
			else if(pWnd == m_pBtnKickMember)// 开革成员
			{

				OnClickedKickMember();
				return;
			}
			else if(pWnd == m_pBtnDemissPostion)// 辞去职务
			{
				OnClickedDemissPostion();
				return;
			}
			//向第一页的方向翻页
 			else if( pWnd == m_pBtnPageBack )
 			{
 				//不能再翻页了，已经是第一页了，第一页索引为0
 				if( m_nCurPage <= 0 )
 					break;
 
 				m_nCurPage--;
 				RefreshGuildsDisplay();
 			}
 			//向最后一页的方向翻页
 			else if( pWnd == m_pBtnPageFront )
 			{
 				//不能再翻页了，已经是最后一页了
 				if( m_nCurPage >= (GetPages()-1) )
 					break;
 
 				m_nCurPage++;
 				RefreshGuildsDisplay();
 			}
			else if( pWnd == m_pBtnSearch )
			{
				m_pWndInput->SetText( _T("") );
				m_pWndSearch->SetInvisible( FALSE );
				m_pWndSearch->FlipToTop();
			}
			else if( pWnd == m_pBtnLevelUp[EFT_Guild] )	//帮派升级
			{
				HandInItems(EFT_Guild);
			}
			else if( pWnd == m_pBtnLevelUp[EFT_Fund] )	//金库升级
			{
				HandInItems(EFT_Fund);
			}
			else if( pWnd == m_pBtnLevelUp[EFT_Material] )	//资财库升级
			{
				HandInItems(EFT_Material);
			}
			else if( pWnd == m_pBtnLevelUp[EFT_Academy] )	//魔法学院升级
			{
				HandInItems(EFT_Academy);
			}
			else if (pWnd == m_pBtnActiveResearch)	//激活公会技能
			{
				const tagGuildMember* pLpInfo = GuildMgr::Inst()->GetLpInfo();
				if(!P_VALID(pLpInfo))
					break;

				if (pLpInfo->eGuildPos == EGMP_HuiZhang || 
					pLpInfo->eGuildPos == EGMP_FuHuiZhang)
				{
					m_pWndActiveSkill->SetInvisible(FALSE);
					m_pIbnActiveItem->RefreshItem();
				}
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_ActiveNoPower")]);
				}
				
			}
			else if (pWnd == m_pBnActiveClose)	//关闭激活公会技能界面
			{
				m_pWndActiveSkill->SetInvisible(TRUE);
				m_strActiveItem.clear();
			}
			else if (pWnd == m_pBnActive)
			{
				if(m_dwCurChooseSkill == GT_INVALID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_ActiveNoSkill")]);
					break;
				}

				if (m_strActiveItem.dwID == 0 || m_strActiveItem.dwID == GT_INVALID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_ActiveNoItem")]);
					break;
				}
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_strActiveItem.n64ID);
				if (P_VALID(pItem))
				{
					const tagItemProto* pProto= pItem->GetProto();
					if(P_VALID(pProto))
					{
						if (pProto->nSpecFuncVal1 == m_dwCurChooseSkill)
						{
							tagNC_ActiveGuildSkill cmd;
							cmd.n64ItemID = m_strActiveItem.n64ID;
							cmd.dwGuildSkillID = MTransSkillID(m_dwCurChooseSkill);
							m_pSession->Send(&cmd);
						}
						else
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_ActiveItemNotSuit")]);
							break;
						}
					}
				}

				
			}
			//搜索界面的关闭按钮
			else if( pWnd == m_pBtnSearchClose )
			{
				m_pWndSearch->SetInvisible( TRUE );
			}
			//搜索界面的取消按钮
			else if( pWnd == m_pBtnSearchCancel )
			{
				m_pWndSearch->SetInvisible( TRUE );
			}
			//搜索界面的确定按钮
			else if( pWnd == m_pBtnSearchOk )
			{
				INT nType  = GT_INVALID;
				if( EGUIBS_PushDown == m_pPbtnGuild->GetState() )
					nType = 0;
				else if( EGUIBS_PushDown == m_pPbtnLeader->GetState() )
					nType = 1;

				if( GT_VALID(nType) )
				{
					int nIndex = SearchGuild( nType, m_pWndInput->GetText() );
					if( !GT_VALID(nIndex) )
					{
						tagNC_SearchGuild cmd;
						cmd.dwSearchMode = nType;
						INT len = _tcslen(m_pWndInput->GetText());
						INT size = sizeof(cmd.szName)/sizeof(cmd.szName[0]);
						len = min(len,size-1);
						_tcsncpy( cmd.szName, m_pWndInput->GetText(),len );
						cmd.szName[X_SHORT_NAME-1]=0;
						m_pSession->Send( &cmd );
						break;
					}

					//得到目标帮派的页
					int nPage = nIndex / GUILD_APPLY_MAX_ITEMS_PER_PAGE_2;
					m_nCurPage = nPage;
					RefreshGuildsDisplay();
					m_pLstGuilds->SetCurSelectedRow( nIndex - m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE_2 );
					SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
					m_pWndSearch->SetInvisible( TRUE );
				}
			}
			else
			{
				for(INT i=0; i<GuildMembers::ELCT_End; ++i)// 排序
				{
					if(pWnd == m_pBtnListIndex[i])
					{
						//重新排序
						GuildMgr::Inst()->SortMembers(i);
						m_eSortType = (GuildMembers::EListColType)(i);
						//刷新列表
						ReflashList();
						break;
					}
				}
			}
			
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pWnd == m_pPbnOffline)
			{
				ReflashList();
				return;
			}
			else
			{
				if (pEvent->dwParam1 != EGUIBS_PushDown)//不是按下不处理
					return;

				if(pWnd == m_pPbnMembers)
				{
					m_eGuildFrameState = EGFS_MemberList;
					m_pSkillPatch->SetInvisible(TRUE);
					m_pGuildMemberPatch->SetInvisible(FALSE);
					m_pRemnantStatePatch->SetInvisible(TRUE);
					m_pGuildWar->SetInvisible(TRUE);
					m_pGuildConstruction->SetInvisible(TRUE);

					m_pWndActiveSkill->SetInvisible(TRUE);
					m_strActiveItem.clear();

					m_pWndAlliance->SetInvisible(TRUE);
					GuildMgr::Inst()->SendGuildAllMembersGet();

					if(P_VALID(GetObj("StartGuild_War")))
						KillObj("StartGuild_War");
					return;
				}
				else if (pWnd == m_pPbnSkills)
				{
					m_eGuildFrameState = EGFS_GuildSkill;
					m_pSkillPatch->SetInvisible(FALSE);
					m_pGuildMemberPatch->SetInvisible(TRUE);
					m_pRemnantStatePatch->SetInvisible(TRUE);
					m_pGuildWar->SetInvisible(TRUE);
					m_pGuildConstruction->SetInvisible(TRUE);
					GuildMgr::Inst()->SendGetGuildSkillInfo();
					m_pWndAlliance->SetInvisible(TRUE);

					if(P_VALID(GetObj("StartGuild_War")))
						KillObj("StartGuild_War");
					return;
				}
				else if (pWnd == m_pPbnInfomation)
				{
					m_eGuildFrameState = EGFS_Remnant;
					m_pSkillPatch->SetInvisible(TRUE);
					m_pGuildMemberPatch->SetInvisible(TRUE);
					m_pRemnantStatePatch->SetInvisible(FALSE);
					m_pGuildWar->SetInvisible(TRUE);
					m_pGuildConstruction->SetInvisible(TRUE);

					m_pWndActiveSkill->SetInvisible(TRUE);
					m_strActiveItem.clear();
					GuildMgr::Inst()->SendGetAllGodMiracleInfo();

					if(P_VALID(GetObj("StartGuild_War")))
						KillObj("StartGuild_War");
					return;
				}
				else if (pWnd == m_pPbnWar)
				{
					tagNC_GetAllGuildSmallInfo cmd;
					m_pSession->Send(&cmd);

					return;
				}
				else if (pWnd == m_pPbnConstruction)
				{
					m_eGuildFrameState = EGFS_Construction;
					m_pGuildWar->SetInvisible(TRUE);
					m_pGuildConstruction->SetInvisible(FALSE);
					m_pSkillPatch->SetInvisible(TRUE);
					m_pGuildMemberPatch->SetInvisible(TRUE);
					m_pRemnantStatePatch->SetInvisible(TRUE);

					m_pWndActiveSkill->SetInvisible(TRUE);
					m_strActiveItem.clear();

					m_pWndAlliance->SetInvisible(TRUE);

					if(P_VALID(GetObj("StartGuild_War")))
						KillObj("StartGuild_War");

					tagNC_GetGuildUpgradeInfo cmd;
					cmd.dwNPCID = GT_INVALID;
					m_pSession->Send(&cmd);
					/*GuildMgr::Inst()->SendGetAllGodMiracleInfo();*/
					return;
				}
			}	 
		}
		break;
	case EGUIE_ItemDblClick:     //双击帮派中成员
		{
			if (pWnd == m_pLstMembers &&m_mapTemp.empty()&& pEvent->dwParam1 < static_cast<DWORD>(m_pLstMembers->GetTotalRowNum()) )
			{
				
				//发送一条消息通知密语更新窗口并打开
				tagOpenMiniChatEvent evt(_T("tagOpenMiniChatEvent"), this);
				evt.name = m_pLstMembers->GetText(pEvent->dwParam1,0);
				m_pMgr->SendEvent(&evt);
				//关闭帮派界面
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), this));
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
			}
		}
		break;
	case EGUISCB_Drop:
		{
			if ( pWnd == m_pIbnActiveItem )
			{
				if( EDT_Item != (EDragType)pEvent->dwParam1 )
					break;

				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
				if ( EISF_GuildSkillActive != pItem->GetItemSpecFunc())
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_Active_NotSuitItem")]);
					break;
				}
				if (P_VALID(pItem))
				{
					m_pIbnActiveItem->RefreshItem( pItem->GetItemTypeID(),pItem->GetItemQuantity() ,pItem->GetItemQuality());
					m_strActiveItem.dwID = pItem->GetItemTypeID();
					m_strActiveItem.n64ID	= pItem->GetItemId();
				}
			}

			if (pWnd == m_pBtnBookIcon)
			{
				if( EDT_Item != (EDragType)pEvent->dwParam1 )
					break;

				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
				if ( EISF_GuildSkillBook != pItem->GetItemSpecFunc())
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildSkill_Active_NotSuitItem2")]);
					break;
				}

				tagUseGuildSkillBookEvent event(_T("GuildEvent_UseGuildSkillBook"), this);
				event.pItem = pItem;
				m_pFrameMgr->SendEvent(&event);
			}
	
		}
		break;
	case EGUIE_ItemClick:
		{
			if (pWnd == m_pGuildMemberListBox)
			{
				GameFrame* pFrame = m_pMgr->GetFrame(_T("MemberPos"));
				if(P_VALID(pFrame))
				{
					m_pFrameMgr->AddToDestroyList(pFrame);
				}

			}
			if (pWnd == m_pLstMembers)
			{
				m_pBtnLookupInfo->SetEnable(TRUE);
				const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
				if(P_VALID(pInfo))
				{
					// 如果是自己直接返回
					if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
					{
						m_pBtnLookupInfo->SetEnable(FALSE);
					}
				}
				else
				{
					m_pBtnLookupInfo->SetEnable(FALSE);
				}

				const tagGuildAppoint *pAppoint = GuildMgr::Inst()->GetAppointList((INT)GuildMgr::Inst()->GetLpInfo()->eGuildPos);
				if(P_VALID(pInfo))
				{
					if(pAppoint->BitSetGuildAppoint[pInfo->sInfo.n8GuildPos])
					{
						m_pBtnManageGuild->SetEnable(true);
					}
					else
					{
						m_pBtnManageGuild->SetEnable(false);
					}

				}
				
			}
			if( pWnd == m_pLstGuilds )
			{
				SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
				
			}
			if( pWnd == m_pLstAlliance )
			{
				SelectListBoxAlly( m_pLstAlliance->GetCurSelectedRow() );

			}
 			if (pWnd == m_pLstClanState)
 			{
 				m_nCurChooseRenmant = static_cast<int>(pEvent->dwParam1);
 				if (m_nCurChooseRenmant >= EGM_NH && m_nCurChooseRenmant < EGM_MAX)
 					UpdateSelectGodInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
 				else
 				{
 					m_pBtnApply->SetEnable(false);
 					m_pStcAttackGuildName->SetText(_T(""));
 					m_pStcAttackGuildLv->SetText(_T(""));
 					m_pStcAttackGuildMember->SetText(_T(""));
 					//m_pStcAttackMoney->SetText(_T(""));
 					m_pStcDefenceGuildName->SetText(_T(""));
 					m_pStcDefenceGuildLv->SetText(_T(""));
 					m_pStcDefenceGuildMember->SetText(_T(""));
 					//m_pStcDefenceDay->SetText(_T(""));
 					m_pWnd->SetRefresh();
 				}
 			}
		}
	}
}


VOID GuildFrame::ReflashGuild()
{
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	if (!P_VALID(m_pGuild))
		return;

	// 名称
	m_pStcName->SetText(GuildMgr::Inst()->GetGuildName(m_pGuild->dwID).c_str());
	// 等级
	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], m_pGuild->byLevel);
	m_pStcLevel->SetText(szTmp);
	// 帮派标志

	// 创始人
	m_pStcFounder->SetText(PlayerNameTab::Inst()->FindNameByNameID(m_pGuild->dwFounderNameID));
	// 帮主
	m_pStcLeader->SetText(PlayerNameTab::Inst()->FindNameByID(m_pGuild->dwLeaderRoleID));
	// 成员数量
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16MemberNum);
	m_pStcMemberNum->SetText(szTmp);
	// 帮派威望
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nReputation);
	m_pStcReputation->SetText(szTmp);
	// 帮派状态

	// 帮派资金
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nFund);
	m_pStcFund->SetText(szTmp);
	// 帮派资材
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nMaterial);
	m_pStcMaterial->SetText(szTmp);
	// 帮派安定
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16Peace);
	//m_pStcPeace->SetText(szTmp);
	// 每日维护费用
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->nDailyCost);
	m_pStcDailyCost->SetText(szTmp);
	
	// 帮派宗旨
	m_pEdtTenet->Clear();
	//-----------------------清一下格式------------------------
	tstring strTenet = GuildMgr::Inst()->GetGuildTenet().c_str();
	tstring::iterator new_end;
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
	strTenet.erase(new_end, strTenet.end());
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
	strTenet.erase(new_end, strTenet.end());
	//---------------------------------------------------------
	m_pEdtTenet->AddText(strTenet.c_str());
	
	for (int i = 0; i < 5; ++i)
	{	
		m_pIstState[i]->SetInvisible(TRUE);
	}
	
	int nCount = 0;
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Shortage))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-duanque.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_01")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Distress))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-poor.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_02")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Chaos))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-luan.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_03")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-war.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_04")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Primacy))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-king.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_05")]);
		nCount++;
	}
	if (GuildMgr::Inst()->IsInSpecState(EGDSS_Refulgence))
	{
		m_pIstState[nCount]->SetPic(_T("data\\ui\\guild\\L_guild-icon-huihuang.bmp"));
		m_pIstState[nCount]->SetInvisible(FALSE);
		m_pIstState[nCount]->SetTips(g_StrTable[_T("GuildState_06")]);
		nCount++;
	}
	//m_pWnd->SetRefresh(true);
}

VOID GuildFrame::ReflashList()
{
	m_pLstMembers->Clear();
	ReflashListImpl();
}

VOID GuildFrame::ReflashListImpl()
{
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	bool bShowOffline = (m_pPbnOffline->GetState() == EGUIBS_PushDown);
		
	m_pLstMembers->Clear();
	m_mapTemp.clear();
	m_vecIndex.clear();

	INT nRow = 0;
	const GuildMembers* pMembers = GuildMgr::Inst()->GetMembers();
	if(P_VALID(pMembers))
	{
		const GuildMembers::MembersArray *members;
		pMembers->GetMembersList(&members);
		for(INT i=0; i<(INT)members->size(); ++i)
		{
			const tagGuildMemberClient* pInfo = members->at(i);

			if(!(bShowOffline || pInfo->sInfo.bOnline))
				continue;
			
			//离线玩家颜色置灰
			DWORD dwColor = 0xFFFFFFFF;
			if(!pInfo->sInfo.bOnline)
				dwColor = 0xFF808080;

			//名称
			LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID);
			if(_tcslen(szName) > 0)
			{
				m_pLstMembers->SetText(nRow, 0, szName, dwColor);
			}
			else
			{
				m_mapTemp.insert(make_pair(pInfo->sInfo.dwRoleID, tagMemberNameReq(nRow, dwColor)));
			}

			//职位
			m_pLstMembers->SetText(nRow, 1, GuildMgr::Inst()->GetMemberPos((INT)pInfo->sInfo.n8GuildPos).c_str(), dwColor);
			//级别
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], pInfo->sInfo.byLevel);
			m_pLstMembers->SetText(nRow, 2, szTmp, dwColor);
			//性别
			m_pLstMembers->SetText(nRow, 3, GuildMgr::Inst()->GetMemberSex(pInfo->sInfo.bySex).c_str(), dwColor);
			//职业
			m_pLstMembers->SetText(nRow, 4, ProfessionMgr::Inst().GetClassName(EClassType(pInfo->sInfo.byClass)), dwColor);
			//帮派贡献
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), pInfo->sInfo.nCurContrib);
			m_pLstMembers->SetText(nRow, 5, szTmp, dwColor);

			//离线时间
			m_pLstMembers->SetText(nRow, 6, GetOfflineTime(pInfo->sInfo.bOnline, pInfo->sInfo.dwTimeLastLogout).c_str(), dwColor);
			
			m_vecIndex.push_back(i);
			++nRow;
		}

		// 成员数量
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_pGuild->n16MemberNum);
		m_pStcMemberNum->SetText(szTmp);
	}
	m_pWnd->SetRefresh(TRUE);
}

VOID GuildFrame::ReflashButton()
{
	m_pGuild = GuildMgr::Inst()->GetGuildInfo();
	if (!P_VALID(m_pGuild))
		return;
	
	// 获取当前玩家权限表
	const tagGuildPower* pPower = GuildMgr::Inst()->GetPowerList((INT)GuildMgr::Inst()->GetLpInfo()->eGuildPos);
	if (!P_VALID(pPower))
		return;

	m_pBtnLeaveGuild->SetEnable(pPower->bLeaveGuild);		// 脱离帮派 
	//m_pBtnTurnLeader->SetEnable(pPower->bTurnoverLeader);	// 移交帮主
	//m_pBtnManageGuild->SetEnable(pPower->bModifyTenet);		// 修改宗旨
	m_pBtnDemissPostion->SetEnable(pPower->bDemissPostion);	// 辞去职务
	m_pBtnStartResearch->SetEnable(pPower->bSetSkill);
	m_pBtnCancelResearch->SetEnable(pPower->bSetSkill);

	//m_pBtnAppointMember->SetEnable(false);
	const tagGuildAppoint *pAppoint = GuildMgr::Inst()->GetAppointList((INT)GuildMgr::Inst()->GetLpInfo()->eGuildPos);
	for (size_t i = 0; i < pAppoint->BitSetGuildAppoint.count(); ++i)
	{
		if (pAppoint->BitSetGuildAppoint[i] != 0)
		{
			//m_pBtnAppointMember->SetEnable(true);
			break;
		}
	}
	
	//根据策划案，帮助虽然不能脱离帮派，但是可以点击脱离帮派的按钮。
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() == static_cast<int>(EGMP_HuiZhang))
	{
		m_pBtnLeaveGuild->SetEnable(true);
		m_pBtnManageGuild->SetEnable(true);
		return;
	}

	
	//其他状态写在后面。。


}

VOID GuildFrame::SendGuildAppoint( DWORD dwDstRoleID, INT nPos )
{
	tagNC_GuildAppoint cmd;
	cmd.dwRoleID = dwDstRoleID;
	cmd.ePos = (EGuildMemberPos)nPos;
	m_pSession->Send(&cmd);

}

VOID GuildFrame::SendGuildLeave()
{
	tagNC_GuildLeave cmd;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildKick( DWORD dwDstRoleID )
{
	tagNC_GuildKick cmd;
	cmd.dwRoleID = dwDstRoleID;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildTurnover( DWORD dwDstRoleID )
{
	tagNC_GuildTurnover cmd;
	cmd.dwRoleID = dwDstRoleID;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildDemiss()
{
	tagNC_GuildDemiss cmd;
	m_pSession->Send(&cmd);
}

VOID GuildFrame::SendGuildTenetChange( tstring strTenetInfo )
{
	//判断宗旨是否非法
	DWORD dwErrorCode = FilterData::Inst()->WordsFilteror(strTenetInfo, 201);
	if(E_Success != dwErrorCode)
	{
		//发送提示信息
		GuildMgr::Inst()->ShowGuildErrorMsg(dwErrorCode);
		return;
	}

	INT nLen = strTenetInfo.size();
	INT nMax = (nLen + 1) * sizeof(TCHAR) + sizeof(tagNC_GuildTenetChange) - 1;
	BYTE* pMsg = new BYTE[nMax];
	tagNC_GuildTenetChange* pCmd = (tagNC_GuildTenetChange*)pMsg;
	pCmd->dwID = pCmd->Crc32("NC_GuildTenetChange");
	memcpy(pCmd->szTenet, strTenetInfo.c_str(), (nLen + 1) * sizeof(TCHAR));
	pCmd->szTenet[nLen] = _T('\0');
	pCmd->dwSize = nMax;
	m_pSession->Send(pCmd);
	SAFE_DEL_ARRAY(pMsg);
}

DWORD GuildFrame::OnEventGetRoleName( tagRoleGetNameEvent* pGameEvent )
{
	if(pGameEvent->bResult)
	{
		NameReqList::iterator it = m_mapTemp.find(pGameEvent->dwRoleID);
		if(it != m_mapTemp.end())
		{
			const tagMemberNameReq& sMember = it->second;
			m_pLstMembers->SetText(sMember.nIndex, 0, pGameEvent->strRoleName.c_str(), sMember.dwColor);
			//m_pLstMembers->SetRefresh(true);
			m_mapTemp.erase(it);
		}
		
		// 如果是帮主，则要设置帮主名称
		if(P_VALID(m_pGuild) && m_pGuild->dwLeaderRoleID == pGameEvent->dwRoleID)
		{
			m_pStcLeader->SetText(pGameEvent->strRoleName.c_str());
			//m_pStcLeader->SetRefresh(true);
		}
	}
	return 0;
}


DWORD GuildFrame::OnEventGetNameByNameID( tagGetNameByNameID* pGameEvent )
{
	if(pGameEvent->bResult)
	{
		if(P_VALID(m_pGuild) && pGameEvent->dwNameID == m_pGuild->dwFounderNameID)
		{
			m_pStcFounder->SetText(pGameEvent->strName.c_str());
			//m_pStcFounder->SetRefresh(true);
		}
	}
	return 0;
}

DWORD GuildFrame::OnEventGuildPosAppoint( tagGuildPosAppointEvent* pGameEvent )
{
	// 如果所变更的职位是没有跑商权限嘀
	const tagGuildPower *pPower = GuildMgr::Inst()->GetPowerList((INT)pGameEvent->ePos);
	if (!pPower->bCommerce)
	{
		if (GetObj("CommerceWarning_MsgBox"))
			KillObj("CommerceWarning_MsgBox");
		CreateObj("CommerceWarning_MsgBox", "MsgBox");

		TObjRef<MsgBox>("CommerceWarning_MsgBox")->Init(_T(""), g_StrTable[_T("GuildCommerceWarning0")], 
			_T("OnEventCommerceWarningMsgBox"), MsgBoxFlag(MBF_OK | MBF_Cancel), TRUE);
		m_dwTempRoleID = pGameEvent->dwRoleID;
		m_nTempPOS = (INT)pGameEvent->ePos;
		return 0;
	}

	SendGuildAppoint(pGameEvent->dwRoleID, (INT)pGameEvent->ePos);
	return 0;
}

DWORD GuildFrame::OnEventGuildInfoReflash( tagGuildInfoReflashEvent* pGameEvent )
{
	// 如果是本地玩家，则要刷新一下按钮
	if(pGameEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
	{
		ReflashButton();
	}
	ReflashGuild();
	ReflashList();
	return 0;
}

DWORD GuildFrame::OnEventGuildInfoReq( tagGuildInfoReflashEvent* pGameEvent )
{
	//发送请求事件
	tagNC_GuildRefMember cmd;
	cmd.dwRoleID = pGameEvent->dwRoleID;
	m_pSession->Send(&cmd);
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxInputTenet( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBIT_Text == pGameEvent->eInputType && MBF_OK == pGameEvent->eResult)
	{
		// 发送修改事件
		SendGuildTenetChange(pGameEvent->strInputName.c_str());
	}
	return 0;
}

DWORD GuildFrame::OnEventGetGuildName( tagGuildNameGetEvent* pGameEvent )
{
	if(P_VALID(m_pGuild) && pGameEvent->dwGuildID == m_pGuild->dwID)
	{
		m_pStcName->SetText(pGameEvent->strName.c_str());
		//m_pStcName->SetRefresh(true);
	}
	return 0;
}

DWORD GuildFrame::OnEventGetGuildTenet( tagGameEvent* pGameEvent )
{
	m_pEdtTenet->Clear();
	//-----------------------清一下格式------------------------
	tstring strTenet = GuildMgr::Inst()->GetGuildTenet().c_str();
	tstring::iterator new_end;
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
	strTenet.erase(new_end, strTenet.end());
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
	strTenet.erase(new_end, strTenet.end());
	//---------------------------------------------------------
	m_pEdtTenet->SetText(strTenet.c_str());
	//m_pEdtTenet->SetRefresh(true);
	return 0;
}

DWORD GuildFrame::OnEventGuildListReflash( tagGameEvent* pGameEvent )
{
	// 刷新列表
	GuildMgr::Inst()->SortMembers(m_eSortType);
	ReflashList();
	return 0;
}


VOID GuildFrame::OnClickedLookupInfo()
{
	GameFrame* pFrame = m_pMgr->GetFrame(_T("MemberInfo"));
	if(P_VALID(pFrame))// 有窗体，则发送更新事件
	{
		const tagGuildMemberClient* pMember = this->GetMember(m_pLstMembers->GetCurSelectedRow());
		if(P_VALID(pMember))
		{
			// 如果还没获取扩展信息则需要获取一下
			if(!pMember->bGetInfo)
			{
				// 发送网络消息获取成员扩展信息
				tagNC_GuildGetMemberEx cmd;
				cmd.dwRoleID = pMember->sInfo.dwRoleID;
				m_pSession->Send(&cmd);
			}

			tagGuildInfoReflashEvent event(_T("GuildEvent_InfoUpdate"), this);
			event.dwRoleID = pMember->sInfo.dwRoleID;
			m_pMgr->SendEvent(&event);
		}
	}
	else// 没有则重新创建
	{
		INT nIndex = m_pLstMembers->GetCurSelectedRow();
		if(nIndex >= 0 && nIndex < (INT)m_vecIndex.size())
			m_pMgr->CreateFrame(m_strName.c_str(), _T("MemberInfo"), _T("GuildMemberInfoFrame"), m_vecIndex[nIndex]);
		const tagGuildMemberClient* pMember = this->GetMember(m_pLstMembers->GetCurSelectedRow());
		if(P_VALID(pMember))
		{
			// 如果还没获取扩展信息则需要获取一下
			if(!pMember->bGetInfo)
			{
				// 发送网络消息获取成员扩展信息
				tagNC_GuildGetMemberEx cmd;
				cmd.dwRoleID = pMember->sInfo.dwRoleID;
				m_pSession->Send(&cmd);
			}

			tagGuildInfoReflashEvent event(_T("GuildEvent_InfoUpdate"), this);
			event.dwRoleID = pMember->sInfo.dwRoleID;
			m_pMgr->SendEvent(&event);
		}
	}
}

VOID GuildFrame::OnClickedLeaveGuild()
{
	// 如果是帮主
	if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() == static_cast<int>(EGMP_HuiZhang))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_BangZhuLeaveCheck")]);
		return;
	}
	else if( GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InGodBattle")]);
		return;
	}
	else
	{
		// 弹出确认对话框
		if(P_VALID(GetObj("LeaveGuild")))
			KillObj("LeaveGuild");

		CreateObj("LeaveGuild", "MsgBox");
		TObjRef<MsgBox>("LeaveGuild")->Init(
			_T(""),g_StrTable[_T("GuildText_LeaveCheck")],
			_T("MsgBox_LeaveCheck"));
	}
}

VOID GuildFrame::OnClickedTurnoverLeader()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// 如果是自己直接返回
		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
			return;
		
		// 存储目标ID
		m_dwDstRoleID = pInfo->sInfo.dwRoleID;


		// 弹出确认对话框
		if(P_VALID(GetObj("TurnoverLeader")))
			KillObj("TurnoverLeader");

		TCHAR szTmp[X_LONG_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildText_TurnoverCheck")], 
			PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID));

		CreateObj("TurnoverLeader", "MsgBox");
		TObjRef<MsgBox>("TurnoverLeader")->Init(
			_T(""), szTmp, _T("MsgBox_TurnoverCheck"));
	}
}

VOID GuildFrame::OnClickedAppointMember()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// 如果是自己直接返回
// 		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
// 			return;

		// 查找界面，如果有则直接刷新，没有则创建
		GameFrame* pFrame = m_pMgr->GetFrame(_T("MemberPos"));
		if(P_VALID(pFrame))
		{
			((GuildMemberPosFrame*)pFrame)->ReflashPosButton(m_vecIndex[m_pLstMembers->GetCurSelectedRow()]);
		}
		else
		{
			m_pMgr->CreateFrame(m_strName.c_str(), _T("MemberPos"), _T("GuildMemberPosFrame"), m_vecIndex[m_pLstMembers->GetCurSelectedRow()]);
		}
	}
}

/*VOID GuildFrame::OnClickedModifyTenet()
{
	// 弹出帮派宗旨输入对话框
	if(P_VALID(GetObj("ModifyTenet")))
		KillObj("ModifyTenet");

	CreateObj("ModifyTenet", "MsgInputBox");
	TObjRef<MsgInputBox>("ModifyTenet")->Init(
		_T(""),g_StrTable[_T("GuildText_InputTenet")],
		_T("MsgBox_ModifyTenet"),GT_INVALID, MBIT_Text);
}*/

void GuildFrame::OnClickManageGuild()
{
	GuildApplyManageFrame* pFrame = (GuildApplyManageFrame*)m_pMgr->GetFrame( _T("GuildApplyManageFrame") );
	if( !P_VALID(pFrame) )
	{
		pFrame = (GuildApplyManageFrame*)m_pMgr->CreateFrame( _T("World"), _T("GuildApplyManageFrame"), _T("GuildApplyManageFrame"), 0 );
	}

	if( !P_VALID(pFrame) )
		return;

	pFrame->ShowWnd( TRUE );
}

VOID GuildFrame::OnClickedKickMember()
{
	const tagGuildMemberClient* pInfo = this->GetMember(m_pLstMembers->GetCurSelectedRow());
	if(P_VALID(pInfo))
	{
		// 如果是自己直接返回
		if(pInfo->sInfo.dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
			return;
		// 判断是否可以开革该玩家
		const tagGuildMember* pLp = GuildMgr::Inst()->GetLpInfo();
		if(!P_VALID(pLp))
			return;
		// 获取开革表
		const tagGuildKick* pKick = GuildMgr::Inst()->GetKickList((INT)pLp->eGuildPos);
		if(!P_VALID(pKick))
			return;
		// 判断是否可开革
		if(!pKick->BitSetGuildKick[pInfo->sInfo.n8GuildPos])
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildError_Kick")]);
			return;
		}

		// 存储目标ID
		m_dwDstRoleID = pInfo->sInfo.dwRoleID;

		// 弹出确认对话框
		if(P_VALID(GetObj("KickMember")))
			KillObj("KickMember");

		TCHAR szTmp[X_LONG_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildText_KickCheck")], 
			PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID), PlayerNameTab::Inst()->FindNameByID(pInfo->sInfo.dwRoleID));
		
		CreateObj("KickMember", "MsgBox");
		TObjRef<MsgBox>("KickMember")->Init(
			_T(""), szTmp, _T("MsgBox_KickCheck"));
	}
}

VOID GuildFrame::OnClickedDemissPostion()
{
	// 弹出确认对话框
	if(P_VALID(GetObj("DemissPostion")))
		KillObj("DemissPostion");

	CreateObj("DemissPostion", "MsgBox");
	TObjRef<MsgBox>("DemissPostion")->Init(
		_T(""),g_StrTable[_T("GuildText_DemissCheck")],
		_T("MsgBox_DemissCheck"));
}

BOOL GuildFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Faction"), this));
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_Book"), NULL));
	return TRUE;
}

VOID GuildFrame::DestroyMsgBox()
{
	if(P_VALID(GetObj("ModifyTenet")))
		KillObj("ModifyTenet");

	if(P_VALID(GetObj("LeaveGuild")))
		KillObj("LeaveGuild");	

	if(P_VALID(GetObj("TurnoverLeader")))
		KillObj("TurnoverLeader");

	if(P_VALID(GetObj("KickMember")))
		KillObj("KickMember");

	if(P_VALID(GetObj("DemissPostion")))
		KillObj("DemissPostion");

	if (GetObj("CommerceWarning_MsgBox"))
		KillObj("CommerceWarning_MsgBox");

	if(P_VALID(GetObj("DeclareGuildWar")))
		KillObj("DeclareGuildWar");

	if(P_VALID(GetObj("StartGuild_War")))
		KillObj("StartGuild_War");
}

DWORD GuildFrame::OnEventMsgBoxLeaveCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildLeave();
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxTurnoverCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildTurnover(m_dwDstRoleID);
		m_dwDstRoleID = GT_INVALID;
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxKickCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildKick(m_dwDstRoleID);
		m_dwDstRoleID = GT_INVALID;
	}
	return 0;
}

DWORD GuildFrame::OnEventMsgBoxDemissCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		SendGuildDemiss();
	}
	return 0;
}

tstring GuildFrame::GetOfflineTime( bool bIsOnline, DWORD dwOfflineTime /*= GT_INVALID*/ )
{
	if(bIsOnline)// 在线
	{
		return g_StrTable[_T("GuildList_Online")];
	}
	
	// 获取当前时间和离线时间
	tagDWORDTime srcTime = (tagDWORDTime)dwOfflineTime;
	tagDWORDTime dstTime = ServerTime::Inst()->CalCurrentServerDwordTime();
	// 计算过了多少秒
	DWORD dwSecond = CalcTimeDiff(dstTime, srcTime);
	if(dwSecond > 365*24*60*60)// 大于1年
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Year")], dwSecond/(365*24*60*60));
		return szTmp;
	}
	else if(dwSecond > 30*24*60*60)// 大于1个月
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Month")], dwSecond/(30*24*60*60));
		return szTmp;
	}
	else if(dwSecond > 24*60*60)// 大于1天
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Day")], dwSecond/(24*60*60));
		return szTmp;
	}
	else if(dwSecond > 60*60)// 大于1小时
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Hour")], dwSecond/(60*60));
		return szTmp;
	}
	else// 小于一小时
	{
		return g_StrTable[_T("GuildList_Offline_Min")];
	}
}

const tagGuildMemberClient* GuildFrame::GetMember( INT nIndex )
{
	if(nIndex <0 || nIndex >= (INT)m_vecIndex.size())
		return NULL;

	return GuildMgr::Inst()->GetMember(m_vecIndex[nIndex]);
}

DWORD GuildFrame::OnEventRefreshGuildSkills(tagGameEvent *pEvent)
{
	//m_pGuildMemberPatch->SetInvisible(TRUE);
	RefreshSkills();
	//m_pSkillPatch->SetInvisible(FALSE);
	return 0;
}

DWORD GuildFrame::OnEventShowGuildFrame(tagGameEvent *pEvent)
{
	const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
	if( P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tagNC_GetGuildApplierBill cmd;
		m_pSession->Send( &cmd );
	}
	// 刷新帮派
	ReflashGuild();
	// 刷新功能按钮
	ReflashButton();

	//m_pLstMembers->Clear();

	m_pWnd->SetInvisible(FALSE);

	m_pFrameMgr->SendEvent(&tagGameEvent( _T("Open_Guild_UI"), NULL ));
	m_bShow = true;
	return 0;
}

DWORD GuildFrame::OnEventHideGuildFrame(tagGameEvent *pEvent)
{
	if(P_VALID(GetObj("StartGuild_War")))
		KillObj("StartGuild_War");
	// 公会申请同盟界面
	m_pWndAlliance->SetInvisible(TRUE);
	// 公会技能激活界面
	m_pWndActiveSkill->SetInvisible(TRUE);
	m_strActiveItem.clear();
	//确认同盟界面
	m_pWndAlliance->SetInvisible(TRUE);
	//帮派宗旨界面
	m_pWndtenet->SetInvisible(TRUE);
	//帮派搜索界面
	m_pWndSearch->SetInvisible(TRUE);
	// 隐藏技能页
	m_pSkillPatch->SetInvisible(TRUE);
	// 隐藏神迹列
	//m_pRemnantStatePatch->SetInvisible(TRUE);
	// 显示成员列
	m_pGuildMemberPatch->SetInvisible(FALSE);

	m_pPbnMembers->SetState(EGUIBS_PushDown, TRUE);
	m_eGuildFrameState = EGFS_MemberList;
	// 整体隐藏
	m_pWnd->SetInvisible(TRUE);
	m_bShow = false;
	m_pFrameMgr->SendEvent(&tagGameEvent( _T("Close_Guild_UI"), NULL ));
	
	return 0;
}

void GuildFrame::SetSkillDefaultUI()
{
	for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
	{
		m_pSkillItem[i]->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
		m_pSkillName[i]->SetText(_T(""));
		m_pSkillLevel[i]->SetText(_T(""));
	}
	m_pBtnCurSkillIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
	m_pBtnBookIcon->RefreshItem();

	m_nCurPageNum = 0;
	m_pBtnBack->SetEnable(false);
	m_pBtnNext->SetEnable(false);
	const std::vector<tagGuildSkills>& vecSkills = GuildMgr::Inst()->GetSkill();
	int nSize = static_cast<int>(vecSkills.size());
	if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
	{
		m_pBtnNext->SetEnable(true);
	}
	m_pProResearchProgress->SetValue(0);
}

void GuildFrame::RefreshSkills()
{
	//SetSkillDefaultUI();
	const std::vector<tagGuildSkills>& vecSkills = GuildMgr::Inst()->GetSkill();
	size_t size = vecSkills.size();
	size_t ItemBegin = m_nCurPageNum * GUILD_SKILL_ITEM_NUM_PERPAGE;
	if (ItemBegin < size)
	{
		DWORD dwItem = 0;
		for (size_t i = ItemBegin; i < size; ++i, ++dwItem)
		{
			DWORD dwID = vecSkills[i].dwSkillID;
			bool bActive = vecSkills[i].bActive;
			bool bMaxLevel = false;
			const tagSkillProtoClient*  pProto = SkillProtoData::Inst()->FindSkillProto(dwID);
			if (!P_VALID(pProto))
			{
				bMaxLevel = true;
			}

			if ( !bMaxLevel/*MTransSkillLevel(dwID) <= nPlayerSkillMaxLv*/)
			{
				tstring strIconPath = SkillMgr::Inst()->GetSkillIconPath(dwID);
				m_pSkillItem[i]->SetPic(strIconPath);
				if (!bActive)
				{
					m_pSkillItem[i]->SetPicColor(0xCC1D1A2C);
				}
				else
				{	
					m_pSkillItem[i]->SetPicColor(0xFFFFFFFF);
				}
				DWORD dwLevel = MTransSkillLevel(dwID) - 1;
				DWORD dwCurPlayerSkillLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwID));
				if (P_VALID(pData))
					dwCurPlayerSkillLevel = pData->nLevel;
				else
					dwCurPlayerSkillLevel = 0;

				tstring strFormat = _T("%d/%d");
				TCHAR buffer[10];
				_sntprintf(buffer, 10, strFormat.c_str(), dwCurPlayerSkillLevel, dwLevel);
				m_pSkillLevel[i]->SetText(buffer);

				const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwID);
				if (P_VALID(pInfo))
					m_pSkillName[i]->SetText(pInfo->strName.c_str());
			}
			else
			{
				// 转换到上一级
				DWORD dwChangeID = MTransSkillTypeID((MTransSkillID(dwID)),(MTransSkillLevel(dwID) - 1));
				tstring strIconPath = SkillMgr::Inst()->GetSkillIconPath(dwChangeID);
				m_pSkillItem[i]->SetPic(strIconPath);
				if (!bActive)
				{
					m_pSkillItem[i]->SetPicColor(0xCC1D1A2C);
				}
				else
				{	
					m_pSkillItem[i]->SetPicColor(0xFFFFFFFF);
				}
				DWORD dwLevel = MTransSkillLevel(dwID) - 1;
				DWORD dwCurPlayerSkillLevel;
				const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwID));
				if (P_VALID(pData))
					dwCurPlayerSkillLevel = pData->nLevel;
				else
					dwCurPlayerSkillLevel = 0;
				tstring strFormat = _T("%d/%d");
				TCHAR buffer[10];
				_sntprintf(buffer, 10, strFormat.c_str(), dwCurPlayerSkillLevel, dwLevel);
				m_pSkillLevel[i]->SetText(buffer);

				const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwChangeID);
				if (P_VALID(pInfo))
					m_pSkillName[i]->SetText(pInfo->strName.c_str());
			}
		}
	}
	// 设置当前研究技能
	DWORD curResearchSkillID= GuildMgr::Inst()->GetCurrentResearchSkillID();
	const tagSkillProtoClient*  pProto = SkillProtoData::Inst()->FindSkillProto(curResearchSkillID);
	if (P_VALID(curResearchSkillID) && MTransSkillLevel(curResearchSkillID) <= MAX_GUILD_SKILL_LEVEL && P_VALID(pProto))
	{
		INT32 curResearchSkillProgress = GuildMgr::Inst()->GetCurrentResearchSkillProgress();
		tstring strCurResearchSkillIcon = SkillMgr::Inst()->GetSkillIconPath(curResearchSkillID);
		m_pBtnCurSkillIcon->SetPic(strCurResearchSkillIcon);
		const tagGuildSkillInfo *pCurInfo = GuildMgr::Inst()->FindSkillInfo(curResearchSkillID);
		if (P_VALID(pCurInfo))
		{
			m_pStcCurSkillName->SetText(pCurInfo->strName.c_str());
			tstring strFormat = _T("%d");
			TCHAR buffer[10];
			_sntprintf(buffer, 10, strFormat.c_str(), MTransSkillLevel(pCurInfo->dwSkillID));
			//m_pStcCurSkillLevel->SetText(buffer);
			_sntprintf(buffer, 10, strFormat.c_str(), pCurInfo->nResearchFund);
			m_pStcFundCost->SetText(buffer);
			_sntprintf(buffer, 10, strFormat.c_str(), pCurInfo->nResearchMaterial);
			m_pStcMaterialCost->SetText(buffer);
			m_pProResearchProgress->SetMaxValue((FLOAT)pCurInfo->n16Fulfill);
			m_pProResearchProgress->SetValue((FLOAT)curResearchSkillProgress, true);
			m_pProResearchProgress->SetRefresh();
		}
	}
	// 如果当前研究技能不存在
	else
	{
		m_pBtnCurSkillIcon->SetPic(_T("data\\ui\\main\\l_icon_back.bmp"));
		m_pStcCurSkillName->SetText(_T(""));
		//m_pStcCurSkillLevel->SetText(_T(""));
		m_pStcFundCost->SetText(_T(""));
		m_pStcMaterialCost->SetText(_T(""));
		m_pProResearchProgress->SetMaxValue(0);
		m_pProResearchProgress->SetValue(0, true);
	}

	// 当技能升级以后自动给当前选中等级增加一级
	if (P_VALID(m_dwCurChooseSkill))
	{
		std::vector<tagGuildSkills>::const_iterator iter = vecSkills.begin();
		for ( ;iter != vecSkills.end() ; ++iter)
		{
			// 说明当前技能的等级没有发生变化
			if (iter->dwSkillID == m_dwCurChooseSkill)
				break;
		}
			// 没有找到
		if(iter == vecSkills.end())
		{
			int nLevels = MTransSkillLevel(m_dwCurChooseSkill);
			DWORD dwSkillid = MTransSkillTypeID(MTransSkillID(m_dwCurChooseSkill), nLevels +1);

			std::vector<tagGuildSkills>::const_iterator iter2 = vecSkills.begin();
			for ( ;iter2 != vecSkills.end() ; ++iter2)
			{
				// 说明当前技能的等级没有发生变化
				if (iter2->dwSkillID == dwSkillid)
				{
					m_dwCurChooseSkill = dwSkillid;
					break;
				}		
			}
		}
	}


	if (!P_VALID(m_dwCurChooseSkill))
		m_pBtnLearnSkill->SetEnable(false);
	else{
		DWORD dwCurPlayerSkillNextLevel;
		const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(m_dwCurChooseSkill));
		if (P_VALID(pData))
			dwCurPlayerSkillNextLevel = pData->nLevel + 1;
		else
			dwCurPlayerSkillNextLevel = 1;
		DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(m_dwCurChooseSkill), dwCurPlayerSkillNextLevel);
		if (MTransSkillLevel(dwNextSkillID) > MAX_GUILD_SKILL_LEVEL)
			return;
		m_pBtnLearnSkill->SetEnable(true);

		tstring strFormat = _T("%d");
		TCHAR buffer[20];

		const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
		if(P_VALID(pInfo))
		{
			_sntprintf(buffer, 20, strFormat.c_str(),MSilver2DBGold(pInfo->nLearnSilver));
			m_pStcLearnGoldCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(), MSilver2DBSilver(pInfo->nLearnSilver));
			m_pStcLearnSilverCost->SetText(buffer);
			//_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnFund);
			//m_pStcLearnGuildFundCost->SetText(buffer);
			// 		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnMaterial);
			// 		m_pStcLearnGuildMaterialCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnContribution);
			m_pStcLearnContributionNeed->SetText(buffer);
		}
		else
		{
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnGoldCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnSilverCost->SetText(buffer);
			//_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnFund);
			//m_pStcLearnGuildFundCost->SetText(buffer);
			// 		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnMaterial);
			// 		m_pStcLearnGuildMaterialCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnContributionNeed->SetText(buffer);
		}
	}



	const tagGuildMember* pLpInfo = GuildMgr::Inst()->GetLpInfo();
	if(P_VALID(pLpInfo))
	{
		if (pLpInfo->eGuildPos == EGMP_HuiZhang || pLpInfo->eGuildPos == EGMP_FuHuiZhang)
		{
			m_pBtnActiveResearch->SetEnable(TRUE);
			m_pBtnStartResearch->SetEnable(TRUE);
		}
		else
		{
			m_pBtnActiveResearch->SetEnable(FALSE);
			m_pBtnStartResearch->SetEnable(FALSE);
		}		
	}	
}

void GuildFrame::OnSkillButtonClick(int nButton)
{
	int nChooseItem = m_nCurPageNum * (GUILD_SKILL_ITEM_NUM_PERPAGE - 1) + nButton;
	const std::vector<tagGuildSkills>& vecSkill = GuildMgr::Inst()->GetSkill();
	if ((size_t)nChooseItem < vecSkill.size())
	{
		DWORD dwSkillID = vecSkill[nChooseItem].dwSkillID;
		m_dwCurChooseSkill = dwSkillID;
		DWORD dwCurPlayerSkillNextLevel;
		const tagSkillData* pData =  SkillMgr::Inst()->GetSkillData(MTransSkillID(dwSkillID));
		if (P_VALID(pData))
			dwCurPlayerSkillNextLevel = pData->nLevel + 1;
		else
			dwCurPlayerSkillNextLevel = 1;
		DWORD dwNextSkillID = MTransSkillTypeID(MTransSkillID(dwSkillID), dwCurPlayerSkillNextLevel);
		m_pSkillItemBack[nButton]->SetPic(_T("data\\ui\\Common\\L_icon-l.bmp"));
		if (MTransSkillLevel(dwNextSkillID) > MAX_GUILD_SKILL_LEVEL)
			return;
		m_pBtnLearnSkill->SetEnable(true);
		
		tstring strFormat = _T("%d");
		TCHAR buffer[20];

		const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwNextSkillID);
		if(P_VALID(pInfo))
		{
			_sntprintf(buffer, 20, strFormat.c_str(),MSilver2DBGold(pInfo->nLearnSilver));
			m_pStcLearnGoldCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(), MSilver2DBSilver(pInfo->nLearnSilver));
			m_pStcLearnSilverCost->SetText(buffer);
			//_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnFund);
			//m_pStcLearnGuildFundCost->SetText(buffer);
			// 		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnMaterial);
			// 		m_pStcLearnGuildMaterialCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnContribution);
			m_pStcLearnContributionNeed->SetText(buffer);
		}
		else
		{
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnGoldCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnSilverCost->SetText(buffer);
			//_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnFund);
			//m_pStcLearnGuildFundCost->SetText(buffer);
			// 		_sntprintf(buffer, 20, strFormat.c_str(), pInfo->nLearnMaterial);
			// 		m_pStcLearnGuildMaterialCost->SetText(buffer);
			_sntprintf(buffer, 20, strFormat.c_str(),0);
			m_pStcLearnContributionNeed->SetText(buffer);
		}

		for (int i = 0; i < GUILD_SKILL_ITEM_NUM_PERPAGE; ++i)
		{
			if (i == nButton)
				continue;
			m_pSkillItemBack[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
		}
	}
}

void GuildFrame::OnClickPreOrNextButton(bool bOrder)
{
	// 如果是后退按钮
	if (bOrder)
	{
		if (m_nCurPageNum > 0)
		{
			m_nCurPageNum--;
			m_pBtnNext->SetEnable(true);
		}
		if (m_nCurPageNum == 0)
			m_pBtnBack->SetEnable(false);
	}
	// 如果是前进按钮
	else
	{
		const std::vector<tagGuildSkills>& vecSkill = GuildMgr::Inst()->GetSkill();
		int nSize = static_cast<int>(vecSkill.size());
		if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
		{
			m_nCurPageNum++;
			m_pBtnNext->SetEnable(true);
		}
		if (nSize > (m_nCurPageNum + 1) * GUILD_SKILL_ITEM_NUM_PERPAGE)
			m_pBtnNext->SetEnable(false);
	}
}

DWORD GuildFrame::OnEventRefreshGuild(tagGameEvent *pEvent)
{
	ReflashGuild();
	ReflashButton();
	return 0;
}

DWORD GuildFrame::OnEventUsGuildSkillBook(tagUseGuildSkillBookEvent *pGameEvent)
{
	//判断是否有当前研究技能
	if (!P_VALID(GuildMgr::Inst()->GetCurrentResearchSkillID()))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildText_HaveNotCurReserachSkill")]);
		return 0;
	}
	Item *pItem = pGameEvent->pItem;
	tstring strIconPath = ItemMgr::Inst()->GetItemIconPath(pItem->GetItemTypeID());
	if (P_VALID(pItem))
	{
		m_pBtnBookIcon->RefreshItem(pItem->GetItemTypeID(),pItem->GetItemQuantity() ,pItem->GetItemQuality());
	}

	const tagItemProto *pProto = pItem->GetProto();
	if (!P_VALID(pProto))
		return 0;
	m_pSkillBook = pItem;
	INT32 nProgressInc = pProto->nSpecFuncVal1;
	tstring strFormat(_T("%d"));
	TCHAR buffer[12];
	_sntprintf(buffer, 12, strFormat.c_str(), nProgressInc);
	m_pProgressIncrement->SetText(buffer);
	m_pLearnBook->SetEnable(true);
	return 0;
}

DWORD GuildFrame::OnEventRefreshBook(tagGameEvent *pGameEvent)
{
	m_pBtnBookIcon->RefreshItem();
	m_pProgressIncrement->SetText(_T(""));
	m_pLearnBook->SetEnable(false);
	m_pSkillBook = NULL;
	return 0;
}

DWORD GuildFrame::OnEventCommerceWarningMsgBox(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		SendGuildAppoint(m_dwTempRoleID, (INT)m_nTempPOS);
	}
	return 0;
}


DWORD GuildFrame::OnEventRefreshClanRemnantState(tagGameEvent *pGameEvent)
{
	
	m_vecClanStateID.clear();
	m_pLstClanState->Clear();
	
	

	// 记录需要申请名字的帮派ID
	std::vector<DWORD> vecGuildID;
	m_bNeedUpdate = false;
	
	// 查看不能找到的名字
	for (int i = EGM_NH; i < EGM_MAX; ++i)
	{
		EGodMiracle eGod = static_cast<EGodMiracle>(i);
		const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eGod);
		if (P_VALID(pGodInfo))
		{
			if (!GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sDefenceGuild.dwGuildID))
				vecGuildID.push_back(pGodInfo->sDefenceGuild.dwGuildID);
			if (!GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sDefenceGuild.dwGuildID))
				vecGuildID.push_back(pGodInfo->sCapGuild.dwGuildID);
		}
	}

	if (!vecGuildID.empty())
	{
		m_bNeedUpdate = true;
		int nTemp = 0;	
		GuildMgr::Inst()->SendGetSomeGuildName(nTemp, vecGuildID);
	}
	
	RefreshGodState();
	return 0;
	
}




int GuildFrame::GetMember2GuildTips( int nRow )
{
	const GuildMembers* pMembers = GuildMgr::Inst()->GetMembers();
	if( !P_VALID(pMembers) )
		return GT_INVALID;

	const GuildMembers::MembersArray *members;
	pMembers->GetMembersList( &members );

	INT nRowTmp = 0;
	bool bShowOffline = (m_pPbnOffline->GetState() == EGUIBS_PushDown);
	for( INT i=0; i< (INT)members->size(); ++i )
	{
		const tagGuildMemberClient* pInfo = members->at( i );
		if( !pInfo->sInfo.bOnline && !bShowOffline )
			continue;

		if( nRowTmp == nRow )
			return i;

		nRowTmp++;
	}

	return GT_INVALID;
}

void GuildFrame::UpdateSelectGodInfo(EGodMiracle eGod)
{
	
	TCHAR szBuffer[X_LONG_NAME];

	const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eGod);
	if (P_VALID(pGodInfo))
	{
		if (GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sCapGuild.dwGuildID))
		{
			tstring strName = GuildMgr::Inst()->GetGuildName(pGodInfo->sCapGuild.dwGuildID);
			m_pStcAttackGuildName->SetText(strName.c_str());
		}
		else
			m_bNeedUpdate = true;

		if (pGodInfo->sCapGuild.nLevel)
		{
			_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->sCapGuild.nLevel);
			m_pStcAttackGuildLv->SetText(szBuffer);
		}
		else
			m_pStcAttackGuildLv->SetText(_T(""));
		
		if (pGodInfo->sCapGuild.nNum)
		{
			_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->sCapGuild.nNum);
			m_pStcAttackGuildMember->SetText(szBuffer);
		}
		else
			m_pStcAttackGuildMember->SetText(_T(""));
		
// 		_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->dwApplyMoney);
// 		m_pStcAttackMoney->SetText(szBuffer);
		if( pGodInfo->dwApplyMoney == 0 )
			m_pStcAttackGuildName->SetText(_T(""));

		if (GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sDefenceGuild.dwGuildID))
		{
			tstring strName = GuildMgr::Inst()->GetGuildName(pGodInfo->sDefenceGuild.dwGuildID);
			m_pStcDefenceGuildName->SetText(strName.c_str());
		}
		else
			m_bNeedUpdate = true;

		if (pGodInfo->sDefenceGuild.nLevel)
		{
			_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->sDefenceGuild.nLevel);
			m_pStcDefenceGuildLv->SetText(szBuffer);
		}
		else
			m_pStcDefenceGuildLv->SetText(_T(""));
		
		if (pGodInfo->sDefenceGuild.nNum)
		{
			_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->sDefenceGuild.nNum);
			m_pStcDefenceGuildMember->SetText(szBuffer);
		}
		else
			m_pStcDefenceGuildMember->SetText(_T(""));
		
		tagDWORDTime time = static_cast<tagDWORDTime>(pGodInfo->dwStarttime);
		DWORD dwSec = CalcTimeDiff(ServerTime::Inst()->CalCurrentServerDwordTime(), time);
		
		// 转化成天
// 		DWORD dwDay = dwSec / (24 * 60 * 60);
// 		_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodState_Text5")], dwDay);
// 		m_pStcDefenceDay->SetText(szBuffer);

		// 进攻者同盟
		for (int i =0; i< MAX_ATTACK_ALLIANCE_GUILD;i++)
		{
			m_pStcAttackAllies[i]->SetText(_T(""));
		}

		for (int i =0; i< MAX_DEFANCE_ALLIANCE_GUILD;i++)
		{
			m_pStcDefenceAllies[i]->SetText(_T(""));
		}

		m_bNeedRefreshAlliance = false;

		for (int i =0; i< MAX_ATTACK_ALLIANCE_GUILD;i++)
		{
			m_pStcAttackAllies[i]->SetText(_T(""));
			if (pGodInfo->dwAttAlliance[i] == 0 || pGodInfo->dwAttAlliance[i] == GT_INVALID)
				break;

			tstring ss = GuildMgr::Inst()->GetGuildName(pGodInfo->dwAttAlliance[i]);
			if (ss == _T(""))
				m_bNeedRefreshAlliance = true;

			m_pStcAttackAllies[i]->SetText(ss.c_str());
		}

		// 防御者同盟
		for (int i =0; i< MAX_DEFANCE_ALLIANCE_GUILD;i++)
		{
			m_pStcDefenceAllies[i]->SetText(_T(""));
			if (pGodInfo->dwDefAlliance[i] == 0 || pGodInfo->dwDefAlliance[i] == GT_INVALID)
				break;

			tstring ss = GuildMgr::Inst()->GetGuildName(pGodInfo->dwDefAlliance[i]);
			if (ss == _T(""))
				m_bNeedRefreshAlliance = true;

			m_pStcDefenceAllies[i]->SetText(ss.c_str());
		}

		// 如果神迹处于非竞标状态
		if (pGodInfo->eGodState == EGMS_Rest)
		{
			//BYTE nCurHour = ServerTime::Inst()->GetCurrentHour();
			//if (nCurHour >= 15 && nCurHour <= 18)
			//	m_pBtnApply->SetEnable(true);
			//else
			//	m_pBtnApply->SetEnable(false);
		}
		// 总是打开

		_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pGodInfo->dwBidFund);
		m_pStcNowBegMoney->SetText(szBuffer);

		// 追加竞价和竞标只能有一个可以使用
		if (pGodInfo->dwBidFund)
		{
			m_pBtnBackMoney->SetEnable(true);
			m_pBtnApply->SetEnable(false);

		} 
		else
		{
			m_pBtnBackMoney->SetEnable(false);
			m_pBtnApply->SetEnable(true);
		}
	}
	m_pWnd->SetRefresh(TRUE);
	
}


VOID GuildFrame::Update()
{
	if (m_eGuildFrameState == EGFS_Remnant)
	{
		if (m_bNeedUpdate)
			RefreshGodState();

		if (m_bNeedRefreshAlliance)
			RefreshAllianceDisplay();
	}
	if (needRefresh)
		RefreshAlliance(m_eTypeEx);
}

void GuildFrame::RefreshGodState()
{
	
	m_bNeedUpdate = false;

	tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
	EWeek week = ServerTime::Inst()->GetCurrentWeekDay();
	switch (week)
	{
	case EWeek_MON:
		{
			m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_1")]);
			tagDWORDTime tempTime = IncreaseTime(time,3600*24*4);
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("22:00") );
			m_pStcBattleTime->SetText(szBuffer);

			m_pBtnApplyAttackAlly->SetEnable(FALSE);
			m_pBtnApplyDefenceAlly->SetEnable(FALSE);
			confirmAllys->SetEnable(FALSE);
		}
		break;
	case EWeek_TUES:
		{
			m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_1")]);
			tagDWORDTime tempTime = IncreaseTime(time,3600*24*3);
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("22:00") );
			m_pStcBattleTime->SetText(szBuffer);

			m_pBtnApplyAttackAlly->SetEnable(FALSE);
			m_pBtnApplyDefenceAlly->SetEnable(FALSE);
			confirmAllys->SetEnable(FALSE);
		}
		break;
	case EWeek_WEDNES:
		{
			m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_1")]);
			tagDWORDTime tempTime = IncreaseTime(time,3600*24*2);
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("22:00") );
			m_pStcBattleTime->SetText(szBuffer);

			m_pBtnApplyAttackAlly->SetEnable(FALSE);
			m_pBtnApplyDefenceAlly->SetEnable(FALSE);
			confirmAllys->SetEnable(FALSE);
		}
		break;
	case EWeek_THURS:
		{
			m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_1")]);
			tagDWORDTime tempTime = IncreaseTime(time,3600*24*1);
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("22:00") );
			m_pStcBattleTime->SetText(szBuffer);

			m_pBtnApplyAttackAlly->SetEnable(FALSE);
			m_pBtnApplyDefenceAlly->SetEnable(FALSE);
			confirmAllys->SetEnable(FALSE);
		}
		break;
	case EWeek_FRI:
		{
			if (time.hour < 22)
			{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_1")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*0);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("22:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(FALSE);
				m_pBtnApplyDefenceAlly->SetEnable(FALSE);
				confirmAllys->SetEnable(FALSE);
			}
			else
			{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_2")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*1);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("18:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(TRUE);
				m_pBtnApplyDefenceAlly->SetEnable(TRUE);
				confirmAllys->SetEnable(TRUE);
			}
		}
		break;
	case EWeek_SAT:
		{
			if (time.hour < 18)
			{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_2")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*0);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("18:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(TRUE);
				m_pBtnApplyDefenceAlly->SetEnable(TRUE);
				confirmAllys->SetEnable(TRUE);
			}
			else if (time.hour < 20)
			{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_3")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*0);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("20:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(FALSE);
				m_pBtnApplyDefenceAlly->SetEnable(FALSE);
				confirmAllys->SetEnable(FALSE);
			}
			else if(time.hour < 21)
			{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_4")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*0);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("21:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(FALSE);
				m_pBtnApplyDefenceAlly->SetEnable(FALSE);
				confirmAllys->SetEnable(FALSE);
			}
			else{
				m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_5")]);
				tagDWORDTime tempTime = IncreaseTime(time,3600*24*1);
				TCHAR szBuffer[X_LONG_NAME] = {0};
				_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("00:00") );
				m_pStcBattleTime->SetText(szBuffer);

				m_pBtnApplyAttackAlly->SetEnable(FALSE);
				m_pBtnApplyDefenceAlly->SetEnable(FALSE);
				confirmAllys->SetEnable(FALSE);
			}
		}
		break;
	case EWeek_SUN:
		{
			m_pStcBattleDesc->SetText(g_StrTable[_T("combat_time_desc_5")]);
			tagDWORDTime tempTime = IncreaseTime(time,3600*24*0);
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("combat_time_time")],tempTime.year,tempTime.month,tempTime.day,_T("00:00") );
			m_pStcBattleTime->SetText(szBuffer);

			m_pBtnApplyAttackAlly->SetEnable(FALSE);
			m_pBtnApplyDefenceAlly->SetEnable(FALSE);
			confirmAllys->SetEnable(FALSE);
		}
		break;
	default:
		break;
	}
	// 遍历神迹
	for (int i = EGM_NH; i < EGM_MAX; ++i)
	{
		EGodMiracle eGod = static_cast<EGodMiracle>(i);
		m_pLstClanState->SetText(i, 0, GuildMgr::Inst()->GetGodName(eGod));
		const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eGod);
		if (P_VALID(pGodInfo))
		{
			if (pGodInfo->sDefenceGuild.dwGuildID == 0)
			{
				m_pLstClanState->SetText(i, 1, g_StrTable[_T("GodState_Text0")]);
			}
			else
			{
				// 看看能不能找到帮派名
				if (GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sDefenceGuild.dwGuildID))
				{
					tstring strName = GuildMgr::Inst()->GetGuildName(pGodInfo->sDefenceGuild.dwGuildID);
					// 如果是玩家所在的帮派，画成绿色
					if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->sDefenceGuild.dwGuildID)
						m_pLstClanState->SetText(i, 1, strName.c_str(), 0xFF00FF00);
					else
						m_pLstClanState->SetText(i, 1, strName.c_str());
				}
				else
				{
					GuildMgr::Inst()->GetGuildName(pGodInfo->sDefenceGuild.dwGuildID);
					m_bNeedUpdate = true;
				}
			}

			if (pGodInfo->sCapGuild.dwGuildID == 0)
			{
				m_pLstClanState->SetText(i, 2, g_StrTable[_T("GodState_Text0")]);
			}
			else
			{
				// 看看能不能找到帮派名
				if (GuildMgr::Inst()->IsGuildNameExist(pGodInfo->sCapGuild.dwGuildID))
				{
					tstring strName = GuildMgr::Inst()->GetGuildName(pGodInfo->sCapGuild.dwGuildID);
					// 如果是玩家所在的帮派，画成绿色
					if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->sCapGuild.dwGuildID)
						m_pLstClanState->SetText(i, 2, strName.c_str(), 0xFF00FF00);
					else
						m_pLstClanState->SetText(i, 2, strName.c_str());
				}
				else
				{
					GuildMgr::Inst()->GetGuildName(pGodInfo->sCapGuild.dwGuildID);
					m_bNeedUpdate = true;
				}
			}
			// 占领状态
			switch (pGodInfo->eGodState)
			{
			case EGMS_Closed:
				{
					m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_9")],GrayColour);
				} break;
			case EGMS_Null:
				{
					m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_0")],BlueColour);
				} break;
			case EGMS_Rest:
				{
					// 计算时间
					BYTE nCurHour = ServerTime::Inst()->GetCurrentHour();
					BYTE nMin = ServerTime::Inst()->GetCurrentMinute();
					EWeek week = ServerTime::Inst()->GetCurrentWeekDay();
					if (	(week > EWeek_SUN && week < EWeek_THURS )
						||	(week == EWeek_FRI && nCurHour < 22))
					{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_2")],YellowColour);
					}
					else if ( week == EWeek_FRI || (week == EWeek_SAT && nCurHour <18))
					{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_3")],YellowColour);
					}
					else
					{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_1")],GrayColour);
					}
				} break;
			case EGMS_Capture:
				{
					for (int n = 0;n<MAX_ATTACK_ALLIANCE_GUILD; n++)
					{
						if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->dwAttAlliance[n])
						{
							m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_8")],RedColour);
							break;
						}
					}

					for (int n = 0;n<MAX_DEFANCE_ALLIANCE_GUILD; n++)
					{
						if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->dwDefAlliance[n])
						{
							m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_6")],GreenColour);
							break;
						}
					}

					if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->sCapGuild.dwGuildID)
					{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_7")],RedColour);
					}
					else if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() == pGodInfo->sCapGuild.dwGuildID)
					{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_5")],GreenColour);
					}
					else{
						m_pLstClanState->SetText(i, 3, g_StrTable[_T("GodState_Text_new_4")],GrayColour);
					}
				} break;
			default: break;
			}
		}
	}
	// 刷新选中
	if (m_nCurChooseRenmant >= EGM_NH && m_nCurChooseRenmant < EGM_MAX)
		UpdateSelectGodInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
		

}

DWORD GuildFrame::OnEventApply(tagMsgBoxEvent *pEvent)
{
 	if (pEvent->eResult == MBF_OK)
 	{
 		tagNC_ApplyCapGodMiracle cmd;
 		cmd.dwGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
 		INT nMoney = _ttoi(m_pEdtMoney->GetText());
 		cmd.dwMoney = static_cast<DWORD>(nMoney);
		cmd.bAddBid = false;
 		TObjRef<NetSession>()->Send(&cmd);
 		m_pEdtMoney->SetText(_T(""));
 	}
	return 0;
}

DWORD GuildFrame::OnEventAddMoney(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		tagNC_ApplyCapGodMiracle cmd;
		cmd.dwGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
		INT nMoney = _ttoi(m_pEdtMoney->GetText());
		cmd.dwMoney = static_cast<DWORD>(nMoney);
		cmd.bAddBid = true;
		TObjRef<NetSession>()->Send(&cmd);
		m_pEdtMoney->SetText(_T(""));
	}
	return 0;
}

DWORD GuildFrame::OnEventShowRemnant(tagGameEvent *pEvent)
{
	m_pPbnInfomation->SetState(EGUIBS_PushDown, TRUE);
	return 0;
}


DWORD GuildFrame::OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam )
{
	if( pMsg->n16BillNum > 0 )
		m_bIsHaveGuildApply = TRUE;
	else 
		m_bIsHaveGuildApply = FALSE;
	//如果有人申请入帮并且是帮主则闪烁按钮
	const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
	if(m_bIsHaveGuildApply&& P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pBtnManageGuild->SetGlintPicColor(0xff00ffff);
		m_pBtnManageGuild->BeginGlint();
	}
	else m_pBtnManageGuild->EndGlint();

	return 0;
}


DWORD GuildFrame::OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam )
{
 	if( !P_VALID(pMsg) )
 		return 0;
 
 	//获取帮派失败
 	if( pMsg->dwErrorCode != E_Success )
 	{
 		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_FailedToGetGuildInfo")] );
 		return 0;
 	}
 
 	if( pMsg->nGuildNum == 0 )
 		return 0;
 
 	m_lstGuildInfo.clear();
 	UpdateGuildList( pMsg );
 	RefreshGuildsDisplay();
 	SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
	return 0;
}

DWORD GuildFrame::OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam )
{
 	if( pMsg->dwErrorCode == E_Success )
 	{
 		if( pMsg->nGuildNum == 0 )
 			return 0;
 
 		tagGuildSmallInfo info;
 		INT nGuildCount = pMsg->nGuildNum;
 		for( INT i = 0; i< nGuildCount; i++ )
 		{
 			memcpy( &info, &(pMsg->guilds[i]), sizeof(tagGuildSmallInfo) );
 			AddGuildInfo( info );
 		}
 
 		int nIndex = m_lstGuildInfo.size()-1;
 		int nPage = nIndex/GUILD_APPLY_MAX_ITEMS_PER_PAGE_2;
 		m_nCurPage = nPage;
 		RefreshGuildsDisplay();
 		m_pLstGuilds->SetCurSelectedRow( nIndex - m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE_2 );
 		SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
 
 
 		return 0;
 	}

	CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NotFindTheGuild")] );

	return 0;
}

void GuildFrame::UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg )
{
 	tagGuildSmallInfo info;
 	INT nGuildCount = pMsg->nGuildNum;
 
 	for( INT i = 0; i< nGuildCount; i++ )
 	{
 		// 		info.byLevel = pMsg->guilds[i].byLevel;
 		// 		info.dwGuildID = pMsg->guilds[i].dwGuildID;
 		// 		info.dwLeaderID = pMsg->guilds[i].dwLeaderID;
 		// 		info.nGuildMembersNum = pMsg->guilds[i].nGuildMembersNum;
 		// 		_tcscpy( info.szGuildName,pMsg->guilds[i].szGuildName );
 		// 		_tcscpy( info.szLeaderName, pMsg->guilds[i].szLeaderName );
 		//info = pMsg->guilds[i];
 		memcpy( &info, &(pMsg->guilds[i]), sizeof(tagGuildSmallInfo) );

		// 不存存储自己的公会信息
		if ( RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID() != info.dwGuildID )
		{
			AddGuildInfo( info );
		}	
 	}
}

void GuildFrame::AddGuildInfo( tagGuildSmallInfo& info )
{
 	std::list<tagGuildSmallInfo>::iterator iter;
 	for( iter = m_lstGuildInfo.begin(); iter != m_lstGuildInfo.end(); ++iter )
 	{
 		if(iter->dwGuildID == info.dwGuildID )
 		{
 			iter->dwLeaderID = info.dwLeaderID;
 			iter->byLevel = info.byLevel;
 			iter->nGuildMembersNum = info.nGuildMembersNum;
 			_tcscpy( iter->szGuildName,info.szGuildName );
 			_tcscpy( iter->szLeaderName, info.szLeaderName );	
 
 			return;
 		}
 	}
 
 	m_lstGuildInfo.push_back( info );
}

void GuildFrame::RefreshGuildsDisplay()
{ 
	//再次校正页数
	if( m_nCurPage < 0 )
		m_nCurPage = 0;

	if( m_nCurPage >= GetPages() )
		m_nCurPage = GetPages() - 1;

	//得到要显示的listbox索引范围
	INT nBeginNum = m_nCurPage * GUILD_APPLY_MAX_ITEMS_PER_PAGE_2;			//要显示的记录的启示索引
	TCHAR szNum[X_SHORT_NAME] = {0};

	//定位到当前要显示的页的位置
	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
	{
		//从nBeginNum索引开始显示
		if( nRowIndex < nBeginNum )
			continue;
		else
			break;
	}

	ClearListBox();
	for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
	{
		//每页显示这么多记录
		if( nRowIndex >= GUILD_APPLY_MAX_ITEMS_PER_PAGE_2 )
			break;

		INT nColNum = 0;
		DWORD colour = GT_INVALID;
		std::set<DWORD> sets = GuildMgr::Inst()->GetWarGuildSet();
		//设置帮派的名字
		std::set<DWORD>::iterator it = sets.find(iter->dwGuildID);
		if (it != sets.end())
		{
			colour = RedColour;
		}

		m_pLstGuilds->SetText( nRowIndex, nColNum, iter->szGuildName ,colour);
		nColNum++;

		//设置帮派的等级
		m_pLstGuilds->SetText( nRowIndex, nColNum, _itot( iter->byLevel, szNum, 10 ) );
		nColNum++;

		//设置帮派人数
		m_pLstGuilds->SetText( nRowIndex, nColNum, _itot( iter->nGuildMembersNum, szNum, 10 ) );
		nColNum++;

		//设置帮主的名字
		m_pLstGuilds->SetText( nRowIndex, nColNum, iter->szLeaderName);
		nColNum++;

		//宣战状态
		if (colour == RedColour)
		{
			m_pLstGuilds->SetText( nRowIndex, nColNum, g_StrTable[_T("GuildWar_InWar")] ,colour);
		}
		
	}

	m_pStcPageNum->SetText( _itot( m_nCurPage+1, szNum, 10 ) );
	m_pStcPageNum->SetRefresh( TRUE );
	m_pLstGuilds->SetRefresh( TRUE );

	// 帮派宗旨
	m_pEdtTenet->Clear();
	//-----------------------清一下格式------------------------
	tstring strTenet = GuildMgr::Inst()->GetGuildTenet().c_str();
	tstring::iterator new_end;
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
	strTenet.erase(new_end, strTenet.end());
	new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
	strTenet.erase(new_end, strTenet.end());
	//---------------------------------------------------------
	m_pEdtTenet->AddText(strTenet.c_str());
	m_pEdtTenet->SetRefresh( TRUE );

	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), GuildMgr::Inst()->GetWarTimes());
	m_pStcWarTimes->SetText(szBuffer);

	//刷新工会宣战
	RefreshGuildXuanZhan();

}

void GuildFrame::ClearListBox()
{
 	if( !P_VALID(m_pLstGuilds) )
 		return;
 
 	for( int nRow = 0; nRow< GUILD_APPLY_MAX_ITEMS_PER_PAGE_2; nRow++ )
 	{
 		for( int nCol = 0; nCol <=4; nCol++ )
 		{
 			m_pLstGuilds->SetText( nRow, nCol, _T("") );
 		}
 	}
}


void GuildFrame::SelectListBoxItem( INT nSelectItem )
{
 	if( nSelectItem == m_nCurSelectItem )
 		return;
 
 	if( nSelectItem < 0 || nSelectItem >= GUILD_APPLY_MAX_ITEMS_PER_PAGE_2 )
 	{
 		//m_pStcTenet->SetInvisible( TRUE );
 		m_nCurSelectItem = GT_INVALID;
 		return;
 	}
 
 	m_nCurSelectItem = nSelectItem;

	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
	{
		if ((m_nCurSelectItem + m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE_2) == nRowIndex)
		{
			if(GuildMgr::Inst()->IsGuildWarEnemy(iter->dwGuildID))
			{
				m_pBtnPartyWarStart->SetEnable(FALSE);
				//m_pLstGuilds->SetSelectedRowTextColour(RedColour);
			}
				
			else
			{
				m_pBtnPartyWarStart->SetEnable(TRUE);
				//m_pLstGuilds->SetSelectedRowTextColour(DefaultColour);
			}
			return;
		}
	}

	if (m_nCurSelectItem >= m_lstGuildInfo.size())
	{
		m_pBtnPartyWarStart->SetEnable(FALSE);
	}
}

int GuildFrame::SearchGuild( INT nType, tstring strName )
{
	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int nIndex = 0; iter != m_lstGuildInfo.end(); ++iter, ++nIndex )
	{
		//按照帮派的名字搜索
		if( nType == 0 )
		{
			if( _tcscmp( iter->szGuildName, strName.c_str() ) == 0 )
				return nIndex;
		}
		//按照帮主的名字搜索
		else if( nType == 1 )
		{
			if( _tcscmp( iter->szLeaderName, strName.c_str() ) == 0 )
				return nIndex;
		}
	}

	return GT_INVALID;
}

DWORD GuildFrame::OnNS_DeclareGuildWar(tagNS_DeclareGuildWar* pMsg, DWORD dwParam)
{

	switch (pMsg->dwErrorCode)
	{
	case E_GuildDeclareWar_Success:
		{
			//只刷新宣战方的宣战次数
// 			DWORD localID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();
// 			if (pMsg->dwAttackGuildID == localID)
// 			{
// 
// 				GuildMgr::Inst()->SetWarTimes(MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY - pMsg->byTodayDeclareWarTimes);
// 			}
// 			
// 			DWORD realID = (localID == pMsg->dwAttackGuildID ? pMsg->dwDefenceGuildID : pMsg->dwAttackGuildID);
// 
// 			GuildMgr::Inst()->AddWarGuilds(realID);

// 			TCHAR szBuff[64] = {0};
// 			GuildListIterator iter = m_lstGuildInfo.begin();
// 			for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
// 			{
// 				if(iter->dwGuildID == realID)
// 				{
// 					_stprintf(szBuff, g_StrTable[_T("StartGuildWar")], iter->szGuildName);
// 					break;
// 				}
// 			}

// 			if(P_VALID(GetObj("DeclareGuildWar")))
// 				KillObj("DeclareGuildWar");
// 
// 			CreateObj("DeclareGuildWar", "MsgBox");
// 			TObjRef<MsgBox>("DeclareGuildWar")->Init(_T(""), szBuff, _T(""));


			RefreshGuildsDisplay();
		}
		break;
	case E_GuildDeclareWar_Gold_Not_Enough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_Gold_Not_Enough")]);
		break;
	case E_GuildDeclareWar_No_Spec_Item:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_No_Spec_Item")]);
		break;
	case E_GuildDeclareWar_Declare_Times_Full:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_Declare_Times_Full")]);
		break;
	case E_GuildDeclareWar_Not_Guild_Leader:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_Not_Guild_Leader")]);
		break;
	case E_GuildDeclareWar_Same_Guild_Full:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_Same_Guild_Full")]);
		break;
	case E_GuildDeclareWar_Guild_Not_Exist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_GuildDeclareWar_Guild_Not_Exist")]);
		break;
	default:
		break;
	}
	
	return 0;
}

DWORD GuildFrame::OnNS_GuildWarEnd(tagNS_GuildWarEnd* pMsg, DWORD dwParam)
{
	/*
 	DWORD localID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();
 	DWORD realID = (localID == pMsg->dwAttackGuildID ? pMsg->dwDefenceGuildID : pMsg->dwAttackGuildID);
 	GuildMgr::Inst()->EraseWarGuilds(realID);
	*/
 	RefreshGuildsDisplay();
	return 0;
}

DWORD GuildFrame::OnNS_SyncWarStateGuild(tagNS_SyncWarStateGuild* pMsg, DWORD dwParam)
{
	/*
 	DWORD ID;
 	for (int i =0;i< pMsg->nGuildCount; i++)
 	{
 		memcpy( &ID, &(pMsg->dwGuildID[i]), sizeof(DWORD) );
 		GuildMgr::Inst()->AddWarGuilds(ID);
 	}
	*/
 	TCHAR szBuffer[X_SHORT_NAME] = {0};
 	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), pMsg->nRemainWarDeclareTimes);
 	m_pStcWarTimes->SetText(szBuffer);

	RefreshGuildsDisplay();
	return 0;
}

VOID GuildFrame::OnStartWar()
{

	// 弹出确认对话框
	if(P_VALID(GetObj("StartGuild_War")))
		KillObj("StartGuild_War");

	CreateObj("StartGuild_War", "MsgBox");
	TObjRef<MsgBox>("StartGuild_War")->Init(_T(""), g_StrTable[_T("StartGuildWar_Check")], _T("MsgBox_WarCheck"));

}

VOID GuildFrame::UpdataFacilitiesInfoUI( const EFacilitiesType eType )
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};

	if( !P_VALID(m_pWnd) )
		return;
	if( eType < 0 || eType >= MAX_GUILD_FACILITIES_TYPE )
		return;

	// 找到升级设施信息
	tagGuildFacilitiesInfo* pInfo = NULL;
	for( int i = 0; i < MAX_GUILD_FACILITIES_TYPE; i++ )
	{
		if( eType == m_sFacilitiesInfo[i].eType )
			pInfo = &m_sFacilitiesInfo[i];
	}
	if( NULL == pInfo )
		return;

	// 所需资金
	{
		INT64 n64Silver = pInfo->nNeedFund;
		int nGold = MSilver2DBGold(n64Silver);
		int nSilver = MSilver2DBSilver(n64Silver);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), pInfo->nNeedFund/*nGold*/);
		m_pStcConstrGold[eType]->SetText(szBuffer);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nSilver);
		m_pStcConstrSilver[eType]->SetText(szBuffer);

	}
	// 所需资财
	{
		tstringstream stream;
		stream<<pInfo->nMaterial;
		m_pStcConstrResource[eType]->SetText( stream.str().c_str() );
	}
	// 等级标签
// 	{
// 		tstringstream stream;
// 		stream<<_T("Guild_Facilitie")<<eType;
// 		TCHAR szBuff[X_SHORT_NAME] = { 0 };
// 		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("GuildUpgrade_Level")], g_StrTable[stream.str().c_str()] );
// 		m_pStcConstrLevel[eType]->SetText( szBuff );
// 	}
	// 等级
	{
		tstringstream stream;
		stream<<pInfo->byLevel;
		m_pStcConstrLevel[eType]->SetText( stream.str().c_str() );
	}
	// 说明
// 	{
// 		tstringstream stream;
// 		stream<<_T("GuildUpgrade_Tips")<<eType;
// 		m_pSt_Tips->SetText( g_StrTable[stream.str().c_str()] );
// 	}
	// 进度
	m_pProConstResearch[eType]->SetMaxValue( pInfo->nFulfill );
	m_pProConstResearch[eType]->SetValue( pInfo->nProgress, true );

	const tagGuildMember* pLpInfo = GuildMgr::Inst()->GetLpInfo();
	if(P_VALID(pLpInfo))
	{
		if (pLpInfo->eGuildPos == EGMP_HuiZhang || pLpInfo->eGuildPos == EGMP_FuHuiZhang)
		{
			for (int i = 0; i< EFT_End; i++ )
			{
				m_pBtnLevelUp[i]->SetEnable(TRUE);
			}
		}
		else
		{
			for (int i = 0; i< EFT_End; i++ )
			{
				m_pBtnLevelUp[i]->SetEnable(FALSE);
			}
		}		
	}	
	

	m_pGuildConstruction->SetRefresh();
}

DWORD GuildFrame::OnNS_GetGuildUpgradeInfo( tagNS_GetGuildUpgradeInfo* pCmd, DWORD )
{
	memcpy( m_sFacilitiesInfo, pCmd->sFacilitiesInfo, sizeof(pCmd->sFacilitiesInfo) );

	UpdataFacilitiesInfoUI( EFT_Guild );
	UpdataFacilitiesInfoUI( EFT_Fund );
	UpdataFacilitiesInfoUI( EFT_Material );
	UpdataFacilitiesInfoUI( EFT_Academy );
	return 0;
}

DWORD GuildFrame::OnNS_UpdateFacilitiesInfo( tagNS_UpdateFacilitiesInfo* pCmd, DWORD )
{
	ASSERT( pCmd->sFacilitiesInfo.eType >= 0 && pCmd->sFacilitiesInfo.eType < MAX_GUILD_FACILITIES_TYPE );

	for( int i = 0; i < MAX_GUILD_FACILITIES_TYPE; i++ )
	{
		if( m_sFacilitiesInfo[i].eType == pCmd->sFacilitiesInfo.eType )
		{
			memcpy( &m_sFacilitiesInfo[i], &pCmd->sFacilitiesInfo, sizeof(pCmd->sFacilitiesInfo) );
			break;
		}
	}

	UpdataFacilitiesInfoUI( EFT_Guild );
	UpdataFacilitiesInfoUI( EFT_Fund );
	UpdataFacilitiesInfoUI( EFT_Material );
	UpdataFacilitiesInfoUI( EFT_Academy );

	return 0;
}

VOID GuildFrame::HandInItems(const EFacilitiesType eType)
{
	// 检查帮派状态
	if( GuildMgr::Inst()->IsInSpecState( EGDSS_Shortage ) ||
		GuildMgr::Inst()->IsInSpecState( EGDSS_Distress ) ||
		GuildMgr::Inst()->IsInSpecState( EGDSS_Chaos ) ||
		GuildMgr::Inst()->IsInSpecState( EGDSS_Warfare ) )
	{
		GuildMgr::Inst()->ShowGuildErrorMsg(E_Guild_State_Limit);
		return;
	}

	CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame( _T("CombatSys") );
	ASSERT( P_VALID( pCombatSys ) && GT_INVALID != pCombatSys->GetCurTargetID() );

	tagNC_HandInItems cmd;
	cmd.dwNPCID = GT_INVALID;
	cmd.eType = eType;
	m_pSession->Send( &cmd );
	
}

DWORD GuildFrame::OnEventKickGuildMember(tagGameEvent *pEvent)
{
	OnClickedKickMember();
	return 0;
}

VOID GuildFrame::RefreshAlliance(EGodMiracle eType)
{
	if( !P_VALID(m_pLstAlliance) )
		return;

	needRefresh = false;

	const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(eType);
	if (!P_VALID(pGodInfo))
		return;

	int nAttackNum = 0;
	int nDefenceNum = 0;
	for (int i =0; i< MAX_ATTACK_ALLIANCE_GUILD;i++)
	{
		if (pGodInfo->dwAttAlliance[i] == 0 || pGodInfo->dwAttAlliance[i] == GT_INVALID)
			continue;

		nAttackNum++;
	}

	// 防御者同盟
	for (int i =0; i< MAX_DEFANCE_ALLIANCE_GUILD;i++)
	{
		if (pGodInfo->dwDefAlliance[i] == 0 || pGodInfo->dwDefAlliance[i] == GT_INVALID)
			continue;

		nDefenceNum++;
	}


 	m_pLstAlliance->Clear();
 	INT nColNum = 0;
 	TCHAR szNum[X_SHORT_NAME] = {0};
 	TCHAR szBuffer[X_SHORT_NAME] = {0};
	std::map<EGodMiracle,tagGuildAlliance> map = GuildMgr::Inst()->GetGuildAlliance();
	std::map<EGodMiracle,tagGuildAlliance>::iterator iter = map.begin(); // 这里的map中实际上只有一个元素，所以每次找begin就可以了
	if(iter != map.end())
	{
		tagGuildAlliance alliance;
		memcpy( &alliance, &(iter->second), sizeof(tagGuildAlliance) );

		for( int i = 0, nRowIndex = 0; i < alliance.dwGuildCnt; ++nRowIndex ,++i)
		{
			INT nColNum = 0;
			tstring ss = GuildMgr::Inst()->GetGuildName(alliance.dwGuildInfo[i].dwGuildID);
			if (ss == _T(""))
			{
				needRefresh = true;
				continue;
			}
			m_pLstAlliance->SetText( nRowIndex, nColNum, ss.c_str());
			nColNum++;

			//设置帮派的等级
			m_pLstAlliance->SetText( nRowIndex, nColNum, _itot( alliance.dwGuildInfo[i].nLevel, szNum, 10 ) );
			nColNum++;

			//设置帮派人数
			m_pLstAlliance->SetText( nRowIndex, nColNum, _itot( alliance.dwGuildInfo[i].nNum, szNum, 10 ) );

		}

		
		int numRemain =0;
		if (alliance.eGuildCapType == EGCT_Defend)
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),nDefenceNum );
			m_pStcNowAllies->SetText(szBuffer);
			numRemain = MAX_DEFANCE_ALLIANCE_GUILD - nDefenceNum;
		}
		else if (alliance.eGuildCapType == EGCT_Capture)
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),nAttackNum );
			m_pStcNowAllies->SetText(szBuffer);
			numRemain = MAX_ATTACK_ALLIANCE_GUILD - nAttackNum;
		}
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),numRemain );
		m_pStcRemainAllies->SetText(szBuffer);
		m_pStcNowAllies->SetRefresh(TRUE);
		m_pStcRemainAllies->SetRefresh(TRUE);
	}
	else
	{
		int numRemain =0;
		const tagGuildBase* pInfo = GuildMgr::Inst()->GetGuildInfo();
		if (!P_VALID(pInfo))
			return;

		if (pInfo->dwID == pGodInfo->sCapGuild.dwGuildID) // 如果是进攻帮派
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),nAttackNum );
			m_pStcNowAllies->SetText(szBuffer);
			numRemain = MAX_ATTACK_ALLIANCE_GUILD - nAttackNum;
		}
		else if (pInfo->dwID == pGodInfo->sDefenceGuild.dwGuildID) // 如果是防御帮派
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),nDefenceNum );
			m_pStcNowAllies->SetText(szBuffer);
			numRemain = MAX_DEFANCE_ALLIANCE_GUILD - nDefenceNum;
		}
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),numRemain );
		m_pStcRemainAllies->SetText(szBuffer);
		m_pStcNowAllies->SetRefresh(TRUE);
		m_pStcRemainAllies->SetRefresh(TRUE);
	}

}
DWORD GuildFrame::OnEventOpenGuildAllianceList(tagSureAllianceEvent *pEvent)
{
	m_eTypeEx = pEvent->eType;
	RefreshAlliance(pEvent->eType);

	if (m_nCurChooseRenmant >= EGM_NH && m_nCurChooseRenmant < EGM_MAX)
		UpdateSelectGodInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));

	return 0;
}

VOID GuildFrame::SureAlliance()
{
 	std::map<EGodMiracle,tagGuildAlliance> map = GuildMgr::Inst()->GetGuildAlliance();
 	std::map<EGodMiracle,tagGuildAlliance>::iterator iter = map.begin();
	if(iter != map.end())
	{
		int nSelectRow = m_pLstAlliance->GetCurSelectedRow();
		if ( nSelectRow < MAX_ALLIANCE)
		{
			tagNC_ConfirmAllianceGuild cmd;
			cmd.dwGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
			cmd.dwGuildID = iter->second.dwGuildInfo[nSelectRow].dwGuildID;
			m_pSession->Send(&cmd);

			return;
		}

	}

}

VOID GuildFrame::SelectListBoxAlly(INT nSelectItem)
{
	if( nSelectItem == m_nCurAlly )
		return;

	if( nSelectItem < 0 || nSelectItem >= GuildMgr::Inst()->GetGuildAlliance().size())
	{
		m_nCurAlly = GT_INVALID;
		return;
	}

	m_nCurAlly = nSelectItem;

}

DWORD GuildFrame::OnEventMsgBoxWarCheck( tagMsgBoxEvent* pGameEvent )
{
	if(pGameEvent->eResult == MBF_OK)
	{
		GuildListIterator iter = m_lstGuildInfo.begin();
		for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
		{
			if ((m_nCurSelectItem + m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE_2) == nRowIndex)
			{
				tagNC_DeclareGuildWar cmd;
				cmd.dwDefenceGuildID =  iter->dwGuildID;
				cmd.dwAttackGuildID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();

				if (cmd.dwDefenceGuildID == cmd.dwAttackGuildID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantWarToYourself")]);
					return 0;
				}
				m_pSession->Send( &cmd );

				return 0;
			}
		}
	}
	return 0;
}

DWORD GuildFrame::OnEventApplyAttackAlly(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		const tagGodInfo *pGodInfo = GuildMgr::Inst()->GetGodMiracleInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
		if (P_VALID(pGodInfo))
		{

			if(pGodInfo->eGodState == EGMS_Closed)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodState_Closed")]);
				return 0;
			}

			tagNC_ApplyForAlliance cmd;
			cmd.dwGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
			cmd.eGuildCapType = EGCT_Capture;
			m_pSession->Send(&cmd);
		}

	}
	return 0;
}

DWORD GuildFrame::OnEventApplyDefenceAlly(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		tagNC_ApplyForAlliance cmd;
		cmd.dwGodID = static_cast<EGodMiracle>(m_nCurChooseRenmant);
		cmd.eGuildCapType = EGCT_Defend;
		m_pSession->Send(&cmd);
	}
	return 0;
}

DWORD GuildFrame::OnEventMoneyConfirmApply(tagMoneyConfirmApply *pGameEvent)
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),pGameEvent->dwMoney );
	m_pStcNowBegMoney->SetText(szBuffer);
	return 0;
}

DWORD GuildFrame::OnEventTurnOverHuiZhang(tagGameEvent *pGameEvent)
{
	OnClickedTurnoverLeader();
	return 0;
}

VOID GuildFrame::RefreshAllianceDisplay()
{
	if (m_nCurChooseRenmant >= EGM_NH && m_nCurChooseRenmant < EGM_MAX)
		UpdateSelectGodInfo(static_cast<EGodMiracle>(m_nCurChooseRenmant));
}

DWORD GuildFrame::OnNS_ActiveGuildSkill( tagNS_ActiveGuildSkill* pCmd, DWORD )
{
	if ( E_Success == pCmd->dwErrorCode)
	{
		m_pIbnActiveItem->RefreshItem();
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Active_GuildSkill_Success")]);
		m_strActiveItem.clear();
		GuildMgr::Inst()->SendGetGuildSkillInfo();
	}

	return 0;
}

VOID GuildFrame::RefreshGuildXuanZhan()
{
	m_eGuildFrameState = EGFS_War;
	m_pGuildWar->SetInvisible(FALSE);
	m_pGuildConstruction->SetInvisible(TRUE);
	m_pSkillPatch->SetInvisible(TRUE);
	m_pGuildMemberPatch->SetInvisible(TRUE);
	m_pRemnantStatePatch->SetInvisible(TRUE);
	/*GuildMgr::Inst()->SendGetAllGodMiracleInfo();*/

	m_nCurPage = 0;

	m_pWndActiveSkill->SetInvisible(TRUE);
	m_strActiveItem.clear();

	m_pWndAlliance->SetInvisible(TRUE);
}