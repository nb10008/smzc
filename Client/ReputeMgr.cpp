#include "StdAfx.h"
#include "ReputeMgr.h"
#include "..\WorldDefine\msg_famehall.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"
#include "GodEvilReputeFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "SkillMgr.h"
#include "RoleEvent.h"

ReputeMgr::ReputeMgr(void):m_Trunk(this)
{
	memset(m_dwGERepute,NULL,sizeof(m_dwGERepute));
	memset(m_dwGESoul,NULL,sizeof(m_dwGESoul));
}

ReputeMgr::~ReputeMgr(void)
{
}

VOID ReputeMgr::LoadFormFile()
{
	LPCTSTR szPulseProto = _T("data\\system\\attdata\\soulcrystalatt_proto.xml");
	LPCTSTR szPulseLevel = _T("data\\system\\attdata\\soulcrystallevel.xml");
	tstring strPath = g_strLocalPath + _T("\\attdata\\soulcrystalatt_name.xml");
	LPCTSTR szPulseName = strPath.c_str(); 

	list<tstring> ClergyProtoFieldList;
	list<tstring> ClergyNameFieldList;
	list<tstring> ClergyLevelFieldList;
	list<tstring>::iterator iter;

	TCHAR szTemp[X_LONG_NAME] = {0};

	//---------------------------------------------------------------------------------
	//读取level
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPulseLevel = CreateObj("HunJingLevel", "VarContainer");
	if(!VarPulseLevel->Load("VFS_System", szPulseLevel, "id", &ClergyLevelFieldList))
		IMSG(_T("Load file HunJingLevel.xml failed\r\n"));

	for(iter = ClergyLevelFieldList.begin(); iter != ClergyLevelFieldList.end(); ++iter)
	{
		tagHunJingLevel stProto;

		stProto.nLevel = VarPulseLevel->GetDword(_T("id"),							(*iter).c_str(), GT_INVALID);
		stProto.dwHunNum = VarPulseLevel->GetDword(_T("soulrequire"),							(*iter).c_str(), GT_INVALID);
		stProto.dwMoney =  VarPulseLevel->GetDword(_T("goldrequire"),							(*iter).c_str(), GT_INVALID);
	
		m_mapHunJingLevel.insert(make_pair(stProto.nLevel, stProto));

	}

	KillObj("HunJingLevel");

	//---------------------------------------------------------------------------------
	//读取proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPulseProto = CreateObj("HunJingProto", "VarContainer");
	if(!VarPulseProto->Load("VFS_System", szPulseProto, "id", &ClergyProtoFieldList))
		IMSG(_T("Load file HunJingProto.xml failed\r\n"));

	for(iter = ClergyProtoFieldList.begin(); iter != ClergyProtoFieldList.end(); ++iter)
	{
		tagHunJingName stProto;

		stProto.dwID = VarPulseProto->GetDword(_T("id"),							(*iter).c_str(), GT_INVALID);
		stProto.nLevel = VarPulseProto->GetDword(_T("level"),							(*iter).c_str(), GT_INVALID);
		stProto.dwAttType =  VarPulseProto->GetDword(_T("atttype"),							(*iter).c_str(), GT_INVALID);
		stProto.dwAttValue =  VarPulseProto->GetDword(_T("value"),							(*iter).c_str(), GT_INVALID);


		m_mapHunJingName.insert(make_pair(stProto.dwID, stProto));

	}

	KillObj("HunJingProto");



	//---------------------------------------------------------------------------------
	//读取name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPulseName = CreateObj("HunJingName", "VarContainer");
	if(!VarPulseName->Load("VFS_System", szPulseName, "id", &ClergyNameFieldList))
		IMSG(_T("Load file HunJingName.xml failed\r\n"));

	for(iter = ClergyNameFieldList.begin(); iter != ClergyNameFieldList.end(); ++iter)
	{
		DWORD dwID = VarPulseName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		map<DWORD, tagHunJingName>::iterator iter2 = m_mapHunJingName.find(dwID);
		if (iter2 != m_mapHunJingName.end())
		{
			_tcsncpy( iter2->second.szName,			VarPulseName->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(iter2->second.szName)/sizeof(TCHAR));
			_tcsncpy( iter2->second.szDesc,			VarPulseName->GetString( _T("dis"),				(*iter).c_str(), _T("")	), sizeof(iter2->second.szDesc)/sizeof(TCHAR));
		}
	}

	KillObj("HunJingName");
}

VOID ReputeMgr::Init()
{
    // 初始化一些成员变量
    for (int i = ECLT_BEGIN; i < ECLT_NUM; ++i)
    {
        m_nReputeValue[i]   =   0;
        m_bisFame[i]        =   FALSE;
        m_nActCount[i]      =   0;
        UlityCalReputeLevel(m_ReputeLevel[i], m_nReputeValue[i]);
    }

	// 声望进度条颜色文件
	m_strPrgssForePic_Red = _T("data\\ui\\state\\L_sw-jindu-c.bmp");
	m_strPrgssForePic_Yellow = _T("data\\ui\\state\\L_sw-jindu-e.bmp");
	m_strPrgssForePic_Green = _T("data\\ui\\state\\L_sw-jindu-a.bmp");
	m_strPrgssForePic_Blue = _T("data\\ui\\state\\L_sw-jindu-b.bmp");
	m_strPrgssForePic_Orange = _T("data\\ui\\state\\L_sw-jindu-d.bmp");

	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pUtil	= TObjRef<Util>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pSession = TObjRef<NetSession>();

	LoadDataFromFile();
	LoadFormFile();

	m_pFrameMgr->RegisterEventHandle(_T("OpenGodReputeFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeMgr::OnOpenGodReputeFrame));
	m_pFrameMgr->RegisterEventHandle(_T("OpenEvilReputeFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeMgr::OnOpenEvilReputeFrame));

	//注册网络消息
	m_pCmdMgr->Register("NS_GetRoleInitState_Reputation",   (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnInitReputation),         _T("NS_GetRoleInitState_Reputation"));
	m_pCmdMgr->Register("NS_RoleReputeChange",              (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnReputeChange),           _T("NS_RoleReputeChange"));
    m_pCmdMgr->Register("NS_BecomeFame",                    (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnBecomeFame),             _T("NS_BecomeFame"));
    m_pCmdMgr->Register("NS_TreasureActCountChange",        (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnActCountChange),         _T("NS_TreasureActCountChange"));
    m_pCmdMgr->Register("NS_ClanConUpperLimit",             (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ClanConUpperLimit),   _T("NS_ClanConUpperLimit"));
	m_pCmdMgr->Register("NS_ChangeSoul",					(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ChangeSoul),			_T("NS_GPetGotMail"));
	m_pCmdMgr->Register("NS_ActiveSoulSkill",				(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ActiveSoulSkill),		_T("NS_GPetGotMail"));
	m_pCmdMgr->Register("NS_ContinueSoulSkill",				(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ContinueSoulSkill),	_T("NS_GPetGotMail"));
	m_pCmdMgr->Register("NS_SoulCrystalGrow",				(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_SoulCrystalGrow),	_T("NS_SoulCrystalGrow"));
	m_pCmdMgr->Register("NS_SoulCrystalRenew",				(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_SoulCrystalRenew),	_T("NS_SoulCrystalRenew"));

}

VOID ReputeMgr::Destroy()
{
	ClearAllData();
	m_pFrameMgr->UnRegisterEventHandler(_T("OpenGodReputeFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeMgr::OnOpenGodReputeFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("OpenEvilReputeFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeMgr::OnOpenEvilReputeFrame));
	//注销网络消息
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Reputation", (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnInitReputation));
	m_pCmdMgr->UnRegister("NS_RoleReputeChange",            (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnReputeChange));
    m_pCmdMgr->UnRegister("NS_BecomeFame",                  (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnBecomeFame));
    m_pCmdMgr->UnRegister("NS_TreasureActCountChange",      (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnActCountChange));
    m_pCmdMgr->UnRegister("NS_ClanConUpperLimit",           (NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ClanConUpperLimit));
	m_pCmdMgr->UnRegister("NS_ChangeSoul",					(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ChangeSoul));
	m_pCmdMgr->UnRegister("NS_ActiveSoulSkill",				(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ActiveSoulSkill));
	m_pCmdMgr->UnRegister("NS_ContinueSoulSkill",			(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_ContinueSoulSkill));
	m_pCmdMgr->UnRegister("NS_SoulCrystalGrow",			(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_SoulCrystalGrow));
	m_pCmdMgr->UnRegister("NS_SoulCrystalRenew",			(NETMSGPROC)m_Trunk.sfp2(&ReputeMgr::OnNS_SoulCrystalRenew));
}

ReputeMgr g_ReputeMgr;
ReputeMgr* ReputeMgr::Inst()
{
	return &g_ReputeMgr;
}

VOID ReputeMgr::SetReputeValue( ECLanType eCtype, INT nValue, INT nChange )
{
	if (JDG_VALID(ECLT, eCtype))
	{
		m_nReputeValue[eCtype] = nValue;
		UlityLocalPlayerRepuLvl(eCtype, nValue, nChange);
		UlityCalReputeLevel(m_ReputeLevel[eCtype], nValue);

		tagReputeChange msg(_T("ReputeChange"), NULL);
		msg.eClanType	=	eCtype;
		msg.nValue		=	nValue;
		msg.nChange		=	nChange;
		m_pFrameMgr->SendEvent(&msg);
	}
}

VOID ReputeMgr::ClearAllData()
{
	m_mapHunJingName.clear();
	m_mapHunJingLevel.clear();
	m_mapForceSkill.clear();
}

DWORD ReputeMgr::OnInitReputation( tagNS_GetRoleInitState_Reputation* pNetCmd, DWORD )
{
	for (int i = ECLT_BEGIN; i < ECLT_NUM; ++i)
    {
		m_nReputeValue[i]   =   pNetCmd->nReputation[i];
        m_bisFame[i]        =   (BOOL)pNetCmd->bisFame[i];
        m_nActCount[i]      =   (INT)pNetCmd->nActiveCount[i];
		
		OnInitRepuLvl((ECLanType)i, pNetCmd->nReputation[i]);
        UlityCalReputeLevel(m_ReputeLevel[i], pNetCmd->nReputation[i]);
    }

	// 跟在这后面发送当前所有激活的氏族珍宝请求
	for (INT i = ECLT_BEGIN; i < ECLT_NUM; ++i)
	{
		tagNC_GetActClanTreasure e;
		e.byClanType	=	(ECLanType)i;
		if (m_pSession->IsConnect())
			m_pSession->Send(&e);
	}
	
	// 如果玩家帮派ID有效，发送帮派初始化请求消息
	if(GT_VALID(RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID()))
	{
		tagNC_GetRoleInitState cmd;
		cmd.eType = ERIT_Guild;
		m_pSession->Send(&cmd);
	}
	else
	{
		// 在初始化消息最后发送本地玩家初始化完成消息
		tagGameEvent evtInit( _T("tagInitLPAttEvent"), NULL );
		m_pFrameMgr->SendEvent(&evtInit);
	}

	return 0;
}

DWORD ReputeMgr::OnNS_ChangeSoul(tagNS_ChangeSoul* pCmd, DWORD)
{
	//飘字
	if(pCmd->nGod - m_dwGESoul[0] != 0)
	{
		tagPctNExpAchieve event(_T("UpdateShenHunValue"),NULL);
		event.nPctMultiplied = pCmd->nGod - m_dwGESoul[0];
		m_pFrameMgr->SendEvent(&event);
	}
	if(pCmd->nMonster - m_dwGESoul[1] != 0)
	{
		tagPctNExpAchieve event(_T("UpdateMohHunValue"),NULL);
		event.nPctMultiplied = pCmd->nMonster - m_dwGESoul[1];
		m_pFrameMgr->SendEvent(&event);
	}

	m_dwGERepute[0] = pCmd->nGodPoint;
	m_dwGERepute[1] = pCmd->nMonsterPoint;
	m_dwGESoul[0] = pCmd->nGod;
	m_dwGESoul[1] = pCmd->nMonster;
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("ReputeEvent_RefreshGEInfo"),NULL));

	return 0;
}

DWORD ReputeMgr::OnNS_ActiveSoulSkill(tagNS_ActiveSoulSkill* pCmd, DWORD)
{
	if (pCmd->dwResult == E_Success)
	{
		SkillMgr::Inst()->SetSkillRenewTime(pCmd->dwSkillID,pCmd->dwTime);
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("ReputeEvent_RefreshGEInfo"),NULL));
	}
	else
		ShowGEMsgCode(pCmd->dwResult);

	return 0;
}

DWORD ReputeMgr::OnNS_ContinueSoulSkill(tagNS_ContinueSoulSkill* pCmd, DWORD)
{
	if (pCmd->dwResult == E_Success)
	{
		SkillMgr::Inst()->SetSkillRenewTime(pCmd->dwSkillID,pCmd->dwTime);
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("ReputeEvent_RefreshGEInfo"),NULL));
	}
	else
		ShowGEMsgCode(pCmd->dwResult);

	return 0;
}

VOID ReputeMgr::ShowGEMsgCode(DWORD dwErrorCode)
{
	if (dwErrorCode != E_Success)
	{
		TCHAR szTem[X_LONG_NAME] = {0};
		_sntprintf(szTem,X_LONG_NAME,_T("GERepute_Error%d"),dwErrorCode);
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[szTem] );
	}
}

VOID ReputeMgr::SendNC_ActiveSoulSkill(DWORD _dwSkillID)
{
	tagNC_ActiveSoulSkill cmd;
	cmd.dwSkillID        = _dwSkillID;
	m_pSession->Send(&cmd);
}

VOID ReputeMgr::SendNC_ContinueSoulSkill(DWORD _dwSkillID,INT64 _n64ItemID)
{
	tagNC_ContinueSoulSkill cmd;
	cmd.n64ItemID		 = _n64ItemID;
	cmd.dwSkillID        = _dwSkillID;
	m_pSession->Send(&cmd);
}

DWORD ReputeMgr::OnReputeChange( tagNS_RoleReputeChange* pNetCmd, DWORD )
{
	SetReputeValue((ECLanType)pNetCmd->byClanType, pNetCmd->nRepute, pNetCmd->nChange);

	return 0;
}

BOOL ReputeMgr::UlityCalReputeLevel( tagReputeLevel& ReputeSrc, INT nReputeValue )
{
	if (nReputeValue >= -46800 && nReputeValue < -10800)
	{
		ReputeSrc.eLevel			=	ERL_Hostile;
		ReputeSrc.eColor			=	EPC_RED;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel0")];
		ReputeSrc.nFrom				=	-46800;
		ReputeSrc.nTo				=	-10800;
		ReputeSrc.nDiffValue		=	36000;
		ReputeSrc.nMaxContribute	=	0;
	}
	else if (nReputeValue >= -10800 && nReputeValue < -3600)
	{
		ReputeSrc.eLevel			=	ERL_Hated;
		ReputeSrc.eColor			=	EPC_RED;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel1")];
		ReputeSrc.nFrom				=	-10800;
		ReputeSrc.nTo				=	-3600;
		ReputeSrc.nDiffValue		=	7200;
		ReputeSrc.nMaxContribute	=	0;
	}
	else if (nReputeValue >= -3600 && nReputeValue < 0)
	{
		ReputeSrc.eLevel			=	ERL_Callous;
		ReputeSrc.eColor			=	EPC_YELLOW;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel2")];
		ReputeSrc.nFrom				=	-3600;
		ReputeSrc.nTo				=	0;
		ReputeSrc.nDiffValue		=	3600;
		ReputeSrc.nMaxContribute	=	5000;
	}
	else if (nReputeValue >= 0 && nReputeValue < 3600)
	{
		ReputeSrc.eLevel			=	ERL_Neutrality;
		ReputeSrc.eColor			=	EPC_YELLOW;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel3")];
		ReputeSrc.nFrom				=	0;
		ReputeSrc.nTo				=	3600;
		ReputeSrc.nDiffValue		=	3600;
		ReputeSrc.nMaxContribute	=	5000;
	}
	else if (nReputeValue >= 3600 && nReputeValue < 10800)
	{
		ReputeSrc.eLevel			=	ERL_Friendly;
		ReputeSrc.eColor			=	EPC_GREEN;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel4")];
		ReputeSrc.nFrom				=	3600;
		ReputeSrc.nTo				=	10800;
		ReputeSrc.nDiffValue		=	7200;
		ReputeSrc.nMaxContribute	=	10000;
	}
	else if (nReputeValue >= 10800 && nReputeValue < 22800)
	{
		ReputeSrc.eLevel			=	ERL_Intimate;
		ReputeSrc.eColor			=	EPC_GREEN;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel5")];
		ReputeSrc.nFrom				=	10800;
		ReputeSrc.nTo				=	22800;
		ReputeSrc.nDiffValue		=	12000;
		ReputeSrc.nMaxContribute	=	20000;
	}
	else if (nReputeValue >= 22800 && nReputeValue < 40800)
	{
		ReputeSrc.eLevel			=	ERL_Reliable;
		ReputeSrc.eColor			=	EPC_GREEN;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel6")];
		ReputeSrc.nFrom				=	22800;
		ReputeSrc.nTo				=	40800;
		ReputeSrc.nDiffValue		=	18000;
		ReputeSrc.nMaxContribute	=	40000;
	}
	else if (nReputeValue >= 40800 && nReputeValue < 76800)
	{
		ReputeSrc.eLevel			=	ERL_Respected;
		ReputeSrc.eColor			=	EPC_BLUE;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel7")];
		ReputeSrc.nFrom				=	40800;
		ReputeSrc.nTo				=	76800;
		ReputeSrc.nDiffValue		=	36000;
		ReputeSrc.nMaxContribute	=	70000;
	}
	else if (nReputeValue >= 76800 && nReputeValue < 148800)
	{
		ReputeSrc.eLevel			=	ERL_Worshipped;
		ReputeSrc.eColor			=	EPC_BLUE;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel8")];
		ReputeSrc.nFrom				=	76800;
		ReputeSrc.nTo				=	148800;
		ReputeSrc.nDiffValue		=	72000;
		ReputeSrc.nMaxContribute	=	120000;
	}
	else if (nReputeValue >= 148800 && nReputeValue <= 328800)
	{
		ReputeSrc.eLevel			=	ERL_Legend;
		ReputeSrc.eColor			=	EPC_ORANGE;
		ReputeSrc.szLevel			=	g_StrTable[_T("ReputeLevel9")];
		ReputeSrc.nFrom				=	148800;
		ReputeSrc.nTo				=	328800;
		ReputeSrc.nDiffValue		=	180000;
		ReputeSrc.nMaxContribute	=	200000;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

VOID ReputeMgr::UlitySetPrgrssColor( ProgressEx* pProgress, EPrgrssColor eColor )
{
	switch (eColor)
	{
	case EPC_RED:
		if (pProgress->GetForePic() != m_strPrgssForePic_Red)
			pProgress->SetForePic(m_strPrgssForePic_Red);
		break;
	case EPC_YELLOW:
		if (pProgress->GetForePic() != m_strPrgssForePic_Yellow)
			pProgress->SetForePic(m_strPrgssForePic_Yellow);
		break;
	case EPC_GREEN:
		if (pProgress->GetForePic() != m_strPrgssForePic_Green)
			pProgress->SetForePic(m_strPrgssForePic_Green);
		break;
	case EPC_BLUE:
		if (pProgress->GetForePic() != m_strPrgssForePic_Blue)
			pProgress->SetForePic(m_strPrgssForePic_Blue);
		break;
	case EPC_ORANGE:
		if (pProgress->GetForePic() != m_strPrgssForePic_Orange)
			pProgress->SetForePic(m_strPrgssForePic_Orange);
		break;
	}
}

DWORD ReputeMgr::OnBecomeFame( tagNS_BecomeFame* pNetCmd, DWORD )
{
    ECLanType   eCtype   =   (ECLanType)pNetCmd->eClanType;
    if (JDG_VALID(ECLT, eCtype))
    {
        m_bisFame[eCtype]   =   TRUE;
    }

    // 发送游戏事件
    tagBecomeEvent e(_T("tagBecomeEvent"), NULL);
    m_pFrameMgr->SendEvent(&e);
	return 0;
}

DWORD ReputeMgr::OnActCountChange( tagNS_TreasureActCountChange* pNetCmd, DWORD )
{
    ECLanType   eCtype   =   (ECLanType)pNetCmd->eClanType;
    if (JDG_VALID(ECLT, eCtype))
    {
        m_nActCount[eCtype] =   (INT)pNetCmd->nActCount;
    }

    // 发送游戏事件
    tagTreasureActCountChange e(_T("tagTreasureActCountChange"), NULL);
    m_pFrameMgr->SendEvent(&e);
	return 0;
}

VOID ReputeMgr::OnInitRepuLvl( ECLanType eType, INT nReputeValue )
{
	if (nReputeValue >= -46800 && nReputeValue < -10800)
	{
		m_eRepuLvl[eType]			= ERL_Hostile;
	}
	else if (nReputeValue >= -10800 && nReputeValue < -3600)
	{
		m_eRepuLvl[eType]			= ERL_Hated;
	}
	else if (nReputeValue >= -3600 && nReputeValue < 0)
	{
		m_eRepuLvl[eType]			= ERL_Callous;
	}
	else if (nReputeValue >= 0 && nReputeValue < 3600)
	{
		m_eRepuLvl[eType]			= ERL_Neutrality;
	}
	else if (nReputeValue >= 3600 && nReputeValue < 10800)
	{
		m_eRepuLvl[eType]			= ERL_Friendly;
	}
	else if (nReputeValue >= 10800 && nReputeValue < 22800)
	{
		m_eRepuLvl[eType]			= ERL_Intimate;
	}
	else if (nReputeValue >= 22800 && nReputeValue < 40800)
	{
		m_eRepuLvl[eType]			= ERL_Reliable;
	}
	else if (nReputeValue >= 40800 && nReputeValue < 76800)
	{
		m_eRepuLvl[eType]			= ERL_Respected;
	}
	else if (nReputeValue >= 76800 && nReputeValue < 148800)
	{
		m_eRepuLvl[eType]			= ERL_Worshipped;
	}
	else if (nReputeValue >= 148800 && nReputeValue <= 328800)
	{
		m_eRepuLvl[eType]			= ERL_Legend;
	}
}

void ReputeMgr::UlityLocalPlayerRepuLvl( ECLanType eType, INT nReputeValue, INT nChange )
{
	bool bChange = false;
	if (nReputeValue >= -46800 && nReputeValue < -10800)
	{
		if( m_eRepuLvl[eType] != ERL_Hostile)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Hostile;
	}
	else if (nReputeValue >= -10800 && nReputeValue < -3600)
	{
		if( m_eRepuLvl[eType] != ERL_Hated)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Hated;
	}
	else if (nReputeValue >= -3600 && nReputeValue < 0)
	{
		if( m_eRepuLvl[eType] != ERL_Callous)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Callous;
	}
	else if (nReputeValue >= 0 && nReputeValue < 3600)
	{
		if( m_eRepuLvl[eType] != ERL_Neutrality)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Neutrality;
	}
	else if (nReputeValue >= 3600 && nReputeValue < 10800)
	{
		if( m_eRepuLvl[eType] != ERL_Friendly)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Friendly;
	}
	else if (nReputeValue >= 10800 && nReputeValue < 22800)
	{
		if( m_eRepuLvl[eType] != ERL_Intimate)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Intimate;
	}
	else if (nReputeValue >= 22800 && nReputeValue < 40800)
	{
		if( m_eRepuLvl[eType] != ERL_Reliable)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Reliable;
	}
	else if (nReputeValue >= 40800 && nReputeValue < 76800)
	{
		if( m_eRepuLvl[eType] != ERL_Respected)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Respected;
	}
	else if (nReputeValue >= 76800 && nReputeValue < 148800)
	{
		if( m_eRepuLvl[eType] != ERL_Worshipped)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Worshipped;
	}
	else if (nReputeValue >= 148800 && nReputeValue <= 328800)
	{
		if( m_eRepuLvl[eType] != ERL_Legend)
		{
			bChange = true;
		}
		m_eRepuLvl[eType]			= ERL_Legend;
	}

	if( bChange )
	{
		tagReputeLvlChange change(_T("tagReputeLvlChange"), NULL);
		change.eClanType = eType;
		change.eLevel = m_eRepuLvl[eType];
		m_pFrameMgr->SendEvent(&change);
	}
}

DWORD ReputeMgr::OnNS_ClanConUpperLimit( tagNS_ClanConUpperLimit* pNetCmd, DWORD )
{
    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClanContributeLimit")], _T(""));

    return 0;
}


DWORD ReputeMgr::OnOpenGodReputeFrame(tagGameEvent* pGameEvent)
{
	GodEvilReputeFrame* pFrame = (GodEvilReputeFrame*)m_pFrameMgr->GetFrame(_T("GodEvilRepute"));
	if (!P_VALID(pFrame))
		pFrame = (GodEvilReputeFrame*)m_pFrameMgr->CreateFrame(_T("World"),_T("GodEvilRepute"),_T("GodEvilReputeFrame"),0);

	if (P_VALID(pFrame))
	{
		pFrame->OpenGEReputeFrame(0);
	}

	return 0;
}

DWORD ReputeMgr::OnOpenEvilReputeFrame(tagGameEvent* pGameEvent)
{
	GodEvilReputeFrame* pFrame = (GodEvilReputeFrame*)m_pFrameMgr->GetFrame(_T("GodEvilRepute"));
	if (!P_VALID(pFrame))
		pFrame = (GodEvilReputeFrame*)m_pFrameMgr->CreateFrame(_T("World"),_T("GodEvilRepute"),_T("GodEvilReputeFrame"),0);

	if (P_VALID(pFrame))
	{
		pFrame->OpenGEReputeFrame(1);
	}
	return 0;
}

VOID ReputeMgr::LoadDataFromFile()
{
	list<tstring> listProto;
	tstring strForceSKill = _T("data\\system\\attdata\\force_skill.xml");
	TObjRef<VarContainer> varForceSKillProto = CreateObj("Forceskill", "VarContainer");
	if (!varForceSKillProto->Load("VFS_System", strForceSKill.c_str(), "id", &listProto))
	{
		IMSG(_T("Load Force_skill file failed"));
		KillObj("Forceskill");
		return;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagForceSkill data;

		data.dwID = varForceSKillProto->GetInt(_T("id"), begin->c_str());
		data.dwClassTypeLimited = varForceSKillProto->GetInt(_T("job"), begin->c_str());
		data.eType = (EReputeType)varForceSKillProto->GetInt(_T("side"), begin->c_str());
		data.dwReputeLevel = varForceSKillProto->GetInt(_T("level"), begin->c_str());
		data.dwNeedSkill = varForceSKillProto->GetInt(_T("needskill"), begin->c_str(),NULL);
		data.bForever = (BOOL)varForceSKillProto->GetInt(_T("forever"), begin->c_str());
		data.dwActvNeedSoul = varForceSKillProto->GetInt(_T("open_soul"), begin->c_str());
		data.dwActvNeedMoney = varForceSKillProto->GetInt(_T("open_money"), begin->c_str());
		data.dwRenewNeedSoul = varForceSKillProto->GetInt(_T("continue_soul"), begin->c_str());
		data.dwRenewNeedMoney = varForceSKillProto->GetInt(_T("continue_money"), begin->c_str());
		data.bNeedDel = (BOOL)varForceSKillProto->GetInt(_T("delete"), begin->c_str());

		m_mapForceSkill.insert(make_pair(data.dwID,data));
	}
	KillObj("Forceskill");


	listProto.clear();
	tstring strForceLevel = _T("data\\system\\attdata\\force_level.xml");
	TObjRef<VarContainer> varForceLevelProto = CreateObj("ForceLevel", "VarContainer");
	if (!varForceLevelProto->Load("VFS_System", strForceLevel.c_str(), "id", &listProto))
	{
		IMSG(_T("Load Force_Level file failed"));
		KillObj("ForceLevel");
		return;
	}

	INT nIndex = 0;
	for (list<tstring>::iterator begin = listProto.begin(); begin !=  listProto.end(); ++begin,++nIndex)
	{
		if (nIndex < GODEVIL_SKILL_LEVEL)
			m_lstReputeLevel[nIndex]= varForceLevelProto->GetInt(_T("point"), begin->c_str());
	}
	KillObj("ForceLevel");
}

const tagForceSkill* ReputeMgr::GetForceSkillInfo(DWORD dwSkillID)
{
	map<DWORD,tagForceSkill>::iterator iter = m_mapForceSkill.find(dwSkillID);
	if (iter != m_mapForceSkill.end())
	{
		return &(iter->second);
	}
	return NULL;
}

vector<DWORD> ReputeMgr::GetLocalPlayerForceSkillList(EReputeType eType,DWORD dwLevel)
{
	vector<DWORD> lstRet;
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return lstRet;

	map<DWORD,tagForceSkill>::iterator iter = m_mapForceSkill.begin();
	for (;iter != m_mapForceSkill.end();iter++)
	{
		if (pLp->VocationCheck(iter->second.dwClassTypeLimited) && iter->second.dwReputeLevel == dwLevel && eType == iter->second.eType)
		{
			lstRet.push_back(iter->first);
		}
	}
	return lstRet;

}

DWORD ReputeMgr::GetForceLevelPoint(INT nLevel)
{
	if (nLevel >=0 && nLevel < GODEVIL_SKILL_LEVEL)
	{
		return m_lstReputeLevel[nLevel];
	}
	return GT_INVALID;
}

tagHunJingName* ReputeMgr::FindHunJingName(DWORD dwID)
{
	map<DWORD,tagHunJingName>::iterator it = m_mapHunJingName.find(dwID);
	if (it != m_mapHunJingName.end())
	{
		return &(it->second);
	}

	return NULL;
}
tagHunJingLevel* ReputeMgr::FindHunJingLevelInfo(INT nLevel )
{
	map<INT,tagHunJingLevel>::iterator it = m_mapHunJingLevel.find(nLevel);
	if (it != m_mapHunJingLevel.end())
	{
		return &(it->second);
	}
	return NULL;
}

DWORD ReputeMgr::OnNS_SoulCrystalGrow(tagNS_SoulCrystalGrow* pCmd, DWORD)
{
	if (pCmd->dwErrorCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_LevelUpSuccess")]);
	}
	else if (pCmd->dwErrorCode == E_SoulCrystal_Grow_MaxLvl)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_SoulCrystal_Grow_MaxLvl")]);
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_LevelUpFail")]);

	return 0;
}
DWORD ReputeMgr::OnNS_SoulCrystalRenew(tagNS_SoulCrystalRenew* pCmd, DWORD)
{
	if (pCmd->dwErrorCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_ResetAttSuccess")]);
	}
	else if (pCmd->dwErrorCode == E_SoulCrystal_Renew_ItemInvalid)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_SoulCrystal_Renew_ItemInvalid")]);
	}
	else if (pCmd->dwErrorCode == E_SoulCrystal_Renew_IMLimit)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_SoulCrystal_Renew_IMLimit")]);
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_ResetAttFail")]);

	return 0;
}

DWORD ReputeMgr::GetGEReputeLevel(EReputeType eType)
{
	if (MIsValidReputeType(eType))
	{
		for (INT i = 0; i < GODEVIL_SKILL_LEVEL; i++)
		{
			if (m_dwGERepute[eType]<m_lstReputeLevel[i])
				return max(0,i - 1);
		}
		// 超出最高等级了
		return GODEVIL_SKILL_LEVEL - 1;
	}

	return 0;
}