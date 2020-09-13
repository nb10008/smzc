
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Frustum Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_FRUSTUM_H__
#define __JAM_FRUSTUM_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
    {
        JAM_FP_NEAR   = 0,
        JAM_FP_FAR    = 1,
        JAM_FP_LEFT   = 2,
        JAM_FP_RIGHT  = 3,
        JAM_FP_TOP    = 4,
        JAM_FP_BOTTOM = 5
    }frustum_plane_t;


	/** 通过矩阵获取平截椎体 */
	JAM_API void jam_frustum_from_matrix(frustum_t* f, const mat44_t proj, const mat44_t view);


	/** 检测一个点是否在平截椎体内 */
	JAM_API bool jam_frustum_point_in(const frustum_t* f, const vec3_t p);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_FRUSTUM_H__ */
