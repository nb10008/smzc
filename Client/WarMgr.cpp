#include "StdAfx.h"
#include "WarMgr.h"
#include "RoleMgr.h"
#include "RoleEvent.h"

WarMgr g_WarMgr;
WarMgr* WarMgr::Inst()
{
	return &g_WarMgr;
}

WarMgr::WarMgr(void):m_trunk(this)
{
	m_dwRoleID = GT_INVALID;
}
WarMgr::~WarMgr(void)
{

}

/** 进入游戏世界时初始化
*/
void WarMgr::Init()
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();

	m_dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();

	//注册网络消息
	m_pCmdMgr->Register("NS_SyncRoleAttr",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_SyncRoleAttr),			_T("NS_SyncRoleAttr"));
	m_pCmdMgr->Register("NS_SyncRoleAttrToOther",	(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_SyncRoleAttrToOther),	_T("NS_SyncRoleAttrToOther"));
	m_pCmdMgr->Register("NS_ShowWarResult",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_ShowWarResult),			_T("NS_ShowWarResult"));
	m_pCmdMgr->Register("NS_RefreshBaoXiang",		(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_RefreshBaoXiang),		_T("NS_RefreshBaoXiang"));
	m_pCmdMgr->Register("NS_GetWarBaoXiang",		(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_GetWarBaoXiang),			_T("NS_GetWarBaoXiang"));
}
/** 离开游戏世界时释放所有数据
*/
void WarMgr::Destroy()
{
	m_pCmdMgr->UnRegister("NS_SyncRoleAttr",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_SyncRoleAttr));
	m_pCmdMgr->UnRegister("NS_SyncRoleAttrToOther",		(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_SyncRoleAttrToOther));
	m_pCmdMgr->UnRegister("NS_ShowWarResult",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_ShowWarResult));
	m_pCmdMgr->UnRegister("NS_RefreshBaoXiang",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_RefreshBaoXiang));
	m_pCmdMgr->UnRegister("NS_GetWarBaoXiang",			(NETMSGPROC)m_trunk.sfp2(&WarMgr::OnNS_GetWarBaoXiang));
}

DWORD WarMgr::OnNS_SyncRoleAttr(tagNS_SyncRoleAttr* pCmd, DWORD)
{
	std::map<DWORD, tagWarnfo>::iterator it = m_mapWarInfo.find(m_dwRoleID);
	if(it != m_mapWarInfo.end())	//找到了
	{
		(*it).second.AWarInfo[pCmd->eAttr] = pCmd->nVal;
	}
	else		//未找到
	{
		tagWarnfo warInfo;
		warInfo.AWarInfo[pCmd->eAttr] = pCmd->nVal;
		m_mapWarInfo.insert(make_pair(m_dwRoleID, warInfo));
	}

	//todo发送客户端更新事件
	tagRefreshWarInfo evt(_T("RefreshWarInfo_Event"), NULL);
	evt.dwRoleID = m_dwRoleID;
	evt.eType = pCmd->eAttr;
	evt.dwValue = pCmd->nVal;
	m_pFrameMgr->SendEvent(&evt);

	return 0;
}

DWORD WarMgr::OnNS_SyncRoleAttrToOther(tagNS_SyncRoleAttrToOther* pCmd, DWORD)
{
	std::map<DWORD, tagWarnfo>::iterator it = m_mapWarInfo.find(pCmd->dwRoleID);
	if(it != m_mapWarInfo.end())	//找到了
	{
		(*it).second.AWarInfo[pCmd->eAttr] = pCmd->nVal;
	}
	else		//未找到
	{
		tagWarnfo warInfo;
		warInfo.AWarInfo[pCmd->eAttr] = pCmd->nVal;
		m_mapWarInfo.insert(make_pair(pCmd->dwRoleID, warInfo));
	}

	//todo发送客户端更新事件
	tagRefreshWarInfo evt(_T("RefreshWarInfo_Event"), NULL);
	evt.dwRoleID = pCmd->dwRoleID;
	evt.eType = pCmd->eAttr;
	evt.dwValue = pCmd->nVal;
	m_pFrameMgr->SendEvent(&evt);

	return 0;
}

DWORD WarMgr::OnNS_RefreshBaoXiang(tagNS_RefreshBaoXiang* pCmd, DWORD)
{
	m_sResult.dwItem1 = pCmd->dwItemID1;
	m_sResult.dwItem2 = pCmd->dwItemID2;
	m_sResult.dwItem3 = pCmd->dwItemID3;
	if((EWarRoleResult)pCmd->byErrorCode == EWRR_Lose)
		m_sResult.bIsSuccess = FALSE;
	else
		m_sResult.bIsSuccess = TRUE;

	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Show_WarResult"), NULL));

	return 0;
}

DWORD WarMgr::OnNS_ShowWarResult(tagNS_ShowWarResult* pCmd, DWORD)
{
	m_sResult.dwItem1 = pCmd->dwItem1ID;
	m_sResult.dwItem2 = pCmd->dwItem2ID;
	m_sResult.dwItem3 = pCmd->dwItem3ID;
	m_sResult.bIsSuccess = pCmd->byIsSuccess;

	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Show_WarResult"), NULL));

	return 0;
}

DWORD WarMgr::OnNS_GetWarBaoXiang(tagNS_GetWarBaoXiang* pCmd, DWORD)
{
	m_mapWarInfo.clear();
	m_sResult.bIsSuccess = FALSE;
	m_sResult.dwItem1 = GT_INVALID;
	m_sResult.dwItem2 = GT_INVALID;
	m_sResult.dwItem3 = GT_INVALID;

	m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_WarFrame"), NULL));

	return 0;
}

void WarMgr::ClearData()
{
	std::map<DWORD, tagWarnfo>::iterator it;
	for(it = m_mapWarInfo.begin(); it != m_mapWarInfo.end(); ++it)
	{
		for(int i = ERoleAttrWarGold; i < ERoleAttrEnd; i++)
		{
			(*it).second.AWarInfo[i] = 0;
		}
	}
}

DWORD WarMgr::GetWarMoneyNum()
{
 	std::map<DWORD, tagWarnfo>::iterator it = m_mapWarInfo.find(m_dwRoleID);
 	if(it != m_mapWarInfo.end())
 	{
 		return (*it).second.AWarInfo[ERoleAttrWarGold];
 	}

	return 0;
}