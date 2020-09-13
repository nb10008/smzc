//-----------------------------------------------------------------------------
//!\file msg_appearance_transform.h
//!
//!\date 2010-5-26
//!
//!\brief 客户端与服务器之间关于易容师的消息
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#define APPEARANCE_TRANSFORM_ITEM		3070066	// 易容卷道具ID
#define APPEARANCE_TRANS_ITEM_EXPEND	1		// 每次易容消耗易容卷个数


// 易容相关错误码定义
enum E_Appearance_Transform_Error
{
	EATE_Success = 0,
	EATE_NoTransformItem = 1,	// 没有易容卷道具
	EATE_SendBigVisTileMsg = 2,	// 给九宫格里面的玩家发送有玩家易容了
};


// 客户端-->服务器 角色请求易容
CMD_START(NC_AppearanceTransform)
	WORD	wHairMdlID;			// 发型
	WORD	wHairTexID;			// 发色
	WORD	wFaceMdlID;			// 面部五官
	WORD	wFaceDetailTexID;	// 面部细节
	DWORD	dwNPCID;			// NPCID
CMD_END


// 服务器-->客户端 返回易容结果
CMD_START(NS_AppearanceTransform)
	DWORD	dwErrorCode;
	DWORD	dwRoleID;			// 易容角色的角色ID
	WORD	wHairMdlID;			// 发型
	WORD	wHairTexID;			// 发色
	WORD	wFaceMdlID;			// 面部五官
	WORD	wFaceDetailTexID;	// 面部细节
CMD_END


#pragma pack(pop)