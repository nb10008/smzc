
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			B+ Tree Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_BTREEDB_H__
#define __JAM_BTREEDB_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_BDB_OVER = 1,						/** 覆盖数据库记录 */
		JAM_BDB_KEEP,							/** 保持原数据库记录 */
		JAM_BDB_CAT,							/** 追加写入数据库记录末尾 */
		JAM_BDB_DUP,							/** 允许重复关键字 */
		JAM_BDB_DUPR							/** 允许重复关键字并倒序排列 */
	}bdb_mode_t;

	typedef enum
	{
		JAM_BDB_CURRENT = 1,					/** 覆盖当前记录 */
		JAM_BDB_BEFORE,							/** 在当前记录之前插入一个记录 */
		JAM_BDB_AFTER							/** 在当前记录之后插入一个记录 */
	}bdb_cp_mode_t;

	typedef enum
	{
		JAM_BDB_STRING = 1,						/** 记录集按字符串排序 */
		JAM_BDB_NUMBER,							/** 记录集按数字排序 */
		JAM_BDB_BINARY							/** 记录集按二进制排序 */
	}bdb_sort_type_t;


	typedef handle_t							btreedb_t;


	/** 分配一个数据库句柄 */
	JAM_API btreedb_t jam_bdb_alloc(const char* filename, bdb_sort_type_t sort);


	/** 关闭一个数据库句柄 */
	JAM_API void jam_bdb_free(btreedb_t db);


	/** 设置一个数据 */
	JAM_API bool jam_bdb_put(btreedb_t db, const byte_t* key, size_t ksize, const byte_t* data, size_t dsize, bdb_mode_t mode);


	/** 获取一个数据 */
	JAM_API size_t jam_bdb_get(btreedb_t db, byte_t* data, size_t dsize, const byte_t* key, size_t ksize);


	/** 删除一个数据 */
	JAM_API bool jam_bdb_erase(btreedb_t db, const byte_t* key, size_t ksize);


	/** 获取数据大小 */
	JAM_API size_t jam_bdb_data_size(btreedb_t db, const byte_t* key, size_t ksize);


	/** 数据库中数据个数 */
	JAM_API size_t jam_bdb_count(btreedb_t db);


	/** 修改当前记录 */
	JAM_API bool jam_bdb_cursor_put(btreedb_t db, const byte_t* data, size_t dsize, bdb_cp_mode_t mode);


	/** 查找定位记录 */
	JAM_API bool jam_bdb_cursor_find(btreedb_t db, const byte_t* key, size_t size);


	/** 第一条记录 */
	JAM_API bool jam_bdb_cursor_first(btreedb_t db);


	/** 最后一条记录 */
	JAM_API bool jam_bdb_cursor_last(btreedb_t db);


	/** 下一条记录 */
	JAM_API bool jam_bdb_cursor_next(btreedb_t db);


	/** 上一条记录 */
	JAM_API bool jam_bdb_cursor_prev(btreedb_t db);


	/** 删除当前记录 */
	JAM_API bool jam_bdb_cursor_erase(btreedb_t db);


	/** 记录关键字长度 */
	JAM_API size_t jam_bdb_cursor_key_size(btreedb_t db);


	/** 记录数据长度 */
	JAM_API size_t jam_bdb_cursor_data_size(btreedb_t db);


	/** 记录关键字 */
	JAM_API size_t jam_bdb_cursor_key(btreedb_t db, byte_t* key, size_t size);


	/** 记录数据 */
	JAM_API size_t jam_bdb_cursor_data(btreedb_t db, byte_t* data, size_t size);


	/** 开始事务 */
	JAM_API bool jam_bdb_trans_begin(btreedb_t db);


	/** 提交事务 */
	JAM_API bool jam_bdb_trans_commit(btreedb_t db);


	/** 回滚事务 */
	JAM_API bool jam_bdb_trans_abort(btreedb_t db);


	/** 刷新记录集 */
	JAM_API void jam_bdb_flush(btreedb_t db);


	/** 优化收缩文件数据库 */
	JAM_API bool jam_bdb_optimize(btreedb_t db);


	/** 修复文件数据库 */
	JAM_API bool jam_bdb_repair(const char* filename, bdb_sort_type_t sort);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_BTREEDB_H__ */
