
#include "to_listen.h"
#include "serv_list.h"
#include "user_list.h"
#include "event.h"


#define BUF_GET(buf)						(buf + sizeof(uint16_t))
#define BUF_SIZE(buf)						(*((uint16_t*)buf))
#define BUF_WRITE(buf, data, size)			do{ BUF_SIZE(buf) = (uint16_t)size; memcpy(BUF_GET(buf), data, size); }while(0)
#define BUF_ZERO(buf)						memset(buf, 0, BUFFER_SIZE)



typedef struct
{
	socket_t			listen;
	arrayset_t			fd_buf;
	list_t				conn_list;
}listen_handle_t;


static listen_handle_t			g_listen = {0};


/* 连接事件 */
static void __on_accept(socket_t sock, void* param)
{
    char ip[JAM_MAX_PATH] = {0};

	jam_list_push_back(g_listen.conn_list, sock);

	/* 10秒后检测套接字连接状态 */
	event_timer_add(PTR_TO_ID(sock), 10 * 1000, validate_conn, sock);

    jam_ip_to_host(ip, jam_socket_addr(sock));
    jam_lg(LG_INF, "connected:%s.", ip);
}


/* 断线事件 */
static void __on_lost(socket_t sock, void* param)
{
    char ip[JAM_MAX_PATH] = {0};
	byte_t* buf;

	serv_list_erase(sock);
	event_timer_remove(PTR_TO_ID(sock));
	/* 删除套接字占用的缓存 */
	if(buf = (byte_t*)jam_arrayset_get(g_listen.fd_buf, jam_socket_fd(sock)))
	{
		jam_arrayset_erase(g_listen.fd_buf, jam_socket_fd(sock));
		jam_free(buf);
	}
	/* 从连接队列中删除 */
	if(jam_list_cursor_first(g_listen.conn_list))
	{
		socket_t fd;
		do
		{
			fd = (socket_t)jam_list_cursor_data(g_listen.conn_list);
			if(fd == sock)
			{
				jam_list_cursor_erase(g_listen.conn_list);
				break;
			}
		}while(jam_list_cursor_next(g_listen.conn_list));
	}

    jam_ip_to_host(ip, jam_socket_addr(sock));
    jam_lg(LG_INF, "disconnected:%s.", ip);
}


/* 网络命令 */
static int __on_command(socket_t sock, uint32_t cmd, const void* buffer, size_t len, void* param)
{
	switch(cmd)
	{
	case CMD_NC_LOGIN_CM:
		{
			uint32_t section_id = *(uint32_t*)buffer;
			serv_t* serv = serv_list_get_by_sectionid(section_id);
			if(serv != NULL) close_connection(serv->fd);
			if(!serv_list_put(sock, section_id))
			{
				jam_socket_close(sock);
			}			
			else
			{
				jam_lg(LG_INF, "connected: %d.", section_id);
			}
		}	break;
	case CMD_NC_ANTIENTHRALL:
		{
			serv_t* serv = serv_list_get(sock);
			if(serv)
			{
				uint32_t count = *(uint32_t*)buffer;
				char* p = (char*)buffer + sizeof(uint32_t);
				while(count--)
				{
					user_update(p, serv->sid);
					p += 16;
				}
			}
		}	break;
	case CMD_NC_ANTIQUERYSTATE:
		{
			char* p = (char*)buffer; 
			uint32_t online_time = 0;
			online_info_t* oi = user_get(p);
			if(oi!=NULL)
			{
				//pInfo->next_notify = 1; //notify at next wake up;
				online_time = oi->online_period;
			}
			send_user_state(sock, p, online_time);
		} break;
	default:
		return 0; break;
	}
	return 1;
}

int32_t send_user_state(socket_t socket, char* name, uint32_t online_time)
{
	uint32_t plen = 29, mlen = 21;
	uint32_t cmd = CMD_NS_ANTIENTHRALL;
	byte_t buf[33] = {0};
	serv_t* s;
	byte_t color = 1;
	if(online_time<= GREEN_TIME)
	{
		/* 在线时间小于 3 小时, 1 小时通知一次"绿色"时间 */
		color = COLOR_GREEN;
	}
	else if(online_time <= RED_TIME)
	{
		/* 在线时间在 3 - 5 小时之间, 30 分钟通知一次"橙色"时间 */
		color = COLOR_ORANGE;
	}
	else
	{
		/* 在线时间超过 5 小时, 15 分钟通知一次"红色"时间 */
		color = COLOR_RED;
	}
	online_time = online_time/1000;
	//uint32_t online_time = oi->online_period/1000;

	memcpy(buf, &plen, sizeof(uint32_t));
	memcpy(buf + 4, &cmd, sizeof(uint32_t));
	memcpy(buf + 8, &mlen, sizeof(uint32_t));
	strncpy(buf + 12, name, 16);
	memcpy(buf + 28, &color, sizeof(int8_t));
	memcpy(buf + 29, &online_time, sizeof(uint32_t));
	jam_socket_send(socket, buf, plen+4, 0);
	//jam_lg(LG_INF, "send user state, username(%s) state(%u) onlinetime(%u).", name, color, online_time);
	return 0;
}

/* 网络消息 */
static void __on_data(socket_t sock, sockaddr_t host, uint16_t port, const void* buffer, size_t len, void* param)
{
	uint32_t cmd, p_len, m_len;
	byte_t temp[BUFFER_SIZE] = {0};
	byte_t* ptr = temp;
	serv_t* serv = NULL;
	uint16_t bufSize = 0;
	size_t bufLen=0;
	byte_t* pBuffer = (byte_t*)buffer;

	/* 缓存拼接 */
	byte_t* buf = (byte_t*)jam_arrayset_get(g_listen.fd_buf, jam_socket_fd(sock));
	if(!buf)
	{
		buf = (byte_t*)jam_malloc(BUFFER_SIZE);
		BUF_ZERO(buf);
		jam_arrayset_put(g_listen.fd_buf, jam_socket_fd(sock), buf);
	}	
	
	while(len >0)
	{	
		size_t csize;
		bufLen = BUF_SIZE(buf);
		if(bufLen >0)
		{
			memcpy(temp, BUF_GET(buf), bufLen);
			BUF_ZERO(buf);		
		}

		csize =  MIN(len,BUFFER_SIZE-bufLen);
		if(csize == 0) 
			goto error;

		ptr = temp;
		memcpy(temp + bufLen, pBuffer, csize);
		len -= csize;
		bufLen += csize;	
		pBuffer += csize;		

		while(bufLen > 0)
		{
			/* 解析网络协议 */
			memcpy(&p_len, ptr, sizeof(uint32_t));
			if(p_len < 3*sizeof(uint32_t)) 
				goto error;
			if(p_len <= bufLen)
			{
				bufLen -= sizeof(uint32_t);
				ptr += sizeof(uint32_t);
				memcpy(&cmd, ptr, sizeof(uint32_t));
				ptr += sizeof(uint32_t);
				memcpy(&m_len, ptr, sizeof(uint32_t));
				if(m_len < sizeof(uint32_t)*2) 
					goto error;
				ptr += sizeof(uint32_t);

				if(!__on_command(sock, cmd, ptr, m_len - sizeof(uint32_t)*2, param))
					goto error;
				ptr += m_len-sizeof(uint32_t)*2;
				bufLen -= p_len;
			}
			else
			{					
				break;
			}
		}
		if(bufLen >0)
			BUF_WRITE(buf, ptr, bufLen);
	}	
	goto ok;
error:
	serv = serv_list_get(sock);
	if(serv != NULL) close_connection(sock);
ok:
	return;
}

bool listen_init()
{
	uint16_t port = (uint16_t)cfg_get_int(CONFIG_FILE, "Config","Port");
	g_listen.conn_list = jam_list_alloc();
	g_listen.fd_buf = jam_arrayset_alloc();
	g_listen.listen = jam_server_tcp(port, 0, __on_accept, __on_lost, __on_data);

	jam_lg(LG_SYS, "init listen moudle, port = %u.", port);
	return !!g_listen.listen;
}


bool listen_cleanup()
{
	uint32_t fd;
	int32_t times = 10;

	jam_socket_close(g_listen.listen);

	/* 关闭所有网络连接 */
	if(jam_list_cursor_first(g_listen.conn_list))
	{
		socket_t fd;
		do
		{
			if(fd = (socket_t)jam_list_cursor_data(g_listen.conn_list))
			{
				jam_socket_close(fd);
			}
		}while(jam_list_cursor_next(g_listen.conn_list));
	}
	/* 等待所有连接都安全关闭 */
	while(times-- && jam_list_size(g_listen.conn_list))
	{
		jam_lg(LG_SYS, "waiting for disconnect all connections.");
		jam_sleep(3000);
	}
	/* 删除网络协议缓存 */
	if(jam_arrayset_first(g_listen.fd_buf, &fd))
	{
		byte_t* buf;
		do
		{
			if(buf = (byte_t*)jam_arrayset_get(g_listen.fd_buf, fd))
			{
				jam_free(buf);
			}
		}while(jam_arrayset_first(g_listen.fd_buf, &fd));
	}
	/* 清理句柄 */
	jam_list_free(g_listen.conn_list);
	jam_arrayset_free(g_listen.fd_buf);

	jam_lg(LG_SYS, "cleanup listen moudle");
	return true;
}

bool close_connection(socket_t sock)
{
	serv_list_erase(sock);
	jam_socket_close(sock);
	return true;
}