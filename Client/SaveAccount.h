#pragma once

class SaveAccount 
{
public:
	SaveAccount(void);
	~SaveAccount(void);

	static SaveAccount* Inst();
	void SetAccount(const TCHAR* szAccount);
	tstring& GetAccount() { return m_account; }
	void SetWorldID(const TCHAR* world_id){ m_world_id = world_id; }
	tstring& GetWorldID() { return m_world_id; }

	void SetWorldName(const TCHAR* world_name){ m_world_name = world_name; }
	tstring& GetWorldName() { return m_world_name; }

	void SetLoginID(const DWORD dwLoginID){ m_dwLoginID = dwLoginID; }
	DWORD GetLoginID() { return m_dwLoginID; }

	INT	GetCurMusic() { return m_nMusic; }
	void SetCurMusic(INT nMusic) { m_nMusic = nMusic; }

	void GoBack(bool b){m_bGoBack = b;}
	bool	IsGoBack(){return m_bGoBack;}

	int	GetLoginMode(){return m_nCurLoginMode ;}
	void SetLoginMode(int mode){ m_nCurLoginMode = mode ;}

	void SetAccountID(DWORD id){ m_dwAccountID = id;}
	DWORD GetAccountID(){return m_dwAccountID;}

	void SetCropID(DWORD id){ m_dwCropid = id;}
	DWORD GetCropID( bool bCommandLine = TRUE );

	void SetIsNeedSetTelNum(BOOL bIsSetTelNum) { m_bIsNeedSetTelNum = bIsSetTelNum; }
	BOOL GetIsNeedSetTelNum() { return m_bIsNeedSetTelNum; }

	// 混服需求 ------------------------------------------------------------------------------------------------ 
	//获得YY登陆的运营商ID，如果不是YY登陆返回0
	DWORD GetYYCropID();
	// 是否是命令行启动的客户端，如果是测存储命令行相关信息
	bool HaveCommandLine(char* cmd);
	// 设置从YY获得的GUID，用来登陆
	void SetYY_GUID(string ss){m_yyGUID = ss;}
	// 设置是否是通过YY登陆
	void SetYYLoginMode(bool b){m_bIsYYlogin = b;}
	bool IsYYLoginMode(){return m_bIsYYlogin;}
	string& GetYY_GUID(){return m_yyGUID;}
	// 获得金山快快命令行中的参数信息
	string& GetJSKK_Channel(){return m_jskkPm;}
	// open passport相关 
	VOID  SetChargeUrl(string  str);
	string  GetChargeUrl(){ return m_ChargeUrl; }
	VOID  SetOpenPassport(BOOL b){ m_bIsOpenPassPort = b; }
	BOOL IsOpenPassport(){ return m_bIsOpenPassPort; }
	const std::vector<DWORD>&  GetCropIDInfo();

	void SetYunLianOn(BOOL bOn){m_bYunLianOn = bOn;}
	BOOL GetYunLianOn(){return m_bYunLianOn;}
private:
	tstring				m_account;
	tstring				m_world_id;
	tstring				m_world_name;

	DWORD				m_dwLoginID;

	INT					m_nMusic;
	bool					m_bGoBack;

	int					m_nCurLoginMode;
	DWORD			m_dwAccountID;
	DWORD			m_dwCropid;

	string				m_yyGUID;
	string				m_jskkPm;
	bool					m_bIsYYlogin;

	string				m_ChargeUrl;
	BOOL				m_bIsOpenPassPort;

	BOOL				m_bIsNeedSetTelNum;

	std::vector<DWORD> m_vecCropIDInfo;

	BOOL				m_bYunLianOn;		//云联的开关

};

