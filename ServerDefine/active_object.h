//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: shielding_mgr.h
// author: Jason
// actor:
// data: 2010-12-16
// last:
// brief: 主动对象
//-------------------------------------------------------------------------------

#ifndef ACTIVEOBJECTH
#define ACTIVEOBJECTH

#include "mini_tool.h"
#include <queue>

class CommandObject
{
protected:
	LPVOID	m_ptr;	// 回调对象
public:
	CommandObject(LPVOID p)
		:m_ptr(p){}
	virtual ~CommandObject(){}
	virtual bool process() = 0;
	virtual void release() {delete this;} // 基于内存池的自己重载一下
};

/**
* \ingroup mythread
*
* 主动对象
*
* \version 1.0
* first version
*
* \date 2010-12-16
*
* \author Jason
*
*/
class ActiveObject : public jm_tool::JMThread
{
	DWORD _flag_data[10];
	BOOL _check();
	jm_tool::JQueue<CommandObject*,jm_tool::tagJSyn,std::queue<CommandObject*> > _com_queue;
protected:
	virtual DWORD svc(LPVOID paramter);
	void clear();
public:
	void insert_cmd(CommandObject * pCmd);

	ActiveObject();
	virtual ~ActiveObject();
	void TerminateEx();
	int ActiveEx(int num,LPVOID param);
	size_t Num()
	{
		return _com_queue.size();
	}
};



#endif // ACTIVEOBJECTH

