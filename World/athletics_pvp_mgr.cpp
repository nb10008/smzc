//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_pvp_mgr.cpp
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: 竞技场房间
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "role.h"

#include "role_mgr.h"
#include "map_mgr.h"
#include "map_creator.h"

#include "athletics_PvP_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"


#include "athletics_room.h"
#include "athletics_pvp.h"
#include "athletics_pvp_mgr.h"


PvPMgr::PvPMgr():m_MapID(ATHLETICS_MAPID),m_pMapMgr(NULL),m_bHaveRoomDestroyed(FALSE),m_dwEveryPvPTime(0)
{
	//m_MapID = 0;
	//dwInstanceID = 0;
	m_MapAthleticsPvP.Clear();
}

PvPMgr::~PvPMgr()
{


}

BOOL PvPMgr::Init()
{
	m_pMapMgr = g_mapCreator.GetMapMgr(m_MapID);

	return TRUE;

}

VOID PvPMgr::Update()
{
	AthleticsPvP* pTemp = NULL;
	TMapAthleticsPvP::TMapIterator it = m_MapAthleticsPvP.Begin();
	while( m_MapAthleticsPvP.PeekNext(it,pTemp))
	{
          if( P_VALID(pTemp))
		  {
			  //pTemp->Update();
			  if( pTemp->GetIsClosePvP())
			  {
                  m_bHaveRoomDestroyed = TRUE;
				  m_dwEveryPvPTime     = pTemp->GetPvPTime();
				  DeletePvP(pTemp->GetRoomSerialID());

			  }
		  }
	}

}

VOID PvPMgr::Destroy()
{

	AthleticsPvP* pTemp = NULL;
	TMapAthleticsPvP::TMapIterator it = m_MapAthleticsPvP.Begin();
	while( m_MapAthleticsPvP.PeekNext(it,pTemp))
	{
		if( P_VALID(pTemp))
		{
			pTemp->Destroy();
			SAFE_DEL(pTemp);
		}
	}
    m_MapAthleticsPvP.Clear();  
}


MapInstance* PvPMgr::CreateMapInstance()
{
     if ( P_VALID(m_pMapMgr))
	 {
		 MapInstance* pMapInstance = m_pMapMgr->CreateInstance(NULL,GT_INVALID);
		 if( P_VALID(pMapInstance) )
		 {
             return pMapInstance;
		 }
	 }
	 return NULL;
}

BOOL PvPMgr::CreatePvP(const RoomBase* pAthleticsRoom)
{
     if( !P_VALID(pAthleticsRoom) )
	 {
		 return FALSE;
	 }
//      MapInstance* pMapInstanceTemp = CreateMapInstance();
// 	 if( !P_VALID(pMapInstanceTemp) )
// 	 {
// 		 return FALSE;
// 	 }

	 AthleticsPvP* pAthleticsPvP = new AthleticsPvP;
	 if( !P_VALID(pAthleticsPvP) )
	 {
		 IMSG(_T("AthleticsPvP init failed Roleid:%d\r\n"), pAthleticsRoom->GetRoomSerialID());
		 SAFE_DEL(pAthleticsPvP);
		 return FALSE;
	 }
	 
     BOOL bPvPInit =  pAthleticsPvP->Init(pAthleticsRoom->GetRoomSerialID(),0,pAthleticsRoom);
	 if ( !bPvPInit)
	 {
		 return FALSE;
	 }
     m_MapAthleticsPvP.Add(pAthleticsRoom->GetRoomSerialID(),pAthleticsPvP);

	 const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
	 if (!pScript)
	 {
		 IMSG(_T("AthleticsPvP creates script event failed roomID:%d\r\n"), pAthleticsRoom->GetRoomSerialID());
         m_MapAthleticsPvP.Erase(pAthleticsRoom->GetRoomSerialID());
		 SAFE_DEL(pAthleticsPvP);
		 return FALSE;

	 }


	 if (pAthleticsRoom->GetRoomType() == E_PvP_Room3v3)
	 {
		 Athletics3V3Room* p3v3Room = (Athletics3V3Room*)pAthleticsRoom;
		 DWORD nPlayerRoleID[P3v3ROOM_TOTAL_PERSON_NUM] = {0};

		 for (int nCnt = 0; nCnt < P3v3ROOM_TOTAL_PERSON_NUM; nCnt++)
		 {
			 //GetPosRoleInfo的参数从1开始，所以nCnt要加1
			 tagPvPRoleInfo& pPvPRoleInfo = p3v3Room->GetPosRoleInfo(nCnt+1);
			 nPlayerRoleID[nCnt] = pPvPRoleInfo.dwRoleID;

			 if ( P_VALID(pPvPRoleInfo.dwRoleID) )
			 {
				 Role* pRole = g_roleMgr.GetRolePtrByID(pPvPRoleInfo.dwRoleID);
				 tagNS_PvPEnterPvP msg;
				 if (P_VALID( pRole))
				 {
					 pRole->SendMessage(&msg,msg.dwSize);
				 }
			 }
		 }

		 //添加脚本触发事件
		 pScript->OnCreatePvPMap3v3(NULL,NULL,(INT16)p3v3Room->GetRoomSerialID(),nPlayerRoleID,P3v3ROOM_TOTAL_PERSON_NUM); 
	 }
	 else
	 {
		 IMSG(_T("AthleticsPvP creates script event failed roomID:%d\r\n"), pAthleticsRoom->GetRoomSerialID());
		 m_MapAthleticsPvP.Erase(pAthleticsRoom->GetRoomSerialID());
		 SAFE_DEL(pAthleticsPvP);
		 return FALSE;
	 }

	  return TRUE;

}

BOOL PvPMgr::DeleteInstance(DWORD dwInstanceId)
{
     MapInstance* pMapInstanceTemp =  m_pMapMgr->GetInstance(dwInstanceId);
      if( P_VALID(pMapInstanceTemp) )
	  {
		  m_pMapMgr->DestroyInstance(pMapInstanceTemp);
		  return TRUE;
	  }
	  return FALSE;
}

BOOL  PvPMgr::DeletePvP(DWORD dwSerialID)
{
   AthleticsPvP* pAthleticsPvP = m_MapAthleticsPvP.Peek(dwSerialID);
   if( P_VALID(pAthleticsPvP))
   {
	   pAthleticsPvP->Destroy();
	   SAFE_DEL(pAthleticsPvP);
	   m_MapAthleticsPvP.Erase(dwSerialID);
	   return TRUE;
   }
   else
   {
	   return FALSE;
   }
}

AthleticsPvP*  PvPMgr::GetPvP(DWORD dwSerialID)
{
	  return  m_MapAthleticsPvP.Peek(dwSerialID);
}