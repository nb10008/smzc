#pragma once

class ClientConfigInCpk
{
	static ClientConfigInCpk s_clientConfigMgr;
	ClientConfigInCpk(void);
	~ClientConfigInCpk(void);
public:
	VOID Init();
	static ClientConfigInCpk* Inst();
	BOOL IsMallOpen(){return (BOOL)m_dwOpenMall;}
	DWORD GetFaceBookMode(){return m_dwFaceBookMode;}
	BOOL Is360Open(){return (BOOL)m_dw360;}
private:
	TSFPTrunk<ClientConfigInCpk>				m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<VarContainer>           m_pCfgVars;              

	TCHAR                           m_szFilename[MAX_PATH];       //配置文件名

	DWORD								m_dwOpenMall;				// 商城开关，0是关，1是开
	DWORD								m_dwFaceBookMode;	// facebook 模式  0台湾，1香港
	DWORD								m_dw360;	// 360 提示
};
