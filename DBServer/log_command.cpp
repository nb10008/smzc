#include "stdafx.h"
#include "log_command.h"

DBCommand::DBCommand(LPVOID p, TTSMPool *pp,DWORD (CLoong::*fuc)(DWORD,DWORD),tagNetCmd * pMsg)
:CommandObject(p),_func(fuc),_pAlloc(pp),_pMsg(0)
{
	if( P_VALID(_pAlloc) )
	{
		_pMsg = (tagNetCmd*)_pAlloc->Alloc(pMsg->dwSize);
		memcpy(_pMsg,pMsg,pMsg->dwSize);
	}
}
DBCommand::~DBCommand()
{
	if( P_VALID(_pMsg) && P_VALID(_pAlloc) )
	{
		_pAlloc->Free(_pMsg);
		_pMsg = NULL;
	}
}
bool DBCommand::process()
{
	if( P_VALID(m_ptr) && P_VALID(_pMsg) && P_VALID(_pAlloc) )
	{
		_func((CLoong*)m_ptr,(DWORD)_pMsg,0);
		_pAlloc->Free(_pMsg);
		_pMsg = NULL;
		return true;
	}
	return false;
}
void DBCommand::release()
{
	DBCommand * p = this;
	free_object(_pAlloc,DBCommand,p);
}


ActiveObject g_logProcessor;
static TTSMPool stg_MemPool;
TTSMPool * g_pLogCmdMemPool = &stg_MemPool;

