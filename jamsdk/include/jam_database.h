
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Database Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_DATABASE_H__
#define __JAM_DATABASE_H__

#include "jam_hashdb.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				database_t;
	typedef handle_t				db_record_t;


	/** 打开一个记录 */
	JAM_API db_record_t jam_dbr_alloc(const char* name);


	/** 关闭记录 */
	JAM_API void jam_dbr_free(db_record_t dbr);


	/** 获取记录名称 */
	JAM_API const char* jam_dbr_name(db_record_t dbr);


	/** 设置记录属性数据 */
	JAM_API bool jam_dbr_put(db_record_t dbr, const char* attr, const byte_t* data, size_t size);


	/** 获取记录属性数据 */
	JAM_API const byte_t* jam_dbr_get(db_record_t dbr, size_t* size, const char* attr);


	/** 删除记录属性 */
	JAM_API bool jam_dbr_erase(db_record_t dbr, const char* attr);


	/** 删除记录所有属性 */
	JAM_API size_t jam_dbr_clear(db_record_t dbr);


	/** 记录属性数 */
	JAM_API size_t jam_dbr_attr_size(db_record_t dbr);


	/** 记录第一个属性 */
	JAM_API const char* jam_dbr_attr_first(db_record_t dbr);


	/** 记录下一个属性 */
	JAM_API const char* jam_dbr_attr_next(db_record_t dbr);



	/** 分配一个数据库句柄 */
	JAM_API database_t jam_db_alloc(hdb_type_t type, const char* filename);


	/** 关闭一个数据库句柄 */
	JAM_API void jam_db_free(database_t db);


	/** 存入一条记录 */
	JAM_API bool jam_db_put(database_t db, db_record_t dbr);


	/** 存入一个记录属性 */
	JAM_API bool jam_db_put_attr(database_t db, const char* name, const char* attr, const byte_t* data, size_t size);


	/** 获取一个记录 */
	JAM_API db_record_t jam_db_get(database_t db, const char* name);


	/** 获取一个记录属性 */
	JAM_API size_t jam_db_get_attr(database_t db, byte_t* data, size_t size, const char* name, const char* attr);


	/** 删除一个记录 */
	JAM_API bool jam_db_erase(database_t db, const char* name);


	/** 删除一个记录属性 */
	JAM_API bool jam_db_erase_attr(database_t db, const char* name, const char* attr);


	/** 获取文档个数 */
	JAM_API size_t jam_db_count(database_t db);


	/** 获取第一个记录 */
	JAM_API db_record_t jam_db_first(database_t db);


	/** 获取下一个记录 */
	JAM_API db_record_t jam_db_next(database_t db);


	/** 刷新记录集 */
	JAM_API void jam_db_flush(database_t db);


	/** 优化收缩文件数据库 */
	JAM_API bool jam_db_optimize(database_t db);


	/** 修复数据库 */
	JAM_API bool jam_db_repair(const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_DATABASE_H__ */
