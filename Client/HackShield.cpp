#include "stdafx.h"
#include "HackShield.h"
#include "axcltinterface.h"
#include "..\WorldDefine\msg_game_guarder.h"
#include "ApexClient.h"
//#define NO_HACKSHIELD
//#define APEX_MLXY

#ifdef APEX_MLXY
#pragma comment(lib,"..\\Apex\\AHClientInterface.lib")
#endif

/*
	pfRec 是 Apex Client 提供的数据处理函数;当,GameClient 收到GameServer发来属于Apex 的数据的时候调用这个函数指针处理;
	详见:  int		NoticeApec_UserData(const char * pBuf,int nBufLen);
*/

_FUNC_C_REC		pfRec = NULL;

HackShield::HackShield( void ):m_Trunk(this)
{

}

HackShield::~HackShield( void )
{

}

HackShield* HackShield::Inst()
{
	static HackShield hackshield;
	return &hackshield;
}

void HackShield::Init()
{
#ifdef NO_HACKSHIELD
	return;
#endif

	TObjRef<NetCmdMgr>()->Register("NS_GameGuarder", (NETMSGPROC)m_Trunk.sfp2(&HackShield::NoticeApec_UserData), _T("NS_GameGuarder"));

	TObjRef<NetCmdMgr>()->Register("NS_XTrap", (NETMSGPROC)m_Trunk.sfp2(&HackShield::OnNS_XTrap), _T("NS_XTrap"));

}

long NetSendToGameServer( const char * pBuffer,int nLen )
{
	char szBuff[1024] = {0};
	tagNC_GameGuarder* msg = (tagNC_GameGuarder*)szBuff;
	msg->dwID = TObjRef<Util>()->Crc32("NC_GameGuarder");
	msg->chCmd = 'T';
	memcpy( msg->chData, pBuffer, nLen );
	msg->nLen = (INT16)nLen;
	msg->dwSize= sizeof(tagNC_GameGuarder) + nLen - sizeof(char);
	TObjRef<NetSession>()->Send(msg);
	//IMSG(_T("NC_GameGuarder        T\n"));
	return 0;
}

int HackShield::StartApexClient()
{
#ifdef NO_HACKSHIELD
	return 0;
#endif

	long re =0;
	bool b_mlxy = false;
#ifdef APEX_MLXY
	re = CHCStart(NetSendToGameServer, pfRec);
	b_mlxy = true;
#endif
	if (!b_mlxy)
		re = ax_CHCStart(NetSendToGameServer, pfRec);

	char szBuff[1024] = {0};
	tagNC_GameGuarder* msg = (tagNC_GameGuarder*)szBuff;
	msg->dwID = TObjRef<Util>()->Crc32("NC_GameGuarder");
	msg->chCmd = 'R';
	memcpy(msg->chData, &re, sizeof(long));
	msg->dwSize= sizeof(tagNC_GameGuarder) + sizeof(long) - sizeof(char);
	TObjRef<NetSession>()->Send(msg);
	//IMSG(_T("NC_GameGuarder     R,          re: %d\n"), re);
	return 0;
}

int HackShield::StopApexClient()
{
#ifdef NO_HACKSHIELD
	return 0;
#endif

	TObjRef<NetCmdMgr>()->UnRegister("NS_GameGuarder", (NETMSGPROC)m_Trunk.sfp2(&HackShield::NoticeApec_UserData));


	TObjRef<NetCmdMgr>()->UnRegister("NS_XTrap", (NETMSGPROC)m_Trunk.sfp2(&HackShield::OnNS_XTrap));

	bool b_mlxy = false;
#ifdef APEX_MLXY
	CHCEnd();
	b_mlxy = true;
#endif
	if (!b_mlxy)
		ax_CHCEnd();

	return 0;
}

DWORD HackShield::NoticeApec_UserData( tagNS_GameGuarder* pMsg, DWORD pPrama )
{
	if(pfRec)
	{
		switch(pMsg->chCmd)
		{
		case 'T':
			{
				pfRec(pMsg->chData, pMsg->nLen);
				//IMSG(_T("NS_GameGuarder        T\n"));
			}
			break;
		default:
			break;
		}
	}
	return 0;
}


DWORD	HackShield::OnNS_XTrap(tagNS_XTrap* pMsg, DWORD pPrama)
{
	if ( XTRAP_CMD_STEP_ONE == pMsg->cmd)
	{
#ifdef ON_X_TRAP
		BYTE temp[XTRAP_BUFFER_SIZE]={0};
		XTrap_CS_Step2(pMsg->buffer,temp,XTRAP_PROTECT_PE|XTRAP_PROTECT_CODE|XTRAP_PROTECT_EXCEPT_VIRUS);

		tagNC_XTrap cmd;
		cmd.cmd = XTRAP_CMD_STEP_TWO;
		memcpy(cmd.buffer,temp,XTRAP_BUFFER_SIZE*sizeof(BYTE));
		TObjRef<NetSession>()->Send(&cmd);
#endif
	}
	return 0;
}