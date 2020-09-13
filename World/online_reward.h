/********************************************************************
created:	2010/01/22
created:	22:1:2010   14:23
filename: 	e:\loong\trunk\LoongWorld\OnlineReward.h
file path:	e:\loong\trunk\LoongWorld
file base:	OnlineReward
file ext:	h
author:		Jason

purpose:	神龙赐福扩展：每日在线奖励参数配置
*********************************************************************/


#ifndef OnlineReward_h__
#define OnlineReward_h__

#include "../ServerDefine/mini_tool.h"
using namespace jm_tool;

class OnlieRewardParam
{
public:
	struct tagLessingConfig{
		FLOAT	fAddExpPct;
		DWORD	dwExpireTime;
		DWORD	dwRewardItemID;
		BYTE	byQuality;
		INT		nItemCount;
		tagLessingConfig()
		{
			memset(this,0,sizeof(tagLessingConfig));
		}
		tagLessingConfig(const tagLessingConfig & t)
			:fAddExpPct(t.fAddExpPct),dwExpireTime(t.dwExpireTime),dwRewardItemID(t.dwRewardItemID),byQuality(t.byQuality),nItemCount(t.nItemCount) {}
		tagLessingConfig & operator = (const tagLessingConfig & t)
		{
			fAddExpPct		= t.fAddExpPct;
			dwExpireTime	= t.dwExpireTime;
			dwRewardItemID	= t.dwRewardItemID;
			byQuality		= t.byQuality;
			nItemCount		= t.nItemCount;
			return *this;
		}
	} ;
	INT		m_nConfigCount;
	vector<tagLessingConfig> m_vecLessingExConfig;
	DWORD	_dwItemType;
	INT		_nItemCount;
	DWORD	_dwTimeout;
	CSCreatedLock	_cs;
public:
	OnlieRewardParam()
		:_dwItemType(0),_nItemCount(0),_dwTimeout(0),m_nConfigCount(0)//,_bOpen(TRUE)
	{}
	~OnlieRewardParam(){}

	VOID IniParam(VarContainer * pVar,LPCTSTR field);
	BOOL GetParam( INT ndx, tagLessingConfig & config );
	INT GetCount();

	static INT LoongLessingSwitch(void);
	static void LoongLessingSwitch(INT switch_v);
};

extern OnlieRewardParam g_OnlineRewardParam;

#endif // OnlineReward_h__

