#include "StdAfx.h"
#include "chat_mgr.h"
#include "chat_logger.h"
#include "world.h"

ChatLogger::ChatLogger()
: m_Trunk(this)
{
	m_bOn = FALSE;
	m_pMsgQueue = NULL;
	m_bTerminated = TRUE;
	m_nIndex = 0;
	memset(m_pBuf, 0, sizeof(m_pBuf));
	m_szDefaultName[0] = _T('\0');
}

ChatLogger& ChatLogger::getInstance()
{
	static ChatLogger _instance;
	return _instance;
}

BOOL ChatLogger::Init()
{
	m_pThread		=	"Thread";

	// 初始化成员属性
	TObjRef<VarContainer> pVar = "VarContainer";
	m_bOn		=	(BOOL)pVar->GetDword(_T("on chat_log"));

	if(m_bOn)
	{
		m_pMsgQueue = new MsgQueueTS(TRUE, FALSE);
		m_bTerminated = FALSE;
		vEngine::g_pInterfaceMgr->Create("ChatLogger", "Log");
		m_Logger = "ChatLogger";
		LPTSTR pLogFileName = CreateLogFileName();
		m_Logger->Create(pLogFileName);

		m_dwChatCmdID = TObjRef<Util>()->Crc32("NS_RoleChat");
		m_dwEquipCmdID = TObjRef<Util>()->Crc32("NS_RoleShowEquip");
		m_dwItemCmdID = TObjRef<Util>()->Crc32("NS_RoleShowItem");

		m_InitTime = g_world.GetWorldTime();

		//BEGIN UDP

		/*WORD wVersionRequested;  
		WSADATA wsaData;  
		int err;  
		wVersionRequested=MAKEWORD(2,2);  
		err=WSAStartup(wVersionRequested,&wsaData);  
		if(err!=0){  
			return 0;  
		}  
		if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wHighVersion)!=2)  
		{  
			WSACleanup();  
			return 0 ;  
		}  

		sockClient=socket(AF_INET,SOCK_DGRAM,0);   

		addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");  
		addrSrv.sin_family=AF_INET;  
		addrSrv.sin_port=htons(9999);  */ 

		//END UDP

		// 启动线程
		if(!m_pThread->CreateThread(_T("ChatLogger"), 
			(vEngine::THREADPROC)m_Trunk.sfp0(&ChatLogger::ThreadWriteLog), NULL))
		{
			return FALSE;
		}

		while( !m_pThread->IsThreadActive(_T("ChatLogger")) )
		{
			continue;
		}
	}
	return TRUE;
}

VOID ChatLogger::Update()
{
	if(m_bOn)
	{
		if ( g_world.GetWorldTime().day != m_InitTime.day)
		{
			m_Logger->Close();
			LPTSTR pLogFileName = CreateLogFileName();
			m_Logger->Create(pLogFileName);
			m_InitTime = g_world.GetWorldTime();
		}
	}
}

// struct tagChatContent
// {
//     TCHAR szContent[300];
// 	tagChatContent()
// 	{
// 		memset(this,0,sizeof(szContent));
// 	}
// };

void ChatLogger::Log(const MsgCmd& msgCmd)
{
	if(m_bOn)
	{
		LPCTSTR pStr = Format(msgCmd);
		if(P_VALID(pStr))
		{
			//m_pMsgQueue->AddMsg((LPCVOID)pStr, (_tcslen(pStr)+1)* sizeof(TCHAR));

			try{
 				tstring str = pStr;
				DWORD dwSize = str.size()*2+2;
// 				str += _T("\r\n\0 ");
// 				DWORD dwSize = str.length()*2;
// 				tagChatContent chatContent;
// 				memcpy(&chatContent.szContent,str.c_str(),dwSize);
				//string s = WideToChar(pStr);
				//s += "\r\n\0";
				//char *p=(char*)s.c_str();

				//sendto(sockClient,p,strlen(p)+1,0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
				m_pMsgQueue->AddMsg(pStr,dwSize);
			}
			catch(...)
			{

			}  

			SAFE_DEL_ARRAY(pStr);  

		}
	}
}


UINT ChatLogger::ThreadWriteLog()
{
	while(!m_bTerminated)
	{
		Update();
		DWORD msgNum;
		LPBYTE lpByte = m_pMsgQueue->GetMsg(msgNum);
		LPCTSTR pContent = (LPCTSTR)lpByte;
		while(lpByte)
		{
			//LPCTSTR pMsg = (LPCTSTR)lpByte;
			LPCTSTR pMsg  = pContent;
			size_t len = _tcslen(pMsg);
			if( len < sizeof(m_pBuf) / sizeof(TCHAR) )
			{
				if(len + m_nIndex > sizeof(m_pBuf) / sizeof(TCHAR) - 1)
					FlushToDisk();
				memcpy(m_pBuf + m_nIndex, pMsg, len*sizeof(TCHAR));
				m_nIndex += len;
			}	
			m_pMsgQueue->FreeMsg(lpByte);
			lpByte = m_pMsgQueue->GetMsg(msgNum);
		}
		::WaitForSingleObject(m_pMsgQueue->GetEvent(), 200);
		FlushToDisk();
	}
	FlushToDisk();
	return 0;
}

LPTSTR ChatLogger::CreateLogFileName()
{
	// 加上时间
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// 生成合适的log文件名
		static TCHAR szTime[MAX_PATH], szTemp[MAX_PATH],szDir[MAX_PATH];
		szTime[0] = _T('\0');
		szTemp[0] = _T('\0');
		szDir[0] = _T('\0');
		GetModuleFileName(NULL, szTemp, MAX_PATH);

		// 截去路径
		TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
		pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
		_stprintf(m_szDefaultName, _T("Log\\LogChat\\%s"), pResult);
		// 截去文件扩展名
		pResult = _tcsrchr(m_szDefaultName, _T('.'));
		if( pResult )
			*pResult = '\0';	

		CreateDir(_T("Log\\LogChat\\"));

		wsprintf(szDir, _T("LogChat[%d-%d-%d %02d-%02d-%02d]"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);
		_tcscat(m_szDefaultName,szDir);
		_tcscat(m_szDefaultName,_T("\\"));
		CreateDir(m_szDefaultName);
		


		// 		// 截去路径
		// 		TCHAR *pResult = _tcsrchr(szTemp, _T('\\'));
		// 		pResult = pResult ?	pResult+1 :	pResult = (TCHAR *)szTemp;
		// 		_stprintf(m_szDefaultName, _T("LogChat\\%s"), pResult);
		// 		// 截去文件扩展名
		// 		pResult = _tcsrchr(m_szDefaultName, _T('.'));
		// 		if( pResult )
		// 			*pResult = '\0';	

		szTime[0] = _T('\0');
		// 年月日以及时分秒
		wsprintf(szTime, _T("LogChat[%d-%d-%d %02d-%02d-%02d].log"),
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2);
		_tcscat(m_szDefaultName, szTime);
		//_tcscpy_s(m_szDefaultName,sizeof(szTemp),szTime);
	}

	return m_szDefaultName;
}

void ChatLogger::CreateDir(const TCHAR* Path)
{
	TCHAR DirName[256];
	const TCHAR* p = Path;
	TCHAR* q = DirName;	

	while(*p)
	{
		if ((_T('\\') == *p) || (_T('/') == *p))
		{
			if (_T(':') != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = _T('\0');
	}
	CreateDirectory(DirName, NULL);
}

void ChatLogger::Destroy()
{
	//closesocket(sockClient);  
	//WSACleanup();  

	InterlockedExchange((LPLONG)&m_bTerminated, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ChatLogger"), INFINITE);
	SAFE_DEL(m_pMsgQueue);
}

void ChatLogger::FlushToDisk()
{
	if(m_nIndex > 0)
	{
		m_pBuf[m_nIndex] = 0;
		m_Logger->Write(m_pBuf);
	}
	m_nIndex = 0;
}


LPTSTR ChatLogger::Format(const MsgCmd& msgCmd)
{
	MTRANS_ELSE_RET(pSender, msgCmd.GetSender(), Role, NULL);
	MTRANS_ELSE_RET(pSend, msgCmd.GetSendMsg(), tagNetCmd, NULL);
	MTRANS_ELSE_RET(pRecv, msgCmd.GetRecvMsg(), tagNetCmd, NULL);
	ESendChatChannel eChannel = msgCmd.GetChannelType();
	DWORD roleID = pSender->GetID();
	DWORD receiverID = msgCmd.GetReceiverID();
	TCHAR pPre[256];
	tagDWORDTime now = g_world.GetWorldTime();
	_stprintf_s(pPre, 256, _T("%02d-%02d-%02d %02d:%02d:%02d, %d %d %d, "), now.year, now.month, now.day, now.hour, now.min, now.sec, roleID, eChannel, receiverID);
	TCHAR pBuf[256];
	DWORD dSize = 0;
	if( pSend->dwID == m_dwEquipCmdID)
	{
		dSize += sizeof(_T("show equipment "));
		memcpy(pBuf, _T("show equipment "), sizeof(_T("show equipment ")));
	}
	else if (pSend->dwID == m_dwItemCmdID)
	{
		tagNS_RoleShowItem* pShowItem = static_cast<tagNS_RoleShowItem*>(pSend);
		_stprintf_s(pBuf, 256, _T("show item %d"), pShowItem->dwTypeID);
		dSize += _tcslen(pBuf)* sizeof(TCHAR); 
	}
	DWORD preSize = _tcslen(pPre) * sizeof(TCHAR);
	DWORD size = dSize + msgCmd.GetContentSize() + preSize + 6;
	LPBYTE pOut = new BYTE[size];
	memset(pOut, 0, size);
	memcpy(pOut, pPre, preSize);
	if(dSize > 0) memcpy(pOut+preSize, pBuf, dSize);
	if(msgCmd.GetContent() && msgCmd.GetContentSize() >0)
	{
		memcpy(pOut+preSize+dSize, msgCmd.GetContent(), msgCmd.GetContentSize());
	}
	_tcscat((wchar_t*)pOut, _T("\r\n"));

	return (LPTSTR)pOut;
}