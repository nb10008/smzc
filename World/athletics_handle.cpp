//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_handle.cpp
// author: Lamoair
// actor:
// data: 2010-02-26
// last:
// brief: 客户端和服务器端间消息处理 -- 竞技相关
//-----------------------------------------------------------------------------

#include "StdAfx.h"


#include "../WorldDefine/RoleDefine.h"
#include "role.h"
#include "role_mgr.h"
#include "map.h"

#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "athletics_room.h"
#include "athletics_room_mgr.h"
#include "athletics_system.h"

#include "player_session.h"

#define MAX_NUM_ROOMINFO	10

//竞技系统相关 

//-----------------------------------------------------------------------------
// 进入竞技场大厅
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandlePvPEnterHall(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPEnterHall);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
     
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0; 

	
    //地图和坐标判断待定
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPEnterHall send;
		send.dwErrorCode = E_PvP_UnOpened;
		pRole->SendMessage(&send,send.dwSize);
		return GT_INVALID;

	}

    EPvPError ePvPError = E_PvP_Success;
	DWORD dwRoleID  = pRole->GetID();
    tagNS_PvPEnterHall send;
	if( pRole->GetLevel() < PVPROOM_ROLE_LOWERLEVEL)
	{
        ePvPError  =  E_PvP_InvalidLevel;
		send.dwErrorCode = ePvPError;
		pRole->SendMessage(&send,send.dwSize);
		return GT_INVALID;
	}

	if( pRole->IsDead())
	{
		ePvPError  =  E_PvP_UnOpened;
		send.dwErrorCode = ePvPError;
		pRole->SendMessage(&send,send.dwSize);
		return GT_INVALID;   
	} 

	E_PvP_RoomType roleType = pRecv->eRoomType;
    
	///看角色是否已经删除了
	tagPvPRoleInRoom* pRoleInRoomTemp = g_AthleticsSystem.GetRoomMgr().GetInRoomRole(dwRoleID);

	if( P_VALID(pRoleInRoomTemp ))
	{
		if( pRoleInRoomTemp->dwRoleID == dwRoleID)
		{


			ePvPError  =  E_PvP_Success;
			send.dwErrorCode = ePvPError;
			pRole->SendMessage(&send,send.dwSize);
			return GT_INVALID;
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

		pRoleInRoom->dwRoleID   = pRole->GetID();
		pRoleInRoom->dwSerialID = 0;
		g_AthleticsSystem.GetRoomMgr().AddRoleToAthletics(pRoleInRoom);

		send.dwErrorCode = ePvPError;
		pRole->SendMessage(&send,send.dwSize);
		break;
	case E_PvP_Room6v6:
        return GT_INVALID;
		break;
	default:
		return GT_INVALID;
		break;
	}	
  g_AthleticsSystem.ModHallPersonNumTemp(1);
	
 
   return (DWORD)ePvPError;
}

DWORD PlayerSession::HandlePvPLeaveHall(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPLeaveHall);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0; 


	//地图和坐标判断待定
	Role* pRole = GetRole();
	if( !P_VALID(pRole))
	{
		return GT_INVALID;
	}
    EPvPError ePvPError = E_PvP_Success;
	g_AthleticsSystem.GetRoomMgr().OnOffLine(pRole->GetID());

    tagNS_PvPLeaveHall send;
	send.dwErrorCode = ePvPError;
	pRole->SendMessage(&send,send.dwSize);

	
   return (DWORD)ePvPError;
}

DWORD PlayerSession::HandlePvPGetRoomList(tagNetCmd* pCmd)
{

   MGET_MSG(pRecv, pCmd, NC_PvPGetRoomList);
   if ( !P_VALID(pRecv) )
   {
	   return GT_INVALID;
   }

   //--------------------------------------------------------------------------
   //SM 竞技场的最近修改
   //屏蔽该部分功能
   //--------------------------------------------------------------------------
   return 0; 


   E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;
   if( eRoomLevel <= E_PvP_RoomLevelNull || eRoomLevel > E_PvP_Senior)
   {
	   return 0;
   }
   INT16 nRoomNum = 0;
   INT32 nMsgSz = 0;
   switch( eRoomLevel )
   {
   case E_PvP_Junior:
	   nRoomNum = g_AthleticsSystem.GetRoomMgr().GetTotalJuniorRoomNumber();
       nMsgSz = sizeof(tagNS_PvPGetRoomList) + (nRoomNum-1) * sizeof(INT16);
	 
       
	   break;
   case E_PvP_Middle:
	   nRoomNum = g_AthleticsSystem.GetRoomMgr().GetTotalInterMediateRoomNumber();
	     nMsgSz = sizeof(tagNS_PvPGetRoomList) + (nRoomNum-1) * sizeof(INT16);

	   break;
   case E_PvP_Senior:
	   nRoomNum = g_AthleticsSystem.GetRoomMgr().GetTotalSeniorRoomNumber();
	   nMsgSz = sizeof(tagNS_PvPGetRoomList) + (nRoomNum-1) * sizeof(INT16);

	   break;
   }
   if( nRoomNum < 0)
   {  
	   return GT_INVALID;
   }
   MCREATE_MSG(pSend, nMsgSz, NS_PvPGetRoomList);
   pSend->eRoomLevel = eRoomLevel;
   pSend->dwCount    = nRoomNum ;
   g_AthleticsSystem.GetRoomMgr().GetHallRoomId(nRoomNum,eRoomLevel,pSend->pData);
   if( P_VALID(m_pRole) )
   {
         m_pRole->SendMessage(pSend,pSend->dwSize);
   }
   MDEL_MSG(pSend);


   return 0;
}

DWORD PlayerSession::HandlePvPGetRoomInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPGetRoomInfo);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0; 


	E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;//房间等级	
    DWORD dwErrorCode = 0;
    Role* pRole  = GetRole();
	if ( !P_VALID(pRole) )
	{
        return GT_INVALID;
	}
    


	if(pRecv->dwCount > MAX_NUM_ROOMINFO || pRecv->dwCount <=0)
	{
		return GT_INVALID;
	}
    
	tagNS_PvPGetRoomInfo MsgRoomInfo;  
	DWORD dwRoomInfoSize = pRecv->dwCount * (sizeof(tagPvPRoom) + sizeof(tagPvPRoleInfo)*(P3v3ROOM_TOTAL_PERSON_NUM-1)); //分配足够大小
	PBYTE pMem = new BYTE[dwRoomInfoSize];
	if ( !P_VALID(pMem) )
	{
		return GT_INVALID; 
	}
	PBYTE pData = pMem;
	for(int i=0; i< pRecv->dwCount; i++)
	{
		g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(pRecv->pData[i], pRecv->eRoomLevel, pData);
	}
	DWORD dwSize = sizeof(tagNS_PvPGetRoomInfo) - sizeof(tagPvPRoom) + (pData-pMem);
	MCREATE_MSG(pSend,dwSize, NS_PvPGetRoomInfo);
	pSend->dwCount    = pRecv->dwCount ;
	pSend->dwErrorCode = E_PvP_Success;
	IFASTCODE->MemCpy(pSend->pData, pMem, pData-pMem);
	delete [] pMem;
	SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}


DWORD PlayerSession::HandlePvPGetSimRoomInfo(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_PvPGetSimRoomInfo);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0; 


	E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;//房间等级	
	DWORD dwRoomNum   = pRecv->dwCount;
	DWORD dwErrorCode = 0;
	Role* pRole  = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}



	if(pRecv->dwCount > MAX_NUM_ROOMINFO || pRecv->dwCount <=0)
	{
		return GT_INVALID;
	}

	tagNS_PvPGetRoomInfo MsgRoomInfo;
	
	DWORD dwRoomInfoSize  =  dwRoomNum*( sizeof(tagPvPSimRoom)) ;
	PBYTE pData = new BYTE[dwRoomInfoSize];
	PBYTE pTemp = pData;

	for( int i = 0 ; i < dwRoomNum ;i++)
	{
		 tagPvPSimRoom&  simRoom =  (*(tagPvPSimRoom*)pData);
         tagPvPRoom*     p =  g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(pRecv->pData[i],pRecv->eRoomLevel);
		 if( P_VALID(p))
		 {
             simRoom.dwManagerID      =  p->dwManagerID;
			 simRoom.dwSerialID       =  p->dwSerialID;
			 simRoom.dwWuXunLimit     =  p->dwWuXunLimit;
			 simRoom.eLevel           =  p->eLevel;
			 simRoom.eRoomState       =  p->eRoomState;
			 simRoom.eRoomType        =  p->eRoomType;
			 simRoom.iLevelLowerLimit =  p->iLevelLowerLimit;
			 simRoom.iLevelUpperLimit =  p->iLevelUpperLimit;
			 simRoom.nRoomID          =  p->nRoomID;
              _tcscpy(simRoom.szManagerName,p->szManagerName); 
		     simRoom.ucCurrentPersonNum = p->ucCurrentPersonNum;
			 simRoom.ucTotalPersonNum   = p->ucTotalPersonNum;
			 pData += sizeof(tagPvPSimRoom);

		 }
		 else
		 {
			 memset(&simRoom, 0, sizeof(tagPvPSimRoom));
			 simRoom.nRoomID = pRecv->pData[i];
			 pData += sizeof(tagPvPSimRoom);
		 }
		 
	}
	DWORD MsgSize  =  sizeof(tagNS_PvPGetSimRoomInfo) - sizeof(tagPvPSimRoom)+ pData-pTemp;

	MCREATE_MSG(pSend,MsgSize, NS_PvPGetSimRoomInfo);
	pSend->dwCount    = pRecv->dwCount ;
	pSend->dwErrorCode = E_PvP_Success;
	IFASTCODE->MemCpy(pSend->pData, pTemp, pData-pTemp);
	delete [] pTemp;
	SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	

   return 0;
}

DWORD PlayerSession::HandlePvPInvite(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPInvite);

	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
    return 0; 


	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
    
    tagPvPRoleInRoom* pRoleInRoom =  g_AthleticsSystem.GetRoomMgr().GetInRoomRole(pRole->GetID());
	if ( !P_VALID(pRoleInRoom) )
	{
		return GT_INVALID ;
	}

    RoomBase* pRoom  = g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
	if ( !P_VALID(pRoom) )
	{
		return GT_INVALID;
	}
    
	Athletics3V3Room* p3V3Room = NULL;
	if ( pRoom->GetRoomType() == E_PvP_Room3v3)
	{ 
         p3V3Room = (Athletics3V3Room*)pRoom;
	}
	else
	{
		return GT_INVALID;
	}
    
	if ( p3V3Room->GetRoomState() != E_PvP_Waiting || p3V3Room->GetRoomPersonNumber() == 6 
		|| p3V3Room->GetRoomType()!= pRecv->eRoomType || p3V3Room->GetRoomLevel()!=pRecv->eRoomLevel || p3V3Room->GetRoomID() != pRecv->nRoomID)
	{
		tagNS_PvPInvite msg;
		msg.dwErrorCode = E_PvP_InviteFull ;
		msg.eRoomType = pRecv->eRoomType;
		msg.eRoomLevel = pRecv->eRoomLevel;
		msg.dwFriendID = pRecv->dwFriendID;
		msg.nRoomID = pRecv->nRoomID;
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
    
	Role* pFriendRole = g_roleMgr.GetRolePtrByID(pRecv->dwFriendID);
	if ( !P_VALID(pFriendRole) )
	{   
		tagNS_PvPInvite msg;
		msg.dwErrorCode = E_PvP_InviteNotOnLine ;
		msg.eRoomType = pRecv->eRoomType;
		msg.eRoomLevel = pRecv->eRoomLevel;
		msg.dwFriendID = pRecv->dwFriendID;
		msg.nRoomID = pRecv->nRoomID;
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
           
	}
	else
	{    
		 tagPvPRoleInRoom* pRoleInRoom =  g_AthleticsSystem.GetRoomMgr().GetInRoomRole( pRecv->dwFriendID);
		 if ( P_VALID(pRoleInRoom) )
		 {
			 if ( pRoleInRoom->dwSerialID  != 0 )
			 { 
				 tagNS_PvPInvite msg;
				 msg.dwErrorCode = E_PvP_InviteRefused ;
				 msg.eRoomType = pRecv->eRoomType;
				 msg.eRoomLevel = pRecv->eRoomLevel;
				 msg.dwFriendID = pRecv->dwFriendID;
				 msg.nRoomID = pRecv->nRoomID;
				 pRole->SendMessage(&msg,msg.dwSize);
				 return GT_INVALID;
			 }
		 } 
		
		 BOOL bIsInBlackList = FALSE;
		 tagFriend* pI = pFriendRole->GetFriendPtr(pRole->GetID());
		 if (!P_VALID( pI) )
		 {
			 tagNS_PvPInvite msg;
			 msg.dwErrorCode = E_PvP_InviteNotFriend ;
			 msg.eRoomType = pRecv->eRoomType;
			 msg.eRoomLevel = pRecv->eRoomLevel;
			 msg.dwFriendID = pRecv->dwFriendID;
			 msg.nRoomID = pRecv->nRoomID;
			 pRole->SendMessage(&msg,msg.dwSize);
			 return GT_INVALID;
		 }
		 else
		 {
			 for ( INT i = 0 ; i < MAX_BLACKLIST ; i++)
			 {
				  DWORD dwId = pFriendRole->GetBlackList(i);
				  if ( dwId == pRole->GetID() )
				  {
					  bIsInBlackList = TRUE;
					  break;
				  }
			 }
			 if ( bIsInBlackList )
			 {
				 tagNS_PvPInvite msg;
				 msg.dwErrorCode = E_PvP_InviteRefused ;
				 msg.eRoomType = pRecv->eRoomType;
				 msg.eRoomLevel = pRecv->eRoomLevel;
				 msg.dwFriendID = pRecv->dwFriendID;
				 msg.nRoomID = pRecv->nRoomID;
				 pRole->SendMessage(&msg,msg.dwSize);
				 return GT_INVALID;
			 }
			 else
			 {

				 if ( pFriendRole->IsDead())
				 {
					 tagNS_PvPInvite msg;
					 msg.dwErrorCode = E_PvP_InviteRefused ;
					 msg.eRoomType = pRecv->eRoomType;
					 msg.eRoomLevel = pRecv->eRoomLevel;
					 msg.dwFriendID = pRecv->dwFriendID;
					 msg.nRoomID = pRecv->nRoomID;
					 pRole->SendMessage(&msg,msg.dwSize);
					 return GT_INVALID;
				 }
				 DWORD dwRoleLevel = pFriendRole->GetLevel();
				 DWORD dwValiantMerit = pFriendRole->GetAttValue(ERA_WuXun); 
				 DWORD dwValiantMeritLevel = pFriendRole->GetValiantMeritLevel(dwValiantMerit);
                 if ( dwRoleLevel < p3V3Room->GetRoomLowerRoleLevel() || dwRoleLevel > p3V3Room->GetRoomUpperRoleLevel() )
                 {
					 tagNS_PvPInvite msg;
					 msg.dwErrorCode = E_PvP_FrindLevelLimitError ;
					 msg.eRoomType = pRecv->eRoomType;
					 msg.eRoomLevel = pRecv->eRoomLevel;
					 msg.dwFriendID = pRecv->dwFriendID;
					 msg.nRoomID = pRecv->nRoomID;
					 pRole->SendMessage(&msg,msg.dwSize);
					 return GT_INVALID;
                 }
				 if ( dwValiantMeritLevel < p3V3Room->GetRoomValiantMeritLevel() )
				 {
					 tagNS_PvPInvite msg;
					 msg.dwErrorCode = E_PvP_FrindLevelLimitError ;
					 msg.eRoomType = pRecv->eRoomType;
					 msg.eRoomLevel = pRecv->eRoomLevel;
					 msg.dwFriendID = pRecv->dwFriendID;
					 msg.nRoomID = pRecv->nRoomID;
					 pRole->SendMessage(&msg,msg.dwSize);
					 return GT_INVALID;
				 }

                 tagNS_PvPInviteJoin SendMsg;
				 SendMsg.dwInvitorID = pRole->GetID();
				 SendMsg.eRoomLevel  = p3V3Room->GetRoomLevel() ;
				 SendMsg.eRoomType   = p3V3Room->GetRoomType() ;
				 SendMsg.nRoomID     = p3V3Room->GetRoomID();
				 pFriendRole->SendMessage(&SendMsg,SendMsg.dwSize);
				 return 0 ;
			 }
			 

		 }

	}
    


    return 0;
}

//------------------------------------------------------------
//name:  HandlePvPAutoJoinRoom
//desc:  玩家自动加入竞技场
//param: 
//return: NULL
//date:  2010-2-26
//last:  2010-10-25 
//------------------------------------------------------------

DWORD PlayerSession::HandlePvPAutoJoinRoom(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPAutoJoinRoom);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}

	//先加入竞技场大厅 由之前修改来，以免更大的修改

	DWORD dwErrorCode =  g_AthleticsSystem.GetRoomMgr().AddToHall(E_PvP_Room3v3,pRole->GetID());
	if ( dwErrorCode != E_Success)
	{
		return 0;
	}

	tagNS_PvPJoinRoom Msg;

	DWORD dwRoleLevel = pRole->GetLevel();
	DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
	DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);

	// 	if ( dwRoleLevel < 30)
	// 	{
	// 		Msg.dwErrorCode = E_PvP_InvalidLevel;
	// 		Msg.eRoomLevel  = E_PvP_Junior;
	// 		Msg.nRoomID     = 0;
	// 		pRole->SendMessage(&Msg,Msg.dwSize);
	// 		return 0;
	// 	}

	TMap<INT16, tagPvPRoom*>& pvpMapRoomInfo = g_AthleticsSystem.GetRoomMgr().GetMapPvPRoomInfo(E_PvP_Junior);
	TMap<INT16, tagPvPRoom*>::TMapIterator it = pvpMapRoomInfo.Begin();
	tagPvPRoom* pRoomInfo = NULL;
	BOOL  bFindOut   = FALSE ;
	INT  nRoomID    =  0;
	while ( pvpMapRoomInfo.PeekNext(it,pRoomInfo))
	{
		if ( P_VALID(pRoomInfo) )
		{
			if ( pRoomInfo->eRoomState != E_PvP_Waiting || pRoomInfo->ucCurrentPersonNum == 6)
			{
				continue;
			}
			if ( dwValiantMeritLevel >= pRoomInfo->dwWuXunLimit  && dwRoleLevel  >= pRoomInfo->iLevelLowerLimit && dwRoleLevel <= pRoomInfo->iLevelUpperLimit)
			{
				bFindOut  = TRUE ;
				nRoomID = pRoomInfo->nRoomID ;
				break;
			}
		}
	}

	if ( !bFindOut)   // 没有找到就自动创建一个适合玩家的房间
	{

		TCHAR szRoleName[X_SHORT_NAME];
		DWORD dwRoleId = pRole->GetID();
		g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);

		INT  nLevelUpperLimit =  dwRoleLevel + (10 - ((dwRoleLevel%10)== 0 ? 10 : (dwRoleLevel%10) ));
		INT  nLevelLowerLimit =  dwRoleLevel - ((dwRoleLevel%10)== 0 ? 10  : (dwRoleLevel%10) );
		if ( 30 == dwRoleLevel)
		{
			nLevelLowerLimit = 30 ;
			nLevelUpperLimit = 40 ;
		}

		tagPvPRoom PvPRoom;
		PvPRoom.nRoomID            = (INT16)g_AthleticsSystem.GetRoomMgr().GetAvailRoomId(E_PvP_Junior);
		PvPRoom.dwManagerID        = dwRoleId;
		PvPRoom.dwWuXunLimit       = 1;
		PvPRoom.eLevel             = E_PvP_Junior;
		PvPRoom.eRoomState         = E_PvP_Waiting;
		PvPRoom.eRoomType          = E_PvP_Room3v3;
		PvPRoom.iLevelLowerLimit   = nLevelLowerLimit;
		PvPRoom.iLevelUpperLimit   = nLevelUpperLimit;
		_tcscpy_s(PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
		PvPRoom.ucCurrentPersonNum = 1;
		PvPRoom.ucTotalPersonNum   = 6;


		if ( pRole->IsDead())
		{
			dwErrorCode = E_PvP_CreateRoomFail;
			Msg.dwErrorCode = dwErrorCode;
			Msg.eRoomLevel  = E_PvP_RoomLevelNull;
			Msg.nRoomID     = 0;
			pRole->SendMessage(&Msg,Msg.dwSize);
			return 0;	
		}

		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
// 		if ( P_VALID( pRoleScript ) )
// 		{
// 			BOOL bCanCreate = FALSE;
// 			pRoleScript->OnRoleCreateArenaRoom(pRole,E_PvP_Junior,PvPRoom.nRoomID,bCanCreate);
// 			if ( !bCanCreate)
// 			{    
// 				dwErrorCode = E_PvP_CreateRoomFail;
// 				Msg.dwErrorCode = dwErrorCode;
// 				Msg.eRoomLevel  = E_PvP_RoomLevelNull;
// 				Msg.nRoomID     = 0;
// 				pRole->SendMessage(&Msg,Msg.dwSize);
// 				return 0;		
// 
// 			}
// 		}
// 		else
// 		{
// 			return 0;
// 		}


		dwErrorCode = g_AthleticsSystem.GetRoomMgr().CreateAthleticsRoom(PvPRoom);

		if (  dwErrorCode == E_PvP_Success)
		{
			g_AthleticsSystem.ModHallRoomNumTemp(1);
		}

		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = PvPRoom.eLevel;
		Msg.nRoomID     = PvPRoom.nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize);
		return 0;   
	}
	else
	{

		if ( pRole->IsDead())
		{
			Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
			Msg.eRoomLevel  = E_PvP_Junior;
			Msg.nRoomID     = nRoomID;
			pRole->SendMessage(&Msg,Msg.dwSize); 
			return 0;

		}

// 		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
// 		if ( P_VALID( pRoleScript ) )
// 		{
// 			BOOL bCanJoin = FALSE;
// 			pRoleScript->OnRoleJoinToArenaRoom(pRole,E_PvP_Junior,nRoomID,bCanJoin);
// 			if ( !bCanJoin)
// 			{    
// 				return 0;
// 			}
// 		}
// 		else
// 		{
// 			return 0;
// 		}

		dwErrorCode = g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),E_PvP_Junior,nRoomID); 


		Msg.dwErrorCode = dwErrorCode;
		Msg.eRoomLevel  = E_PvP_Junior;
		Msg.nRoomID     = nRoomID;
		pRole->SendMessage(&Msg,Msg.dwSize); 
	}

// 	DWORD dwErrorCode = E_PvP_Success;
// 	DWORD dwRoleLevel = pRole->GetLevel();
// 	DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
// 	DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);
// 
// 	if ( dwRoleLevel < 30)
// 	{
// 		tagNS_PvPJoinRoom Msg;
// 		Msg.dwErrorCode = E_PvP_Unknown;
// 		Msg.eRoomLevel  = pRecv->eRoomLevel;
// 		Msg.nRoomID     = 0;
// 		pRole->SendMessage(&Msg,Msg.dwSize); 
// 		return GT_INVALID;
// 	}
// 
//     TMap<INT16, tagPvPRoom*>& pvpMapRoomInfo = g_AthleticsSystem.GetRoomMgr().GetMapPvPRoomInfo(pRecv->eRoomLevel);
// 	TMap<INT16, tagPvPRoom*>::TMapIterator it = pvpMapRoomInfo.Begin();
//     tagPvPRoom* pRoomInfo = NULL;
// 	BOOL  bFindOut   = FALSE ;
// 	INT  nRoomID    =  0;
// 	while ( pvpMapRoomInfo.PeekNext(it,pRoomInfo))
// 	{
// 		if ( P_VALID(pRoomInfo) )
// 		{
// 			 if ( pRoomInfo->eRoomState != E_PvP_Waiting || pRoomInfo->ucCurrentPersonNum == 6)
// 			 {
// 				 continue;
// 			 }
//              if ( dwValiantMeritLevel >= pRoomInfo->dwWuXunLimit  && dwRoleLevel  >= pRoomInfo->iLevelLowerLimit && dwRoleLevel <= pRoomInfo->iLevelUpperLimit)
//              {
//                   bFindOut  = TRUE ;
// 				  nRoomID = pRoomInfo->nRoomID ;
// 				  break;
//              }
// 		}
// 	}
// 
// 	if ( !bFindOut)   // 没有找到就自动创建一个适合玩家的房间
// 	{
// 		
// 		TCHAR szRoleName[X_SHORT_NAME];
// 		DWORD dwRoleId = pRole->GetID();
// 		g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
// 		
// 		INT  nLevelUpperLimit =  dwRoleLevel + (10 - ((dwRoleLevel%10)== 0 ? 10 : (dwRoleLevel%10) ));
// 		INT  nLevelLowerLimit =  dwRoleLevel - ((dwRoleLevel%10)== 0 ? 10  : (dwRoleLevel%10) );
// 		if ( 30 == dwRoleLevel)
// 		{
// 			nLevelLowerLimit = 30 ;
// 			nLevelUpperLimit = 40 ;
// 		}
// 
// 		tagPvPRoom PvPRoom;
// 		PvPRoom.nRoomID            = (INT16)g_AthleticsSystem.GetRoomMgr().GetAvailRoomId(pRecv->eRoomLevel);
// 		PvPRoom.dwManagerID        = dwRoleId;
// 		PvPRoom.dwWuXunLimit       = 1;
// 		PvPRoom.eLevel             = pRecv->eRoomLevel;
// 		PvPRoom.eRoomState         = E_PvP_Waiting;
// 		PvPRoom.eRoomType          = E_PvP_Room3v3;
// 		PvPRoom.iLevelLowerLimit   = nLevelLowerLimit;
// 		PvPRoom.iLevelUpperLimit   = nLevelUpperLimit;
// 		_tcscpy(PvPRoom.szManagerName, szRoleName);
// 		PvPRoom.ucCurrentPersonNum = 1;
// 		PvPRoom.ucTotalPersonNum   = 6;
//         tagNS_PvPCreateRoom MsgCreateRoom;
// 
// 		if ( pRole->IsDead())
// 		{
// 			dwErrorCode = E_PvP_CreateRoomFail;
// 			MsgCreateRoom.dwErrorCode = dwErrorCode;
// 			MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
// 			MsgCreateRoom.nRoomID     = 0;
// 			pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
// 			return dwErrorCode;	
// 		}
// 
// 		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
// 		if ( P_VALID( pRoleScript ) )
// 		{
// 			BOOL bCanCreate = FALSE;
// 			pRoleScript->OnRoleCreateArenaRoom(pRole,pRecv->eRoomLevel,PvPRoom.nRoomID,bCanCreate);
// 			if ( !bCanCreate)
// 			{    
// 				dwErrorCode = E_PvP_CreateRoomFail;
// 				MsgCreateRoom.dwErrorCode = dwErrorCode;
// 				MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
// 				MsgCreateRoom.nRoomID     = 0;
// 				pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
// 				return dwErrorCode;		
// 
// 			}
// 		}
// 		else
// 		{
// 			return GT_INVALID;
// 		}
// 
// 
// 		dwErrorCode = g_AthleticsSystem.GetRoomMgr().CreateAthleticsRoom(PvPRoom);
// 
// 		if (  dwErrorCode == E_PvP_Success)
// 		{
// 			g_AthleticsSystem.ModHallRoomNumTemp(1);
// 		}
// 
// 		MsgCreateRoom.dwErrorCode = dwErrorCode;
// 		MsgCreateRoom.eRoomLevel  = PvPRoom.eLevel;
// 		MsgCreateRoom.nRoomID     = PvPRoom.nRoomID;
// 		pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
// 		return dwErrorCode;   
// 	}
// 	else
// 	{
// 
//         if ( pRole->IsDead())
//         {
// 			tagNS_PvPJoinRoom Msg;
// 			Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
// 			Msg.eRoomLevel  = pRecv->eRoomLevel;
// 			Msg.nRoomID     = nRoomID;
// 			pRole->SendMessage(&Msg,Msg.dwSize); 
// 			return GT_INVALID;
// 
//         }
// 
// 		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
// 		if ( P_VALID( pRoleScript ) )
// 		{
// 			BOOL bCanJoin = FALSE;
// 			pRoleScript->OnRoleJoinToArenaRoom(pRole,pRecv->eRoomLevel,nRoomID,bCanJoin);
// 			if ( !bCanJoin)
// 			{    
// 				tagNS_PvPJoinRoom Msg;
// 				Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
// 				Msg.eRoomLevel  = pRecv->eRoomLevel;
// 				Msg.nRoomID     = nRoomID;
// 				pRole->SendMessage(&Msg,Msg.dwSize); 
// 				return GT_INVALID;
// 			}
// 		}
// 		else
// 		{
// 			return GT_INVALID;
// 		}
// 
//         dwErrorCode = g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),pRecv->eRoomLevel,nRoomID); 
//         
// 		tagNS_PvPJoinRoom Msg;
// 		Msg.dwErrorCode = dwErrorCode;
// 		Msg.eRoomLevel  = pRecv->eRoomLevel;
// 		Msg.nRoomID     = nRoomID;
//     	pRole->SendMessage(&Msg,Msg.dwSize); 
//            
// 
// 	}
    return 0;
}

DWORD PlayerSession::HandlePvPInviteJoin(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPInviteJoin);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}


	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
    return 0;

	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_UnOpened ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;
	}

    Role* pInviteRole = g_roleMgr.GetRolePtrByID( pRecv->dwInvitorID);
    if ( !P_VALID(pInviteRole) )
    {
		return GT_INVALID;
    }
    if ( pInviteRole->IsDead() || pRole->IsDead())
    {
		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_CanNotJoinRoom ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;
    }

	if ( !pRecv->bIsAccpeted )
	{
		tagNS_PvPInvite msg;
		msg.dwErrorCode = E_PvP_InviteRefused;
		msg.eRoomType = pRecv->eRoomType;
		msg.eRoomLevel = pRecv->eRoomLevel;
		msg.dwFriendID = pRole->GetID();
		msg.nRoomID = pRecv->nRoomID;
		pInviteRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
   
	tagPvPRoleInRoom* pRoleInRoom =  g_AthleticsSystem.GetRoomMgr().GetInRoomRole( pRole->GetID());
	if ( P_VALID(pRoleInRoom) )
	{
		if ( pRoleInRoom->dwSerialID  != 0 )
		{ 
			tagNS_PvPInvite msg;
			msg.dwErrorCode = E_PvP_InviteRefused ;
			msg.eRoomType = pRecv->eRoomType;
			msg.eRoomLevel = pRecv->eRoomLevel;
			msg.dwFriendID = pRole->GetID();
			msg.nRoomID = pRecv->nRoomID;
            pInviteRole->SendMessage(&msg,msg.dwSize); 

			tagNS_PvPJoinRoom MsgjoinRoom;
			MsgjoinRoom.dwErrorCode = E_PvP_AlreadyInRoom ;
			MsgjoinRoom.eRoomLevel  = pRecv->eRoomLevel;
			MsgjoinRoom.nRoomID     = pRecv->nRoomID;

			pRole->SendMessage(&MsgjoinRoom,MsgjoinRoom.dwSize); 
			return GT_INVALID;
		}
	} 
	else
	{
		tagPvPRoleInRoom* pRoleInRoom = new tagPvPRoleInRoom;
		if( !P_VALID(pRoleInRoom) )
		{ 
			IMSG(_T("The Athletics room of Creation failed cityid:%d\r\n"), (DWORD)E_PvP_Room3v3);
			return GT_INVALID;       
		}
		switch(pRecv->eRoomType)
		{
		case E_PvP_Room3v3: 

			pRoleInRoom->dwRoleID   = pRole->GetID();
			pRoleInRoom->dwSerialID = 0;
			g_AthleticsSystem.GetRoomMgr().AddRoleToAthletics(pRoleInRoom);

		
			break;
		case E_PvP_Room6v6:
            delete  pRoleInRoom ;
			pRoleInRoom = NULL;
			return GT_INVALID;
			break;
		default:
			delete pRoleInRoom;
			pRoleInRoom = NULL;
			return GT_INVALID;
			break;
		}	
		g_AthleticsSystem.ModHallPersonNumTemp(1); 
	}

	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if ( P_VALID( pRoleScript ) )
	{
		BOOL bCanJoin = FALSE;
		pRoleScript->OnRoleAgreeToBeInvited(pRole,pRecv->eRoomLevel,pRecv->nRoomID,pRole->GetID(),bCanJoin);
		if ( !bCanJoin)
		{    

			tagNS_PvPInvite Msg;
			Msg.dwErrorCode = E_PvP_InviteCannotJoin ;
			Msg.eRoomType = pRecv->eRoomType;
			Msg.eRoomLevel = pRecv->eRoomLevel;
			Msg.dwFriendID = pRole->GetID();
			Msg.nRoomID = pRecv->nRoomID;
			pInviteRole->SendMessage(&Msg,Msg.dwSize); 

			tagNS_PvPJoinRoom MsgjoinRoom;
			MsgjoinRoom.dwErrorCode = E_PvP_CanNotJoinRoom ;
			MsgjoinRoom.eRoomLevel  = pRecv->eRoomLevel;
			MsgjoinRoom.nRoomID     =  pRecv->nRoomID;

			pRole->SendMessage(&MsgjoinRoom,MsgjoinRoom.dwSize); 

			return GT_INVALID;
		}
	}
	else
	{
		return GT_INVALID;
	}

	
    //常规的监测
	E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;
	DWORD           dwRoomId   = pRecv->nRoomID;

	DWORD dwErrorCode         =  E_PvP_Success;
	DWORD dwRoleLevel         =  pRole->GetLevel();
	DWORD dwValiantMerit      =  pRole->GetAttValue(ERA_WuXun); 
	DWORD dwValiantMeritLevel =  pRole->GetValiantMeritLevel(dwValiantMerit);

	tagPvPRoom* pPvPRoom = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(dwRoomId,eRoomLevel);
	if ( !P_VALID(pPvPRoom) )
	{
		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_NoRoom ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;
	}
	E_PvP_RoomState eRoomState = pPvPRoom->eRoomState;
	if ( eRoomState == E_PvP_Fighting || eRoomState == E_PvP_CountDown)
	{
		dwErrorCode  =  E_PvP_RoomFighting ;
	}
	else if( dwValiantMeritLevel < pPvPRoom->dwWuXunLimit)
	{
		dwErrorCode  =  E_PvP_NoEnoughWuxin ;
	}
	else if( dwRoleLevel > pPvPRoom->iLevelUpperLimit || dwRoleLevel < pPvPRoom->iLevelLowerLimit)
	{
		dwErrorCode  =  E_PvP_InvalidLevel ;
	}
	else
	{
		dwErrorCode =g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),eRoomLevel,dwRoomId);


	} 

	tagNS_PvPJoinRoom MsgjoinRoom;
	MsgjoinRoom.dwErrorCode = dwErrorCode ;
	MsgjoinRoom.eRoomLevel  = pPvPRoom->eLevel;
	MsgjoinRoom.nRoomID     = pPvPRoom->nRoomID;

	pRole->SendMessage(&MsgjoinRoom,MsgjoinRoom.dwSize); 
	 

    
     
    return 0;
}

DWORD PlayerSession::HandlePvPJoinRoom(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_PvPJoinRoom);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
    return 0;
	
	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_UnOpened ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;
	}
    if (pRole->IsDead())
    {

		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_CanNotJoinRoom ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;

    }

    const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if ( P_VALID( pRoleScript ) )
	{
		BOOL bCanJoin = FALSE;
		pRoleScript->OnRoleJoinToArenaRoom(pRole,pRecv->eRoomLevel,pRecv->nRoomID,bCanJoin);
		if ( !bCanJoin)
		{    
			tagNS_PvPJoinRoom Msg;
			Msg.dwErrorCode = E_PvP_CanNotJoinRoom;
			Msg.eRoomLevel  = pRecv->eRoomLevel;
			Msg.nRoomID     = pRecv->nRoomID;

			pRole->SendMessage(&Msg,Msg.dwSize); 
			return GT_INVALID;
		}
	}
	else
	{
		return GT_INVALID;
	}

	E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;
	DWORD           dwRoomId   = pRecv->nRoomID;

	DWORD dwErrorCode = E_PvP_Success;
	DWORD dwRoleLevel = pRole->GetLevel();
    DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
	DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);
	
	tagPvPRoom* pPvPRoom = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo(dwRoomId,eRoomLevel);
	if ( !P_VALID(pPvPRoom) )
	{
		tagNS_PvPJoinRoom Msg;
		Msg.dwErrorCode = E_PvP_NoRoom ;
		Msg.eRoomLevel  = pRecv->eRoomLevel;
		Msg.nRoomID     = pRecv->nRoomID;

		pRole->SendMessage(&Msg,Msg.dwSize); 
		return GT_INVALID;
	}
	E_PvP_RoomState eRoomState = pPvPRoom->eRoomState;
    if ( eRoomState == E_PvP_Fighting)
    {
         dwErrorCode  =  E_PvP_RoomFighting ;
    }
	else if( dwValiantMeritLevel < pPvPRoom->dwWuXunLimit)
	{
         dwErrorCode  =  E_PvP_NoEnoughWuxin ;
	}
    else if( dwRoleLevel > pPvPRoom->iLevelUpperLimit || dwRoleLevel < pPvPRoom->iLevelLowerLimit)
	{
         dwErrorCode  =  E_PvP_InvalidLevel ;
	}
	else
	{
		dwErrorCode =g_AthleticsSystem.GetRoomMgr().RoleJoinInRoom(pRole->GetID(),eRoomLevel,dwRoomId);
		

	} 
	
	tagNS_PvPJoinRoom MsgjoinRoom;
	MsgjoinRoom.dwErrorCode = dwErrorCode ;
	MsgjoinRoom.eRoomLevel  = pPvPRoom->eLevel;
	MsgjoinRoom.nRoomID     = pPvPRoom->nRoomID;

	pRole->SendMessage(&MsgjoinRoom,MsgjoinRoom.dwSize); 
	

	return dwErrorCode;
}

DWORD PlayerSession::HandlePvPCreateRoom(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPCreateRoom);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	if ( pRecv->iLevelLowerLimit < 30 || pRecv->iLevelUpperLimit > 150)
	{
		return GT_INVALID;
	}

	if (pRole->IsDead())
	{
		tagNS_PvPCreateRoom MsgCreateRoom;
		 ;
		MsgCreateRoom.dwErrorCode = E_PvP_CreateRoomFail;
		MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
		MsgCreateRoom.nRoomID     = 0;
		pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
		return E_PvP_CreateRoomFail;	

	}
	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		tagNS_PvPCreateRoom MsgCreateRoom;
		MsgCreateRoom.dwErrorCode = E_PvP_UnOpened;
		MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
		MsgCreateRoom.nRoomID     = 0;
		pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize); 
		return GT_INVALID;
	}

	DWORD dwErrorCode = 0;
	tagNS_PvPCreateRoom MsgCreateRoom;
    if ( pRecv->iLevelLowerLimit > pRecv->iLevelUpperLimit)
    {
        dwErrorCode = E_PvP_LevelLimitError;
        MsgCreateRoom.dwErrorCode = dwErrorCode;
		MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
		MsgCreateRoom.nRoomID     = 0;
        pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
		return dwErrorCode;
    }
     
	DWORD dwRoleLevel = pRole->GetLevel();
	DWORD dwValiantMerit = pRole->GetAttValue(ERA_WuXun); 
	DWORD dwValiantMeritLevel = pRole->GetValiantMeritLevel(dwValiantMerit);

    if( pRecv->dwWuXunLimit > dwValiantMeritLevel)
	{

		dwErrorCode = E_PvP_WuxunLimitError;
		MsgCreateRoom.dwErrorCode = dwErrorCode;
		MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
		MsgCreateRoom.nRoomID     = 0;
		pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
		return dwErrorCode;
            
	}

	if( dwRoleLevel < pRecv->iLevelLowerLimit || dwRoleLevel > pRecv->iLevelUpperLimit)
	{
		dwErrorCode = E_PvP_CreateRoomFail;
		MsgCreateRoom.dwErrorCode = dwErrorCode;
		MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
		MsgCreateRoom.nRoomID     = 0;
		pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
		return dwErrorCode;

	}

    E_PvP_RoomLevel eRoomLevel = pRecv->eRoomLevel;
    

    TCHAR szRoleName[X_SHORT_NAME];
    DWORD dwRoleId = pRole->GetID();
    g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
    
    tagPvPRoom PvPRoom;
	PvPRoom.nRoomID            = (INT16)g_AthleticsSystem.GetRoomMgr().GetAvailRoomId(eRoomLevel);
	PvPRoom.dwManagerID        = dwRoleId;
	PvPRoom.dwWuXunLimit       = pRecv->dwWuXunLimit;
	PvPRoom.eLevel             = pRecv->eRoomLevel;
	PvPRoom.eRoomState         = E_PvP_Waiting;
	PvPRoom.eRoomType          = E_PvP_Room3v3;
	PvPRoom.iLevelLowerLimit   = pRecv->iLevelLowerLimit;
	PvPRoom.iLevelUpperLimit   = pRecv->iLevelUpperLimit;
	_tcscpy(PvPRoom.szManagerName, szRoleName);
    PvPRoom.ucCurrentPersonNum = 1;
	PvPRoom.ucTotalPersonNum   = 6;


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if ( P_VALID( pRoleScript ) )
	{
		BOOL bCanCreate = FALSE;
		pRoleScript->OnRoleCreateArenaRoom(pRole,pRecv->eRoomLevel,PvPRoom.nRoomID,bCanCreate);
		if ( !bCanCreate)
		{    
			dwErrorCode = E_PvP_CreateRoomFail;
			MsgCreateRoom.dwErrorCode = dwErrorCode;
			MsgCreateRoom.eRoomLevel  = E_PvP_RoomLevelNull;
			MsgCreateRoom.nRoomID     = 0;
			pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
			return dwErrorCode;		
			
		}
	}
	else
	{
		return GT_INVALID;
	}


	dwErrorCode = g_AthleticsSystem.GetRoomMgr().CreateAthleticsRoom(PvPRoom);
   
	if (  dwErrorCode == E_PvP_Success)
	{
		g_AthleticsSystem.ModHallRoomNumTemp(1);
	}
	
	MsgCreateRoom.dwErrorCode = dwErrorCode;
	MsgCreateRoom.eRoomLevel  = PvPRoom.eLevel;
	MsgCreateRoom.nRoomID     = PvPRoom.nRoomID;
	pRole->SendMessage(&MsgCreateRoom,MsgCreateRoom.dwSize);
	return dwErrorCode;
	
	
	
}

DWORD PlayerSession::HandlePvPLeaveRoom(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_PvPLeaveRoom);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	
	DWORD dwRoleId = pRole->GetID();
	tagNS_PvPLeaveRoom MsgLeaveRoom;
	 
    MsgLeaveRoom.dwErrorCode = g_AthleticsSystem.GetRoomMgr().RoleLeaveRoom(dwRoleId);
	pRole->SendMessage(&MsgLeaveRoom,MsgLeaveRoom.dwSize);
   
	return E_PvP_Success;
}

DWORD PlayerSession::HandlePvPChangePos(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPChangePosition);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	DWORD dwRoleId = pRole->GetID();
  	tagNS_PvPChangePosition MsgChangePos;
	tagPvPRoleInRoom* pRoleInRoom =g_AthleticsSystem.GetRoomMgr().GetInRoomRole(dwRoleId);
	if( P_VALID( pRoleInRoom ) )
	{
		RoomBase*   pRoom =   g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
		if ( P_VALID( pRoom ) )
		{

			if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			{
				Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				if( p->GetRoleState(dwRoleId) != E_PvP_Ready )
				{
					if( !p->ChangePos(dwRoleId,pRecv->ucNewPos) )
					{
						MsgChangePos.dwErrorCode = E_PvP_PosIsSolid;
					}
					else
					{
						MsgChangePos.dwErrorCode = E_PvP_Success ;
						pRole->SendMessage(&MsgChangePos,MsgChangePos.dwSize);
						return E_PvP_Success;
					}

					
				}
			}
			else 
			{
				ASSERT(0);
				return GT_INVALID;
			}

		}
	}
	MsgChangePos.dwErrorCode = E_PvP_ChangePosFail ;
	pRole->SendMessage(&MsgChangePos,MsgChangePos.dwSize);


	return E_PvP_ChangePosFail;
}

DWORD PlayerSession::HandlePvPPrepare(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPUnPrepare);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	DWORD dwRoleID = pRole->GetID();
	tagNS_PvPPrepare MsgUnPrepare;
	tagPvPRoleInRoom* pRoleInRoom =g_AthleticsSystem.GetRoomMgr().GetInRoomRole(pRole->GetID());
	if( P_VALID( pRoleInRoom ) )
	{
		RoomBase*   pRoom =   g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
		if ( P_VALID( pRoom ) )
		{

			if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			{
				MsgUnPrepare.dwErrorCode = E_PvP_Success;
				Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				if( p->GetRoomState() == E_PvP_Fighting)
				{
					MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail;
				}  
				else if (p->GetMasterRoleID() == dwRoleID)
				{
					MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail;
				}
				else 
				{
					E_PvP_RoleState eRoleState = p->GetRoleState( dwRoleID );	
					if(eRoleState == E_PvP_Preparing)
					{
						if ( !p->SetRoleState(dwRoleID,E_PvP_Ready))
						{
							MsgUnPrepare.dwErrorCode = E_PvP_Unknown ;
						}
						else
						{
							p->SendRoomInfoToEveryRole();
							MsgUnPrepare.dwErrorCode = E_PvP_Success ;
						}
					}
				}

				pRole->SendMessage(&MsgUnPrepare,MsgUnPrepare.dwSize);
				return E_PvP_Success;
			}
			else 
			{
				ASSERT(0);
				return E_PvP_Unknown;
			}

		}
	}
	MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail ;
	pRole->SendMessage(&MsgUnPrepare,MsgUnPrepare.dwSize);
	return E_PvP_PrepareFail;

}

DWORD PlayerSession::HandlePvPUnPrepare(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPUnPrepare);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	DWORD dwRoleID = pRole->GetID();
	tagNS_PvPUnPrepare MsgUnPrepare;
	tagPvPRoleInRoom* pRoleInRoom =g_AthleticsSystem.GetRoomMgr().GetInRoomRole(pRole->GetID());
	if( P_VALID( pRoleInRoom ) )
	{
		RoomBase*   pRoom =   g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
		if ( P_VALID( pRoom ) )
		{
			if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			{
				MsgUnPrepare.dwErrorCode = E_PvP_Success;

				Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				if( p->GetRoomState() == E_PvP_Fighting)
				{
					MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail;
				}  
				else if (p->GetMasterRoleID() == dwRoleID)
				{
					MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail;
				}
				else 
				{
					E_PvP_RoleState eRoleState = p->GetRoleState( dwRoleID );	
					if(eRoleState == E_PvP_Ready)
					{
						if ( !p->SetRoleState(dwRoleID,E_PvP_Preparing))
						{
							MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail;
						}
						else
						{
							p->SendRoomInfoToEveryRole();
						}
					}
				}
				
				pRole->SendMessage(&MsgUnPrepare,MsgUnPrepare.dwSize);
				return E_PvP_Success;
			}
			else
			{
				ASSERT(0);
				return E_PvP_PrepareFail;
			}

		}
	}
	MsgUnPrepare.dwErrorCode = E_PvP_PrepareFail ;
	pRole->SendMessage(&MsgUnPrepare,MsgUnPrepare.dwSize);
	return E_PvP_PrepareFail;
	
}

DWORD PlayerSession::HandlePvPKickPlayer(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPKickPlayer);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	DWORD dwRoleID = pRole->GetID();

	tagNS_PvPKickPlayer MsgKickPlayer;
	tagPvPRoleInRoom* pRoleInRoom =g_AthleticsSystem.GetRoomMgr().GetInRoomRole(pRecv->dwPlayerID);
	if( P_VALID( pRoleInRoom ) )
	{
		RoomBase*   pRoom =   g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
		if ( P_VALID( pRoom ) )
		{

			if( pRoom->GetRoomType() == E_PvP_Room3v3) 
			{
				MsgKickPlayer.dwErrorCode = E_PvP_Success ;
				Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
				DWORD dwMasterId = p->GetMasterRoleID();
				if( p->GetRoomState() == E_PvP_Fighting)
				{
					 MsgKickPlayer.dwErrorCode = E_PvP_KickPlayerFail;
				}					
				else if ( dwMasterId != dwRoleID)
				{
                     MsgKickPlayer.dwErrorCode = E_PvP_KickPlayerFail;
				}
				else if( pRecv->dwPlayerID == dwRoleID)
				{
                     MsgKickPlayer.dwErrorCode = E_PvP_KickPlayerFail;
				}
				else
				{
                     MsgKickPlayer.dwErrorCode = g_AthleticsSystem.GetRoomMgr().KickRoleToHall(pRecv->dwPlayerID);
				}
				
				
                if(MsgKickPlayer.dwErrorCode == E_PvP_Success)
				{
					p->SendRoomInfoToEveryRole();
					Role* pKicked = g_roleMgr.GetRolePtrByID( pRecv->dwPlayerID );
					if(P_VALID(pKicked))
					{
						tagNS_PvPKicked MsgLeaveRoom;
						MsgLeaveRoom.dwManagerID = dwRoleID ;
						pKicked->SendMessage(&MsgLeaveRoom, MsgLeaveRoom.dwSize);
					}
				}
				
				pRole->SendMessage(&MsgKickPlayer,MsgKickPlayer.dwSize);
				return E_PvP_Success;
			}
			else
			{
				ASSERT(0);
				return GT_INVALID;
			}

		}
	}
	MsgKickPlayer.dwErrorCode = E_PvP_KickPlayerFail ;
	pRole->SendMessage(&MsgKickPlayer,MsgKickPlayer.dwSize);
	return 0;
}

DWORD PlayerSession::HandlePvPStart(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PvPStart);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//屏蔽该部分功能
	//--------------------------------------------------------------------------
	return 0;

	if( g_AthleticsSystem.GetPvPState() == 0)
	{
		return 0;

	}

	DWORD dwRoleID = pRole->GetID();
	tagNS_PvPStart MsgStart;
	tagPvPRoleInRoom* pRoleInRoom =g_AthleticsSystem.GetRoomMgr().GetInRoomRole(pRole->GetID());
	if( !P_VALID( pRoleInRoom ) )
	{   
		return GT_INVALID;
		
	}
	RoomBase*   pRoom =   g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRoleInRoom->dwSerialID);
	if ( !P_VALID( pRoom ) )
	{
		 return GT_INVALID;
	}
	if( pRoom->GetRoomType() == E_PvP_Room3v3)
	{
		Athletics3V3Room * p = (Athletics3V3Room*)pRoom;
		if( p->GetRoomState() == E_PvP_Fighting)
		{
			MsgStart.dwErrorCode =  E_PvP_CannotStart;
			pRole->SendMessage(&MsgStart,MsgStart.dwSize);
			return GT_INVALID;
		}
		if( p->GetRoomState() == E_PvP_CountDown)
		{
           return GT_INVALID;
		}
		DWORD dwMasterId = p->GetMasterRoleID();	
		if( dwMasterId != dwRoleID )
		{
             MsgStart.dwErrorCode =  E_PvP_CannotStart;       
		}
		else if ( p->GetRoomPersonNumber() < P3v3ROOM_TOTAL_PERSON_NUM)
		{
			 MsgStart.dwErrorCode = E_PvP_NeedMorePlayer;
		}
		else 
		{
			if( !p->Start())
			{
				MsgStart.dwErrorCode = E_PvP_PlayerNotReady;
			}
			else
			{
                  tagNS_PvPStarted  msg;
				  for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++)
				  {
                      tagPvPRoleInfo& RoleInfo = p->GetPosRoleInfo(i+1); 
					  if (  RoleInfo.eState != E_PvP_RoleStateNull)
					  {
						  Role* pRoomRole = g_roleMgr.GetRolePtrByID(RoleInfo.dwRoleID);
						  if ( P_VALID(pRoomRole) )
						  {
							  pRoomRole->SendMessage(&msg,msg.dwSize);
						  }
					  }
				  }
				  
			}
   		}
		

         
	}
	pRole->SendMessage(&MsgStart,MsgStart.dwSize);
    
	return E_PvP_Success;
}


