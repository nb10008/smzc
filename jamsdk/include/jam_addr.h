
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Address Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ADDR_H__
#define __JAM_ADDR_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t					sockaddr_t;


	/** 转换字符串到IP地址(支持域名) */
	JAM_API sockaddr_t jam_host_to_ip(const char* host);


	/** 转换IP到字符串 */
	JAM_API void jam_ip_to_host(char addr[JAM_MAX_PATH], sockaddr_t ipv4);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ADDR_H__ */
