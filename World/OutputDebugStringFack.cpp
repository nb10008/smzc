#include "stdafx.h"
#include "OutputDebugStringFack.h"
#include "windows.h"

#define PAGE_SIZE			4096
#define DBWIN_MAP			"DBWIN_BUFFER"
#define DBWIN_WIRTEEVENT	"DBWIN_BUFFER_READY"
#define DBWIN_READEVENT		"DBWIN_DATA_READY"
#define DBWIN_MUTEX			"DBWinMutex"

typedef struct _tag_DBMap
{
	DWORD dwProcessID;
	CHAR szString[PAGE_SIZE-sizeof(DWORD)];
} DB_MAP, *PDB_MAP;

#ifndef P_VALID
#define P_VALID(a) ((a)!=NULL && (a) != (void*)-1)
#endif
void
OutputDebugStringFackA(
					   char* lpOutputString
					   )
{
	HANDLE hMutex(NULL), hMap(NULL), hReadEvent(NULL), hWriteEvent(NULL);
	PDB_MAP pDBMap(NULL);
	while(true)
	{
		hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, DBWIN_MUTEX);
		if(!P_VALID(hMutex)) break;
		::WaitForSingleObject(hMutex, INFINITE);
		hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, DBWIN_MAP);
		if(!P_VALID(hMap)) break;
		hReadEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, DBWIN_READEVENT);
		if(!P_VALID(hReadEvent))break;
		hWriteEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, DBWIN_WIRTEEVENT);
		if(!P_VALID(hWriteEvent))break;
		if(::WaitForSingleObject(hWriteEvent, 10*1000) != WAIT_TIMEOUT)
		{
			pDBMap = (PDB_MAP)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
			if(!P_VALID(pDBMap))break;
			pDBMap->dwProcessID = ::GetCurrentProcessId();
			strcpy_s(pDBMap->szString, PAGE_SIZE-sizeof(DWORD), lpOutputString);
			pDBMap->szString[PAGE_SIZE-sizeof(DWORD)-1] = 0;
			//FlushViewOfFile((LPVOID)pDBMap, PAGE_SIZE);	
			::SetEvent(hReadEvent);
			::Sleep(0);
		}
		break;
	}

	if(P_VALID(pDBMap)) UnmapViewOfFile((LPVOID)pDBMap);
	if(P_VALID(hMap)) CloseHandle(hMap);
	if(P_VALID(hReadEvent))CloseHandle(hReadEvent);
	if(P_VALID(hWriteEvent))CloseHandle(hWriteEvent);
	if(P_VALID(hMutex))
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);	
	}
}

void
OutputDebugStringFackW(
					   wchar_t* lpOutputString
					   )
{
	HANDLE hMutex(NULL), hMap(NULL), hReadEvent(NULL), hWriteEvent(NULL);
	PDB_MAP pDBMap(NULL);
	while(true)
	{
		hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, DBWIN_MUTEX);
		if(!P_VALID(hMutex)) break;
		::WaitForSingleObject(hMutex, INFINITE);
		hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, DBWIN_MAP);
		if(!P_VALID(hMap)) break;
		hReadEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, DBWIN_READEVENT);
		if(!P_VALID(hReadEvent))break;
		hWriteEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, DBWIN_WIRTEEVENT);
		if(!P_VALID(hWriteEvent))break;
		if(::WaitForSingleObject(hWriteEvent, 10*1000) != WAIT_TIMEOUT)
		{
			pDBMap = (PDB_MAP)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
			if(!P_VALID(pDBMap))break;
			pDBMap->dwProcessID = ::GetCurrentProcessId();
			DWORD dwSize = ::WideCharToMultiByte(CP_OEMCP, NULL, lpOutputString, -1, NULL, 0, NULL, FALSE );
			if(dwSize > PAGE_SIZE-sizeof(DWORD)) dwSize = PAGE_SIZE-sizeof(DWORD);
			::WideCharToMultiByte(CP_OEMCP, NULL, lpOutputString, -1, pDBMap->szString, dwSize, NULL, FALSE );
			pDBMap->szString[PAGE_SIZE-sizeof(DWORD)-1] = 0;
			//FlushViewOfFile((LPVOID)pDBMap, PAGE_SIZE);	
			::SetEvent(hReadEvent);
			::Sleep(0);
		}
		break;
	}

	if(P_VALID(pDBMap)) UnmapViewOfFile((LPVOID)pDBMap);
	if(P_VALID(hMap)) CloseHandle(hMap);
	if(P_VALID(hReadEvent))CloseHandle(hReadEvent);
	if(P_VALID(hWriteEvent))CloseHandle(hWriteEvent);
	if(P_VALID(hMutex)) 
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);	
	}
}