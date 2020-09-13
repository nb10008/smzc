#pragma once
#include "BidData.h"
struct tagStartNPCTalkEvent;

class BidMgr
{
	static BidMgr s_guiBidMgr;
	BidMgr(void);
	~BidMgr(void);
public:
	VOID Init();
	VOID Destroy();
	VOID Update();

	static BidMgr* Inst();
	const tagBidDataLocal* FindData(const DWORD& id) const;

private:
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt);
private:
	TSFPTrunk<BidMgr>				m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	BidData									m_bidData;
};
