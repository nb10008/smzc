///

#include "stdafx.h"
#include "kr_util.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//		Registry Utility
//
//////////////////////////////////////////////////////////////////////////

// 문자열을 읽는다.
BOOL RegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize) 
{
/*	HKEY key;
	DWORD dwDisp;
	DWORD Size;
	if(ERROR_SUCCESS != RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_READ,NULL,&key,&dwDisp))
		return FALSE;

	Size=nSize;

	if(ERROR_SUCCESS != RegQueryValueEx(key,lpValue,0,NULL,(LPBYTE)lpRet,&Size) )
	{
		strcpy(lpRet, lpDefault);
		return FALSE;
	}

	RegCloseKey(key);*/
	return TRUE;
}

// 문자열을 쓴다.
BOOL RegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	/*HKEY key;
	DWORD dwDisp;
	if(ERROR_SUCCESS != RegCreateKeyEx(hKey,lpKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisp))
		return FALSE;

	if(ERROR_SUCCESS != RegSetValueEx(key,lpValue,0,REG_SZ,(LPBYTE)lpData,(DWORD)(strlen(lpData)+1)))
		return FALSE;

	RegCloseKey(key);*/
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//
//	Convert Hex String Util
//
//

//Function to convert unsigned char to string of length 2
void Char2Hex(const unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void Hex2Char(const char* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}    

//Function to convert string of unsigned chars to string of chars
void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}

BYTE  iv[ 16 ] =
{ 0x70, 0xD7, 0x3B, 0x85, 0x09, 0xF5, 0x3C, 0x41, 0x98, 0x88, 0xC7, 0x90, 0x9D, 0xC4, 0xA6, 0xDF };		

//@@ just random generation....
BYTE key[ 32 ] =
{ 
	0x56, 0xA5, 0xED, 0x7A, 0x16, 0x3F, 0x02, 0x8C, 0x1B, 0x14, 0x62, 0x4D, 0xC5, 0x41, 0x95, 0xB6,
	0x87, 0x4A, 0xA9, 0xA8, 0xCC, 0x44, 0x56, 0x51, 0x04, 0x1F, 0xC4, 0xCE, 0x18, 0x27, 0xD0, 0xFF 
};	

#define RESULT_SUCCESS				0
#define	RESULT_UNKNOWNCONTENTS		1
#define RESULT_UNKNOWNERROR			2
#define RESULT_NOTFOUNDCONTENTS		3
#define RESULT_REGISTRYWRITEERROR	4
#define RESULT_CLIENTEXECUTEERROR	5
