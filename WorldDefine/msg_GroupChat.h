//-----------------------------------------------------------------------------
// filename: msg_GroupChat.h
// author: Lamoair
// actor:
// data: 2010-6-4
// last:
// brief: 群聊天系统消息定义
//-----------------------------------------------------------------------------

#pragma  once
#pragma pack(push, 1)

#define   MAX_GROUPCHAT_LEN      8 
#define  MAX_GROUPCHAT_ROLENUM   50


enum GROUP_CHAT
{
	GRC_AlreadyInGroupChat         =  1,      //已经在群组中
	GRC_AddRoleToGroupChatError    =  2,      //添加角色到群组失败
	GRC_LoginOutGroupChatError     =  3,      //退出群聊失败
	GRC_GroupChatNameInvalid       =  4,      //输入名字无效
	GRC_RoleNotOnline              =  5,      //角色不在线
	GRC_RefuseJoinGroupChat        =  6,      //拒绝加入群
	GRC_GroupChatNotExist          =  7,      //群不存在
    GRC_NotGroupChatMaster         =  8,      //不是群主没有权限
	GRC_AddRoleNotExist            =  9,      //加入的角色不存在
	GRC_LoginOutGroupRoleNotExist  =  10,     //登出群的角色不存在
	GRC_GroupChatRoleOverMaxNum    =  11,     //群超过最大限度
};

struct tagGroupChatMember
{
	DWORD dwRoleId;
	TCHAR szRoleName[X_SHORT_NAME];
};
//-----------------------------------------------------------------------------
// 创建聊天群
//-----------------------------------------------------------------------------
CMD_START(NC_CreateGroupChat)
TCHAR szGroupChatName[MAX_GROUPCHAT_LEN];
CMD_END

CMD_START(NS_CreateGroupChat)
DWORD   dwErrorCode;
TCHAR   szGroupChatName[MAX_GROUPCHAT_LEN];
DWORD   dwGroupChatId;
CMD_END


//-----------------------------------------------------------------------------
// 删除聊天群
//-----------------------------------------------------------------------------
CMD_START(NC_DeleteGroupChat)
DWORD  dwGroupChatId;
CMD_END

CMD_START(NS_DeleteGroupChat)
DWORD  dwError ;
DWORD  dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// T出房间
//-----------------------------------------------------------------------------
CMD_START(NC_KickRoleFromGroupChat)
DWORD  dwGroupChatId;
DWORD  dwRoleId     ;          //被T的人id
CMD_END

CMD_START(NS_KickRoleFromGroupChat)
DWORD  dwError ;
DWORD  dwRoleId;                //被T的人id
DWORD  dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// 退出聊天群
//-----------------------------------------------------------------------------
CMD_START(NC_LoginOutGroupChat)
DWORD  dwGroupChatId;
CMD_END

CMD_START(NS_LoginOutGroupChat)
DWORD  dwError ;
DWORD  dwRoleId;                             //群出的玩家
DWORD  dwGroupChatId;                        //群的id
DWORD  dwGroupChatMasterId ;                  //房主id
CMD_END

//-----------------------------------------------------------------------------
// 邀请角色到聊天群
//-----------------------------------------------------------------------------
CMD_START(NC_InviteRoleToGroupChat)
DWORD   dwGroupChatId;                        //群的id
DWORD   dwDestRoleID;                         //被邀请者ID
CMD_END

CMD_START(NS_InviteRoleToGroupChat)
DWORD   dwError ;
TCHAR   szName[X_SHORT_NAME];                 //角色名字
CMD_END

CMD_START(NS_InviteRoleJoinToGroupChat)
DWORD   dwInviteRoleId ;                        //邀请者id
DWORD   dwGroupChatId ;                         //群的id
TCHAR   szGroupChatName[MAX_GROUPCHAT_LEN];
CMD_END

//-----------------------------------------------------------------------------
// 被邀请角色加到聊天群
//-----------------------------------------------------------------------------
CMD_START(NC_AddRoleToGroupChat)
DWORD   dwGroupChatId;                        //群的id
BOOL    bIsAccepted;                          //是否接受
DWORD   dwInviteRoleId;                       //邀请者id
CMD_END

CMD_START(NS_AddRoleToGroupChat)
DWORD                dwError ;                             //是否能成功加入
TCHAR                szGroupChatName[MAX_GROUPCHAT_LEN];   //群的名字
DWORD                dwGroupChatId;                        //群的id
DWORD                dwGroupChatMasterId;                  //群主id   
DWORD                dwMemberNum;                          //群成员个数 
tagGroupChatMember   GroupChatMember[1];                 //群员的名字
CMD_END

//-----------------------------------------------------------------------------
// 角色加入到聊天群
//-----------------------------------------------------------------------------
CMD_START(NS_RoleJoinToGroupChat)
DWORD    dwRoleID ;                                      //加入群的角色id
TCHAR    szName[X_SHORT_NAME];                           //角色名字
DWORD    dwGroupChatId;
CMD_END

//-----------------------------------------------------------------------------
// 发送聊天群消息
//-----------------------------------------------------------------------------
CMD_START(NC_SendGroupChatMsg)
CMD_END

#pragma pack(pop)