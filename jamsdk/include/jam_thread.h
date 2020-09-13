
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Thread Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_THREAD_H__
#define __JAM_THREAD_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t							thread_t;
	typedef handle_t							threadpool_t;

	typedef void (*task_t)(void* param);


	/** 分配线程池 */
	JAM_API threadpool_t jam_thread_alloc(uint32_t max_thread);


	/** 释放线程池, 会等待线程全部安全退出 */
	JAM_API void jam_thread_free(threadpool_t tp);


	/** 让线程池执行一个任务, 如果tp指针为0, 则自动创建一个线程 */
	JAM_API void jam_thread_exec(threadpool_t tp, task_t task, void* param);


	/** 获取线程池最大线程数 */
	JAM_API size_t jam_thread_max_count(threadpool_t tp);


	/** 线程池当前线程数 */
	JAM_API size_t jam_thread_count(threadpool_t tp);


	/** 获取线程编号 */
	JAM_API thread_t jam_thread_self();


	/** 让出CPU */
	JAM_API void jam_thread_yield();


	/** 释放CPU资源 */
	JAM_API void jam_thread_sleep(uint32_t millisec);


	#define jam_yield				jam_thread_yield
	#define jam_sleep				jam_thread_sleep


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_THREAD_H__ */
