/* Copyright (c) 2010  大承网络有限公司
*  All rights reserve
*
*  文件名称:  group_chat.h
*  摘    要:  玩家可在线群聊的聊天系统
*  当前版本:  1.0
*  完成日期:  2010-6-4
*  作    者:  lamoair
*  
*/

#pragma once




class group_chat
{
public:
	//---------------------------------------------------------------------
	// 构造函数 和 析构函数
	//---------------------------------------------------------------------
	group_chat();
	~group_chat();
	
	VOID Init(const TCHAR* pGroupChatName,DWORD dwGroupChatMasterId,DWORD dwGroupChatId);

	//---------------------------------------------------------------------
	//添加角色到聊天群中
	//---------------------------------------------------------------------
	DWORD   AddRoleToGroup(Role* pRole);

	//---------------------------------------------------------------------
	//T掉角色出聊天群
	//---------------------------------------------------------------------
    DWORD   KickRoleFrouGroup(DWORD dwRoleId);
	//---------------------------------------------------------------------
	//退出一个群
	//---------------------------------------------------------------------
    DWORD   LoginOutGroup(Role* pRole);
  
	VOID    RoleDeleteGroup();
	//---------------------------------------------------------------------
	// 发送消息内容给聊天群成员
	//---------------------------------------------------------------------
	VOID   SendGroupMsg(VOID* pMsg,DWORD dwSize);

    TSafeList<DWORD>&  GetListGroupMemberId()
	{
		 return m_listGroupMemberId;
	}

	DWORD  GetGroupMemberNum()
	{
		return m_mapGroupMember.Size();
	}

    BOOL   GetGroupChatRole(DWORD dwRoleId);

	DWORD  GetGroupChatId()const {  return m_dwGroupChatId; }

	DWORD  GetGroupChatMasterId()const  { return m_dwGroupChatMasterId ;}
	 
	const TCHAR*  GetGroupChatName()const {  return m_szGroupChatName; }

	template<typename UnitOperation>
	VOID ForEachRoleInGroupChat( UnitOperation oper );
	
private:

	TSafeMap<DWORD,Role*>                m_mapGroupMember;
	TSafeList<DWORD>                     m_listGroupMemberId;
	DWORD                                m_dwGroupChatMasterId;
	DWORD                                m_dwRoleNum;
	TObjRef<Util>                        m_Util;
	TCHAR                                m_szGroupChatName[MAX_GROUPCHAT_LEN];
	DWORD                                m_dwGroupChatId; 
};

template<typename UnitOperation>
VOID group_chat::ForEachRoleInGroupChat( UnitOperation oper )
{
	std::list<Role*> listGroupChatRole;
	m_mapGroupMember.ExportAllValue(listGroupChatRole);
	std::list<Role*>::iterator it = listGroupChatRole.begin();
	for (; it != listGroupChatRole.end();it++)
	{
		if ( P_VALID(*it) )
		{
			oper(*it);
		}
	}
}

class GroupChatMgr
{
public:
	//---------------------------------------------------------------------
	// 构造函数 和 析构函数
	//---------------------------------------------------------------------
	GroupChatMgr();
	~GroupChatMgr();
    
	//---------------------------------------------------------------------
	// 获取一个角色聊天群
	//---------------------------------------------------------------------
    group_chat* GetGroupChat(DWORD dwGroupChatID) ;

	//---------------------------------------------------------------------
	// 角色一个角色聊天群
	//---------------------------------------------------------------------
	group_chat* CreateGroupChat(DWORD dwGroupChatID);
	VOID  DeleteGroupChat(DWORD dwGroupChatID);
	DWORD GetDefGroupChatNum()const  { return  m_dwDefGroupChatNum ;}

	DWORD CreateGroupChatId(){  return m_dwGroupChatId++; }

private:
	TSafeMap<DWORD,group_chat*>               m_mapGroupChat;     //聊天群容器
	volatile DWORD                            m_dwDefGroupChatNum; //创建群的总个数
    volatile DWORD                            m_dwGroupChatId;

};


extern  GroupChatMgr g_GroupChatMgr;