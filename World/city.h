//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city.h
// author: Sunnee
// actor:
// data: 2009-08-11
// last:
// brief: 城市系统
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/city_define.h"
#include "../ServerDefine/activity_define.h"
#include "city_struggle.h"

struct tagGuildClanValue;

enum EWarTimeType 
{
	EWT_NOT_WARTIME_INTERNAL		= 0,  //非周末

	EWT_BEFOR_WAR		= 1,  //属于周末 开战前
	EWT_WARING			= 2,  //属于周末 城战中	
	EWT_AFTER_WAR		= 3,  //属于周末 成战后

};


class FareCompare
{
public:
	bool operator()(const tagCitySignUp* lhs, const tagCitySignUp* rhs)
	{
		return lhs->nFare > rhs->nFare;
	}
};

class ReputationCompare
{
public:
	bool operator()(const tagGuildClanValue& lhs, const tagGuildClanValue& rhs);
};

class Map;
class Guild;

class City
{
public:
	typedef TMap<DWORD, tagCityItemOutput*>		MapItemOutput;		// 物品类型、产出信息
	typedef std::map<DWORD, tagCitySignUp*>		MapSignUp;			// 帮派ID、报名信息

public:
	City();
	~City();

	// 初始化、更新、销毁
	BOOL	Init(DWORD dwCityID);
	VOID	Update(const tagDWORDTime &tm);
	VOID	Destory();

	BOOL	IsInitOK()		{ return m_bInitOK; }

	// 从数据库载入城市数据
	BOOL	LoadCityInfo(tagCityAtt2DB* pCityInfo, INT nSignUpNum, tagCitySignUp* pSignUpInfo);

//-------------------------------------------------------------------------
// 城市属性操作
//-------------------------------------------------------------------------
public:
	// 获取城市属性
	DWORD	GetCityAtt(Role* pRole, tagCityAtt2Client* pCityInfo);
	VOID	GetCityAppointedAtt(ECityAttType eType, INT& nDataNum, LPVOID pData);
	INT		GetCityAppointedAttSize(ECityAttType eType);

	Map*	GetShellCity()		{ return m_pShellCity; }
	Map*	GetKernelCity()		{ return m_pKernelCity; }

	// 统治权变更
	DWORD	ChangeCityHolder(DWORD dwGuildID);

	// 城市产出
	VOID	CityProduce();

	// 生产力提升
	DWORD	UpgradeProlificacy(Role* pRole, EProlificacyUpgrade eType);

	// 调整税率
	DWORD	ModifyTaxRate(Role* pRole, INT32 nTaxRate);

	// 提取税金
	DWORD	TransformTaxation(Role* pRole, ETaxTransformType eType, INT32 nTaxation);

	// todo:守护神祭祀(任务?)

public:
	// 取得城门类型
	ECityGateType GetCityGateType();

	// 防御度
	BOOL	IncCityDefence(INT32 nDefence);
	BOOL	DecCityDefence(INT32 nDefence);

	// 守护神契合度
	BOOL	IncEudemonTally(INT32 nEudemonTally);
	BOOL	DecEudemonTally(INT32 nEudemonTally);

	// 税率
	BOOL	ExchangeCityTaxRate(INT32 nTaxRate);

	// 税金
	BOOL	IncCityTaxation(INT32 nTaxation);
	BOOL	DecCityTaxation(INT32 nTaxation);

	// 生产力
	BOOL	IncCityProlificacy(INT32 nProlificacy);
	BOOL	DecCityProlificacy(INT32 nProlificacy);

private:

	VOID	MineUpdate(const tagDWORDTime &tm);
	VOID	DoHouseKeepingForSigupInfo();

	// 保存到数据库
	VOID	SaveCityAtt2DB();
	VOID	SaveAppointedAtt2DB(ECityAttType eType);

//-------------------------------------------------------------------------
// 城战相关
//-------------------------------------------------------------------------
public:
	// 得到报名信息
	VOID	GetCitySigupInfo(Role *pRole,tagNetCmd* pCmd);

	// 得到城战信息
	VOID	GetCityWarInfo(Role *pRole,tagNetCmd* pCmd);

	// 报名
	DWORD	SignupForCitywar(Role* pRole, ECityForceType eForce, INT32 nFare);
	DWORD	SignupForCitywar(DWORD dwGuildID, ECityForceType eForce, INT32 nFare);

	// 进攻权确认
	DWORD	ConfirmAttackForce();
	VOID	ConfirmAttackForce(INT nNum, tagGuildClanValue* pClanValue);

	// 防御权确认
	DWORD	ConfirmDefenceForce(Role* pRole, DWORD dwGuildID[MAX_CITY_DEFENCE_FORCE]);

	// 传出城战参与帮派
	BOOL	GetConfirmGuild(ECityForceType eForce, INT& nNum, DWORD* pGuildID);

	// 报名信息检查
	BOOL	IsAlreadySignup(DWORD dwGuildID, ECityForceType eForce);

	// 设置城战状态
	VOID	SetCityStruggleState(ECityStruggleState eState);

	// 加入复活队列
	VOID	AddToReviveMap(Role* pRole);

	// 设置复活等待时间
	VOID	SetReviveTime(ECityForceType eForce, INT nSec);

	// 设置守护神召唤时间
	VOID	SetSummonTime(INT nSec);

	// 获取玩家阵营
	BOOL	GetForceType(Role* pRole, ECityForceType& eForce);

	// 城战时间类型
	EWarTimeType	GetWarTimeType(const tagDWORDTime &tm);

	//是否被帮派占领
	BOOL IsGuildOccupied() {return P_VALID(m_pGuild);}

	// 获取城战对象
	CityStruggle&	GetCityStruggle()	{ return m_CityStruggle; }

private:
	// 时间判断
	BOOL	IsCityWarInterval(const tagDWORDTime &tm);

private:
	//原型数据
	const tagCityProto*	m_pProto;

	BOOL			m_bIsCityWarInterval;

	BOOL			m_bInitOK;

	// 卫城与禁城地图信息
	Map*			m_pShellCity;
	Map*			m_pKernelCity;

	// 统治者信息
	Guild*			m_pGuild;

	// 城市属性
	DWORD			m_dwCityID;			// 城市ID
	ECLanType		m_eClanType;		// 氏族类型

	INT32			m_nDefence;			// 防御度
	INT32			m_nEudemonTally;	// 守护神契合度

	INT32			m_nTaxRate;			// 税率
	DWORD			m_dwTaxRateTime;	// 税率更新时间
	INT32			m_nTaxation;		// 税金
	INT32			m_nProlificacy;		// 生产力

	INT32			m_nFundOutput;		// 基础资金产出
	INT32			m_nMaterialOutput;	// 基础资材产出
	MapItemOutput	m_mapItemOutput;	// 物品产出

	DWORD			m_dwMinSigupFare;	// 最低报名费用
	// 城战报名
	MapSignUp		m_mapAttackForce;	// 攻方报名信息
	MapSignUp		m_mapDefenceForce;	// 守方报名信息

	// 城战
	CityStruggle	m_CityStruggle;

	// 城战开始时间
	tagWarTime		m_WarTm;

	// 城战活动属性
	EActMode		m_ActMode;

	BOOL m_bSigupConfirmed;

	BOOL m_bSigupInfoClear;

	BOOL m_bOutputItemed;

};