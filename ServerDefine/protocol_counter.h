//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: protocol_counter.h
// author: Jason
// actor:
// data: 2010-12-31
// last:
// brief: 网络协议计数器
//-------------------------------------------------------------------------------

#ifndef PROTOCOL_COUNTER_H
#define PROTOCOL_COUNTER_H
#include <map>
#include <string>
namespace vEngine{

class ProtocolCounter
{
	std::map<DWORD ,std::string> _map_proid_name; // 协议id与协议名对应
	std::map<DWORD,DWORD> _map_proid_counter;// 协议id与协议发送或者接收的条数
	std::map<DWORD,DWORD> _map_proid_zero;
	std::map<DWORD,DWORD> _map_proid_size;
	char _buf[100];
public:
	// 从文件读取协议id与名称对应关系
	BOOL LoadIDNameFromFile(LPCSTR strFileName);
	// 计数，给某个协议加1，该方法目前是线程安全的
	DWORD Count(DWORD dwCmdID, DWORD dwSize);
	// 输出到文件
	BOOL DumpCountersToFile(LPCSTR strFile);
	// 清空
	VOID Clear();

	VOID SetLocale(LPCTSTR strLanguage = NULL);
};
}
#endif // PROTOCOL_COUNTER_H

