
#include "event.h"
#include "serv_list.h"
#include "user_list.h"


static event_t			g_event = 0;


bool event_init()
{
	g_event = jam_event_alloc();
	return !!g_event;
}


void event_cleanup()
{
	jam_event_free(g_event);
}


void event_timer_add(uint32_t id, uint32_t timeout, event_proc_t do_event, void* param)
{
	jam_event_timer_add(g_event, id, timeout, do_event, param);
}


void event_timer_remove(uint32_t id)
{
	jam_event_timer_remove(g_event, id);
}


int32_t validate_conn(uint32_t identify, void* param)
{
	socket_t fd = (socket_t)param;
	serv_t* s = serv_list_get(fd);	/* 查询服务器连接队列 */
	if(s)
	{
		/* 已登录 */
		event_timer_remove(PTR_TO_ID(fd));
	}
	else
	{
		/* 未登录，关闭套接字 */
		char ip[JAM_MAX_PATH] = {0};

		jam_ip_to_host(ip, jam_socket_addr(fd));
		jam_socket_close(fd);

		jam_lg(LG_INF, "validate timeout:%s.", ip);
	}
	return 0;
}

int32_t loop_user(uint32_t identify, void* param)
{
	online_info_t* oi = (online_info_t*)param;
	datetime_t now = jam_time_now();

// 	printf("loop user %s\r\n", oi->name);
	if(!user_standby(oi))		/* 玩家非待机状态的话... */
	{
		uint64_t online_period;
#if (VERSION_DEFINE==1)
		/* 取当前时间，如果为零点，更新为健康时间 */
		int32_t now_hour = jam_time_hour(now);
		int32_t now_minute = jam_time_minute(now);
#endif
		if(oi->update_time)
			online_period = jam_time_diff(now, oi->update_time);
		else
			online_period = 0;

		if(user_is_online(oi))	/* 玩家如果在线的话... */
		{
			uint64_t time_diff;
			int32_t i;
			
// 			printf("loop user %s is online\r\n", oi->name);
#if (VERSION_DEFINE==1)
			if (now_hour == CHECK_POINT_TIME && now_minute == 0)
			{
				goto reset_online_period;
			}
#endif
// 			printf("loop user %s start to check online server list\r\n", oi->name);

			/* 检测玩家是否在部分登录服务器下线 */
			for(i = 0; i < MAX_SID; ++i)
			{
				if(oi->serv[i].sid)
				{
					if((!serv_list_get_by_sid(oi->serv[i].sid)) || (jam_time_diff(now, oi->serv[i].last_time) >= OFFLINE_TIMEOUT))
					{
						oi->serv[i].sid = 0;
						oi->serv[i].last_time = 0;
// 						printf("user offline time too long, set to offline %s %d\r\n", oi->name, oi->serv[i].sid);
					}
					else
					{
						oi->online_period += online_period;
					}
				}
			}
			if(!user_check_offline(oi))		/* 检测玩家是否在所有登录服务器都下线 */
			{			
				if(oi->online_period < GREEN_TIME)
				{
					/* 在线时间小于 3 小时, 1 小时通知一次"绿色"时间 */
					oi->notify_color = COLOR_GREEN;
				}
				else if(oi->online_period < RED_TIME)
				{
					/* 在线时间在 3 - 5 小时之间, 30 分钟通知一次"橙色"时间 */
					oi->notify_color = COLOR_ORANGE;
				}
				else
				{
					/* 在线时间超过 5 小时, 15 分钟通知一次"红色"时间 */
					oi->notify_color = COLOR_RED;
				}

				if(oi->next_notify && oi->next_notify <= oi->online_period)
				{
					uint32_t plen = 29, mlen = 21;
					uint32_t cmd = CMD_NS_ANTIENTHRALL;
					byte_t buf[33] = {0};
					serv_t* s;
					uint32_t online_time = oi->online_period/1000;

					memcpy(buf, &plen, sizeof(uint32_t));
					memcpy(buf + 4, &cmd, sizeof(uint32_t));
					memcpy(buf + 8, &mlen, sizeof(uint32_t));
					strncpy(buf + 12, oi->name, 16);
					memcpy(buf + 28, &oi->notify_color, sizeof(int8_t));
					memcpy(buf + 29, &online_time, sizeof(uint32_t));

					/* 通知所有登录服务器，玩家在线情况 */
					for(i = 0; i < MAX_SID; ++i)
					{
						if(oi->serv[i].sid && (s = serv_list_get_by_sid(oi->serv[i].sid)))
						{							
							jam_socket_send(s->fd, buf, plen+4, 0);
						}
					}		
					//jam_lg(LG_INF, "send user state, username(%s) state(%u) onlinetime(%u).", oi->name, oi->notify_color, oi->online_period/1000);
				}	

				/* 开始计算下一次通知的时间 */
				if(!oi->next_notify || oi->next_notify <= oi->online_period)
				{
					if(oi->online_period < GREEN_TIME)
					{
						/* 在线时间小于 3 小时, 1 小时通知一次"绿色"时间 */
						oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(GREEN_NOTIFY_PERIOD))) * GREEN_NOTIFY_PERIOD*1000 ;
						oi->notify_color = COLOR_GREEN;
					}
					else if(oi->online_period < RED_TIME)
					{
						/* 在线时间在 3 - 5 小时之间, 30 分钟通知一次"橙色"时间 */
						oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(ORANGE_NOTIFY_PERIOD))) * ORANGE_NOTIFY_PERIOD*1000 ;
						oi->notify_color = COLOR_ORANGE;
					}
					else
					{
						/* 在线时间超过 5 小时, 15 分钟通知一次"红色"时间 */
						oi->next_notify = ((uint32_t)(1+((uint32_t)(oi->online_period/1000))/(REDNOTIFY_PERIOD))) * REDNOTIFY_PERIOD*1000 ;
						oi->notify_color = COLOR_RED;
					}					
				}
			}			
			oi->update_time = now;
		}
		else
		{
#if (VERSION_DEFINE==1)
reset_online_period:
#endif
			if(oi->online_period>0)  //add offline time only when user is online
			{
#if (VERSION_DEFINE==0)
				oi->offline_period += online_period;
				if(oi->offline_period >= OFFLINE_CLEAR)
#elif (VERSION_DEFINE==1)
				if (now_hour == CHECK_POINT_TIME && now_minute == 0)
#endif
				{
					/* 下线时间累计超过5小时的玩家，更新为健康时间 */
					/* 玩家进入待机状态 */
					oi->online_time = 0;
					oi->offline_time = 0;
					//oi->notify_time = 0;
					oi->last_time = 0;
					oi->online_period = 0;
					oi->next_notify = 0;
					oi->update_time = 0;
					oi->notify_color = 1;
					oi->offline_period = 0;
				}
				else
				{
					oi->update_time = now;
				}
			}
		}
		
	}

	return 0;
}


