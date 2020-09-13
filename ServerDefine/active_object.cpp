//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: shielding_mgr.h
// author: Jason
// actor:
// data: 2010-12-16
// last:
// brief: 主动对象
//-------------------------------------------------------------------------------
#include "../vEngine/vEngine.h"
#include "active_object.h"

DWORD ActiveObject::svc(LPVOID paramter)
{
	while(!IsTerminate())
	{
		if(!_check())
			break;
		if( _com_queue.size() > 0 )
		{
			CommandObject * pCmd = NULL;
			if( _com_queue.pop(pCmd,100) )
			{
				if( P_VALID(pCmd) )
				{
					pCmd->process();
					pCmd->release();
				}
			}
		}
		//::Sleep(0);
		_com_queue.wait_whenprocess(100);
	}
	return 0;
}
void ActiveObject::insert_cmd(CommandObject * pCmd)
{
	if(!_check())
	{
		if( P_VALID(pCmd) )
			pCmd->release();
		return;
	}
	_com_queue.push(pCmd,100);
}
#define MAGIC_DATA (0xABADFEED)
ActiveObject::ActiveObject()
:jm_tool::JMThread(),_com_queue(0,4)
{
	for(int i = 0; i < _countof(_flag_data); ++i)
		_flag_data[i] = MAGIC_DATA;
}
ActiveObject::~ActiveObject()
{
	memset(_flag_data,-1,sizeof(_flag_data));
	clear();
}
BOOL ActiveObject::_check()
{
	int count = _countof(_flag_data);
	int mid = count / 2;
	if( _flag_data[0] == MAGIC_DATA && _flag_data[mid] == MAGIC_DATA && _flag_data[count-1] == MAGIC_DATA )
		return TRUE;
	return FALSE;
}
void ActiveObject::TerminateEx()
{
	if(!_check())
		return;
	_com_queue.terminate(TRUE);
	Terminate();
	clear();
}
int ActiveObject::ActiveEx(int num,LPVOID param)
{
	if(!_check())
		return -1;
	_com_queue.terminate(FALSE);
	return Active(num,this);
}
void ActiveObject::clear()
{
	if(!_check())
		return;
	while( _com_queue.size() > 0 )
	{
		CommandObject * pCmd = NULL;
		if( _com_queue.pop(pCmd) )
		{
			if( P_VALID(pCmd) )
			{
				pCmd->release();
			}
		}
	}
}


