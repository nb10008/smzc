#include "../vEngine/vEngine.h"

#define __USE_JM_TOOL_TEMP

#include "mini_tool.h"

size_t jm_tscopy_s( LPTSTR pDes, size_t stElementNum, LPCTSTR pSrc )
{
	if( !P_VALID(pDes) || stElementNum < 1 || !P_VALID(pSrc) )
		return 0;
	size_t re = 0;
	while( (*pDes++ = *pSrc++) && --stElementNum > 0 )
		++re;
	return re;
}

size_t jm_tscopy_s( LPTSTR pDes, size_t stElementNum, LPCTSTR pSrc, size_t stNumTobeCopied )
{
	if( !P_VALID(pDes) || stElementNum < 1 || !P_VALID(pSrc) || stNumTobeCopied < 1 )
		return 0;
	size_t re = 0;
	while( (*pDes++ = *pSrc++) && --stElementNum > 0 && --stNumTobeCopied > 0 )
		++re;
	return re;
}

#undef __USE_JM_TOOL_TEMP
