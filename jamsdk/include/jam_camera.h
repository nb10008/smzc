
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Camera Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CAMERA_H__
#define __JAM_CAMERA_H__

#include "jam_vector.h"
#include "jam_quat.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_CAM_PERSPECTIVE,
		JAM_CAM_ORTHO
	}cam_type_t;

	typedef handle_t				camera_t;


	/** 分配一个摄像机 */
	JAM_API camera_t jam_cam_alloc();


	/** 释放摄像机资源 */
	JAM_API void jam_cam_free(camera_t cam);


	/** 设置摄像机位置 */
	JAM_API void jam_cam_set_position(camera_t cam, const vec3_t pos);


	/** 获取摄像机位置 */
	JAM_API void jam_cam_get_position(camera_t cam, vec3_t out);


	/** 设置摄像机方向 */
	JAM_API void jam_cam_set_direction(camera_t cam, const vec3_t dir);


	/** 获取摄像机方向 */
	JAM_API void jam_cam_get_direction(camera_t cam, vec3_t out);


	/** 移动摄像机 */
	JAM_API void jam_cam_move(camera_t cam, const vec3_t pos);


	/** 自动生成摄像机方向 */
	JAM_API void jam_cam_lookat(camera_t cam, const vec3_t target);


	/** Z 轴旋转 */
	JAM_API void jam_cam_roll(camera_t cam, float angle);


	/** Y 轴旋转 */
	JAM_API void jam_cam_yaw(camera_t cam, float angle);


	/** X 轴旋转 */
	JAM_API void jam_cam_pitch(camera_t cam, float angle);


	/** 旋转 */
	JAM_API void jam_cam_rotate(camera_t cam, const quat_t q);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CAMERA_H__ */
