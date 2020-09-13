//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: currency.h
// author: Sxg
// actor:
// data: 2008-08-14
// last:
// brief: 货币类 -- 管理背包仓库中的金钱元宝
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/cost_type_define.h"
class Role;
//-----------------------------------------------------------------------------
// log用容器类型枚举
//-----------------------------------------------------------------------------
enum ELogConType
{
	ELCT_Null		= 0,

	ELCT_Bag		= 1,	// 背包
	ELCT_RoleWare	= 2,	// 角色仓库
	ELCT_BaiBao		= 3,	// 百宝袋
};

//-----------------------------------------------------------------------------
// 模版
//-----------------------------------------------------------------------------
template<class T>
class Currency
{
public:
	Currency(T nMoney, T nMaxMoney);

public:
	T	Gain(T nMoney);
	T	Spend(T nMoney);
	T	SetMoney(T nMoney);

public:
	T	GetCur() const { return m_nMoney; }
	T	GetMax() const { return m_nMaxMoney; }

private:
	volatile T	m_nMoney;
	T	m_nMaxMoney;
};
class ClanData;
class ItemMgr;
//-----------------------------------------------------------------------------
// 管理类 -- 背包仓库中的金钱元宝
//-----------------------------------------------------------------------------
class CurrencyMgr
{
friend class ItemMgr;

public:
	CurrencyMgr(Role *pRole, INT32 nBagGold, INT32 nBagSilver, INT32 nBagYuanBao, 
				INT64 nWareSilver, INT32 nBaiBaoYuanBao, INT32 nExVolume,INT32 nVIPPoint);

	VOID Update();

public:
	// 当前货币是否满足要求
	BOOL IsEnough(ECurCostType eCurType, INT32 nNeed);
	// 扣除指定货币
	BOOL DecCurrency(ECurCostType eCurType, INT32 nNeed, DWORD dwCmdID);

public:
	// 获得当前金钱数	
	INT64 GetBagSilver()		const { return m_BagSilver.GetCur(); }
	INT32 GetBagYuanBao()		const { return m_BagYuanBao.GetCur(); }
	INT64 GetWareSilver()		const { return m_WareSilver.GetCur(); }
	INT32 GetBaiBaoYuanBao()	const { return m_BaiBaoYuanBao.GetCur(); }
	INT32 GetExchangeVolume()	const { return m_ExchangeVolume.GetCur(); }
	INT32 GetVIPPoint()	        const { return m_VIPPoint.GetCur(); }

	// 获得可存储最大金钱数
	INT64 GetMaxBagSilver()			const { return m_BagSilver.GetMax(); }
	INT32 GetMaxBagYuanBao()		const { return m_BagYuanBao.GetMax(); }
	INT64 GetMaxWareSilver()		const { return m_WareSilver.GetMax(); }
	INT32 GetMaxBaiBaoYuanBao()		const { return m_BaiBaoYuanBao.GetMax(); }
	INT32 GetMaxExchangeVolume()	const { return m_ExchangeVolume.GetMax(); }
	INT32 GetMaxVIPPoint()	    const     { return m_VIPPoint.GetMax(); }
	

	// 还可以存入的金钱数目
	INT64 GetCanIncBagSilver()		const { return GetMaxBagSilver() - GetBagSilver(); }
	INT32 GetCanIncBagYuanBao()		const { return GetMaxBagYuanBao() - GetBagYuanBao(); }
	INT64 GetCanIncWareSilver()		const { return GetMaxWareSilver() - GetWareSilver(); }
	INT32 GetCanIncBaiBaoYuanBao()	const { return GetMaxBaiBaoYuanBao() - GetBaiBaoYuanBao(); }
	INT32 GetCanIncExchangeVolume()	const { return GetMaxExchangeVolume() - GetExchangeVolume(); }
	INT32 GetCanIncVIPPoint()	    const { return GetMaxVIPPoint() -     GetVIPPoint(); }

	// 获得金钱或元宝
	BOOL IncBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel = GT_INVALID);
	BOOL IncBagYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL IncWareSilver(INT64 n64Silver, DWORD dwCmdID);
	BOOL IncBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID, BOOL bSaveDB = TRUE);
	BOOL IncExchangeVolume(INT32 nExVolume, DWORD dwCmdID);
    BOOL IncVIPPoint(INT32 nVIPPoint, DWORD dwCmdID);

	// 失去金钱或元宝
	BOOL DecBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel = GT_INVALID);
	BOOL DecBagYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL DecWareSilver(INT64 n64Silver, DWORD dwCmdID);
	BOOL DecBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID);
	BOOL DecExchangeVolume(INT32 nExVolume, DWORD dwCmdID);
	BOOL DecVIPPoint(INT32 nVIPPoint, DWORD dwCmdID);

	VOID SetCurBagYuanBao(INT32 nBagYuanBao);
	VOID SetCurBaiBaoYuanBao(INT32 nBaiBaoYuanBao);

	BOOL	SendBagYuanBao();
	BOOL	SendBaibaoYuanBao();

	//战场金币相关操作
	DWORD GetWarMoney();
	VOID AddWarMoney(DWORD dwWarMoney);

public:
	// 针对离线玩家的接口
	static BOOL ModifyBaiBaoYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID);
	static BOOL ModifyWareSilver(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID);
	static BOOL ModifyBagYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID);
	static BOOL ModifyBagGold(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID);
	static BOOL IncBagYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID);
	static BOOL IncBagGold(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID);
private:
	static VOID SendBaiBaoYB2DB(DWORD dwAccountID, INT nBaiBaoYuanBao);
	static VOID LogBaiBaoYuanBao(const DWORD dwAccountID, const INT nNum, const DWORD dwCmdID);
	static VOID SendWareSilver2DB(DWORD dwAccountID, INT64 n64WareSilver);
	static VOID LogWareSilver(const DWORD dwAccountID, const INT64 n64Num, const DWORD dwCmdID);
	static VOID LogBagYuanBao(const DWORD dwRoleID, const INT nNum, const DWORD dwCmdID);
	static VOID LogBagGold(const DWORD dwRoleID, const INT nNum, const DWORD dwCmdID);
	static VOID SendBagYB2DB(DWORD dwRoleID, INT nBagYuanBao, BOOL bIncrement = FALSE);
	static VOID SendBagGold2DB(DWORD dwRoleID, INT nBagGod, BOOL bIncrement = FALSE);
public:
	// 氏族贡献消耗
	BOOL DecClanCon(INT32 nClanCon, DWORD dwCmdID, ECLanType eClanType);
	BOOL IncClanCon(INT32 nIncr, DWORD dwCmdID, ECLanType eClanType);
	INT32 GetCanIncClanCon(ECLanType eClanType)	const;
	INT32 GetMaxClanCon(ECLanType eClanType) const;
	INT32 GetClanCon(ECLanType eClanType) const;

private:
	// 向客户端发送消息
	VOID SendMessage(LPVOID pMsg, DWORD dwSize);

	// 记录log，向LoongDB发消息
	VOID LogSilver(const ELogConType eLogConType, const INT64 n64Num, 
				  const INT64 n64TotalNum, const DWORD dwCmdID, const DWORD dwRoleIDRel = GT_INVALID);
	VOID LogYuanBao(const ELogConType eLogConType, const INT nNum, 
					const INT nTotalNum, const DWORD dwCmdID);
	VOID LogExVolume(const INT nNum, const INT nTotalNum, const DWORD dwCmdID);
	VOID LogVIPPoint(const INT nNum, const INT nTotalNum, const DWORD dwCmdID);
	VOID LogTimeStat();
	BOOL DecContribeOrExploit( ECurCostType eCurType, INT32 nNeed );

private:
	Role*				m_pRole;
	INT					m_nTimeStatCountDown;	// 定时统计倒计时

private:
	Currency<INT64>		m_BagSilver;
	Currency<INT32>		m_BagYuanBao;
	Currency<INT64>		m_WareSilver;
	Currency<INT32>		m_BaiBaoYuanBao;
	Currency<INT32>		m_ExchangeVolume;
	Currency<INT32>		m_VIPPoint;
	ClanData*			m_pRoleClanData;		// 氏族贡献
	DWORD				m_dwWarMoney;			// 战场金币
};


//-----------------------------------------------------------------------------
// 模版类实现
//-----------------------------------------------------------------------------
template<class T>
Currency<T>::Currency(T nMoney, T nMaxMoney)
{
	m_nMoney	= nMoney;
	m_nMaxMoney = nMaxMoney;

	if(nMoney > nMaxMoney)
	{
		ASSERT(nMoney <= nMaxMoney);
		nMoney = nMaxMoney;
	}

	if(nMoney < 0)
	{
		ASSERT(nMoney >= 0);
		nMoney = 0;
	}
}

template<class T>
T Currency<T>::Gain(T nMoney)
{
	if(nMoney <= 0)
	{
		return 0;
	}

	T nChange = m_nMaxMoney - m_nMoney;

	if(nChange < nMoney)
	{
		ASSERT(nChange >= nMoney);
		m_nMoney = m_nMaxMoney;
	}
	else
	{
		m_nMoney += nMoney;
		nChange = nMoney;
	}

	return nChange;
}

template<class T>
T Currency<T>::Spend(T nMoney)
{
	if(nMoney <= 0)
	{
		return 0;
	}

	T nChange = nMoney;

	if(m_nMoney < nMoney)
	{
		ASSERT(m_nMoney >= nMoney);
		nChange = m_nMoney;
		m_nMoney = 0;
	}
	else
	{
		m_nMoney -= nMoney;
	}

	return nChange;
}

template<class T>
T Currency<T>::SetMoney(T nMoney)
{
	if(nMoney <= 0)
	{
		return 0;
	}

	m_nMoney = nMoney;

	if(m_nMaxMoney < m_nMoney)
	{
		m_nMoney = m_nMaxMoney;
	}

	return m_nMoney;
}

////-----------------------------------------------------------------------------
//class Currency
//{
//public:
//	Currency();
//	~Currency();
//
//public:
//	// 初始化从数据库读入的数据
//	VOID Init(INT32 nBagGold, INT32 nBagSilver, INT32 nBagYuanBao, 
//		INT32 nWareGold, INT32 nWareSilver, INT32 nBaiBaoYuanBao);
//
//	// 获得金钱或元宝
//	INT64 IncBagSilver(INT64 n64Silver);
//	INT32 IncBagYuanBao(INT32 nYuanBao);
//	INT64 IncWareSilver(INT64 n64Silver);
//	INT32 IncBaiBaoYuanBao(INT32 nYuanBao);
//
//	// 失去金钱或元宝
//	INT64 DecBagSilver(INT64 n64Silver);
//	INT32 DecBagYuanBao(INT32 nYuanBao);
//	INT64 DecWareSilver(INT64 n64Silver);
//	INT32 DecBaiBaoYuanBao(INT32 nYuanBao);
//
//
//public:
//	INT64 GetBagSilver()	const { return m_n64BagSilver; }
//	INT32 GetBagYuanBao()	const { return m_nBagYuanBao; }
//	INT64 GetWareSilver()	const { return m_n64WareSilver; }
//	INT32 GetBaiBaoYuanBao()	const { return m_nBaiBaoYuanBao; }
//
//private:
//	INT64	m_n64BagSilver;
//	INT64	m_n64WareSilver;
//	INT32	m_nBagYuanBao;
//	INT32	m_nBaiBaoYuanBao;
//};