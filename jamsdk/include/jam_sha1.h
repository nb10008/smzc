
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			SHA1 Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SHA1_H__
#define __JAM_SHA1_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** SHA1加密二进制 */
	JAM_API void jam_enc_sha1(byte_t sha1[20], const byte_t* input, size_t len);


	/** SHA1加密文件 */
	JAM_API void jam_enc_sha1_file(byte_t sha1[20], const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SHA1_H__ */
