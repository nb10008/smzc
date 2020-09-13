#pragma once

#pragma pack(push, 1)

//玩家聊天发言频道
enum ESendChatChannel
{
	ESCC_NULL = -1,
	
	ESCC_World = 0,				//世界
	ESCC_Pvt,				//私聊
	ESCC_Guild,				//帮派
	ESCC_Troop,				//队伍
	ESCC_Current,			//当前
	ESCC_Scene ,            //场景
	ESCC_Family,			//家族
	ESCC_Combat,			//战斗
	ESCC_Leave,				//留言
	ESCC_Affiche,			//公告
	ESCC_GM,				//GM
	ESCC_System,			//系统
	ESCC_Group ,            //群聊
	ESCC_CastTalk ,		    //传音
	ESCC_End,		
};

//世界频道时间间隔
const INT	WORLD_CHANNEL_INTERVAL	= TICK_PER_SECOND * 30;

//场景频道时间间隔
const INT	SCENE_CHANNEL_INTERVAL	= TICK_PER_SECOND * 10;


//世界频道消息长度限制
const INT	WORLD_CHANNEL_MSG_LEN	= 200;

//世界频道消息玩家等级限制
const INT	WORLD_CHANNEL_ROLE_LVL	= 10;

//传音IM物品ID 3070001 （绑定物品）
const DWORD TYPEID_IM_CASTTALK		= 3070001 ;//绑定物品  龙 1360001

//传音IM物品ID 3070002 （非绑定物品）
const DWORD TYPEID_IM_CASTTALK_UNBIND		= 3070002 ;//非绑定物品  龙1360001

//世界频道物品ID 3070001 （绑定物品）
const DWORD TYPEID_IM_WORLD         = 3070001;
 
//世界频道物品ID 3070002 （非绑定物品） 
const DWORD TYPEID_IM_WORLD_UNBIND         = 3070002;

//世界频道物品ID （网吧专用，这个要优先用掉）
const DWORD TYPEID_IM_WORLD_NETBAR		= 3070096;

#pragma pack(pop) 