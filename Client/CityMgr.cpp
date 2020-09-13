#include "stdafx.h"
#include "..\WorldDefine\msg_city.h"
#include "..\WorldDefine\msg_city_struggle.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\currency_define.h"
#include "CityEvent.h"
#include "CityMgr.h"
#include "CityFrame.h"
#include "CityWarInfoFrame.h"
#include "CityTaxRateFrame.h"
#include "CityPickUpTaxFrame.h"
#include "CityWarApplyFrame.h"
#include "CityWarApplyMoneyFrame.h"
#include "CityWarAssistConfirmFrame.h"
#include "CityWarStateFrame.h"
#include "CityWarResultFrame.h"
#include "WorldFrame.h"
#include "QuestMgr.h"
#include "GuildMgr.h"
#include "MapMgr.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ChatFrame.h"
#include "CurrencyMgr.h"


CityMgr * CityMgr::Inst()
{
	static CityMgr Inst;
	return &Inst;
}

CityMgr::~CityMgr()
{

}

void CityMgr::Init()
{
	m_pGameFrameMgr->RegisterEventHandle(_T("Open_City"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCity)));
	m_pGameFrameMgr->RegisterEventHandle(_T("Open_City_By_ID"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityByID)));
	m_pGameFrameMgr->RegisterEventHandle(_T("Open_City_Set_Rate"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCitySetRate)));
	m_pGameFrameMgr->RegisterEventHandle(_T("Open_City_Pick_Up_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityPickUpTax)));
	m_pGameFrameMgr->RegisterEventHandle(_T("Open_City_War_Apply"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityWarApply)));
	m_pGameFrameMgr->RegisterEventHandle(_T("ViewCityInfoEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnMsgBoxEventViewCityInfo)));

	m_pNetCmdMgr->Register("NS_GetCityInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetCityInfo)), _T("NS_GetCityInfo"));
	m_pNetCmdMgr->Register("NS_GetAppointedCityAtt", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetAppointedCityAtt)), _T("NS_GetAppointedCityAtt"));
	m_pNetCmdMgr->Register("NS_SetTaxRate", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetSetTaxRate)), _T("NS_SetTaxRate"));
	m_pNetCmdMgr->Register("NS_GetSigupInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetApplyInfo)), _T("NS_GetSigupInfo"));
	m_pNetCmdMgr->Register("NS_UpgradeProlificacy", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetUpgradeProlificacy)), _T("NS_UpgradeProlificacy"));
	m_pNetCmdMgr->Register("NS_GetRoleInitState_City", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetRoleInitStateCity)), _T("NS_GetRoleInitState_City"));
	m_pNetCmdMgr->Register("NS_ConfirmDefenceForce", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetConfirmDefenceForce)), _T("NS_ConfirmDefenceForce"));
	m_pNetCmdMgr->Register("NS_CityStruggleInfo",	(NETMSGPROC)m_Trunk.sfp2(&CityMgr::OnNetCityStruggleInfo),		_T("NS_CityStruggleInfo"));	
	m_pNetCmdMgr->Register("NS_CityStruggleInfoChange",	(NETMSGPROC)m_Trunk.sfp2(&CityMgr::OnNetCityStruggleInfoChange),	_T("NS_CityStruggleInfoChange"));

	m_mapName2CityID.clear();
	m_mapProduct.clear();

	const TCHAR* szPath = _T("data\\system\\attdata\\city_proto.xml");
	list<tstring> CityProtoList;
	TObjRef<VarContainer> CityCon = CreateObj( "City", "VarContainer" );
	if(!CityCon->Load("VFS_System", szPath, "id", &CityProtoList))
		IMSG(_T("Load file city_proto.xml failed\r\n"));

	list<tstring>::iterator end = CityProtoList.end();
	for (list<tstring>::iterator iter = CityProtoList.begin(); iter != end; ++iter)
	{
		DWORD dwCityID = CityCon->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tstring strMapName = CityCon->GetString(_T("shell_city"), (*iter).c_str(), _T(""));
		m_mapName2CityID.insert(std::make_pair(strMapName, dwCityID));
		strMapName = CityCon->GetString(_T("kernel_city"), (*iter).c_str(), _T(""));
		m_mapName2CityID.insert(std::make_pair(strMapName, dwCityID));
		DWORD dwMinSigfare = CityCon->GetInt(_T("min_sigfare"), iter->c_str(), 0);
		m_mapCityID2MinSigfare.insert(std::make_pair(dwCityID, dwMinSigfare));

		std::vector<tagCityProduct> vecCityPro;
		// 读取产出相关
		tagCityProduct cp;
		cp.dwTypeID = CityCon->GetDword(_T("item_type_1"), iter->c_str(), GT_INVALID);
		cp.nNum = CityCon->GetInt(_T("item_num_1"), iter->c_str(), 0);
		cp.nLimit = CityCon->GetInt(_T("item_limit_1"), iter->c_str(), 0);
		vecCityPro.push_back(cp);

		cp.dwTypeID = CityCon->GetDword(_T("item_type_2"), iter->c_str(), GT_INVALID);
		cp.nNum = CityCon->GetInt(_T("item_num_2"), iter->c_str(), 0);
		cp.nLimit = CityCon->GetInt(_T("item_limit_2"), iter->c_str(), 0);
		vecCityPro.push_back(cp);

		cp.dwTypeID = CityCon->GetDword(_T("item_type_3"), iter->c_str(), GT_INVALID);
		cp.nNum = CityCon->GetInt(_T("item_num_3"), iter->c_str(), 0);
		cp.nLimit = CityCon->GetInt(_T("item_limit_3"), iter->c_str(), 0);
		vecCityPro.push_back(cp);

		cp.dwTypeID = CityCon->GetDword(_T("item_type_4"), iter->c_str(), GT_INVALID);
		cp.nNum = CityCon->GetInt(_T("item_num_4"), iter->c_str(), 0);
		cp.nLimit = CityCon->GetInt(_T("item_limit_4"), iter->c_str(), 0);
		vecCityPro.push_back(cp);

		cp.dwTypeID = CityCon->GetDword(_T("item_type_5"), iter->c_str(), GT_INVALID);
		cp.nNum = CityCon->GetInt(_T("item_num_5"), iter->c_str(), 0);
		cp.nLimit = CityCon->GetInt(_T("item_limit_5"), iter->c_str(), 0);
		vecCityPro.push_back(cp);

		m_mapProduct.insert(std::make_pair(dwCityID, vecCityPro));
	}
	KillObj("City");

	m_mapCityID2Name.clear();
	tstring strCityNamePath = g_strLocalPath + _T("\\attdata\\city_name.xml");
	list<tstring> CityNameList;
	TObjRef<VarContainer> CityNameCon = CreateObj("CityName", "VarContainer");
	if (!CityNameCon->Load("VFS_System", strCityNamePath.c_str(), "id", &CityNameList))
		IMSG(_T("Load file city_name.xml failed\r\n"));
	list<tstring>::iterator iend = CityNameList.end();
	for (list<tstring>::iterator iter = CityNameList.begin(); iter != iend; ++iter)
	{
		tagCityNameAndIcon tci;
		DWORD dwCityID = CityNameCon->GetDword(_T("id"), iter->c_str(), GT_INVALID);
		tci.strCityName = CityNameCon->GetString(_T("name"), iter->c_str(), _T(""));
		// 路径
		tstring strPath;
		strPath = CityNameCon->GetString(_T("icon"), iter->c_str(), _T(""));
		tci.strIconPath = _T("data\\ui\\city\\");
		tci.strIconPath.append(strPath);
		tci.strIconPath.append(_T(".bmp"));
		tci.strGodName = CityNameCon->GetString(_T("PatronGod"), iter->c_str(), _T(""));
		m_mapCityID2Name.insert(std::make_pair(dwCityID, tci));

	}
	KillObj("CityName");

	//发送消息初始化税收商店有关信息
	tagNC_GetRoleInitState cmd;
	cmd.eType = ERIT_City;
	m_pSession->Send(&cmd);

	// 人肉创建
	//m_pGameFrameMgr->CreateFrame(_T("World"), _T("CityWarInfo"), _T("CityWarInfoFrame"), 0);
	m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("CityWarStateFrame"), _T("CityWarStateFrame"), 0);
	m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("CityWarResultFrame"), _T("CityWarResultFrame"), 0);
}

void CityMgr::Destroy()
{
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Open_City"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCity)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Open_City_By_ID"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityByID)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Open_City_Set_Rate"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCitySetRate)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Open_City_Pick_Up_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityPickUpTax)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Open_City_War_Apply"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnEventOpenCityWarApply)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("ViewCityInfoEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityMgr::OnMsgBoxEventViewCityInfo)));

	m_pNetCmdMgr->UnRegister("NS_GetCityInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetCityInfo)));
	m_pNetCmdMgr->UnRegister("NS_GetAppointedCityAtt", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetAppointedCityAtt)));
	m_pNetCmdMgr->UnRegister("NS_SetTaxRate", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetSetTaxRate)));
	m_pNetCmdMgr->UnRegister("NS_GetSigupInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetApplyInfo)));
	m_pNetCmdMgr->UnRegister("NS_UpgradeProlificacy", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetUpgradeProlificacy)));
	m_pNetCmdMgr->UnRegister("NS_GetRoleInitState_City", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetGetRoleInitStateCity)));
	m_pNetCmdMgr->UnRegister("NS_ConfirmDefenceForce", (NETMSGPROC)(m_Trunk.sfp2(&CityMgr::OnNetConfirmDefenceForce)));
	m_pNetCmdMgr->UnRegister("NS_CityStruggleInfo",				(NETMSGPROC)m_Trunk.sfp2(&CityMgr::OnNetCityStruggleInfo));	
	m_pNetCmdMgr->UnRegister("NS_CityStruggleInfoChange",			(NETMSGPROC)m_Trunk.sfp2(&CityMgr::OnNetCityStruggleInfoChange));	

	if (P_VALID(GetObj("ViewCityInfoMsgBox")))
		KillObj("ViewCityInfoMsgBox");
}

VOID CityMgr::SendGetCityInfo(DWORD dwNPCID)
{
	if (!m_bSendGetCityInfo)
	{
		tagNC_GetCityInfo cmd;
		cmd.dwNPCID = dwNPCID;
		m_pSession->Send(&cmd);
		m_bSendGetCityInfo = true;
	}
}

VOID CityMgr::SendGetAppointedCityAtt( DWORD dwNPCID, ECityAttType eType )
{
	tagNC_GetAppointedCityAtt cmd;
	cmd.dwNPCID = dwNPCID;
	cmd.eCityAttType = eType;
	m_pSession->Send(&cmd);
}

VOID CityMgr::SendSetTaxRate( DWORD dwNPCID, INT32 nTaxRate )
{
	tagNC_SetTaxRate cmd;
	cmd.dwNPCID = dwNPCID;
	cmd.nTaxRate = nTaxRate;
	m_pSession->Send(&cmd);
	m_bSendSetTaxRate = true;
}

VOID CityMgr::SendTransformTax( DWORD dwNPCID, ETaxTransformType eType, INT32 nTax )
{
	tagNC_TransformTaxation cmd;
	cmd.dwNPCID = dwNPCID;
	cmd.eTransType = eType;
	cmd.nTaxation = nTax;
	m_pSession->Send(&cmd);
}

VOID CityMgr::SendGetApplyInfo(DWORD dwNPCID)
{
	tagNC_GetSigupInfo cmd;
	cmd.dwNPCID = dwNPCID;
	m_pSession->Send(&cmd);
}

//----------------------------------

CityMgr::CityMgr()
: m_Trunk(this)
, m_bSendGetCityInfo(false)
, m_bSendGetLastRateSetTime(false)
, m_bSendGetTax(false)
, m_bSendSetTaxRate(false)
{

}

DWORD CityMgr::OnEventOpenCity(tagGameEvent *pEvent)
{

	if (P_VALID(GetObj("ViewCityInfoMsgBox")))
		KillObj("ViewCityInfoMsgBox");

	CreateObj("ViewCityInfoMsgBox", "MsgBox");
	TObjRef<MsgBox>("ViewCityInfoMsgBox")->Init(_T(""), g_StrTable[_T("City_Text04")], _T("ViewCityInfoEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
	return 0;
}


DWORD CityMgr::OnEventOpenCityByID(tagOpenCityEvent *pEvent)
{
	CityFrame* pFrame = (CityFrame*)m_pGameFrameMgr->GetFrame(_T("City"));
	if (!P_VALID(pFrame))
	{
		m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("City"), _T("CityFrame"), 0);
	}
	tagNC_GetCityInfoByCityID cmd;
	cmd.dwCityID = pEvent->dwCityID;
	m_pSession->Send(&cmd);
	return 0;
}


DWORD CityMgr::OnEventOpenCitySetRate( tagGameEvent *pEvent )
{
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInGuild() == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error01")]);
		return 0;
	}
	EGuildMemberPos ePos = GuildMgr::Inst()->GetLpInfo()->eGuildPos;
	if (ePos != EGMP_HuiZhang && ePos != EGMP_FuHuiZhang)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error01")]);
		return 0;
	}
	// 向服务器查询上次更改税率的时间
	DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	SendGetAppointedCityAtt(dwNPCID, ECAT_TaxRateTime);
	m_bSendGetLastRateSetTime = true;
	return 0;
}


DWORD CityMgr::OnEventOpenCityPickUpTax(tagGameEvent *pEvent)
{
	// 取得玩家所在地图的城市ID
	tstring strMapName = MapMgr::Inst()->GetCurMapName();
	MapNameToCityIDMap::iterator iter = m_mapName2CityID.find(strMapName);
	DWORD dwCityID = GT_INVALID;
	if (iter != m_mapName2CityID.end())
		dwCityID = iter->second;
	bool bOwned = GuildMgr::Inst()->IsBeOwned(static_cast<BYTE>(dwCityID));
	if (bOwned == false)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error03")]);
		return 0;
	}
	// 向服务器查询税金
	DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	SendGetAppointedCityAtt(dwNPCID, ECAT_Taxation);

	return 0; 
}


DWORD CityMgr::OnEventOpenCityWarApply(tagGameEvent *pEvent)
{
	CityWarApplyFrame *pFrame = (CityWarApplyFrame*)m_pGameFrameMgr->GetFrame(_T("CityWarApply"));
	if (!P_VALID(pFrame))
	{
		m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("CityWarApply"), _T("CityWarApplyFrame"), 0);
	}	

	DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();

	SendGetApplyInfo(dwNPCID);
	return 0;
}



DWORD CityMgr::OnNetGetCityInfo(tagNS_GetCityInfo *pNetCmd, DWORD)
{
	m_bSendGetCityInfo = false;
	if (pNetCmd->dwErrorCode == E_Success)
	{
		CityFrame* pFrame = (CityFrame*)m_pGameFrameMgr->GetFrame(_T("City"));
		if (P_VALID(pFrame))
		{
			pFrame->Show(TRUE);
		}
		tagCityAttEvent Event(_T("Refresh_CityAtt"), NULL);
		Event.pCityAtt = &pNetCmd->sCityAtt;
		m_pGameFrameMgr->SendEvent(&Event);
	}
	else if (pNetCmd->dwErrorCode == E_City_Struggle_Not_Open)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Not_Open")]);
	}
	return 0;
}

DWORD CityMgr::OnNetGetAppointedCityAtt( tagNS_GetAppointedCityAtt *pNetCmd, DWORD )
{
	switch (pNetCmd->sCityAtt.eType)
	{
	case ECAT_TaxRateTime:
		{
			if (m_bSendGetLastRateSetTime == true)
			{
				m_bSendGetLastRateSetTime = false;
				tagDWORDTime CurTime = ServerTime::Inst()->CalCurrentServerDwordTime();
				tagDWORDTime LastRateSetTime = *(reinterpret_cast<tagDWORDTime*>(pNetCmd->sCityAtt.byData));
				DWORD dwTimeDiff = CalcTimeDiff(CurTime,LastRateSetTime);
				if (LastRateSetTime != 0 && dwTimeDiff <= (2 * 24 * 60 * 60))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error02")]);		
					return 0;
				}
				else
				{
					CityTaxRateFrame* pFrame = (CityTaxRateFrame*)m_pGameFrameMgr->GetFrame(_T("CityTaxRate"));
					if (!P_VALID(pFrame))
					{
						m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("CityTaxRate"), _T("CityTaxRateFrame"), 0);
					}
				}
			}
		} break;
	case ECAT_TaxRate:
		{
			CityTaxRateFrame* pFrame = (CityTaxRateFrame*)m_pGameFrameMgr->GetFrame(_T("CityTaxRate"));
			if (P_VALID(pFrame))
			{
				DWORD dwRate = *(reinterpret_cast<DWORD*>(pNetCmd->sCityAtt.byData));
				pFrame->SetCurTaxRateText(dwRate);
			}
		} break;
	case ECAT_Taxation:
		{
			CityPickUpTaxFrame *pFrame = (CityPickUpTaxFrame*)m_pGameFrameMgr->GetFrame(_T("CityPickUpTax"));
			if (!P_VALID(pFrame))
			{
				pFrame = (CityPickUpTaxFrame*)m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("CityPickUpTax"), _T("CityPickUpTaxFrame"), 0);
			}
			INT32 nTax = *(reinterpret_cast<INT32*>(pNetCmd->sCityAtt.byData));
			pFrame->SetCurTax(nTax);

		} break;
	}

	return 0;
}

DWORD CityMgr::OnNetSetTaxRate(tagNS_SetTaxRate *pNetCmd, DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		TCHAR szBuffer[256] = {0};
		tstring strName = GetCityName(pNetCmd->dwCityID);
		_sntprintf(szBuffer, 256, g_StrTable[_T("City_SetTaxRateSuccess")], strName.c_str(), pNetCmd->nTaxRate);
		ChatFrame* pFrame = (ChatFrame*)m_pGameFrameMgr->GetFrame(_T("Chat"));
		if (P_VALID(pFrame))
		{
			pFrame->PushScrollText(szBuffer);
			pFrame->PushInfo(szBuffer, ESCC_Affiche);
		}
	}
	else
	{
		if (m_bSendSetTaxRate == true)
		{
			m_bSendSetTaxRate = false;
			switch (pNetCmd->dwErrorCode)
			{
			case E_City_Guild_NotMatch:
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error06")]);
				} break;
			case E_City_Not_BeOccupied:
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error05")]);
				} break;
			case E_City_Time_Limit:
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error02")]);
				} break;
			default: break;
			}
		}
	}
	return 0;		
}


DWORD CityMgr::OnNetGetApplyInfo(tagNS_GetSigupInfo *pNetCmd, DWORD)
{
	// 主城名称

	//.........
	std::vector<DWORD> vecReqNameList;	// 需要向服务器请求帮派名称的vector

	// 统治权所属帮派
	DWORD dwOwnerID = pNetCmd->dwGuildID;
	if (dwOwnerID == GT_INVALID)
	{
	}
	else
	{
		bool bResult = GuildMgr::Inst()->IsGuildNameExist(dwOwnerID);
		if (!bResult)
			vecReqNameList.push_back(dwOwnerID);
	}

	int nTotalGuildNum = pNetCmd->nAckNum + pNetCmd->nDefNum + pNetCmd->nConfirmNum;

	for (int i = 0; i < nTotalGuildNum; ++i)
	{
		DWORD dwGuildID = pNetCmd->dwData[i];
		bool bResult = GuildMgr::Inst()->IsGuildNameExist(dwGuildID);
		if (!bResult)
			vecReqNameList.push_back(dwGuildID);
	}
	int nFlag;
	if (!vecReqNameList.empty())
	{
		GuildMgr::Inst()->SendGetSomeGuildName(nFlag, vecReqNameList);
	}

	tagCityWarApplyInfoRefreshEvent event(_T("tagCityWarApplyInfoRefreshEvent"), NULL);
	event.bAllNameCanFind = vecReqNameList.empty() ? true : false;
	event.nGetGuildNameFlag = vecReqNameList.empty() ? 0 : nFlag;
	event.dwCityID = pNetCmd->dwCityID;
	event.dwOwnGuild = pNetCmd->dwGuildID;	
	event.dwAvgAckMoney = pNetCmd->dwAvgAckMoney;
	event.warTime = pNetCmd->WarTime;
	int i = 0;
	for (; i < pNetCmd->nAckNum; ++i)
	{
		event.vecAttackIDs.push_back(pNetCmd->dwData[i]);
	}
	for (; i < (pNetCmd->nAckNum +pNetCmd->nDefNum); ++i)
	{
		event.vecDefenseIDs.push_back(pNetCmd->dwData[i]);
	}
	for (; i < (pNetCmd->nAckNum + pNetCmd->nDefNum + pNetCmd->nConfirmNum); ++i)
	{
		event.vecConfirmIDs.push_back(pNetCmd->dwData[i]);
	}

	m_pGameFrameMgr->SendEvent(&event);

	return 0;
}


DWORD CityMgr::OnNetUpgradeProlificacy(tagNS_UpgradeProlificacy *pNetCmd, DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_City_Prolificacy_LackItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Productivity_Error1")]);
		} break;
	case E_City_Not_BeOccupied:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Productivity_Error2")]);
		} break;
	case E_City_Guild_NotMatch:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Productivity_Error3")]);
		} break;
	default:
		break;
	}
	return 0;
}


DWORD CityMgr::OnNetGetRoleInitStateCity(tagNS_GetRoleInitState_City *pNetCmd, DWORD)
{
	m_mapCityID2TaxRate.clear();
	for (BYTE i = 0; i < pNetCmd->byCityNum; ++i)
	{
		tagCityTaxRate *pCityTaxRate = &(pNetCmd->stData[i]);
		m_mapCityID2TaxRate.insert(std::make_pair((DWORD)pCityTaxRate->byCityID, (INT32)pCityTaxRate->byTaxRate));
	}
	return 0;
}

DWORD CityMgr::GetCityID( const tstring &strMapName ) const
{
	DWORD dwCityID = GT_INVALID;
	MapNameToCityIDMap::const_iterator iter = m_mapName2CityID.find(strMapName);
	if (iter != m_mapName2CityID.end())
	{
		dwCityID =  iter->second;
	}
	return dwCityID;
}

tstring CityMgr::GetCityName( DWORD dwCityID ) const
{
	CityIDToNameMap::const_iterator iter = m_mapCityID2Name.find(dwCityID);
	if (iter != m_mapCityID2Name.end())
		return iter->second.strCityName;
	return _T("");
}

tstring CityMgr::GetCityIcon(DWORD dwCityID) const
{
	CityIDToNameMap::const_iterator iter = m_mapCityID2Name.find(dwCityID);
	if (iter != m_mapCityID2Name.end())
		return iter->second.strIconPath;
	return _T("");
}


tstring CityMgr::GetCityGod(DWORD dwCityID) const
{
	CityIDToNameMap::const_iterator iter = m_mapCityID2Name.find(dwCityID);
	if (iter != m_mapCityID2Name.end())
		return iter->second.strGodName;
	return _T("");
}


INT CityMgr::GetMinSigfare(DWORD dwCityID) const
{
	CityIDToMinSigFareMap::const_iterator iter = m_mapCityID2MinSigfare.find(dwCityID);
	if (iter != m_mapCityID2MinSigfare.end())
		return iter->second;
	return 0;
}


INT32 CityMgr::GetTaxRate(DWORD dwCityID) const
{
	CityIDToTaxRateMap::const_iterator iter = m_mapCityID2TaxRate.find(dwCityID);
	ASSERT(iter != m_mapCityID2TaxRate.end());
	if (iter != m_mapCityID2TaxRate.end())
	{
		return iter->second;
	}
	return 0;
}



VOID CityMgr::RegisterFrames(GameFrameMgr* pMgr)
{
	if (P_VALID(pMgr))
	{
		pMgr->Register(TWrap<CityFrame>(), _T("CityFrame"));
		pMgr->Register(TWrap<CityWarInfoFrame>(), _T("CityWarInfoFrame"));
		pMgr->Register(TWrap<CityPickUpTaxFrame>(), _T("CityPickUpTaxFrame"));
		pMgr->Register(TWrap<CityTaxRateFrame>(), _T("CityTaxRateFrame"));
		pMgr->Register(TWrap<CityWarApplyFrame>(), _T("CityWarApplyFrame"));
		pMgr->Register(TWrap<CityWarApplyMoneyFrame>(), _T("CityWarApplyMoneyFrame"));
		pMgr->Register(TWrap<CityWarAssistConfirmFrame>(), _T("CityWarAssistConfirmFrame"));
		pMgr->Register(TWrap<CityWarStateFrame>(), _T("CityWarStateFrame"));
		pMgr->Register(TWrap<CityWarResultFrame>(), _T("CityWarResultFrame"));
	}
}

VOID CityMgr::SendGetCityWarCity()
{
	tagNC_GetCityWarCity cmd;
	m_pSession->Send(&cmd);
}

INT32 CityMgr::GetProductMinLimit(DWORD dwCityID, DWORD dwItemTypeID)
{
	CityIDToProductMap::const_iterator iter = m_mapProduct.find(dwCityID);
	ASSERT(iter != m_mapProduct.end());
	if (iter != m_mapProduct.end())
	{
		for (size_t i = 0; i < iter->second.size(); ++i)
		{
			if (dwItemTypeID == iter->second[i].dwTypeID)
				return iter->second[i].nLimit;
		}
	}
	return 0;
}

VOID CityMgr::GetCityDefaultProduct(DWORD dwCityID, std::vector<tagCityProduct>& out)
{
	out.clear();
	CityIDToProductMap::const_iterator iter = m_mapProduct.find(dwCityID);
	ASSERT(iter != m_mapProduct.end());
	if (iter != m_mapProduct.end())
	{
		out.resize(iter->second.size());
		std::copy(iter->second.begin(), iter->second.end(), out.begin());
	}
}

DWORD CityMgr::OnNetConfirmDefenceForce(tagNS_ConfirmDefenceForce *pNetCmd, DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
		//case E_Success:
		//	{
		//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Defence_Confirm_Success")]);
		//	} break;
	case E_City_SignUp_Force_Full:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Assist_Confirm_Error3")]);
		} break;
	case E_City_Not_BeOccupied:
		{

		} break;
	case E_City_Guild_NotMatch:
		{

		} break;
	case E_City_Time_Limit:
		{

		} break;
	case E_City_SignUp_Info_Error:
		{

		} break;
	case E_City_SignUp_Already_Confirm:
		{

		} break;
	case E_City_Struggle_Not_Open:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Not_Open")]);
		} break;
	default: break;
	}
	return 0;
}

DWORD CityMgr::OnNetCityStruggleInfo(tagNS_CityStruggleInfo* pMsg, DWORD)
{
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer) || (!pPlayer->IsInState(ES_CSAttack) && !pPlayer->IsInState(ES_CSDefence))) return 0;
	DWORD mapID = MapMgr::Inst()->GetCurMapID();
	if(pMsg->sLongZhuState.eForce == ECFT_Attack)
		m_mapDragonState[mapID] = TRUE;
	else if(pMsg->sLongZhuState.eForce == ECFT_Defence)
		m_mapDragonState[mapID] = FALSE;
	for(int i=0; i<MAX_CITY_DOOR_NUM; i++)
	{
		m_mapDoorState[i][mapID] = pMsg->sDoorState[i].bDestroied;
	}
	for(int i=0; i<MAX_YINYANGZHU_NUM; i++)
	{
		m_mapColumnState[i][mapID] = pMsg->sYinYangZhuState[i].eForce == ECFT_Attack;
	}
	return 0;
}

DWORD CityMgr::OnNetCityStruggleInfoChange(tagNS_CityStruggleInfoChange* pMsg, DWORD)
{
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer) || (!pPlayer->IsInState(ES_CSAttack) && !pPlayer->IsInState(ES_CSDefence))) return 0;
	DWORD mapID = MapMgr::Inst()->GetCurMapID();
	switch(pMsg->eType1)
	{	
	case ECSCT_LongZhuBeSeized:					// 龙珠被封印中
		if(pMsg->nData == ECFT_Attack)
			m_mapDragonState[mapID] = TRUE;
		else if(pMsg->nData == ECFT_Defence)
			m_mapDragonState[mapID] = FALSE;
		break;	
	case ECSCT_YinYangZhuSeized:					// 阴阳柱被另一方占领
		if(pMsg->nType2 >= 0 && pMsg->nType2 < NUM_OF_COLUMN)
			m_mapColumnState[pMsg->nType2][mapID] = pMsg->nData == ECFT_Attack;
		break;
	case ECSCT_DoorDestroied:	// 城门被击破
		if(pMsg->nType2 <= NUM_OF_DOOR && pMsg->nType2>0)
			m_mapDoorState[pMsg->nType2-1][mapID] = TRUE;
		break;
	case ECSCT_DoorRepaired:					// 城门修复
		if(pMsg->nType2 <= NUM_OF_DOOR && pMsg->nType2>0)
			m_mapDoorState[pMsg->nType2-1][mapID] = FALSE;
		break;
	}
	return 0;
}

DWORD CityMgr::OnMsgBoxEventViewCityInfo(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		// 检查玩家的金钱
		INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
		if (n64Silver < 10 * GOLD2SILVER)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error07")]);
			return 0;
		}
		CityFrame* pFrame = (CityFrame*)m_pGameFrameMgr->GetFrame(_T("City"));
		if (!P_VALID(pFrame))
		{
			m_pGameFrameMgr->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("City"), _T("CityFrame"), 0);
		}
		DWORD dwGlobalNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		// 发送消息申请城市信息
		SendGetCityInfo(dwGlobalNPCID);
		return 0;
	}
	return 0;
}