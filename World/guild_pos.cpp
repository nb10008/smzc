//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_pos.cpp
// author: Sxg
// actor:
// data: 2009-4-20
// last:
// brief: 帮派职位管理
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_pos.h"
#include "role_mgr.h"
#include "../ServerDefine/role_data_define.h"
//-------------------------------------------------------------------------------
// 构造&析构
//-------------------------------------------------------------------------------
GuildPos::GuildPos()
{
	m_n8CurNumChairman	 = 0;
	m_n8CurNumViceChairman	 = 0;

	m_n16CurNumJingYingAndSimple	 = 0;
	m_n16MaxNumJingYingAndSimple	 = 0;

	m_n8CurBackBoneMember	 = 0;
	m_n8MaxBackBoneMember	 = 0;

	m_n16CurNumMember	 = 0;
	m_n16MaxNumMember	 = 0;
}

GuildPos::~GuildPos()
{
}

//-------------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------------
VOID GuildPos::Init(BYTE byGuildLevel)
{
#if 0  //《圣魔之血》里面没有堂了
	for(INT i=0; i<X_GUILD_TANG_NUM; ++i)
	{
		m_Tang[i].Init(byGuildLevel);
	}
#endif
	m_n16MaxNumJingYingAndSimple	= MGuildMaxMember(byGuildLevel);
	m_n8MaxBackBoneMember = MGuildBackBoneMember(byGuildLevel);
	m_n16MaxNumMember	= MGuildMaxMember(byGuildLevel)+MGuildBackBoneMember(byGuildLevel)+MAX_GUILDPOS_BANGZHU+MAX_GUILDPOS_FUBANGZHU;
}

//-------------------------------------------------------------------------------
// 指定职位是满员
//-------------------------------------------------------------------------------
BOOL GuildPos::IsPosFull(EGuildMemberPos eGuildPos)
{
	ASSERT(!IsMemberFull());
	ASSERT(::IsGuildPosValid(eGuildPos));
	
	// 帮内直属职位判断
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//公会会长
		return IsChairmanFull();

	case EGMP_FuHuiZhang:		//公会副会长
		return IsViceChairmanFull();

	case EGMP_GuGan:			//骨干
		return IsBackBoneFull();

	case EGMP_JingYing:	//精英
	case EGMP_HuiYuan:		//普通会员
		return IsJingYingAndSimpleFull();

	default:
		return TRUE;
	}

	// 没有进入上面流程返回，说明职位有问题
	return TRUE;
}

//-------------------------------------------------------------------------------
// 添加成员
//-------------------------------------------------------------------------------
DWORD GuildPos::AddMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(!IsPosFull(eGuildPos));

	DWORD dwErrorCode = GT_INVALID;
	BOOL  bRunDefault = FALSE;

	// 帮内直属职位判断
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//公会会长
		dwErrorCode = AddChairman(dwRoleID);
		break;
	case EGMP_FuHuiZhang:		//公会副会长
		dwErrorCode = AddViceChairman(dwRoleID);
		break;
	case EGMP_GuGan:			//骨干
		dwErrorCode = AddBackStone(dwRoleID);
		break;
	case EGMP_JingYing:	//精英
	case EGMP_HuiYuan:		//普通会员
		dwErrorCode = AddJingYingOrSimple(dwRoleID);
		break;

	default:
		bRunDefault = TRUE;
	}

	if(bRunDefault)
	{
		// 执行到此处，调用该函数前的逻辑有问题
		ASSERT(0);
	}

	if(E_Success == dwErrorCode)
	{
		++m_n16CurNumMember;
	}

	return dwErrorCode;
}

DWORD GuildPos::AddChairman(DWORD dwRoleID)
{
	ASSERT(!IsChairmanFull());
	++m_n8CurNumChairman;
	return E_Success;
}

DWORD GuildPos::AddViceChairman(DWORD dwRoleID)
{
	ASSERT(!IsViceChairmanFull());
	++m_n8CurNumViceChairman;
	return E_Success;
}

DWORD GuildPos::AddJingYingOrSimple(DWORD dwRoleID)
{
	ASSERT(!IsJingYingAndSimpleFull());
	++m_n16CurNumJingYingAndSimple;
	return E_Success;
}

DWORD GuildPos::AddBackStone(DWORD dwRoleID)
{
	ASSERT(!IsBackBoneFull());
	++m_n8CurBackBoneMember;
	return E_Success;
}

//-------------------------------------------------------------------------------
// 删除
//-------------------------------------------------------------------------------
VOID GuildPos::RemoveMember(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	ASSERT(m_n16CurNumMember >= 1);

	BOOL  bRunDefault = FALSE;
	
	// 帮内直属职位判断
	switch(eGuildPos)
	{
	case EGMP_HuiZhang:			//公会会长
		RemoveChairman(dwRoleID);
		break;
	case EGMP_FuHuiZhang:		//公会副会长
		RemoveViceChairman(dwRoleID);
		break;
	case EGMP_GuGan:			//骨干
		RemoveBackStone(dwRoleID);
		break;
	case EGMP_JingYing:	//精英
	case EGMP_HuiYuan:		//普通会员
		RemoveJingYingOrSimple(dwRoleID);
		break;

	default:
		bRunDefault = TRUE;
	}

	if(bRunDefault)
	{
		// 执行到此处，调用该函数前的逻辑有问题
		ASSERT(0);
		return;
	}

	--m_n16CurNumMember;
}

VOID GuildPos::RemoveChairman(DWORD dwRoleID)
{
	--m_n8CurNumChairman;
	ASSERT(m_n8CurNumChairman >= 0);
}

VOID GuildPos::RemoveViceChairman(DWORD dwRoleID)
{
	--m_n8CurNumViceChairman;
	ASSERT(m_n8CurNumViceChairman >= 0);
}

VOID GuildPos::RemoveJingYingOrSimple(DWORD dwRoleID)
{
	--m_n16CurNumJingYingAndSimple;
	ASSERT(m_n16CurNumJingYingAndSimple >= 0);
}

VOID GuildPos::RemoveBackStone(DWORD dwRoleID)
{
	--m_n8CurBackBoneMember;
	ASSERT(m_n8CurBackBoneMember >= 0);
}

//-------------------------------------------------------------------------------
// 职位特殊要求
//-------------------------------------------------------------------------------
DWORD GuildPos::SpecGuildPosLimit(DWORD dwRoleID, EGuildMemberPos eGuildPos)
{
	return E_Success;
}
INT16	GuildPos::GetPosNum(EGuildMemberPos eGuildPos) const
{
	switch( eGuildPos )
	{
	case EGMP_HuiZhang:
		return m_n8CurNumChairman;
	case EGMP_FuHuiZhang:
		return m_n8CurNumViceChairman;
	case EGMP_GuGan:
		return m_n16CurNumJingYingAndSimple;
	case EGMP_JingYing:
		return m_n8CurBackBoneMember;
	case EGMP_HuiYuan:
		return m_n16CurNumMember;
	}
	return 0;
}
VOID	GuildPos::IncPosNum(EGuildMemberPos eGuildPos,INT a )
{
	switch( eGuildPos )
	{
	case EGMP_HuiZhang:
		m_n8CurNumChairman += a;
	case EGMP_FuHuiZhang:
		m_n8CurNumViceChairman += a;
	case EGMP_GuGan:
		m_n16CurNumJingYingAndSimple += a;
	case EGMP_JingYing:
		m_n8CurBackBoneMember += a;
	case EGMP_HuiYuan:
		m_n16CurNumMember += a;
	}
}

