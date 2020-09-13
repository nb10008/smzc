#include "../vEngine/vEngine.h"
#include "protocol_counter.h"
#include <fstream>
#include <locale.h>
#ifdef _UNICODE
#include <wchar.h>
#endif
namespace vEngine{

BOOL ProtocolCounter::LoadIDNameFromFile(LPCSTR strFileName)
{
	TObjRef<Util> pUtil;
	_map_proid_counter.clear();
	_map_proid_name.clear();
	_map_proid_zero.clear();
	_map_proid_size.clear();
	std::ifstream fs(strFileName);
	if(!fs.is_open())
		return FALSE;
	memset(_buf,0,sizeof(_buf));
	while(!fs.eof())
	{
		_buf[0] = 0;
		fs.getline(_buf,100);
		std::string tem = _buf;
		int pos = tem.find('\t');
		if( pos != std::string::npos )
		{
			std::string name = tem.substr(0,pos);
// 			std::string id = tem.substr(pos+1,tem.length()-pos-1);
// 			DWORD dwID = atoi(id.c_str());
			DWORD dwID = pUtil->Crc32(name.c_str());
			_map_proid_name[dwID] = name;
			_map_proid_zero[dwID] = 0;
		}
	}
	fs.close();
	_map_proid_counter = _map_proid_zero;
	_map_proid_size = _map_proid_zero;
	return TRUE;
}
DWORD ProtocolCounter::Count(DWORD dwCmdID, DWORD dwSize)
{
	std::map<DWORD,DWORD>::iterator iCounter = _map_proid_counter.find(dwCmdID);
	std::map<DWORD,DWORD>::iterator iSize = _map_proid_size.find(dwCmdID);
	if( iCounter != _map_proid_counter.end() && iSize != _map_proid_size.end())
	{
		//i->second = i->second + 1;
		InterlockedIncrement((LPLONG)&(iCounter->second));
		InterlockedExchangeAdd((LPLONG)&iSize->second, dwSize);
		return iCounter->second;
	}
	return 0;
}
VOID MoveFilePos(HANDLE dwFileHandle, DWORD dwDistanceToMove, 
				 DWORD dwMoveMethod)
{
	LARGE_INTEGER li, liOut;
	li.HighPart = 0;
	li.LowPart = dwDistanceToMove;
	liOut.HighPart = 0;
	liOut.LowPart = dwDistanceToMove;
	SetFilePointerEx((HANDLE)dwFileHandle, li, &liOut, dwMoveMethod);
}
BOOL ProtocolCounter::DumpCountersToFile(LPCSTR strFile)
{
	//std::ofstream fs(strFile);
	//if(fs.bad())
	//	return FALSE;
	HANDLE hFile = ::CreateFileA(strFile,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	char buf[MAX_PATH] = {0};
	DWORD dwLen = 0;
	for(std::map<DWORD,DWORD>::iterator i = _map_proid_counter.begin(); i != _map_proid_counter.end(); ++i)
	{
		if( i->second == 0 )
			continue;
		std::map<DWORD,DWORD>::iterator iSize = _map_proid_size.find(i->first);
		if (iSize == _map_proid_size.end())
			continue;
		std::map<DWORD ,std::string>::iterator iFind = _map_proid_name.find(i->first);
		if( iFind != _map_proid_name.end() )
		{
			//fs << iFind->second << "\t" << i->second << std::endl;
			sprintf(buf,"%s\t%u\t%u\r\n",iFind->second.c_str(),i->second, iSize->second);
		}
		else
		{
			//fs << i->first << "\t" << i->second << std::endl;
			sprintf(buf,"%u\t%u\t%u\r\n",i->first,i->second, iSize->second);
		}
		//fs.write(buf,strlen(buf));
		MoveFilePos(hFile, 0, GT_SEEK_END);
		if( !::WriteFile(hFile,buf,strlen(buf),&dwLen,NULL) )
			break;
	}
//	fs.close();
	::CloseHandle(hFile);
	return TRUE;
}
VOID ProtocolCounter::Clear()
{
	_map_proid_counter.clear();
	_map_proid_counter = _map_proid_zero;
	_map_proid_size = _map_proid_zero;
}
VOID ProtocolCounter::SetLocale(LPCTSTR strLanguage)
{
#ifdef _UNICODE
	_wsetlocale(LC_ALL,strLanguage ? _T(""):strLanguage);
#else
	setlocale(LC_ALL,strLanguage ? "":strLanguage);
#endif
}
}