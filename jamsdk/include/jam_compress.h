
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Compress Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_COMPRESS_H__
#define __JAM_COMPRESS_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 获取压缩所需的字节数 */
	JAM_API size_t jam_compress_bound(size_t size);


	/** 压缩 */
	JAM_API size_t jam_compress(byte_t* output, size_t output_len, const byte_t* input, size_t input_len);


	/** 解压 */
	JAM_API size_t jam_uncompress(byte_t* output, size_t output_len, const byte_t* input, size_t input_len);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_COMPRESS_H__ */
