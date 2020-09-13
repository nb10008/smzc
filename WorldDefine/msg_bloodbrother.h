#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// 错误码
//-----------------------------------------------------------------------
enum
{
	E_bloodbrother_Success = 0,						// 成功
	E_bloodbrother_AlreadyHaveTeather,		// 已经有大哥带了
	E_bloodbrother_NotHaveItem,					// 缺少道具
	E_bloodbrother_TeamNotHave2,				// 队伍中只能有2个人
	E_bloodbrother_NotFriend,						// 2个人不是好友
	E_bloodbrother_TeamLevelNotFeet,			// 队伍中等级不满足需求
};

//-----------------------------------------------------------------------------
//	申请结拜
//-----------------------------------------------------------------------------
CMD_START(NC_TryMakeBrother)	
	DWORD dwDays;				// 申请结拜的天数
CMD_END

CMD_START(NS_TryMakeBrother)				
	DWORD dwErrorCode;
	DWORD dwRoleID;	// 结拜的ID，方便播放特效使用
CMD_END

//-----------------------------------------------------------------------------
//	结拜剩余天数
//-----------------------------------------------------------------------------
CMD_START(NS_BrotherLeftSecond)	
	DWORD	dwTeacherID;
	tagDWORDTime dwEndTime;
CMD_END

//-----------------------------------------------------------------------------
//	结拜剩余天数
//-----------------------------------------------------------------------------
CMD_START(NS_BrotherEnd)	
	BOOL bReason;			// 到期原因 0 是超时到期，1是 到达80级到期
CMD_END