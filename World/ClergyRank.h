//-----------------------------------------------------------------------------
//!\file ClergyRank.h
//!\author zr
//!
//!\date 2010-1-13
//! last 
//!
//!\brief 游戏世界中管理神职竞选的类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "mutex.h"
#include "../WorldDefine/clergy_define.h"
#include "../ServerDefine/rankings_define.h"
#include "../ServerDefine/msg_rankings.h"

class ClergyRankingMgr
{
public:
	ClergyRankingMgr();
	void		Init();	
	//void		ReInit(); // 每次竞选结束后的初始化
	void		Destroy();
	VOID		DoInit(tagNDBS_InitClergyRank*);
	VOID		DoInit(TMap<DWORD, tagClergyNode*>&, ECampaignGodType eType, tagClergyNode*);
	VOID		ClearPreClergy(ECampaignGodType);
	
	BOOL		IsRoleWinClergy(DWORD dwRoleID, ECampaignGodType& eType1, EClergyType& eType2);

	// Get系列
	TMap<DWORD, tagClergyNode*>& GetClergyRank(ECampaignGodType eType)
	{ 
		switch(eType)
		{
		case ECGT_War:
			return m_mapClergyMars;
		case ECGT_Sun:
			return m_mapClergyApollo;
		case ECGT_Mana:
			return m_mapClergyRabbi;
		case ECGT_Peace:
			return m_mapClergyPeace;
// 		default:
// 			return NULL;
		}
	}
	
	VOID		UpdateCandidateCount(DWORD* dwCount)
	{
		memcpy(m_dwCandidateCount, dwCount, sizeof(DWORD)*ECGT_Num);
	}

	VOID		GetCandidateCount(DWORD* dwCount)
	{
		memcpy(dwCount, m_dwCandidateCount, sizeof(DWORD)*ECGT_Num);
	}

	VOID		Ask4CandidateCount()
	{
		if (m_bCandidateCountChg)
		{
			tagNDBC_Ask4CandidateCount msg;
			g_dbSession.Send(&msg, msg.dwSize);
		}
	}
	
	VOID		SetCandidateCountChg()
	{
		if(!m_bCandidateCountChg) m_bCandidateCountChg = true;
	}

private:
	Mutex				m_Lock;
	bool				m_bCandidateCountChg;
	TMap<DWORD, tagClergyNode*>			m_mapClergyMars;
	TMap<DWORD, tagClergyNode*>			m_mapClergyApollo;
	TMap<DWORD, tagClergyNode*>			m_mapClergyRabbi;
	TMap<DWORD, tagClergyNode*>	        m_mapClergyPeace; 
	DWORD				m_dwCandidateCount[ECGT_Num];
};

extern ClergyRankingMgr g_ClergyRankingMgr;
extern DWORD GetClergyID(ECampaignGodType eType1, EClergyType eType2);