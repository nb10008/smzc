#pragma once

#include "..\WorldDefine\role_att.h"

#define MAX_War_Role_Num 6	//战场人数
struct tagRefreshWarInfo;
struct tagWarnfo
{
	INT AWarInfo[ERoleAttrEnd];
	tagWarnfo()
	{
		for(int i = 0; i < ERoleAttrEnd; i++)
		{
			AWarInfo[i] = 0;
		}
	}
};
struct tagWarResult
{
	DWORD dwItem1;
	DWORD dwItem2;
	DWORD dwItem3;
	BOOL bIsSuccess;
};
class WarMgr
{
public:
	WarMgr(void);
	~WarMgr(void);

	static WarMgr* Inst();
	/** 进入游戏世界时初始化
	*/
	void Init();
	/** 离开游戏世界时释放所有数据
	*/
	void Destroy();

	void ClearData();

	DWORD OnNS_SyncRoleAttr(tagNS_SyncRoleAttr* pCmd, DWORD);
	DWORD OnNS_SyncRoleAttrToOther(tagNS_SyncRoleAttrToOther* pCmd, DWORD);
	DWORD OnNS_ShowWarResult(tagNS_ShowWarResult* pCmd, DWORD);
	DWORD OnNS_RefreshBaoXiang(tagNS_RefreshBaoXiang* pCmd, DWORD);
	DWORD OnNS_GetWarBaoXiang(tagNS_GetWarBaoXiang* pCmd, DWORD);

	//获取本地玩家战场金币数量
	DWORD GetWarMoneyNum();
	//获取战场数据map
	const std::map<DWORD, tagWarnfo>& GetWarInfo() {return m_mapWarInfo;};
	//获取战场结果
	tagWarResult* GetWarResult(){return &m_sResult;}

private:
	TSFPTrunk<WarMgr>			m_trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;

	std::map<DWORD, tagWarnfo>  m_mapWarInfo;
	DWORD						m_dwRoleID;
	tagWarResult				m_sResult;		//战场结果
};