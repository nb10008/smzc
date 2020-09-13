#include "stdafx.h"
#include "GameGuard.h"
#include "..\WorldDefine\role_att.h"

GameGuard GameGuard::s_inst;
GameGuard* GameGuard::Inst()
{
	return &s_inst;
}

GameGuard::GameGuard():m_Trunk(this)
{

}

GameGuard::~GameGuard()
{
}


BOOL GameGuard::NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
#ifdef ON_GAMEGUARD
TCHAR szText[128] = {0};
	BOOL bAppExit = FALSE;
	switch (dwMsg)
	{
	case NPGAMEMON_CHECK_CSAUTH3:
		{
			PCSAuth3Data pCSAuth3 = (PCSAuth3Data)dwArg;

			DWORD dwSize = sizeof(tagNC_AnswerKoreaCSApprove) + pCSAuth3->dwPacketSize-1;
			tagNC_AnswerKoreaCSApprove* pSend = (tagNC_AnswerKoreaCSApprove*)new BYTE[dwSize];
			tagNC_AnswerKoreaCSApprove temp;
			memcpy(pSend, &temp, sizeof(temp));
			pSend->dwSize = dwSize;
			memcpy(pSend->byData, pCSAuth3->bPacket, pCSAuth3->dwPacketSize);
			TObjRef<NetSession>()->Send(pSend);

			return TRUE;
		}
		break;
	case NPGAMEMON_COMM_ERROR:
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true;
		break;
	case NPGAMEMON_COMM_CLOSE:
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_INIT_ERROR:
		//_stprintf(szText, _T("GameGuard 初始化错误: %lu"), dwArg);
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_SPEEDHACK:
		//_stprintf(szText, _T("检测到加速外挂"));
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_GAMEHACK_KILLED:
		//_stprintf(szText, _T("检测到外挂.\r\n%s"), g_npgl.GetInfo());
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_GAMEHACK_DETECT:
		//_stprintf(szText, _T("检测到外挂.\r\n%s"), g_npgl.GetInfo());
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_GAMEHACK_DOUBT:
		//_stprintf(szText, _T("游戏或GameGuard已被篡改"));
		_stprintf(szText, _T("Error Code:%lu"), dwArg);
		bAppExit = true; // 结束代码
		break;
	case NPGAMEMON_GAMEHACK_REPORT:
		{
			DWORD dwHackInfoSize = 0;
			LPBYTE pHackInfo = NULL;
			pHackInfo = g_npgl.GetHackInfo(&dwHackInfoSize);
			if (pHackInfo && dwHackInfoSize > 0)
			{
				DWORD dwSize = sizeof(tagNC_GameGuardReport) + dwHackInfoSize-1;
				tagNC_GameGuardReport* pSend = (tagNC_GameGuardReport*)new BYTE[dwSize];
				tagNC_GameGuardReport temp;
				memcpy(pSend, &temp, sizeof(temp));
				pSend->dwSize = dwSize;
				memcpy(pSend->byData, pHackInfo, dwHackInfoSize);
				TObjRef<NetSession>()->Send(pSend);
			}
		}
		break;
	}
	// 游戏结束前
	// 显示GameGuard相关信息
	if (szText[0])
	{
		MessageBox(NULL, szText, _T("nProtect GameGuard"), MB_OK);
		return TRUE;
	}
	if(bAppExit)
		exit(0);

	return TRUE;
#endif
	return FALSE;
}

#ifdef ON_GAMEGUARD
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	return GameGuard::Inst()->NPGameMonCallback(dwMsg, dwArg);
}
#endif
