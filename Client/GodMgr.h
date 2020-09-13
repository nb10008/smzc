#pragma once

#include "..\WorldDefine\role_att.h"
#include "GodAreaData.h"
#include "..\WorldDefine\msg_combat.h"

/** \class GodMgr
\brief 神系统管理器
*/

struct tagUseSkillEvent;
class CondensedFrame;
class ClergyFrame;
class GodRelationFrame;

struct tagStartNPCTalkEvent;

class GodMgr
{
public:
	GodMgr(void);
	~GodMgr(void);

	void Init();
	void Destroy();
	static GodMgr* Inst();
	DWORD OnUseSkillEvent(tagUseSkillEvent* pEvent);
	const tagGodAreaName* GetAreaDataByID(const DWORD& dwID);
	int GetFreePickAreaNumByGodHead(int nGodHead);
	void GetFreePickAreaListByNum(int num ,TList<DWORD>& list);
private:
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt);
	DWORD OnOpenCampaingFrame(tagGameEvent* pEvent);

	DWORD OnNS_UseLimitedItem(tagNS_UseLimitedItem* pCmd, DWORD);
private:
	TSFPTrunk<GodMgr>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;

	CondensedFrame* m_pCondensedFrame;
	ClergyFrame*    m_pClergyFrame;
	GodRelationFrame* m_pGodRelationFrame;

	GodAreaData		m_sGodAreaData;
};
