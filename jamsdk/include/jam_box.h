
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Box Function
 *
 *			   1---------2
 *			  /|        /|
 *			 / |       / |
 *			5---------4  |
 *			|  0------|--3
 *			| /       | /
 *			|/        |/
 *			6---------7
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_BOX_H__
#define __JAM_BOX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_BV_FAR_BOTTOM_LEFT,
		JAM_BV_FAR_TOP_LEFT,
		JAM_BV_FAR_TOP_RIGHT,
		JAM_BV_FAR_BOTTOM_RIGHT,
		JAM_BV_NEAR_TOP_RIGHT,
		JAM_BV_NEAR_TOP_LEFT,
		JAM_BV_NEAR_BOTTOM_LEFT,
		JAM_BV_NEAR_BOTTOM_RIGHT,
	}box_vertex_t;


	/** 检测一个点是否在盒子内 */
	JAM_API bool jam_box_point_in(const box_t* b, const vec3_t p);


	/** 检测两个盒子是否相交 */
	JAM_API bool jam_box_intersect(const box_t* b1, const box_t* b2);


	/** 获取盒子中心点 */
	JAM_API void jam_box_center(vec3_t p, const box_t* b);


	/** 获取盒子一半尺寸 */
	JAM_API void jam_box_half_size(vec3_t p, const box_t* b);


	/** 获取盒子八个顶点 */
	JAM_API void jam_box_vertex(vec3_t v[8], const box_t* b);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_BOX_H__ */
