
#ifndef __HEAD_H__
#define __HEAD_H__


#include <jam_sdk.h>

#define LG_SYS						JAM_SRV_LG_SYS
#define LG_INF						"<INF>"
#define LG_WRN						"<WRN>"
#define LG_ERR						"<ERR>"

#define CONFIG_FILE					"antiserv.ini"

#define VERSION_DEFINE			0						/* 版本，0--大陆 1--越南 */


#define NAME_LEN					32					/* 用户名最大长度 */
#define MAX_SID						64					/* 用户最多可登录的服务器数 */
#define BUFFER_SIZE					8192				/* 网络协议解析缓存 */


extern uint32_t OFFLINE_TIMEOUT;				//(120 * 1000)				/* 下线超时检测时间 */
extern uint32_t OFFLINE_CLEAR;				//(5 * 60 * 60 * 1000)		/* 恢复健康游戏所需的时间 */
extern uint32_t GREEN_TIME;					//(3 * 60 * 60 * 1000)		/* 健康游戏的时间 */
extern uint32_t RED_TIME;					//(5 * 60 * 60 * 1000)		/* 非健康游戏的时间 */
extern uint32_t GREEN_NOTIFY_PERIOD;
extern uint32_t ORANGE_NOTIFY_PERIOD;
extern uint32_t REDNOTIFY_PERIOD;
#if (VERSION_DEFINE==1)
extern uint32_t CHECK_POINT_TIME;						/* 重置在线时间的时间点 */
#endif

#define COLOR_GREEN					1					/* 健康游戏时间. 3 小时以内 */
#define COLOR_ORANGE				2					/* 收益减半. 3 - 5 小时之间 */
#define COLOR_RED					3					/* 收益为零. 5 小时以上 */


#define CMD_NC_LOGIN_CM				4226247465			/* 服务器登录协议 */
#define CMD_NC_ANTIENTHRALL			2591108563			/* 在线状态更新协议 */
#define CMD_NC_ANTIQUERYSTATE		3556684230			/* 查询用户状态*/

#define CMD_NS_ANTIENTHRALL			3044263451


#define PTR_TO_ID(p)				((uint32_t)(uint64_t)p)		/* 指针转整型 */
#define ID_TO_PTR(type, id)			((type*)(uint64_t)id)		/* 整型转指针 */
#define MIN(a,b)					((a)>(b)?(b):(a))
#define MAX(a,b)					((a)>(b)?(a):(b))


/* 服务器连接信息 */
typedef struct
{
	int32_t					sid;
	uint32_t				section_id;
	socket_t				fd;
}serv_t;

/* 登录服务器信息 */
typedef struct
{
	int32_t					sid;
	datetime_t				last_time;
}online_serv_info_t;

/* 玩家在线信息 */
typedef struct
{
	char					name[NAME_LEN];			/* 玩家名称 */
	online_serv_info_t		serv[MAX_SID];			/* 已登录的服务器列表 */
	datetime_t				last_time;				/* 最后通信时间 */
	datetime_t				online_time;			/* 上线时间 */
	datetime_t				offline_time;			/* 下线时间 */
	//datetime_t				notify_time;			/* 下一次通知登录服务器的时间 */
	uint32_t				online_period;			/* 在线时间统计*/
	datetime_t				update_time;			/* 最后跟新时间 */
	uint32_t				next_notify;			/* 下次通知时间间隔*/
	int8_t					notify_color;			/* 通知的颜色(1, 2, 3) */
	uint32_t				offline_period;			/* 下线时间*/
}online_info_t;


#endif
