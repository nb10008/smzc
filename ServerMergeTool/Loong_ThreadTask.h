//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong_ThreadTask.h
// author: songg
// actor:
// data: 2010-11-16
// last:
// brief: 线程任务
//-------------------------------------------------------------------------------
#pragma once
#include "Loong_CommonDef.h"

// 任务基类
class ThreadTaskBase
{
public:
	// 任务函数
	typedef BOOL (*TaskProc)(LPVOID pParam);

public:
	ThreadTaskBase() {}
	virtual ~ThreadTaskBase(){}

public:
	// 注册任务执行函数
	VOID RegisterTaskProc(TaskProc tp)
	{
		if(P_VALID(tp))
			m_lisTasks.PushBack(tp);
	}

	VOID Clear()
	{
		m_lisTasks.Clear();
	}

public:
	virtual BOOL Excute(LPVOID pParam) = 0;

protected:
	TSafeList<TaskProc> m_lisTasks; //任务
};

// 任务执行实现
class ThreadTaskPool: public ThreadTaskBase
{
public:
	ThreadTaskPool(){}
	virtual ~ThreadTaskPool(){}

public:
	virtual BOOL Excute(LPVOID pParam)
	{
		TaskProc tp = m_lisTasks.PopFront();
		if(!P_VALID(tp))// 找不到任务了
			return TRUE;

		return (*tp)(pParam);
	}
};

// 任务方法声明
BOOL Loong_ThreadTaskDel(LPVOID pParam);
BOOL Loong_ThreadTaskItem(LPVOID pParam);
BOOL Loong_ThreadTaskTask(LPVOID pParam);