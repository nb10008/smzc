
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Hash Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_HASHDB_H__
#define __JAM_HASHDB_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_HDB_FILE = 1,					/** 文件数据库, 最大支持 2 GB */
		JAM_HDB_MEMORY,						/** 内存数据库, 最大支持视内存而定 */
	}hdb_type_t;


	typedef enum
	{
		JAM_HDB_OVER = 1,					/** 覆盖数据库记录 */
		JAM_HDB_KEEP,						/** 保持原数据库记录 */
		JAM_HDB_CAT							/** 追加写入数据库记录末尾 */
	}hdb_mode_t;

	
	typedef handle_t						hashdb_t;


	/** 分配一个数据库句柄 */
	JAM_API hashdb_t jam_hdb_alloc(hdb_type_t type, const char* open_str);


	/** 关闭一个数据库句柄 */
	JAM_API void jam_hdb_free(hashdb_t db);


	/** 数据库类型 */
	JAM_API hdb_type_t jam_hdb_type(hashdb_t db);


	/** 设置一个数据 */
	JAM_API bool jam_hdb_put(hashdb_t db, const byte_t* key, size_t ksize, const byte_t* data, size_t dsize, hdb_mode_t mode);


	/** 获取一个数据 */
	JAM_API size_t jam_hdb_get(hashdb_t db, byte_t* data, size_t dsize, const byte_t* key, size_t ksize, size_t start);


	/** 删除一个数据 */
	JAM_API bool jam_hdb_erase(hashdb_t db, const byte_t* key, size_t ksize);


	/** 获取数据大小 */
	JAM_API size_t jam_hdb_data_size(hashdb_t db, const byte_t* key, size_t ksize);


	/** 数据库中数据个数 */
	JAM_API size_t jam_hdb_count(hashdb_t db);


	/** 第一条记录 */
	JAM_API size_t jam_hdb_first(hashdb_t db, byte_t* key, size_t ksize);


	/** 下一条记录 */
	JAM_API size_t jam_hdb_next(hashdb_t db, byte_t* key, size_t ksize);


	/** 开始事务 */
	JAM_API bool jam_hdb_trans_begin(hashdb_t db);


	/** 提交事务 */
	JAM_API bool jam_hdb_trans_commit(hashdb_t db);


	/** 回滚事务 */
	JAM_API bool jam_hdb_trans_abort(hashdb_t db);


	/** 刷新记录集(文件数据库会将缓存写入磁盘) */
	JAM_API void jam_hdb_flush(hashdb_t db);


	/** 优化收缩文件数据库 */
	JAM_API bool jam_hdb_optimize(hashdb_t db);


	/** 修复文件数据库 */
	JAM_API bool jam_hdb_repair(const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_HASHDB_H__ */
