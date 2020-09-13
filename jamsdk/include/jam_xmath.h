
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Graphics Math Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_XMATH_H__
#define __JAM_XMATH_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 检测盒子和球体相交 */
	JAM_API bool jam_intersect_box_sphere(const box_t* b, const sphere_t* s);


	/** 检测平截椎体和盒子相交 */
	JAM_API bool jam_intersect_frustum_box(const frustum_t* f, const box_t* b);


	/** 检测平截椎体和球体相交 */
	JAM_API bool jam_intersect_frustum_sphere(const frustum_t* f, const sphere_t* s);


	/** 检测射线与盒子相交, distance 返回交点到射线起点的距离 */
	JAM_API bool jam_intersect_ray_box(float* distance, const ray_t* r, const box_t* b);


	/** 检测射线与球体相交, distance 返回交点到射线起点的距离 */
	JAM_API bool jam_intersect_ray_sphere(float* distance, const ray_t* r, const sphere_t* s);


	/** 检测射线与面相交, distance 返回交点到射线起点的距离 */
	JAM_API bool jam_intersect_ray_plane(float* distance, const ray_t* r, const plane_t* p);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_XMATH_H__ */
