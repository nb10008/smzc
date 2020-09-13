
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Dynamic Link Library Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_DL_H__
#define __JAM_DL_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 读取一个动态链接库 */
	JAM_API handle_t jam_dl_load(const char* filename);


	/** 获取函数指针 */
	JAM_API void* jam_dl_proc(handle_t handle, const char* func);


	/** 释放链接库句柄 */
	JAM_API void jam_dl_free(handle_t handle);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_DL_H__ */
