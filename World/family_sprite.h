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
#define		FAMILY_NAME_MAX		16
//---------------------------------------------------------------------------
struct tagFabao;

typedef		TMap<DWORD, tagFamilySpriteRequirementSave*>					FamilySpriteTrainMap;

struct tagRoleSpriteData
{
	tagFabao				*pFabao;
	tagRoleSpriteDataSave	RoleSpriteDataSave;

	tagRoleSpriteData( DWORD dwInRoleID )
	{
		Clear();
		RoleSpriteDataSave.dwRoleID = dwInRoleID;
	}

	~tagRoleSpriteData()
	{
		Clear();
	}

	VOID Clear()
	{
		RoleSpriteDataSave.Clear();
		pFabao = NULL;
	}
};

typedef		TMap<DWORD, tagRoleSpriteData*>		RoleSpriteMap;

class FamilySprite
{
public:
	FamilySprite(DWORD dwFimilyID);
	virtual ~FamilySprite();

	//设置家族妖精名字
	VOID	SetName(const TCHAR *ptszName);

	//清除所有的修炼内容
	VOID	ClearFamilySpriteTrain();

	//清除所有的修炼内容到DB
	VOID	ClearFamilySpriteTrain2DB();

	//清除
	VOID	Clear();

	//是否存在修炼内容
	BOOL	IsExistFamilySpriteTrain( DWORD dwID );

	//设置家族妖精数据
	VOID	SetFamilySpriteData(tagFamilySpriteSave *pFamilySpriteSave);

	//设置家族妖精修炼内容数据
	VOID	SetFamilySpriteRequirementData(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum);

	//更新家族妖精数据到DB
	VOID	UpdateFamilySpriteData2DB();

	//建立家族妖精修炼数据
	VOID	CreateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete );

	//将家族妖精修炼数据发送到DB
	VOID	CreateFamilySpriteRequirementData2DB();
	
	//更新家族妖精修炼数据
	VOID	UpdateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete, INT nRet, DWORD dwRoleID );

	//获得家族妖精修炼内容数量
	INT		GetFamilySpriteRequirementNum(){return m_mapFamilySpriteTrain.Size();}

	//获得家族妖精修炼内容
	DWORD	GetFamilySpriteRequirement(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum);

	//获得修炼内容状态
	BOOL	IsFamilySpriteTrainComplete( DWORD dwID );

	//获得当前能力值
	INT		GetAbility(EFamilySpriteAtt eType);

	//设置当前能力值
	VOID	SetAbility(EFamilySpriteAtt eType, INT nData);

	//家族妖精经验，你懂的
	VOID	SetExp(INT64 n8Exp){m_FamilySpriteData.n8Exp = n8Exp;}
	INT64	GetExp(){return m_FamilySpriteData.n8Exp;}

	//家族妖精等级，你懂的
	VOID	SetLevel(UINT16 un2Level){m_FamilySpriteData.un2Level = un2Level;}
	UINT16	GetLevel(){return m_FamilySpriteData.un2Level;}

	//获取家族妖精基本数据
	tagFamilySpriteSave* GetFamilySpriteData(){return &m_FamilySpriteData;}

	//注册玩家法宝
	tagFabao *RegisterRoleSprite( DWORD dwRoleID, tagFabao *pFabao );

	//取回玩家法宝
	tagFabao* ReturnRoleSprite( DWORD dwRoleID );

	//判断玩家是否已经有登记的法宝了
	BOOL	HaveRoleSprite( DWORD dwRoleID ){return m_mapRoleSprite.IsExist(dwRoleID);}

	//设置玩家法宝
	VOID	SetRoleSprite(DWORD dwRoleID, tagFabao *pFabao);

	//更新玩家角色的属性
	VOID	UpdateAllRoleAtt();

	//更新玩家角色的属性
	VOID	UpdateRoleAtt( DWORD dwRoleID );

	//移除玩家角色妖精
	tagFabao*	RemoveRoleSprite(DWORD dwRoleID);

	//移除玩家角色妖精(角色离线状态)
	VOID		RemoveRoleSpriteOffline(DWORD dwRoleID);

	//获取玩家角色妖精
	tagFabao*	GetRoleSprite(DWORD dwRoleID);

	//获取玩家登记的妖精数量
	INT		GetFamilyRoleSpriteNum(){return m_mapRoleSprite.Size();}

	//获取玩家登记的妖精
	DWORD	GetFamilyRoleSprite( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNum );

	//设置更新家族妖精修炼标志
	VOID	SetUpdateSpriteRequirementFlag(BOOL bFlag){m_UpdateTraining = bFlag;} 
	BOOL	GetUpdateSpriteRequirementFlag(){ return m_UpdateTraining; } 

	//设置玩家妖精信息
	VOID	SetRoleSpriteInfo( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite );

	//移除所有玩家角色妖精
	VOID	RemoveAllRoleSprite();

	//计算玩家属性加成
	VOID	CaculateRoleAtt( DWORD dwRoleID, Role *pRoleIn = NULL );

	//激活属性
	VOID	ActivationFamilySpriteAbility();

	//增加家族妖精经验
	VOID	AddExp( int nAddExp );

	//清除玩家属性加成
	VOID	ClearRoleAtt( DWORD dwRoleID );

private:
	//更新家族妖精修炼数据到DB
	VOID	UpdateFamilySpriteRequirementData2DB( DWORD dwTrainID, BOOL bComplete, INT nRet, TCHAR *ptszRoleName );

	//计算共鸣值
	FLOAT	CaculateGongming( DWORD dwRoleID );

private:
	tagFamilySpriteSave		m_FamilySpriteData;					//家族妖精数据
	FamilySpriteTrainMap	m_mapFamilySpriteTrain;				//存储当前家族妖精的修炼内容
	RoleSpriteMap			m_mapRoleSprite;					//玩家角色登记的妖精
	BOOL					m_UpdateTraining;					//更新家族妖精修炼标志
	DWORD					m_dwFimilyID;						//家族ID
	
};

