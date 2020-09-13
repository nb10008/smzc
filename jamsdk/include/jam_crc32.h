
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			CRC32 Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CRC32_H__
#define __JAM_CRC32_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 计算 CRC32 值 */
	JAM_API uint32_t jam_crc32(uint32_t crc, const byte_t* buffer, size_t len);


	/** 计算文件 CRC32 值 */
	JAM_API uint32_t jam_crc32_file(const char* filename);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CRC32_H__ */
