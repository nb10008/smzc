
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Plane Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_PLANE_H__
#define __JAM_PLANE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_SIDE_NO, 
		JAM_SIDE_FRONT, 
		JAM_SIDE_BACK, 
		JAM_SIDE_BOTH
	}side_type_t;


	/** 通过三个点生成一个平面 */
	JAM_API void jam_plane_from_point(plane_t* plane, const vec3_t p1, const vec3_t p2, const vec3_t p3);


	/** 通过一个法线和一个点生成一个平面 */
	JAM_API void jam_plane_from_normal(plane_t* plane, const vec3_t norm, const vec3_t p);


	/** 平面归一化 */
	JAM_API void jam_plane_normalize(plane_t* out, const plane_t* p);


	/** 平面到点的距离 */
	JAM_API float jam_plane_distance(const plane_t* plane, const vec3_t p);


	/** 检测点在平面的位置 */
	JAM_API side_type_t jam_plane_side_point(const plane_t* plane, const vec3_t p);


	/** 检测盒子在平面的位置 */
	JAM_API side_type_t jam_plane_side_box(const plane_t* plane, const box_t* b);


	/** 检测球体在平面的位置 */
	JAM_API side_type_t jam_plane_side_sphere(const plane_t* plane, const sphere_t* s);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_PLANE_H__ */
