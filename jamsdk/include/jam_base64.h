
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Base64 Encrypt/Decrypt Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_BASE64_H__
#define __JAM_BASE64_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** Base64 加密需要的字节数 */
	JAM_API size_t jam_base64_enc_size(size_t len);


	/** Base64 加密 */
	JAM_API size_t jam_base64_enc(char* output, size_t olen, const byte_t* input, size_t ilen);


	/** Base64 解密需要的字节数 */
	JAM_API size_t jam_base64_dec_size(size_t len);


	/** Base64 解密 */
	JAM_API size_t jam_base64_dec(byte_t* output, size_t olen, const char* input);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_BASE64_H__ */
