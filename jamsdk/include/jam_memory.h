
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Memroy Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MEMORY_H__
#define __JAM_MEMORY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 分配内存 */
	JAM_API void* __jam_malloc(size_t size, const char* filename, const char* func, int32_t line);


	/** 重新分配内存 */
	JAM_API void* __jam_realloc(void* mem, size_t size, const char* filename, const char* func, int32_t line);


	/** 释放内存 */
	JAM_API void __jam_free(void* p, const char* filename, const char* func, int32_t line);


	/** 获取系统页大小(单位: 字节) */
	JAM_API size_t jam_page_size();


	/** 获取内存总大小(单位: 字节) */
	JAM_API size_t jam_memory_total_size();


	/** 获取内存剩余大小(单位: 字节) */
	JAM_API size_t jam_memory_avail_size();


	#define jam_malloc(n)				__jam_malloc(n, __FILE__, __FUNCTION__, __LINE__)
	#define jam_realloc(m, n)			__jam_realloc(m, n, __FILE__, __FUNCTION__, __LINE__)
	#define jam_free(p)					__jam_free(p, __FILE__, __FUNCTION__, __LINE__)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MEMORY_H__ */
