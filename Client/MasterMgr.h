#pragma once

//! \class  MasterMgr
//! \brief  师徒管理器
//! \author hyu
//! \date   2010-1-31
#include "..\WorldDefine\msg_master_apprentice.h"

struct tagStuLevelUpInfo
{
	DWORD dwLevel;				// 等级
	DWORD dwAddJingWu;			// 升级增加的授业点数(师门)
	DWORD dwNeedJingWu;			// 升级消耗的授业点数(自己)
	DWORD dwNeedNum;			// 升级消耗的道具个数
};

class MasterMgr
{
public:
    MasterMgr(void);
    ~MasterMgr(void);

    void Init();
    void Destroy();
    void Update();

    //! singleton
    static MasterMgr* inst();

    //! show some errorcode
    bool ShowErrMsg(DWORD dwErrCode);

public:
	// 设置授业点数
	void SetJingWuPoint( DWORD dwPoint ) { m_dwMyJingWu = dwPoint; }
	// 设置师德点数
	void SetQinWuPoint( DWORD dwPoint ) { m_dwMyQinWu = dwPoint; }
	// 角色授业点数
	DWORD GetMyJingWu() { return m_dwMyJingWu; }
	// 角色师德点数
	DWORD GetMyQinWu() { return m_dwMyQinWu; }
	// 升级所需授业点数
	DWORD NeedJingWuCurLevel( DWORD dwLevel );
	// 升级所需物品数量
	DWORD NeedItemNumCurLevel( DWORD dwLevel );

private:
	// 读取升级表
	void LoadStuLevelUpInfoFile();
	
	// 得到授业点数
	DWORD OnNetGetJingWuPoint( tagNS_GetJingWuPoint *pMsg, DWORD );

	// 使用授业点数
	DWORD OnUseJingWuPoint( tagMsgBoxEvent *pGameEvent );

	// 徒弟出师,师傅奖励提示
	DWORD OnStuGraduate( tagNS_GraduateNotice *pMsg, DWORD );

private:
    //------------------------------------------------------------------------
    // 工具
    //------------------------------------------------------------------------
    TSFPTrunk<MasterMgr>		m_Trunk;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
	TObjRef<VarContainer>		m_pVarContainer;

	DWORD m_dwMyJingWu;			// 玩家的授业点数
	DWORD m_dwMyQinWu;			// 玩家的师德点数
	vector<tagStuLevelUpInfo> m_vecStuLevelUpInfo;		// 升级消耗
};
