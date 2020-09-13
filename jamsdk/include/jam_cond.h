
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Conditional Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_COND_H__
#define __JAM_COND_H__

#include "jam_datatype.h"
#include "jam_mutex.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t							cond_t;


	/** 分配信号量 */
	JAM_API cond_t jam_cond_alloc();


	/** 关闭信号量 */
	JAM_API void jam_cond_free(cond_t event);


	/** 等待信号量 */
	JAM_API void jam_cond_wait(cond_t event, mutex_t lock);


	/** 限时等待信号量, 返回: true 有信号量, false 超时 */
	JAM_API bool jam_cond_time_wait(cond_t event, mutex_t lock, uint32_t millisec);


	/** 唤醒一个被锁定的线程 */
	JAM_API void jam_cond_signal(cond_t event);


	/** 唤醒所有被锁定的线程 */
	JAM_API void jam_cond_broadcast(cond_t event);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_COND_H__ */
