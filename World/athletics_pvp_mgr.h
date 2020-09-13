//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_pvp_mgr.h
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: 竞技场管理器
//-----------------------------------------------------------------------------

#pragma once




class PvPMgr
{
public:
	PvPMgr();
	~PvPMgr();

	BOOL Init();
	VOID Update();
	VOID Destroy();

	//DWORD GetMapID()const                { return  m_MapID;     }
	//DWORD GetInstanceID()const   { return  dwInstanceID;}
	MapInstance*      CreateMapInstance();
	BOOL              CreatePvP(const RoomBase* pAthleticsPvP);
	BOOL              DeleteInstance(DWORD dwSerialID);
	BOOL              DeletePvP(DWORD dwSerialID);
    AthleticsPvP*     GetPvP(DWORD dwSerialID);
    INT               GetPvPNum()                      {       return m_MapAthleticsPvP.Size();}
	BOOL              GetHaveRoomDestroyed()const      {    return m_bHaveRoomDestroyed;  }  
	VOID              SetHaveRoomDestroyed(BOOL bHaveRoomDestroyed) { m_bHaveRoomDestroyed = bHaveRoomDestroyed; }
	DWORD             GetEveryPvPTime()const           {    return m_dwEveryPvPTime;  }
	VOID              SetEveryPvPTime(DWORD dwPvPTime) {   m_dwEveryPvPTime =  dwPvPTime;  }
 
	DWORD             GetMapID()const    {    return m_MapID;    }
	MapMgr*           GetMapMgrPtr()const{    return m_pMapMgr;  }
private:
	typedef     TMap<DWORD,AthleticsPvP*>     TMapAthleticsPvP;
    
private:

    DWORD                                      m_MapID;  //竞技场副本地图ID
	MapMgr*                                    m_pMapMgr;
    TMapAthleticsPvP                           m_MapAthleticsPvP;
	BOOL                                       m_bHaveRoomDestroyed;   //是否有房间销毁 ，系统用
	DWORD                                      m_dwEveryPvPTime;       //缓存每个房间销毁时的时间

};

