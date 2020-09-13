//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_room_mgr.cpp
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: 竞技场房间管理器
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "mutex.h"

#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"


#include "role.h"
#include "role_mgr.h"
#include "athletics_room.h"
#include "athletics_pvp.h"
#include "athletics_pvp_mgr.h"
#include "athletics_room_mgr.h"
#include "athletics_system.h" 
#include "../ServerDefine/mini_tool.h"


DWORD  AthleticsRoomMgr::m_sdwRoomSerialID = 0;

AthleticsRoomMgr::AthleticsRoomMgr()
{

	m_MapAthleticsRoom.Clear(); 
	m_MapJuniorAthleticsRoom.Clear();
	m_MapInterMediateAthleticsRoom.Clear();
	m_MapSeniorAthleticsRoom.Clear();
	m_ListEnterPvPRoom.Clear();
    m_dwTotalRoomNum = 0;
	m_dwTotalJuniorRoomNum = 0;
	m_dwTotalInterMediateRoomNum = 0;
	m_dwTotalSeniorRoomNum = 0;
	
}


AthleticsRoomMgr::~AthleticsRoomMgr()
{

	m_MapJuniorAthleticsRoom.Clear();
	m_MapInterMediateAthleticsRoom.Clear();
	m_MapSeniorAthleticsRoom.Clear();
}

BOOL AthleticsRoomMgr::Init()
{
    return TRUE;
}

BOOL AthleticsRoomMgr::Destroy()
{
	RoomBase* pRoom = NULL;

	//---------------------------------------------------------------------------------------------
	//删除房间列表
	//---------------------------------------------------------------------------------------------
	MapAthleticsRoom::TMapIterator iter = m_MapAthleticsRoom.Begin();
	while (m_MapAthleticsRoom.PeekNext(iter, pRoom))
	{
		if (P_VALID(pRoom))
		{
		   pRoom->Destroy();
           SAFE_DEL(pRoom);
		}
		
	}
    
	//---------------------------------------------------------------------------------------------
	//删除房间号的列表
	//---------------------------------------------------------------------------------------------
    tagAthRoomId* pRoomId =  NULL;
	ListJuniorAthRoomId::TListIterator it1 = m_ListJuniorAthRoomId.Begin();
	while( m_ListJuniorAthRoomId.PeekNext(it1,pRoomId) )
	{
		if ( P_VALID( pRoomId )  )
		{
			if( pRoomId->eRoomLevel == E_PvP_Junior ) 
			{

				 SAFE_DEL(pRoomId);
			}
		}
             
	}
	pRoomId =  NULL;
	ListInterMediateAthRoomId::TListIterator it2 = m_ListInterMediateAthRoomId.Begin();
	while( m_ListInterMediateAthRoomId.PeekNext(it2,pRoomId) )
	{
		if ( P_VALID( pRoomId )  )
		{
			if( pRoomId->eRoomLevel == E_PvP_Middle ) 
			{

				SAFE_DEL(pRoomId);
			}
		}
	
		
	}
	pRoomId =  NULL;
    
	ListSeniorAthRoomId::TListIterator it3 = m_ListSeniorAthRoomId.Begin();
	while( m_ListSeniorAthRoomId.PeekNext(it3,pRoomId) )
	{
		if ( P_VALID( pRoomId )  )
		{
			if( pRoomId->eRoomLevel == E_PvP_Senior ) 
			{

				SAFE_DEL(pRoomId);
			}
		}
		
	}
    
	//---------------------------------------------------------------------------------------------
	//删除大厅的房间信息列表
	//---------------------------------------------------------------------------------------------
	tagPvPRoom* pRoomInfo = NULL;
	TMapJuniorAthleticsRoom::TMapIterator itRoomInfo1 =  m_MapJuniorAthleticsRoom.Begin() ;
	while( m_MapJuniorAthleticsRoom.PeekNext(itRoomInfo1,pRoomInfo))
	{
         if( P_VALID( pRoomInfo ) )
		 {
			 SAFE_DEL(pRoomInfo);
		 }
	}
    
	pRoomInfo = NULL;
    TMapInterMediateAthleticsRoom::TMapIterator itRoomInfo2 =  m_MapInterMediateAthleticsRoom.Begin() ;
	while (m_MapInterMediateAthleticsRoom.PeekNext(itRoomInfo2,pRoomInfo))
	{
		if( P_VALID( pRoomInfo ) )
		{
			SAFE_DEL(pRoomInfo);
		}
	}

	pRoomInfo = NULL;
	TMapSeniorAthleticsRoom::TMapIterator itRoomInfo3 =  m_MapSeniorAthleticsRoom.Begin() ;
	while (m_MapSeniorAthleticsRoom.PeekNext(itRoomInfo3,pRoomInfo))
	{
		if( P_VALID( pRoomInfo ) )
		{
			SAFE_DEL(pRoomInfo);
		}
	}
    tagPvPRoleInRoom* pRoleInRoom = NULL;
	MapRoleInAthletisc::TMapIterator itRoleInRoom = m_MapRoleInAthletisc.Begin();
	while (m_MapRoleInAthletisc.PeekNext(itRoleInRoom,pRoleInRoom))
	{
		if( P_VALID( pRoleInRoom ) )
		{
			SAFE_DEL(pRoleInRoom);
		}
	}

	m_MapJuniorAthleticsRoom.Clear();
	m_MapInterMediateAthleticsRoom.Clear();
	m_MapSeniorAthleticsRoom.Clear();
    
	m_MapRoleInAthletisc.Clear();
	m_MapAthleticsRoom.Clear();
    return TRUE;
}

VOID AthleticsRoomMgr::Update()
{
	if ( m_dwTotalRoomNum <= 0 )  
	{
		m_sdwRoomSerialID = 0;
		return ;
	}
	RoomBase* pRoom = NULL;
	MapAthleticsRoom::TMapIterator iter = m_MapAthleticsRoom.Begin();
	while (m_MapAthleticsRoom.PeekNext(iter, pRoom))
	{
		if (P_VALID(pRoom)  )
		{
			if( !pRoom->GetIsPvPRoleEnterPvP() )//已经进入竞技场的房间不更新
			{

        		pRoom->Update();
				//假如房间进入战斗状态　就把他指针加到竞技场队列

				if( pRoom->GetRoomState() == E_PvP_Fighting )
				{	
						AddPvPRoomToPvPQueue(pRoom);
						pRoom->SetIsPvPRoleEnterPvP(TRUE);				
				}

				if( pRoom->GetRoomState() == E_PvP_RoomStateNull )
				{

						DWORD dwSerialID = pRoom->GetRoomSerialID() ;
						PvPMgr* pPvpMgr = g_AthleticsSystem.GetPvPMgrPtr();
						if( P_VALID(pPvpMgr) )
						{
							AthleticsPvP* pAthleticsPvP = pPvpMgr->GetPvP(dwSerialID);
							if (P_VALID(pAthleticsPvP)) 
							{
                                pAthleticsPvP->ClosePvP(TRUE);
							}   
						}

						pRoom->Destroy();
						DeleteAthleticsRoom(dwSerialID,pRoom);
					
				}
               
			}

			

		}
	}

	

}

//创建房间和删除房间
DWORD AthleticsRoomMgr::CreateAthleticsRoom(const tagPvPRoom& pvpRoom)
{
	E_PvP_RoomType  eRoomType    = pvpRoom.eRoomType;
	INT16             nRoomID    = pvpRoom.nRoomID;
	E_PvP_RoomLevel eRoomLevel   = pvpRoom.eLevel;
    RoomBase* pRoom = NULL;
	m_sdwRoomSerialID++;
	
	tagPvPRoleInRoom* pRoleInRoom = GetInRoomRole(pvpRoom.dwManagerID);
	if( !P_VALID(pRoleInRoom) )
	{
		m_sdwRoomSerialID--;
		return E_PvP_NotInHall;

	}
	//是否已经在房间
	if ( 0 != pRoleInRoom->dwSerialID )
	{
		m_sdwRoomSerialID--;
		return E_PvP_AlreadyInRoom;
	}
    pRoleInRoom->dwSerialID = m_sdwRoomSerialID; 
	switch( eRoomType )
	{
	case E_PvP_Room3v3:
		pRoom = new Athletics3V3Room;
		if ( !P_VALID(pRoom) )
		{
			IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)E_PvP_Room3v3);
			return E_PvP_CreateRoomFail;
		}

		 break;
	case E_PvP_Room6v6:
		return E_PvP_CreateRoomFail;
		break;
	default:
		return E_PvP_CreateRoomFail;
		break;
	}
	

	if (!pRoom->Init(pvpRoom,m_sdwRoomSerialID))
	{
		IMSG(_T("The Athletics room of initialization failed cityid:%d\r\n"), m_sdwRoomSerialID);
		SAFE_DEL(pRoom);
		pRoleInRoom->dwSerialID = 0 ;
		return E_PvP_CreateRoomFail;
	}
    
	switch( eRoomLevel )
	{
	case E_PvP_Junior:
		m_dwTotalJuniorRoomNum++;
        //m_MapJuniorAthleticsRoom.Add(nRoomID,pvpRoom);
        SetRoomIdState(E_PvP_Junior,nRoomID,ERIDS_Created);
       
		AddListRoom(pvpRoom,m_sdwRoomSerialID);
		break;
	case E_PvP_Middle:
		m_dwTotalInterMediateRoomNum++;
		//m_MapInterMediateAthleticsRoom.Add(nRoomID,pvpRoom);
		 SetRoomIdState(E_PvP_Middle,nRoomID,ERIDS_Created);
		 AddListRoom(pvpRoom,m_sdwRoomSerialID);
		break;
	case E_PvP_Senior:
        m_dwTotalSeniorRoomNum++;
        //m_MapSeniorAthleticsRoom.Add(nRoomID,pvpRoom);
	    SetRoomIdState(E_PvP_Senior,nRoomID,ERIDS_Created);
		AddListRoom(pvpRoom,m_sdwRoomSerialID);
		break;
	}
    
// 	if( !P_VALID(pRoleInRoom) )
// 	{ 
// 		IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)E_PvP_Room3v3);
// 		return FALSE;       
// 	}
// 
//     pRoleInRoom->dwRoleID   = pvpRoom.dwManagerID;
// 	pRoleInRoom->dwSerialID = m_sdwRoomSerialID;
// 	AddRoleToAthletics(pRoleInRoom);

	m_dwTotalRoomNum++;
    pRoom->SetRoomState(E_PvP_Waiting);
	m_MapAthleticsRoom.Add(m_sdwRoomSerialID, pRoom); 

	 
    
     

   
   return  E_PvP_Success;
}

BOOL AthleticsRoomMgr::DeleteAthleticsRoom(DWORD dwSerialID,RoomBase* pRoom)
{
 
    Athletics3V3Room* p3V3Room = NULL;
	if( !P_VALID(pRoom) )
	{
        return FALSE;
	}
	if( pRoom->GetRoomType() == E_PvP_Room3v3 )
	{
         p3V3Room = (Athletics3V3Room*)pRoom;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	E_PvP_RoomLevel eRoomLevel = p3V3Room->GetRoomLevel();
	DWORD           dwRoomID   = p3V3Room->GetRoomID();
	E_PvP_RoomType  eRoomType  = p3V3Room->GetRoomType(); 
	switch(eRoomLevel)
	{
	case E_PvP_Junior:
         SetRoomIdState(E_PvP_Junior,dwRoomID,ERIDS_Vacant);
		 DeleteListRoom(eRoomType,E_PvP_Junior,static_cast<INT16>(dwRoomID));
		 m_dwTotalJuniorRoomNum--;

        
		break;
	case E_PvP_Middle:
		SetRoomIdState(E_PvP_Middle,dwRoomID,ERIDS_Vacant); 
		DeleteListRoom(eRoomType,E_PvP_Middle,static_cast<INT16>(dwRoomID));
		m_dwTotalInterMediateRoomNum--;

		break;
	case E_PvP_Senior:
		SetRoomIdState(E_PvP_Senior,dwRoomID,ERIDS_Vacant); 
		DeleteListRoom(eRoomType,E_PvP_Senior,static_cast<INT16>(dwRoomID));
        m_dwTotalSeniorRoomNum--;
		break;
	}

	SAFE_DEL(pRoom);
	m_MapAthleticsRoom.Erase(dwSerialID);
	m_dwTotalRoomNum--;
             
    return TRUE;
}


//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
DWORD AthleticsRoomMgr::AddToHall(E_PvP_RoomType eRoomType,DWORD dwRoleID)
{
	E_PvP_RoomType roleType = eRoomType;

	///看角色是否已经删除了
	tagPvPRoleInRoom* pRoleInRoomTemp = GetInRoomRole(dwRoleID);

	if( P_VALID(pRoleInRoomTemp ))
	{
		if( pRoleInRoomTemp->dwRoleID == dwRoleID)
		{
			return 0;
		}
	}


	tagPvPRoleInRoom* pRoleInRoom = new tagPvPRoleInRoom;
	if( !P_VALID(pRoleInRoom) )
	{ 
		IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)E_PvP_Room3v3);
		return GT_INVALID;       
	}
	switch(roleType)
	{
	case E_PvP_Room3v3: 
      	pRoleInRoom->dwRoleID   = dwRoleID;
		pRoleInRoom->dwSerialID = 0;
		AddRoleToAthletics(pRoleInRoom);
		break;
	case E_PvP_Room6v6:
		SAFE_DEL(pRoleInRoom);
		return GT_INVALID;
		break;
	default:
		SAFE_DEL(pRoleInRoom);
		return GT_INVALID;
		break;
	}	
    return E_Success;
}
//--------------------------------------------------------------------------
//结束
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
DWORD AthleticsRoomMgr::LeaveHall(DWORD dwRoleID)
{
	OnOffLine(dwRoleID);
	return E_Success;
}


BOOL  AthleticsRoomMgr::IsRoleExist(DWORD dwRoleID)
{
	tagPvPRoleInRoom* pRoleInRoom = GetInRoomRole(dwRoleID);
	if( P_VALID(pRoleInRoom) )
	{
	    RoomBase* pRoom = GetPvPRoom(pRoleInRoom->dwSerialID);
		if (!P_VALID(pRoom) )
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
}
//--------------------------------------------------------------------------
//结束
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//SM 竞技场的最近修改
//增加该部分功能 
//--------------------------------------------------------------------------
//竞技场活动结束
VOID AthleticsRoomMgr::AthleticsActiveOver()
{
    tagPvPRoleInRoom* pRoleInRoom = NULL;
	m_MapRoleInAthletisc.ResetIterator();
    while( m_MapRoleInAthletisc.PeekNext(pRoleInRoom) )
	{
		if ( P_VALID(pRoleInRoom))
		{
			RoomBase* pRoom = GetPvPRoom(pRoleInRoom->dwSerialID);
			if (P_VALID(pRoom) )
			{
				if ( E_PvP_Waiting == pRoom->GetRoomState())
				{
					OnOffLine(pRoleInRoom->dwRoleID,FALSE);
				}
			}
		}
	}

}

BOOL AthleticsRoomMgr::AddListRoom(const tagPvPRoom& pvpRoom,DWORD dwSerialId)
{
	INT16 nRoomID              = pvpRoom.nRoomID;
    E_PvP_RoomLevel eRoomLevel  = pvpRoom.eLevel;
    E_PvP_RoomType eRoomType   = pvpRoom.eRoomType;
	tagPvPRoom* pRoomInfo = NULL;

	pRoomInfo = new tagPvPRoom;
	if ( !P_VALID(pRoomInfo) )
	{
		IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)E_PvP_Junior);
		return FALSE;
	}
	pRoomInfo->dwManagerID      = pvpRoom.dwManagerID;
	pRoomInfo->dwWuXunLimit     = pvpRoom.dwWuXunLimit;
	pRoomInfo->eLevel           = pvpRoom.eLevel;
	pRoomInfo->eRoomState       = pvpRoom.eRoomState;
	pRoomInfo->eRoomType        = pvpRoom.eRoomType;
	pRoomInfo->iLevelLowerLimit = pvpRoom.iLevelLowerLimit;
	pRoomInfo->iLevelUpperLimit = pvpRoom.iLevelUpperLimit;
	pRoomInfo->nRoomID          = pvpRoom.nRoomID;
	//_tcscpy(pRoomInfo->szManagerName, pvpRoom.szManagerName);
	jm_tscopy_s(pRoomInfo->szManagerName,X_SHORT_NAME, pvpRoom.szManagerName);
	pRoomInfo->ucCurrentPersonNum = pvpRoom.ucCurrentPersonNum ;
	pRoomInfo->ucTotalPersonNum   = pvpRoom.ucTotalPersonNum ;
    pRoomInfo->dwSerialID       = dwSerialId;
	switch( eRoomType )
	{
	case E_PvP_Room3v3:
		switch( eRoomLevel )
		{
		case E_PvP_Junior:			
             m_MapJuniorAthleticsRoom.Add(nRoomID,pRoomInfo);
			break;
		case E_PvP_Middle:
			m_MapInterMediateAthleticsRoom.Add(nRoomID,pRoomInfo);
			break;
		case E_PvP_Senior:
			m_MapSeniorAthleticsRoom.Add(nRoomID,pRoomInfo);
			break;
		}

	case E_PvP_Room6v6:
		ASSERT(0);
		return FALSE;
		break;
	default:
		ASSERT(0);
		return FALSE;
		break;
	}
    return FALSE;
}

BOOL AthleticsRoomMgr::DeleteListRoom(E_PvP_RoomType eRoomType,E_PvP_RoomLevel eRoomLevel,INT16 nRoomId)
{
    tagPvPRoom* pRoomInfo = NULL;
	switch( eRoomType )
	{
	case E_PvP_Room3v3:

		switch( eRoomLevel )
		{
		case E_PvP_Junior:
			pRoomInfo = m_MapJuniorAthleticsRoom.Peek(nRoomId);
			if ( P_VALID( pRoomInfo ))
			{
				SAFE_DEL(pRoomInfo);
				m_MapJuniorAthleticsRoom.Erase(nRoomId);
			}
			
			break;
		case E_PvP_Middle:
			pRoomInfo = m_MapInterMediateAthleticsRoom.Peek(nRoomId);
			if ( P_VALID( pRoomInfo ))
			{
				SAFE_DEL(pRoomInfo);
				m_MapInterMediateAthleticsRoom.Erase(nRoomId);
			}
			break;
		case E_PvP_Senior:
			pRoomInfo = m_MapSeniorAthleticsRoom.Peek(nRoomId);
			if ( P_VALID( pRoomInfo ))
			{
				SAFE_DEL(pRoomInfo);
				m_MapSeniorAthleticsRoom.Erase(nRoomId);
			}
			break;
		}
		break;
	case E_PvP_Room6v6:
		ASSERT(0);
		return FALSE;
		break;
	default:
		ASSERT(0);
		return FALSE;
		break;
	}
    return TRUE;
}

//-------------------------------------------------------------------------------------------------------------
// 获取有效房间号
//-------------------------------------------------------------------------------------------------------------
DWORD AthleticsRoomMgr::GetAvailRoomId(E_PvP_RoomLevel eRoomLevel)
{
	 tagAthRoomId* pRoomId = NULL ;
	 DWORD dwRoomId = 1;
	 ListJuniorAthRoomId::TListIterator       it1 = m_ListJuniorAthRoomId.Begin();
	 ListInterMediateAthRoomId::TListIterator it2 = m_ListInterMediateAthRoomId.Begin();
	 ListSeniorAthRoomId::TListIterator       it3 = m_ListSeniorAthRoomId.Begin();
     switch(eRoomLevel)
	 {
	 case E_PvP_Junior:
		 while( m_ListJuniorAthRoomId.PeekNext(it1,pRoomId) )
		 {
			 if ( P_VALID( pRoomId )  )
			 {
				 if( pRoomId->eRoomLevel == E_PvP_Junior && pRoomId->eRoomIDState != ERIDS_Created) 
				 {
					 
					return  pRoomId->nRoomID ;
	
				 }
			 }
			 dwRoomId++;
			
		 }
		
		 break;

	 case E_PvP_Middle:
		 
		 while( m_ListInterMediateAthRoomId.PeekNext(it2,pRoomId) )
		 {
			 
				 if( pRoomId->eRoomLevel == E_PvP_Middle && pRoomId->eRoomIDState != ERIDS_Created) 
				 {

					 return pRoomId->nRoomID ;
				 }
			 
			 dwRoomId++;
			 
		 }
		

		 break;
	case E_PvP_Senior:

		 while(  m_ListSeniorAthRoomId.PeekNext(it3,pRoomId) )
		 {
			 if ( P_VALID( pRoomId )  )
			 {
				 if( pRoomId->eRoomLevel == E_PvP_Senior && pRoomId->eRoomIDState != ERIDS_Created) 
				 {

					 return pRoomId->nRoomID ;
				 }
			 }
			 dwRoomId++;
			

		 }
		
		 break;

	 }
	  return dwRoomId;
}

//-------------------------------------------------------------------------------------------------------------
// 设置房间号的状态
//-------------------------------------------------------------------------------------------------------------
VOID  AthleticsRoomMgr::SetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId,E_RoomId_State eRoomIdState)
{
     tagAthRoomId* pRoomIdTemp =  NULL;
	 ListJuniorAthRoomId::TListIterator it1 = m_ListJuniorAthRoomId.Begin();
     ListInterMediateAthRoomId::TListIterator it2 = m_ListInterMediateAthRoomId.Begin(); 
	 ListSeniorAthRoomId::TListIterator it3 = m_ListSeniorAthRoomId.Begin();
     
	 tagAthRoomId* pRoomId1 = NULL;
     tagAthRoomId* pRoomId2 = NULL;
     tagAthRoomId* pRoomId3 = NULL;

	 switch( eRoomLevel )
	 {
	 case E_PvP_Junior:
		
 		 while( m_ListJuniorAthRoomId.PeekNext(it1, pRoomIdTemp) )
 		 {
 			 if ( P_VALID( pRoomIdTemp )  )
 			 {
 				 if( pRoomIdTemp->eRoomLevel == E_PvP_Junior && pRoomIdTemp->nRoomID == dwRoomId) 
 				 {
 					 pRoomIdTemp->eRoomIDState = eRoomIdState;
 					 pRoomIdTemp->nRoomID      = static_cast<INT16>(dwRoomId);
 					 return ;
 				 }
 			 }
 		 }
 		 pRoomId1 = new tagAthRoomId;
 		 if (!P_VALID( pRoomId1) )
 		 {
 			 IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)dwRoomId);
			 return ;
 		 }
 		 pRoomId1->eRoomLevel   = E_PvP_Junior ;
 		 pRoomId1->nRoomID      =  static_cast<INT16>(dwRoomId) ;
 		 pRoomId1->eRoomIDState = eRoomIdState ;
 		 m_ListJuniorAthRoomId.PushBack(pRoomId1);
		 break;
	 case E_PvP_Middle:
		 
		 while( m_ListInterMediateAthRoomId.PeekNext(it2, pRoomIdTemp) )
		 {
			 if ( P_VALID( pRoomIdTemp )  )
			 {
				 if( pRoomIdTemp->eRoomLevel == E_PvP_Middle && pRoomIdTemp->nRoomID == dwRoomId) 
				 {
					 pRoomIdTemp->eRoomIDState = eRoomIdState;
					 pRoomIdTemp->nRoomID      =  static_cast<INT16>(dwRoomId);
					 return;
				 }
			 }
		 }

		 pRoomId2 = new tagAthRoomId;
		 if (!P_VALID( pRoomId2) )
		 {
			 IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)dwRoomId);
			 return ;
		 }
		 pRoomId2->eRoomLevel   = E_PvP_Middle ;
		 pRoomId2->nRoomID      =  static_cast<INT16>(dwRoomId) ;
		 pRoomId2->eRoomIDState = eRoomIdState ;
		 m_ListInterMediateAthRoomId.PushBack(pRoomId2);
		 break;
	 case E_PvP_Senior:

		 
		 while( m_ListSeniorAthRoomId.PeekNext(it3, pRoomIdTemp) )
		 {
			 if ( P_VALID( pRoomIdTemp )  )
			 {
				 if( pRoomIdTemp->eRoomLevel == E_PvP_Senior && pRoomIdTemp->nRoomID == dwRoomId) 
				 {
					 pRoomIdTemp->eRoomIDState = eRoomIdState;
					 pRoomIdTemp->nRoomID      =  static_cast<INT16>(dwRoomId);
					 return;
				 }
			 }
		 }

		 pRoomId3 = new tagAthRoomId;
		 if (!P_VALID( pRoomId3) )
		 {
			 IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)dwRoomId);
			 return ;
		 }
		 pRoomId3->eRoomLevel   = E_PvP_Senior ;
		 pRoomId3->nRoomID      =  static_cast<INT16>(dwRoomId) ;
		 pRoomId3->eRoomIDState = eRoomIdState ;
		 m_ListSeniorAthRoomId.PushBack(pRoomId3);
		 break;
	 }
}

//-------------------------------------------------------------------------------------------------------------
// 获取房间号的状态
//-------------------------------------------------------------------------------------------------------------
tagAthRoomId*  AthleticsRoomMgr::GetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId)
{
 
   tagAthRoomId* pRoomId  = NULL ;
   ListJuniorAthRoomId::TListIterator it1 = m_ListJuniorAthRoomId.Begin();
   ListInterMediateAthRoomId::TListIterator it2 = m_ListInterMediateAthRoomId.Begin();
   ListSeniorAthRoomId::TListIterator it3 = m_ListSeniorAthRoomId.Begin();



	 switch( eRoomLevel )
	{
	case E_PvP_Junior:
		
		while( m_ListJuniorAthRoomId.PeekNext(it1, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Junior && pRoomId->nRoomID == dwRoomId) 
				{
					return pRoomId ;
				}
			}
		}

		break;
	case E_PvP_Middle:
		
		while( m_ListInterMediateAthRoomId.PeekNext(it2, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Middle && pRoomId->nRoomID == dwRoomId) 
				{
					return pRoomId ;
				}
			}
		}

		break;
	case E_PvP_Senior:

		
		while( m_ListSeniorAthRoomId.PeekNext(it3, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Senior && pRoomId->nRoomID == dwRoomId) 
				{
					return pRoomId ;
				}
			}
		}
		break;
	}
	pRoomId = NULL; 
    return pRoomId;
}

RoomBase*    AthleticsRoomMgr::GetPvPRoom(DWORD dwSerialID)
{
    return m_MapAthleticsRoom.Peek(dwSerialID);
}

RoomBase*   AthleticsRoomMgr::GetPvPRoom(DWORD dwRoleID ,E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId)
{
	  tagPvPRoleInRoom* pRoleInRoom = GetInRoomRole(dwRoleID);
      if( P_VALID(pRoleInRoom) )
	  {
          return GetPvPRoom(pRoleInRoom->dwSerialID);
	  }
	  else
		  return NULL;

}

//把大厅的房间信息发给客户端
//---------------------------------------------------------------------------------------
VOID AthleticsRoomMgr::SendHallRoomInfoToClient()
{

}

//---------------------------------------------------------------------------------------
//把房间的具体信息发给客户端;
//---------------------------------------------------------------------------------------
VOID AthleticsRoomMgr::SendRoomInfoToClient()
{

}

/*tagPvPRoom* AthleticsRoomMgr::GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel)
{
   
     tagPvPRoom* 
	 switch( eRoomLevel )
	 {
	 case E_PvP_Junior:
		  PvPRoom = m_MapJuniorAthleticsRoom.Peek(nRoomID);
		 break;
	 case E_PvP_Middle:

		 break;
	 case E_PvP_Senior:
		 break;

	 }
	   

}*/

DWORD    AthleticsRoomMgr::RoleJoinInRoom(DWORD dwRoleID,E_PvP_RoomLevel eRoomLevel,INT16 nRoomID)
{
	
	tagPvPRoom*  pRoomlist = GetHallRoomInfo(nRoomID,eRoomLevel);
	if( !P_VALID(pRoomlist) )
	{
        return E_PvP_NoRoom;
	}

	DWORD dwSerialId = pRoomlist->dwSerialID;
	tagPvPRoleInRoom* pRoleInRoom = NULL;

	pRoleInRoom = m_MapRoleInAthletisc.Peek(dwRoleID);
	if( !P_VALID(pRoleInRoom) )
	{ 
		return E_PvP_NotInHall;
	}
	if ( 0 != pRoleInRoom->dwSerialID)
	{
         return E_PvP_AlreadyInRoom;
	}
	

	RoomBase*   pRoom =   GetPvPRoom(dwSerialId);
	if( !P_VALID(pRoom))
	{
		return E_PvP_NoRoom;
	}
	if ( pRoom->GetRoomState() != E_PvP_Waiting)
	{
		return E_PvP_RoomIsFull;
	}

	if( pRoomlist->eRoomType == E_PvP_Room3v3 )
	{
		Athletics3V3Room * p = (Athletics3V3Room*)pRoom;	
		if( !p->AddRole( dwRoleID ))
		{
			return E_PvP_RoomIsFull;
		}
		else
		{
		   pRoleInRoom->dwSerialID =  dwSerialId ;
           pRoomlist->dwManagerID        = p->GetMasterRoleID();
		   pRoomlist->eRoomState         = p->GetRoomState();
		   pRoomlist->dwSerialID         = p->GetRoomSerialID();
		   _tcscpy_s(pRoomlist->szManagerName,X_SHORT_NAME, p->GetMasterName());
		   pRoomlist->ucCurrentPersonNum = p->GetRoomPersonNumber();
		}
	}  
	else
	{
		 ASSERT(0);
		 return E_PvP_NoRoom;
	}
	return E_PvP_Success;
}

DWORD   AthleticsRoomMgr::KickRoleToHall(DWORD dwRoleID,BOOL bSendMsg)
{
	tagPvPRoleInRoom* pRoleInRoom = GetInRoomRole(dwRoleID);
	if( P_VALID( pRoleInRoom ) )
	{
		RoomBase*   pRoom =   GetPvPRoom(pRoleInRoom->dwSerialID);
		if ( P_VALID( pRoom ) )
		{

			if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			{
				Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				if( !P_VALID(p) )
				{
					return E_PvP_KickPlayerFail;
				}
				else if( !p->KickRole(dwRoleID,bSendMsg) )
				{
					return E_PvP_KickPlayerFail;
				}
				else 
				{
					pRoleInRoom->dwSerialID = 0;
					tagPvPRoom*  pRoomlist = GetHallRoomInfo((INT16)p->GetRoomID(),p->GetRoomLevel());
					if ( P_VALID( pRoomlist ) )
					{
						pRoomlist->dwManagerID        = p->GetMasterRoleID();
						pRoomlist->eRoomState         = p->GetRoomState();
						pRoomlist->dwSerialID         = p->GetRoomSerialID();
						_tcscpy_s(pRoomlist->szManagerName,X_SHORT_NAME, p->GetMasterName());
						pRoomlist->ucCurrentPersonNum = p->GetRoomPersonNumber();
					}
				}
				if (p->GetRoomState() == E_PvP_RoomStateNull)
				{

					SetRoomIdState(p->GetRoomLevel(),p->GetRoomID(),ERIDS_Vacant);
					if( !DeleteListRoom(p->GetRoomType(),p->GetRoomLevel(),(INT16)p->GetRoomID()))
					{
						return E_PvP_KickPlayerFail;
					}

				}
				return E_PvP_Success;
			}
			else
			{
				ASSERT(0);
				return E_PvP_KickPlayerFail;
			}

		}
	}
	return E_PvP_KickPlayerFail;

}

DWORD   AthleticsRoomMgr::RoleLeaveRoom(DWORD dwRoleID)
{
     tagPvPRoleInRoom* pRoleInRoom = GetInRoomRole(dwRoleID);
	 if( P_VALID( pRoleInRoom ) )
	 {
         RoomBase*   pRoom =   GetPvPRoom(pRoleInRoom->dwSerialID);
		 if ( P_VALID( pRoom ) )
		 {

			 if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			 {
				 Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				 if( !P_VALID(p) )
				 {
					 return E_PvP_LeaveRoomFail;
				 }
				 if( p->GetRoomState() == E_PvP_Fighting)
				 {
					 return E_PvP_RoomInFighting;
				 } 
                 if( !p->LeaveRole(dwRoleID) )
				 {
					 return E_PvP_RoleInReadyState;
				 }
				 else 
				 {
					 pRoleInRoom->dwSerialID = 0;
                     tagPvPRoom*  pRoomlist = GetHallRoomInfo((INT16)p->GetRoomID(),p->GetRoomLevel());
					 if ( P_VALID( pRoomlist ) )
					 {
						 pRoomlist->dwManagerID        = p->GetMasterRoleID();
						 pRoomlist->eRoomState         = p->GetRoomState();
						 pRoomlist->dwSerialID         = p->GetRoomSerialID();
						 _tcscpy_s(pRoomlist->szManagerName,X_SHORT_NAME, p->GetMasterName());
						 pRoomlist->ucCurrentPersonNum = p->GetRoomPersonNumber();
					 }
				 }
				 if (p->GetRoomState() == E_PvP_RoomStateNull)
				 {
					  SetRoomIdState(p->GetRoomLevel(),p->GetRoomID(),ERIDS_Vacant);
                      if( !DeleteListRoom(p->GetRoomType(),p->GetRoomLevel(),(INT16)p->GetRoomID()))
					  {
						  return E_PvP_LeaveRoomFail;
					  }
					 
				 }

				 LeaveHall(dwRoleID);
				 return E_PvP_Success;
			 }
			 else
			 {
				 ASSERT(0);
				  LeaveHall(dwRoleID);
				 return  E_PvP_LeaveRoomFail;
			 }

		 }
		 else
		 {
			 LeaveHall(dwRoleID);
			 return E_PvP_Success;
		 }
	 }
	 return E_PvP_LeaveRoomFail;
}

VOID  AthleticsRoomMgr::AddRoleToAthletics(const tagPvPRoleInRoom* pRoleInRoom )
{
	if( !P_VALID( pRoleInRoom ))
	{
		return;
	}
    tagPvPRoleInRoom* pRoleInRoomTemp = const_cast<tagPvPRoleInRoom*>(pRoleInRoom);
    m_MapRoleInAthletisc.Add(pRoleInRoom->dwRoleID,pRoleInRoomTemp);
}

tagPvPRoleInRoom* AthleticsRoomMgr::GetInRoomRole(DWORD dwRoleID)
{
    return  m_MapRoleInAthletisc.Peek(dwRoleID);
}



BOOL  AthleticsRoomMgr::DeleteRoleFromAthletics(DWORD dwRoleID)
{
    tagPvPRoleInRoom* pRoleInRoom = NULL;

	pRoleInRoom = m_MapRoleInAthletisc.Peek(dwRoleID);
	if ( P_VALID(pRoleInRoom) )
	{
		SAFE_DEL( pRoleInRoom );
		if(m_MapRoleInAthletisc.Erase(dwRoleID))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
		return FALSE;
}

VOID  AthleticsRoomMgr::GetHallRoomId(INT16 nRoomNum, E_PvP_RoomLevel eRoomLevel,INT16 nRoomID[])
{
	

    if( nRoomNum <= 0)
	{
		return;
	}
    
	tagAthRoomId* pRoomId  = NULL ;
	INT nCount = 0;
	ListJuniorAthRoomId::TListIterator it1 = m_ListJuniorAthRoomId.Begin();
	ListInterMediateAthRoomId::TListIterator it2 = m_ListInterMediateAthRoomId.Begin();
	ListSeniorAthRoomId::TListIterator it3 = m_ListSeniorAthRoomId.Begin();

	switch( eRoomLevel )
	{
	case E_PvP_Junior:

		while( m_ListJuniorAthRoomId.PeekNext(it1, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Junior && pRoomId->eRoomIDState == ERIDS_Created) 
				{
					if(nCount >= nRoomNum )
					{
						return;
					}
					nRoomID[nCount] = pRoomId->nRoomID ;
                    nCount++;
				}
			}
		}
		break;
	case E_PvP_Middle:
		while( m_ListInterMediateAthRoomId.PeekNext(it2, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Middle && pRoomId->eRoomIDState == ERIDS_Created) 
				{
					if(nCount >= nRoomNum )
					{
						return;
					}
					nRoomID[nCount] = pRoomId->nRoomID ;
					nCount++;
				}
			}
		}

		break;
	case E_PvP_Senior:
		while( m_ListSeniorAthRoomId.PeekNext(it3, pRoomId) )
		{
			if ( P_VALID( pRoomId )  )
			{
				if( pRoomId->eRoomLevel == E_PvP_Senior && pRoomId->eRoomIDState == ERIDS_Created) 
				{
					if(nCount >= nRoomNum )
					{
						return;
					}
					nRoomID[nCount] = pRoomId->nRoomID ;
					nCount++;
				}
			}
		}

		break;
	}
}

TMap<INT16, tagPvPRoom*>&   AthleticsRoomMgr::GetMapPvPRoomInfo(E_PvP_RoomLevel eRoomLevel)
{
	 switch(eRoomLevel)
	 {
	 case  E_PvP_Junior:
		 return  m_MapJuniorAthleticsRoom;
	 case E_PvP_Middle:
		 return  m_MapInterMediateAthleticsRoom;
	 case E_PvP_Senior:
		 return m_MapSeniorAthleticsRoom;
	 
	 }
	 return m_MapSeniorAthleticsRoom;
}

VOID AthleticsRoomMgr::GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*输出*/PBYTE& pData)
{
	tagPvPRoom* pRoomInfo = NULL;
	if( eRoomLevel == E_PvP_Junior)
	{
		pRoomInfo                       = m_MapJuniorAthleticsRoom.Peek( nRoomID );		
	}
	else if ( eRoomLevel == E_PvP_Middle)
	{
		pRoomInfo                       = m_MapInterMediateAthleticsRoom.Peek( nRoomID );
		
	}
	else if( eRoomLevel == E_PvP_Senior)
	{
		pRoomInfo                       = m_MapSeniorAthleticsRoom.Peek( nRoomID );
		
	}
	
	tagPvPRoom& pvpRoom = *(tagPvPRoom*)pData;
	pvpRoom.eLevel                 = eRoomLevel;
	pvpRoom.nRoomID				   = nRoomID;
	if(  !P_VALID( pRoomInfo ))
	{
		pvpRoom.ucCurrentPersonNum = 0;
		pData += sizeof(tagPvPRoom) -sizeof(tagPvPRoleInfo);
		return ;
	}
	
	pvpRoom.dwWuXunLimit           = pRoomInfo->dwWuXunLimit;	
	pvpRoom.eRoomType              = pRoomInfo->eRoomType;
	pvpRoom.iLevelLowerLimit       = pRoomInfo->iLevelLowerLimit;
	pvpRoom.iLevelUpperLimit       = pRoomInfo->iLevelUpperLimit;
	pvpRoom.nRoomID                = pRoomInfo->nRoomID;
	pvpRoom.dwSerialID			   = pRoomInfo->dwSerialID;
	
	pvpRoom.ucCurrentPersonNum     = 0;
	pvpRoom.ucTotalPersonNum       = pRoomInfo->ucTotalPersonNum;
	memset(&pvpRoom.pData,0,sizeof(pvpRoom.pData));
	
	if(pvpRoom.eRoomType == E_PvP_Room3v3)
	{
		Athletics3V3Room* pRoom =  (Athletics3V3Room*)GetPvPRoom(pRoomInfo->dwSerialID);
		if ( !P_VALID(pRoom) )
		{
              return ;
		}
	    pvpRoom.eRoomState      = pRoom->GetRoomState();
		pvpRoom.dwManagerID     = pRoom->GetMasterRoleID();
		memcpy(pvpRoom.szManagerName,pRoom->GetMasterName(), X_SHORT_NAME* sizeof(TCHAR));
		for(int i=0; i< P3v3ROOM_TOTAL_PERSON_NUM; i++)
		{
			tagPvPRoleInfo& roleInfo = pRoom->GetPosRoleInfo(i+1);
			if(roleInfo.eState != E_PvP_RoleStateNull)
			{				
				memcpy(&pvpRoom.pData[pvpRoom.ucCurrentPersonNum], &roleInfo, sizeof(tagPvPRoleInfo));
				pvpRoom.ucCurrentPersonNum++;
			}
		}
	}
	else
	{
		ASSERT(0);
		return ;
	}
	pData += sizeof(tagPvPRoom) + (pvpRoom.ucCurrentPersonNum-1)*sizeof(tagPvPRoleInfo);
	return ;
}

VOID   AthleticsRoomMgr::OnOffLine(DWORD dwRoleID,BOOL bIsSend)
{
      tagPvPRoleInRoom* pRoleInRoom = NULL;
      pRoleInRoom = GetInRoomRole(dwRoleID);   
	 if( P_VALID(pRoleInRoom))
	 {
           if( 0 == pRoleInRoom->dwSerialID )
		 {
             DeleteRoleFromAthletics(dwRoleID); 
			 //--------------------------------------------------------------------------
			 //SM 竞技场的最近修改
			 //屏蔽该部分功能 
			 //--------------------------------------------------------------------------

				// 			 Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
				// 			 if ( P_VALID(pRole))
				// 			 {
				// 				 tagNS_PvPLeaveHall send;
				// 				 send.dwErrorCode = 0;
				// 				 pRole->SendMessage(&send,send.dwSize);
				// 			 }
		 }
		 else
		 {
             KickRoleToHall(dwRoleID,bIsSend);
             DeleteRoleFromAthletics(dwRoleID); 
             
			 Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
			 if ( P_VALID(pRole))
			 {
				 tagNS_PvPLeaveRoom MsgLeaveRoom;
				 MsgLeaveRoom.dwErrorCode = 0;
				 pRole->SendMessage(&MsgLeaveRoom,MsgLeaveRoom.dwSize);

				 //--------------------------------------------------------------------------
				 //SM 竞技场的最近修改
				 //屏蔽该部分功能 
				 //--------------------------------------------------------------------------
					// 				 tagNS_PvPLeaveHall send;
					// 				 send.dwErrorCode = 0;
					// 				 pRole->SendMessage(&send,send.dwSize);
			 }
			 
		 }
	 }


}

tagPvPRoom* AthleticsRoomMgr::GetHallRoomInfo(INT16 nRoomID, E_PvP_RoomLevel eRoomLevel)
{
	if ( eRoomLevel == E_PvP_Junior)
	{
		return  m_MapJuniorAthleticsRoom.Peek( nRoomID );
	}
	else if( eRoomLevel == E_PvP_Middle)
	{
       return m_MapInterMediateAthleticsRoom.Peek( nRoomID ); 
	}
	else if( eRoomLevel == E_PvP_Senior)
	{
       return  m_MapSeniorAthleticsRoom.Peek( nRoomID );
	}
	else
	{
		return NULL;
	}
}

VOID  AthleticsRoomMgr::GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*输出*/tagPvPRoom& pvpRoom)
{
     
	      tagPvPRoom* pRoomInfo = NULL;
	     if( eRoomLevel == E_PvP_Junior)
		 {
			 pRoomInfo                       = m_MapJuniorAthleticsRoom.Peek( nRoomID );
			 if(  !P_VALID( pRoomInfo ))
			 {
				 IMSG(_T("The Athletics room of Creation failed roomID:%d\r\n"), (DWORD)nRoomID);
				 return ;
			 }
			 pvpRoom.dwManagerID            = pRoomInfo->dwManagerID;
			 pvpRoom.dwWuXunLimit           = pRoomInfo->dwWuXunLimit;
			 pvpRoom.eLevel                 = pRoomInfo->eLevel;
			 pvpRoom.eRoomState             = pRoomInfo->eRoomState;
			 pvpRoom.eRoomType              = pRoomInfo->eRoomType;
			 pvpRoom.iLevelLowerLimit       = pRoomInfo->iLevelLowerLimit;
			 pvpRoom.iLevelUpperLimit       = pRoomInfo->iLevelUpperLimit;
			 pvpRoom.nRoomID                = pRoomInfo->nRoomID;
             pvpRoom.dwSerialID             = pRoomInfo->dwSerialID;
			 _tcscpy_s(pvpRoom.szManagerName,X_SHORT_NAME,pRoomInfo->szManagerName);
			 pvpRoom.ucCurrentPersonNum     = pRoomInfo->ucCurrentPersonNum;
			 pvpRoom.ucTotalPersonNum       = pRoomInfo->ucTotalPersonNum;
			 memset(&pvpRoom.pData,0,sizeof(pvpRoom.pData));
			 return ;

		 }
		 else if ( eRoomLevel == E_PvP_Middle)
		 {
			 pRoomInfo                       = m_MapInterMediateAthleticsRoom.Peek( nRoomID );
			 if(  !P_VALID( pRoomInfo ))
			 {
				 IMSG(_T("The Athletics room of Creation failed roomID:%d\r\n"), (DWORD)nRoomID);
				 return ;
			 }
			 pvpRoom.dwManagerID            = pRoomInfo->dwManagerID;
			 pvpRoom.dwWuXunLimit           = pRoomInfo->dwWuXunLimit;
			 pvpRoom.eLevel                 = pRoomInfo->eLevel;
			 pvpRoom.eRoomState             = pRoomInfo->eRoomState;
			 pvpRoom.eRoomType              = pRoomInfo->eRoomType;
			 pvpRoom.iLevelLowerLimit       = pRoomInfo->iLevelLowerLimit;
			 pvpRoom.iLevelUpperLimit       = pRoomInfo->iLevelUpperLimit;
			 pvpRoom.nRoomID                = pRoomInfo->nRoomID;
			 pvpRoom.dwSerialID             = pRoomInfo->dwSerialID;
			 _tcscpy_s(pvpRoom.szManagerName,X_SHORT_NAME,pRoomInfo->szManagerName);
			 pvpRoom.ucCurrentPersonNum     = pRoomInfo->ucCurrentPersonNum;
			 pvpRoom.ucTotalPersonNum       = pRoomInfo->ucTotalPersonNum;
			 memset(&pvpRoom.pData,0,sizeof(pvpRoom.pData));
             return ;
			

		 }
		 else if( eRoomLevel == E_PvP_Senior)
		 {
			 pRoomInfo                       = m_MapSeniorAthleticsRoom.Peek( nRoomID );
			 if(  !P_VALID( pRoomInfo ))
			 {
				 IMSG(_T("The Athletics room of Creation failed roomID:%d\r\n"), (DWORD)nRoomID);
				 return ;
			 }
			 pvpRoom.dwManagerID            = pRoomInfo->dwManagerID;
			 pvpRoom.dwWuXunLimit           = pRoomInfo->dwWuXunLimit;
			 pvpRoom.eLevel                 = pRoomInfo->eLevel;
			 pvpRoom.eRoomState             = pRoomInfo->eRoomState;
			 pvpRoom.eRoomType              = pRoomInfo->eRoomType;
			 pvpRoom.iLevelLowerLimit       = pRoomInfo->iLevelLowerLimit;
			 pvpRoom.iLevelUpperLimit       = pRoomInfo->iLevelUpperLimit;
			 pvpRoom.nRoomID                = pRoomInfo->nRoomID;
			 _tcscpy_s(pvpRoom.szManagerName,X_SHORT_NAME,pRoomInfo->szManagerName);
			 pvpRoom.ucCurrentPersonNum     = pRoomInfo->ucCurrentPersonNum;
			 pvpRoom.ucTotalPersonNum       = pRoomInfo->ucTotalPersonNum;
			 memset(&pvpRoom.pData,0,sizeof(pvpRoom.pData));
			 return ;

			

		 }
	 

}


 VOID AthleticsRoomMgr::AddPvPRoomToPvPQueue(RoomBase* pRoom)
{
	  if( P_VALID(pRoom))
	  {
		  m_ListEnterPvPRoom.PushBack(pRoom);
	  }
}


/*
BOOL  AthleticsRoomMgr::AddRoleToRoom(DWORD dwSerialId,DWORD dwRoleID1,DWORD dwRoleID2,DWORD dwRoleID3,DWORD dwRoleID4,DWORD dwRoleID5,DWORD dwRoleID6)
{
   RoomBase* pRoom = GetPvPRoom(dwSerialId);
   if ( !P_VALID(pRoom ))
   {
       return FALSE;
   }
   INT nRoleNum = 0;
   if( 0 == dwRoleID1 ){ nRoleNum++;}
   if( 0 == dwRoleID2) { nRoleNum++;}
   if( 0 == dwRoleID3) { nRoleNum++;}
   if( 0 == dwRoleID4) { nRoleNum++;}
   if( 0 == dwRoleID5) { nRoleNum++;}
   if( 0 == dwRoleID6) { nRoleNum++;}

   tagPvPRoleInfo*  pTemp = new tagPvPRoleInfo[nRoleNum];
   if ( !P_VALID(pTemp) )
   {
	   return FALSE;
   }
   



}*/
