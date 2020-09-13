//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_mgr.cpp
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: 城市系统管理
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "../ServerDefine/msg_city.h"
#include "../WorldDefine/msg_city.h"

#include "city_mgr.h"
#include "city.h"
#include "att_res.h"
#include "db_session.h"
#include "unit.h"

CityMgr g_cityMgr;

//-------------------------------------------------------------------------
// 构造和析构
//-------------------------------------------------------------------------
CityMgr::CityMgr()
{
	m_mapCity.Clear();
}

CityMgr::~CityMgr()
{
	Destory();
}

//-------------------------------------------------------------------------
// 初始化、更新、销毁
//-------------------------------------------------------------------------
BOOL CityMgr::Init()
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("signupforcitywar"), m_Trunk.sfp4(&CityMgr::SignupForCityWar), _T("Signup For City War"), 4);

	m_mapCity.Clear();

	// 取得城市ID范围
	DWORD dwCityStart	= GT_INVALID;
	DWORD dwCityEnd		= GT_INVALID;
	g_attRes.GetCityIDRange(dwCityStart, dwCityEnd);
	if (!GT_VALID(dwCityStart) || !GT_VALID(dwCityEnd))
	{
		return TRUE;	// 没有城市信息
	}

	// 初始化城市
	for (DWORD dwID = dwCityStart; dwID <= dwCityEnd; dwID++)
	{
		City* pCity = new City;
		if (!pCity->Init(dwID))
		{
			IMSG(_T("city init failed cityid:%d\r\n"), dwID);
			SAFE_DEL(pCity);
			continue;
		}
		m_mapCity.Add(dwID, pCity);
	}

	return TRUE;
}

VOID CityMgr::Update()
{
	CHECK_TIME()
	tagDWORDTime tm = g_world.GetWorldTime();

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (P_VALID(pCity))
		{
			pCity->Update(tm);
		}
	}
}

VOID CityMgr::Destory()
{
	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, pCity))
	{
		SAFE_DEL(pCity);
	}
	m_mapCity.Clear();
}

//-------------------------------------------------------------------------
// 检查数据库中城市信息
//-------------------------------------------------------------------------
VOID CityMgr::RequestCityInfoFromDB()
{
	if (m_mapCity.Empty())
	{
		return;
	}

	// 创建消息
	INT nMsgSize = sizeof(tagNDBC_CreateCityInfo) + sizeof(DWORD) * (m_mapCity.Size() - 1);
	MCREATE_MSG(pSend, nMsgSize, NDBC_CreateCityInfo);

	City* pCity		= NULL;
	DWORD dwCityID	= GT_INVALID;
	MapCity::TMapIterator iter = m_mapCity.Begin();
	while (m_mapCity.PeekNext(iter, dwCityID, pCity))
	{
		if (P_VALID(pCity) && GT_VALID(dwCityID))
		{
			pSend->dwCityID[pSend->nCityNum++] = dwCityID;
		}
	}
	if (pSend->nCityNum > 0)
	{
		// 重新计算消息大小
		pSend->dwSize = sizeof(tagNDBC_CreateCityInfo) + sizeof(DWORD) * (pSend->nCityNum - 1);

		g_dbSession.Send(pSend, pSend->dwSize);

		// 申请每一个城市信息
		for (int n=0; n<pSend->nCityNum; n++)
		{
			tagNDBC_LoadCityInfo send;
			send.dwCityID = pSend->dwCityID[n];
			g_dbSession.Send(&send, send.dwSize);
		}
	}
}

//-------------------------------------------------------------------------
// 检查帮派报名信息
//-------------------------------------------------------------------------
BOOL CityMgr::IsAlreadySignup( DWORD dwGuildID, ECityForceType eForce )
{
	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (P_VALID(pCity) && pCity->IsAlreadySignup(dwGuildID, eForce))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------
// 查找指定氏族所在城市
//-------------------------------------------------------------------------
City* CityMgr::GetCity( ECLanType eType )
{
	City* pCity = NULL;
	INT nNum = 0;
	ECLanType eClanType;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		pCity->GetCityAppointedAtt(ECAT_ClanType, nNum, &eClanType);
		if ((nNum > 0) && (eClanType == eType))
		{
			return pCity;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
// 根据地图指针得到城市
//-------------------------------------------------------------------------
City* CityMgr::GetCity( Map* pMap )
{
	if (!P_VALID(pMap))
	{
		return NULL;
	}
	City* pCity = NULL;
	INT nNum = 0;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!(P_VALID(pCity) && pCity->IsInitOK()))
			continue;

		if ((pMap->GetMapID() == pCity->GetShellCity()->GetMapID())
			|| pMap->GetMapID() == pCity->GetKernelCity()->GetMapID())
		{
			return pCity;
		}
	}

	return NULL;

}
//-------------------------------------------------------------------------
// 查找指定单位所在城市
//-------------------------------------------------------------------------
City* CityMgr::GetCity( Unit* pUnit )
{
	if (!P_VALID(pUnit))
	{
		return NULL;
	}

	Map* pMap = pUnit->GetMap();
	if (!P_VALID(pMap))
	{
		return NULL;
	}

	City* pCity = NULL;
	INT nNum = 0;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!(P_VALID(pCity) && pCity->IsInitOK()))
			continue;

		if ((pMap->GetMapID() == pCity->GetShellCity()->GetMapID())
			|| (pMap->GetMapID() == pCity->GetKernelCity()->GetMapID()))
		{
			return pCity;
		}
	}

	return NULL;
}
//-------------------------------------------------------------------------
// 得到城市数目
//-------------------------------------------------------------------------
DWORD CityMgr::GetCityNum()
{
	return m_mapCity.Size();
}

//-------------------------------------------------------------------------
// 得到所有的城市税率
//-------------------------------------------------------------------------
VOID CityMgr::GetAllCityTaxRate(LPVOID pData)
{
	INT nDataNum = 0;

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		//得到cityid和税率
		INT nTemp = 0;
		pCity->GetCityAppointedAtt(ECAT_CityID,nTemp,&((tagCityTaxRate*)pData)[nDataNum].byCityID);
		pCity->GetCityAppointedAtt(ECAT_TaxRate,nTemp,&((tagCityTaxRate*)pData)[nDataNum].byTaxRate);
		++nDataNum;
	}
}
//-------------------------------------------------------------------------
// 得到城战城市的ID
//-------------------------------------------------------------------------
VOID CityMgr::GetCityWarCity(tagNetCmd* pCmd)
{
	MGET_MSG(pSend, pCmd, NS_GetCityWarCity);

	DWORD dwNum = 0;
	tagDWORDTime tm = GetCurrentDWORDTime();

	City* pCity = NULL;
	MapCity::TMapIterator iter = m_mapCity.Begin();

	while (m_mapCity.PeekNext(iter, pCity))
	{
		if (!P_VALID(pCity))
			continue;

		INT nDataNum = 0;
		pCity->GetCityAppointedAtt(ECAT_CityID,nDataNum,&pSend->dwCityID[dwNum++]);
	}
	
	//当前城战中城市的数目
	pSend->nNum = dwNum;
}

// 城战报名（控制台命令用）
VOID CityMgr::SignupForCityWar(LPCTSTR szCityID, LPCTSTR szGuildID, LPCTSTR szForceType, LPCTSTR szFare)
{
	DWORD				dwCityID = _tcstoul(szCityID, NULL, 10);
	DWORD				dwGuildID = _tcstoul(szGuildID, NULL, 10);			// 报名帮派ID
	ECityForceType		eCityForceType = static_cast<ECityForceType>(_ttoi(szForceType));	// 报名参与势力
	INT32				nFare = _tcstoul(szFare, NULL, 10);					// 报名费(当联盟为防御时代表是否经过承认)

	City* pCity = m_mapCity.Peek(dwCityID);
	if (!P_VALID(pCity))
	{
		IMSG(_T("city id invalid or city is not exist!! \r\n"));
		return;
	}

 	if (GT_INVALID != pCity->SignupForCitywar(dwGuildID, eCityForceType, nFare))
	{
		IMSG(_T("Signup For City War Success!!!\r\n"));
	}

}
