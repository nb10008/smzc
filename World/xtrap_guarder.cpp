// Xtrap反外挂封装

#include "stdafx.h"
#include "xtrap_guarder.h"
#include "../WorldDefine/msg_game_guarder.h"
#include "role.h"

XTrapGuarder g_XTrapGuarder;
BYTE g_XTrapMap[CSFILE_NUM][XTRAP_CS4_BUFSIZE_MAP];

XTrapGuarder::XTrapGuarder()
:m_bEnable(FALSE)
{
}
XTrapGuarder::~XTrapGuarder()
{
	if (!IsEnable())
		return;
	XTrap_S_FreeDll();
}

VOID XTrapGuarder::ReadMapFile()
{
	for (INT nCnt = 0; nCnt < CSFILE_NUM; nCnt++)
	{
		char szFileName[64] = {0};
		sprintf(szFileName,"server_config\\map%d.cs3", nCnt);
		FILE* pFile = fopen((char*)szFileName, "rb");
		if (!P_VALID(pFile))
			continue;
		fread(g_XTrapMap[nCnt], XTRAP_CS4_BUFSIZE_MAP, 1, pFile);
		fclose(pFile);
	}
}

// 服务器初始化
VOID XTrapGuarder::OnServerStart()
{
	if (!IsEnable())
		return;

	ReadMapFile();
	XTrap_S_LoadDll();
	INT nRet = XTrap_S_Start(XTRAP_TIMEOUT, CSFILE_NUM, g_XTrapMap, NULL);
	if (nRet != 0)
		ILOG->Write(_T("X-Trap::XTrap_S_Start,return value=%d\r\n"),nRet);

	nRet = XTrap_S_SetAllowDelay(XTRAP_ALLOW_DELAY_2);
	if (nRet != 0)
		ILOG->Write(_T("X-Trap::XTrap_S_SetAllowDelay,return value=%d\r\n"),nRet);
}

// PlayerSession初始化
VOID XTrapGuarder::OnPlayerSessionInit(char* pSessionBuff)
{
	if (!IsEnable() || !P_VALID(pSessionBuff))
		return;

	INT nRet = XTrap_S_SessionInit(XTRAP_TIMEOUT, CSFILE_NUM, g_XTrapMap, pSessionBuff);
	if (nRet != 0)
		ILOG->Write(_T("X-Trap::XTrap_S_SessionInit,return value=%d\r\n"),nRet);
}

// 每20秒调用XTrap_CS_Step1，并将传出的buff向客户端发送
VOID XTrapGuarder::UpdateCSStep1(void* pSession, tagDWORDTime& dwPreXtrapUpdateTime, char* pSessionBuff)
{
	if (!IsEnable() || !P_VALID(pSession) || !P_VALID(pSessionBuff))
		return;

	if (CalcTimeDiff(GetCurrentDWORDTime(), dwPreXtrapUpdateTime) >= XTRAP_NOTIFY_CLIENT_SECOND)
	{
		PlayerSession* pPlayerSession = (PlayerSession*)pSession;
		tagNS_XTrap send;
		INT nRet = 0;
		send.cmd = XTRAP_CMD_STEP_ONE;
		nRet = XTrap_CS_Step1(pSessionBuff, send.buffer);
		pPlayerSession->SendMessage((LPBYTE)&send, send.dwSize);
		dwPreXtrapUpdateTime = GetCurrentDWORDTime();
		// 必须先通知客户端，然后在断开连接
		if (nRet != 0)
		{
			DWORD dwDetectCode = 0;
			if (nRet == XTRAP_API_RETURN_DETECTHACK)
			{
				memcpy(&dwDetectCode, ((const char*)pSessionBuff+8), 4);
				ILOG->Write(_T("XTRAP kick player [%u],XTrap_CS_Step1 return:%d,DetectCode=%x\r\n"), pPlayerSession->GetInternalIndex(), nRet, dwDetectCode);
			}
			else
			{
				ILOG->Write(_T("XTRAP kick player [%u],XTrap_CS_Step1 return:%d\r\n"), pPlayerSession->GetInternalIndex(), nRet);
			}
			g_worldSession.Kick(pPlayerSession->GetInternalIndex());
			pPlayerSession->SetKicked();
		}
	}
}

// 处理客户端返回的buffer
VOID XTrapGuarder::OnRecvStep2DataFromClient(void* pSession, void* pMsg, char* pSessionBuff)
{
	if (!IsEnable() || !P_VALID(pMsg) || !P_VALID(pSessionBuff) || !P_VALID(pSession))
		return;

	tagNC_XTrap* pRecv = (tagNC_XTrap*)pMsg;
	INT nRet = GT_INVALID;
	switch(pRecv->cmd)
	{
	case XTRAP_CMD_STEP_TWO:
		nRet = XTrap_CS_Step3(pSessionBuff, pRecv->buffer);
		if (nRet != 0)
		{
			PlayerSession* pPlayerSession = (PlayerSession*)pSession;
			ILOG->Write(_T("XTRAP-->player [%u],XTrap_CS_Step3 return:%d\r\n"), pPlayerSession->GetInternalIndex(), nRet);
		}
		break;
	default:
		break;
	}
}
