
#ifndef __USER_LIST_H__
#define __USER_LIST_H__

#include "head.h"


/* 玩家队列初始化 */
bool user_list_init();


/* 玩家队列销毁 */
bool user_list_cleanup();


/* 根据名字查询在线信息 */
online_info_t* user_get(const char* name);


/* 登录服务器更新一个玩家在线信息 */
online_info_t* user_update(const char* name, int32_t sid);


/* 检测玩家下线 */
bool user_check_offline(online_info_t* info);


/* 玩家是否在线状态 */
#define user_is_online(info)				(info->online_time > info->offline_time)


/* 玩家是否待机状态 */
#define user_standby(info)					(!info->online_time && !info->offline_time && !info->last_time)

/*枚举用户*/
typedef void (*eUserFunc)(online_info_t*, void*);
void enum_user(eUserFunc, void*);
#endif
