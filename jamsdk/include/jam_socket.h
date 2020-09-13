
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Socket Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SOCKET_H__
#define __JAM_SOCKET_H__

#include "jam_addr.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_MAX_CONN_SIZE				65535		/** 最大连接数(包含TCP与UDP) */


	typedef enum
	{
		JAM_SERVER_TCP = 1,
		JAM_SERVER_UDP,
		JAM_CLIENT_TCP,
		JAM_CLIENT_UDP,
	}sock_type_t;


	typedef handle_t						socket_t;

	typedef void (*on_accept_t)(socket_t sock, void* param);
	typedef void (*on_lost_t)(socket_t sock, void* param);
	typedef void (*on_data_t)(socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param);


	/** 初始化网络资源 */
	JAM_API bool jam_network_init();


	/** 清理网络资源 */
	JAM_API bool jam_network_cleanup();


	/** 创建一个TCP服务器连接 */
	JAM_API socket_t jam_server_tcp(uint16_t port, void* param, on_accept_t onaccept, on_lost_t onlost, on_data_t ondata);


	/** 创建一个TCP客户端连接 */
	JAM_API socket_t jam_client_tcp(sockaddr_t host, uint16_t port, void* param, on_lost_t onlost, on_data_t ondata);


	/** 创建一个UDP服务器连接 */
	JAM_API socket_t jam_server_udp(uint16_t port, void* param, on_data_t ondata);


	/** 创建一个UDP客户端连接 */
	JAM_API socket_t jam_client_udp(void* param, on_data_t ondata);


	/** 获取连接类型 */
	JAM_API sock_type_t jam_socket_type(socket_t sock);


	/** 关闭连接 */
	JAM_API bool jam_socket_close(socket_t sock);


	/** 进行一次TCP发送操作 */
	JAM_API void jam_socket_send(socket_t sock, const void* buffer, size_t len, int32_t flag);


	/** 进行一次UDP发送操作 */
	JAM_API void jam_socket_sendto(socket_t sock, const void* buffer, size_t len, sockaddr_t host, uint16_t port, int32_t flag);


	/** 设置网络限速(单位: 字节) */
	JAM_API void jam_socket_limit(socket_t sock, uint32_t speed);


	/** 获取接收字节数 */
	JAM_API size_t jam_socket_recv_bytes(socket_t sock);


	/** 获取发送字节数 */
	JAM_API size_t jam_socket_sent_bytes(socket_t sock);


	/** 获取IP地址 */
	JAM_API sockaddr_t jam_socket_addr(socket_t sock);


	/** 获取远程端口 */
	JAM_API uint16_t jam_socket_remote_port(socket_t sock);


	/** 获取本地端口 */
	JAM_API uint16_t jam_socket_local_port(socket_t sock);


	/** 关闭缓存 */
	JAM_API void jam_socket_no_buffer(socket_t sock, bool enable);


	/** 设置缓存大小 */
	JAM_API void jam_socket_buffer_size(socket_t sock, size_t len);


	/** 设置超时时间 */
	JAM_API void jam_socket_set_timeout(socket_t sock, uint32_t timeout);


	/** 获取超时时间 */
	JAM_API uint32_t jam_socket_get_timeout(socket_t sock);


	/** 设置从容关闭时间 */
	JAM_API void jam_socket_set_linger(socket_t sock, uint32_t time);


	/** I/O命令操作 */
	JAM_API bool jam_socket_ioctl(socket_t sock, int32_t command, uint32_t* arg);


	/** 设置选项 */
	JAM_API bool jam_socket_set_option(socket_t sock, int32_t level, int32_t name, const void* value, int32_t len);


	/** 获取选项 */
	JAM_API bool jam_socket_get_option(socket_t sock, int32_t level, int32_t name, void* value, int32_t* len);


	/** 获取套接字系统句柄 */
	JAM_API int32_t jam_socket_fd(socket_t sock);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SOCKET_H__ */
