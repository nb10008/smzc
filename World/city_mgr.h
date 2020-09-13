//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_mgr.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: 城市系统管理
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"

class City;
class Unit;

class CityMgr
{
public:
	typedef TMap<DWORD, City*>	MapCity;

public:
	CityMgr();
	~CityMgr();

	BOOL	Init();
	VOID	Update();
	VOID	Destory();

	City*	GetCity(DWORD dwCityID)			{ return m_mapCity.Peek(dwCityID); }
	City*	GetCity(ECLanType eType);
	City*	GetCity(Unit* pUnit);
	City*	GetCity(Map* pMap);

	// 检查数据库中城市信息
	VOID	RequestCityInfoFromDB();

	// 检查帮派报名信息
	BOOL	IsAlreadySignup(DWORD dwGuildID, ECityForceType eForce);

	// 帮派解散处理
	VOID	OnGuildDismissed(DWORD dwGuildID);

	// 得到城市数
	DWORD	GetCityNum();

	// 得到城市税率
	VOID	GetAllCityTaxRate(LPVOID pData);

	// 得到城战中的城市id
	VOID GetCityWarCity(tagNetCmd* pCmd);

private:

	// 城战报名（控制台命令用）
	VOID SignupForCityWar(LPCTSTR szCityID, LPCTSTR szGuildID, LPCTSTR szForceType, LPCTSTR szFare);

private:
	TSFPTrunk<CityMgr> m_Trunk;

	MapCity		m_mapCity;
};

extern CityMgr g_cityMgr;
