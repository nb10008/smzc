
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			LUA Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_LUA_H__
#define __JAM_LUA_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct lua_State*					lua_t;
	typedef int (*lua_func_t)(lua_t l);


	typedef enum
	{
		JAM_LUA_FILENAME = 1,
		JAM_LUA_PROGRAM
	}lua_type_t;


	/** 打开一个LUA句柄 */
	JAM_API lua_t jam_lua_alloc();


	/** 释放句柄 */
	JAM_API void jam_lua_free(lua_t l);


	/** 执行脚本(可以从文件或内存) */
	JAM_API bool jam_lua_do(lua_t l, const char* str, lua_type_t type);


	/** 最后的错误信息 */
	JAM_API const char* jam_lua_last_error(lua_t l);


	/** 获取LUA堆栈大小 */
	JAM_API size_t jam_lua_stack_size(lua_t l);


	/** 清空LUA堆栈 */
	JAM_API void jam_lua_stack_zero(lua_t l);


	/** 压入一个数字到栈中 */
	JAM_API void jam_lua_push_number(lua_t l, double value);


	/** 压入一个字符串到栈中 */
	JAM_API void jam_lua_push_string(lua_t l, const char* str);


	/** 压入一个布尔值到栈中 */
	JAM_API void jam_lua_push_boolean(lua_t l, bool value);


	/** 压入一个用户自定义数据到栈中 */
	JAM_API void jam_lua_push_object(lua_t l, void* value);


	/** 压入一个空值到栈中 */
	JAM_API void jam_lua_push_null(lua_t l);


	/** 将栈中指定位置的元素转成数字 */
	JAM_API double jam_lua_to_number(lua_t l, int32_t idx);


	/** 将栈中指定位置的元素转成字符串 */
	JAM_API const char* jam_lua_to_string(lua_t l, int32_t idx);


	/** 将栈中指定位置的元素转成布尔值 */
	JAM_API bool jam_lua_to_boolean(lua_t l, int32_t idx);


	/** 将栈中指定位置的元素转成用户自定义数据 */
	JAM_API void* jam_lua_to_object(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是数字 */
	JAM_API bool jam_lua_is_number(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是字符串 */
	JAM_API bool jam_lua_is_string(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是布尔值 */
	JAM_API bool jam_lua_is_boolean(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是自定义数据 */
	JAM_API bool jam_lua_is_object(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是表 */
	JAM_API bool jam_lua_is_table(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是函数 */
	JAM_API bool jam_lua_is_function(lua_t l, int32_t idx);


	/** 检查栈中指定位置的元素是否是空 */
	JAM_API bool jam_lua_is_null(lua_t l, int32_t idx);


	/** 获取全局元素(变量, 函数等) */
	JAM_API void jam_lua_get_global(lua_t l, const char* name);


	/** 设置全局元素(变量, 函数等) */
	JAM_API void jam_lua_set_global(lua_t l, const char* name);


	/** 注册一个C函数 */
	JAM_API void jam_lua_reg_func(lua_t l, const char* name, lua_func_t func);


	/** 准备调用一个LUA函数 */
	JAM_API bool jam_lua_begin_call(lua_t l, const char* func);


	/** 调用一个LUA函数 */
	JAM_API bool jam_lua_end_call(lua_t l, int32_t param, int32_t ret);


	/** 读取一个数字变量 */
	JAM_API double jam_lua_get_number(lua_t l, const char* name);


	/** 读取一个字符串变量 */
	JAM_API const char* jam_lua_get_string(lua_t l, const char* name);


	/** 读取一个布尔变量 */
	JAM_API bool jam_lua_get_boolean(lua_t l, const char* name);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_LUA_H__ */
