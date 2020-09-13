//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_system.cpp
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: 竞技场房间管理器
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "role.h"

#include "role_mgr.h"
#include "map_mgr.h"
#include "map_creator.h"

#include "../WorldDefine/time.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"

#include "athletics_PvP_define.h"
#include "athletics_room.h"
#include "athletics_pvp.h"
#include "athletics_room_mgr.h"
#include "athletics_pvp_mgr.h"
#include "athletics_system.h" 

AthleticsSystem g_AthleticsSystem;
DWORD  AthleticsSystem::m_dwPvPState = 0;
AthleticsSystem::AthleticsSystem():m_MapID1(ATHLETICS_ENTER_SPOT1),m_MapID2(ATHLETICS_ENTER_SPOT2),m_MapID3(ATHLETICS_ENTER_SPOT3),m_dwBuffID(ATHLETICS_BUFF_ID),m_pPvPMgr(NULL),m_IsSaved(FALSE)
{
	   m_dwPvPState  = 0;
	   m_dwHallPersonNumTemp = 0;
	   m_dwHallRoomNumTemp = 0;
	   m_dwFightingRoomNumTemp = 0;
     
    
}


AthleticsSystem::~AthleticsSystem() 
{
    if (P_VALID(m_pPvPMgr))
    {
		SAFE_DEL(m_pPvPMgr);
    }
}


BOOL AthleticsSystem::Init()  
{
	TObjRef<VarContainer> pVar = "VarContainer";
    m_dwPvPState  =  pVar->GetDword(_T("on arena")); 
    
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("openarena"), m_Trunk.sfp1(&AthleticsSystem::OpenArenaNetCmd), _T("Turn on ArenaSystemCmd"));
	pConsole->Register(_T("closearena"),m_Trunk.sfp1(&AthleticsSystem::CloseArenaNetCmd), _T("Turn off ArenaSystemCmd"));

    m_pPvPMgr = new PvPMgr;
    if ( !P_VALID(m_pPvPMgr))
    {
		return FALSE;
    }
	BOOL bInitSuccess = m_RoomMgr.Init();
	if( !bInitSuccess )
	{
		return FALSE;
	}
	bInitSuccess = m_pPvPMgr->Init();
	if( !bInitSuccess )
	{
		return FALSE;
	}

	m_pSystemInfo  = new  tagPvPSystemInfo;
	if (!P_VALID(m_pSystemInfo))
	{
		return FALSE;
	}

	m_pSystemData = new tagPvPSystemData;
	if( !P_VALID( m_pSystemData ))
	{
		return FALSE;
	}

	memset(m_pSystemInfo,0,sizeof(tagPvPSystemInfo));
	m_pSystemInfo->dwSerialVal = 1;
	memset(m_pSystemData,0,sizeof(tagPvPSystemData));
	
	return TRUE;
}

VOID AthleticsSystem::Destroy() 
{
	if ( P_VALID(m_pSystemData))
	{
		SAFE_DEL(m_pSystemData);
	}

	if (P_VALID(m_pSystemInfo))
	{
		SAFE_DEL(m_pSystemInfo);
	}
	 m_pPvPMgr->Destroy();
     m_RoomMgr.Destroy();

}

VOID AthleticsSystem::Update()
{
	
CHECK_TIME()

    



   m_RoomMgr.Update();
	
    

	TList<RoomBase*>& listTemp = GetRoomMgr().GetEnterPvPRoomDequeue();
	if( !listTemp.Empty())
	{
		RoomBase* pRoom =  NULL;
		while( !listTemp.Empty() )
		{
			//这里从队列头里取出 开战状态的房间，创建竞技场副本，要删除房间千万别在这里释放房间内存
			pRoom = listTemp.PopFront();
			if ( P_VALID( pRoom )  )
			{
				//--------------------------------------------------------------------------
				//SM 竞技场的最近修改
				//修改该部分功能
				//--------------------------------------------------------------------------
				 				PvPMgr* pPvpMgr = GetPvPMgrPtr();
				 				if( P_VALID(pPvpMgr) )
				 				{
									if( !pPvpMgr->CreatePvP(pRoom))
									{
										pRoom->SetRoomState(E_PvP_RoomStateNull);
										pRoom->SetIsPvPRoleEnterPvP(FALSE);
										continue;
									}
				 
				 				}

				//--------------------------------------------------------------------------
				//SM 竞技场的最近修改
				//屏蔽该部分功能
				//--------------------------------------------------------------------------
				// 				WorldScript* pScript = g_ScriptMgr.GetWorldScript();
				// 				if(!P_VALID(pScript))
				// 				{
				//                     pRoom->SetRoomState(E_PvP_RoomStateNull);
				// 					pRoom->SetIsPvPRoleEnterPvP(FALSE);
				// 					continue;
				// 				}
				// 
				// 				if (pRoom->GetRoomType() == E_PvP_Room3v3)
				// 				{
				// 					Athletics3V3Room* p3v3Room = (Athletics3V3Room*)pRoom;
				// 					DWORD nPlayerRoleID[P3v3ROOM_TOTAL_PERSON_NUM] = {0};
				// 
				// 					for (int nCnt = 0; nCnt < P3v3ROOM_TOTAL_PERSON_NUM; nCnt++)
				// 					{
				// 						//GetPosRoleInfo的参数从1开始，所以nCnt要加1
				// 						tagPvPRoleInfo& pPvPRoleInfo = p3v3Room->GetPosRoleInfo(nCnt+1);
				// 						nPlayerRoleID[nCnt] = pPvPRoleInfo.dwRoleID;
				// 
				// 						if ( P_VALID(pPvPRoleInfo.dwRoleID) )
				// 						{
				// 							Role* pRole = g_roleMgr.GetRolePtrByID(pPvPRoleInfo.dwRoleID);
				// 							tagNS_PvPEnterPvP msg;
				// 							if (P_VALID( pRole))
				// 							{
				// 								pRole->SendMessage(&msg,msg.dwSize);
				// 							}
				// 						}
				// 					}
				//        			    pScript->OnCreatePvPMap3v3(NULL,NULL,p3v3Room->GetRoomSerialID(),nPlayerRoleID,P3v3ROOM_TOTAL_PERSON_NUM); 
				// 				}
				// 				else
				// 				{
				// 					pRoom->SetRoomState(E_PvP_RoomStateNull);
				// 					pRoom->SetIsPvPRoleEnterPvP(FALSE);
				// 					continue;
				// 				}

			}

		}

	}

	m_pPvPMgr->Update();
    

			// 	if ( m_pPvPMgr->GetHaveRoomDestroyed() )
			// 	{
			// 		 DWORD dwPvPTime = m_pPvPMgr->GetEveryPvPTime();
			//          if ( dwPvPTime < 120)
			//          {
			// 			 m_pSystemData->n16Below120Sec++;
			//          }
			// 		 else if ( dwPvPTime < 210)
			// 		 {
			// 			 m_pSystemData->n16Below210Sec++;
			// 		 }
			// 		 else if( dwPvPTime < 300)
			// 		 {
			// 			 m_pSystemData->n16Below300Sec++;
			// 		 }
			// 		 else
			// 		 {
			// 			 m_pSystemData->n16Over300Sec++;
			// 		 }
			// 		 m_pPvPMgr->SetHaveRoomDestroyed(FALSE);
			// 		 m_pPvPMgr->SetEveryPvPTime(0);
			// 	}
			// 
			// 	const tagDWORDTime&  tCurrentTime = GetCurrentDWORDTime();
			// 	if ( tCurrentTime.min == 0 || tCurrentTime.min == 30)
			// 	{
			// 		if( !m_IsSaved )
			// 		{
			//            SaveSystemDataToDB(tCurrentTime);
			//            m_IsSaved = TRUE;
			// 		}
			// 		
			// 	}
			// 	else
			// 	{
			//            m_IsSaved = FALSE;
			// 	}
} 

VOID AthleticsSystem::SaveSystemDataToDB(const tagDWORDTime&  CurrentTime)
{
     tagNDBC_SaveArenaDataToDB  Msg;
	 
	 Msg.tPvPSystemData.dwSerialId        = m_pSystemInfo->dwSerialVal;
     Msg.tPvPSystemData.dwHallPersonNum   = m_RoomMgr.GetTotalPersonNumber();
	 Msg.tPvPSystemData.dwFightRoomNum    = m_pPvPMgr ? m_pPvPMgr->GetPvPNum() : 0;
	 Msg.tPvPSystemData.dwHallRoomNum     = m_RoomMgr.GetTotalRoomNumber();
	 Msg.tPvPSystemData.saveTime          = CurrentTime;
	 Msg.tPvPSystemData.n16Below120Sec    = m_pSystemData->n16Below120Sec;
	 Msg.tPvPSystemData.n16Below210Sec    = m_pSystemData->n16Below210Sec;
	 Msg.tPvPSystemData.n16Below300Sec    = m_pSystemData->n16Below300Sec;
	 Msg.tPvPSystemData.n16Over300Sec     = m_pSystemData->n16Over300Sec;

	 Msg.tPvPSystemInfo.n16SystemId       = 1;
	 Msg.tPvPSystemInfo.dwSerialVal       = ++m_pSystemInfo->dwSerialVal;
	 g_dbSession.Send(&Msg,Msg.dwSize);
	 memset(m_pSystemData,0,sizeof(tagPvPSystemData));
     m_dwHallPersonNumTemp   = 0;
	 m_dwHallRoomNumTemp     = 0;
	 m_dwFightingRoomNumTemp = 0;

	 tagNDBC_SaveSerialValToDB MsgSerialVal;
	 MsgSerialVal.dwSerialId  = 1 ;
	 MsgSerialVal.dwSerialVal = m_pSystemInfo->dwSerialVal;
	 g_dbSession.Send(&MsgSerialVal,MsgSerialVal.dwSize);
}


VOID  AthleticsSystem::SetSystemInfoSerialVal(DWORD  dwSerialVal) 
{
	   m_pSystemInfo->dwSerialVal = dwSerialVal;
}


