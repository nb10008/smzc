
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Message Event Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_EVENT_H__
#define __JAM_EVENT_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				event_t;

	typedef int32_t (*event_proc_t)(uint32_t identify, void* param);


	/** 分配消息事件 */
	JAM_API event_t jam_event_alloc();


	/** 释放消息事件 */
	JAM_API void jam_event_free(event_t evt);


	/** 注册一个事件 */
	JAM_API bool jam_event_add(event_t evt, uint32_t msg, event_proc_t do_event);


	/** 注册一个计时器事件(单位: 毫秒) */
	JAM_API void jam_event_timer_add(event_t evt, uint32_t id, uint32_t timeout, event_proc_t do_event, void* param);


	/** 注销一个计时器事件 */
	JAM_API void jam_event_timer_remove(event_t evt, uint32_t id);


	/** 同步触发消息事件 */
	JAM_API int32_t jam_event_send(event_t evt, uint32_t msg, void* param);


	/** 异步触发消息事件 */
	JAM_API void jam_event_post(event_t evt, uint32_t msg, void* param);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_EVENT_H__ */
