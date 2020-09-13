#pragma once

enum
{
	OverSea_NPC = 0,
	OverSea_Item,
	OverSea_Formual,
	OverSea_Quest,
};

class OverSeaData
{
	static OverSeaData s_gOverSeaData;
	public:
	OverSeaData(void);
	~OverSeaData(void);
	void Init();
	static OverSeaData* Inst();
	void LoadFormFile();
	bool IsFormualHide(const DWORD& id);
	bool IsQuestHide(const DWORD& id);
	bool IsNPCHide(const DWORD& id);
	bool IsFuncOpen();
	void EnableFunc(bool bEnable){m_bOpenFunc = bEnable;}
private:
	TSFPTrunk<OverSeaData>		m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	bool						m_bOpenFunc;
	
	list<DWORD>		m_listFormualID;
	list<DWORD>		m_listQuestID;
	list<DWORD>		m_listNPCID;
};
