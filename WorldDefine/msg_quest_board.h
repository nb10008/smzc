//-----------------------------------------------------------------------------
//!\file 
//!
//!\date 
//!
//!\brief 
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

enum
{
	E_QuestBoard_Level_Limit	= 1,	// 不足5级的玩家不能获得任务
	E_RefreshBoard_IM_Limit		= 2,	// 刷新任务板缺少道具
};

enum EQuestBoardType
{
	EQBT_Null = 0,
	EQBT_Reward = 1,
	EQBT_YellowList = 2,
	EQBT_Guild = 3,
};

//-----------------------------------------------------------
// 打开任务板
//-----------------------------------------------------------
CMD_START(NC_OpenQuestBoard)
	DWORD		dwNPCID;
	DWORD		dwNPCTypeID;		
CMD_END

CMD_START(NS_OpenQuestBoard)
	DWORD		dwErrorCode;
	UINT16		u16Quest[QUEST_BOARD_MAX_CAPACITY];
	EQuestBoardType eQuestBoardType;
CMD_END

//-----------------------------------------------------------
// 强制刷新任务板
//-----------------------------------------------------------
CMD_START(NC_RenewQuestBoard)
	INT64		n64ItemSerial;			// 刷新道具的ID
	DWORD		dwNPCID;
	DWORD		dwNPCTypeID;
CMD_END

#pragma pack(pop)