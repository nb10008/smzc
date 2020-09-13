

#pragma once

#define IDS_HELLO                       1

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
BOOL RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);

void Char2Hex(const unsigned char ch, char* szHex);
void Hex2Char(const char* szHex, unsigned char& rch);

void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize);
void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);

extern BYTE  iv[];
extern BYTE key[];