#include "stdafx.h"
#include "RoleMgr.h"
#include "HelpPop.h"
#include "QuestMgr.h"
#include "LocalPlayer.h"
#include "ItemEvent.h"
#include "..\WorldDefine\quest.h"

const int	HELP_WND_W = 171;
const int	HELP_WND_H = 120;
const TCHAR* STR_HELP_WND_NAME = _T("HelpPop_");
const char*	HELP_WND_NAME = "HelpPop_";
static BOOL g_bFinishInit = FALSE;
HelpPop::HelpPop( void ) : m_trunk(this)
{

}

HelpPop::~HelpPop( void )
{

}

void HelpPop::Init()
{
	if ( FALSE == g_bFinishInit )
	{
		LoadFromFile();
		g_bFinishInit = TRUE;
	}

	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPLevel"),	(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnUpdateLPLevel));
	m_pFrameMgr->RegisterEventHandle(_T("tagInitLPAttEvent"),	(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnEvent_LPAttIsOK));
	m_pFrameMgr->RegisterEventHandle(_T("CombatMsg_ItemAdd"),	(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnCombatMsg_ItemAdd));
	TObjRef<NetCmdMgr>()->Register("NS_AddQuest",				(NETMSGPROC)m_trunk.sfp2(&HelpPop::NetRecvMessage_AddQuest),  _T("NS_AddQuest"));
	TObjRef<NetCmdMgr>()->Register("NS_CompleteQuest",			(NETMSGPROC)m_trunk.sfp2(&HelpPop::NetRecvMessage_CompleteQuest), _T("NS_CompleteQuest"));
}

void HelpPop::Destroy()
{
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPLevel"),		(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnUpdateLPLevel));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagInitLPAttEvent"),	(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnEvent_LPAttIsOK));
	m_pFrameMgr->UnRegisterEventHandler(_T("CombatMsg_ItemAdd"),	(FRAMEEVENTPROC)m_trunk.sfp1(&HelpPop::OnCombatMsg_ItemAdd));
	TObjRef<NetCmdMgr>()->UnRegister("NS_AddQuest",					(NETMSGPROC)m_trunk.sfp2(&HelpPop::NetRecvMessage_AddQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_CompleteQuest",			(NETMSGPROC)m_trunk.sfp2(&HelpPop::NetRecvMessage_CompleteQuest));

	vector<tagHelpPopFrame>::iterator iter = m_vecHelpInfo.begin();
	for( ; iter != m_vecHelpInfo.end(); iter++)
		TObjRef<GUISystem>()->AddToDestroyList(iter->pWnd);
	m_vecHelpInfo.clear();

}
HelpPop* HelpPop::Inst()
{
	static HelpPop inst;
	return &inst;
}

void HelpPop::HelpPopFrame(const tagHelpInfo &HelpInfo)
{
	TCHAR szWndName[X_LONG_NAME]={0};
	_stprintf(szWndName, _T("%s%s%d"), _T("\\desktop\\"), STR_HELP_WND_NAME, HelpInfo.dwID);
	GUIStaticEx* pWnd = NULL;
	pWnd = (GUIStaticEx*)TObjRef<GUISystem>()->GetWnd(szWndName);
	if( P_VALID(pWnd) )
		return;

	char szName[32]= {0};
	sprintf(szName, "%s%d", HELP_WND_NAME, HelpInfo.dwID);
	XmlElement ele;
	ele.SetAttribute("Name", szName);
	ele.SetAttribute("ClassName", "CGUIStaticEx");
	ele.SetAttribute("Size_x", HELP_WND_W);
	ele.SetAttribute("Size_y", HELP_WND_H);
	ele.SetAttribute("AlignOffset_x", HelpInfo.nLogic_x);
	ele.SetAttribute("AlignOffset_y", HelpInfo.nLogic_y);
	ele.SetAttribute("AlignType", TObjRef<Util>()->UnicodeToUnicode8(HelpInfo.strAlignType.c_str()));
	ele.SetAttribute("AlignControl", TObjRef<Util>()->UnicodeToUnicode8(HelpInfo.strAlignControl.c_str()));
	ele.SetAttribute("Font", TObjRef<Util>()->UnicodeToUnicode8(g_StrTable[_T("Font_MSYH")]));
	ele.SetAttribute("FontWidth", 0);
	ele.SetAttribute("FontHeight", 16);
	ele.SetAttribute("TextRect_left", 17);
	ele.SetAttribute("TextRect_top", 17);		
	ele.SetAttribute("TextRect_right", HELP_WND_W-17);
	ele.SetAttribute("TextRect_bottom", HELP_WND_H-17);
	ele.SetAttribute("TextAlign", "LeftTop");
	ele.SetAttribute("AlphaWrite", EGUIAW_NoWrite);
	ele.SetAttribute("TextColor", 0xFFFFFFFF);
	ele.SetAttribute("ShadowColor", 0xFF290404);
	ele.SetAttribute("PicColor", 0x88FFFFFF);
	ele.SetAttribute("PicFileName", TObjRef<Util>()->UnicodeToUnicode8(HelpInfo.strPicName.c_str()));
	ele.SetAttribute("Resizable", "yes");
	ele.SetAttribute("CanMoveOutOfFather", "yes");
	ele.SetAttribute("ReadOnly", "yes");
	ele.SetAttribute("AlphaWrite", EGUIAW_Write);
	ele.SetAttribute("Movable", "yes");
	if( HelpInfo.dwID != 1 && HelpInfo.dwID != 2 && HelpInfo.byType != HPT_LevelUp )
		ele.SetAttribute("TopMost", "yes");
	pWnd = (GUIStaticEx*)TObjRef<GUISystem>()->CreateWnd(_T("\\desktop"), &ele);
	if( !P_VALID(pWnd) )
		return;
	tagHelpPopFrame PopFrame;
	PopFrame.pWnd = pWnd;
	PopFrame.dwCurTime = 0;
	PopFrame.dwMaxTime = HelpInfo.dwShowTime;
	int nPos = HelpInfo.strAlignControl.find_first_of('\\', 10);
	if( nPos != HelpInfo.strAlignControl.npos )
		PopFrame.strAlignWnd = HelpInfo.strAlignControl.substr(0, nPos);
	else
		PopFrame.strAlignWnd = HelpInfo.strAlignControl;
	m_vecHelpInfo.push_back(PopFrame);
	pWnd->SetText(HelpInfo.strInfo.c_str());

	tagPoint size = pWnd->GetSize();
	size.y = pWnd->GetTextTotalHeight() + 17;
	pWnd->SetResizable(TRUE);
	pWnd->Resize( size );
	pWnd->SetResizable(FALSE);
	pWnd->SetRefresh(TRUE);

	GUIWnd *pAlignWnd = TObjRef<GUISystem>()->GetWnd(HelpInfo.strAlignControl.c_str());
	if( P_VALID(pAlignWnd) && !pAlignWnd->IsInvisible() )
		pWnd->SetInvisible(false);
	else
		pWnd->SetInvisible(true);
}

VOID HelpPop::LoadFromFile()
{
	tstring strPath =  _T("data\\local\\zhCN\\attdata\\show_pic.xml");

	list<tstring> HelpInfoList;
	TObjRef<VarContainer> con = CreateObj( "PopHelpInfo", "VarContainer" );
	if(!con->Load("VFS_System", strPath.c_str(), "id", &HelpInfoList))
	{
		IMSG(_T("Load file HelpInfo.xml failed\r\n"));
		return;
	}
	list<tstring>::iterator end = HelpInfoList.end();
	for (list<tstring>::iterator iter = HelpInfoList.begin(); iter != end; ++iter)
	{
		tagHelpInfo HelpInfo;
		HelpInfo.dwID = con->GetDword(_T("id"), iter->c_str(), 0);
		HelpInfo.byType = (BYTE)con->GetInt(_T("Type"), iter->c_str(), GT_INVALID);
		HelpInfo.dwCondition = con->GetDword(_T("Qualification"), iter->c_str(), GT_INVALID);
		HelpInfo.strAlignType = con->GetString(_T("AlignType"), iter->c_str(), _T(""));
		HelpInfo.strInfo= con->GetString(_T("Text"), iter->c_str(), _T(""));
		HelpInfo.strAlignControl= con->GetString(_T("AlignControl"), iter->c_str(), _T(""));
		HelpInfo.strPicName= con->GetString(_T("Picname"), iter->c_str(), _T(""));
		HelpInfo.dwShowTime = con->GetDword(_T("time"), iter->c_str(), GT_INVALID);
		HelpInfo.nLogic_x = con->GetInt(_T("logic_x"), iter->c_str(), GT_INVALID);
		HelpInfo.nLogic_y = con->GetInt(_T("logic_y"), iter->c_str(), GT_INVALID);
		m_mapHelpInfo.insert(make_pair(HelpInfo.dwID, HelpInfo));
	}
	KillObj("PopHelpInfo");
}

void HelpPop::Update()
{
	vector<tagHelpPopFrame>::iterator iter = m_vecHelpInfo.begin();
	for( ; iter != m_vecHelpInfo.end(); )
	{
		GUIWnd *pAlignWnd = TObjRef<GUISystem>()->GetWnd(iter->strAlignWnd.c_str());
		if( P_VALID(pAlignWnd) && !pAlignWnd->IsInvisible() )
			iter->pWnd->SetInvisible(false);
		else
			iter->pWnd->SetInvisible(true);
		iter->pWnd->Align();
		iter->dwCurTime += Kernel::Inst()->GetDeltaTimeDW();
		if( iter->dwCurTime > iter->dwMaxTime )
		{
			TObjRef<GUISystem>()->AddToDestroyList(iter->pWnd);
			iter = m_vecHelpInfo.erase(iter);
		}
		else
			++iter;
	}
}

DWORD HelpPop::OnUpdateLPLevel(tagUpdateLPLevel* pEvent)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	map<DWORD, tagHelpInfo>::iterator iter = m_mapHelpInfo.begin();
	for( ; iter != m_mapHelpInfo.end(); iter++)
	{
		if(pLp->GetRoleLevel() == iter->second.dwCondition )
			HelpPopFrame(iter->second);
	}
	return 0;
}

DWORD HelpPop::NetRecvMessage_AddQuest( tagNS_AddQuest* pMsg, DWORD pParam )
{
	QuestQuery* pQQ = QuestMgr::Inst()->GetQuery();
	if(!P_VALID(pQQ))
		return 0;
	map<DWORD, tagHelpInfo>::iterator iter = m_mapHelpInfo.begin();
	for( ; iter != m_mapHelpInfo.end(); iter++ )
	{
		if( ( pMsg->u16QuestID == iter->second.dwCondition && iter->second.byType == HPT_AcceptQuest ) || 
			( pMsg->u16QuestID == iter->second.dwCondition && pQQ->QuestCanComplete(pMsg->u16QuestID) ) )
		{
			HelpPopFrame(iter->second);
			break;
		}
	}
	return 0;
}

DWORD HelpPop::OnEvent_LPAttIsOK( tagGameEvent* pEvent )
{
	QuestQuery* pQQ = QuestMgr::Inst()->GetQuery();
	if(!P_VALID(pQQ))
		return 0;
	const QuestQuery::QuestsMap& quests = pQQ->GetCurrentQuests();
	for( QuestQuery::QuestsMap::const_iterator iter = quests.begin(); iter!=quests.end(); ++iter )
	{
		map<DWORD, tagHelpInfo>::iterator iiter = m_mapHelpInfo.begin();
		for( ; iiter != m_mapHelpInfo.end(); iiter++)
		{
			if( iter->first == iiter->second.dwCondition && pQQ->QuestCanComplete(iter->first) )
				HelpPopFrame(iiter->second);
		}
	}
	return 0;
}

DWORD HelpPop::NetRecvMessage_CompleteQuest( tagNS_CompleteQuest* pMsg, DWORD pParam )
{
	if( pMsg->dwErrorCode == E_Success )
	{
		QuestQuery* pQQ = QuestMgr::Inst()->GetQuery();
		if(!P_VALID(pQQ))
			return 0;
		map<DWORD, tagHelpInfo>::iterator iter = m_mapHelpInfo.begin();
		for( ; iter != m_mapHelpInfo.end(); iter++)
		{
			if( pMsg->u16QuestID == iter->second.dwCondition && iter->second.byType == HPT_DoneQuest )
				HelpPopFrame(iter->second);
		}
	}
	return 0;
}

DWORD HelpPop::OnCombatMsg_ItemAdd( tagCombatMsgForItemChange* pEvent )
{
	map<DWORD, tagHelpInfo>::iterator iter = m_mapHelpInfo.begin();
	for( ; iter != m_mapHelpInfo.end(); iter++ )
	{
		if( pEvent->dwTypeID == iter->second.dwCondition && iter->second.byType == HPT_GetItem )
			HelpPopFrame(iter->second);
	}
	return 0;
}
