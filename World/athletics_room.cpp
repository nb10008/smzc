//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_room.cpp
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: 竞技场房间
//-----------------------------------------------------------------------------


#include "stdafx.h"

#include "role.h"

#include "role_mgr.h"
#include "title_mgr.h"
#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/role_att.h"





#include "athletics_room_mgr.h"
#include "athletics_system.h"
#include "athletics_room.h"
#include "../ServerDefine/mini_tool.h"

Athletics3V3Room::Athletics3V3Room():m_dwSerialID(0),m_bIsEnterPvP(FALSE)
{
	memset(&m_PvPRoleInfo,0,sizeof(m_PvPRoleInfo)); 
	ZeroMemory(&m_PvPRoomPosState,sizeof(m_PvPRoomPosState));
	ZeroMemory(&m_PvPRoom,sizeof(m_PvPRoom));
	ZeroMemory(&m_PvPRoleRemainTime,sizeof(m_PvPRoleRemainTime));
	m_dwRemainTime      = ROOM_REMAIN_TIME ;
	m_dwRemainTimeStart = ROOM_REMAIN_TIME_START;
}


Athletics3V3Room::~Athletics3V3Room() 
{ 


}


BOOL Athletics3V3Room::Init( const tagPvPRoom& pvpRoom,DWORD  dwSerialID)
{
    m_PvPRoom.dwManagerID              = pvpRoom.dwManagerID;
	m_PvPRoom.dwWuXunLimit             = pvpRoom.dwWuXunLimit;
	m_PvPRoom.eLevel                   = pvpRoom.eLevel;
	m_PvPRoom.eRoomState               = pvpRoom.eRoomState;
	m_PvPRoom.eRoomType                = pvpRoom.eRoomType;
	m_PvPRoom.iLevelLowerLimit         = pvpRoom.iLevelLowerLimit;
	m_PvPRoom.iLevelUpperLimit         = pvpRoom.iLevelUpperLimit;
	m_PvPRoom.nRoomID                  = pvpRoom.nRoomID;
	_tcscpy_s(m_PvPRoom.szManagerName,X_SHORT_NAME, pvpRoom.szManagerName);
	m_PvPRoom.ucCurrentPersonNum       = pvpRoom.ucCurrentPersonNum ;
	m_PvPRoom.ucTotalPersonNum         = pvpRoom.ucTotalPersonNum;
    m_PvPRoom.dwSerialID               = dwSerialID;
	m_dwSerialID                       = dwSerialID;
    m_dwRemainTime                     = ROOM_REMAIN_TIME ;
	

	SetRoomState(E_PvP_Waiting);

	SetPosState(1,ERPS_Solid);
    Role* pRole = g_roleMgr.GetRolePtrByID(pvpRoom.dwManagerID);
	if( !P_VALID(pRole) )
	{
		return FALSE;
	}
	tagPvPRoleInfo roleInfo;
    roleInfo.dwRoleID    = pvpRoom.dwManagerID;
	roleInfo.eState      = E_PvP_Ready;
	roleInfo.ucPos       = 1;
    roleInfo.eRoleInSide = ERBS_DefenseSide;
	roleInfo.eRoleInPvP  = ERIP_InPvP;
	roleInfo.dwRoleLevel = pRole->GetLevel();
	_tcscpy_s(roleInfo.szName,X_SHORT_NAME, pvpRoom.szManagerName);
	roleInfo.dwWuXun = pRole->GetAttValue(ERA_WuXun);
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//增加该部分功能
	//--------------------------------------------------------------------------
	roleInfo.ePlayerProfession = pRole->GetClass();
	
	SetPosRoleInfo(1,roleInfo);


	tagNS_PvPRanRoomInfo Msg;
	
	Msg.bCount                   = 1;
	Msg.pInfo[0].ePlayerProfession = roleInfo.ePlayerProfession;
	Msg.pInfo[0].nPlayerLevel    =  roleInfo.dwRoleLevel;
	Msg.pInfo[0].nPos            =  roleInfo.ucPos;
	Msg.pInfo[0].dwPlayerID      =  roleInfo.dwRoleID;
	_tcscpy_s(Msg.pInfo[0].szName,X_SHORT_NAME, pvpRoom.szManagerName);

	pRole->SendMessage(&Msg,Msg.dwSize);

	 
    
	
    return TRUE;
}

VOID Athletics3V3Room::Destroy()
{

}

VOID Athletics3V3Room::Update()
{
	 UpdateRoomState();
}

VOID  Athletics3V3Room::UpdateRoomState()
{
	if ( m_PvPRoom.eRoomState == E_PvP_Waiting)
	{
		//--------------------------------------------------------------------------
		//SM 竞技场的最近修改
		//屏蔽该部分功能
		//RunRemainTimeKickRole();
		//--------------------------------------------------------------------------
		 RunRemainTimeStart();
	}
	// 	if ( m_PvPRoom.eRoomState  == E_PvP_CountDown)
	// 	{
	//          RunRemainTime();
	// 	}

}

VOID Athletics3V3Room::RunRemainTime()
{
	if( GetRoomState() == E_PvP_CountDown)
	{
		m_dwRemainTime--;
		if ( m_dwRemainTime <= 0 )
		{
              
			if( !SetRoleState(GetMasterRoleID(),E_PvP_Ready))
			{
				return ;
			}else
			{
				SetRoomState(E_PvP_Fighting); 
				SendRoomInfoToEveryRole();
			}


			tagPvPRoom*  pRoomlist = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo((INT16)GetRoomID(),GetRoomLevel());
			if ( P_VALID( pRoomlist ) )
			{
				pRoomlist->dwManagerID        = GetMasterRoleID();
				pRoomlist->eRoomState         = GetRoomState();
				pRoomlist->dwSerialID         = GetRoomSerialID();
				pRoomlist->ucCurrentPersonNum = GetRoomPersonNumber();
				_tcscpy_s(pRoomlist->szManagerName,X_SHORT_NAME,GetMasterName());
			}
			
			//g_AthleticsSystem.ModFightingRoomNumTemp(1);
		
			m_dwRemainTime      = REMAIN_TIME ;
			m_dwRemainTimeStart = ROOM_REMAIN_TIME_START;

		}
		else
		{
			INT nReadyCount = 0 ;
			for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
			{

				tagPvPRoleInfo& RoleInfo   = m_PvPRoleInfo[i];
				E_PvP_RoleState eRoleState = RoleInfo.eState;
				if( eRoleState != E_PvP_Ready)
				{
					if( RoleInfo.dwRoleID == GetMasterRoleID())
					{
						continue;
					}
					else
					{
						break;
					}			 
				}
				nReadyCount++; 
			}
			if (nReadyCount != P3v3ROOM_TOTAL_PERSON_NUM-1)
			{
				m_dwRemainTime      = ROOM_REMAIN_TIME ;
				m_dwRemainTimeStart = ROOM_REMAIN_TIME_START;
				SetRoomState(E_PvP_Waiting);
				SendRoomInfoToEveryRole(); 
			}
		}
	}
}

VOID Athletics3V3Room::RunRemainTimeKickRole()
{
	for (INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM; i++)
	{
		if( 0 != m_PvPRoleInfo[i].dwRoleID && m_PvPRoleInfo[i].dwRoleID != m_PvPRoom.dwManagerID)
		{
			if ( m_PvPRoleInfo[i].eState == E_PvP_Preparing)
			{
				m_PvPRoleRemainTime[i]--;
                DWORD dwErrorCode = 0;
				if( m_PvPRoleRemainTime[i] <= 0 )
				{
					DWORD dwRoleId  = m_PvPRoleInfo[i].dwRoleID ;
					dwErrorCode = g_AthleticsSystem.GetRoomMgr().KickRoleToHall(m_PvPRoleInfo[i].dwRoleID);

					if(dwErrorCode == E_PvP_Success)
					{
						SendRoomInfoToEveryRole();
						Role* pKicked = g_roleMgr.GetRolePtrByID(dwRoleId);
						if(P_VALID(pKicked))
						{
							tagNS_PvPKicked MsgLeaveRoom;
							MsgLeaveRoom.dwManagerID = 0 ;
							pKicked->SendMessage(&MsgLeaveRoom, MsgLeaveRoom.dwSize);

						}
					}
				}
			}
		}
	}
}


VOID Athletics3V3Room::RunRemainTimeStart()
{
	if ( GetRoomPersonNumber() != P3v3ROOM_TOTAL_PERSON_NUM)
	{
		return ;
	}

     m_dwRemainTimeStart--;
	 INT nReadyCount = GetRoomPersonNumber() ;
	// 	 for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
	// 	 {
	// 
	// 		 tagPvPRoleInfo& RoleInfo   = m_PvPRoleInfo[i];
	// 		 E_PvP_RoleState eRoleState = RoleInfo.eState;
	// 		 if( eRoleState != E_PvP_Ready)
	// 		 {
	// 			 if( RoleInfo.dwRoleID == GetMasterRoleID())
	// 			 {
	// 				 continue;
	// 			 }
	// 			 else
	// 			 {
	// 				 break;
	// 			 }			 
	// 		 }
	// 		 nReadyCount++; 
	// 	 }
	 if ( nReadyCount != P3v3ROOM_TOTAL_PERSON_NUM )
	 {
           m_dwRemainTimeStart = ROOM_REMAIN_TIME_START ;
	 }
	 else
	 {
		   if ( m_dwRemainTimeStart == ROOM_REMAIN_TIME)
		   {
			   tagNS_PvPStarted  msg;
			   for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++)
			   {
				   tagPvPRoleInfo& RoleInfo = GetPosRoleInfo(i+1); 
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

           if ( m_dwRemainTimeStart <= 0)
           {
			   m_dwRemainTime = ROOM_REMAIN_TIME_START ;
			  
			   SetRoomState(E_PvP_Fighting); 
			   SendRoomInfoToEveryRole();
			   
			   tagPvPRoom*  pRoomlist = g_AthleticsSystem.GetRoomMgr().GetHallRoomInfo((INT16)GetRoomID(),GetRoomLevel());
			   if ( P_VALID( pRoomlist ) )
			   {
				   pRoomlist->dwManagerID        = GetMasterRoleID();
				   pRoomlist->eRoomState         = GetRoomState();
				   pRoomlist->dwSerialID         = GetRoomSerialID();
				   pRoomlist->ucCurrentPersonNum = GetRoomPersonNumber();
				   _tcscpy_s(pRoomlist->szManagerName,X_SHORT_NAME,GetMasterName());
			   }
			   //g_AthleticsSystem.ModFightingRoomNumTemp(1);
			   //-------------------------------------------------------------------------------------------------
			   //通知所有玩家进入竞技场
			   //-------------------------------------------------------------------------------------------------
			   
           }
	 }
}

BOOL Athletics3V3Room::AddRole(DWORD dwRoleID)
{
    

    if( P3v3ROOM_TOTAL_PERSON_NUM <= GetRoomPersonNumber() )
	{
		return FALSE;
	}
	
	INT nPos = 0;
    for(INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++)
	{
         E_Room_Pos_State   eRoomPosState = GetPosState(i+1);
		 if ( eRoomPosState == ERPS_Empty )
		 {
              nPos = i+1;
			  break;
		 }
	}
	if ( nPos <=0 || nPos >P3v3ROOM_TOTAL_PERSON_NUM )
	{
		return FALSE;
	}
	
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if ( !P_VALID(pRole))
	{
		return FALSE;
	}
	SetPosState(nPos,ERPS_Solid);
	m_PvPRoom.ucCurrentPersonNum++;

    tagPvPRoleInfo posRoleInfo;
    posRoleInfo.dwRoleID      = dwRoleID ;
    posRoleInfo.eState        = E_PvP_Ready;
	posRoleInfo.ucPos         = nPos;
    posRoleInfo.eRoleInPvP    = ERIP_InPvP;
	posRoleInfo.dwRoleLevel   = pRole->GetLevel();
	g_roleMgr.GetRoleNameByID(dwRoleID,posRoleInfo.szName);
	posRoleInfo.dwWuXun = pRole->GetAttValue(ERA_WuXun);
	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//增加该部分功能 
	//--------------------------------------------------------------------------
	posRoleInfo.ePlayerProfession = pRole->GetClass();

	SetPosRoleInfo(nPos,posRoleInfo);
	if( nPos > 0 )
	m_PvPRoleRemainTime[nPos-1] = REMAIN_TIME;

	
    SendRoomInfoToEveryRole();
    return TRUE;

}

BOOL Athletics3V3Room::Start()
{
	if( GetRoomPersonNumber() < P3v3ROOM_TOTAL_PERSON_NUM)
	{
		return FALSE;
	}
	if( GetRoomState() == E_PvP_Fighting)
	{
		return FALSE;
	}
	INT nReadyCount = 0 ;
	for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM; i++)
	{
		 tagPvPRoleInfo& RoleInfo   = m_PvPRoleInfo[i];
		 E_PvP_RoleState eRoleState = RoleInfo.eState;
		 if( eRoleState != E_PvP_Ready)
		 {
			 if( RoleInfo.dwRoleID == GetMasterRoleID())
			 {
                 continue;
			 }
			 else
			 {
                return FALSE;
			 }			 
		 }
		 nReadyCount++;
	}
	if ( nReadyCount != P3v3ROOM_TOTAL_PERSON_NUM - 1 )
	{
		return FALSE;
	}
	else
	{
		SetRoomState(E_PvP_CountDown); 
		m_dwRemainTime = ROOM_REMAIN_TIME ;
	}
	
	return TRUE;
}

BOOL Athletics3V3Room::LeaveRole(DWORD dwRoleID)
{
	if (GetRoomPersonNumber() < 0)
	{
		return FALSE;
	}

	

	INT nPos = 0;
	for ( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++  )
	{
		E_Room_Pos_State   eRoomPosState = GetPosState(i+1);
		if ( eRoomPosState == ERPS_Solid )
		{
			nPos = i+1;
            tagPvPRoleInfo& posRoleInfo = GetPosRoleInfo(nPos);
			if( posRoleInfo.dwRoleID == dwRoleID )
			{
				// 				if( posRoleInfo.eState == E_PvP_Ready)
				// 				{
				// 					return FALSE;
				// 				}

				m_PvPRoom.ucCurrentPersonNum--;
				if( 0 == GetRoomPersonNumber())
				{
					SetPosState(nPos,ERPS_Empty);
					posRoleInfo.dwRoleID = 0;
					posRoleInfo.eState   = E_PvP_RoleStateNull;
					posRoleInfo.ucPos    = 0;
					posRoleInfo.eRoleInSide = ERBS_NULL;
					posRoleInfo.eRoleInPvP  = ERIP_NULL;
					posRoleInfo.dwRoleLevel = 0;
					posRoleInfo.dwWuXun     = 0;
					posRoleInfo.ePlayerProfession = EV_Null;
					memset(posRoleInfo.szName,0,X_SHORT_NAME);
					SetRoomState(E_PvP_RoomStateNull);
					return TRUE;
				}

                SetPosState(nPos,ERPS_Empty);

				//--------------------------------------------------------------------------
				//SM 竞技场的最近修改
				//屏蔽该部分功能
				//--------------------------------------------------------------------------

// 				if( GetMasterRoleID() ==  dwRoleID )
// 				{
// 					INT tPos = 1;
// 
//                     while( tPos <= P3v3ROOM_TOTAL_PERSON_NUM )
// 					{
//                        E_Room_Pos_State   ePosState = GetPosState(tPos);
// 					   if ( ePosState == ERPS_Solid)
// 					   {
//                            tagPvPRoleInfo& posRoleInfoTemp = GetPosRoleInfo(tPos);
//                            posRoleInfoTemp.eState = E_PvP_Preparing;
//                            SetMaster(tPos,posRoleInfoTemp.dwRoleID);
// 						   TCHAR szRoleName[X_SHORT_NAME];
// 						   DWORD dwRoleId = posRoleInfoTemp.dwRoleID;
// 						   g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
// 						   _tcscpy(m_PvPRoom.szManagerName, szRoleName);
// 						   break;
// 					   }
// 					   tPos++;
// 					}
// 				}
				posRoleInfo.dwRoleID = 0;
				posRoleInfo.eState   = E_PvP_RoleStateNull;
				posRoleInfo.ucPos    = 0;
				posRoleInfo.eRoleInSide = ERBS_NULL;
				posRoleInfo.eRoleInPvP  = ERIP_NULL;
				posRoleInfo.dwRoleLevel = 0;
				posRoleInfo.dwWuXun     = 0;
				posRoleInfo.ePlayerProfession = EV_Null;
				memset(posRoleInfo.szName,0,X_SHORT_NAME);
                
				SendRoomInfoToEveryRole();
				
				
				return TRUE;
			}
		}

	}
	return FALSE;
     
}

BOOL Athletics3V3Room::KickRole(DWORD dwRoleID,BOOL bIsSend)
{
	if (GetRoomPersonNumber() < 0)
	{
		return FALSE;
	}


	INT nPos = 0;
	for ( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++  )
	{
		E_Room_Pos_State   eRoomPosState = GetPosState(i+1);
		if ( eRoomPosState == ERPS_Solid )
		{
			nPos = i+1;
			tagPvPRoleInfo& posRoleInfo = GetPosRoleInfo(nPos);
			if( posRoleInfo.dwRoleID == dwRoleID )
			{
				

				m_PvPRoom.ucCurrentPersonNum--;
				if( 0 == GetRoomPersonNumber())
				{
					SetPosState(nPos,ERPS_Empty);
					posRoleInfo.dwRoleID = 0;
					posRoleInfo.eState   = E_PvP_RoleStateNull;
					posRoleInfo.ucPos    = 0;
					posRoleInfo.eRoleInSide = ERBS_NULL;
					posRoleInfo.eRoleInPvP  = ERIP_NULL;
					posRoleInfo.dwRoleLevel = 0;
					posRoleInfo.dwWuXun     = 0;
					posRoleInfo.ePlayerProfession = EV_Null;
					memset(posRoleInfo.szName,0,X_SHORT_NAME);
					SetRoomState(E_PvP_RoomStateNull);
					//SetIsPvPRoleEnterPvP(FALSE);
					return TRUE;
				}

				SetPosState(nPos,ERPS_Empty);

				//--------------------------------------------------------------------------
				//SM 竞技场的最近修改
				//屏蔽该部分功能
				//--------------------------------------------------------------------------

				// 				if( GetMasterRoleID() ==  dwRoleID )
				// 				{
				// 					INT tPos = 1;
				// 
				// 					while( tPos <= P3v3ROOM_TOTAL_PERSON_NUM )
				// 					{
				// 						E_Room_Pos_State   ePosState = GetPosState(tPos);
				// 						if ( ePosState == ERPS_Solid)
				// 						{
				// 							tagPvPRoleInfo& posRoleInfoTemp = GetPosRoleInfo(tPos);
				// 							posRoleInfoTemp.eState = E_PvP_Preparing;
				// 							SetMaster(tPos,posRoleInfoTemp.dwRoleID);
				// 							TCHAR szRoleName[X_SHORT_NAME];
				// 							DWORD dwRoleId = posRoleInfoTemp.dwRoleID;
				// 							g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
				// 							_tcscpy_s(m_PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
				// 							break;
				// 						}
				// 						tPos++;
				// 					}
				// 				}
				posRoleInfo.dwRoleID = 0;
				posRoleInfo.eState   = E_PvP_RoleStateNull;
				posRoleInfo.ucPos    = 0;
				posRoleInfo.eRoleInSide = ERBS_NULL;
				posRoleInfo.eRoleInPvP  = ERIP_NULL;
				posRoleInfo.dwRoleLevel = 0;
				posRoleInfo.dwWuXun     = 0;
				memset(posRoleInfo.szName,0,X_SHORT_NAME);
				posRoleInfo.ePlayerProfession = EV_Null;

				if ( bIsSend )
				{
					SendRoomInfoToEveryRole();
				}

				


				return TRUE;
			}
		}

	}
	return FALSE;
}

BOOL Athletics3V3Room::ChangePos(DWORD dwRoleID,INT nNewPos)
{
	if ( nNewPos <= 0 || nNewPos > P3v3ROOM_TOTAL_PERSON_NUM )
	{
		return FALSE;
	}

	E_Room_Pos_State   eRoomPosState = GetPosState(nNewPos);
	if( eRoomPosState == ERPS_Solid )
	{
		return FALSE;      
	}
	INT nPos = GetRolePos(dwRoleID);
	if ( !GT_VALID(nPos) )
	{
		return FALSE;
	}

	if( nNewPos == nPos)
	{
		return FALSE;
	}

	//得到旧位置的角色信息
	tagPvPRoleInfo& posRoleInfo = GetPosRoleInfo(nPos);

	posRoleInfo.ucPos = nNewPos;
	// 设置新位置的角色信息
	SetPosRoleInfo(nNewPos,posRoleInfo);
	m_PvPRoleRemainTime[nNewPos-1] = m_PvPRoleRemainTime[nPos -1];

	//新位置设置
	SetPosState(nNewPos,ERPS_Solid);
    
	//把旧的信息设置成空
    posRoleInfo.dwRoleID    = 0;
	posRoleInfo.ucPos       = 0;
	posRoleInfo.eState      = E_PvP_RoleStateNull;
    posRoleInfo.eRoleInPvP  = ERIP_NULL ;
    posRoleInfo.eRoleInSide = ERBS_NULL;
	posRoleInfo.dwRoleLevel = 0;
	posRoleInfo.dwWuXun     = 0;
	memset(posRoleInfo.szName,0,X_SHORT_NAME);
	m_PvPRoleRemainTime[nPos-1] = 0;

	//把旧位置设置的空
	SetPosState(nPos,ERPS_Empty);
   
	//向其他玩家广播房间状态
    SendRoomInfoToEveryRole();


	return TRUE;
}


INT Athletics3V3Room::GetRolePos(DWORD dwRoleID)
{
    INT  nPos = 0 ;
	for( INT i = 0; i < P3v3ROOM_TOTAL_PERSON_NUM; i++)
	{
		nPos = i+1;
		tagPvPRoleInfo& posRoleInfoTemp = GetPosRoleInfo(nPos);
		if ( posRoleInfoTemp.dwRoleID == dwRoleID )
		{
			return nPos;
		}
	}
	return 0;
}

BOOL Athletics3V3Room::SetRoleState(DWORD dwRoleID,E_PvP_RoleState eRoleState)
{
    INT nPos = GetRolePos( dwRoleID );
	if ( nPos <= 0 || nPos > P3v3ROOM_TOTAL_PERSON_NUM)
	{
		return FALSE;
	}
	tagPvPRoleInfo& RoleInfo = GetPosRoleInfo( nPos );
	RoleInfo.eState = eRoleState;
	if ( eRoleState == E_PvP_Preparing)
	{
		m_PvPRoleRemainTime[nPos-1] = REMAIN_TIME;
		//RoleInfo.dwRemainTime = REMAIN_TIME ;
	}
	return TRUE;

}

E_PvP_RoleState Athletics3V3Room::GetRoleState(DWORD dwRoleID)
{
	INT nPos = GetRolePos( dwRoleID );
	tagPvPRoleInfo& RoleInfo = GetPosRoleInfo( nPos );
	return  RoleInfo.eState ;
}


E_Role_BothSide  Athletics3V3Room::GetRoleInSide(DWORD dwRoleID)
{
	INT nPos = GetRolePos( dwRoleID );
	tagPvPRoleInfo& RoleInfo = GetPosRoleInfo( nPos );
	return RoleInfo.eRoleInSide;
}

E_Role_InPvP   Athletics3V3Room::GetInPvP(DWORD dwRoleID)
{
	INT nPos = GetRolePos( dwRoleID );
	tagPvPRoleInfo& RoleInfo = GetPosRoleInfo( nPos );
	return RoleInfo.eRoleInPvP;
}

VOID Athletics3V3Room::SendRoomInfoToEveryRole()
{
	INT16 nPersonNum = 0;
	INT32 nMsgSz = 0;
	nPersonNum = GetRoomPersonNumber();
	if( nPersonNum <=0 )
	{
		return;
	}

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//增加了该部分功能  
	//--------------------------------------------------------------------------
    nMsgSz = sizeof(tagNS_PvPRanRoomInfo)  + (nPersonNum-1)*(sizeof(tagRanPvPInfo)) ;    
	MCREATE_MSG(pSend, nMsgSz, NS_PvPRanRoomInfo);
	pSend->bCount = 0;

	for (INT i = 0; i < P3v3ROOM_TOTAL_PERSON_NUM; i++ )
	{
		if( m_PvPRoleInfo[i].eState != E_PvP_RoleStateNull )
		{
			if (pSend->bCount >= nPersonNum)
			{
				break;
			}
			pSend->pInfo[pSend->bCount].ePlayerProfession = m_PvPRoleInfo[i].ePlayerProfession;
			pSend->pInfo[pSend->bCount].nPlayerLevel      = m_PvPRoleInfo[i].dwRoleLevel;
			pSend->pInfo[pSend->bCount].nPos              = m_PvPRoleInfo[i].ucPos;
			pSend->pInfo[pSend->bCount].dwPlayerID        = m_PvPRoleInfo[i].dwRoleID;
			_tcscpy_s(pSend->pInfo[pSend->bCount].szName,X_SHORT_NAME, m_PvPRoleInfo[i].szName);
			pSend->bCount++;
		}

	}

	for(INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
	{

		if( m_PvPRoleInfo[i].eState != E_PvP_RoleStateNull )
		{  
			Role* pRole = g_roleMgr.GetRolePtrByID( m_PvPRoleInfo[i].dwRoleID );
			if( P_VALID(pRole) )
			{
				pRole->SendMessage(pSend,pSend->dwSize);  
			}

		}

	}

    MDEL_MSG(pSend);

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//注释了该部分功能  
	//--------------------------------------------------------------------------
// 	nMsgSz = sizeof(tagNS_PvPGetRoomInfo)  + (nPersonNum-1)*(sizeof(tagPvPRoleInfo)) ;
// 	
// 	MCREATE_MSG(pSend, nMsgSz, NS_PvPGetRoomInfo);
// 	pSend->dwErrorCode = 0;
// 	pSend->dwCount    = 1 ;
// 	pSend->pData[0].dwManagerID = GetMasterRoleID();
// 	pSend->pData[0].dwSerialID  = GetRoomSerialID();
// 	pSend->pData[0].nRoomID		= m_PvPRoom.nRoomID;
//     pSend->pData[0].dwWuXunLimit= m_PvPRoom.dwWuXunLimit;
// 	pSend->pData[0].eLevel      = m_PvPRoom.eLevel;
//     pSend->pData[0].eRoomState  = m_PvPRoom.eRoomState;
// 	pSend->pData[0].eRoomType   = m_PvPRoom.eRoomType;
// 	pSend->pData[0].iLevelLowerLimit = m_PvPRoom.iLevelLowerLimit;
// 	pSend->pData[0].iLevelUpperLimit = m_PvPRoom.iLevelUpperLimit;
// 	pSend->pData[0].ucCurrentPersonNum  = 0;
// 	
// 	for(INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
// 	{
// 		if( m_PvPRoleInfo[i].eState != E_PvP_RoleStateNull )
// 		{
//            memcpy(&(pSend->pData[0].pData[pSend->pData[0].ucCurrentPersonNum ]), &m_PvPRoleInfo[i], sizeof(tagPvPRoleInfo));
// 		   pSend->pData[0].ucCurrentPersonNum ++;
// 		}
// 
// 	}
// 	_tcscpy(pSend->pData[0].szManagerName,m_PvPRoom.szManagerName);
// 	pSend->pData[0].ucTotalPersonNum = m_PvPRoom.ucTotalPersonNum;
// 
// 	for(INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
// 	{
// 
// 		if( m_PvPRoleInfo[i].eState != E_PvP_RoleStateNull )
// 		{  
// 			Role* pRole = g_roleMgr.GetRolePtrByID( m_PvPRoleInfo[i].dwRoleID );
// 			if( P_VALID(pRole) )
// 			{
// 				pRole->SendMessage(pSend,pSend->dwSize);  
// 			}
// 
// 		}
// 
// 	}
//     MDEL_MSG(pSend);
}

//---------------------------------------------------------------------
//向竞技场房间的角色广播远程属性，如血调属性
//---------------------------------------------------------------------
VOID  Athletics3V3Room::SendRemoteAttToPvPRole(DWORD dwRoleID,BOOL bIsInit)
{
	tagNS_RemoteAttChangeSingle Msg;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID( pRole))
	{
		return ;
	}

	for( INT i = 0; i < P3v3ROOM_TOTAL_PERSON_NUM; i++)
	{
		if( 0 == m_PvPRoleInfo[i].dwRoleID || GT_INVALID == m_PvPRoleInfo[i].dwRoleID ||  m_PvPRoleInfo[i].dwRoleID == dwRoleID)
		{
			continue;
		}
		Role* pRoomRole = g_roleMgr.GetRolePtrByID(m_PvPRoleInfo[i].dwRoleID);
		if ( !P_VALID(pRoomRole))
		{
			continue;
		}

		if ( !bIsInit )
		{
			if ( pRole->IsInState(ES_PvPAttack) ==  pRoomRole->IsInState(ES_PvPAttack))
			{
				continue;
			}
		}

		Msg.dwRoleID = dwRoleID;
		Msg.eType    = ERRA_MaxHP;
		Msg.nValue   = pRole->GetAttValue(ERA_MaxHP);
		pRoomRole->SendMessage(&Msg,Msg.dwSize);

		Msg.eType    = ERRA_HP;
		Msg.nValue   = pRole->GetAttValue(ERA_HP);
		pRoomRole->SendMessage(&Msg,Msg.dwSize);

		if ( bIsInit )
		{
			Msg.dwRoleID = m_PvPRoleInfo[i].dwRoleID;
			Msg.eType    = ERRA_MaxHP;
			Msg.nValue   = pRoomRole->GetAttValue(ERA_MaxHP);
			pRole->SendMessage(&Msg,Msg.dwSize);

			Msg.eType   = ERRA_HP;
			Msg.nValue  = pRoomRole->GetAttValue(ERA_HP);
			pRole->SendMessage(&Msg,Msg.dwSize);
		}

		
	}


}

//等待修改
BOOL Athletics3V3Room::AddRoleFromPvP(const tagPvPRoleInfo* pRoleInfo,INT16 nRoleNum)
{
	  if( !P_VALID(pRoleInfo) || nRoleNum <= 0 )
	  {
		  return FALSE;
	  }
	  memset(&m_PvPRoomPosState,0,sizeof(m_PvPRoomPosState));
	  memset(&m_PvPRoleInfo,0,sizeof(m_PvPRoleInfo));
	  BOOL bIsMasterInRoom = FALSE;
	  for(INT16 i = 0 ; i < nRoleNum; i++)
	  {
		  if( GetMasterRoleID() == pRoleInfo->dwRoleID)
		  {
              bIsMasterInRoom = TRUE;
		  }
		  SetPosRoleInfo(pRoleInfo->ucPos,*(pRoleInfo+i));
		  SetPosState(pRoleInfo->ucPos,ERPS_Solid);
	  }
      m_PvPRoom.ucCurrentPersonNum  =(INT16)nRoleNum;

      //假如找不到房主，　就重新设置房主
	  if( !bIsMasterInRoom )
	  {
          INT tPos = 1;
          while( tPos <= P3v3ROOM_TOTAL_PERSON_NUM )
		  {
			  E_Room_Pos_State   ePosState = GetPosState(tPos);
			  if ( ePosState == ERPS_Solid)
			  {
				  tagPvPRoleInfo& posRoleInfoTemp = GetPosRoleInfo(tPos);
				  posRoleInfoTemp.eState = E_PvP_Preparing;
				  SetMaster(tPos,posRoleInfoTemp.dwRoleID);
				  TCHAR szRoleName[X_SHORT_NAME];
				  DWORD dwRoleId = posRoleInfoTemp.dwRoleID;
				  g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
				  _tcscpy_s(m_PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
				  break;
			  }
			  tPos++;
		  }
	  }

	  SetRoomState(E_PvP_Waiting);

	  
	  return TRUE;
}

BOOL Athletics3V3Room::DeleteRoleFromPvP(DWORD dwRoleID1,DWORD dwRoleID2,DWORD dwRoleID3,DWORD dwRoleID4,DWORD dwRoleID5,DWORD dwRoleID6)
{
	 INT nRoleNum = 0 ;
	 BOOL bIsReSetMaster = TRUE;
	 DWORD  dwRoleID[P3v3ROOM_TOTAL_PERSON_NUM] = {0};
	 dwRoleID[0] = dwRoleID1 ;
	 dwRoleID[1] = dwRoleID2 ; 
	 dwRoleID[2] = dwRoleID3 ;
	 dwRoleID[3] = dwRoleID4 ;
	 dwRoleID[4] = dwRoleID5 ;
	 dwRoleID[5] = dwRoleID6 ;

	 m_PvPRoom.ucCurrentPersonNum = nRoleNum;

	 SetRoomState(E_PvP_RoomStateNull);	

	 //------------------------------------------------------------------------
	 //把房间更新打开
	 //------------------------------------------------------------------------
	 SetIsPvPRoleEnterPvP(FALSE);
	 //如果没有角色了，就返回
	 if ( nRoleNum <= 0)
	 {		 
		 //SetRoomState(E_PvP_RoomStateNull);
		 //SetIsPvPRoleEnterPvP(FALSE);
		 // Jay 2010 4-11 link error of inline functions make server crush
		 m_PvPRoom.eRoomState = E_PvP_RoomStateNull;
		 m_bIsEnterPvP = FALSE;
		 return TRUE;
	 }

	 return TRUE;

		/* tagPvPRoleInfo  PvPRoleInfo[P3v3ROOM_TOTAL_PERSON_NUM];
		 memset(&PvPRoleInfo,0,sizeof(PvPRoleInfo));
		 memcpy(&PvPRoleInfo,&m_PvPRoleInfo,sizeof(m_PvPRoleInfo));
		 memset(&m_PvPRoleInfo,0,sizeof(m_PvPRoleInfo));

		 for( INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++)
		 {
			 if ( 0 == dwRoleID[i] || -1 == dwRoleID[i] )
			 {
				 nRoleNum--;
				 continue;
			 }
			 else
			 {
				 INT nP =  0;
				 for( INT nI = 0; nI < P3v3ROOM_TOTAL_PERSON_NUM ; nI++)
				 {
					 if(PvPRoleInfo[nI].dwRoleID == dwRoleID[i])
					 {
						 nP = PvPRoleInfo[nI].ucPos;
						 break;
					 }
				 }
				 if ( 0 == nP || nP > P3v3ROOM_TOTAL_PERSON_NUM )
				 {
					 nRoleNum--;
					 continue;
				 }
				 else
				 {

					 if (dwRoleID[i] == GetMasterRoleID())
					 {
						 bIsReSetMaster = FALSE;
					 }
					 memcpy(&m_PvPRoleInfo[nP-1],&PvPRoleInfo[nP-1],sizeof(PvPRoleInfo[0]));
					 m_PvPRoleInfo[nP-1].eState = E_PvP_Preparing ;
					 m_PvPRoleRemainTime[nP-1]  = REMAIN_TIME;

				 }
			 }

		 }*/
     
	


	 
	// 
	// 	 //假如找不到房主，　就重新设置房主
	// 	 if( bIsReSetMaster )
	// 	 {
	// 		 INT tPos = 1;
	// 		 while( tPos <= P3v3ROOM_TOTAL_PERSON_NUM )
	// 		 {
	// 			 E_Room_Pos_State   ePosState = GetPosState(tPos);
	// 			 if ( ePosState == ERPS_Solid)
	// 			 {
	// 				 tagPvPRoleInfo& posRoleInfoTemp = GetPosRoleInfo(tPos);
	// 				 posRoleInfoTemp.eState = E_PvP_Preparing;
	// 				 SetMaster(tPos,posRoleInfoTemp.dwRoleID);
	// 				 TCHAR szRoleName[X_SHORT_NAME];
	// 				 DWORD dwRoleId = posRoleInfoTemp.dwRoleID;
	// 				 g_roleMgr.GetRoleNameByID(dwRoleId,szRoleName);
	// 				 _tcscpy_s(m_PvPRoom.szManagerName,X_SHORT_NAME, szRoleName);
	// 				 break;
	// 			 }
	// 			 tPos++;
	// 		 }                                                                
	// 	 }
	
	
  //   //------------------------------------------------------------------------
	 ////向在线的玩家同步 回到房间的消息
	 ////------------------------------------------------------------------------
	 //tagNS_PvPSetIntoRoom  Msg;
	 //Msg.eLevel    = GetRoomLevel();
	 //Msg.hallType  = GetRoomType();
	 //Msg.roomID    = GetRoomID();
	
	 //for(INT i = 0 ; i < P3v3ROOM_TOTAL_PERSON_NUM ; i++ )
	 //{

		// if( m_PvPRoleInfo[i].eState != E_PvP_RoleStateNull )
		// {  
		//	 Role* pRole = g_roleMgr.GetRolePtrByID( m_PvPRoleInfo[i].dwRoleID );
		//	 if( P_VALID(pRole) )
		//	 {
		//		 pRole->SendMessage(&Msg,Msg.dwSize); 
		//		
		//		 TitleMgr* pTitle = pRole->GetTitleMgr();
		//		 if( P_VALID(pTitle))
		//		 {
		//			 pTitle->SigEvent(ETE_WUXUN_LEVEL,GT_INVALID,pRole->GetAttValue(ERA_WuXun));
		//		 }
		//	 }

		// }

	 //}

	 
	 return TRUE;
}