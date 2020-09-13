
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			MD5 Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MD5_H__
#define __JAM_MD5_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** MD5加密二进制 */
	JAM_API void jam_enc_md5(byte_t md5[16], const byte_t* input, size_t len);


	/** MD5加密文件 */
	JAM_API void jam_enc_md5_file(byte_t md5[16], const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MD5_H__ */
