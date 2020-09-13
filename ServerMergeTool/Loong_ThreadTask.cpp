//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Loong_ThreadTask.cpp
// author: songg
// actor:
// data: 2010-11-16
// last:
// brief: 线程任务
//-------------------------------------------------------------------------------
#include "StdAfx.h"
#include "Loong_ThreadTask.h"
#include "ServerMerge.h"
#include "Loong_ServerMerge.h"
#include "Loong_OutputCtrl.h"
#include "Loong_TMethod.h"


//---------------------------------------------------------------------------------------
// itemdel表 equipdel表
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskDel(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//////////////////////////////////////////////////////////////////////////
	//itemdel
	g_clsOutput.Output(_T("开始合并--itemdel!\r\n"));
	tagTWrap_Loong_ItemDel *TypeItemDel = NULL;
	BOOL bRet = TMethod_Loong_PagingServerMerge(TypeItemDel,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--itemdel!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--itemdel!\r\n"));

	//equipdel
	g_clsOutput.Output(_T("开始合并--equipdel!\r\n"));
	tagTWrap_Loong_EquipDel *TypeEquipDel = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeEquipDel,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--equipdel!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--equipdel!\r\n"));

	return TRUE;
}

//---------------------------------------------------------------------------------------
// item表 item_baibao表 equip表 equip_baibao  (item itembaibao equip equipbaibao顺序不能变)
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskItem(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//////////////////////////////////////////////////////////////////////////
	//item
	g_clsOutput.Output(_T("开始合并--item!\r\n"));
	tagTWrap_Loong_Item *TypeItem = NULL;
	BOOL bRet = TMethod_Loong_AccountServerMerge(TypeItem,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--item!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--item!\r\n"));

	//////////////////////////////////////////////////////////////////////////
	//itembaibao
	g_clsOutput.Output(_T("开始合并--itembaibao!\r\n"));
	tagTWrap_Loong_ItemBaiBao *TypeItemBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemBaiBao,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并itembaibao失败!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--itembaibao!\r\n"));

	//equip
	g_clsOutput.Output(_T("开始合并--equip!\r\n"));
	tagTWrap_Loong_Equip *TypeEquip = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeEquip,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--equip!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--equip!\r\n"));

	//equipbaibao
	g_clsOutput.Output(_T("开始合并--Equipbaibao!\r\n"));
	tagTWrap_Loong_EquipBaiBao *TypeEquipBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeEquipBaiBao,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--Equipbaibao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--Equipbaibao!\r\n"));

	return TRUE;
}

//---------------------------------------------------------------------------------------
// task表 task_done表
//---------------------------------------------------------------------------------------
BOOL Loong_ThreadTaskTask(LPVOID pParam)
{
	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	//task
	g_clsOutput.Output(_T("开始合并--Task!\r\n"));
	tagTWrap_Loong_Task *TypeTask = NULL;
	BOOL bRet = TMethod_Loong_PagingServerMerge(TypeTask,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--Task!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--Task!\r\n"));

	//taskdone
	g_clsOutput.Output(_T("开始合并--TaskDone!\r\n"));
	tagTWrap_Loong_TaskDone *TypeTaskDone = NULL;
	bRet = TMethod_Loong_PagingServerMerge(TypeTaskDone,pPara->m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--TaskDone!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--TaskDone!\r\n"));

	return TRUE;
}