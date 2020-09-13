//-----------------------------------------------------------------------------
//!\file mem_map.cpp
//!\author Lyp
//!
//!\date 2004-06-17
//! last 2008-02-15
//!
//!\brief 内存分析
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "mem_map.h"
#include "../console/console_gui.h"
#include "../../Cool3D/Util/UnicodeUtil.h"
#include <stdio.h>
#include <stdlib.h>


#ifdef _MEM_TRACE_

#pragma init_seg(lib)

#undef new
#undef malloc
#undef calloc
#undef realloc
#undef free

vEngine::MemMap g_MemMap;	// 全局对象，以便于程序结束后析构

#define	MAX_MEM_FILE_SIZE  1024*1024
//-----------------------------------------------------------------------------
//! AllocateMemory
//-----------------------------------------------------------------------------
LPVOID AllocateMemory(size_t uiSize, LPCSTR szFile, INT nLine, BOOL bArray, LPVOID pAddress)
{
	if( pAddress )
		DeallocateMemory(pAddress, FALSE);

	return g_MemMap.Alloc(szFile, nLine, uiSize, bArray);
}

//-----------------------------------------------------------------------------
//! DeallocateMemory
//-----------------------------------------------------------------------------
VOID DeallocateMemory(LPVOID pAddress, BOOL bArray)
{
	if( 0 == pAddress )
		return;  // ANSI states that delete will allow NULL pointers.

	g_MemMap.Free(pAddress, bArray);
}

VOID WriteMem()
{
	g_MemMap.WriteFile();

}

VOID ShowMemUseSorted()
{
	g_MemMap.ShowMemUseSorted();
}

LPCTSTR EnvString()
{
	return g_MemMap.Env();

}


namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
MemMap::MemMap()
{
	m_dwBreakAddress = GT_INVALID;
	m_bTerminate = FALSE;
	m_dwStartTick = ::GetTickCount();
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
MemMap::~MemMap()
{
	m_bTerminate = TRUE;
	WriteFile();	// 程序结束时写出log
}


//-----------------------------------------------------------------------------
// InsertMemNode
//-----------------------------------------------------------------------------
LPVOID MemMap::Alloc(LPCSTR szFile, INT nLine, size_t uiSize, BOOL bArray)
{
	LPVOID pAddress = NULL;
	size_t uiNewSize = uiSize + sizeof(DWORD) * 2;	// 头尾防溢出标志
	
	while( true )	// 尝试分配内存
	{
		pAddress = malloc(uiNewSize);
		if( pAddress )	// 分配成功
		{
			if( m_bTerminate )
				return pAddress;
			break;
		}

		// 分配不成功，找出当前出错处理函数
		new_handler fGlobalHandler = set_new_handler(0);
		set_new_handler(fGlobalHandler);

		if( fGlobalHandler )
			(*fGlobalHandler)();	// 调用出错处理
		else 
			throw std::bad_alloc();
	}

	// 分配成功后写入首尾防溢出符号
	*(DWORD*)pAddress = 0xDeadBeef;
	*(DWORD*)((BYTE*)pAddress + uiSize + sizeof(DWORD)) = 0xDeadBeef;

	// 将指针后移到外部能用的地址
	pAddress = (BYTE*)pAddress + sizeof(DWORD);
	
	tagMemNode* pMemNode = (tagMemNode*)malloc(sizeof(tagMemNode));
	pMemNode->pAddress = pAddress;
	pMemNode->szFile = szFile;
	pMemNode->nLine = nLine;
	pMemNode->uiSize = uiSize;
	pMemNode->bArray = (bArray == TRUE);
	pMemNode->bArrayError = false;
	pMemNode->bBoundError = false;
	pMemNode->uiTickCount = ::GetTickCount();

	ASSERT( !m_mapMem.IsExist(pAddress) );
	m_mapMem.Add(pAddress, pMemNode);


	if( GT_VALID(m_dwBreakAddress) && (DWORD)pAddress == m_dwBreakAddress )
	{
		__asm int 3;	// 断点
	}

	return pAddress;
}


//-----------------------------------------------------------------------------
// RemoveMemNode
//-----------------------------------------------------------------------------
VOID MemMap::Free(LPVOID pAddress, BOOL bArray)
{
	if( m_bTerminate )	// 防止对象析构后被继续调用
		return;

	tagMemNode* pNode = m_mapMem.Peek(pAddress);
	if( P_VALID(pNode) )
	{
		m_mapMem.Erase(pAddress);
		pAddress = (BYTE*)pAddress - sizeof(DWORD);	// 得到实际地址

		BOOL bStartChanged = (*(DWORD*)pAddress != 0xDeadBeef);
		BOOL bEndChanged = (*(DWORD*)((BYTE*)pAddress + pNode->uiSize + sizeof(DWORD)) != 0xDeadBeef);

		if( bStartChanged || bEndChanged )	// 溢出检测
		{
			pNode->bBoundError = true;
			m_listBoundErrorNode.PushBack(pNode);
		}
		else if( pNode->bArray != (bArray==TRUE) ) // 数组释放检测
		{
			pNode->bArrayError = true;
			m_listArrayErrorNode.PushBack(pNode);
		}
		else	// 无错
		{
			free(pNode);	// 释放节点内容
		}
	}

	free(pAddress);	// 释放实际内存
}


//-----------------------------------------------------------------------------
// Refresh 刷新
//-----------------------------------------------------------------------------
FILE * MemMap::CreateMemFile()
{
	// 生成合适的文件名
	CHAR szFileName[MAX_PATH], szTime[MAX_PATH], szTemp[MAX_PATH];
	GetModuleFileNameA(NULL, szTemp, MAX_PATH);

	// 截去路径
	CHAR *pResult = strrchr(szTemp, '\\');
	pResult = pResult ?	pResult+1 :	pResult = (CHAR *)szTemp;
	sprintf(szFileName, "log\\%s", pResult);
	// 截去文件扩展名
	pResult = strrchr(szFileName, '.');
	if( pResult )
		*pResult = '\0';
	// 加上时间
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	szTime[0] = '\0';
	WORD wDate, wTime;
	if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
		FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	{
		// 年月日以及时分秒和进程id
		sprintf(szTime, "_Mem[%d-%d-%d %02d%02d%02d %05d].log",
			(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
			(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
			GetCurrentProcessId());
		strcat(szFileName, szTime);
	}

	FILE *fp = fopen(szFileName, "wb"); // 清空原先的内容
	if( !fp )
		return NULL;

	return fp;
}
//-----------------------------------------------------------------------------
// Refresh 刷新
//-----------------------------------------------------------------------------
VOID MemMap::WriteFile()
{
	// 生成合适的文件名
	//CHAR szFileName[MAX_PATH], szTime[MAX_PATH], szTemp[MAX_PATH];
	//GetModuleFileNameA(NULL, szTemp, MAX_PATH);

	//// 截去路径
	//CHAR *pResult = strrchr(szTemp, '\\');
	//pResult = pResult ?	pResult+1 :	pResult = (CHAR *)szTemp;
	//sprintf(szFileName, "log\\%s", pResult);
	//// 截去文件扩展名
	//pResult = strrchr(szFileName, '.');
	//if( pResult )
	//	*pResult = '\0';
	//// 加上时间
	//FILETIME CurrentTime;
	//GetSystemTimeAsFileTime(&CurrentTime);
	//szTime[0] = '\0';
	//WORD wDate, wTime;
	//if (FileTimeToLocalFileTime(&CurrentTime, &CurrentTime) &&
	//	FileTimeToDosDateTime(&CurrentTime, &wDate, &wTime))
	//{
	//	// 年月日以及时分秒和进程id
	//	sprintf(szTime, "_Mem[%d-%d-%d %02d%02d%02d %05d].log",
	//		(wDate / 32) & 15, wDate & 31, (wDate / 512) + 1980,
	//		(wTime >> 11), (wTime >> 5) & 0x3F, (wTime & 0x1F) * 2, 
	//		GetCurrentProcessId());
	//	strcat(szFileName, szTime);
	//}

	//FILE *fp = fopen(szFileName, "wb"); // 清空原先的内容
	//if( !fp )
	//	return;

	UINT unSize = 0;
	CHAR szTemp[1024] = {0};
	UINT unLen = 0;

	FILE *fp = CreateMemFile();
	if( !fp )
		return;

	std::list<tagMemNode*> listNode;
	std::list<tagMemNode*>::iterator it;
	
	// 内存泄露
	sprintf( szTemp, "-----------------  Memory Leaks  -----------------\r\n" );
	unSize += strlen(szTemp);
	fprintf( fp, szTemp);

	UINT uiTotalSize = 0;
	m_mapMem.ExportAllValue(listNode);
	std::map<std::string, tagNodeStat> leakMap;

	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		sprintf( szTemp, "%s, line:%d, size:%d, alloctime:%d, address:0x%X\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize, (*it)->uiTickCount, (*it)->pAddress );
		unSize += strlen(szTemp);
		if(unSize > MAX_MEM_FILE_SIZE)
		{
			fclose( fp );
			fp = CreateMemFile();
			if( !fp )
				return;

			unSize = 0;
		}

		fprintf(fp, szTemp);
		uiTotalSize += (*it)->uiSize;
		if ((*it)->uiTickCount - this->m_dwStartTick  > START_MEM_CHECK_TICK)
		{
			char temp[256] = {0}; 
			memset(temp, 0, sizeof(temp));
			_snprintf(temp, 256, "(%s,%d)", (*it)->szFile, (*it)->nLine);
			std::string key = temp;
			if (leakMap.find(key) == leakMap.end())
			{
				leakMap[key].nodeIdentify = key;
				leakMap[key].uiAllocCount = 1;
				leakMap[key].uiRemainSize = (*it)->uiSize;
			}
			else
			{
				leakMap[key].uiAllocCount ++;
				leakMap[key].uiRemainSize += (*it)->uiSize;
			}
		}
	}
	
	sprintf( szTemp, "Total leak size:%d\r\n\r\n", uiTotalSize );
	unSize += strlen(szTemp);
	if(unSize > MAX_MEM_FILE_SIZE)
	{
		fclose( fp );
		fp = CreateMemFile();
		if( !fp )
			return;

		unSize = 0;
	}
	//fprintf( fp, "Total leak size:%d\r\n\r\n", uiTotalSize);
	fprintf( fp, szTemp);

	uiTotalSize = 0;
	for (std::map<std::string, tagNodeStat>::iterator pos = leakMap.begin(); pos != leakMap.end(); ++pos)
	{
		sprintf( szTemp,  "%s ,count:%d, totalSize:%d\r\n", pos->second.nodeIdentify.c_str(), pos->second.uiAllocCount, pos->second.uiRemainSize );
		unSize += strlen(szTemp);
		if(unSize > MAX_MEM_FILE_SIZE)
		{
			fclose( fp );
			fp = CreateMemFile();
			if( !fp )
				return;

			unSize = 0;
		}
		//fprintf(fp, "%s ,count:%d, totalSize:%d\r\n", pos->second.nodeIdentify.c_str(), pos->second.uiAllocCount, pos->second.uiRemainSize);
		fprintf( fp, szTemp);

		uiTotalSize += pos->second.uiRemainSize;
	}
	sprintf( szTemp,  "Total leak size after %d second :%d\r\n\r\n",START_MEM_CHECK_TICK / 1000,  uiTotalSize );
	unSize += strlen(szTemp);
	if(unSize > MAX_MEM_FILE_SIZE)
	{
		fclose( fp );
		fp = CreateMemFile();
		if( !fp )
			return;

		unSize = 0;
	}
	//ffprintf( fp, "Total leak size after %d second :%d\r\n\r\n",START_MEM_CHECK_TICK / 1000,  uiTotalSize);
	fprintf( fp, szTemp);
	

	// 释放类型错误
	listNode.clear();
	fprintf( fp, "-----------------  Free Array Error  -----------------\r\n");
	m_listArrayErrorNode.Export(listNode);
	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		sprintf( szTemp,  "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize );
		unSize += strlen(szTemp);
		if(unSize > MAX_MEM_FILE_SIZE)
		{
			fclose( fp );
			fp = CreateMemFile();
			if( !fp )
				return;

			unSize = 0;
		}
		//fprintf( fp, "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize);
		fprintf( fp, szTemp);
		
	}

	// 越界错误
	listNode.clear();
	fprintf( fp, "-----------------  Bound Error  -----------------\r\n");
	m_listBoundErrorNode.Export(listNode);
	for(it= listNode.begin(); it!=listNode.end(); ++it)
	{
		sprintf( szTemp,  "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize );
		unSize += strlen(szTemp);
		if(unSize > MAX_MEM_FILE_SIZE)
		{
			fclose( fp );
			fp = CreateMemFile();
			if( !fp )
				return;

			unSize = 0;
		}
		//fprintf( fp, "%s, line:%d, size:%d\r\n", (*it)->szFile, (*it)->nLine, (*it)->uiSize);
		fprintf( fp, szTemp);
		
	}

	fclose( fp );
}

LPCTSTR MemMap::Env()
{
	static wchar_t pEnv[256];
	wsprintf(pEnv, _T("mem_map->m_mapMem(%x), mem_map->m_listBoundErrorNode(%x)"), &m_mapMem, &m_listBoundErrorNode);
	return (LPCTSTR)pEnv;
}

VOID MemMap::ShowMemUseSorted()
{
	std::list<tagMemNode*> listNode;
	listNode.clear();
	m_mapMem.ExportAllValue(listNode);
	if (listNode.empty())
	{
		return;
	}

	struct tagShowNode
	{
		CHAR szFileName[50];
		size_t uiSize;
		INT nLine;
		INT nNum;

		tagShowNode()
		{
			ZeroMemory(szFileName, 50);
			uiSize = 0;
			nLine = 0;
			nNum = 0;
		}
	};

	std::list<tagShowNode> listShow;
	listShow.clear();

	//std::list<tagMemNode*>::iterator it = listNode.begin();
	while(listNode.begin() != listNode.end())
	{
		tagMemNode* pNode = listNode.front();
		if (!P_VALID(pNode->pAddress) || pNode->szFile == NULL || pNode->uiSize != 1044)
		{
			listNode.pop_front();
			continue;
		}
		
		std::list<tagShowNode>::iterator  it = listShow.begin();
		while(it != listShow.end())
		{
			tagShowNode show = *it;
			if((*it).nLine == pNode->nLine && strcmp(pNode->szFile, (*it).szFileName) == 0)
			{
				((*it).nNum)++;
				break;
			}

			it++;
		}

		if(it == listShow.end())
		{
			tagShowNode newshow;
			sprintf_s(newshow.szFileName, 50, pNode->szFile);
			newshow.nLine = pNode->nLine;
			newshow.nNum = 1;
			newshow.uiSize = pNode->uiSize;
			listShow.push_back(newshow);
		}
		
		listNode.pop_front();
	}

	std::list<tagShowNode>::iterator  it = listShow.begin();
	while(listShow.begin() != listShow.end())
	{
		tagShowNode show = *(listShow.begin());
		IMSG(_T("\r\nAlloc memory %d times from file: %s, line:%d\n"), show.nNum, CharToWide(show.szFileName), show.nLine);
		listShow.pop_front();
	}
}

} // namespace vEngine {
#endif // #ifdef _MEM_TRACE_
