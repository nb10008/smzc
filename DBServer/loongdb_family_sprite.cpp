//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_guild.cpp
// author: Sxg
// actor:
// data: 2008-04-14
// last:
// brief: 数据库操作应用层实现 -- 保存稀有领域数据相关
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "../WorldDefine/family_define.h"


//------------------------------------------------------------------------------
// 读取家族妖精
BOOL CLoongDB::LoadFamilySprite(LPVOID pBuff, INT32& nGuildNum)
{
	return Load(pBuff, &nGuildNum, GT_INVALID, GT_INVALID, 
				&CLoongDB::FormatLoadAllFamilySprite, &CLoongDB::ProcResLoadAllFamilySprite);
}
//------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllFamilySprite(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("family_sprite", 
		"FamilyID,"	"Level,"	"Exp,"	"Name,"	"HP,"
		"EXAttack,"	"InAttack,"	"EXDefense,"	"InDefense,"	"EXAttackDeeper,"
		"InAttackDeeper,"	"EXAttackResistance,"	"InAttackResistance,"	"Toughness,"	"CritDes,"
		"ControleffectDeepen,"	"ControleffectResistance,"	"SlowingeffectDeepen,"	"SlowingeffectResistance,"	"FixedeffectDeepen,"
		"FixedeffectResistance,"	"AgingeffectDeepen,"	"AgingeffectResistance"	);
}
//------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllFamilySprite(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilySpriteSave);

	*pNum = pRes->GetRowCount();

	for( INT32 i = 0; i < *pNum; ++i )
	{
		p->dwFamilyID								= (*pRes)[0].GetDword();
		p->un2Level									= (*pRes)[1].GetShort();
		p->n8Exp									= (*pRes)[2].GetLong();
		(*pRes)[3].GetTCHAR(p->tszName, X_SHORT_NAME);
		p->nAbility[EFSA_HP]						= (*pRes)[4].GetInt();
		p->nAbility[EFSA_EXAttack]					= (*pRes)[5].GetInt();
		p->nAbility[EFSA_InAttack]					= (*pRes)[6].GetInt();
		p->nAbility[EFSA_EXDefense]					= (*pRes)[7].GetInt();
		p->nAbility[EFSA_InDefense]					= (*pRes)[8].GetInt();
		p->nAbility[EFSA_EXAttackDeeper]			= (*pRes)[9].GetInt();
		p->nAbility[EFSA_InAttackDeeper]			= (*pRes)[10].GetInt();
		p->nAbility[EFSA_EXAttackResistance]		= (*pRes)[11].GetInt();
		p->nAbility[EFSA_InAttackResistance]		= (*pRes)[12].GetInt();
		p->nAbility[EFSA_Toughness]					= (*pRes)[13].GetInt();
		p->nAbility[EFSA_CritDes]					= (*pRes)[14].GetInt();
		p->nAbility[EFSA_ControleffectDeepen]		= (*pRes)[15].GetInt();
		p->nAbility[EFSA_ControleffectResistance]	= (*pRes)[16].GetInt();
		p->nAbility[EFSA_SlowingeffectDeepen]		= (*pRes)[17].GetInt();
		p->nAbility[EFSA_SlowingeffectResistance]	= (*pRes)[18].GetInt();
		p->nAbility[EFSA_FixedeffectDeepen]			= (*pRes)[19].GetInt();
		p->nAbility[EFSA_FixedeffectResistance]		= (*pRes)[20].GetInt();
		p->nAbility[EFSA_AgingeffectDeepen]			= (*pRes)[21].GetInt();
		p->nAbility[EFSA_AgingeffectResistance]		= (*pRes)[22].GetInt();

		pRes->NextRow();
		++p;
	}
}
//------------------------------------------------------------------------------
DWORD CLoongDB::InsertFamilySprite(LPVOID pData)
{
	tagFamilySpriteSave* pFamilySpriteSave = (tagFamilySpriteSave*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetInsert("family_sprite");
	pStream->FillString("FamilyID=") << pFamilySpriteSave->dwFamilyID;
	pStream->FillString(",Level=") << pFamilySpriteSave->un2Level;
	pStream->FillString(",Exp=") << pFamilySpriteSave->n8Exp;
	pStream->FillString(",Name='").FillString(pFamilySpriteSave->tszName).FillString("'");
	pStream->FillString(",HP=") << pFamilySpriteSave->nAbility[EFSA_HP];
	pStream->FillString(",EXAttack=") << pFamilySpriteSave->nAbility[EFSA_EXAttack];
	pStream->FillString(",InAttack=") << pFamilySpriteSave->nAbility[EFSA_InAttack];					
	pStream->FillString(",EXDefense=") << pFamilySpriteSave->nAbility[EFSA_EXDefense];					
	pStream->FillString(",InDefense=") << pFamilySpriteSave->nAbility[EFSA_InDefense];					
	pStream->FillString(",EXAttackDeeper=") << pFamilySpriteSave->nAbility[EFSA_EXAttackDeeper];			
	pStream->FillString(",InAttackDeeper=") << pFamilySpriteSave->nAbility[EFSA_InAttackDeeper];			
	pStream->FillString(",EXAttackResistance=") << pFamilySpriteSave->nAbility[EFSA_EXAttackResistance];		
	pStream->FillString(",InAttackResistance=") << pFamilySpriteSave->nAbility[EFSA_InAttackResistance];		
	pStream->FillString(",Toughness=") << pFamilySpriteSave->nAbility[EFSA_Toughness];					
	pStream->FillString(",CritDes=") << pFamilySpriteSave->nAbility[EFSA_CritDes];					
	pStream->FillString(",ControleffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_ControleffectDeepen];		
	pStream->FillString(",ControleffectResistance=") << pFamilySpriteSave->nAbility[EFSA_ControleffectResistance];	
	pStream->FillString(",SlowingeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_SlowingeffectDeepen];		
	pStream->FillString(",SlowingeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_SlowingeffectResistance];	
	pStream->FillString(",FixedeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_FixedeffectDeepen];			
	pStream->FillString(",FixedeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_FixedeffectResistance];		
	pStream->FillString(",AgingeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_AgingeffectDeepen];			
	pStream->FillString(",AgingeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_AgingeffectResistance];


	BOOL bRes = m_pDBLoong->Execute(pStream);
	
	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
DWORD CLoongDB::DeleteFamilySprite(DWORD dwFamilyID)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family_sprite");
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateFamilySprite(tagFamilySpriteSave* pFamilySpriteSave)
{
	//return Update(GT_INVALID, pGlobalLimitedItem, 1, 0, &CLoongDB::FormatUpdateRareArea);

	if(!P_VALID(pFamilySpriteSave))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();
	pStream->SetUpdate("family_sprite");
	pStream->FillString("Level=") << pFamilySpriteSave->un2Level;
	pStream->FillString(",Exp=") << pFamilySpriteSave->n8Exp;
	pStream->FillString(",Name='").FillString(pFamilySpriteSave->tszName).FillString("'");
	pStream->FillString(",HP=") << pFamilySpriteSave->nAbility[EFSA_HP];
	pStream->FillString(",EXAttack=") << pFamilySpriteSave->nAbility[EFSA_EXAttack];
	pStream->FillString(",InAttack=") << pFamilySpriteSave->nAbility[EFSA_InAttack];					
	pStream->FillString(",EXDefense=") << pFamilySpriteSave->nAbility[EFSA_EXDefense];					
	pStream->FillString(",InDefense=") << pFamilySpriteSave->nAbility[EFSA_InDefense];					
	pStream->FillString(",EXAttackDeeper=") << pFamilySpriteSave->nAbility[EFSA_EXAttackDeeper];			
	pStream->FillString(",InAttackDeeper=") << pFamilySpriteSave->nAbility[EFSA_InAttackDeeper];			
	pStream->FillString(",EXAttackResistance=") << pFamilySpriteSave->nAbility[EFSA_EXAttackResistance];		
	pStream->FillString(",InAttackResistance=") << pFamilySpriteSave->nAbility[EFSA_InAttackResistance];		
	pStream->FillString(",Toughness=") << pFamilySpriteSave->nAbility[EFSA_Toughness];					
	pStream->FillString(",CritDes=") << pFamilySpriteSave->nAbility[EFSA_CritDes];					
	pStream->FillString(",ControleffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_ControleffectDeepen];		
	pStream->FillString(",ControleffectResistance=") << pFamilySpriteSave->nAbility[EFSA_ControleffectResistance];	
	pStream->FillString(",SlowingeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_SlowingeffectDeepen];		
	pStream->FillString(",SlowingeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_SlowingeffectResistance];	
	pStream->FillString(",FixedeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_FixedeffectDeepen];			
	pStream->FillString(",FixedeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_FixedeffectResistance];		
	pStream->FillString(",AgingeffectDeepen=") << pFamilySpriteSave->nAbility[EFSA_AgingeffectDeepen];			
	pStream->FillString(",AgingeffectResistance=") << pFamilySpriteSave->nAbility[EFSA_AgingeffectResistance];
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << pFamilySpriteSave->dwFamilyID;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//------------------------------------------------------------------------------
// 读取家族妖精修炼需求
BOOL CLoongDB::LoadFamilySpriteRequirement(LPVOID pBuff, INT32& nGuildNum, DWORD dwFamilyID)
{
	return Load(pBuff, &nGuildNum, dwFamilyID, GT_INVALID, 
				&CLoongDB::FormatLoadAllFamilySpriteRequirement, &CLoongDB::ProcResLoadAllFamilySpriteRequirement);
}
//------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllFamilySpriteRequirement(Beton::MyStream *pStream, DWORD dwFamilyID, DWORD dwRoleID)
{
	pStream->SetSelect("family_sprite_requirement", 
		"FamilyID,"	"RequirementID,"	"Complete,"	"RoleName," "Result");
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;
}
//------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllFamilySpriteRequirement(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilySpriteRequirementSave);

	*pNum = pRes->GetRowCount();

	for( INT32 i = 0; i < *pNum; ++i )
	{
		p->dwRequirementID			= (*pRes)[1].GetDword();
		p->bComplete				= (*pRes)[2].GetBool();
		(*pRes)[3].GetTCHAR(p->tszRoleName, X_SHORT_NAME);
		p->nRet						= (*pRes)[4].GetInt();

		pRes->NextRow();
		++p;
	}
}

//------------------------------------------------------------------------------
DWORD CLoongDB::InsertFamilySpriteRequirement(DWORD dwFamilyID, LPVOID pData, INT nNum)
{
	tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = (tagFamilySpriteRequirementSave*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	BOOL bRes = FALSE;
	for( int i = 0; i < nNum; ++i )
	{
		pStream->Clear();
		pStream->SetInsert("family_sprite_requirement");
		pStream->FillString("FamilyID=") << dwFamilyID;
		pStream->FillString(",RequirementID=") << pFamilySpriteRequirementSave->dwRequirementID;
		pStream->FillString(",Complete=") << pFamilySpriteRequirementSave->bComplete;
		pStream->FillString(",RoleName='").FillString(pFamilySpriteRequirementSave->tszRoleName).FillString("'");
		pStream->FillString(",Result=") << pFamilySpriteRequirementSave->nRet;

		bRes = m_pDBLoong->Execute(pStream);
		if(!bRes)
			return bRes;

		pFamilySpriteRequirementSave++;
	}
	
	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
DWORD CLoongDB::DeleteFamilySpriteRequirement(DWORD dwFamilyID)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family_sprite_requirement");
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateFamilySpriteRequirement(DWORD dwFamilyID, tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave)
{
	if(!P_VALID(pFamilySpriteRequirementSave))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();
	pStream->SetUpdate("family_sprite_requirement");
	pStream->FillString("Complete=") << pFamilySpriteRequirementSave->bComplete;
	pStream->FillString(",RoleName='").FillString(pFamilySpriteRequirementSave->tszRoleName).FillString("'");;
	pStream->FillString(",Result=") << pFamilySpriteRequirementSave->nRet;
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;
	pStream->FillString(" and RequirementID=") << pFamilySpriteRequirementSave->dwRequirementID;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//------------------------------------------------------------------------------
DWORD CLoongDB::DeleteALLFamilySpriteRequirement( )
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family_sprite_requirement");

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
// 读取家族玩家登记的妖精信息
BOOL CLoongDB::LoadFamilyRoleSprite(LPVOID pBuff, INT32& nGuildNum, DWORD dwFamilyID)
{
	return Load(pBuff, &nGuildNum, dwFamilyID, GT_INVALID, 
				&CLoongDB::FormatLoadAllFamilyRoleSprite, &CLoongDB::ProcResLoadFamilyRoleSprite);
}
//------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllFamilyRoleSprite(Beton::MyStream *pStream, DWORD dwFamilyID, DWORD dwRoleID)
{
	pStream->SetSelect("family_role_sprite", 
		"FamilyID,"	"SpriteID,"	"RoleName,"	"Shili," "Gongming,"
		"RoleID," "Level," "Quality");
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;
}
//------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadFamilyRoleSprite(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagRoleSpriteDataSave);

	*pNum = pRes->GetRowCount();				

	for( INT32 i = 0; i < *pNum; ++i )
	{
		p->dwSpriteType			= (*pRes)[1].GetDword();
		(*pRes)[2].GetTCHAR(p->tszRoleName, X_SHORT_NAME);
		p->fShili				= (*pRes)[3].GetInt();
		p->fGongming			= (*pRes)[4].GetFloat();
		p->dwRoleID				= (*pRes)[5].GetFloat();
		p->un2Level				= (*pRes)[6].GetShort();
		p->bQuality				= (*pRes)[7].GetByte();

		pRes->NextRow();
		++p;
	}
}


//------------------------------------------------------------------------------
BOOL CLoongDB::InsertRoleSpriteData( DWORD dwFamilyID, tagRoleSpriteDataSave* pRoleSpriteDataSave )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	pStream->SetInsert("family_role_sprite");
	pStream->FillString("SpriteID=") << pRoleSpriteDataSave->dwSpriteType;
	pStream->FillString(",RoleName='").FillString(pRoleSpriteDataSave->tszRoleName).FillString("'");;
	pStream->FillString(",Shili=") << pRoleSpriteDataSave->fShili;
	pStream->FillString(",Gongming=") << pRoleSpriteDataSave->fGongming;
	pStream->FillString(",Level=") << pRoleSpriteDataSave->un2Level;
	pStream->FillString(",Quality=") << pRoleSpriteDataSave->bQuality;
	pStream->FillString(",FamilyID=") << dwFamilyID;
	pStream->FillString(",RoleID=") << pRoleSpriteDataSave->dwRoleID;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateRoleSpriteData( DWORD dwFamilyID, tagRoleSpriteDataSave* pRoleSpriteDataSave )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();
	pStream->SetUpdate("family_role_sprite");
	pStream->FillString("SpriteID=") << pRoleSpriteDataSave->dwSpriteType;
	pStream->FillString(",RoleName='").FillString(pRoleSpriteDataSave->tszRoleName).FillString("'");;
	pStream->FillString(",Shili=") << pRoleSpriteDataSave->fShili;
	pStream->FillString(",Gongming=") << pRoleSpriteDataSave->fGongming;
	pStream->FillString(",Level=") << pRoleSpriteDataSave->un2Level;
	pStream->FillString(",Quality=") << pRoleSpriteDataSave->bQuality;
	pStream->SetWhere();
	pStream->FillString("FamilyID=") << dwFamilyID;
	pStream->FillString(" and RoleID=") << pRoleSpriteDataSave->dwRoleID;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//------------------------------------------------------------------------------
BOOL CLoongDB::DeleteRoleSpriteData( DWORD dwRoleID )
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family_role_sprite");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}
//------------------------------------------------------------------------------
