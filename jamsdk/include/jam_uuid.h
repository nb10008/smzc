
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			UUID Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_UUID_H__
#define __JAM_UUID_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** Éú³É UUID */
	JAM_API void jam_uuid_generate(byte_t uuid[16]);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_UUID_H__ */
