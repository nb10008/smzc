#pragma once
#include "gameframe.h"


struct tagGuildMemberClient;
struct tagGuildInfoReflashEvent;
class HttpPicEx;
struct tagGetHeadpicByID;

class GuildMemberInfoFrame :
	public GameFrame
{
public:
	GuildMemberInfoFrame(void);
	~GuildMemberInfoFrame(void);
	
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventGuildInfoUpdate(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildInfoReflash(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildPosChange(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildRemoveMember(tagGuildInfoReflashEvent* pGameEvent);

	VOID ReflashInfo(const tagGuildMemberClient* pMember);
	DWORD OnEventGetHeadPicByID(tagGetHeadpicByID *pEvent);

private:
	TSFPTrunk<GuildMemberInfoFrame>		m_Trunk;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GUISystem>					m_pGUI;

	GUIWnd*								m_pWnd;
	GUIWnd*								m_pWndInfo;
	GUIButton*							m_pBtnClose;		// 关闭
	GUIButton*							m_pBtnFriend;		// 加为好友
	GUIButton*							m_pBtnTeam;			// 组队邀请
	GUIButton*							m_pBtnChat;			// 设为私聊
	GUIButton*							m_pBtnReflash;		// 更新信息
	HttpPicEx*							m_pStcImage;		// 大头贴
	GUIStaticEx*							m_pStcName;			// 名称
	GUIStaticEx*							m_pStcPosition;		// 职位
	GUIStaticEx*							m_pStcLevel;		// 等级
	GUIStaticEx*							m_pStcSex;			// 性别
	GUIStaticEx*							m_pStcClass;		// 职业
	GUIStaticEx*							m_pStcCont;			// 帮派贡献
	GUIStaticEx*							m_pStcTotalCont;	// 累计贡献


	DWORD								m_dwRoleID;			// 当前玩家
};
