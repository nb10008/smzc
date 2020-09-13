#pragma once
#include "MultChatData.h"
#include "..\WorldDefine\chat.h"
#include "..\WorldDefine\chat_define.h"

class MultChatMgr
{
public:
	MultChatMgr(void);
	~MultChatMgr(void);

public:
	static MultChatMgr* Inst();
	void	Init();
	void	Destroy();
//---------------------------处理游戏事件------------------------------------
	DWORD OnOpen_MultChatFrame(tagGameEvent* pEvent);
	DWORD OnClose_MultChatFrame(tagGameEvent* pEvent);
//---------------------------按键点选的响应函数-------------------------------------
	//设置当前选中的群，由点选的索引定位
	VOID	SetCurPickGroupID( INT index );			 
	//得到当前选中的群
	DWORD	GetCurPickGroupID() const;			
	//得到当前选中的群主ID,返回群主ID，-1（总列表空）、0（无效ID）
	DWORD	GetCurPickGroupMasterID() const;	 
	//设置当前选中的成员
	VOID	SetCurPickMemberID( INT index  );			 
	 //得到当前选中的成员，由点选的索引定位
	DWORD	GetCurPickMemberID() const;	
	//得到该群的群员数目
	INT		GetMemberNum(DWORD id) const;
//---------------------------向服务器发送消息的按键响应函数-------------------------
	VOID   CreateNewGroup(const tstring&);				 //创建新的群聊组
	VOID   DeleteGurGroup();							 //删除一个聊天组
	VOID   QuitCurGroup();							    //退出聊天群
	VOID   KickMember(INT index);								//剔除某个玩家
	VOID   InviteMember(const DWORD);					//邀请者发送邀请
	VOID   AddNewMember(tagMsgBoxEvent* pGameEvent);	//被邀请者发送是否接受邀请
	VOID   SendMessage(const tstring&, INT );			//发送消息
//----------------------------与Frame交互函数----------------------------
	//好友名字列表
	std::vector<LPCTSTR>&  GetFriendsNameList();
	//好友ID列表
	std::vector<DWORD>&    GetFriendsIDList();
	//获得群列表
	std::vector<DWORD>&    GetGroupIDList();
	//获得成员列表
	std::vector<DWORD>&   GetMemberIDList();
	//获得群聊总结构
	std::map<DWORD,tagLocalChatGroup*>& GetGroupChatData() { return m_mapGroupChatData; }
	//成员ID转名字
	const TCHAR*  MemberID2Name(DWORD);
	//群ID转名字
	const TCHAR*  GroupID2Name(DWORD);
	//返回当前群的聊天内容
	const TCHAR*   GetCurGroupChatContent();
	//找到对应ID群的群主ID
	DWORD GetGrouptMasterID(DWORD dwGroupID);
	//添加闪烁群
	VOID  AddGlintGroup(DWORD id);
	//减少闪烁群
	VOID  DelGlintGroup(DWORD id);
	//获得闪烁群ID索引
	std::vector<DWORD>& GetGlintGroup(){ return m_vecGroupClint; }
	//该ID的群是否需要闪烁
	BOOL IfNeedGlint(DWORD id);
	//是否需要Frame更新
	BOOL  IsNeedRefresh() 
	{ 
		BOOL  b = m_bIsNeedRefresh;
		m_bIsNeedRefresh = FALSE;
		return  b; 
	} 

	VOID SetFrameRefresh( BOOL b)
	{
		m_bIsNeedRefresh = b;
	}
private:
	BOOL  DealWithErrorCode(DWORD); //错误码处理函数
//---------------------------接受服务器发送消息的响应函数---------------------------
	//创建新的群聊组
	DWORD NetRecCreateNewGroupMsg( tagNS_CreateGroupChat* pNetCmd, DWORD );
	//删除一个聊天组
	DWORD NetRecDeleteGurGroupMsg( tagNS_DeleteGroupChat* pNetCmd, DWORD );
	//退出聊天群
	DWORD NetRecQuitCurGroupMsg( tagNS_LoginOutGroupChat* pNetCmd, DWORD );
	//剔除某个玩家
	DWORD NetRecKickMemberMsg( tagNS_KickRoleFromGroupChat* pNetCmd, DWORD );
	//被邀请者收到信息
	DWORD NetRecInviteMemberMsg( tagNS_InviteRoleJoinToGroupChat* pNetCmd, DWORD );
	//加入新成员,列表中加入新成员（邀请者、被邀请者，其他群成员）
	DWORD NetRecNewMemberJoinMsg( tagNS_AddRoleToGroupChat* pNetCmd, DWORD );
	//接收服务器发送的消息,返回-1表示非群聊消息，返回0为有ERROR
	DWORD NetRecNewMessage( tagNS_RoleChat* pNetCmd, DWORD );
	//群成员接收到加入新成员
	DWORD NetRecAddNewMemberMsg( tagNS_RoleJoinToGroupChat* pNetCmd, DWORD );
	//被邀请加入与否给邀请者的反馈
	DWORD NetRecInviteRoleToGroupChat( tagNS_InviteRoleToGroupChat* pNetCmd, DWORD );
	//更新好友的ID和name列表，并使两个表下标对应
	VOID  UpdateFriendsList(); 
	VOID  KillObjs();
	//返回是否需要刷新的BOOL
	BOOL  RefreshGroupAndMemberList();

private:
	TSFPTrunk<MultChatMgr>					m_trunk;
	TObjRef<NetCmdMgr>						m_pNetCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>		           m_pFrameMgr;
	
	std::map<DWORD,tagLocalChatGroup*>		m_mapGroupChatData;     //本地的群聊map。key：群ID，值：群聊结构
	DWORD									m_dwCurChatGroupID;     //选中的当前聊天群ID
	DWORD									m_dwCurChatMemberID;    //选中的当前群成员ID
	BOOL                                 m_bIsNeedRefresh;
	std::vector<DWORD>						m_vecGroupIndex;		//群ID索引					
	std::vector<DWORD>						m_vecMemberIndex;		//成员ID索引
	
	//好友名字列表和ID列表，相同则好友下标相同,UpdateFriendsList()在get函数中实时更新
	std::vector<LPCTSTR>					m_vecFriendsArray;		//好友名字列表
	std::vector<DWORD>						m_vecFriendsIDList;		//好友ID列表

	INT										m_iPlayerCreateGroupNum; //本地玩家创建群的数目
	std::vector<DWORD>						m_vecGroupClint;		 //闪烁的群	

};
