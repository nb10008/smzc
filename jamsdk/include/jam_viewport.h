
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Viewport Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_VIEWPORT_H__
#define __JAM_VIEWPORT_H__

#include "jam_camera.h"
#include "jam_color.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t				viewport_t;


	#define JAM_VP_CLEAR_COLOR							(1 << 0)
	#define JAM_VP_CLEAR_DEPTH							(1 << 1)



	/** 设置视口背景色 */
	JAM_API void jam_vp_set_background_color(viewport_t vp, color_t argb_clr);


	/** 获取视口背景色 */
	JAM_API color_t jam_vp_get_background_color(viewport_t vp);


	/** 设置视口渲染前清屏 */
	JAM_API void jam_vp_set_clear_frame(viewport_t vp, uint32_t cf);


	/** 获取视口清屏方式 */
	JAM_API uint32_t jam_vp_get_clear_frame(viewport_t vp);


	/** 设置视口的摄像机 */
	JAM_API void jam_vp_set_camera(viewport_t vp, camera_t cam);


	/** 获取视口的摄像机 */
	JAM_API camera_t jam_vp_get_camera(viewport_t vp);


	/** 获取视口的 Z 排序 */
	JAM_API int32_t jam_vp_z_order(viewport_t vp);


	/** 获取视口左边坐标 (0.0f - 1.0f) */
	JAM_API float jam_vp_left(viewport_t vp);


	/** 获取视口顶部坐标 (0.0f - 1.0f) */
	JAM_API float jam_vp_top(viewport_t vp);


	/** 获取视口宽度 (0.0f - 1.0f) */
	JAM_API float jam_vp_width(viewport_t vp);


	/** 获取视口高度 (0.0f - 1.0f) */
	JAM_API float jam_vp_height(viewport_t vp);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_VIEWPORT_H__ */
