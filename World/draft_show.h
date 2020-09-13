//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: draft_show.h
// author: zxzhang
// actor:
// data: 2010-3-31
// last:
// brief: 选秀活动
//-----------------------------------------------------------------------------

#pragma once
//得票和投票数最大的人数  限定为 10
 
 
struct tagShowPlayer;
struct tagVotePlayer;

class   DraftShow
{
public:
	DraftShow();
	virtual ~DraftShow();


	VOID   Init();
	VOID   Update();
	VOID   Destroy();


    tagShowPlayer*  GetShowPlayer(DWORD dwSignNum);
    tagVotePlayer*  GetVotePlayer(DWORD dwVoteRoleId);
    BOOL            AddShowPlayer(const tagShowPlayer* showPlayer);
	BOOL            AddVotePlayer(const tagVotePlayer* votePlayer);

    const tagShowPlayer* GetMaxPointShowPlayer(DWORD dwPos)const;  //访问从1开始
	const tagVotePlayer* GetMaxPointVotePlayer(DWORD dwPos)const;  // 访问从1开始
    VOID  ModMaxPointShowPlayer(DWORD dwPos,const tagShowPlayer* showPlayer);
	VOID  ModMaxPointVotePlayer(DWORD dwPos,const tagVotePlayer* votePlayer);


    VOID  AddMaxPointShowPlayer(const tagShowPlayer* showPlayer);
    VOID  AddMaxPointVotePlayer(const tagVotePlayer* votePlayer);

	DWORD  GetShowPlayerRankingSize();
	DWORD  GetVotePlayerRankingSize();
	VOID   SortShowPlayerRanking(DWORD dwSize = MAX_VOTES_PERSON_NUM);
	VOID   SortVotePlayerRanking(DWORD dwSize = MAX_VOTES_PERSON_NUM);

    DWORD  GetShowRankingMinPointPos()const;  //获得选秀排名中最低分的位置
    DWORD  GetVoteRankingMinPointPos()const;  //获得投票排名中最低分的位置
    //投票
    DWORD  Vote(DWORD dwSignNum,DWORD dwVoteRoleId);
	//投票自己得分
	DWORD  VoteReward(DWORD dwVoteRoleId);

	DWORD   ReadSignDataTable(DWORD);
    VOID*  SovleSignFile(OUT LPVOID nProtoType, LPCTSTR szField);
	
	VOID   SaveSignToDB();
	DWORD  GetShowPlayerRankingsSize();
	DWORD  GetVotePlayerRankingsSize();


	DWORD  GetNeckLaceTypeId()   {         return  m_dwTypeId;        }
	DWORD  GetTalentShow()       {         return  m_dwTalentShowState;}  
	DWORD  OpenTalentShow(DWORD) {    m_dwTalentShowState = 1;     return  m_dwTalentShowState;}
	DWORD  CloseTalentShow(DWORD){    m_dwTalentShowState = 0;     return  m_dwTalentShowState;}

	DWORD  LoadDataFromDB();

private:
    TObjRef<VarContainer>	                          m_pVar;
    TSFPTrunk<DraftShow>                              m_Trunk;
	static  TMap<DWORD,tagShowPlayer*>                m_MapShowPlayer;
	TMap<DWORD, tagVotePlayer*>                       m_MapVotePlayer;
	static      const   INT   MAX_VOTES_PERSON_NUM    = 10 ;  
	tagShowPlayer*                                    m_pMaxPointShowPlayer[MAX_VOTES_PERSON_NUM+1];
    tagVotePlayer*                                    m_pMaxPointVotePlayer[MAX_VOTES_PERSON_NUM+1];
	static      const   DWORD  m_dwTypeId             = 3301056;

	static      DWORD                                 m_dwTalentShowState ;
    static      DWORD                                 m_dwSerialId;  // 保存投票人的到数据库中的关键字
	static      DWORD                                 m_dwShowSerialId;


};

extern DraftShow    g_DraftShow;


//------------------------------------------------------------------------------------------------
// 世博活动
//------------------------------------------------------------------------------------------------
struct tagActiveRankData;

class CScoreActiveRank
{
public:
	bool operator() (const pair<DWORD, INT>& lhs, const pair<DWORD, INT>& rhs)
	{
		return lhs.second > rhs.second;
	}
};

class WorldExpo
{
public:
	WorldExpo();
	~WorldExpo();
	VOID Init();
	VOID Update();
	VOID Destroy();

    VOID GetRoleDisplayRankings(DWORD dwRoleId,E_ActiveDataType eDataType,INT nLowRank, INT nHighRank);
	VOID ModRankingsData(DWORD dwRoleId,E_ActiveDataType eDataType,INT nValue);
	INT  GetRoleRankingsData(DWORD dwRoleId,E_ActiveDataType eDataType);
   
	VOID SaveRoleDataToDB();
    tagActiveRankData* GetActiveRankData(DWORD dwRoleId);
	VOID AddActiveRankData(const tagActiveRankData* pActiveRankData);

	VOID SortDisplayRankings(E_ActiveDataType eDataType);
	VOID LoadRankDataFromDB();
	
private:
	TSafeMap<DWORD,tagActiveRankData*>    m_MapActiveRankData;
    vector< pair<DWORD, INT> >		         m_vecScore;				// 玩家积分排名
	

};

extern WorldExpo g_WorldExpo;