//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.h
// author: liutingting
// actor:
// data: 2011-01-19
// last:
// brief: 神职领域相关内容操作
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/msg_god.h"
#include "world.h"
#include "global_limited_item.h"

//-----------------------------------------------------------------------------
typedef TMap<DWORD,BOOL> MapArea;
class Role;

struct tagAreaInfo
{
	DWORD	dwAreaID;		//领域ID
	DWORD	dwCD;			//CD
	BOOL	bActive;		//是否已激活
};

typedef TMap<DWORD,tagAreaInfo*> MapAreaInfo;

class AreaMgr
{
public:
	AreaMgr(Role *pRole);
	virtual ~AreaMgr();

	//改变领域
	DWORD	ChangeArea( DWORD dwAreaID, BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith = TRUE, BOOL bJudgePre =TRUE );
	
	//清除神职领域
	VOID	ClearClergyArea();

	//初始化神职领域
	DWORD	InitGodArea();

	//改变领域能力
	DWORD	ChangeAreaAbility( DWORD dwAreaID, BOOL bActive, BOOL bByCD = TRUE );

	//从数据库初始化
	DWORD	InitFromDB( const BYTE* &pData, INT nNum );

	//存储到数据库
	DWORD	Save2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	//发送数据到客户端
	VOID	SendInitStateArea();

	//随机减少一个稀有领域能力,实际就是减少第一个找到的
	DWORD	RandCancleAreaAbility();

	//随机减少一个稀有领域,实际就是减少第一个找到的
	DWORD	RandDelArea();

	//获得已激活的某种领域数量
	INT		GetAbilityAreaNum(EGodAreaType eType,BOOL bAll=TRUE);

	//获得领域CD时间
	DWORD	GetAreaCD(DWORD dwAreaID);

	VOID	Update();

	BOOL	IsHaveGodArea(DWORD dwAreaID) { return m_mapGodArea.IsExist(dwAreaID); }

	//去掉稀有领域特效buff
	VOID	RemoveAreaBuff();
	//去掉所有稀有领域buff
	VOID	RemoveAllAreaBuff();

	VOID AddAreaBuff();//添加稀有领域buff add by xp

	DWORD	GetRandRareArea();

	BOOL	IsHaveArea(DWORD dwAreaID);

	BOOL	DirectDelOneRareArea(DWORD dwAreaID);

private:
	//增加领域
	DWORD	AddArea( DWORD dwAreaID, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith = TRUE, BOOL bJudgePre = TRUE  );

	//减少领域
	DWORD	DelArea( DWORD dwAreaID );

	//直接设置领域，如果没有则无条件加入
	DWORD	SetArea( DWORD dwAreaID, BOOL bActive, DWORD dwCD = 0 );

	//是否可以激活该领域能力
	DWORD	CanActivationAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	//激活领域能力
	DWORD	ActivationAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	//是否可以取消该领域能力
	DWORD	CanCancleAreaAbility(DWORD dwAreaID, BOOL bByCD = TRUE);

	//取消领域能力
	DWORD	CancleAreaAbility( DWORD dwAreaID, BOOL bByCD = TRUE );

	BOOL	DelSameArea(DWORD dwAreaID);

	BOOL	CancleAreaAbilityAndDelArea(DWORD dwAreaID);

	DWORD	CanAddArea(DWORD dwAreaID);

	VOID	SetAreaEffect( INT nNumPre, INT nNumCur );

private:
	Role			*m_pRole;				//角色指针
	MapArea			m_mapGodArea;			//神职领域，该领域来源于神职本身
	MapAreaInfo		m_mapOptionalArea;		//自选领域
	MapAreaInfo		m_mapRareArea;			//稀有领域

	DWORD			m_dwLastTime; 
	DWORD			m_dwCurTime;
};

