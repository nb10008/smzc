
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Packet Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_PACKET_H__
#define __JAM_PACKET_H__

#include "jam_datatype.h"
#include "jam_socket.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_PKT_BROKEN					(-1)		/** 数据包不完整 */
	#define JAM_PKT_ERROR					(-2)		/** 错误的数据包 */


	typedef handle_t						packet_t;


	/** 分配打包句柄 */
	JAM_API packet_t jam_pkt_alloc(size_t len);


	/** 释放打包句柄 */
	JAM_API void jam_pkt_free(packet_t pkt);


	/** 制作一个副本 */
	JAM_API packet_t jam_pkt_clone(packet_t pkt);


	/** 重置 */
	JAM_API void jam_pkt_reset(packet_t pkt);


	/** 调整缓存大小 */
	JAM_API void jam_pkt_resize(packet_t pkt, size_t size);


	/** 当前数据大小 */
	JAM_API size_t jam_pkt_size(packet_t pkt);


	/** 最大容量 */
	JAM_API size_t jam_pkt_max_size(packet_t pkt);


	/** 当前指针位置 */
	JAM_API uint32_t jam_pkt_tell(packet_t pkt);


	/** 设置指针 */
	JAM_API void jam_pkt_seek(packet_t pkt, uint32_t offset);


	/** 写入指定大小的二进制 */
	JAM_API bool jam_pkt_write_binary(packet_t pkt, const byte_t* buffer, size_t len);


	/** 写入一个8位整数 */
	JAM_API bool jam_pkt_write_int8(packet_t pkt, uint8_t i8);


	/** 写入一个16位整数 */
	JAM_API bool jam_pkt_write_int16(packet_t pkt, uint16_t i16);


	/** 写入一个32位整数 */
	JAM_API bool jam_pkt_write_int32(packet_t pkt, uint32_t i32);


	/** 写入一个64位整数 */
	JAM_API bool jam_pkt_write_int64(packet_t pkt, uint64_t i64);


	/** 写入一个字符串 */
	JAM_API bool jam_pkt_write_string(packet_t pkt, const char* str);


	/** 读取一个字符串 */
	JAM_API const char* jam_pkt_read_string(packet_t pkt);


	/** 读取指定大小的二进制 */
	JAM_API const byte_t* jam_pkt_read_binary(packet_t pkt, size_t len);


	/** 读取一个8位整数 */
	JAM_API uint8_t jam_pkt_read_int8(packet_t pkt);


	/** 读取一个16位整数 */
	JAM_API uint16_t jam_pkt_read_int16(packet_t pkt);


	/** 读取一个32位整数 */
	JAM_API uint32_t jam_pkt_read_int32(packet_t pkt);


	/** 读取一个64位整数 */
	JAM_API uint64_t jam_pkt_read_int64(packet_t pkt);


	/** 从尾部读取指定大小的二进制 */
	JAM_API const byte_t* jam_pkt_reverse_read_binary(packet_t pkt, size_t len);


	/** 从尾部读取一个8位整数 */
	JAM_API uint8_t jam_pkt_reverse_read_int8(packet_t pkt);


	/** 从尾部读取一个16位整数 */
	JAM_API uint16_t jam_pkt_reverse_read_int16(packet_t pkt);


	/** 从尾部读取一个32位整数 */
	JAM_API uint32_t jam_pkt_reverse_read_int32(packet_t pkt);


	/** 从尾部读取一个64位整数 */
	JAM_API uint64_t jam_pkt_reverse_read_int64(packet_t pkt);


	/** 解析一个二进制, 如果失败返回 JAM_PKT_BROKEN 或 JAM_PKT_ERROR, 否则返回解析字节数 */
	JAM_API int32_t jam_pkt_parse(packet_t pkt, const byte_t* buffer, size_t len);


	/** 检测加密后需要的空间字节数 */
	JAM_API size_t jam_pkt_build_size(packet_t pkt);


	/** 加密并压缩数据 */
	JAM_API size_t jam_pkt_build(packet_t pkt, byte_t* buffer, size_t len);


	/** TCP 发送数据包 */
	JAM_API void jam_pkt_send(socket_t sock, packet_t pkt, int32_t flag);


	/** UDP 发送数据包 */
	JAM_API void jam_pkt_sendto(socket_t sock, packet_t pkt, sockaddr_t host, uint16_t port, int32_t flag);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_PACKET_H__ */
