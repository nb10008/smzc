#pragma once

#include "../WorldDefine/family_define.h"

#pragma pack(push, 1)

//--------------------------------------------------------------------------
//表family_sprite相关
//建立一个家族妖精
CMD_START( NDBC_CreateFamilySprite )
	tagFamilySpriteSave		fs;
CMD_END

//更新一个家族妖精
CMD_START( NDBC_UpdateFamilySprite )
	tagFamilySpriteSave		fs;
CMD_END

//删除一个家族妖精
CMD_START( NDBC_DeleteFamilySprite )
	DWORD	dwFamilyID;						//家族ID
CMD_END

//获取家族妖精信息
CMD_START( NDBC_LoadFamilySprite )
CMD_END

CMD_START( NDBS_LoadFamilySprite )
	tagFamilySpriteSave				fs;
	INT								nNumRequirement;
	INT								nNumSprite;
	tagFamilySpriteRequirementSave	fsr[1];
	tagRoleSpriteDataSave			rsds[1];
CMD_END


//存储所有家族妖精信息
CMD_START( NDBC_SaveAllFamilySprite )
	INT								nNum;
	tagFamilySpriteSave				fs[1];
CMD_END

//--------------------------------------------------------------------------
//表family_sprite_requirement相关
//建立一个家族妖精修炼需求
CMD_START( NDBC_CreateFamilySpriteRequirement )
	DWORD	dwFamilyID;					//家族ID
	INT		nNum;						//数量
	tagFamilySpriteRequirementSave	fsr[1];
CMD_END

//更新家族妖精的一个修炼需求
CMD_START( NDBC_UpdateFamilySpriteRequirement )
	DWORD dwFamilyID;					//家族ID
	tagFamilySpriteRequirementSave	fsr;
CMD_END

//删除一类家族妖精的修炼需求
CMD_START( NDBC_DeleteFamilySpriteRequirement )
	DWORD	dwFamilyID;						//家族ID
CMD_END

//清空该表
CMD_START( NDBC_DeleteALLFamilySpriteRequirement )
CMD_END
//--------------------------------------------------------------------------
//表family_role_sprite相关
//--------------------------------------------------------------------------
//建立登记妖精信息
CMD_START( NDBC_CreateRoleSpriteData )
	DWORD	dwFamilyID;
	tagRoleSpriteDataSave	rsd;
CMD_END

//更新登记的妖精信息
CMD_START( NDBC_UpdateRoleSpriteData )
	DWORD	dwFamilyID;
	tagRoleSpriteDataSave	rsd;
CMD_END

//删除登记的妖精信息
CMD_START( NDBC_DeleteRoleSpriteData )
	DWORD	dwRoleID;
CMD_END

#pragma pack(pop)

