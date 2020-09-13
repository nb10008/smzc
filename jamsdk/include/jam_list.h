
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			List Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LIST_H__
#define __JAM_LIST_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				list_t;


	/** 分配列表句柄 */
	JAM_API list_t jam_list_alloc();


	/** 释放列表资源 */
	JAM_API void jam_list_free(list_t l);


	/** 获取列表元素个数 */
	JAM_API size_t jam_list_size(list_t l);


	/** 游标移到列表头部 */
	JAM_API bool jam_list_cursor_first(list_t l);


	/** 游标移到列表尾部 */
	JAM_API bool jam_list_cursor_last(list_t l);


	/** 游标向后移一个 */
	JAM_API bool jam_list_cursor_next(list_t l);


	/** 游标向前移一个 */
	JAM_API bool jam_list_cursor_prev(list_t l);


	/** 当前游标的用户数据 */
	JAM_API void* jam_list_cursor_data(list_t l);


	/** 当前游标插入数据 */
	JAM_API bool jam_list_cursor_insert(list_t l, void* data);


	/** 删除当前游标数据 */
	JAM_API bool jam_list_cursor_erase(list_t l);


	/** 将一个元素加入到列表头部 */
	JAM_API bool jam_list_push_front(list_t l, void* data);


	/** 将一个元素加入到列表尾部 */
	JAM_API bool jam_list_push_back(list_t l, void* data);


	/** 删除列表头部一个元素 */
	JAM_API void* jam_list_pop_front(list_t l);


	/** 删除列表尾部一个元素 */
	JAM_API void* jam_list_pop_back(list_t l);


	/** 获取列表头部元素 */
	JAM_API void* jam_list_front(list_t l);


	/** 获取列表尾部元素 */
	JAM_API void* jam_list_back(list_t l);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LIST_H__ */
