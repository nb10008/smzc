//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: team.h
// author: wjiang
// actor:
// data: 2008-12-10
// last:
// brief: 脚本通用消息流管理类
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/ScriptMsgInfo.h"
#include "mutex.h"

class Role;
class Team;
class Guild;

class MsgInfoMgr
{
public:
	MsgInfoMgr():m_dMsgInfoID(1) {}
	~MsgInfoMgr();

	BOOL	Init() { return TRUE; }

	DWORD	BeginMsgEvent();
	VOID	AddMsgEvent(DWORD dwMsgInfoID, EMsgUnitType eMsgUnitType, LPVOID pData);
	VOID	DispatchRoleMsgEvent(DWORD dwMsgInfoID, Role *pRole);
	VOID	DispatchWorldMsgEvent(DWORD dwMsgInfoID);
	VOID	DispatchMapMsgEvent(DWORD dwMsgInfoID, Map* pMap);
	VOID	DispatchTeamMapMsgEvent(DWORD dwMsgInfoID, Map *pMap, Team* pTeam);
	VOID	DispatchGuildMsgEvent(DWORD dwMsgInfoID, Guild* pGuild);
	// Jason 谨慎使用，否则宕机不管
	VOID	GetDispatchMsg(DWORD dwMsgInfoID,BYTE * & pData,INT & size);
	VOID	DiscardMsg(DWORD dwMsgInfoID,LPVOID pData);
private:
	VOID	RemoveMsgInfo(MsgInfo* pInfo)	{ m_mapMsgInfo.Erase(pInfo->GetMsgID()); SAFE_DEL(pInfo); }
private:
	TSafeMap<DWORD, MsgInfo*>	m_mapMsgInfo;
	DWORD					m_dMsgInfoID;
	Mutex					m_Lock;	
};

extern MsgInfoMgr g_MsgInfoMgr;