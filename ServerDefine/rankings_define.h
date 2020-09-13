#pragma once

#pragma pack(push, 1)

#include "../WorldDefine/msg_rankings.h"
#include "../WorldDefine/clergy_define.h"

struct tagRankingsNode
{
	INT				nNum;						// 该角色名次	
	INT				nPreNum;					// 该角色上次名次	
	TCHAR			szRoleName[X_SHORT_NAME];	// 角色名字
	DWORD			dwRoleID;					// 角色ID
	ERankingType	eType;						// 排行类型
	INT64			n64Value;					// 值

	tagRankingsNode()
	{
		ZeroMemory( this, sizeof(tagRankingsNode) );
	}
};

struct tagPetRankingsNode
{
	INT				nNum;						// 该角色名次	
	INT				nPreNum;					// 该角色上次名次	
	TCHAR			szPetName[X_SHORT_NAME];	// 宠物名字
	DWORD			dwRoleID;					// 角色ID
	ERankingType	eType;						// 排行类型
	DWORD			dwPetID;					// 宠物ID
	INT64			n64Value;					// 值
	//INT64			n64Test;

	tagPetRankingsNode()
	{
		ZeroMemory( this, sizeof(tagPetRankingsNode) );
	}
};

struct tagClergyNode
{
	INT				nNum;						// 该角色名次	
	//INT			nPreNum;					// 该角色上次名次	
	//TCHAR			szRoleName[X_SHORT_NAME];	// 角色名字
	DWORD			dwRoleID;					// 角色ID
	EClergyType		eClergy;					// 职位
	DWORD			dwPoint;					// 值
	BOOL			bRecv;						// 是否被领取

	tagClergyNode()
	{
		ZeroMemory( this, sizeof(tagClergyNode) );
	}
};

#pragma pack(pop)