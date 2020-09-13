#ifndef _OUT_PUT_DEBUG_STRING_FACK_H_ 
#define _OUT_PUT_DEBUG_STRING_FACK_H_

void
OutputDebugStringFackA(
				   char* lpOutputString
				   );

void
OutputDebugStringFackW(
				   wchar_t* lpOutputString
				   );
#ifdef UNICODE
#define OutputDebugStringFack  OutputDebugStringFackW
#else
#define OutputDebugStringFack  OutputDebugStringFackA
#endif // !UNICODE
#endif