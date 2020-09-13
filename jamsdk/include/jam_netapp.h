
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Network Application Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_NET_APP_H__
#define __JAM_NET_APP_H__

#include "jam_socket.h"
#include "jam_packet.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef void (*on_msg_t)(socket_t sock, sockaddr_t host, uint16_t port, packet_t pkt, void* param);


	/** 初始化服务 */
	JAM_API bool jam_svc_init();


	/** 关闭服务 */
	JAM_API bool jam_svc_cleanup();


	/** 关闭所有套接字 */
	JAM_API size_t jam_svc_close_all(bool wait);


	/** 托管一个套接字 */
	JAM_API bool jam_svc_insert(socket_t sock);


	/** 移除一个套接字 */
	JAM_API void jam_svc_remove(socket_t sock);


	/** 为套接字绑定一个数据 */
	JAM_API bool jam_svc_bind(socket_t sock, void* data);


	/** 获取套接字自定义数据 */
	JAM_API void* jam_svc_userdata(socket_t sock);


	/** 目前托管的连接数 */
	JAM_API size_t jam_svc_size();


	/** 解析缓存到数据包 */
	JAM_API bool jam_svc_parse(on_msg_t on_msg, socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_NET_APP_H__ */
