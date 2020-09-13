#pragma once

struct tagNS_TransmitReplyFairyBirth;
struct tagNS_TransmitApplyFairyBirth;
struct tagStartNPCTalkEvent;
struct tagMsgBoxEvent;
class SpriteMgr
{
	SpriteMgr(void);
	~SpriteMgr(void);
	static SpriteMgr s_guiSpriteMgr;
public:
	VOID Init();
	VOID Destroy();
	VOID Update();
	
	VOID OpenSpriteBornFrame(DWORD dwNPCID);
	VOID OpenSpritePartnerShipFrame(DWORD dwNPCID);

	static SpriteMgr* Inst();
private:
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt);
	DWORD OnNS_TransmitReplyFairyBirth(tagNS_TransmitReplyFairyBirth* pNetCmd,DWORD);
	DWORD OnNS_TransmitApplyFairyBirth(tagNS_TransmitApplyFairyBirth* pNetCmd,DWORD);
	DWORD OnSpriteevent_IsProductSpriteMsgBox( tagMsgBoxEvent* pEvent ); 
private:
	TSFPTrunk<SpriteMgr>			m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	DWORD									m_dwApplyPlayerID;

	bool											m_bNeedCheckFeedBack;
	DWORD									m_dwFeedBackTime;
};
