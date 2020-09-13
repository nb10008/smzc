
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			String Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_STRING_H__
#define __JAM_STRING_H__

#include "jam_datatype.h"
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint8_t							utf8_t;
	typedef uint16_t						utf16_t;
	typedef uint32_t						utf32_t;


#if defined(JAM_WIN)
	#define strcasecmp						_stricmp
	#define strncasecmp						_strnicmp
	#define snprintf						_snprintf
#endif


	/** 检测字符串是否为数字串 */
	JAM_API bool jam_is_numeric(const char* str);


	/** 字符串转小写 */
	JAM_API char* jam_lowercase(char* str);


	/** 字符串转大写 */
	JAM_API char* jam_uppercase(char* str);


	/** 去除字符串左边指定字节 */
	JAM_API char* jam_trim_left(char* str, char ch);


	/** 去除字符串右边指定字节 */
	JAM_API char* jam_trim_right(char* str, char ch);


	/** 去除字符串指定字节 */
	#define jam_trim(str, ch)					jam_trim_left(jam_trim_right(str, ch), ch)


	/** 字符串转二进制 */
	JAM_API size_t jam_string_to_hex(byte_t* bin, size_t size, const char* str);


	/** 二进制转字符串 */
	JAM_API size_t jam_hex_to_string(char* str, size_t len, const byte_t* bin, size_t size);


	/** ASCII 转 UTF8 */
	JAM_API size_t jam_ascii_to_utf8(utf8_t* utf8, size_t len, const char* ascii, size_t count);
	/** ASCII 转 UTF16 */
	JAM_API size_t jam_ascii_to_utf16(utf16_t* utf16, size_t len, const char* ascii, size_t count);
	/** ASCII 转 UTF32 */
	JAM_API size_t jam_ascii_to_utf32(utf32_t* utf32, size_t len, const char* ascii, size_t count);

	/** UTF8 转 ASCII */
	JAM_API size_t jam_utf8_to_ascii(char* ascii, size_t len, const utf8_t* utf8, size_t count);
	/** UTF8 转 UTF16 */
	JAM_API size_t jam_utf8_to_utf16(utf16_t* utf16, size_t len, const utf8_t* utf8, size_t count);
	/** UTF8 转 UTF32 */
	JAM_API size_t jam_utf8_to_utf32(utf32_t* utf32, size_t len, const utf8_t* utf8, size_t count);

	/** UTF16 转 ASCII */
	JAM_API size_t jam_utf16_to_ascii(char* ascii, size_t len, const utf16_t* utf16, size_t count);
	/** UTF16 转 UTF8 */
	JAM_API size_t jam_utf16_to_utf8(utf8_t* utf8, size_t len, const utf16_t* utf16, size_t count);
	/** UTF16 转 UTF32 */
	JAM_API size_t jam_utf16_to_utf32(utf32_t* utf32, size_t len, const utf16_t* utf16, size_t count);

	/** UTF32 转 ASCII */
	JAM_API size_t jam_utf32_to_ascii(char* ascii, size_t len, const utf32_t* utf32, size_t count);
	/** UTF32 转 UTF8 */
	JAM_API size_t jam_utf32_to_utf8(utf8_t* utf8, size_t len, const utf32_t* utf32, size_t count);
	/** UTF32 转 UTF16 */
	JAM_API size_t jam_utf32_to_utf16(utf16_t* utf16, size_t len, const utf32_t* utf32, size_t count);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_STRING_H__ */
