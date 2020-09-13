
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			CPU Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CPU_H__
#define __JAM_CPU_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 获取 CPU 个数 */
	JAM_API int32_t jam_cpu_num();


	/** 是否是高字节对齐 CPU */
	JAM_API bool jam_is_big_endian();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CPU_H__ */
