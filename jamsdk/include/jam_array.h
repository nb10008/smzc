
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Array List Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_ARRAY_H__
#define __JAM_ARRAY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				array_t;


	/** 分配数组列表对象 */
	JAM_API array_t jam_array_alloc();


	/** 释放数组列表对象 */
	JAM_API void jam_array_free(array_t array);


	/** 获取数组大小 */
	JAM_API size_t jam_array_size(array_t array);


	/** 插入一个数据在数组末尾 */
	JAM_API bool jam_array_push(array_t array, void* data);


	/** 插入一个数据到数组指定位置 */
	JAM_API bool jam_array_put(array_t array, int32_t index, void* data);


	/** 修改指定位置上的数据 */
	JAM_API bool jam_array_set(array_t array, int32_t index, void* data);


	/** 获取指定位置上的数据 */
	JAM_API void* jam_array_get(array_t array, int32_t index);


	/** 删除一个数据 */
	JAM_API bool jam_array_erase(array_t array, int32_t index);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_ARRAY_H__ */
