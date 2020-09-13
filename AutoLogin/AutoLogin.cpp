// AutoLogin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "..\WorldDefine\login.h"
#include "..\WorldDefine\selectrole.h"
#include "shellapi.h"
#include <iphlpapi.h>
#include <comutil.h>
#include <fstream>

#pragma comment(lib, "IPHlpAPI.lib")
//#pragma comment(lib, "comsuppw.lib")

#define MAX_CONNECT_TIME 5000	//超时时间
#define SWAP_SPACE_TIME 10000	//SWAP TIME

VOID InitDump();

//获得本机mac地址
void __fastcall GetMAC(string& strMAC)   
{
	DWORD   Err;     
	PIP_ADAPTER_INFO   pAdapterInfo,   pAdapt;   
	DWORD   AdapterInfoSize;   

	AdapterInfoSize = 0;   
	if((Err = ::GetAdaptersInfo(NULL,&AdapterInfoSize))!=0)   
	{   
		if(Err != ERROR_BUFFER_OVERFLOW)   
		{   
			IMSG(_T("GetAdaptersInfo sizing failed with error: %u\n"), Err);   
			strMAC = "";
			return;   
		}   
	}   

	if((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize))==NULL)   
	{   
		IMSG(_T("Memory   allocation   error\n"));   
		strMAC = "";
		return;   
	}   

	//   Get   actual   adapter   information   
	if((Err = ::GetAdaptersInfo(pAdapterInfo,  &AdapterInfoSize))!= 0)   
	{   
		IMSG(_T("GetAdaptersInfo failed with error %u\n"), Err);   
		strMAC = "";
		return;   
	}   

	pAdapt = pAdapterInfo;
	char szBuff[128] = {0};
	std::string str("");   
	if(pAdapt) 
	{
		int  i=1;   	
		for(int i=0; i<6; i++)   
		{      
			INT64 n64Value = INT64(pAdapt->Address[i]);
			std::string strTemp(_i64toa(n64Value, szBuff, 16));
			transform(strTemp.begin(), strTemp.end(), strTemp.begin(), toupper);
			str = str + strTemp;
		}   
	}
	strMAC = str;
}




//-----------------------------------------------------------------------------
// 得到Dump FTP配置
//-----------------------------------------------------------------------------
VOID InitDump()
{
	TObjRef<VarContainer> pCfg = CreateObj("DumpConfig", "VarContainer");

	if( pCfg->Load(NULL, _T("config\\DumpCfg.xml")) )
	{
		const int MAX_STRING_SIZE = 512;
		TCHAR szVer[MAX_STRING_SIZE] = {0};
		::GetPrivateProfileString(_T("version"), _T("latest_game"), NULL,  szVer, MAX_STRING_SIZE, _T("launcher\\patchinfo.txt"));

		vEngine::InitDump(pCfg->GetString(_T("ftp")), 
			pCfg->GetString(_T("user")), pCfg->GetString(_T("password")), szVer);
	}

	KillObj("DumpConfig");
}

struct tagLoginData
{
	tstring loginIP;	//loginIP地址
	int		loginPorts;	//login端口
	tstring	severName;	//服务器名称	
};

enum ELoginerState
{
	ELS_Begin,					//开始新链接
	ELS_Waiting,				//等待消息
	ELS_XClient_Connecting,
	ELS_NetSession_Connecting,
	ELS_NetSession_WaitProof,
	ELS_NetSession_JoiningGame,
	ELS_SwapSeverSpaceTime,

};

class GameLoginer
{
public:
	GameLoginer():m_Trunk(this){}
	~GameLoginer(){}


	void Init()
	{

		m_pCmdMgr->Register("NS_JoinGame", (NETMSGPROC)m_Trunk.sfp2(&GameLoginer::NetRecvJoinGame), _T("NS_JoinGame"));

		stringstream stream;
		stream << "XClient_Login" << this;
		m_pStream = CreateObj(stream.str().c_str(), "XClient");
		m_pStream->Init(FALSE);

		LoadFromFile();

		m_iter = m_mapLoginData.begin();

		m_loginIP = m_iter->loginIP;
		m_loginPorts = m_iter->loginPorts;
		m_severName = m_iter->severName;

		

		m_eState = ELS_Begin;
		m_dwWaittingTime = 0;
		m_dwSwapTime = 0;
		m_dwLastTime = 0;//用于计时

		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);

		std::cout <<"<<---------------------------服务器自动登录检测系统--------------------------->>" << std::endl;
		std::cout <<"<<--" << CurTime.wYear << "年 " << CurTime.wMonth << "月 " << CurTime.wDay << "日 " << 
					 CurTime.wHour << "时 " << CurTime.wMinute << "分 " << CurTime.wSecond << "秒 " <<"-->>" << std::endl;
		std::cout << std::endl;

	}

// 	string ws2s(const tstring& ws)
// 	{
// 		_bstr_t t =ws.c_str();
// 		char* pchar = (char*)t;
// 		string result = pchar;
// 		return result;
// 	}

	void Destroy()
	{
		m_pCmdMgr->UnRegister("NS_JoinGame", (NETMSGPROC)m_Trunk.sfp2(&GameLoginer::NetRecvJoinGame));
	}

	void LeaveGame()
	{
		m_pSession->Disconnect();
		m_pStream->Disconnect();
	}
	// 处理JoinGame消息

	void NextSever()
	{
		++m_iter;
		if (m_iter == m_mapLoginData.end())
		{
			m_iter = m_mapLoginData.begin();
		}

		m_loginIP = m_iter->loginIP;
		m_loginPorts = m_iter->loginPorts;
		m_severName = m_iter->severName;
	}

	void ForceNextSever()
	{
		LeaveGame();
		m_eState = ELS_SwapSeverSpaceTime;
		NextSever();
	}

	DWORD NetRecvJoinGame(tagNS_JoinGame* pMsg, DWORD pPrama)
	{
		if( pMsg->dwID != m_pUtil->Crc32("NS_JoinGame") )
			return 0;

		ClearWaitTime();

		if (pMsg->dwErrorCode)
		{
			// todo  提示
			//tstring szValue= _T("data\\sound\\ui\\txjxl10_2-1.wav");
			Cool3D::AudioSystem::Inst()->Play2DEventSound( m_Music.c_str(), 0.7 );

			SYSTEMTIME CurTime;
			GetLocalTime(&CurTime);

			string ss=WideToChar(m_severName.c_str()); 
			std::cout <<"服务器：<<" <<  ss.c_str() << ">>     登录失败！！！" << "   Time: "<<CurTime.wDay<<"日 " << CurTime.wHour<< ":" <<CurTime.wMinute << ":" <<CurTime.wSecond << std::endl;
			LeaveGame();
			m_eState = ELS_SwapSeverSpaceTime;
			NextSever();
			return 0;
		}

		switch(pMsg->dwErrorCode)
		{
		case E_Success:
			{
				string ss=WideToChar(m_severName.c_str());
				//string ss = ws2s(m_severName);
				SYSTEMTIME CurTime;
				GetLocalTime(&CurTime);
				LeaveGame();
				std::cout <<"服务器：<<" <<  ss.c_str() << ">>     Login Success!" << "   Time: "<<CurTime.wDay<<"日 "<<  CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;
				m_eState = ELS_SwapSeverSpaceTime;
				NextSever();
				return 0;
			}
		}
		return 0;
	}

	void WaitTimeCheck()
	{
		m_dwWaittingTime += (timeGetTime() - m_dwLastTime);
		m_dwLastTime = timeGetTime();
		if (m_dwWaittingTime > MAX_CONNECT_TIME)
		{
			string ss=WideToChar(m_severName.c_str());
			//string ss = ws2s(m_severName);
			SYSTEMTIME CurTime;
			GetLocalTime(&CurTime);
			LeaveGame();
			std::cout <<"服务器：<<" <<  ss.c_str() << ">>     登录超时！！！ " << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;

			
			//tstring szValue= _T("data\\sound\\effect\\txjxl10_2-1.wav");
			Cool3D::AudioSystem::Inst()->Play2DEventSound( m_Music.c_str(), 10 );
			//Cool3D::AudioSystem::Inst()->PlayMusic(szValue.c_str(), TRUE, 0.1, 1000);
			m_eState = ELS_SwapSeverSpaceTime;
			NextSever();

			m_dwWaittingTime = 0;
		}
	}

	void ClearWaitTime()
	{
		m_dwWaittingTime = 0;
		m_dwLastTime = timeGetTime();
	}

	void Update()
	{
		switch (m_eState)
		{
		case ELS_Begin:
			{
				Connect();
				m_eState = ELS_XClient_Connecting;

				ClearWaitTime();
			}
			break;
		case ELS_XClient_Connecting:
			{
				if( IsConnected() )
				{
					SendLoginMsg();
					m_eState = ELS_NetSession_WaitProof;
					ClearWaitTime();
				}
				WaitTimeCheck();
			}
			break;
		case ELS_NetSession_WaitProof:
			{
				WaitProofResult();
				WaitTimeCheck();
			}
			break;
		case ELS_NetSession_Connecting:
			{
				if( !waitSessionConnect() )
				{	
					m_eState = ELS_NetSession_JoiningGame;
					ClearWaitTime();
				}
				WaitTimeCheck();
			}
			break;
		case ELS_NetSession_JoiningGame:
			{
				WaitTimeCheck();
			}
			break;
		case ELS_SwapSeverSpaceTime:
			{
				
				m_dwSwapTime += (timeGetTime() - m_dwLastTime);
				m_dwLastTime = timeGetTime();
				if (m_dwSwapTime > SWAP_SPACE_TIME)
				{
					m_eState = ELS_Begin;
					m_dwSwapTime = 0;
				}
			}
			break;
		}
		
		m_pSession->Update();
	}
	// 过滤用户名和密码中的回车换行等无意义字符
	VOID FilterString(tstring& str)
	{
		// 去掉回车
		INT nFind = 0;
		while( (nFind = str.find(_T('\r'), nFind)) != str.npos )
			str.replace(nFind, 1, _T(""));

		// 去掉换行
		nFind = 0;
		while( (nFind = str.find(_T('\n'), nFind)) != str.npos )
			str.replace(nFind, 1, _T(""));

	}

	void Connect()
	{

		tstring strIP = m_loginIP;
		INT nPort = m_loginPorts;//m_pVarContainer->GetDword(_T("port"), stream.str().c_str());

		// 尝试连接
		m_pStream->TryToConnect(m_pUtil->UnicodeToUnicode8(strIP.c_str()), nPort);
		m_dwCurPos = 0;

	}

	bool IsConnected()
	{
		if(m_pStream->IsConnected())
			return true;

		return false;
	}
	
	void CutOffZone()
	{
		m_pSession->Disconnect();
	}
	void WaitProofResult()
	{

		LPBYTE pMsg = NULL;
		DWORD dwMsgSize = 0;

		pMsg = m_pStream->Recv(dwMsgSize);
		if( P_VALID(pMsg) )
		{
			ASSERT(m_dwCurPos + dwMsgSize < sizeof(m_pMsgBuff));
			memcpy(m_pMsgBuff+m_dwCurPos, pMsg, dwMsgSize);
			m_pStream->FreeRecved(pMsg);
			m_dwCurPos += dwMsgSize;
		}
		pMsg = m_pMsgBuff;
		if(m_dwCurPos >= sizeof(tagNetCmd) && ((tagNetCmd*)pMsg)->dwSize <= m_dwCurPos)
		{
			DWORD dwSize = ((tagNetCmd*)pMsg)->dwSize;
			tagNLS_MiBao* pMiBao = (tagNLS_MiBao*)pMsg;
			if( pMiBao->dwID == m_pUtil->Crc32("NLS_MiBao") )
			{
			}

			tagNLS_ProofResult* pRecv = (tagNLS_ProofResult*)pMsg;
			if( pRecv->dwID == m_pUtil->Crc32("NLS_ProofResult") )
			{
				if( pRecv->dwErrorCode != E_Success )
				{
					std::cout <<"ErrorCode：<<" <<  pRecv->dwErrorCode << std::endl;
					{
						ofstream wFileStream;
						wFileStream.open("d:\\AutoLoginLog.txt",ios::out|ios::app);
						if (wFileStream)
						{
							SYSTEMTIME CurTime;
							GetLocalTime(&CurTime);
							string ss=WideToChar(m_severName.c_str());
							wFileStream <<"服务器："<<  ss.c_str() << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond << " ErrorCode: " << pRecv->dwErrorCode <<endl;
							wFileStream.close();
						}
					}

					if (  pRecv->dwErrorCode == E_ProofResult_Wrong_Build_Number)
					{
						string ss=WideToChar(m_severName.c_str());
						//string ss = ws2s(m_severName);
						SYSTEMTIME CurTime;
						GetLocalTime(&CurTime);
						LeaveGame();
						std::cout <<"服务器：<<" <<  ss.c_str() << ">>     版本号错误! " << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;
					}

					if (  pRecv->dwErrorCode == E_ProofResult_Account_No_Match)
					{
						string ss=WideToChar(m_severName.c_str());
						//string ss = ws2s(m_severName);
						SYSTEMTIME CurTime;
						GetLocalTime(&CurTime);
						LeaveGame();
						std::cout <<"服务器：<<" <<  ss.c_str() << ">>     账号非法~ " << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;
					}

					if (  pRecv->dwErrorCode == E_ProofResult_Queue)
					{
						string ss=WideToChar(m_severName.c_str());
						//string ss = ws2s(m_severName);
						SYSTEMTIME CurTime;
						GetLocalTime(&CurTime);
						LeaveGame();
						std::cout <<"服务器：<<" <<  ss.c_str() << ">>     排队中~ " << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;
					}
					if (  pRecv->dwErrorCode == E_ProofResult_Account_In_Use)
					{
						string ss=WideToChar(m_severName.c_str());
						//string ss = ws2s(m_severName);
						SYSTEMTIME CurTime;
						GetLocalTime(&CurTime);
						LeaveGame();
						std::cout <<"服务器：<<" <<  ss.c_str() << ">>     账号已经登录~ " << "   Time: "<<CurTime.wDay<<"日 "<< CurTime.wHour<<":" <<CurTime.wMinute << ":" <<CurTime.wSecond <<std::endl;
					}
					if (  pRecv->dwErrorCode == E_ProofResult_Account_In_Use
						||pRecv->dwErrorCode == E_ProofResult_Queue
						||pRecv->dwErrorCode == E_ProofResult_Account_No_Match)
					{
						ForceNextSever();
						return;
					}
					m_dwCurPos -= dwSize;
					for(DWORD i=0; i<m_dwCurPos; i++)
						m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
					return;
				}
				m_dwZoneIP		= pRecv->dwIP;
				m_dwZonePort	= pRecv->dwPort;
				m_dwAccountID	= pRecv->dwAccountID;
				m_dwVerifyCode	= pRecv->dwVerifyCode;
				if (pRecv->dwErrorCode == 0)
					m_bGuard = pRecv->bGuard;
				CHAR* szIP = m_pStream->IP2String(m_dwZoneIP);
				m_pSession->Connect(m_pUtil->Unicode8ToUnicode(szIP), m_dwZonePort);		//与zone服务器建立连接
				
				m_dwCurPos -= dwSize;
				for(DWORD i=0; i<m_dwCurPos; i++)
					m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	

				m_eState = ELS_NetSession_Connecting;
				ClearWaitTime();

				return;
			}
			m_dwCurPos -= dwSize;
			for(DWORD i=0; i<m_dwCurPos; i++)
				m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];		
		}
	}

	void SendLoginMsg()
	{

		TCHAR szTemp[X_LONG_NAME*2];
		tstring version = m_version;//szTemp;

		tstring strAccount = m_UserName;//m_pEditAccount->GetText();
		tstring strPassword = m_UserPassWord;//m_pEditPassword->GetText();
		this->FilterString(strAccount);
		this->FilterString(strPassword);
		string strAct = m_pUtil->UnicodeToAnsi(strAccount.c_str());

		tagNLC_SafeProof send;

		send.dwType = 1;	

		tstring szWorldName = m_severName;//m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
		send.dwWorldNameCrc = m_pUtil->Crc32(szWorldName.c_str());
		send.dwCurVersionID = m_pUtil->Crc32(version.c_str());
		strncpy( send.szUserName, strAct.c_str(), X_SHORT_NAME );

		if(!strPassword.empty())
		{
			
			string pass = m_pUtil->UnicodeToAnsi(strPassword.c_str());
			//密钥
			pass += "xvDvgqZq";
			BYTE digest[16];
			m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
			string strPswMD5;
			m_pUtil->MD5ToString( strPswMD5, digest );
			char szBuff1[X_LONG_NAME] = {0};
			strncpy( szBuff1, strPswMD5.c_str(), MAX_MD5_ARRAY );
			strupr( szBuff1 );
			strncpy( send.szPsd, szBuff1, MAX_MD5_ARRAY );
		}
		

		//MAC地址+C盘的序列号
		string strMAC;
		GetMAC(strMAC);
		DWORD Volue ;
		//API获得某个特定驱动器的序列号,这个程序中是获得C盘的序列号
		::GetVolumeInformation(_T("c:\\"), NULL, 12, &Volue, NULL, NULL, NULL, 10);
		char szBuff[64];
		std::string strTemp(_itoa(Volue, szBuff, 16));
		strMAC += strTemp;
		BYTE digest1[16];
		m_pUtil->MD5ForString( (char*)strMAC.c_str(), digest1 );
		string strGUIDMD5;
		m_pUtil->MD5ToString( strGUIDMD5, digest1 );
		strncpy( send.szGUID, strGUIDMD5.c_str(), MAX_MD5_ARRAY );

		DWORD dwKeyResult;
		ProofKey(dwKeyResult, (&send));
		send.dwProofKey     = dwKeyResult;/*2277331485*/

		m_pStream->Send(&send, sizeof(send));
	}

	
	bool waitSessionConnect()
	{
		if(m_pSession->IsConnect())
		{
			m_pStream->Disconnect();						//断开与login服务器的连接
			tagNC_JoinGame joinGameMsg;
			joinGameMsg.dwAccountID = m_dwAccountID;
			joinGameMsg.dwVerifyCode = m_dwVerifyCode;
			m_pSession->Send(&joinGameMsg);

			return false;
		}
		return true;
	}

	BOOL LoadFromFile()
	{
		// 读取Traindata对应的名字和描述
		list<tstring> listName;
		tstring szTrainName = _T("launcher\\serverinfo_back.xml");
		TObjRef<VarContainer> varTrainName = CreateObj("CameraData", "VarContainer");
		if (!varTrainName->Load(NULL, szTrainName.c_str(), "name", &listName))
		{
			IMSG(_T("Load CameraData file failed"));
			KillObj("CameraData");
			return FALSE;
		}
		list<tstring>::iterator iend = listName.end();
		for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)
		{
			tagLoginData data;
			data.severName = varTrainName->GetString(_T("name"), begin->c_str(),_T(""));
			data.loginIP = varTrainName->GetString(_T("ip"), begin->c_str(),_T(""));
			data.loginPorts = varTrainName->GetInt(_T("port"), begin->c_str(),0);
			m_mapLoginData.push_back(data);
		}
		KillObj("CameraData");

		list<tstring> GodSkillList;
		tstring szTrainName2 = _T("client\\name.xml");
		TObjRef<VarContainer> con = CreateObj("SoarSkill", "VarContainer");
		if (!con->Load(NULL, szTrainName2.c_str(), "name", &GodSkillList))
			return false;
		std::list<tstring>::iterator end = GodSkillList.end();
		for (std::list<tstring>::iterator iter = GodSkillList.begin(); iter != end; ++iter)
		{
			m_UserName = con->GetString(_T("name"), iter->c_str(),_T(""));
			m_UserPassWord = con->GetString(_T("password"), iter->c_str(),_T(""));
			m_version	= con->GetString(_T("version"), iter->c_str(),_T(""));
			m_Music = con->GetString(_T("music"), iter->c_str(),_T(""));
	
		}
		KillObj("SoarSkill");

		return TRUE;
	}
public:
	TSFPTrunk<GameLoginer>		m_Trunk;
	TObjRef<XClient>			m_pStream;
	TObjRef<Util>				m_pUtil;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;

public:
	typedef std::list<tagLoginData>	CameraDataMap;
	std::list<tagLoginData>::iterator m_iter;
	CameraDataMap m_mapLoginData;
	

	BYTE m_pMsgBuff[1024];
	DWORD m_dwCurPos;

	DWORD						m_dwZoneIP;			//选择的Zone服务器IP
	DWORD						m_dwZonePort;		//选择的Zone服务器port
	DWORD						m_dwAccountID;		//joingame用到
	DWORD						m_dwVerifyCode;		//joingame用到
	BOOL						m_bGuard;			//是否防沉迷账号

	bool						waiting;
	DWORD						m_dwWaittingTime;
	DWORD						m_dwSwapTime;

	DWORD						m_dwLastTime;


	ELoginerState				m_eState;

	tstring						m_UserName;
	tstring						m_UserPassWord;
	tstring						m_version;
	tstring						m_Music;

	// 当前需要链接的
	tstring m_loginIP;			//loginIP地址
	int		m_loginPorts;		//login端口
	tstring	m_severName;		//服务器名称	
	
};

int _tmain(int argc, _TCHAR* argv[])
{

	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("AutoLogin_client"));

	// 禁止屏幕保护及电源管理
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	vEngine::InitNetwork();
	vEngine::InitEngine();
	_set_se_translator( vEngine::SEH_Translation );
	InitDump();

	// 向vEngine注册客户端自己实现的模块
	//vEngine::Register(TWrap<Client>(),			"Client");
	//vEngine::Register(TWrap<GUIRender>(),		"GUIRender");
	vEngine::Register(TWrap<GameFrameMgr>(),	"GameFrameMgr");
	vEngine::Register(TWrap<GameScriptMgr>(),	"GameScriptMgr");
	vEngine::Register(TWrap<NetSession>(),		"NetSession");

	vEngine::Register(TWrap<GameInputMap>(),	"GameInputMap");
	vEngine::Register(TWrap<MsgBox>(),			"MsgBox");
	//vEngine::Register(TWrap<MsgBoxEx>(),		"MsgBoxEx");
	//vEngine::Register(TWrap<MsgInputBox>(),		"MsgInputBox");
	//vEngine::Register(TWrap<MsgMoneyInputBox>(),"MsgMoneyInputBox");
	//vEngine::Register(TWrap<MsgOptionBox>(),    "MsgOptionBox");

// 	CreateObj("VFS_System", "VirtualFileSys");
// 
// 
// 	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
// 	Cool3D::AudioSystem::Inst()->SetFS(pIFS);
	AudioSystem::Inst()->Init();

	CreateObj("GameConfig", "VarContainer");

	CreateObj("VFS_System", "VirtualFileSys");
	CreateObj("VFS_Animation", "VirtualFileSys");
	CreateObj("VFS_Model", "VirtualFileSys");
	CreateObj("VFS_Map", "VirtualFileSys");

	TObjRef<VirtualFileSys>("VFS_System")->SetWorkingPackage(_T("data\\system.cpk"));
	TObjRef<VirtualFileSys>("VFS_Animation")->SetWorkingPackage(_T("data\\system.cpk"));
	TObjRef<VirtualFileSys>("VFS_Model")->SetWorkingPackage(_T("data\\system.cpk"));
	TObjRef<Log>()->Create(NULL);

	tstring strPath = g_strLocalPath + _T("\\string_table.xml");
	g_StrTable.Init("VFS_System", strPath.c_str());


	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	ResMgr::Inst()->SetFS(ResMgr::EFST_System, pIFS);
	NavResMgr::Inst()->SetFS(NavResMgr::EFST_System, pIFS);

	pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Animation");
	ResMgr::Inst()->SetFS(ResMgr::EFST_Animation, pIFS);

	pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Model");
	ResMgr::Inst()->SetFS(ResMgr::EFST_Model, pIFS);
	NavResMgr::Inst()->SetFS(NavResMgr::EFST_Model, pIFS);

	pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	ResMgr::Inst()->SetFS(ResMgr::EFST_Effect, pIFS);

	pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Map");
	ResMgr::Inst()->SetFS(ResMgr::EFST_Map, pIFS);
	NavResMgr::Inst()->SetFS(NavResMgr::EFST_Map, pIFS);

	//--初始化NavMap
	const int navPoolBytes=8*1024*1024;
	NavKernel::Inst()->Init(navPoolBytes);

	pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	Cool3D::AudioSystem::Inst()->SetFS(pIFS);

	//--引擎资源管理器打开多线程读盘
	ResMgr::Inst()->EnableIOAsync(true);
	NavResMgr::Inst()->EnableIOAsync(true);

	// 设置程序默认路径
	vEngine::SetDefaultDir();

	
	

	DWORD lastTime;
	DWORD delay;

	try
	{

		GameLoginer m_Loginer;
		m_Loginer.Init();
		//m_Loginer.Connect();
		bool connecting = false;
		bool Sessionconnecting = false;
		lastTime = timeGetTime();
		while (true)
		{
			delay = timeGetTime() - lastTime;
			Cool3D::AudioSystem::Inst()->Update(delay);
			lastTime = timeGetTime();
	

			m_Loginer.Update();
			//cout << "asdasda" << endl;
		}
		getchar();
		// to do main loop here
	} 
	catch(vEngine::Exception)
	{
		throw;	// 方便调试器调试
	}

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	::CloseHandle(hMutex);
	exit(0);
	return 0;
}

