#include "stdafx.h"

#include "PvPMgr.h"
#include "../WorldDefine/athletics_define.h"
#include "PvPHall.h"
#include "PvPRoomList.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "PvPRoom.h"

PvPMgr::PvPMgr()
{	
}

PvPMgr::~PvPMgr()
{
	for(std::map<E_PvP_RoomType, PvPHall*>::iterator itor = m_hallMap.begin(); itor != m_hallMap.end(); itor++)
	{
		delete itor->second;
	}
	m_hallMap.clear();
}

PvPMgr* PvPMgr::Inst()
{
	static PvPMgr _instance;
	return &_instance;
}

PvPRoomList* PvPMgr::GetRoomList(E_PvP_RoomType hallType, E_PvP_RoomLevel level)
{
	if(hallType == E_PvP_RoomTypeNull || level == E_PvP_RoomLevelNull) return NULL;
	return m_hallMap[hallType]->GetRoomList(level);
}

PvPRoom* PvPMgr::GetRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomID)
{
	if(hallType == E_PvP_RoomTypeNull || level == E_PvP_RoomLevelNull) return NULL;
	return m_hallMap[hallType]->GetRoomList(level)->GetRoom(roomID);
}

INT	PvPMgr::Wuxun2Level(DWORD wuXun)const
{
	//return INT(wuXun / 200) + 1;
	DWORD level = 1;
	for(std::map<DWORD, tagWuXunLevel>::const_iterator itor = m_wuXunMap.begin(); itor != m_wuXunMap.end(); itor++)
	{
		if(itor->second.dwWuXunLowerBound <= wuXun) level = max(level, itor->first);
	}
	return level;
}

BOOL PvPMgr::CanEnterHall(E_PvP_RoomLevel eType)const
{
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pPlayer)) return FALSE;
	switch(eType)
	{
	case E_PvP_Junior:
		//return Wuxun2Level(pPlayer->GetAttribute(ERA_WuXun)) <= 3; // 新手场只有1～3级别可以进
		//break;
	case E_PvP_Middle:
		return pPlayer->GetRoleLevel() >= 30;	// 进入普通场等级必须大于30
		break;
	case E_PvP_Senior:
		return Wuxun2Level(pPlayer->GetAttribute(ERA_WuXun)) >= 6; // 高级场只有 6～10级可以进
		break;
	}
	return FALSE;
}

void PvPMgr::SetRoomToRoleMap(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomID)
{
	m_roleNameMap.clear();
	PvPRoom* pRoom = GetRoom(hallType, level, roomID);
	if(P_VALID(pRoom))
	{
		for(BYTE i= 1; i<= pRoom->GetNumLimit(); i++)
		{
			PvPRoom::PRoleInfo pRoleInfo = pRoom->GetRoleInfoAtPos(i);
			if(P_VALID(pRoleInfo))
				m_roleNameMap.insert(std::pair<DWORD, tstring>(pRoleInfo->dwRoleID, tstring(pRoleInfo->szName)));
		}
	}
}


DWORD PvPMgr::Init()
{
	for(std::map<E_PvP_RoomType, PvPHall*>::iterator itor = m_hallMap.begin(); itor != m_hallMap.end(); itor++)
	{
		delete itor->second;
	}
	m_hallMap.clear();
	m_hallMap.insert(std::make_pair(E_PvP_Room3v3, new PvPHall(E_PvP_Room3v3)));
	m_hallMap.insert(std::make_pair(E_PvP_Room6v6, new PvPHall(E_PvP_Room6v6)));

	LPCTSTR szWuXunPro = _T("data\\system\\attdata\\arena_proto.xml");
	TObjRef<VarContainer> varWuXunProto = CreateObj("WuXunProto", "VarContainer");

	list<tstring> wuXunProtoFiledList;
	if(!varWuXunProto->Load("VFS_System", szWuXunPro, "Level", &wuXunProtoFiledList))
		IMSG(_T("Load file skill_proto.xml failed\r\n"));
	
	m_wuXunMap.clear();
	for(list<tstring>::iterator iter = wuXunProtoFiledList.begin(); iter != wuXunProtoFiledList.end(); ++iter)
	{
		tagWuXunLevel pro;
		pro.dwLevel = _ttoi((*iter).c_str());
		pro.dwWuJiMax = varWuXunProto->GetDword(_T("WuJi_Max"), (*iter).c_str(), 0);
		pro.dwWuXunLowerBound = varWuXunProto->GetDword(_T("WuXun"), (*iter).c_str(), 0);
		m_wuXunMap.insert(std::pair<DWORD, tagWuXunLevel>(pro.dwLevel, pro));
	}
	KillObj("WuXunProto");
	return TRUE;
}