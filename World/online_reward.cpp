
#include "StdAfx.h"
#include "online_reward.h"

VOID OnlieRewardParam::IniParam(VarContainer * pVar,LPCTSTR field)
{
	if( P_VALID(pVar) && field )
	{
		// MutexGuard<CSCreatedLock> guard(_cs);
		tagLessingConfig t;
		t.byQuality		= pVar->GetDword(_T("Quality"),field);
		t.dwExpireTime	= pVar->GetDword(_T("ExpireTime"),field) * 60;
		t.dwRewardItemID= pVar->GetDword(_T("RewardItemID"),field);
		t.fAddExpPct	= pVar->GetFloat(_T("Experience"),field);
		t.nItemCount	= pVar->GetDword(_T("ItemCount"),field);
		m_vecLessingExConfig.push_back(t);
	}
}
BOOL OnlieRewardParam::GetParam( INT ndx, OnlieRewardParam::tagLessingConfig & config )
{
	size_t size = m_vecLessingExConfig.size();
	if( ndx < 0 || ndx >= size )
		return FALSE;

	config = m_vecLessingExConfig[ndx];
	return TRUE;
}
INT OnlieRewardParam::GetCount()
{
	size_t size = m_vecLessingExConfig.size();
	return size;
}

OnlieRewardParam g_OnlineRewardParam;

LONG VOLATILE lLoongLessing = 1;

INT OnlieRewardParam::LoongLessingSwitch(void)
{
	return lLoongLessing;
}
void OnlieRewardParam::LoongLessingSwitch(INT switch_v)
{
	::InterlockedExchange((PLONG)&lLoongLessing,switch_v);
}
