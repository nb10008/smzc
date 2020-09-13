
#ifndef __EVENT_H__
#define __EVENT_H__

#include "head.h"


/* 系统事件初始化 */
bool event_init();


/* 系统事件销毁 */
void event_cleanup();


/* 注册循环事件 */
void event_timer_add(uint32_t id, uint32_t timeout, event_proc_t do_event, void* param);


/* 删除循环事件 */
void event_timer_remove(uint32_t id);


/* 连接验证事件 */
int32_t validate_conn(uint32_t identify, void* param);


/* 玩家在线时间检测事件 */
int32_t loop_user(uint32_t identify, void* param);
#endif
