
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Ray Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_RAY_H__
#define __JAM_RAY_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 检测射线与点相交, distance 返回交点到射线起点的距离 */
	JAM_API bool jam_intersect_ray_point(float* distance, const ray_t* r, const vec3_t p);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_RAY_H__ */
