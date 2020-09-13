
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			OCTree Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_OCTREE_H__
#define __JAM_OCTREE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				octree_t;


	/** 分配一个八叉树 */
	JAM_API octree_t jam_octree_alloc(float max_x, float max_y, float max_z);


	/** 释放一个八叉树 */
	JAM_API void jam_octree_free(octree_t oct);


	/** 以球体的方式进行剔除 */
	JAM_API size_t jam_octree_cull_sphere(octree_t oct, uint64_t* list, size_t len, const sphere_t* s);


	/** 以包围盒的方式进行剔除 */
	JAM_API size_t jam_octree_cull_box(octree_t oct, uint64_t* list, size_t len, const box_t* b);


	/** 以平截椎体的方式进行剔除 */
	JAM_API size_t jam_octree_cull_frustum(octree_t oct, uint64_t* list, size_t len, const frustum_t* f);


	/** 往八叉树中放入一个对象 */
	JAM_API uint64_t jam_octree_obj_put(octree_t oct, float x, float y, float z, void* obj);


	/** 通过树对象编号获取对象 */
	JAM_API void* jam_octree_obj_get(octree_t oct, uint64_t id);


	/** 通过树对象编号删除对象 */
	JAM_API bool jam_octree_obj_erase(octree_t oct, uint64_t id);


	/** 移动一个对象 */
	JAM_API bool jam_octree_obj_move(octree_t oct, uint64_t id, float x, float y, float z);


	/** 获取对象 x 坐标 */
	JAM_API float jam_octree_obj_x(octree_t oct, uint64_t id);


	/** 获取对象 y 坐标 */
	JAM_API float jam_octree_obj_y(octree_t oct, uint64_t id);


	/** 获取对象 z 坐标 */
	JAM_API float jam_octree_obj_z(octree_t oct, uint64_t id);


	/** 获取八叉树对象总数 */
	JAM_API size_t jam_octree_obj_count(octree_t oct);


	/** 获取八叉树节点总数 */
	JAM_API size_t jam_octree_node_count(octree_t oct);


	/** 获取八叉树当前深度 */
	JAM_API uint16_t jam_octree_depth(octree_t oct);


	/** 获取八叉树最大 x 坐标 */
	JAM_API float jam_octree_max_x(octree_t oct);


	/** 获取八叉树最大 y 坐标 */
	JAM_API float jam_octree_max_y(octree_t oct);


	/** 获取八叉树最大 z 坐标 */
	JAM_API float jam_octree_max_z(octree_t oct);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_OCTREE_H__ */
