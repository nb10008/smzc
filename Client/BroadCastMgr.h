#pragma once
#include "..\WorldDefine\chat_define.h"
#include "BroadCastProtoData.h"

struct tagNS_MsgBroadCastBlob;
struct tagRoleGetNameEvent;
struct tagGuildNameGetEvent;
struct tagEnterPromptEvent;
struct tagPushSpecInfoEvent;

const DWORD BROADCAST_OVERTIME_SET = 5 * 1000; // 超时设置 5 秒

class BroadCastMgr
{
public:
	enum EMsgCacheType
	{
		EMCT_Broadcast,		// 广播
		EMCT_BigBroadcast,	// 大广播
		EMCT_Chat,			// 聊天频道

		//新广播系统需要

		EMCT_BigNewBroadcast,   // 大字屏显
		EMCT_SmallRunHorse, 	// 小字跑马灯
		EMCT_BigRunHorse,		// 大字跑马灯
		EMCT_ChatNew		    // 聊天栏公告
		
	};

	struct tagMsgCache
	{
		tstring strContent;
		EMsgCacheType eType;		
		ESendChatChannel eChannel;
		bool bHasRoleOrGuild;
		INT  npriority;
		DWORD color;
		tagMsgCache()
		{
			eType = EMCT_Broadcast;
			eChannel = ESCC_NULL;
		}
	};
	

public:
	BroadCastMgr(void);
	~BroadCastMgr(void);

	static BroadCastMgr* Inst();

	void Init();
	void Destroy();
	void Update();

	void BroadCast(LPCTSTR szContent,BOOL bHasRoleOrGuild);
	void ToChatChannel(LPCTSTR szContent, ESendChatChannel eChannel, BOOL bHasRoleOrGuid);
	void BigBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild);
	void NewBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild,DWORD msgID);

	const BroadCastProtoData&	GetBroadCastData() const { return m_BroadcastData; }

private:
	//解析广播
	bool ParseBroadCast(tstring& strContext);

	void PushBroadCast(LPCTSTR szContent);
	void PushChatInfo(LPCTSTR szContent, ESendChatChannel eChannel);
	void PushBigBroadCast(LPCTSTR szContent);

	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pGameEvent);
	DWORD OnGuildGetNameEvent(tagGuildNameGetEvent *pGameEvent);
	DWORD OnEnterPromptEvent(tagEnterPromptEvent* pGameEvent);
	DWORD OnPushSpecInfo(tagPushSpecInfoEvent* pGameEvent);
	

	bool LoadBroadCastProto();

	//得到相应频道的字体颜色
	DWORD GetChannelColor(ENewChannelType escc);
private:
	TSFPTrunk<BroadCastMgr>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GameScriptMgr>		m_pGSMgr;

	typedef std::vector<tagMsgCache> MsgCache;
	MsgCache					m_msgCache;			// 广播消息缓存

	typedef std::vector<pair<bool,DWORD>> RoleCache;
	RoleCache					m_roleCache;		// 广播消息中的角色缓存

	typedef std::vector<pair<bool, DWORD>> GuildCache;
	GuildCache					m_guildCache;		// 广播消息中的帮派缓存

	bool m_bAllRoleFinded;
	bool m_bAllGuildFinded;

	DWORD m_dwLastParseTime;	// 上次处理发生的时间
	DWORD m_dwCurTime;

	BroadCastProtoData         m_BroadcastData;     // 广播原始数据图
};