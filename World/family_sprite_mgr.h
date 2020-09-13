//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.h
// author: liutingting
// actor:
// data: 2011-03-24
// last:
// brief: 家族法宝相关内容操作
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/family_define.h"
#include "../ServerDefine/msg_family_sprite.h"
#define		FAMILY_NAME_MAX		16
//---------------------------------------------------------------------------
class FamilySprite;
typedef		TMap<DWORD, FamilySprite*>		FamilySpriteMap;
struct tagFabao;

class FamilySpriteMgr
{
public:
	FamilySpriteMgr();
	virtual ~FamilySpriteMgr();

	//清除
	VOID	Clear();

	//Update
	VOID	Update();

	//清除家族妖精修炼需求
	VOID	ClearAllFamilySpriteRequirement();

	//生成一个家族妖精
	FamilySprite*	CreateFamilySprite( DWORD dwFamilyID, const TCHAR *pszName );

	//找一个家族妖精
	FamilySprite*	FindFamilySprite( DWORD dwFamilyID );

	//找一个家族登记妖精
	tagFabao*	FindRoleFamilySprite( DWORD dwFamilyID, DWORD dwRoleID );
	
	//去掉一个家族妖精
	VOID	DelFamilySprite(DWORD dwFamilyID);

	//全部存库
	VOID	SaveAll2DB();

	//从库初始化
	VOID	InitFromDB(tagFamilySpriteSave *pFamilySpriteSave,tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNumRequirement, tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite);

private:
	//生成守护妖精的修炼需求
	VOID	CreateFamilySpriteRequirement();
	VOID	CreateFamilySpriteRequirement(DWORD dwFamilyID);

	//随机获得家族妖精修炼内容
	VOID	RandGetSpriteTrain( FamilySprite *pFamilySprite, DWORD dwPickTypeNum1, DWORD dwPickTypeNum2 );


private:
	FamilySpriteMap		m_mapFamilySprite;	//家族妖精map
	tagDWORDTime		m_dwRequirementTime;

};

extern FamilySpriteMgr g_FamilySpriteMgr;