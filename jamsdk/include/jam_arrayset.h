
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Array Set Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ARRAY_SET_H__
#define __JAM_ARRAY_SET_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				arrayset_t;


	/** 分配数组集合对象 */
	JAM_API arrayset_t jam_arrayset_alloc();


	/** 释放数组集合对象 */
	JAM_API void jam_arrayset_free(arrayset_t set);


	/** 获取集合大小 */
	JAM_API size_t jam_arrayset_size(arrayset_t set);


	/** 插入一个数据到集合指定位置 */
	JAM_API bool jam_arrayset_put(arrayset_t set, uint32_t key, void* data);


	/** 获取指定位置上的数据 */
	JAM_API void* jam_arrayset_get(arrayset_t set, uint32_t key);


	/** 删除一个数据 */
	JAM_API bool jam_arrayset_erase(arrayset_t set, uint32_t key);


	/** 第一条记录 */
	JAM_API bool jam_arrayset_first(arrayset_t set, uint32_t* key);


	/** 最后一条记录 */
	JAM_API bool jam_arrayset_last(arrayset_t set, uint32_t* key);


	/** 下一条记录 */
	JAM_API bool jam_arrayset_next(arrayset_t set, uint32_t* key);


	/** 上一条记录 */
	JAM_API bool jam_arrayset_prev(arrayset_t set, uint32_t* key);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ARRAY_SET_H__ */
