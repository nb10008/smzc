//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_pvp.h
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: 竞技场
//-----------------------------------------------------------------------------

#pragma once

struct tagAthleticsPvPRoomInfo;
struct tagPvPRoomRoleInfo;
class RoomBase;
class Athletics3V3Room;

const DWORD  PVP_LEFT_TIME  =  60*10 ;      // 竞技时间 10分钟
const DWORD  PVP_COUNT_DOWNTIME = 15 ;      // 倒计时15秒
class AthleticsPvP 
{
public:
   AthleticsPvP();
   ~AthleticsPvP();

   BOOL Init(DWORD dwSerialId,DWORD dwInstanceID,const RoomBase* pRoom);
   VOID Update();
   VOID Destroy();

   DWORD GetMapID()const                 { return  m_dwMapID;     }
   DWORD GetInstanceID()const            { return  m_dwInstanceID;}
   
   DWORD GetRoomSerialID()const          { return  m_dwSerialId;}
   VOID  ClosePvP(BOOL bIsClosePvP)      { m_bIsClosePvP = bIsClosePvP; }
   BOOL  GetIsClosePvP()const            { return  m_bIsClosePvP; }
   

   DWORD GetPvPTime()const               { return  m_dwPvPTime; }

private:
	
	DWORD                                  m_dwMapID;
	DWORD                                  m_dwInstanceID;

    DWORD                                  m_dwRedSidePersonNum;
	DWORD                                  m_dwBlueSidePersonNum;
    
    
	DWORD                                  m_dwLeftTime ;        // 剩余时间
	BOOL                                   m_bPvPOver;           //竞技游戏结束
	DWORD                                  m_dwCountDown;        //倒计时
	BOOL                                   m_bCountDown;         
	BOOL                                   m_bIsClosePvP;

	DWORD                                  m_dwPvPTime;            //竞技场时间单位：秒
    DWORD                                  m_dwSerialId;           //房间的序列号  
	DWORD                                  m_dwPvPPersonNum;
};