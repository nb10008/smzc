//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: log_command.h
// author: Jason
// actor:
// data: 2010-12-16
// last:
// brief: log对象，线程化
//-------------------------------------------------------------------------------

#ifndef log_command_h__
#define log_command_h__

#include "../ServerDefine/active_object.h"
#include "loong.h"
// TSMPool
typedef jm_tool::TSMPool<jm_tool::CSCreatedLock> TTSMPool;
class DBCommand : public CommandObject
{
public:
	typedef TMemFun2<DWORD,CLoong,DWORD,DWORD> Tfnobj;

	DBCommand(LPVOID p,TTSMPool *,DWORD (CLoong::*fuc)(DWORD,DWORD),tagNetCmd * pMsg);
	virtual ~DBCommand();

	//void mempool(TTSMPool * p) { _pAlloc = p; }
	//TTSMPool * mempool(VOID) {return _pAlloc;}
	//void netmsg(tagNetCmd * p) { _pMsg = p;}
	//tagNetCmd * netmsg(void) { return _pMsg; }

	//void set_func(DWORD (CLoong::*pFunc)(DWORD,DWORD));

	virtual bool process() ;
	virtual void release();

private:
	Tfnobj _func;
	TTSMPool * _pAlloc;
	tagNetCmd	* _pMsg;
};

extern TTSMPool * g_pLogCmdMemPool;
extern ActiveObject g_logProcessor; // 处理log的对象

#define _USE_OTHER_THREAD_

#if defined( _USE_OTHER_THREAD_ )
#	define DBCmdCall(Method,pDBMsg) \
	do{\
		DBCommand * pCmd = NULL;\
		make_object(g_pLogCmdMemPool,DBCommand,pCmd,(this,g_pLogCmdMemPool,&CLoong::##Method,(tagNetCmd *)pDBMsg));\
		g_logProcessor.insert_cmd(pCmd);\
	}while(0)
#else
#	define DBCmdCall(Method,pDBMsg) 
#endif

#endif // log_command_h__

