//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_struggle.h
// author: twan
// actor:
// data: 2009-9-10
// last:
// brief: 城战
//-------------------------------------------------------------------------------
#pragma once

#include "mutex.h"
#include "guild.h"
#include "guild_mgr.h"
#include "../WorldDefine/city_struggle_define.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

class City;

class CSScoreRank
{
public:
	bool operator() (const pair<DWORD, INT>& lhs, const pair<DWORD, INT>& rhs)
	{
		return lhs.second > rhs.second;
	}
};

class CityStruggle
{
public:
	typedef TMap<DWORD, tagStruggleRevive*>			ReviveMap;
public:
	CityStruggle();
	~CityStruggle();

	//--------------------------------------------------------------------
	// 初始化
	//--------------------------------------------------------------------
	VOID	Init(City* pCity);

	//--------------------------------------------------------------------
	// 设置城战状态
	//--------------------------------------------------------------------
	VOID	SetCityStruggleState(ECityStruggleState eState);
	
	//--------------------------------------------------------------------
	// 销毁
	//--------------------------------------------------------------------
	VOID	Destroy();

	//--------------------------------------------------------------------
	// 更新
	//--------------------------------------------------------------------
	VOID	Update();

	//--------------------------------------------------------------------
	// 更新守护神
	//--------------------------------------------------------------------
	VOID	UpdateEudemon();

	//--------------------------------------------------------------------
	// 更新城战玩家复活
	//--------------------------------------------------------------------
	VOID	UpdateRoleRevive();

	//--------------------------------------------------------------------
	// 更新龙珠
	//--------------------------------------------------------------------
	VOID	UpdateBall();

	//--------------------------------------------------------------------
	// 更新阴阳柱
	//--------------------------------------------------------------------
	VOID	UpdateColumns();

	//--------------------------------------------------------------------
	// 处理玩家加入复活队列
	//--------------------------------------------------------------------
	VOID	AddToReviveMap(Role* pRole);

	//--------------------------------------------------------------------
	// 玩家是否在阵营中
	//--------------------------------------------------------------------
	BOOL	IsRoleInForce(Role* pRole, ECityForceType eForce);

	//--------------------------------------------------------------------
	// 获得玩家阵营
	//--------------------------------------------------------------------
	BOOL	GetForceType(Role* pRole, ECityForceType& eForce);

	//--------------------------------------------------------------------
	// 复活玩家
	//--------------------------------------------------------------------
	VOID	ReviveRole(DWORD dwRoleID);

	//--------------------------------------------------------------------
	// 设置联盟复活时间
	//--------------------------------------------------------------------
	VOID	SetReviveTime(ECityForceType eForce, INT nSec);

	//--------------------------------------------------------------------
	// 设置守护神召唤时间
	//--------------------------------------------------------------------
	VOID	SetSummonTime(INT nSec);

	//--------------------------------------------------------------------
	// 终止召唤守护神
	//--------------------------------------------------------------------
	VOID	StopSummon();

	//--------------------------------------------------------------------
	// 设置城战结果
	//--------------------------------------------------------------------
	VOID	SetResult(ECityStruggleResult eResult);

	//--------------------------------------------------------------------
	// 各种get
	//--------------------------------------------------------------------
	ECityStruggleState		GetCityStruggleState()	{ return m_eState; }
	ECityStruggleResult		GetResult()	{ return m_eResult; }

	//--------------------------------------------------------------------
	// 城战玩家数据各种操作
	//--------------------------------------------------------------------
	VOID	InitCSRoleData();
	VOID	AddRoleDataInfo(DWORD dwRoleID);
	VOID	AddKillRoleNum(DWORD dwKillerID, DWORD dwTargetID);
	VOID	AddRoleData(DWORD dwRoleID, ECityStruggleRoleData eType, DWORD dwData);
	tagCSRoleData*	GetRoleDataPtr(DWORD dwRoleID)	{ return m_mapRoleData.Peek(dwRoleID); }
	VOID	SortByScore();
	vector< pair<DWORD, INT> >& GetRankData()		{ return m_vecScore; }

private:
	ECityStruggleState		m_eState;						// 当前城战状态

	ReviveMap				m_mapRevive;					// 待复活的角色，ID对应于该角色死亡时的城战计时
	Mutex					m_mutexRevive;					
	volatile INT			m_nUpdateTick;					// 城战更新计时，每tick加1
	volatile INT			m_nAtkReviveTick;				// 攻击联盟成员复活所需要等待的tick数
	volatile INT			m_nDefReviveTick;				// 防御联盟成员复活所需要等待的tick数
	volatile INT			m_nEudemonTick;					// 召唤守护神所需等待的tick数
	City*					m_pCity;						// 城市指针
	BOOL					m_bBeginSummonCountDown;		// 开始召唤倒计时

	TMap<DWORD, tagCSRoleData*>		m_mapRoleData;			// 城战玩家数据
	vector< pair<DWORD, INT> >		m_vecScore;				// 玩家积分排名
	ECityStruggleResult		m_eResult;
};