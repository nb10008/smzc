//-----------------------------------------------------------------------------
//!\file msg_frame.h
//!
//!\date 2010-01
//!
//!\brief
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

enum EFrameType
{
	//角色界面、行囊界面、天资界面、任务界面、灵兽界面、好友界面、帮派界面、称号界面、声望界面、固定活动界面
	EFRT_Role = 1,
	EFRT_Bag,
	EFRT_TalentPoint,
	EFRT_Quest,
	EFRT_Pet,
	EFRT_Friend,
	EFRT_Guild,
	EFRT_Title,
	EFRT_Fame,
	EFRT_Activity,
	EFRT_Area,
};

CMD_START(NS_OpenFramePage)
	EFrameType		eType;
CMD_END

#pragma pack(pop)