
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Timer Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_TIMER_H__
#define __JAM_TIMER_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t							timer_t;


	/** 分配一个计时器句柄 */
	JAM_API timer_t jam_timer_alloc();


	/** 关闭计时器句柄 */
	JAM_API void jam_timer_free(timer_t timer);


	/** 刷新计时器 */
	JAM_API void jam_timer_refresh(timer_t timer);


	/** 获取毫秒数 */
	JAM_API uint32_t jam_timer_millisec(timer_t timer);


	/** 获取微妙数 */
	JAM_API uint32_t jam_timer_microsec(timer_t timer);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_TIMER_H__ */
