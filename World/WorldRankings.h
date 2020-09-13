//-----------------------------------------------------------------------------
//!\file WorldRankings.h
//!\author zr
//!
//!\date 2009-12
//! last 
//!
//!\brief 游戏世界中管理所有排行榜的类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "mutex.h"
#include "../ServerDefine/rankings_define.h"
#include "../ServerDefine/msg_rankings.h"

class WorldRankingMgr
{
public:

	WorldRankingMgr();

	BOOL		Init();
	void		Destroy();
	//VOID		Update();
	
	VOID		DoInit(tagNDBS_InitRankings*);
	VOID		DoInitStrengthRankings(tagNDBS_InitRankings*);
	VOID		DoInitLevelRankings(tagNDBS_InitRankings*);
	VOID		DoInitFameRankings(tagNDBS_InitRankings*);
	VOID		DoInitPetRankings(tagNDBS_InitPetRankings*);
	VOID		DoInitEquipRankings(tagNDBS_InitRankings*);
    VOID		DoInitWuXunRankings(tagNDBS_InitRankings*);
	VOID		DoInitFlowerRankings(tagNDBS_InitFlowerRankings*);
	VOID		DoInitEggRankings(tagNDBS_InitFlowerRankings*);


	// Get系列
	TMap<INT, tagRankingsNode*>			GetLevelRanking()			{ return m_mapLevelRanking;			}
	TMap<INT, tagRankingsNode*>			GetFameRanking()			{ return m_mapFameRanking;			}
	TMap<INT, tagRankingsNode*>			GetStrengthRanking()		{ return m_mapStrengthRanking;		}
	TMap<INT, tagPetRankingsNode*>		GetPetRanking()				{ return m_mapPetRanking;			}	
	TMap<INT, tagRankingsNode*>			GetEquipRanking()			{ return m_mapEquipRanking;			}
	TMap<INT, tagRankingsNode*>			GetWuXunRanking()			{ return m_mapWuXunRanking;			}
	TMap<INT, tagRankingsNode*>			GetFlowerRanking()			{ return m_mapLikeRanking;			}
	TMap<INT, tagRankingsNode*>			GetEggRanking()				{ return m_mapHateRanking;			}

	VOID	SetUpdateMark( ERankingType eType, BOOL bMark )	{ m_UpdateMark[eType] = bMark; }

private:
	VOID SyncCanRankFlag(DWORD dwRoleID, BYTE byCanRankFlag);

private:
	Mutex								m_Lock;
	TMap<INT, tagRankingsNode*>			m_mapLevelRanking;
	TMap<INT, tagRankingsNode*>			m_mapFameRanking;
	TMap<INT, tagRankingsNode*>			m_mapStrengthRanking;
	TMap<INT, tagPetRankingsNode*>		m_mapPetRanking;
	TMap<INT, tagRankingsNode*>			m_mapEquipRanking;
	TMap<INT, tagRankingsNode*>	        m_mapWuXunRanking; 
	TMap<INT, tagRankingsNode*>			m_mapLikeRanking;	//鲜花排行榜
	TMap<INT, tagRankingsNode*>			m_mapHateRanking;	//鸡蛋排行榜

	tagDWORDTime						m_dwLastUpdate;
	BOOL								m_UpdateMark[ERT_All];
};

extern WorldRankingMgr g_WorldRankingMgr;