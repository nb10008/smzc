
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Log System Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LG_H__
#define __JAM_LG_H__

#include "jam_trace.h"


#ifdef __cplusplus
extern "C" {
#endif


	/**
		初始化快速日志系统
		日志文件格式: {path}/{yymm}/{dd}/{name}_{hh}_{mm}_{ss}_{ms}.log}
	 */
	JAM_API bool jam_lg_init(const char* path, const char* name, size_t len);


	/** 清理日志系统 */
	JAM_API bool jam_lg_cleanup();


	/** 日志是否输出日期时间 */
	JAM_API void jam_lg_enable_time(bool enable);


	/** 打印日志 */
	JAM_API void jam_lg_enable_print(bool prefix, bool trace);


	/** 日志文件名 */
	JAM_API const char* jam_lg_filename();


	/** 快速日志记录 */
	JAM_API void jam_lg(const char* prefix, const char* format, ...);
	/** 快速日志记录 */
	JAM_API void jam_vlg(const char* prefix, const char* format, va_list argv);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LG_H__ */
