
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Sphere Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SPHERE_H__
#define __JAM_SPHERE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 检测一个点是否在球体内 */
	JAM_API bool jam_sphere_point_in(const sphere_t* s, const vec3_t p);


	/** 检测两个球体是否相交 */
	JAM_API bool jam_sphere_intersect(const sphere_t* s1, const sphere_t* s2);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SPHERE_H__ */
