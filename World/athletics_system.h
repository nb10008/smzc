//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_system.h
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: 竞技系统
//-----------------------------------------------------------------------------

#pragma once

enum E_PvP_State
{
	E_PvP_Off    = 0,
	E_PvP_On     = 1,
	E_PvP_State_End,
};


struct tagPvPSystemInfo;
struct tagPvPSystemData;
class PvPMgr;
class AthleticsRoomMgr;


//-----------------------------------------------------------------------------
// 竞技系统类
//-----------------------------------------------------------------------------
class AthleticsSystem
{
public:
	//------------------------------------------------------------------------
	//Constructor & Destructor
	//------------------------------------------------------------------------
	AthleticsSystem();
	~AthleticsSystem();
    
	//------------------------------------------------------------------------
	//Initialization of data
	//------------------------------------------------------------------------
	BOOL Init();
	VOID Destroy();
	VOID Update();

    
	AthleticsRoomMgr* GetRoomMgrPtr()    {    return &m_RoomMgr; }
 	AthleticsRoomMgr& GetRoomMgr()       {    return m_RoomMgr;  }
	PvPMgr*           GetPvPMgrPtr()     {    return m_pPvPMgr;  }
	PvPMgr&           GetPvPMgr()        {    return *m_pPvPMgr;   }
	DWORD             GetMapID1()const   {    return m_MapID1;   }
	DWORD             GetMapID2()const   {    return m_MapID2;   }
	DWORD             GetMapID3()const   {    return m_MapID3;   }
	DWORD             GetBuffID()const   {    return m_dwBuffID;}
	DWORD             GetPvPState()const {    return m_dwPvPState;}


	
	VOID              SaveSystemDataToDB(const tagDWORDTime&  CurrentTime);

	VOID              SetSystemInfoSerialVal(DWORD  dwSerialVal); 
	
private:
	AthleticsRoomMgr                           m_RoomMgr;
	PvPMgr*                                    m_pPvPMgr;
private:

	DWORD                                      m_MapID1;   //m04地图入口点
	DWORD                                      m_MapID2;   //a04地图入口点
	DWORD                                      m_MapID3;   
	DWORD                                      m_dwBuffID; //竞技场被晕的技能  
    static      DWORD                          m_dwPvPState;//  


	tagPvPSystemInfo*                           m_pSystemInfo;
    tagPvPSystemData*                           m_pSystemData;
private:

	TSFPTrunk<AthleticsSystem>		           m_Trunk;
	DWORD                              OpenArenaNetCmd(DWORD)  
	{
           m_dwPvPState = 1;
		   return 0 ;
	}
    DWORD                             CloseArenaNetCmd(DWORD)
	{	    
		    m_dwPvPState = 0;
		    return 0 ;
	}
	BOOL                                       m_IsSaved;
private:
	DWORD                                      m_dwHallPersonNumTemp;
	DWORD                                      m_dwHallRoomNumTemp;
	DWORD                                      m_dwFightingRoomNumTemp;
public:
	VOID                                       ModHallPersonNumTemp(DWORD  dwPersonNum)  {   m_dwHallPersonNumTemp += dwPersonNum; }
	VOID                                       ModHallRoomNumTemp(DWORD  dwRoomNum)      {   m_dwHallRoomNumTemp += dwRoomNum; }
	VOID                                       ModFightingRoomNumTemp(DWORD  dwFightingRoomNum)  {   m_dwFightingRoomNumTemp += dwFightingRoomNum; }
};



extern AthleticsSystem g_AthleticsSystem;