
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Hash Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_HASH_H__
#define __JAM_HASH_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 快速哈希 */
	JAM_API uint32_t jam_fast_hash(const byte_t* buffer, size_t len);


	/** 哈希 */
	JAM_API uint32_t jam_hash_normal(const byte_t* buffer, size_t len);


	/** 产生64位哈希 */
	#define jam_hash(buffer, len)			jam_make_qword(jam_fast_hash(buffer, len), jam_hash_normal(buffer, len))


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_HASH_H__ */
