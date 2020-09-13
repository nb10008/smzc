//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_pos.h
// author: Sxg
// actor:
// data: 2009-4-20
// last:
// brief: 帮派职位管理
//-------------------------------------------------------------------------------
#pragma once

#if 0
#include "guild_tang.h"
#else
#include "../WorldDefine/guild_define.h"
#endif
//-------------------------------------------------------------------------------
class GuildPos
{
public:
	GuildPos();
	~GuildPos();

	VOID	Init(BYTE byGuildLevel);

	BOOL	IsMemberFull() const;
	BOOL	IsPosFull(EGuildMemberPos eGuildPos);
	DWORD	AddMember(DWORD dwRoleID, EGuildMemberPos eGuildPos);
	VOID	RemoveMember(DWORD dwRoleID, EGuildMemberPos eGuildPos);
	DWORD	ChangePos(DWORD dwRoleID, EGuildMemberPos eOldGuildPos, EGuildMemberPos eNewGuildPos);
	DWORD	SpecGuildPosLimit(DWORD dwRoleID, EGuildMemberPos eGuildPos);
	INT32	GetNumber() const;

	INT8	GetChairManNum() const {return m_n8CurNumChairman;}
	INT16	GetJingYingAndSimpleNum() const {return m_n16CurNumJingYingAndSimple;}
	INT8	GetBackBoneNum() const {return m_n8CurBackBoneMember;}

	INT16	GetPosNum(EGuildMemberPos eGuildPos) const;
	VOID	IncPosNum(EGuildMemberPos eGuildPos,INT a= 1);

private:
	BOOL	IsChairmanFull() const;
	BOOL	IsViceChairmanFull() const;
	BOOL	IsJingYingAndSimpleFull() const;
	BOOL	IsBackBoneFull() const;

	VOID	RemoveChairman(DWORD dwRoleID);
	VOID	RemoveViceChairman(DWORD dwRoleID);
	VOID	RemoveJingYingOrSimple(DWORD dwRoleID);
	VOID	RemoveBackStone(DWORD dwRoleID);

	DWORD	AddChairman(DWORD dwRoleID);
	DWORD	AddViceChairman(DWORD dwRoleID);
	DWORD	AddJingYingOrSimple(DWORD dwRoleID);
	DWORD	AddBackStone(DWORD dwRoleID);

private:
#if 0 //《圣魔之血》里面没有堂了
	Tang		m_Tang[X_GUILD_TANG_NUM];
#endif
	// 考虑记录相应职位的roleid，当前系统还没有必要

	// 辅助属性
	INT8		m_n8CurNumChairman;
	INT8		m_n8CurNumViceChairman;

	INT16		m_n16CurNumJingYingAndSimple;
	INT16		m_n16MaxNumJingYingAndSimple;

	INT8		m_n8CurBackBoneMember;
	INT8		m_n8MaxBackBoneMember;

	INT16		m_n16CurNumMember;
	INT16		m_n16MaxNumMember;
};


/*************************** 内联实现 *******************************/

inline INT32 GuildPos::GetNumber() const
{
	return m_n16CurNumMember;
}

inline BOOL GuildPos::IsMemberFull() const
{
	return m_n16CurNumMember >= m_n16MaxNumMember;
}

inline BOOL GuildPos::IsChairmanFull() const
{
	return m_n8CurNumChairman >= MAX_GUILDPOS_BANGZHU;
}

inline BOOL GuildPos::IsViceChairmanFull() const
{
	return m_n8CurNumViceChairman >= MAX_GUILDPOS_FUBANGZHU;
}

inline BOOL GuildPos::IsJingYingAndSimpleFull() const
{
	return m_n16CurNumJingYingAndSimple >= m_n16MaxNumJingYingAndSimple;
}

inline BOOL GuildPos::IsBackBoneFull() const
{
	return m_n8CurBackBoneMember >= m_n8MaxBackBoneMember;
}

inline DWORD GuildPos::ChangePos(DWORD dwRoleID, EGuildMemberPos eOldGuildPos, EGuildMemberPos eNewGuildPos)
{
	RemoveMember(dwRoleID, eOldGuildPos);
	return AddMember(dwRoleID, eNewGuildPos);
}