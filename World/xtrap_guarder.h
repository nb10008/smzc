// Xtrap反外挂封装
#pragma once
#include "Xtrap_S_Interface.h"

#define XTRAP_NOTIFY_CLIENT_SECOND 20
#define CSFILE_NUM 2
#define  XTRAP_TIMEOUT 600

class XTrapGuarder
{
public:
	XTrapGuarder();
	~XTrapGuarder();

	// 服务器初始化
	VOID OnServerStart();

	// PlayerSession初始化
	VOID OnPlayerSessionInit(char* pSessionBuff);

	// 每20秒调用XTrap_CS_Step1，并将传出的buff向客户端发送
	VOID UpdateCSStep1(void* pSession, tagDWORDTime& dwPreXtrapUpdateTime, char* pSessionBuff);

	// 处理客户端返回的buffer
	VOID OnRecvStep2DataFromClient(void* pSession, void* pMsg, char* pSessionBuff);

	VOID SetEnable(BOOL bEnable) { m_bEnable = bEnable; }
	BOOL IsEnable() { return m_bEnable; }

private:
	VOID ReadMapFile();

private:
	BOOL m_bEnable;        // 功能是否开启
};

extern XTrapGuarder g_XTrapGuarder;