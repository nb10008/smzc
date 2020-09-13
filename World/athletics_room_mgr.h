//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_room_mgr.h
// author: zxzhang
// actor:
// data: 2010-2-25
// last:
// brief: 竞技场房间管理器
//-----------------------------------------------------------------------------

#pragma once

struct tagPvPRoom;
struct tagAthRoomId;
class  Mutex;

//-----------------------------------------------------------------------------
// 玩家所在的房间 
//-----------------------------------------------------------------------------
struct tagPvPRoleInRoom
{
	DWORD      dwRoleID;
    DWORD      dwSerialID;

};

//-----------------------------------------------------------------------------
// 竞技房间管理器类
//-----------------------------------------------------------------------------
class AthleticsRoomMgr
{

public:
	//-------------------------------------------------------------------------
	//Constructor &  Destructor
	//-------------------------------------------------------------------------
	AthleticsRoomMgr();
    ~AthleticsRoomMgr();
    
	//-------------------------------------------------------------------------
	//Initialization function
	//-------------------------------------------------------------------------
	BOOL Init();
    BOOL Destroy();
	VOID Update();

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//增加该部分功能 
	//--------------------------------------------------------------------------
	DWORD AddToHall(E_PvP_RoomType eRoomType,DWORD dwRoleID);
	DWORD LeaveHall(DWORD dwRoleID);

	BOOL  IsRoleExist(DWORD dwRoleID);
	//--------------------------------------------------------------------------
	//结束
	//--------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	//SM 竞技场的最近修改
	//增加该部分功能 
	//--------------------------------------------------------------------------
	//竞技场活动结束
	VOID AthleticsActiveOver();

	//---------------------------------------------------------------------------------------
	//创建房间和删除房间
	//---------------------------------------------------------------------------------------
    DWORD CreateAthleticsRoom(const tagPvPRoom& pvpRoom);
	BOOL  DeleteAthleticsRoom(DWORD dwSerialID,RoomBase* pRoom);
    
	//---------------------------------------------------------------------------------------
	//创建大厅房间列表和删除大厅房间列表
	//---------------------------------------------------------------------------------------
	
	BOOL AddListRoom(const tagPvPRoom& pvpRoom,DWORD dwSerialId);
    BOOL DeleteListRoom(E_PvP_RoomType eRoomType,E_PvP_RoomLevel eRoomLevel,INT16 nRoomId); 
     

	//---------------------------------------------------------------------------------------
	//获得总的房间个数
	//---------------------------------------------------------------------------------------
	DWORD GetTotalRoomNumber()const
	{
       return m_dwTotalRoomNum;
	}
    
	//---------------------------------------------------------------------------------------
	//获得总的大厅人数
	//---------------------------------------------------------------------------------------
    INT GetTotalPersonNumber()
	{
		return m_MapRoleInAthletisc.Size();
	}

	//---------------------------------------------------------------------------------------
	//获得初级房间个数
	//---------------------------------------------------------------------------------------
	DWORD GetTotalJuniorRoomNumber()const
	{
		return m_dwTotalJuniorRoomNum;
	}

	//---------------------------------------------------------------------------------------
	//获得中级房间个数
	//---------------------------------------------------------------------------------------
	DWORD GetTotalInterMediateRoomNumber()const
	{
		return m_dwTotalInterMediateRoomNum;
	}
    
	

	//---------------------------------------------------------------------------------------
	//获得高级的房间个数
	//---------------------------------------------------------------------------------------
	DWORD GetTotalSeniorRoomNumber()const
	{
		return m_dwTotalSeniorRoomNum;
	}

	//---------------------------------------------------------------------------------------
	//获得有效的房间号
    //---------------------------------------------------------------------------------------
    DWORD GetAvailRoomId(E_PvP_RoomLevel eRoomLevel);
	
	//---------------------------------------------------------------------------------------
	//设置房间号的状态
    //---------------------------------------------------------------------------------------
	VOID  SetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId,E_RoomId_State eRoomIdState);
	
	//---------------------------------------------------------------------------------------
	//获得房间号的状态
	//---------------------------------------------------------------------------------------
	tagAthRoomId*  GetRoomIdState(E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId);

	//---------------------------------------------------------------------------------------
	//获得房间的指针
	//---------------------------------------------------------------------------------------
    RoomBase*     GetPvPRoom(DWORD dwSerialID);
    RoomBase*     GetPvPRoom(DWORD dwRoleID ,E_PvP_RoomLevel eRoomLevel,DWORD dwRoomId); 
	//---------------------------------------------------------------------------------------
	//把大厅的房间信息发给客户端
	//---------------------------------------------------------------------------------------
	VOID SendHallRoomInfoToClient();

	//---------------------------------------------------------------------------------------
	//把房间的具体信息发给客户端;
	//---------------------------------------------------------------------------------------
	VOID    SendRoomInfoToClient();
    DWORD   RoleJoinInRoom(DWORD dwRoleID,E_PvP_RoomLevel eRoomLevel,INT16 nRoomID);
	DWORD   KickRoleToHall(DWORD dwRoleID,BOOL bSendMsg = TRUE);
	DWORD   RoleLeaveRoom(DWORD dwRoleID);
    tagPvPRoleInRoom* GetInRoomRole(DWORD dwRoleID);
	VOID    AddRoleToAthletics(const tagPvPRoleInRoom* pRoleInRoom);
	BOOL    DeleteRoleFromAthletics(DWORD dwRoleID);

	VOID    GetHallRoomId(INT16 nRoomNum, E_PvP_RoomLevel eRoomLevel,/*输出*/INT16 nRoomID[]);
	tagPvPRoom* GetHallRoomInfo(INT16 nRoomID, E_PvP_RoomLevel eRoomLevel);
	VOID    GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*输出*/tagPvPRoom& pvpRoom);
	VOID    GetHallRoomInfo(INT16 nRoomID,E_PvP_RoomLevel eRoomLevel,/*输出*/PBYTE& pData);
    VOID    OnOffLine(DWORD dwRoleID,BOOL bIsSend = TRUE);
    VOID    AddPvPRoomToPvPQueue(RoomBase* pRoom);
    BOOL    AddRoleToRoom(DWORD dwSerialId,DWORD dwRoleID1,DWORD dwRoleID2,DWORD dwRoleID3,DWORD dwRoleID4,DWORD dwRoleID5,DWORD dwRoleID6);
    TMap<INT16, tagPvPRoom*>&   GetMapPvPRoomInfo(E_PvP_RoomLevel eRoomLevel);


private:
	//---------------------------------------------------------------------------------------------
	//大厅的房间信息列表
	//---------------------------------------------------------------------------------------------
	typedef TMap<INT16, tagPvPRoom*>	        TMapJuniorAthleticsRoom;       //初级房间表
	typedef TMap<INT16, tagPvPRoom*>	        TMapInterMediateAthleticsRoom; //中级房间表
	typedef TMap<INT16, tagPvPRoom*>	        TMapSeniorAthleticsRoom;       //高级房间表


	//---------------------------------------------------------------------------------------------
	//标记各个级别房间号列表
	//---------------------------------------------------------------------------------------------
	typedef TList<tagAthRoomId*>                ListJuniorAthRoomId;             //标记初级房间是否被创建
	typedef TList<tagAthRoomId*>                ListInterMediateAthRoomId;       //标记中级房间是否被创建
	typedef TList<tagAthRoomId*>                ListSeniorAthRoomId;             //标记高级房间是否被创建

	//---------------------------------------------------------------------------------------------
	//房间列表
	//---------------------------------------------------------------------------------------------     
    typedef TMap<DWORD, RoomBase*>	            MapAthleticsRoom;             //总的更新列表
	typedef TList<RoomBase*>                    ListEnterPvPRoom;             //进入竞技场的房间队列   //不能释放房间内存

	//---------------------------------------------------------------------------------------------
	//进入大厅 角色列表
	//---------------------------------------------------------------------------------------------    
    typedef TMap<DWORD, tagPvPRoleInRoom*>	                MapRoleInAthletisc;           //进入竞技的角色列表
public:
	   //---------------------------------------------------------------------------------------
	   //获得进入竞技场的房间队列
	   //---------------------------------------------------------------------------------------
	   ListEnterPvPRoom& GetEnterPvPRoomDequeue()
	   {
		   return m_ListEnterPvPRoom;
	   }
private:
	//Mutex                                       m_Mutex;
	
	//---------------------------------------------------------------------------------------------
	//大厅的房间信息列表
	//---------------------------------------------------------------------------------------------
	TMapJuniorAthleticsRoom                     m_MapJuniorAthleticsRoom;
    TMapInterMediateAthleticsRoom               m_MapInterMediateAthleticsRoom;
    TMapSeniorAthleticsRoom                     m_MapSeniorAthleticsRoom;
    
	//---------------------------------------------------------------------------------------------
	//标记各个级别房间号列表
	//---------------------------------------------------------------------------------------------
    ListJuniorAthRoomId                         m_ListJuniorAthRoomId;
    ListInterMediateAthRoomId                   m_ListInterMediateAthRoomId;
    ListSeniorAthRoomId                         m_ListSeniorAthRoomId;   
	
	//---------------------------------------------------------------------------------------------
	//房间列表
	//---------------------------------------------------------------------------------------------
	MapRoleInAthletisc                          m_MapRoleInAthletisc;   
    MapAthleticsRoom                            m_MapAthleticsRoom; 
    ListEnterPvPRoom                            m_ListEnterPvPRoom;

	DWORD                                       m_dwTotalRoomNum;
	DWORD                                       m_dwTotalJuniorRoomNum;
	DWORD                                       m_dwTotalInterMediateRoomNum;
	DWORD                                       m_dwTotalSeniorRoomNum;
	static    DWORD                             m_sdwRoomSerialID;           
};