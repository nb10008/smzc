
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			I/O Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_I_O_H__
#define __JAM_I_O_H__

#include "jam_datatype.h"
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


	/** 检测文件或者文件夹是否存在 */
	JAM_API bool jam_file_exist(const char* filename);


	/** 获取文件大小 */
	JAM_API int32_t jam_file_length(const char* filename);
	/** 获取文件大小 */
	JAM_API int32_t jam_file_length_fp(FILE* fp);


	/** 创建文件夹(自动创建路径上不存在的文件夹) */
	JAM_API bool jam_path_make(const char* path);


	/** 检测路径(文件)名是否符合操作系统要求 */
	JAM_API bool jam_path_valid(const char* path);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_I_O_H__ */
