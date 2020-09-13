//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_pvp.cpp
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: 竞技场房间
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "role.h"

#include "role_mgr.h"
#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"


#include "athletics_room.h"
#include "athletics_pvp.h"



AthleticsPvP::AthleticsPvP():m_dwMapID(0),m_dwInstanceID(0),m_dwRedSidePersonNum(0),m_dwBlueSidePersonNum(0),m_bIsClosePvP(FALSE)
{
   
	m_dwLeftTime     =  PVP_LEFT_TIME;
	m_dwCountDown    =  PVP_COUNT_DOWNTIME;
	m_bPvPOver       =  FALSE;

	m_dwPvPTime      =  0;
	m_dwSerialId     =  0;
	m_dwPvPPersonNum =  0;
	
                   
}

AthleticsPvP::~AthleticsPvP()
{

}
 DWORD dwFirstTime = 0;
BOOL AthleticsPvP::Init(DWORD dwSerialId,DWORD dwInstanceID,const RoomBase* pRoom)
{
	if( !P_VALID(pRoom))
	{
		return FALSE ;
	}
    m_dwMapID                             = 0;
	m_dwInstanceID                        = dwInstanceID;
	Athletics3V3Room* p3v3Room            = NULL;
    if ( pRoom->GetRoomType() == E_PvP_Room3v3)
    {
          p3v3Room            = (Athletics3V3Room*)(const_cast<RoomBase*>(pRoom));
    }
	else
	{
		  ASSERT(0);
		  return FALSE;
	}
	m_dwSerialId                          = p3v3Room->GetRoomSerialID();
	dwFirstTime = timeGetTime();
	return TRUE;
}


VOID AthleticsPvP::Update()
{
	 
			// 	if ( timeGetTime() - dwFirstTime > 1000 )
			// 	{
			// 		 m_dwPvPTime++; 
			// 		 dwFirstTime = timeGetTime();
			// 	}
}

VOID AthleticsPvP::Destroy()
{
	
     
}


