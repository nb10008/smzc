//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: draft_show.cpp
// author: zxzhang
// actor:
// data: 2010-3-31
// last:
// brief: 选秀活动
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "mutex.h" 

#include "role.h"

#include "role_mgr.h"

#include "../WorldDefine/msg_VoteShow_define.h"
#include "../WorldDefine/role_att.h"
#include "../ServerDefine/msg_draft_show.h"

#include "draft_show.h"


const LPCTSTR    FileName_DraftShow				= _T("data/system/attdata/draftshow_name.xml");
TMap<DWORD,tagShowPlayer*>         DraftShow::m_MapShowPlayer;
DWORD DraftShow::m_dwTalentShowState = 0;
DWORD DraftShow::m_dwSerialId = 0;
DWORD DraftShow::m_dwShowSerialId =0;
DraftShow    g_DraftShow;

DraftShow::DraftShow()
{
     memset(m_pMaxPointShowPlayer,0,sizeof(m_pMaxPointShowPlayer));
	 memset(m_pMaxPointVotePlayer,0,sizeof(m_pMaxPointVotePlayer));
	 m_MapShowPlayer.Clear();
	 m_MapVotePlayer.Clear();
	 

}

DraftShow::~DraftShow()
{

}


VOID  DraftShow::Init()
{
	TObjRef<VarContainer> pVar = "VarContainer";
	m_dwTalentShowState  =  pVar->GetDword(_T("on talent_show"));
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("loaddata"), m_Trunk.sfp1(&DraftShow::ReadSignDataTable), _T("Read base data of talent show  VoteShowCmd"));
	pConsole->Register(_T("opentalentshow"), m_Trunk.sfp1(&DraftShow::OpenTalentShow), _T("Open talent show action  VoteShowCmd"));
	pConsole->Register(_T("closetalentshow"), m_Trunk.sfp1(&DraftShow::CloseTalentShow), _T("Close talent show action  VoteShowCmd"));

	g_WorldExpo.Init();
}

VOID   DraftShow::Update()
{
CHECK_TIME()
      g_WorldExpo.Update();

}

VOID   DraftShow::Destroy()
{
	tagShowPlayer* pDraftData = NULL;
	TMap<DWORD,tagShowPlayer*>::TMapIterator it = m_MapShowPlayer.Begin();
	while (m_MapShowPlayer.PeekNext(it,pDraftData) )
	{
		if( P_VALID(pDraftData) )
		{
           SAFE_DEL(pDraftData);
		   
		}
	}
    m_MapShowPlayer.Clear();

	tagVotePlayer* pVoteData = NULL;
	TMap<DWORD,tagVotePlayer*>::TMapIterator it1 = m_MapVotePlayer.Begin();
	while (m_MapVotePlayer.PeekNext(it1,pVoteData) )
	{
		if( P_VALID(pVoteData) )
		{
			SAFE_DEL(pVoteData);

		}
	}
    m_MapVotePlayer.Clear();

	g_WorldExpo.Destroy();
}


tagShowPlayer*  DraftShow::GetShowPlayer(DWORD dwSignNum)
{
     return m_MapShowPlayer.Peek(dwSignNum);
}

tagVotePlayer*  DraftShow::GetVotePlayer(DWORD dwVoteRoleId)
{
     return m_MapVotePlayer.Peek(dwVoteRoleId);
}


BOOL   DraftShow::AddShowPlayer(const tagShowPlayer* showPlayer)
{
      if ( P_VALID( showPlayer))
      {
		     m_dwShowSerialId++;
             m_MapShowPlayer.Add(showPlayer->dwAthleteSignId,(tagShowPlayer*)showPlayer);
			 if (showPlayer->dwAthletePoint == 0 || !GT_VALID(showPlayer->dwAthletePoint))
			 {
				 return TRUE;
			 }
			 else
			 {
				 AddMaxPointShowPlayer(showPlayer);		
				 return TRUE;
			 }
			 
      }
	  else
	  {
		  return FALSE;
	  }
}

BOOL   DraftShow::AddVotePlayer(const tagVotePlayer* votePlayer)
{

	if ( P_VALID( votePlayer))
	{
		m_dwSerialId++;
		m_MapVotePlayer.Add(votePlayer->dwVoteRoleId,(tagVotePlayer*)votePlayer);
		if ( votePlayer->dwVotePoint == 0 || !GT_VALID(votePlayer->dwVotePoint))
		{
			return TRUE;
		}
		else
		{
     		AddMaxPointVotePlayer(votePlayer);
			return TRUE;
		}
		
	}
	else
	{
		return FALSE;
	}
}


const tagShowPlayer* DraftShow::GetMaxPointShowPlayer(DWORD dwPos)const  //访问从1开始
{
	if ( dwPos <= 0 || dwPos > MAX_VOTES_PERSON_NUM+1)
	{
		ASSERT(0);
		return NULL;
	}
	return m_pMaxPointShowPlayer[dwPos-1];
     
}

const tagVotePlayer* DraftShow::GetMaxPointVotePlayer(DWORD dwPos)const  // 访问从1开始
{
	if ( dwPos <= 0 || dwPos > MAX_VOTES_PERSON_NUM+1)
	{
		ASSERT(0);
		return NULL;
	}
	return m_pMaxPointVotePlayer[dwPos-1];
}

VOID  DraftShow::ModMaxPointShowPlayer(DWORD dwPos,const tagShowPlayer* showPlayer)
{
	if ( dwPos <= 0 || dwPos > MAX_VOTES_PERSON_NUM+1)
	{
		ASSERT(0);
		return ;
	}
      m_pMaxPointShowPlayer[dwPos-1] =(tagShowPlayer*)showPlayer;
}

VOID  DraftShow::ModMaxPointVotePlayer(DWORD dwPos,const tagVotePlayer* votePlayer)
{
	if ( dwPos <= 0 || dwPos > MAX_VOTES_PERSON_NUM+1)
	{
		ASSERT(0);
		return ;
	}
    m_pMaxPointVotePlayer[dwPos-1] =(tagVotePlayer*)votePlayer;

}

VOID  DraftShow::AddMaxPointShowPlayer(const tagShowPlayer* showPlayer)
{
	if ( !P_VALID(showPlayer) )
	{
		return;
	}
    DWORD dwSize  = GetShowPlayerRankingSize();
	for( DWORD i = 0; i < dwSize+1; i++)
	{
        const tagShowPlayer* pShowPlayer = GetMaxPointShowPlayer(i+1);
		if ( P_VALID( pShowPlayer ))
		{   
			if( showPlayer->dwAthleteSignId == pShowPlayer->dwAthleteSignId)
			{
                  SortShowPlayerRanking(dwSize+1);
				  return;
			}
		
		}
	}

	if (dwSize < MAX_VOTES_PERSON_NUM)
	{
		ModMaxPointShowPlayer(dwSize+1,showPlayer);
		SortShowPlayerRanking(dwSize+1);
		return;
    }
	
	if ( dwSize == MAX_VOTES_PERSON_NUM)
	{ 
		const tagShowPlayer* pShowPlayer = GetMaxPointShowPlayer(dwSize+1);
		if( P_VALID(pShowPlayer))
		{
     		if ( showPlayer->dwAthleteSignId == pShowPlayer->dwAthleteSignId)
			{ 
				SortShowPlayerRanking(dwSize+1);     
			}
			else if ( showPlayer->dwAthletePoint > pShowPlayer->dwAthletePoint)
			{
				ModMaxPointShowPlayer(dwSize+1,showPlayer); 
				SortShowPlayerRanking(dwSize+1);
			}
		}
		else
		{
			ModMaxPointShowPlayer(dwSize+1,showPlayer);
			SortShowPlayerRanking(dwSize+1);
			return;

		}
	}




}

VOID  DraftShow::AddMaxPointVotePlayer(const tagVotePlayer* votePlayer)
{

	if ( !P_VALID(votePlayer) )
	{
		return;
	}
	DWORD dwSize  = GetVotePlayerRankingSize();
	for( DWORD i = 0; i < dwSize+1; i++)
	{
		const tagVotePlayer* pVotePlayer = GetMaxPointVotePlayer(i+1);
		if ( P_VALID( pVotePlayer ))
		{   
			if( votePlayer->dwVoteRoleId == pVotePlayer->dwVoteRoleId)
			{
				SortVotePlayerRanking(dwSize+1);
				return;
			}

		}
	}

	if (dwSize < MAX_VOTES_PERSON_NUM)
	{
		ModMaxPointVotePlayer(dwSize+1,votePlayer);
		SortVotePlayerRanking(dwSize+1);
		return;
	}

	if ( dwSize == MAX_VOTES_PERSON_NUM)
	{ 
		const tagVotePlayer* pVotePlayer = GetMaxPointVotePlayer(dwSize+1);
		if ( P_VALID(pVotePlayer))
		{

			if ( votePlayer->dwVoteRoleId == pVotePlayer->dwVoteRoleId)
			{ 
				SortVotePlayerRanking(dwSize+1);     
			}
			else if ( votePlayer->dwVotePoint > pVotePlayer->dwVotePoint)
			{
                ModMaxPointVotePlayer(dwSize+1,votePlayer);
				SortVotePlayerRanking(dwSize+1);
			}
		}
		else
		{
			ModMaxPointVotePlayer(dwSize+1,votePlayer);
			SortVotePlayerRanking(dwSize+1);
		}
	}

}

DWORD  DraftShow::GetShowPlayerRankingSize()
{
    return  this->GetShowRankingMinPointPos();
}
DWORD  DraftShow::GetVotePlayerRankingSize()
{
    return this->GetVoteRankingMinPointPos();
}
VOID   DraftShow::SortShowPlayerRanking(DWORD dwSize)
{
	   if ( dwSize <= 1)
	   {
		   return ;
	   }

	   if ( dwSize > MAX_VOTES_PERSON_NUM+1 )
	   {
		   ASSERT(0);
		   return;
	   }
	   tagShowPlayer*       pShowPlayerTemp = NULL; 
	 

	   for( DWORD i = 0 ; i < dwSize - 1; i++)
	   {	
		   DWORD k = 0;
		   for(DWORD j = 0; j < dwSize - i-1 ; j++)
		   {
			   int t = k+1;
			   if ( P_VALID(m_pMaxPointShowPlayer[k]) && P_VALID(m_pMaxPointShowPlayer[t]))
			   {

				   if (m_pMaxPointShowPlayer[k]->dwAthletePoint < m_pMaxPointShowPlayer[t]->dwAthletePoint)
				   {
					   pShowPlayerTemp = m_pMaxPointShowPlayer[k];
					   m_pMaxPointShowPlayer[k] = m_pMaxPointShowPlayer[t];
					   m_pMaxPointShowPlayer[t] = pShowPlayerTemp;
				   }
			   }
			   k++;
		   }
	   }

}

VOID   DraftShow::SortVotePlayerRanking(DWORD dwSize)
{
	if ( dwSize <= 1)
	{
		return ;
	}

	if ( dwSize > MAX_VOTES_PERSON_NUM+1 )
	{
		ASSERT(0);
		return;
	}
	tagVotePlayer*       pVotePlayerTemp = NULL; 

	for( DWORD i = 0 ; i < dwSize - 1; i++)
	{	
		DWORD k = 0;
		for(DWORD j = 0; j < dwSize - i-1 ; j++)
		{
			int t = k+1;
			if ( P_VALID(m_pMaxPointVotePlayer[k]) && P_VALID(m_pMaxPointVotePlayer[t]))
			{

				if (m_pMaxPointVotePlayer[k]->dwVotePoint < m_pMaxPointVotePlayer[t]->dwVotePoint)
				{
					pVotePlayerTemp = m_pMaxPointVotePlayer[k];
					m_pMaxPointVotePlayer[k] = m_pMaxPointVotePlayer[t];
					m_pMaxPointVotePlayer[t] = pVotePlayerTemp;
				}
			}
			k++;
		}
	}
}

DWORD DraftShow::GetShowRankingMinPointPos()const  //获得选秀排名中最低分
{
     const tagShowPlayer* pFirstShowPlayer = GetMaxPointShowPlayer(1);
	 const tagShowPlayer* pLastShowPlayer = pFirstShowPlayer ;
	 DWORD dwPos = 0;
	 if ( !P_VALID(pFirstShowPlayer) )
	 {	 
			 return 0;
		 	 
	 }

	 for ( INT i = 0 ; i < MAX_VOTES_PERSON_NUM; i++)
	 {
		  dwPos =  i+1;
		  pFirstShowPlayer = GetMaxPointShowPlayer(i+1);
		 
		 if ( !P_VALID(pFirstShowPlayer) )
		 {
			return dwPos-1;
			
		 }

		 pLastShowPlayer = pFirstShowPlayer ;

	 }
	 return dwPos;
	
}

DWORD DraftShow::GetVoteRankingMinPointPos()const  //获得投票排名中最低分
{
	const tagVotePlayer* pFirstVotePlayer = GetMaxPointVotePlayer(1);
	const tagVotePlayer* pLastVotePlayer = pFirstVotePlayer ;
	DWORD dwPos = 0;
	if ( !P_VALID(pFirstVotePlayer) )
	{
			return 0;
	}

	for ( INT i = 0 ; i < MAX_VOTES_PERSON_NUM;i++)
	{
		dwPos =  i+1;
		pFirstVotePlayer = GetMaxPointVotePlayer(i+1);

		if ( !P_VALID(pFirstVotePlayer) )
		{
			return dwPos-1;
		}

		pLastVotePlayer = pFirstVotePlayer ;

	}
	return dwPos;

}

//投票
DWORD  DraftShow::Vote(DWORD dwSignNum,DWORD dwVoteRoleId)
{
     tagShowPlayer*  pShowPlayer = GetShowPlayer(dwSignNum);
	 if ( !P_VALID(pShowPlayer) )
	 {
		 return E_VoteShow_SignIdInvaild;  //此报名编号不存在
	 }
     pShowPlayer->dwAthletePoint++;
	 tagNDBC_SaveShowPlayerPoint MsgShowPlayerPoint;
     MsgShowPlayerPoint.showPlayer.dwAthleteSignId = pShowPlayer->dwAthleteSignId;
     MsgShowPlayerPoint.showPlayer.dwAthletePoint  = pShowPlayer->dwAthletePoint;
	 _tcscpy(MsgShowPlayerPoint.showPlayer.tcAthleteRoleName,pShowPlayer->tcAthleteRoleName);
	 MsgShowPlayerPoint.showPlayer.dwAthleteRoleId = pShowPlayer->dwAthleteRoleId;
	 MsgShowPlayerPoint.showPlayer.dwAthleteSerialId = pShowPlayer->dwAthleteSerialId;
	 g_dbSession.Send(&MsgShowPlayerPoint,MsgShowPlayerPoint.dwSize);
     DWORD  dwMinPointShowPlayerPos = GetShowRankingMinPointPos();
	 if ( 0 == dwMinPointShowPlayerPos ) //排名表中没有人
	 {
          ModMaxPointShowPlayer(1,pShowPlayer);
	 }
	 else
	 {
		  AddMaxPointShowPlayer(pShowPlayer);
         
	 }

     VoteReward(dwVoteRoleId);
	 return 0;
  
}


//投票自己得分
DWORD  DraftShow::VoteReward(DWORD dwVoteRoleId)
{

	tagVotePlayer* pVotePlayer = GetVotePlayer(dwVoteRoleId);
	if ( P_VALID(pVotePlayer) )  //已经投过票的
	{
		pVotePlayer->dwVotePoint++;

	}
	else //没有投过票的
	{
		pVotePlayer = new tagVotePlayer;
		if( !P_VALID(pVotePlayer) )
		{
			IMSG(_T("allocate memory failed <%s>!\n"), "new tagVotePlayer");
			return -1;
		}
		m_dwSerialId ++;
		pVotePlayer->dwVoteRoleId  = dwVoteRoleId ;
		g_roleMgr.GetRoleNameByID(dwVoteRoleId,pVotePlayer->tcVoteRoleName);
		pVotePlayer->dwVotePoint   = 1;
		pVotePlayer->dwSerialId    = m_dwSerialId;
		m_MapVotePlayer.Add(dwVoteRoleId,pVotePlayer);	  
	}
	tagNDBC_SaveVotePlayerPoint MsgVotePlayerPoint;
	MsgVotePlayerPoint.VotePlayer.dwVoteRoleId = pVotePlayer->dwVoteRoleId;
	_tcscpy(MsgVotePlayerPoint.VotePlayer.tcVoteRoleName,pVotePlayer->tcVoteRoleName);
	MsgVotePlayerPoint.VotePlayer.dwVotePoint  = pVotePlayer->dwVotePoint;
	MsgVotePlayerPoint.VotePlayer.dwSerialId   = pVotePlayer->dwSerialId;
	g_dbSession.Send(&MsgVotePlayerPoint,MsgVotePlayerPoint.dwSize);


	tagNDBC_SaveVotePlayerDataCount  MsgVotePlayerCount;
    MsgVotePlayerCount.n16RoleId = 0 ;
	MsgVotePlayerCount.dwTotalDataCount = m_MapVotePlayer.Size();
    
	g_dbSession.Send(&MsgVotePlayerCount,MsgVotePlayerCount.dwSize);
	DWORD  dwMinPointVotePlayerPos = GetVoteRankingMinPointPos();
	if ( 0 == dwMinPointVotePlayerPos ) //排名表中没有人
	{
		ModMaxPointVotePlayer(1,pVotePlayer);
	}
	else
	{
		AddMaxPointVotePlayer(pVotePlayer);
		
		

	}
	return 0;


}

DWORD   DraftShow::ReadSignDataTable(DWORD)
{
	CreateObj("DraftShowContainer", "VarContainer");
	m_pVar = "DraftShowContainer";
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, FileName_DraftShow, "id", &ListField))
	{
		IMSG(_T("There is something wrong in reading proto<%s>!\n"), FileName_DraftShow);
		return FALSE;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		tagShowPlayer* pDraftNode = new tagShowPlayer;
    	if( !P_VALID(pDraftNode) )
		{
			IMSG(_T("There is something wrong in reading proto<%s>!\n"), FileName_DraftShow);
			return FALSE;
		}
        DWORD* p = (DWORD*)SovleSignFile(pDraftNode,it->c_str());
		if( !P_VALID(p) )
		{
			IMSG(_T("There is something wrong in reading proto<%s>!\n"), FileName_DraftShow);
			SAFE_DEL(pDraftNode);
			continue;
		}
		tagShowPlayer* pDraftNodeTemp = m_MapShowPlayer.Peek( pDraftNode->dwAthleteSignId);
        if ( P_VALID(pDraftNodeTemp))
        {
			if (pDraftNode->dwAthleteSignId == pDraftNodeTemp->dwAthleteSignId)
			{
                  IMSG(_T("TalentShow SignID <%d> is loaded twice time!\n"), pDraftNodeTemp->dwAthleteSignId);
				  SAFE_DEL(pDraftNode);
			}
			else
			{
				  IMSG(_T("TalentShow SignID <%d> is loaded failed!\n"), pDraftNodeTemp->dwAthleteSignId);
				  SAFE_DEL(pDraftNode);
			}
        }
		else
		{
			
			if ( 0 == pDraftNode->dwAthleteRoleId || !GT_VALID(pDraftNode->dwAthleteRoleId))
			{
				IMSG(_T("Role's name <%s>! is  invalid!The sign ID is <%d>\n"), pDraftNode->tcAthleteRoleName,pDraftNode->dwAthleteSignId);
				SAFE_DEL(pDraftNode);
				continue;

			}
			m_dwShowSerialId++;
			pDraftNode->dwAthleteSerialId = m_dwShowSerialId;
			// 放入map中
			m_MapShowPlayer.Add(pDraftNode->dwAthleteSignId, pDraftNode);
		}
           
		
	}

	// 发送读入基本物品数目信息
	IMSG(_T("Read %d records from file<%s>!\n"), m_MapShowPlayer.Size(), FileName_DraftShow);
    
	SaveSignToDB();
	// 清空容器
	m_pVar->Clear();

	return 0;


}

VOID*   DraftShow::SovleSignFile(OUT LPVOID nProtoType, LPCTSTR szField)
{
   MTRANS_POINTER(pShowPlayer, nProtoType, tagShowPlayer);
   pShowPlayer->dwAthleteSignId = (DWORD)m_pVar->GetDword(_T("id"), szField, GT_INVALID);
   LPCTSTR szName = (LPCTSTR)m_pVar->GetString(_T("name"), szField);
   _tcscpy(pShowPlayer->tcAthleteRoleName,szName);
   DWORD  dwNameCrc = TObjRef<Util>()->Crc32(szName);
   DWORD  dwRoleID  = g_roleMgr.GetRoleIDByNameCrc(dwNameCrc);
   if ( GT_VALID(dwRoleID))
   {
	   pShowPlayer->dwAthleteRoleId	= dwRoleID ;
   }
   else
   {
	   pShowPlayer->dwAthleteRoleId	= 0 ;
   }
   
   pShowPlayer->dwAthletePoint	= 0;
   return &pShowPlayer->dwAthleteSignId;
}


VOID   DraftShow::SaveSignToDB()
{

	  
     tagNDBC_SaveShowPlayerData   SendMsg;
     DWORD dwCount = m_MapShowPlayer.Size();
	 if ( dwCount <= 0)
	 {
		 return ;
	 }
	 
	 DWORD dwSize  = sizeof(tagNDBC_SaveShowPlayerData) + sizeof(tagShowPlayer)*(dwCount);
     MCREATE_MSG(pSend,dwSize, NDBC_SaveShowPlayerData);
     pSend->dwCount = dwCount + 1;
     tagShowPlayer* pDraftData = NULL;
	 TMap<DWORD,tagShowPlayer*>::TMapIterator it = m_MapShowPlayer.Begin();
	 DWORD  dwI = 0;
	 pSend->data[dwI].dwAthleteSignId = 0;
	 _tcscpy(pSend->data[dwI].tcAthleteRoleName,_T("Flags"));
	 pSend->data[dwI].dwAthleteRoleId  = 0;
	 pSend->data[dwI].dwAthletePoint   = 0;
     pSend->data[dwI].dwAthleteSerialId= 0;
	 while (m_MapShowPlayer.PeekNext(it,pDraftData) )
	 {
		 if ( P_VALID(pDraftData))
		 {
			 if (dwI >= (dwCount + 1) )
			 {

				 return ;
			 }
			 dwI++;
			 pSend->data[dwI].dwAthleteSignId = pDraftData->dwAthleteSignId;
			 _tcscpy(pSend->data[dwI].tcAthleteRoleName,pDraftData->tcAthleteRoleName);
			 pSend->data[dwI].dwAthleteRoleId  = pDraftData->dwAthleteRoleId;
			 pSend->data[dwI].dwAthletePoint   = pDraftData->dwAthletePoint;
             pSend->data[dwI].dwAthleteSerialId= pDraftData->dwAthleteSerialId; 


		 }
	 }
	 g_dbSession.Send(pSend,pSend->dwSize);
	 MDEL_MSG(pSend);
     
	 tagNDBC_SaveShowPlayerDataCount msg;
	 msg.n16SignId = 0 ;
	 msg.dwTotalDataCount = dwCount;
	 g_dbSession.Send(&msg,msg.dwSize);
}



DWORD  DraftShow::GetShowPlayerRankingsSize()
{
	 DWORD dwSize = 0;
	 tagShowPlayer* pShowPlayer = NULL;
     for( INT i = 0 ; i < MAX_VOTES_PERSON_NUM ; i++ )
	 {
         pShowPlayer = m_pMaxPointShowPlayer[i];
		 if ( P_VALID(pShowPlayer) )
		 {
			dwSize++;
		 }
		 else
		 {
			 return dwSize;
		 }
	 }
	 return dwSize;
}

DWORD  DraftShow::GetVotePlayerRankingsSize()
{
	DWORD dwSize = 0;
	tagVotePlayer*  pVotePlayer = NULL;
	for( INT i = 0 ; i < MAX_VOTES_PERSON_NUM ; i++ )
	{
		pVotePlayer = m_pMaxPointVotePlayer[i];
		if( P_VALID(pVotePlayer) )
		{
            dwSize++;
		}
		else
		{
			return dwSize;
		}

	}
	return dwSize;
}

DWORD  DraftShow::LoadDataFromDB()
{
	
		tagNDBC_GetShowPlayerData Msg1;
		g_dbSession.Send(&Msg1,Msg1.dwSize);

		tagNDBC_GetVotePlayerData Msg2;
		g_dbSession.Send(&Msg2,Msg2.dwSize);

		m_dwSerialId = 0 ;

		g_WorldExpo.LoadRankDataFromDB();
	
	return 0;
}


WorldExpo g_WorldExpo;


//------------------------------------------------------------------------------------------------
// 世博活动
//------------------------------------------------------------------------------------------------

WorldExpo::WorldExpo()
{
    
}

WorldExpo::~WorldExpo()
{

}

VOID WorldExpo::Init()
{

}

VOID WorldExpo::Update()
{
    
}

VOID WorldExpo::Destroy()
{
	std::list<tagActiveRankData*> listActiveRankData;
	m_MapActiveRankData.ExportAllValue(listActiveRankData);
	std::list<tagActiveRankData*>::iterator it = listActiveRankData.begin();
	for (; it != listActiveRankData.end(); it++)
	{
		tagActiveRankData* pActiveRankData = (*it);
		if ( P_VALID(pActiveRankData) )
		{
            SAFE_DEL(pActiveRankData);
		}

	}
	m_MapActiveRankData.Clear();


}

VOID WorldExpo::GetRoleDisplayRankings(DWORD dwRoleId,E_ActiveDataType eDataType,INT nLowRank, INT nHighRank)
{
	DWORD dwCount = m_MapActiveRankData.Size();
	if (  nLowRank <= 0  || nHighRank < nLowRank)
	{
		ASSERT(0);
		return;
	}

	if ( eDataType < E_DataTypeNull || eDataType >= E_DataTypeEnd)
	{
		ASSERT(0);
		
		return ;
	}
	INT nlowPos  = nLowRank - 1;
    INT nHighPos = nHighRank- 1;
     
	SortDisplayRankings(eDataType);
	
	
	
    DWORD dwSendCount = nHighRank - nLowRank + 1;
	DWORD dwSize = sizeof(tagNS_QueryDragonDefenderRankings) + (dwSendCount-1)*sizeof(tagVoteShowRankingsData);
	MCREATE_MSG(pSend,dwSize, NS_QueryDragonDefenderRankings);
	if ( dwSendCount > dwCount)
	{
        pSend->n16Count = dwCount;
	}
	else
	{
         pSend->n16Count = dwSendCount;
	}
	
	for ( INT i = 0;  i < pSend->n16Count; i++)
	{
        if ( (nlowPos+ i) >= dwCount) //防止越界
        {
			break;
        }
		pSend->data[i].dwPoint    = m_vecScore[nlowPos + i].second;
		g_roleMgr.GetRoleNameByID(m_vecScore[nlowPos+ i].first,pSend->data[i].tcRoleName);

	}


    Role* pRole = g_roleMgr.GetRolePtrByID( dwRoleId) ;
	if ( P_VALID(pRole) )
	{
        pRole->SendMessage(pSend,pSend->dwSize); 
	}
	MDEL_MSG(pSend);
	

}

VOID WorldExpo::ModRankingsData(DWORD dwRoleId,E_ActiveDataType eDataType,INT nValue)
{
	 
	  if ( eDataType < E_DataTypeNull || eDataType >= E_DataTypeEnd)
	  {
		  ASSERT(0);
		 
		  return ;
	  }
	  Role* pRole = g_roleMgr.GetRolePtrByID( dwRoleId);
	  if( !P_VALID(pRole) )
	  {
		 
		  return ;
	  }
      tagActiveRankData* p = GetActiveRankData(dwRoleId);
	  if ( P_VALID(p) )
	  {
		  p->nDataType[eDataType] += nValue;
          if ( P_VALID(pRole))
          {
			  p->n16RoleLevel = pRole->GetLevel();
          }
	  }
	  else
	  {
          p = new tagActiveRankData; 
		  if ( !P_VALID(p))
		  {		   
			  return ;
		  }
        
          if ( P_VALID(pRole))
          {
              p->n16RoleLevel = pRole->GetLevel();
          }
              
		  p->dwRoleId = dwRoleId;
		  g_roleMgr.GetRoleNameByNameID(dwRoleId,p->ucRoleName);
		  p->nDataType[eDataType] += nValue;
		  AddActiveRankData(p);
	  }
	  tagNDBC_SaveActiveRankData dbMsg;
	  dbMsg.ActiveRankData.dwRoleId = p->dwRoleId;
	  dbMsg.ActiveRankData.n16RoleLevel = p->n16RoleLevel;
	  _tcscpy(dbMsg.ActiveRankData.ucRoleName,p->ucRoleName);
	  memcpy(&dbMsg.ActiveRankData.nDataType,p->nDataType,sizeof(INT32)*(8));
	  g_dbSession.Send(&dbMsg,dbMsg.dwSize);
	 
}

INT WorldExpo::GetRoleRankingsData(DWORD dwRoleId,E_ActiveDataType eDataType)
{
	
	if ( eDataType < E_DataTypeNull || eDataType >= E_DataTypeEnd)
	{
		ASSERT(0);
		return 0;
	}	
      tagActiveRankData* p = GetActiveRankData(dwRoleId);
	  if ( P_VALID(p) )
	  {
		  INT nVal = p->nDataType[eDataType];
		
		 return   nVal ;
	  }
	  else
	  {
		  return 0;
	  }
	  
	  
	  
}

tagActiveRankData* WorldExpo::GetActiveRankData(DWORD dwRoleId)
{ 

	 if ( !GT_VALID(dwRoleId))
	 {
		 return NULL;
	 }
     tagActiveRankData* p = m_MapActiveRankData.Peek(dwRoleId);  
	 return p;
}


VOID WorldExpo::AddActiveRankData(const tagActiveRankData* pActiveRankData)
{
	if ( P_VALID(pActiveRankData) )
	{
        m_MapActiveRankData.Add(pActiveRankData->dwRoleId,(tagActiveRankData*)pActiveRankData);
	}
	 
}

VOID WorldExpo::SortDisplayRankings(E_ActiveDataType eDataType)
{
	if ( eDataType < E_DataTypeNull || eDataType >= E_DataTypeEnd)
	{
		ASSERT(0);
		return ;
	}
	m_vecScore.clear();

	// 开始排序
	std::list<tagActiveRankData*> listActiveRankData;
	m_MapActiveRankData.ExportAllValue(listActiveRankData);
	std::list<tagActiveRankData*>::iterator it =  listActiveRankData.begin();
	for(; it != listActiveRankData.end(); it++)
	{
            tagActiveRankData* pData = (*it);
			if ( P_VALID(pData))
			{
               m_vecScore.push_back(make_pair(pData->dwRoleId, pData->nDataType[eDataType]));
			}
	}

	m_MapActiveRankData.Lock();
    sort(m_vecScore.begin(), m_vecScore.end(), CScoreActiveRank());
	m_MapActiveRankData.Unlock();
}

VOID WorldExpo::LoadRankDataFromDB()
{
     tagNDBC_GetActiveRankData msg;
	 g_dbSession.Send(&msg,msg.dwSize);
}