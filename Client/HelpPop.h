#pragma once

struct tagUpdateLPLevel;
struct tagNS_CompleteQuest;
struct tagNS_AddQuest;
struct tagNS_CompleteQuest;
struct tagCombatMsgForItemChange;

struct tagHelpInfo
{
	DWORD dwID;		
	BYTE byType;
	DWORD dwCondition;
	tstring strInfo;
	tstring strAlignType;
	tstring strAlignControl;
	tstring strPicName;
	DWORD dwShowTime;
	INT nLogic_x;
	INT nLogic_y;
	BYTE byReserve;
};

class HelpPop
{
public:
	HelpPop(void);
	~HelpPop(void);
	void Init(); 
	void Destroy();
	static HelpPop* Inst();
	void HelpPopFrame(const tagHelpInfo &HelpInfo);
	void LoadFromFile();
	void Update();

private:
	DWORD OnUpdateLPLevel(tagUpdateLPLevel* pEvent);
	DWORD OnEvent_LPAttIsOK(tagGameEvent* pEvent);
	DWORD OnCombatMsg_ItemAdd( tagCombatMsgForItemChange* pEvent );
	DWORD NetRecvMessage_AddQuest(tagNS_AddQuest* pMsg, DWORD pParam);
	DWORD NetRecvMessage_CompleteQuest(tagNS_CompleteQuest* pMsg, DWORD pParam);
	
private:
	struct tagHelpPopFrame
	{
		GUIWnd *pWnd;
		DWORD dwCurTime;
		DWORD dwMaxTime;
		tstring strAlignWnd;
	};

	enum HelpPopUpType
	{
		HPT_AcceptQuest = 1,
		HPT_DoneQuest,
		HPT_LevelUp,
		HPT_GetItem,
	};
private:
	TSFPTrunk<HelpPop>			m_trunk;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	map<DWORD, tagHelpInfo>		m_mapHelpInfo;
	vector<tagHelpPopFrame>		m_vecHelpInfo;
};