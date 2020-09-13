//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: vip_netbar.h
// author: xlguo
// actor:
// data: 2009-8-3
//		2010-11-23 修改金牌网吧内容 Jason
// last:
// brief: 金牌网吧
//-------------------------------------------------------------------------------
#pragma once
#include <vector>


class Role;
//-------------------------------------------------------------------------
// ip段
//-------------------------------------------------------------------------
class IpRange
{
public:
	IpRange(DWORD dwIpMin, DWORD dwIpMax, DWORD dwVNBId)
		:m_IPMin(dwIpMin), m_IPMax(dwIpMax), m_dwVNBId(dwVNBId){}
	BOOL	Fit(DWORD dwIP)			const	{	return dwIP >= m_IPMin && dwIP <= m_IPMax;	}
	BOOL	OnLeftOf(DWORD dwIP)	const	{	return dwIP > m_IPMax;	}
	BOOL	OnRightOf(DWORD dwIP)	const	{	return dwIP < m_IPMin;	}
	DWORD	GetVNBId()				const	{	return m_dwVNBId;		}
	DWORD	GetIpMin()				const	{	return m_IPMin;			}
	DWORD	GetIpMax()				const	{	return m_IPMax;			}
private:
	DWORD	m_IPMin;
	DWORD	m_IPMax;
	DWORD	m_dwVNBId;
};


struct tagDBVNBPlayers;
struct tagVipNetBar;

//-------------------------------------------------------------------------
// 金牌网吧管理器
//-------------------------------------------------------------------------
class VipNerBarMgr
{
	typedef TMap<DWORD, tagVipNetBar*>	VipNetBarMap;
	typedef std::vector<IpRange>		IpRangeVector;
	typedef std::set<DWORD>				AccountIDSet;
	typedef TMap<DWORD, DWORD>			IP2VNBIdMap;
	typedef std::set<DWORD>				NotifySet;
	std::set<DWORD> _INetbarBuffs;
	std::set<DWORD> _INetbarInstBuffs;

public:
	BOOL	Init();
	VOID	Destroy();
	VOID	InitData(tagDBVNBPlayers* pInitData, BOOL bFirst=FALSE);
	VOID	PlayerLogin(DWORD dwAccountID, DWORD dwIP);
	VOID	PlayerLogout(DWORD dwIP);
	VOID	PlayerNotify(DWORD dwAccountID);
	INT		GetRate(DWORD dwIP, INT nType);
	LPCTSTR	GetVNBName(DWORD dwIP);

	BOOL	RecReward(Role* pRole);
	BOOL	AddInstBuff(Role * pRole);
	BOOL	RemoveInstBuff(Role * pRole);
	BOOL	IsInVipNetbar(Role* pRole,DWORD * pdwBuff = NULL,DWORD * pdwInstBuffID = NULL);
	BOOL	IsOpen() const { return m_bOpen; }
	BOOL	IsVipNetbarBuff(DWORD dwBuffID);
	BOOL	IsVipNetbarInstBuff(DWORD dwInstBuffID);

	VOID	ReloadVipNetbarFromLoginSever(VOID * pLoginMsg,INT num);

private:
	DWORD	TransTSIp2DWORD(LPCTSTR szIP);
	tagVipNetBar* GetVipNetBar(DWORD dwIP);
	DWORD	GetVNBId(DWORD dwIp);
	DWORD	FitNetBar(DWORD dwIp);
	VOID	UpdateDbPlayerLogin(DWORD dwAccountID, DWORD dwTime);
	VOID	GeneralzeIP(DWORD &dwIP);

private:
	IP2VNBIdMap			m_mapIp2VNBId;
	VipNetBarMap		m_mapVipNetBars;
	IpRangeVector		m_vecIpRanges;				// 有序
	AccountIDSet		m_setHistoryAccountID;
	AccountIDSet		m_setTodayAccountID;
	NotifySet			m_setNotify;		

	BOOL				m_bOpen;
public:
	BOOL DynamicTest(DWORD dwTestNo, DWORD dwArg1, LPCTSTR szArg2);
};



//-------------------------------------------------------------------------
// 橘子金牌网吧
//-------------------------------------------------------------------------
class OrangeVipNetBarMgr
{
public:

	OrangeVipNetBarMgr():m_nTickToAddBonusPolint(0){}

	~OrangeVipNetBarMgr(){}


	VOID Init(const TCHAR RoleName[X_SHORT_NAME],const CHAR szAccount[X_SHORT_NAME],const DWORD IP);

	//静态方法
public:
	//开
	static VOID OpenOrangeVipNetBar();

	//关
	static VOID CloseOrangeVipNetBar();

	//橘子金牌网吧是否打开
	static BOOL IfOrangeVipNetBarOpen(){ return m_bOrangeVipBar; }

	//得到间隔时间
	static INT GetIntervalTime() { return m_nIntervalTm; }

	//得到一个间隔时间应加红利点
	static INT GetBonusPoint() { return m_nBonusPoint; }
private:

	volatile static BOOL		m_bOrangeVipBar;						// 是否是橘子vip网吧服务器

	volatile static INT32		m_nIntervalTm;							// 加红利点的间隔时间 单位(毫秒)

	volatile static INT32		m_nBonusPoint;							// 每个间隔时间加红利点的数


public:
	VOID Update();

private:
	//给指定玩家加红利点
	VOID AddBonusPoint();

private:
	INT32						m_nTickToAddBonusPolint; // 计时

	TCHAR						m_szRoleName[X_SHORT_NAME];

	CHAR						m_szAccountName[X_SHORT_NAME];

	DWORD						m_dwIP;

};

extern VipNerBarMgr g_VipNetBarMgr;