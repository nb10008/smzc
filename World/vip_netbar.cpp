#include "stdafx.h"
#include "vip_netbar.h"

#include "../WorldDefine/msg_vip_netbar.h"

#include "../ServerDefine/vip_netbar_define.h"
#include "../ServerDefine/msg_vip_netbar.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "world_session.h"
#include "att_res.h"
#include "player_session.h"
#include "db_session.h"
#include "item_creator.h"
#include "item_mgr.h"
#include "script_mgr.h"
#include "Role.h"
#include "buff.h"
#include "../ServerDefine/login_issue.h"
#include "../ServerDefine/mini_tool.h"


VipNerBarMgr g_VipNetBarMgr;
class IPComp
{
public:
	bool operator()(const IpRange& left, const IpRange& right)
	{
		return left.GetIpMin() < right.GetIpMin();
	}
};


// tbc:
BOOL VipNerBarMgr::Init()
{
	m_setNotify.clear();
	m_mapVipNetBars.Clear();
	m_vecIpRanges.clear();
	m_mapIp2VNBId.Clear();
	m_bOpen = FALSE;
	_INetbarBuffs.clear();
	_INetbarInstBuffs.clear();

	// 生成掉落容器
	TObjRef<VarContainer> pVar;
	CreateObj("VipNetBar", "VarContainer");

	// 从配置文件 world.ini 中得到掉落文件路径
	pVar = "VarContainer";

	// 得到配置文件中怪物掉落表的路径
	tstring strVNBTablePath	= pVar->GetString(_T("path"),	_T("vip_netbar"));
	m_bOpen = pVar->GetInt(_T("open"),_T("vip_netbar"),0);
	ILOG->Write(_T("VipNerBar<<VipNerBarMgr::Init<<m_bOpen=%d\r\n"), m_bOpen);

	// 加载怪物掉落文件
	pVar = "VipNetBar";
	std::list<tstring> listField;
	if (!pVar->Load(NULL, strVNBTablePath.c_str(), "id", &listField))
	{
		pVar->Clear();
		KillObj("VipNetBar");
		return FALSE;
	}
	;

	// 一个一个的加载怪物掉落文件
	for(std::list<tstring>::iterator it = listField.begin(); it != listField.end(); ++it)
	{
		tagVipNetBar* pVNB = new tagVipNetBar;
		ZeroMemory(pVNB, sizeof(tagVipNetBar));

		pVNB->dwID = _tcstol(it->c_str(), NULL, 10);
		_tcsncpy(pVNB->szName, pVar->GetString(_T("name"), it->c_str(), NULL), X_SHORT_NAME);

		if (P_VALID(m_mapVipNetBars.Peek(pVNB->dwID)))
			continue;

		pVNB->dwBuffID			= pVar->GetDword(_T("buffid")		, it->c_str(), GT_INVALID);
		pVNB->dwInstBuffID	= pVar->GetDword(_T("inst_buffid")	, it->c_str(), GT_INVALID);

		m_mapVipNetBars.Add(pVNB->dwID, pVNB);
		
		LPCTSTR tszIpMin = NULL;
		LPCTSTR tszIpMax = NULL;
		DWORD dwIpMin = GT_INVALID;
		DWORD dwIpMax = GT_INVALID;

		for(INT i = 0; i < MAX_VNB_IP_NUM; ++i)
		{
			tstringstream ss;
			ss << _T("ipmin") << i+1;
			tszIpMin = pVar->GetString(ss.str().c_str(), it->c_str(), 0);
			if (!P_VALID(tszIpMin))
				break;
			
			ss.str(_T(""));
			ss << _T("ipmax") << i+1;
			tszIpMax = pVar->GetString(ss.str().c_str(), it->c_str(), 0);
			if (!P_VALID(tszIpMax))
				break;

			dwIpMin = TransTSIp2DWORD(tszIpMin);
			GeneralzeIP(dwIpMin);

			dwIpMax = TransTSIp2DWORD(tszIpMax);
			GeneralzeIP(dwIpMax);

			ASSERT(dwIpMin <= dwIpMax);
			if(dwIpMin > dwIpMax)
				continue;

			m_vecIpRanges.push_back(IpRange(dwIpMin, dwIpMax, pVNB->dwID));
		}
	}

	std::sort(m_vecIpRanges.begin(), m_vecIpRanges.end(), IPComp());
	pVar->Clear();
	KillObj("VipNetBar");

	return TRUE;
}

VOID	VipNerBarMgr::ReloadVipNetbarFromLoginSever(VOID * pLoginMsg,INT num)
{
	// 该处需要注意，没有将所有的容器清空的。。。
	Destroy();
	m_vecIpRanges.clear();
	_INetbarBuffs.clear();
	_INetbarInstBuffs.clear();

	tagNLW_IssueVipNetbarInfo * pMsg = (tagNLW_IssueVipNetbarInfo *)pLoginMsg;
	if( !P_VALID(pMsg) || num <= 0 )
	{
		ILOG->Write(_T("VipNerBar<<VipNerBarMgr::ReloadVipNetbarFromLoginSever<<pMsg Error num=%d！！！\r\n"), num);
		return;
	}

	for( INT i = 0; i < num ; ++i )
	{
		tagVipNetBar* pVNB = new tagVipNetBar;
		ZeroMemory(pVNB, sizeof(tagVipNetBar));
		pVNB->dwID			= pMsg->data[i].dwID;
		pVNB->nPlayerNum	= 0;
		jm_tscopy_s(pVNB->szName,_countof(pVNB->szName),pMsg->data[i].szName,_countof(pMsg->data[i].szName));
		pVNB->dwBuffID		= pMsg->data[i].dwBuffID;
		pVNB->dwInstBuffID= pMsg->data[i].dwInstBuffID;

		m_mapVipNetBars.Add(pVNB->dwID, pVNB);
		if(P_VALID(pMsg->data[i].dwBuffID))
			_INetbarBuffs.insert(pMsg->data[i].dwBuffID);
		if(P_VALID(pMsg->data[i].dwInstBuffID))
			_INetbarInstBuffs.insert(pMsg->data[i].dwInstBuffID);
		//IMSG(_T("load inetcafes%d\r\n"),num);

		DWORD dwIpMin = GT_INVALID;
		DWORD dwIpMax = GT_INVALID;
		INT size = 0;
		for( INT j = 0; j < MAX_VNB_IP_NUM; ++j )
		{
			size = _countof(pMsg->data[i].IpStrRange[j].strIPMin);
			if( size < 1 )
				continue;
			pMsg->data[i].IpStrRange[j].strIPMin[size-1] = 0;
			if( pMsg->data[i].IpStrRange[j].strIPMin[0] == 0)
				continue;

			dwIpMin = TransTSIp2DWORD(pMsg->data[i].IpStrRange[j].strIPMin);
			GeneralzeIP(dwIpMin);

			size = _countof(pMsg->data[i].IpStrRange[j].strIPMax);
			if( size < 1 )
				continue;
			pMsg->data[i].IpStrRange[j].strIPMax[size-1] = 0;
			if( pMsg->data[i].IpStrRange[j].strIPMax[0] == 0)
				continue;

			dwIpMax = TransTSIp2DWORD(pMsg->data[i].IpStrRange[j].strIPMax);
			GeneralzeIP(dwIpMax);
			if(dwIpMin > dwIpMax)
				continue;

			m_vecIpRanges.push_back(IpRange(dwIpMin, dwIpMax, pVNB->dwID));
		}
	}
	std::sort(m_vecIpRanges.begin(), m_vecIpRanges.end(), IPComp());
}

VOID VipNerBarMgr::UpdateDbPlayerLogin(DWORD dwAccountID, DWORD dwTime)
{
	tagDWORDTime dwLoginDate = dwTime;
	dwLoginDate.min = 0;
	dwLoginDate.sec = 0;
	dwLoginDate.hour = 0;
	tagNDBC_UpdateVNBPlayer send;
	send.dwAccountID = dwAccountID;
	send.dwLoginTime = dwLoginDate;
	g_dbSession.Send(&send, send.dwSize);
}

VOID VipNerBarMgr::InitData(tagDBVNBPlayers* pInitData, BOOL bFirst)
{
	if(TRUE == bFirst)
		m_setHistoryAccountID.clear();
	
	for (INT i=0; i<pInitData->nHisPlayers; ++i)
	{
		DWORD dwHisPlayerId = pInitData->dwAccountIDs[i];
		if (m_setHistoryAccountID.find(dwHisPlayerId) == m_setHistoryAccountID.end())
		{
			m_setHistoryAccountID.insert(dwHisPlayerId);
		}
	}
	
	if(TRUE == bFirst)
		m_setTodayAccountID.clear();
	
	for (INT i=0; i<pInitData->nTodaysPlayers; ++i)
	{
		DWORD dwTodayPlayerId = pInitData->dwAccountIDs[i + pInitData->nHisPlayers];
		if (m_setTodayAccountID.find(dwTodayPlayerId) == m_setTodayAccountID.end())
		{
			m_setTodayAccountID.insert(dwTodayPlayerId);
		}
	}
}


DWORD VipNerBarMgr::FitNetBar(DWORD dwIp)
{
	INT nLeft = 0;
	INT nRight = m_vecIpRanges.size() - 1;
	INT	nMiddle = 0;

	while (nLeft <= nRight)
	{
		nMiddle = (nLeft + nRight) / 2;
		if (m_vecIpRanges[nMiddle].Fit(dwIp))
		{
			return m_vecIpRanges[nMiddle].GetVNBId();
		}
		if (m_vecIpRanges[nMiddle].OnRightOf(dwIp))
		{
			nRight = nMiddle - 1;
		}
		else if (m_vecIpRanges[nMiddle].OnLeftOf(dwIp))
		{
			nLeft = nMiddle + 1;
		}
	}

	return GT_INVALID;
}

BOOL	VipNerBarMgr::RecReward(Role* pRole)
{
	if( !P_VALID(pRole) )
		return FALSE;
	PlayerSession * pSession = pRole->GetSession();
	if( !P_VALID(pSession) )
		return FALSE;
	DWORD dwIP = pSession->GetCurLoginIP();

	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (!P_VALID(pVNB))
	{
		ILOG->Write(_T("VipNerBar<<VipNerBarMgr::RecReward<<dwIP not in m_mapVipNetBars,ip=%d！！！\r\n"),dwIP);
		return FALSE;
	}
	BOOL re = FALSE;
	if( P_VALID(pVNB->dwBuffID) && !pRole->IsHaveBuff( Buff::GetIDFromTypeID(pVNB->dwBuffID) ) )
	{
		pRole->TryAddBuffToMyself(pVNB->dwBuffID);
		re = TRUE;
	}
	if( P_VALID(pVNB->dwInstBuffID) )
	{
		pRole->SetVipNetbarInstBuffID(pVNB->dwInstBuffID);
		re = TRUE;
	}

	return re;
}
BOOL	VipNerBarMgr::IsInVipNetbar(Role* pRole,DWORD * pdwBuff,DWORD * pdwInstBuffID )
{
	if( !P_VALID(pRole) )
		return FALSE;
	PlayerSession * pSession = pRole->GetSession();
	if( !P_VALID(pSession) )
		return FALSE;
	DWORD dwIP = pSession->GetCurLoginIP();

	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (!P_VALID(pVNB))
	{
		return FALSE;
	}

	if( P_VALID(pdwBuff) )
		*pdwBuff = pVNB->dwBuffID;
	if( P_VALID(pdwInstBuffID) )
		*pdwInstBuffID = pVNB->dwInstBuffID;

	return TRUE;
}
BOOL	VipNerBarMgr::AddInstBuff(Role * pRole)
{
	if( !P_VALID(pRole) )
		return FALSE;
	PlayerSession * pSession = pRole->GetSession();
	if( !P_VALID(pSession) )
		return FALSE;
	DWORD dwIP = pSession->GetCurLoginIP();

	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (!P_VALID(pVNB))
	{
		ILOG->Write(_T("VipNerBar<<VipNerBarMgr::AddInstBuff<<dwIP not in m_mapVipNetBars！！！\r\n"));
		return FALSE;
	}

	if( P_VALID(pVNB->dwInstBuffID) && !pRole->IsHaveBuff( Buff::GetIDFromTypeID(pVNB->dwInstBuffID) ) )
	{
		pRole->TryAddBuffToMyself(pVNB->dwInstBuffID);
		return TRUE;
	}
	return FALSE;
}

BOOL	VipNerBarMgr::RemoveInstBuff(Role * pRole)
{
	if( !P_VALID(pRole) )
		return FALSE;
	PlayerSession * pSession = pRole->GetSession();
	if( !P_VALID(pSession) )
		return FALSE;
	DWORD dwIP = pSession->GetCurLoginIP();

	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (!P_VALID(pVNB))
	{
		ILOG->Write(_T("VipNerBar<<VipNerBarMgr::RemoveInstBuff<<dwIP not in m_mapVipNetBars！！！\r\n"));
		return FALSE;
	}

	if( P_VALID(pVNB->dwInstBuffID) && pRole->IsHaveBuff( Buff::GetIDFromTypeID(pVNB->dwInstBuffID) ) )
	{
		pRole->RemoveBuff(pVNB->dwInstBuffID,TRUE);
		return TRUE;
	}
	return FALSE;
}

VOID VipNerBarMgr::PlayerLogin( DWORD dwAccountID, DWORD dwIP )
{
	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (!P_VALID(pVNB))
	{
		return;
	}

	pVNB->OnPlayerLogin();
	// UpdateDbPlayerLogin(dwAccountID, GetCurrentDWORDTime());

	// 注销掉原来的做法
#if 0
	if (m_setTodayAccountID.find(dwAccountID) == m_setTodayAccountID.end())
	{
		const tagVNBGiftProto* pGiftProto = g_attRes.GetRandVNBGiftProto();
		if (P_VALID(pGiftProto))
		{
			tagItem* pItem = ItemCreator::Create(EICM_VipNetBar, GT_INVALID, pGiftProto->dwItemTypeID, pGiftProto->nNum);
			if(P_VALID(pItem))
			{
				// 存储到item_baibao表中
				ItemMgr::InsertBaiBao2DBEx(pItem, dwAccountID, ELCLD_VIP_NetBar);
				ItemMgr::ProcBaiBaoRecord(pItem->dwTypeID, 
					GT_INVALID, GT_INVALID, EBBRT_VipNetBarGift);
				// 删除物品
				::Destroy(pItem);

				// 今日礼物
				m_setTodayAccountID.insert(dwAccountID);

				m_setNotify.insert(dwAccountID);
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	if (m_setHistoryAccountID.find(dwAccountID) == m_setHistoryAccountID.end())
	{
		std::list<tagVNBEquipProto*> listEquips;
		g_attRes.GetRandVNBEquipProto(listEquips);
		while (!listEquips.empty())
		{
			const tagVNBEquipProto* pEquipProto = listEquips.front();
			listEquips.pop_front();

			if (P_VALID(pEquipProto))
			{
				tagItem* pEquip = ItemCreator::CreateEx(EICM_VipNetBar, GT_INVALID, pEquipProto->dwEquipTypeID, 1, (EItemQuality)pEquipProto->nQuality);
				if (P_VALID(pEquip))
				{
					ItemMgr::InsertBaiBao2DBEx(pEquip , dwAccountID, ELCLD_VIP_NetBar);
					// 删除物品
					::Destroy(pEquip );

					// 网吧装备
					m_setHistoryAccountID.insert(dwAccountID);			
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
#endif
}

VOID VipNerBarMgr::PlayerLogout(DWORD dwIP)
{
	GeneralzeIP(dwIP);
	// 网吧在线人数
	tagVipNetBar* pVNB = GetVipNetBar(dwIP);
	if (P_VALID(pVNB))
	{
		pVNB->OnPlayerLogout();
	}
}


DWORD VipNerBarMgr::GetVNBId(DWORD dwIp)
{
	DWORD dwVNBId = m_mapIp2VNBId.Peek(dwIp);
	if (!GT_VALID(dwVNBId))
	{
		dwVNBId = FitNetBar(dwIp);
		if (P_VALID(dwVNBId))
		{
			m_mapIp2VNBId.Add(dwIp, dwVNBId);
		}
	}		
	return dwVNBId;
}

VOID VipNerBarMgr::Destroy()
{
	VipNetBarMap::TMapIterator itor = m_mapVipNetBars.Begin();
	tagVipNetBar* pVipNetBar = NULL;
	while(m_mapVipNetBars.PeekNext(itor, pVipNetBar))
	{
		SAFE_DELETE(pVipNetBar);
	}
	m_mapVipNetBars.Clear();
}

struct RateItem
{
	INT		nMinNum;
	INT		nRate;
};
const INT NUM_VNB_EXP_RATE	= 5;
const INT NUM_VNB_LOOT_RATE	= 5;
RateItem ExpRate[NUM_VNB_EXP_RATE] =
{
	{	1,		500		},
	{	5,		1000	},
	{	10,		1500	},
	{	15,		2000	},
	{	20,		2500	},
};

RateItem LootRate[NUM_VNB_LOOT_RATE] =
{
	{	1,		200		},
	{	5,		400	},
	{	10,		800	},
	{	15,		1200	},
	{	20,		1500	},
};



INT VipNerBarMgr::GetRate( DWORD dwIP, INT nType)
{
	return 0;
	GeneralzeIP(dwIP);
	const tagVipNetBar* pVnb = GetVipNetBar(dwIP);
	if (!P_VALID(pVnb))
	{
		return 0;
	}
	
	RateItem* pArr = (nType == 0 ? ExpRate : (nType == 1 ? LootRate : NULL));
	if(!P_VALID(pArr))
	{
		return 0;
	}

	for (INT i=NUM_VNB_EXP_RATE - 1; i>=0; --i)
	{
		if (pVnb->nPlayerNum > pArr[i].nMinNum )
		{
			return pArr[i].nRate;
		}
	}
	
	return 0;
}

LPCTSTR VipNerBarMgr::GetVNBName( DWORD dwIP )
{
	GeneralzeIP(dwIP);
	const tagVipNetBar* pVnb = GetVipNetBar(dwIP);
	if (!P_VALID(pVnb))
	{
		return 0;
	}

	return pVnb->szName;
}

tagVipNetBar* VipNerBarMgr::GetVipNetBar( DWORD dwIP )
{
	DWORD dwVNBId = GetVNBId(dwIP);
	return m_mapVipNetBars.Peek(dwVNBId);
}

BOOL VipNerBarMgr::DynamicTest(DWORD dwTestNo, DWORD dwArg1, LPCTSTR szArg2)
{
	if (!P_VALID(szArg2))
	{
		return FALSE;
	}

	DWORD dwIp = TransTSIp2DWORD(szArg2);
	switch(dwTestNo)
	{
	case 0:
		PlayerLogin(dwArg1, dwIp);
		break;
	case 1:
		PlayerLogout(dwIp);
		break;
	case 2:
		INT nRate0 = GetRate(dwIp, 0);
		INT nRate1 = GetRate(dwIp, 1);
		LPCTSTR szName = GetVNBName(dwIp);
		break;
	}
	return TRUE;
}

VOID VipNerBarMgr::GeneralzeIP( DWORD &dwIP )
{
	DWORD dwTemp = 0;

	dwTemp |= (dwIP >> 24) & 0x000000ff;
	dwTemp |= (dwIP >> 8) & 0x0000ff00;
	dwTemp |= (dwIP << 8) & 0x00ff0000;
	dwTemp |= (dwIP << 24) & 0xff000000;

	dwIP = dwTemp;
}

DWORD VipNerBarMgr::TransTSIp2DWORD( LPCTSTR tszIP )
{
	TObjRef<StreamTransport> pTrans = "StreamTransport";
	TObjRef<Util> pUtil = "Util";
	
	LPSTR szIp = (LPSTR)pUtil->UnicodeToAnsi(tszIP);
	DWORD dwIp = pTrans->StringIP2IP(szIp);
	
	return dwIp;
}

VOID VipNerBarMgr::PlayerNotify( DWORD dwAccountID )
{
	if (m_setNotify.find(dwAccountID) != m_setNotify.end())
	{
		m_setNotify.erase(dwAccountID);
		
		PlayerSession* pSession = g_worldSession.FindSession(dwAccountID);
		if (P_VALID(pSession))
		{
//			tagNS_VipNetBarGift send;
//			pSession->SendMessage(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------
// 橘子金牌网吧
//-------------------------------------------------------------------------

volatile BOOL OrangeVipNetBarMgr::m_bOrangeVipBar = FALSE;
volatile INT32 OrangeVipNetBarMgr::m_nIntervalTm = 0;
volatile INT32 OrangeVipNetBarMgr::m_nBonusPoint = 0;

VOID OrangeVipNetBarMgr::Init(const TCHAR RoleName[X_SHORT_NAME],const CHAR Account[X_SHORT_NAME],const DWORD IP) 
{
	IFASTCODE->MemCpy(m_szRoleName,RoleName,sizeof(m_szRoleName));
	IFASTCODE->MemCpy(m_szAccountName,Account,sizeof(m_szAccountName));

	m_dwIP = IP;
}

//开启橘子金牌网吧
VOID OrangeVipNetBarMgr::OpenOrangeVipNetBar()
{
	InterlockedExchange((LONG*)&m_bOrangeVipBar, TRUE);

	INT nTempNum = 0;

	g_ScriptMgr.GetGlobal("orange_VNB_IntervalTime",nTempNum);
	InterlockedExchange((LONG*)&m_nIntervalTm, nTempNum);

	g_ScriptMgr.GetGlobal("orange_VNB_BonusPoint", nTempNum	);
	InterlockedExchange((LONG*)&m_nBonusPoint, nTempNum);
}

//关闭橘子金牌网吧
VOID OrangeVipNetBarMgr::CloseOrangeVipNetBar()
{
	InterlockedExchange((LONG*)&m_bOrangeVipBar, FALSE);

	InterlockedExchange((LONG*)&m_nIntervalTm, 0);

	InterlockedExchange((LONG*)&m_nBonusPoint, 0);

}

//加红利点
VOID OrangeVipNetBarMgr::AddBonusPoint()
{
	if(m_bOrangeVipBar == FALSE)
		return;

	tagNDBC_Orange_AddBonusPoint send;

	IFASTCODE->MemCpy(send.szRoleName,m_szRoleName,sizeof(m_szRoleName));
	IFASTCODE->MemCpy(send.szAccountName,m_szAccountName,sizeof(m_szAccountName));

	send.dwIP = m_dwIP;
	send.nBonusPoint = m_nBonusPoint;

	g_dbSession.Send(&send,send.dwSize);
}

//更新
VOID OrangeVipNetBarMgr::Update()
{
	if(m_bOrangeVipBar == FALSE)
		return;

	++m_nTickToAddBonusPolint;

	if ( m_nTickToAddBonusPolint*TICK_TIME > m_nIntervalTm)
	{
		//加红利点
		AddBonusPoint();

		m_nTickToAddBonusPolint = 0;
	}
}
BOOL	VipNerBarMgr::IsVipNetbarBuff(DWORD dwBuffID)
{
	if(_INetbarBuffs.find(dwBuffID) != _INetbarBuffs.end())
		return TRUE;
	if(_INetbarInstBuffs.find(dwBuffID) != _INetbarInstBuffs.end())
		return TRUE;
	return FALSE;
}
BOOL	VipNerBarMgr::IsVipNetbarInstBuff(DWORD dwInstBuffID)
{
	if(_INetbarInstBuffs.find(dwInstBuffID) != _INetbarInstBuffs.end())
		return TRUE;
	return FALSE;
}
